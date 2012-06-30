/*
 * libTUIeee1394++: C++ Library for Controlling IIDC 1394-based Digital Cameras
 * Copyright (C) 2003-2006 Toshio UESHIBA
 *   National Institute of Advanced Industrial Science and Technology (AIST)
 *
 * Written by Toshio UESHIBA <t.ueshiba@aist.go.jp>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id: Ieee1394Node.cc,v 1.24 2012-06-30 20:00:26 ueshiba Exp $
 */
#if HAVE_CONFIG_H
#  include <config.h>
#endif
#include "TU/Ieee1394++.h"
#include <unistd.h>
#include <errno.h>
#include <string>
#include <algorithm>
#include <libraw1394/csr.h>
#if defined(USE_VIDEO1394)
#  include <fcntl.h>
#  include <sys/ioctl.h>
#  include <sys/mman.h>
#  if !defined(VIDEO1394_IOC_LISTEN_CHANNEL)
#     define VIDEO1394_IOC_LISTEN_CHANNEL	VIDEO1394_LISTEN_CHANNEL
#     define VIDEO1394_IOC_UNLISTEN_CHANNEL	VIDEO1394_UNLISTEN_CHANNEL
#     define VIDEO1394_IOC_LISTEN_QUEUE_BUFFER	VIDEO1394_LISTEN_QUEUE_BUFFER
#     define VIDEO1394_IOC_LISTEN_WAIT_BUFFER	VIDEO1394_LISTEN_WAIT_BUFFER
#     define VIDEO1394_IOC_TALK_CHANNEL		VIDEO1394_TALK_CHANNEL
#     define VIDEO1394_IOC_UNTALK_CHANNEL	VIDEO1394_UNTALK_CHANNEL
#     define VIDEO1394_IOC_TALK_QUEUE_BUFFER	VIDEO1394_TALK_QUEUE_BUFFER
#     define VIDEO1394_IOC_TALK_WAIT_BUFFER	VIDEO1394_TALK_WAIT_BUFFER
#     define VIDEO1394_IOC_LISTEN_POLL_BUFFER	VIDEO1394_LISTEN_POLL_BUFFER
#  endif
#else
#  include <sys/time.h>
#endif

namespace TU
{
/************************************************************************
*  static functions							*
************************************************************************/
#if defined(USE_VIDEO1394)
static int
video1394_get_fd_and_check(int port_number)
{
    using namespace	std;

    char	dev[] = "/dev/video1394/x";
    dev[strlen(dev)-1] = '0' + port_number;
    int		fd = open(dev, O_RDWR);
    if (fd < 0)
	throw runtime_error(string("TU::raw1394_get_fd_and_check: failed to open video1394!! ") + strerror(errno));
    return fd;
}
#endif
static inline u_int32_t
cycletime_to_subcycle(u_int32_t cycletime)
{
    u_int32_t	sec	 = (cycletime & 0xe000000) >> 25;
    u_int32_t	cycle	 = (cycletime & 0x1fff000) >> 12;
    u_int32_t	subcycle = (cycletime & 0x0000fff);

    return subcycle + 3072*(cycle + 8000*sec);
}

#if defined(DEBUG)
static std::ostream&
print_time(std::ostream& out, u_int64_t localtime)
{
    u_int32_t	usec = localtime % 1000;
    u_int32_t	msec = (localtime / 1000) % 1000;
    u_int32_t	sec  = localtime / 1000000;
    return out << sec << '.' << msec << '.' << usec;
}
#endif
    
#if !defined(__APPLE__)
/************************************************************************
*  class Ieee1394Node::Port						*
************************************************************************/
Ieee1394Node::Port::Port(int portNumber)
    :
#  if defined(USE_VIDEO1394)
     _fd(video1394_get_fd_and_check(portNumber)),
#  endif
     _portNumber(portNumber), _nodes(0)
{
}

Ieee1394Node::Port::~Port()
{
#  if defined(USE_VIDEO1394)
    close(_fd);
#  endif
}

u_char
Ieee1394Node::Port::registerNode(const Ieee1394Node& node)
{
  // Count the number of nodes already registered.
    u_char	nnodes = 0;
    for (int i = 0; i < 8*sizeof(_nodes); ++i)
	if ((_nodes & (0x1ULL << i)) != 0)
	    ++nnodes;

    _nodes |= (0x1ULL << (node.nodeId() & 0x3f));	// Register this node.

    return nnodes;
}

bool
Ieee1394Node::Port::unregisterNode(const Ieee1394Node& node)
{
    return (_nodes &= ~(0x1ULL << (node.nodeId() & 0x3f))) != 0;
}

bool
Ieee1394Node::Port::isRegisteredNode(const Ieee1394Node& node) const
{
    return (_nodes & (0x1ULL << (node.nodeId() & 0x3f))) != 0;
}

/************************************************************************
*  class Ieee1394Node							*
************************************************************************/
std::map<int, Ieee1394Node::Port*>	Ieee1394Node::_portMap;
#endif	// !__APPLE__

//! IEEE1394$B%N!<%I%*%V%8%'%/%H$r@8@.$9$k(B
/*!
  \param unit_spec_ID	$B$3$N%N!<%I$N<oN`$r<($9(BID(ex. IEEE1394$B%G%8%?%k%+%a%i(B
			$B$G$"$l$P(B, 0x00a02d)
  \param uniqId		$B8D!9$N5!4o8GM-$N(B64bit ID. $BF10l$N(BIEEE1394 bus$B$K(B
			$BF10l$N(Bunit_spec_ID$B$r;}$DJ#?t$N5!4o$,@\B3$5$l$F(B
			$B$$$k>l9g(B, $B$3$l$K$h$C$FF1Dj$r9T$&(B. 
			0$B$,M?$($i$l$k$H(B, $B;XDj$5$l$?(Bunit_spec_ID$B$r;}$A(B
			$B$^$@(B#Ieee1394Node$B%*%V%8%'%/%H$r3d$jEv$F$i$l$F(B
			$B$$$J$$5!4o$N$&$A(B, $B0lHV:G=i$K$_$D$+$C$?$b$N$,$3$N(B
			$B%*%V%8%'%/%H$H7k$S$D$1$i$l$k(B. 
  \param delay		IEEE1394$B%+!<%I$N<oN`$K$h$C$F$O(B, $B%l%8%9%?$NFI$_=q$-(B
			(Ieee1394Node::readQuadlet(),
			Ieee1394Node::writeQuadlet())$B;~$KCY1d$rF~$l$J$$$H(B
			$BF0:n$7$J$$$3$H$,$"$k(B. $B$3$NCY1dNL$r(Bmicro second$BC10L(B
			$B$G;XDj$9$k(B. ($BNc(B: $B%a%k%3$N(BIFC-ILP3$B$G$O(B1, DragonFly
			$BIUB0$N%\!<%I$G$O(B0)
  \param sync_tag	1$B$^$H$^$j$N%G!<%?$rJ#?t$N%Q%1%C%H$KJ,3d$7$F(B
			isochronous$B%b!<%I$G<u?.$9$k:]$K(B, $B:G=i$N%Q%1%C%H$K(B
			$BF14|MQ$N(Btag$B$,$D$$$F$$$k>l9g$O(B1$B$r;XDj(B. $B$=$&$G$J$1$l(B
			$B$P(B0$B$r;XDj(B. 
  \param flags		video1394$B$N%U%i%0(B. VIDEO1394_SYNC_FRAMES, 
			VIDEO1394_INCLUDE_ISO_HEADERS,
			VIDEO1394_VARIABLE_PACKET_SIZE$B$NAH9g$o$;(B. 
*/
Ieee1394Node::Ieee1394Node(u_int unit_spec_ID, u_int64_t uniqId, u_int delay
#if defined(USE_VIDEO1394)
			   , int sync_tag, int flags
#endif
			   )
#if defined(__APPLE__)
    :_handle(raw1394_new_handle(unit_spec_ID, uniqId)), 
#else
    :_port(0), _handle(raw1394_new_handle()),
#endif
     _nodeId(0), _delay(delay), _buf(0), _arrivaltime(0),
#if defined(USE_VIDEO1394)
     _mmap(), _current(0), _buf_size(0)
#else
     _channel(0), _mid(0), _end(0), _current(0), _arrivaltime_next(0)
#endif

{
    using namespace	std;

  // Check whether the handle is valid.
    if (_handle == NULL)
	throw runtime_error(string("TU::Ieee1394Node::Ieee1394Node: failed to get raw1394handle!! ") + strerror(errno));
#if !defined(__APPLE__)
  // Get the number of ports.
    const int	nports = raw1394_get_port_info(_handle, NULL, 0);
    if (nports < 0)
	throw runtime_error(string("TU::Ieee1394Node::Ieee1394Node: failed to get port info!! ") + strerror(errno));
    raw1394_destroy_handle(_handle);

  // Find the specified node yet registered.
    for (int i = 0; i < nports; ++i)		// for each port...
    {
      // Has the i-th port already been created?
	map<int, Port*>::const_iterator	p = _portMap.find(i);
	_port = (p == _portMap.end() ? 0 : p->second);
	
	if ((_handle = raw1394_new_handle_on_port(i)) == NULL)
	    throw runtime_error(string("TU::Ieee1394Node::Ieee1394Node: failed to get raw1394handle and set it to the port!! ") + strerror(errno));
	nodeid_t	localId = raw1394_get_local_id(_handle);
	const int	nnodes  = raw1394_get_nodecount(_handle);
	for (int j = 0; j < nnodes; ++j)	// for each node....
	{
	    _nodeId = (j | 0xffc0);

	    try
	    {
		if (_nodeId != localId				     &&
		    readValueFromUnitDirectory(0x12) == unit_spec_ID &&
		    (uniqId == 0 || globalUniqueId() == uniqId))
		{
		    if (_port == 0)		// If i-th port is not present,
		    {				//
			_port = new Port(i);	// create
			_portMap[i] = _port;	// and insert it to the map.
			goto ok;
		    }
		    else if (!_port->isRegisteredNode(*this))
			goto ok;
		}
	    }
	    catch (exception& err)
	    {
	    }
	}
	raw1394_destroy_handle(_handle);
    }
    throw runtime_error("TU::Ieee1394Node::Ieee1394Node: node with specified unit_spec_ID (and global_unique_ID) not found!!");

  ok:
#  if defined(USE_VIDEO1394)
    _mmap.channel     = _port->registerNode(*this);
    _mmap.sync_tag    = sync_tag;
    _mmap.nb_buffers  = 0;
    _mmap.buf_size    = 0;
    _mmap.packet_size = 0;
    _mmap.fps	      = 0;
    _mmap.flags	      = flags;
#  else
    _channel = _port->registerNode(*this);  // Register this node to the port.
#  endif
#endif	// !__APPLE__
    raw1394_set_userdata(_handle, this);
}
	     
//! IEEE1394$B%N!<%I%*%V%8%'%/%H$rGK2u$9$k(B
Ieee1394Node::~Ieee1394Node()
{
    unmapListenBuffer();
#if !defined(__APPLE__)
    if (!_port->unregisterNode(*this))		// If no nodes on this port,
    {						//
	_portMap.erase(_port->portNumber());	// erase it from the map
	delete _port;				// and delete.
    }
#endif
    raw1394_destroy_handle(_handle);
}

#if defined(__APPLE__)
nodeaddr_t
Ieee1394Node::cmdRegBase() const
{
    return raw1394_command_register_base(_handle);
}
#endif

//! $B$3$N%N!<%I$K7k$SIU$1$i$l$F$$$k5!4o8GM-$N(B64bit ID$B$rJV$9(B
u_int64_t
Ieee1394Node::globalUniqueId() const
{
    u_int64_t	hi = readQuadletFromConfigROM(0x0c),
		lo = readQuadletFromConfigROM(0x10);
    return (hi << 32) | lo;
}

//! $BM?$($i$l$?(Bkey$B$KBP$9$kCM$r(BUnit Dependent Directory$B$+$iFI$_=P$9(B
/*!
  \param key	key$B$9$J$o$A(B4byte$B$NJB$S$N(BMSB$BB&(B8bit
 */
u_int
Ieee1394Node::readValueFromUnitDependentDirectory(u_char key) const
{
  // Read length of Bus Info Block and skip it.
    u_int	offset = 0;
    quadlet_t	quad = readQuadletFromConfigROM(offset);
    offset += 4 * (1 + (quad >> 24));

  // Read unit_directory_offset.
    u_int	tmpOffset = readValueFromDirectory(0xd1, offset);
    offset += 4 * tmpOffset;

  // Read unit_dependent_directory_offset.
    tmpOffset = readValueFromDirectory(0xd4, offset);
    offset += 4 * tmpOffset;

    return readValueFromDirectory(key, offset);
}

//! $BM?$($i$l$?(Bkey$B$KBP$9$kCM$r(BUnit Directory$B$+$iFI$_=P$9(B
/*!
  \param key	key$B$9$J$o$A(B4byte$B$NJB$S$N(BMSB$BB&(B8bit
 */
u_int
Ieee1394Node::readValueFromUnitDirectory(u_char key) const
{
  // Read length of Bus Info Block and skip it.
    u_int	offset = 0;
    quadlet_t	quad = readQuadletFromConfigROM(offset);
    offset += 4 * (1 + (quad >> 24));

  // Read unit_directory_offset.
    u_int	tmpOffset = readValueFromDirectory(0xd1, offset);
    offset += 4 * tmpOffset;

  // Read unit_spec_ID and return it.
    return readValueFromDirectory(key, offset);
}

u_int
Ieee1394Node::readValueFromDirectory(u_char key, u_int& offset) const
{
  // Read length of the directory in quadlets.
    quadlet_t	quad = readQuadletFromConfigROM(offset);
    u_int	length = quad >> 16;
    offset += 4;

  // Read each field of the directory.
    for (u_int i = 0; i < length; ++i)
    {
	quad = readQuadletFromConfigROM(offset);
	if (u_char(quad >> 24) == key)
	    return (quad & 0xffffff);
	offset += 4;
    }

    throw std::runtime_error("TU::Ieee1394Node::readValueFromDirectory: field with specified key not found!!");

    return ~0;
}

quadlet_t
Ieee1394Node::readQuadletFromConfigROM(u_int offset) const
{
    return readQuadlet(CSR_REGISTER_BASE + CSR_CONFIG_ROM + offset);
}
    
//! $B%N!<%IFb$N;XDj$5$l$?%"%I%l%9$+$i(B4byte$B$NCM$rFI$_=P$9(B
/*!
  \param addr	$B8D!9$N%N!<%IFb$N@dBP%"%I%l%9(B
 */
quadlet_t
Ieee1394Node::readQuadlet(nodeaddr_t addr) const
{
    using namespace	std;

    quadlet_t	quad;
    if (raw1394_read(_handle, _nodeId, addr, 4, &quad) < 0)
	throw runtime_error(string("TU::Ieee1394Node::readQuadlet: failed to read from port!! ") + strerror(errno));
    if (_delay != 0)
	::usleep(_delay);
    return quadlet_t(ntohl(quad));
}

//! $B%N!<%IFb$N;XDj$5$l$?%"%I%l%9$K(B4byte$B$NCM$r=q$-9~$`(B
/*!
  \param addr	$B8D!9$N%N!<%IFb$N@dBP%"%I%l%9(B
  \param quad	$B=q$-9~$`(B4byte$B$NCM(B
 */
void
Ieee1394Node::writeQuadlet(nodeaddr_t addr, quadlet_t quad)
{
    using namespace	std;

    quad = htonl(quad);
    if (raw1394_write(_handle, _nodeId, addr, 4, &quad) < 0)
	throw runtime_error(string("TU::Ieee1394Node::writeQuadlet: failed to write to port!! ") + strerror(errno));
    if (_delay != 0)
	::usleep(_delay);
}

//! isochronous$B<u?.MQ$N%P%C%U%!$r3d$jEv$F$k(B
/*!
  \param packet_size	$B<u?.$9$k%Q%1%C%H(B1$B$D$"$?$j$N%5%$%:(B($BC10L(B: byte)
  \param buf_size	$B%P%C%U%!(B1$B$D$"$?$j$N%5%$%:(B($BC10L(B: byte)
  \param nb_buffers	$B3d$jEv$F$k%P%C%U%!?t(B
  \return		$B3d$jEv$F$i$l$?(Bisochronous$B<u?.MQ$N%A%c%s%M%k(B
 */
u_char
Ieee1394Node::mapListenBuffer(size_t packet_size,
			      size_t buf_size, u_int nb_buffers)
{
    using namespace	std;
    
  // Unmap previously mapped buffer and unlisten the channel.
    unmapListenBuffer();

#if defined(USE_VIDEO1394)
  // Change buffer size and listen to the channel.
  //   *Caution: _mmap.buf_size may be changed by VIDEO1394_LISTEN_CHANNEL.
    _mmap.nb_buffers  = nb_buffers;
    _mmap.buf_size    = _buf_size = buf_size;
    _mmap.packet_size = packet_size;
    if (ioctl(_port->fd(), VIDEO1394_IOC_LISTEN_CHANNEL, &_mmap) < 0)
	throw runtime_error(string("TU::Ieee1394Node::mapListenBuffer: VIDEO1394_IOC_LISTEN_CHANNEL failed!! ") + strerror(errno));
    for (int i = 0; i < _mmap.nb_buffers; ++i)
    {
	video1394_wait	wait;
	wait.channel = _mmap.channel;
	wait.buffer  = i;
	if (ioctl(_port->fd(), VIDEO1394_IOC_LISTEN_QUEUE_BUFFER, &wait) < 0)
	    throw runtime_error(string("TU::Ieee1394Node::mapListenBuffer: VIDEO1394_IOC_LISTEN_QUEUE_BUFFER failed!! ") + strerror(errno));
    }

  // Reset buffer status and re-map new buffer.
    if ((_buf = (u_char*)mmap(0, _mmap.nb_buffers * _mmap.buf_size,
			      PROT_READ, MAP_SHARED, _port->fd(), 0))
	== (u_char*)-1)
    {
	_buf = 0;
	throw runtime_error(string("TU::Ieee1394Node::mapListenBuffer: mmap failed!! ") + strerror(errno));
    }

    usleep(100000);
    return _mmap.channel;
#else
  // $B%P%C%U%!(B1$B$DJ,$N%G!<%?$rE>Aw$9$k$?$a$KI,MW$J%Q%1%C%H?t(B
    const u_int	npackets = (buf_size - 1) / packet_size + 1;

  // raw1394_loop_iterate()$B$O!$(Binterval$B8D$N%Q%1%C%H$r<u?.$9$k$?$S$K%f!<%6B&$K(B
  // $B@)8f$rJV$9!%(Blibraw1394$B$G$O$3$N%G%U%)%k%HCM$O%Q%1%C%H?t$N(B1/4$B$G$"$k!%$?$@$7!$(B
  // 512$B%Q%1%C%H$r1[$($kCM$r;XDj$9$k$H!$(Braw1394_loop_iterate()$B$+$i5"$C$F$3$J$/(B
  // $B$J$k$h$&$G$"$k!%(B
#  if defined(__APPLE__)
    const u_int	interval = npackets;
#  else
    const u_int	interval = std::min(npackets/4, 512U);
#  endif
#  if defined(DEBUG)
    cerr << "mapListenBuffer: npackets = " << npackets
	 << ", interval = " << interval << endl;
#  endif
    _buf     = new u_char[buf_size + interval * packet_size];
    _mid     = _buf + buf_size;
    _end     = _mid + interval * packet_size;
    _current = _buf;

    if (raw1394_iso_recv_init(_handle, &Ieee1394Node::receive,
			      nb_buffers * npackets, packet_size, _channel,
			      RAW1394_DMA_BUFFERFILL, interval) < 0)
	throw runtime_error(string("TU::Ieee1394Node::mapListenBuffer: failed to initialize iso reception!! ") + strerror(errno));
    if (raw1394_iso_recv_start(_handle, -1, -1, 0) < 0)
	throw runtime_error(string("TU::Ieee1394Node::mapListenBuffer: failed to start iso reception!! ") + strerror(errno));
    return _channel;
#endif
}

//! isochronous$B%G!<%?$,<u?.$5$l$k$N$rBT$D(B
/*!
  $B<B:]$K%G!<%?$,<u?.$5$l$k$^$G(B, $BK\4X?t$O8F$S=P$7B&$K@)8f$rJV$5$J$$(B. 
  \return	$B%G!<%?$NF~$C$?%P%C%U%!$N@hF,%"%I%l%9(B. 
 */
const u_char*
Ieee1394Node::waitListenBuffer()
{
    using namespace	std;

#if defined(USE_VIDEO1394)
    video1394_wait	wait;
    wait.channel = _mmap.channel;
    wait.buffer  = _current;
    if (ioctl(_port->fd(), VIDEO1394_IOC_LISTEN_WAIT_BUFFER, &wait) < 0)
	throw runtime_error(string("TU::Ieee1394Node::waitListenBuffer: VIDEO1394_IOC_LISTEN_WAIT_BUFFER failed!! ") + strerror(errno));

  // wait.filltime$B$O!$A4%Q%1%C%H$,E~Ce$7$F%P%C%U%!$,0lGU$K$J$C$?;~9o$rI=$9!%(B
  // $B$3$l$+$i:G=i$N%Q%1%C%H$,E~Ce$7$?;~9o$r?dDj$9$k$?$a$K!$%P%C%U%!$"$?$j$N(B
  // $B%Q%1%C%H?t$K%Q%1%C%HE~Ce4V3V(B(125 micro sec)$B$r>h$8$??t$r8:$8$k!%(B
    _arrivaltime = u_int64_t(wait.filltime.tv_sec)*1000000LL
		 + u_int64_t(wait.filltime.tv_usec)
		 - u_int64_t(((_mmap.buf_size - 1)/_mmap.packet_size + 1)*125);

    return _buf + _current * _mmap.buf_size;
#else
#  if defined(DEBUG)
    cerr << "*** BEGIN [waitListenBuffer] ***" << endl;
#  endif
    while (_current < _mid)		// [_buf, _mid)$B$,K~$?$5$l$k$^$G(B
    {
	raw1394_loop_iterate(_handle);	// $B%Q%1%C%H$r<u?.$9$k!%(B
#  if defined(DEBUG)
	cerr << "      current = " << _current - _buf << endl;
#  endif
    }
#  if defined(DEBUG)
    cerr << "*** END   [waitListenBuffer] ***" << endl;
#  endif

    return _buf;
#endif
}

//! $B%G!<%?<u?.:Q$_$N%P%C%U%!$r:F$S%-%e!<%$%s%0$7$F<!$N<u?.%G!<%?$KHw$($k(B
void
Ieee1394Node::requeueListenBuffer()
{
    using namespace	std;
    
#if defined(USE_VIDEO1394)
    video1394_wait	wait;
    wait.channel = _mmap.channel;
    wait.buffer	 = _current;
    if (ioctl(_port->fd(), VIDEO1394_IOC_LISTEN_QUEUE_BUFFER, &wait) < 0)
	throw runtime_error(string("TU::Ieee1394Node::requeueListenBuffer: VIDEO1394_IOC_LISTEN_QUEUE_BUFFER failed!! ") + strerror(errno));
    ++_current %= _mmap.nb_buffers;	// next buffer.
#else
    if (_current != 0)
    {
      // [_buf, _mid) $B$rGQ4~$7(B [_mid, _current)$B$r%P%C%U%!NN0h$N@hF,$K0\$9(B
	const size_t	len = _current - _mid;
	memcpy(_buf, _mid, len);
	_current     = _buf + len;
	_arrivaltime = _arrivaltime_next;
#  if defined(DEBUG)
	cerr << "*** BEGIN [requeueListenBuffer] ***" << endl;
	cerr << "      current = " << _current - _buf
	     << " (" << len << " bytes moved...)" << endl;
	cerr << "*** END   [requeueListenBuffer] ***" << endl;
#  endif
    }
#endif
}

//! $B$9$Y$F$N<u?.MQ%P%C%U%!$NFbMF$r6u$K$9$k(B
void
Ieee1394Node::flushListenBuffer()
{
#if defined(USE_VIDEO1394)
  // Force flushing by doing unmap and then map buffer.
    if (_buf != 0)
	mapListenBuffer(_mmap.packet_size, _buf_size, _mmap.nb_buffers);
#else
    using namespace	std;

    if (raw1394_iso_recv_flush(_handle) < 0)
	throw runtime_error(string("TU::Ieee1394Node::flushListenBuffer: failed to flush iso receive buffer!! ") + strerror(errno));
    _current = _buf;
#endif    
}

//! $B%N!<%I$K3d$jEv$F$?$9$Y$F$N<u?.MQ%P%C%U%!$rGQ4~$9$k(B
void
Ieee1394Node::unmapListenBuffer()
{
    using namespace	std;

    if (_buf != 0)
    {
#if defined(USE_VIDEO1394)
	munmap(_buf, _mmap.nb_buffers * _mmap.buf_size);
	_buf = 0;				// Reset buffer status.
	_buf_size = _current = 0;		// ibid.
	if (ioctl(_port->fd(), VIDEO1394_IOC_UNLISTEN_CHANNEL, &_mmap.channel) < 0)
	    throw runtime_error(string("TU::Ieee1394Node::unmapListenBuffer: VIDEO1394_IOC_UNLISTEN_CHANNEL failed!! ") + strerror(errno));
#else
	raw1394_iso_stop(_handle);
	raw1394_iso_shutdown(_handle);

	delete [] _buf;
	_buf = _mid = _end = _current = 0;
#endif
    }
}

u_int64_t
Ieee1394Node::cycletimeToLocaltime(u_int32_t cycletime) const
{
  // $B8=:_$N%5%$%/%k;~9o$H;~9o$r3MF@$9$k!%(B
    u_int32_t	cycletime0;
    u_int64_t	localtime0;
    raw1394_read_cycle_timer(_handle, &cycletime0, &localtime0);

  // $B8=;~9o$H;XDj$5$l$?;~9o$N%5%$%/%k;~9o$r%5%V%5%$%/%kCM$KD>$7!$(B
  // $BN><T$N$:$l$r5a$a$k!%(B
    u_int32_t	subcycle0 = cycletime_to_subcycle(cycletime0);
    u_int32_t	subcycle  = cycletime_to_subcycle(cycletime);
    u_int64_t	diff	  = (subcycle0 + (128LL*8000LL*3072LL) - subcycle)
			  % (128LL*8000LL*3072LL);

  // $B$:$l$r(Bmicro sec$BC10L$KD>$7$F(B(1 subcycle = 125/3072 usec)$B8=:_;~9o$+$i:9$70z$/(B. 
    return localtime0 - (125LL*diff)/3072LL;
}

u_int64_t
Ieee1394Node::cycleToLocaltime(u_int32_t cycle) const
{
  // $B8=:_$N%5%$%/%k;~9o$H;~9o$r3MF@$9$k!%(B
    u_int32_t	cycletime0;
    u_int64_t	localtime0;
    raw1394_read_cycle_timer(_handle, &cycletime0, &localtime0);

  // $B8=:_$N%5%$%/%k;~9o$+$i%5%$%/%kCM(B($B<~4|!'(B8000)$B$r<h$j=P$7!$M?$($i$l$?(B
  // $B%5%$%/%kCM$H$N$:$l$r5a$a$k!%(B
    u_int32_t	cycle0 = (cycletime0 & 0x1fff000) >> 12;
    u_int32_t	diff   = (cycle0 + 8000 - cycle) % 8000;
    
  // $B$:$l$r(Bmicro sec$BC10L$KD>$7$F(B(1 cycle = 125 micro sec)$B8=:_;~9o$+$i:9$70z$/(B. 
    return localtime0 - u_int64_t(125*diff);
}

#if !defined(USE_VIDEO1394)
//! $B$3$N%N!<%I$K3d$jEv$F$i$l$?(Bisochronous$B<u?.MQ%P%C%U%!$K%Q%1%C%H%G!<%?$rE>Aw$9$k(B
/*!
  $BK\%O%s%I%i$O!$%Q%1%C%H$,(B1$B$D<u?.$5$l$k$?$S$K8F$S=P$5$l$k!%$^$?!$(BmapListenBuffer()
  $BFb$N(B raw1394_iso_recv_init() $B$r8F$s$@;~E@$G4{$K(Bisochronous$BE>Aw$,;O$^$C$F$$$k(B
  $B>l9g$O!$(BwaitListenBuffer() $BFb$G(B raw1394_loop_iterate() $B$r8F$P$J$/$F$b$3$N(B
  $B%O%s%I%i$,8F$P$l$k$3$H$,$"$k$?$a!$(Bbuffer overrun $B$rKI$0J}:v$O$3$N%O%s%I%iFb$G(B
  $B$H$C$F$*$+$J$1$l$P$J$i$J$$!%(B
  \param data	$B%Q%1%C%H%G!<%?(B
  \param len	$B%Q%1%C%H%G!<%?$N%P%$%H?t(B
  \param sy	$B%U%l!<%`$N@hF,%Q%1%C%H$G$"$l$P(B1, $B$=$&$G$J$1$l$P(B0
*/
raw1394_iso_disposition
Ieee1394Node::receive(raw1394handle_t handle,
		      u_char* data, u_int len,
		      u_char channel, u_char tag, u_char sy,
		      u_int cycle, u_int dropped)
{
    using namespace	std;

    Ieee1394Node* const	node = (Ieee1394Node*)raw1394_get_userdata(handle);

  // [_buf, _mid)$B$ND9$5$r%U%l!<%`%5%$%:$K$7$F$"$k$N$G!$(Bsy$B%Q%1%C%H$OI,$:(B
  // _buf, _mid$B$N$$$:$l$+$K5-O?$5$l$k!%(Braw1394_loop_iterate() $B$O!$$3$N(B
  // $B%O%s%I%i$r(Binterval$B2s8F$V!%FC$K(Binterval$B$,(B1$B%U%l!<%`$"$?$j$N%Q%1%C%H?t$N(B
  // $BLs?t$G$J$$>l9g$O!$(B_buf$B$K(Bsy$B%Q%1%C%H$rFI$_9~$s$@8e$K%U%l!<%`A4BN$N<u?.$,(B
  // $B40N;$7$?8e$K!$$5$i$K(B_mid$B$K<!$N%U%l!<%`$N(Bsy$B%Q%1%C%H$,FI$_9~$^$l$k!%(B
    if (sy)				// $B%U%l!<%`$N@hF,%Q%1%C%H$J$i$P(B...
    {
	node->_arrivaltime_next = node->cycleToLocaltime(cycle);
	
	if (node->_current != node->_mid)	// _mid$B$,FI$_9~$_@h$G$J$1$l$P(B
	{
	    node->_current = node->_buf;	// _buf$B$KFI$_9~$`(B
	    node->_arrivaltime = node->_arrivaltime_next;
	}
#  if defined(DEBUG)
	u_int64_t
	    timestamp = node->cycletimeToLocaltime(ntohl(*(u_int32_t*)data));
	cerr << " (sy: current = " << node->_current - node->_buf
	     << ", cycle = " << cycle << ')'
	     << " diff: ";
	print_time(cerr, node->_arrivaltime_next - timestamp);
	cerr << ", arrivaltime: ";
	print_time(cerr, node->_arrivaltime_next);
	cerr << ", timestamp: ";
	print_time(cerr, timestamp);
	cerr << endl;
#  endif	
    }
#  if DEBUG==2
    else
    {
	cerr << " (    current = ";
	if (node->_current != 0)
	    cerr << node->_current - node->_buf;
	else
	    cerr << "NULL";
	cerr << ", cycle = " << cycle << ')';
	u_int64_t	captime = node->cycleToLocaltime(cycle);
	cerr << " captime: ";
	print_time(cerr, captime);
	cerr << endl;
    }
#  endif
    if (node->_current + len <= node->_end)	// overrun$B$,@8$8$J$1$l$P(B...
    {
	memcpy(node->_current, data, len);	// $B%Q%1%C%H$rFI$_9~$`(B
	node->_current += len;			// $B<!$NFI$_9~$_0LCV$K?J$a$k(B
    }
    
    return RAW1394_ISO_OK;
}
#endif
 
}
