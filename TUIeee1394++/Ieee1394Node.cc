/*
 *  $Id: Ieee1394Node.cc,v 1.3 2002-12-09 07:47:50 ueshiba Exp $
 */
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdexcept>
#include <string>
#include "TU/Ieee1394++.h"

namespace TU
{
/************************************************************************
*  class Ieee1394Node							*
************************************************************************/
//! IEEE1394$B%N!<%I%*%V%8%'%/%H$r@8@.$9$k(B
/*!
  \param port		$B$3$N%N!<%I$,@\B3$5$l$F$$$k%]!<%H!%(B
  \param unit_spec_ID	$B$3$N%N!<%I$N<oN`$r<($9(BID(ex. IEEE1394$B%G%8%?%k%+%a%i(B
		       $B$G$"$l$P!$(B0x00a02d).
  \param channel	isochronous$BE>AwMQ$N%A%c%M%kHV9f(B(\f$0 \leq
			\f$channel\f$ < 64\f$)
  \param sync_tag	1$B$^$H$^$j$N%G!<%?$rJ#?t$N%Q%1%C%H$KJ,3d$7$F(B
			isochronous$B%b!<%I$G<u?.$9$k:]$K!$:G=i$N%Q%1%C%H$K(B
			$BF14|MQ$N(Btag$B$,$D$$$F$$$k>l9g$O(B1$B$r;XDj!%$=$&$G$J$1$l(B
			$B$P(B0$B$r;XDj!%(B
  \param flags		video1394$B$N%U%i%0!%(BVIDEO1394_SYNC_FRAMES, 
			VIDEO1394_INCLUDE_ISO_HEADERS,
			VIDEO1394_VARIABLE_PACKET_SIZE$B$NAH9g$o$;!%(B
  \param uniqId		$B8D!9$N5!4o8GM-$N(B64bit ID$B!%F10l$N(BIEEE1394 bus$B$K(B
			$BF10l$N(Bunit_spec_ID$B$r;}$DJ#?t$N5!4o$,@\B3$5$l$F(B
			$B$$$k>l9g!$$3$l$K$h$C$FF1Dj$r9T$&!%(B
			0$B$,M?$($i$l$k$H!$;XDj$5$l$?(Bunit_spec_ID$B$r;}$A(B
			$B$^$@(B#Ieee1394Node$B%*%V%8%'%/%H$r3d$jEv$F$i$l$F(B
			$B$$$J$$5!4o$N$&$A!$0lHV:G=i$K$_$D$+$C$?$b$N$,$3$N(B
			$B%*%V%8%'%/%H$H7k$S$D$1$i$l$k!%(B
*/
Ieee1394Node::Ieee1394Node(Ieee1394Port& port, u_int unit_spec_ID,
			   u_int channel, int sync_tag, int flags,
			   u_int64 uniqId)
    :_port(port), _nodeId(0), _mmap(), _buf_size(0),
     _buf(0), _current(0), _nready(0)
{
  // Find a node yet registered to the port and satisfying the specification.
    u_int	i, nnodes = _port.nnodes();
    for (i = 0; i < nnodes; ++i)
    {
	_nodeId = (i | 0xffc0);		// node on local bus

	if (_nodeId != _port.nodeId()			     &&
	    !_port.isRegisteredNode(*this)		     &&
	    readValueFromUnitDirectory(0x12) == unit_spec_ID &&
	    (uniqId == 0 || globalUniqueId() == uniqId))
	    break;
    }
    if (i == nnodes)
	throw std::runtime_error("TU::Ieee1394Node::Ieee1394Node: node with specified unit_spec_ID (and global_unique_ID) not found!!");

    _mmap.channel     = channel;
    _mmap.sync_tag    = sync_tag;
    _mmap.nb_buffers  = 0;
    _mmap.buf_size    = 0;
    _mmap.packet_size = 0;
    _mmap.fps	      = 0;
    _mmap.flags	      = flags;

  // Register this instance to the port.
    _port.registerNode(*this);
}
	     
//! IEEE1394$B%N!<%I%*%V%8%'%/%H$rGK2u$9$k(B
Ieee1394Node::~Ieee1394Node()
{
    unmapListenBuffer();
    _port.unregisterNode(*this);
}

//! $B$3$N%N!<%I$K7k$SIU$1$i$l$F$$$k5!4o8GM-$N(B64bit ID$B$rJV$9(B
u_int64
Ieee1394Node::globalUniqueId() const
{
    u_int64	hi = readQuadletFromConfigROM(0x0c),
		lo = readQuadletFromConfigROM(0x10);
    return (hi << 32) | lo;
}

//! $BM?$($i$l$?(Bkey$B$KBP$9$kCM$r(BUnit Dependent Directory$B$+$iFI$_=P$9(B
/*!
  \param key	key$B$9$J$o$A(B4byte$B$NJB$S$N(BMSB$BB&(B8bit$B!%(B
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
  \param key	key$B$9$J$o$A(B4byte$B$NJB$S$N(BMSB$BB&(B8bit$B!%(B
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

//! $B%N!<%IFb$N;XDj$5$l$?%"%I%l%9$+$i(B4byte$B$NCM$rFI$_=P$9(B
/*!
  \param addr	$B8D!9$N%N!<%IFb$N@dBP%"%I%l%9!%(B
 */
quadlet_t
Ieee1394Node::readQuadlet(nodeaddr_t addr) const
{
    using namespace	std;

    quadlet_t	quad;
    if (raw1394_read(_port.handle(), _nodeId, addr, 4, &quad) < 0)
	throw runtime_error(string("TU::Ieee1394Node::readQuadlet: failed to read from port!! ") + strerror(errno));
    if (_port.delay() != 0)
	::usleep(_port.delay());
    return quadlet_t(ntohl(quad));
}

//! $B%N!<%IFb$N;XDj$5$l$?%"%I%l%9$K(B4byte$B$NCM$r=q$-9~$`(B
/*!
  \param addr	$B8D!9$N%N!<%IFb$N@dBP%"%I%l%9!%(B
  \param quad	$B=q$-9~$`(B4byte$B$NCM!%(B
 */
void
Ieee1394Node::writeQuadlet(nodeaddr_t addr, quadlet_t quad)
{
    using namespace	std;

    quad = htonl(quad);
    if (raw1394_write(_port.handle(), _nodeId, addr, 4, &quad) < 0)
	throw runtime_error(string("TU::Ieee1394Node::writeQuadlet: failed to write to port!! ") + strerror(errno));
    if (_port.delay() != 0)
	::usleep(_port.delay());
}

//! isochronous$B<u?.MQ$N%P%C%U%!$r3d$jEv$F$k(B
/*!
  \param packet_size	$B<u?.$9$k%Q%1%C%H(B1$B$D$"$?$j$N%5%$%:(B($BC10L(B: byte).
  \param buf_size	$B%P%C%U%!(B1$B$D$"$?$j$N%5%$%:(B($BC10L(B: byte)$B!%(B
  \param nb_buffers	$B3d$jEv$F$k%P%C%U%!?t!%(B
 */
void
Ieee1394Node::mapListenBuffer(size_t packet_size,
			      size_t buf_size, u_int nb_buffers)
{
    using namespace	std;
    
  // Unmap previously mapped buffer and unlisten the channel.
    unmapListenBuffer();

  // Change buffer size and listen to the channel.
  //   *Caution: _mmap.buf_size may be changed by VIDEO1394_LISTEN_CHANNEL.
    _mmap.nb_buffers  = nb_buffers;
    _mmap.buf_size    = _buf_size = buf_size;
    _mmap.packet_size = packet_size;
    if (ioctl(_port.fd(), VIDEO1394_LISTEN_CHANNEL, &_mmap) < 0)
	throw runtime_error(string("TU::Ieee1394Node::mapListenBuffer: VIDEO1394_LISTEN_CHANNEL failed!! ") + strerror(errno));
    for (int i = 0; i < _mmap.nb_buffers; ++i)
    {
	video1394_wait	wait;
	wait.channel = _mmap.channel;
	wait.buffer  = i;
	if (ioctl(_port.fd(), VIDEO1394_LISTEN_QUEUE_BUFFER, &wait) < 0)
	    throw runtime_error(string("Ieee1394Node::mapListenBuffer: VIDEO1394_LISTEN_QUEUE_BUFFER failed!! ") + strerror(errno));
    }

  // Reset buffer status and re-map new buffer.
    if ((_buf = (u_char*)mmap(0, _mmap.nb_buffers * _mmap.buf_size,
			      PROT_READ, MAP_SHARED, _port.fd(), 0))
	== (u_char*)-1)
    {
	_buf = 0;
	throw runtime_error(string("Ieee1394Node::mapListenBuffer: mmap failed!! ") + strerror(errno));
    }

    usleep(100000);
}

//! isochronous$B%G!<%?$,<u?.$5$l$k$N$rBT$D(B
/*!
  $B<B:]$K%G!<%?$,<u?.$5$l$k$^$G!$K\4X?t$O8F$S=P$7B&$K@)8f$rJV$5$J$$!%(B
  \return	$B%G!<%?$NF~$C$?%P%C%U%!$N@hF,%"%I%l%9!%%G!<%?$N%5%$%:$O(B
		getBufferSize()$B$GCN$k$3$H$,$G$-$k!%(B
 */
const u_char*
Ieee1394Node::waitListenBuffer()
{
    using namespace	std;

    if (_nready == 0)
    {
	video1394_wait	wait;
	wait.channel = _mmap.channel;
	wait.buffer  = _current;
	if (ioctl(_port.fd(), VIDEO1394_LISTEN_WAIT_BUFFER, &wait) < 0)
	    throw runtime_error(string("TU::Ieee1394Node::waitListenBuffer: VIDEO1394_LISTEN_WAIT_BUFFER failed!! ") + strerror(errno));
	_nready = 1 + wait.buffer;  // current and subsequent ready buffers.
    }
    return _buf + _current * _mmap.buf_size;
}

//! $B%G!<%?<u?.:Q$_$N%P%C%U%!$r:F$S%-%e!<%$%s%0$7$F<!$N<u?.%G!<%?$KHw$($k(B
void
Ieee1394Node::requeueListenBuffer()
{
    using namespace	std;

    video1394_wait	wait;
    wait.channel = _mmap.channel;
    wait.buffer	 = _current;
    if (ioctl(_port.fd(), VIDEO1394_LISTEN_QUEUE_BUFFER, &wait) < 0)
	throw runtime_error(string("TU::Ieee1394Node::requeueListenBuffer: VIDEO1394_LISTEN_QUEUE_BUFFER failed!! ") + strerror(errno));
    ++_current %= _mmap.nb_buffers;	// next buffer.
    --_nready;				// # of ready buffers.
}

//! $B$9$Y$F$N<u?.MQ%P%C%U%!$NFbMF$r6u$K$9$k(B
void
Ieee1394Node::flushListenBuffer()
{
  // Force flushing by doing unmap and then map buffer.
    if (_buf != 0)
	mapListenBuffer(_mmap.packet_size, _buf_size, _mmap.nb_buffers);
    
  // POLL(kernel-2.4$B0J9_$N$_$GM-8z(B)$B$7$F(BREADY$B>uBV$N%P%C%U%!$rA4$F(Brequeue$B$9$k!%(B
  // 1$B$D$N%P%C%U%!$,0lGU$K$J$kA0$K(Bisochronous$BE>Aw$,Dd;_$5$l$k$H!$$=$N%P%C%U%!(B
  // $B$O(BREADY$B>uBV$K$O$J$i$:(BQUEUED$B>uBV$N$^$^$J$N$G!$IT40A4$J%G!<%?$r;}$C$?$^$^(B
  // $B;D$C$F$7$^$&!%$7$?$,$C$F!$$3$NJ}K!$ONI$/$J$$!%(B(2002.3.7)
  /*    if (_buf != 0)
	for (;;)
	{
	    video1394_wait	wait;
	    wait.channel = _mmap.channel;
	    wait.buffer  = _current;
	    if (ioctl(_port.fd(), VIDEO1394_LISTEN_POLL_BUFFER, &wait) < 0)
		break;
	    _nready = 1 + wait.buffer;
	    requeueListenBuffer();
	    }*/
  // "_nready" must be 0 here(no available buffers).
}

//! $B%N!<%I$K3d$jEv$F$?$9$Y$F$N<u?.MQ%P%C%U%!$rGQ4~$9$k(B
void
Ieee1394Node::unmapListenBuffer()
{
    using namespace	std;

    if (_buf != 0)
    {
	munmap(_buf, _mmap.nb_buffers * _mmap.buf_size);
	_buf = 0;				// Reset buffer status.
	_buf_size = _current = _nready = 0;	// ibid.
	if (ioctl(_port.fd(), VIDEO1394_UNLISTEN_CHANNEL, &_mmap.channel) < 0)
	    throw runtime_error(string("TU::Ieee1394Node::unmapListenBuffer: VIDEO1394_UNLISTEN_CHANNEL failed!! ") + strerror(errno));
    }
}
 
}
