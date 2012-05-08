/*
 *  $Id: Ieee1394++.h,v 1.31 2012-05-08 02:31:26 ueshiba Exp $
 */
/*!
  \mainpage	libTUIeee1394++ - IIDC 1394$B%Y!<%9$N%G%8%?%k%+%a%i$r@)8f$9$k(BC++$B%i%$%V%i%j(B
  \anchor	libTUIeee1394

  \section copyright $BCx:n8"(B
  Copyright (C) 2003-2006 Toshio UESHIBA
  National Institute of Advanced Industrial Science and Technology (AIST)
 
  Written by Toshio UESHIBA <t.ueshiba@aist.go.jp>
 
  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.  This library is
  distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
  License for more details.
 
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA

  \section abstract $B35MW(B
  libTUIeee1394++$B$O!$(B
  <a href="http://www.1394ta.com/Technology/Specifications/specifications.htm">
  IIDC 1394$B%Y!<%9$N%G%8%?%k%+%a%i(B</a>$B$r@)8f$9$k(BC++$B%i%$%V%i%j$G$"$k(B. $BF1(B
  $B0l$^$?$O0[$J$k(BIEEE1394$B%P%9$K@\B3$5$l$?J#?t$N%+%a%i$r!$$=$l$>$lFHN)$K(B
  $B%3%s%H%m!<%k$9$k$3$H$,$G$-$k(B. 

  $B<BAu$5$l$F$$$k<gMW$J%/%i%9$*$h$S$=$N(Bpublic$B$J%a%s%P4X?t$O!$$*$*$^$+$K(B
  $B0J2<$N$h$&$KJ,N`$5$l$k(B. 

  #TU::Ieee1394Node - IEEE1394$B%P%9$K@\B3$5$l$kMM!9$J5!4o$N%Y!<%9$H$J$k%/%i%9(B
  - #TU::Ieee1394Node::nodeId()
  - #TU::Ieee1394Node::globalUniqueId()
  - #TU::Ieee1394Node::filltime()
  - #TU::Ieee1394Node::channel()
  - #TU::Ieee1394Node::delay()

  #TU::Ieee1394Camera - IEEE1394$B%G%8%?%k%+%a%i$rI=$9%/%i%9(B

  - <b>$B4pK\5!G=(B</b>
    - #TU::Ieee1394Camera::inquireBasicFunction()
    - #TU::Ieee1394Camera::powerOn()
    - #TU::Ieee1394Camera::powerOff()
    - #TU::Ieee1394Camera::bayerTileMapping()
    - #TU::Ieee1394Camera::isLittleEndian()
  
  - <b>$B2hA|%U%)!<%^%C%H$H%U%l!<%`%l!<%H(B</b>
    - #TU::Ieee1394Camera::inquireFrameRate()
    - #TU::Ieee1394Camera::setFormatAndFrameRate()
    - #TU::Ieee1394Camera::getFormat()
    - #TU::Ieee1394Camera::getFrameRate()
    - #TU::Ieee1394Camera::width()
    - #TU::Ieee1394Camera::height()
    - #TU::Ieee1394Camera::pixelFormat()

  - <b>$BFC<l%U%)!<%^%C%H(B(Format_7)</b>
    - #TU::Ieee1394Camera::getFormat_7_Info()
    - #TU::Ieee1394Camera::setFormat_7_ROI()
    - #TU::Ieee1394Camera::setFormat_7_PixelFormat()

  - <b>$B2hA|$N;#1F%b!<%I$N@_Dj(B</b>
    - #TU::Ieee1394Camera::continuousShot()
    - #TU::Ieee1394Camera::stopContinuousShot()
    - #TU::Ieee1394Camera::inContinuousShot()
    - #TU::Ieee1394Camera::oneShot()
    - #TU::Ieee1394Camera::multiShot()

  - <b>$B2hA|$N;#1F(B</b>
    - #TU::Ieee1394Camera::snap()

  - <b>$B2hA|$N<h$j9~$_(B</b>
    - #TU::Ieee1394Camera::operator >>()
    - #TU::Ieee1394Camera::captureRGBImage()
    - #TU::Ieee1394Camera::captureDirectly()
    - #TU::Ieee1394Camera::captureRaw()
    - #TU::Ieee1394Camera::captureBayerRaw()

  - <b>$B%+%a%i$NMM!9$J5!G=$N@)8f(B</b>
    - #TU::Ieee1394Camera::inquireFeatureFunction()
    - #TU::Ieee1394Camera::onePush()
    - #TU::Ieee1394Camera::inOnePushOperation()
    - #TU::Ieee1394Camera::turnOn()
    - #TU::Ieee1394Camera::turnOff()
    - #TU::Ieee1394Camera::isTurnedOn()
    - #TU::Ieee1394Camera::setAutoMode()
    - #TU::Ieee1394Camera::setManualMode()
    - #TU::Ieee1394Camera::isAuto()
    - #TU::Ieee1394Camera::setValue()
    - #TU::Ieee1394Camera::getValue()
    - #TU::Ieee1394Camera::getMinMax()
    - #TU::Ieee1394Camera::setWhiteBalance()
    - #TU::Ieee1394Camera::getWhiteBalance()
    - #TU::Ieee1394Camera::getAimedTemperature()

  - <b>$B%H%j%,%b!<%I(B</b>
    - #TU::Ieee1394Camera::setTriggerMode()
    - #TU::Ieee1394Camera::getTriggerMode()
    - #TU::Ieee1394Camera::setTriggerPolarity()
    - #TU::Ieee1394Camera::getTriggerPolarity()

  - <b>$B%+%a%i@_Dj$NJ]B8(B</b>
    - #TU::Ieee1394Camera::saveConfig()
    - #TU::Ieee1394Camera::restoreConfig()
    - #TU::Ieee1394Camera::getMemoryChannelMax()

  #TU::Ieee1394CameraArray - IEEE1394$B%G%8%?%k%+%a%i$NG[Ns$rI=$9%/%i%9(B
    - #TU::Ieee1394CameraArray::Ieee1394CameraArray()

  \file		Ieee1394++.h
  \brief	IEEE1394$B%G%P%$%9$K4XO"$9$k%/%i%9$NDj5A$H<BAu(B
*/
#ifndef __TUIeee1394PP_h
#define __TUIeee1394PP_h

#include <libraw1394/raw1394.h>
#if !defined(__APPLE__)
#  include <map>
#  if defined(USE_VIDEO1394)
#    define __user
#    include <video1394.h>
#  endif
#endif
#include <netinet/in.h>
#include <stdexcept>
#include <sstream>
#if defined(HAVE_LIBTUTOOLS__)
#  include "TU/Image++.h"
#else
  typedef unsigned long long	u_int64_t;
#endif

/*!
  \namespace	TU
  \brief	$BK\%i%$%V%i%j$GDj5A$5$l$?%/%i%9$*$h$S4X?t$rG<$a$kL>A06u4V(B
 */
namespace TU
{
/************************************************************************
*  class Ieee1394Node							*
************************************************************************/
//! IEEE1394$B$N%N!<%I$rI=$9%/%i%9(B
/*!
  $B0lHL$K$O(B, $B$h$j6qBNE*$J5!G=$r;}$C$?%N!<%I(B(ex. $B%G%8%?%k%+%a%i(B)$B$rI=$9(B
  $B%/%i%9$N4pDl%/%i%9$H$7$FMQ$$$i$l$k(B. 
*/
class Ieee1394Node
{
#if !defined(__APPLE__)
  private:
    class Port
    {
      public:
	Port(int portNumber)				;
	~Port()						;

#  if defined(USE_VIDEO1394)
	int	fd()				const	{return _fd;}
#  endif
	int	portNumber()			const	{return _portNumber;}
	
	u_char	registerNode(const Ieee1394Node& node)		;
	bool	unregisterNode(const Ieee1394Node& node)	;
	bool	isRegisteredNode(const Ieee1394Node& node) const;
    
      private:
#  if defined(USE_VIDEO1394)
	const int	_fd;		// file desc. for video1394 device.
#  endif
	const int	_portNumber;
	u_int64_t	_nodes;		// a bitmap for the registered nodes
    };
#endif	// !__APPLE__
  protected:
  //! isochronous$BE>Aw$NB.EY(B
    enum Speed
    {
	SPD_100M = 0,				//!< 100Mbps
	SPD_200M = 1,				//!< 200Mbps
	SPD_400M = 2,				//!< 400Mbps
	SPD_800M = 3,				//!< 800Mbps
	SPD_1_6G = 4,				//!< 1.6Gbps
	SPD_3_2G = 5				//!< 3.2Gbps
    };

  public:
  //! $B$3$N%N!<%I$N(BID(IEEE1394 bus$B>e$N%"%I%l%9(B)$B$rJV$9(B
  /*!
    \return	$B$3$N%N!<%I$N(BID
   */
    nodeid_t	nodeId()			const	{return _nodeId;}

    u_int64_t	globalUniqueId()		const	;

  //! $B$3$N%N!<%I$K3d$jEv$F$i$l$?(Bisochronous$B<u?.MQ%P%C%U%!$,K~$?$5$l$?;~9o$rJV$9(B
  /*!
    \return	$B<u?.MQ%P%C%U%!$,K~$?$5$l$?;~9o(B
   */
    u_int64_t	filltime()			const	{return _filltime;}

  //! $B$3$N%N!<%I$K3d$jEv$F$i$l$?(Bisochronous$B%A%c%s%M%k$rJV$9(B
  /*!
    \return	isochronous$B%A%c%s%M%k(B
   */
#if defined(USE_VIDEO1394)
    u_char	channel()			const	{return _mmap.channel;}
#else
    u_char	channel()			const	{return _channel;}
#endif

  //! $B$3$N%N!<%I$K3d$jEv$F$i$l$?(Basynchronous$BDL?.$NCY1dNL$rJV$9(B
  /*!
    \return	$BCY1dNL!JC10L!'(Bmicro seconds$B!K(B
   */
    u_int	delay()				const	{return _delay;}
    
  protected:
    Ieee1394Node(u_int unit_spec_ID, u_int64_t uniqId, u_int delay
#if defined(USE_VIDEO1394)
		 , int sync_tag, int flag
#endif
		 )							;
    ~Ieee1394Node()							;
#if defined(__APPLE__)
    nodeaddr_t		cmdRegBase()				const	;
#endif
    u_int		readValueFromUnitDependentDirectory(u_char key)
								const	;
    quadlet_t		readQuadlet(nodeaddr_t addr)		const	;
    void		writeQuadlet(nodeaddr_t addr, quadlet_t quad)	;
    u_char		mapListenBuffer(size_t packet_size,
					size_t buf_size,
					u_int nb_buffers)		;
    const u_char*	waitListenBuffer()				;
    void		requeueListenBuffer()				;
    void		flushListenBuffer()				;
    void		unmapListenBuffer()				;
    
  private:
    Ieee1394Node(const Ieee1394Node&)					;
    Ieee1394Node&	operator =(const Ieee1394Node&)			;

    u_int		readValueFromUnitDirectory(u_char key)	const	;
    u_int		readValueFromDirectory(u_char key,
					       u_int& offset)	const	;
    quadlet_t		readQuadletFromConfigROM(u_int offset)	const	;
#if !defined(USE_VIDEO1394)
    static raw1394_iso_disposition
			receive(raw1394handle_t handle, u_char* data,
				u_int len, u_char channel,
				u_char tag, u_char sy,
				u_int cycle, u_int dropped)		;
#endif
#if !defined(__APPLE__)    
    static std::map<int, Port*>	_portMap;

    Port*			_port;
#endif
    raw1394handle_t		_handle;
    nodeid_t			_nodeId;
#if defined(USE_VIDEO1394)
    video1394_mmap		_mmap;		// mmap structure for video1394
    u_int			_current;	// index of current ready buffer
    u_int			_buf_size;	// buffer size excluding header
#else
    u_char			_channel;	// iso receive channel
    u_char*			_current;	// current insertion point
    u_char*			_end;		// the end of the buffer
    bool			_ready;		// buffer is ready
    u_int64_t			_filltime_next;
#endif
    u_char*			_buf;		// mapped buffer
    u_int64_t			_filltime;	// time of buffer filled
    const u_int			_delay;
};
    
/************************************************************************
*  class Ieee1394Camera							*
************************************************************************/
//! IEEE1394$B%G%8%?%k%+%a%i$rI=$9%/%i%9(B
/*!
  1394-based Digital Camera Specification ver. 1.30$B$K=`5r(B. 
*/
class Ieee1394Camera : public Ieee1394Node
{
  public:
  //! $B%+%a%i$N%?%$%W(B
    enum Type
    {
	Monocular	= 0x00a02d,	//!< $BC14c%+%a%i(B
	Binocular	= 0x00b09d	//!< $BFs4c%+%a%i(B
    };
	
  //! $B%+%a%i$,%5%]!<%H$7$F$$$k4pK\5!G=$rI=$9%S%C%H%^%C%W(B
  /*! $B$I$N$h$&$J4pK\5!G=$,%5%]!<%H$5$l$F$$$k$+$O(B, inquireBasicFunction() $B$K(B
      $B$h$C$FCN$k$3$H$,$G$-$k(B. */
    enum BasicFunction
    {
	Advanced_Feature_Inq	= (0x1u << 31),	//!< $B%+%a%i%Y%s%@0MB8$N5!G=(B
	I1394b_mode_Capability	= (0x1u << 23),	//!< 1394b$B%b!<%I(B
	Cam_Power_Cntl_Inq	= (0x1u << 15),	//!< $BEE8;(Bon/off$B$N@)8f(B
	One_Shot_Inq		= (0x1u << 12),	//!< $B2hA|(B1$BKg$@$1$N;#1F(B
	Multi_Shot_Inq		= (0x1u << 11)	//!< $B;XDj$5$l$?Kg?t$N;#1F(B
    };

  //! $B%+%a%i$,=PNO$9$k2hA|$N7A<0(B
    enum Format
    {
	YUV444_160x120	 = 0x200,	//!< Format_0_0: 160x120 YUV(4:4:4)
	YUV422_320x240	 = 0x204,	//!< Format_0_1: 320x240 YUV(4:2:2)
	YUV411_640x480	 = 0x208,	//!< Format_0_2: 640x480 YUV(4:1:1)
	YUV422_640x480	 = 0x20c,	//!< Format_0_3: 640x480 YUV(4:2:2)
	RGB24_640x480	 = 0x210,	//!< Format_0_4: 640x480 RGB
	MONO8_640x480	 = 0x214,	//!< Format_0_5: 640x480 Y(mono)
	MONO16_640x480	 = 0x218,	//!< Format_0_6: 640x480 Y(mono16)
	YUV422_800x600	 = 0x220,	//!< Format_1_0: 800x600 YUV(4:2:2)
	RGB24_800x600	 = 0x224,	//!< Format_1_1: 800x600 RGB
	MONO8_800x600	 = 0x228,	//!< Format_1_2: 800x600 Y(mono)
	YUV422_1024x768	 = 0x22c,	//!< Format_1_3: 1024x768 YUV(4:2:2)
	RGB24_1024x768	 = 0x230,	//!< Format_1_4: 1024x768 RGB
	MONO8_1024x768	 = 0x234,	//!< Format_1_5: 1024x768 Y(mono)
	MONO16_800x600	 = 0x238,	//!< Format_1_6: 800x600 Y(mono16)
	MONO16_1024x768	 = 0x23c,	//!< Format_1_7: 1024x768 Y(mono16)
	YUV422_1280x960	 = 0x240,	//!< Format_2_0: 1280x960 YUV(4:2:2)
	RGB24_1280x960	 = 0x244,	//!< Format_2_1: 1280x960 RGB
	MONO8_1280x960	 = 0x248,	//!< Format_2_2: 1280x960 Y(mono)
	YUV422_1600x1200 = 0x24c,	//!< Format_2_3: 1600x1200 YUV(4:2:2)
	RGB24_1600x1200	 = 0x250,	//!< Format_2_4: 1600x1200 RGB
	MONO8_1600x1200	 = 0x254,	//!< Format_2_5: 1600x1200 Y(mono)
	MONO16_1280x960	 = 0x258,	//!< Format_2_6: 1280x960 Y(mono16)
	MONO16_1600x1200 = 0x25c,	//!< Format_2_7: 1600x1200 Y(mono16)
	Format_5_0	 = 0x2a0,	//!< Format_5_0: 
	MONO8_640x480x2	 = 0x2a4,	//!< Format_5_1: 640x480x2 Y(mono)
	Format_5_2	 = 0x2a8,	//!< Format_5_2: 
	Format_5_3	 = 0x2ac,	//!< Format_5_3: 
	Format_5_4	 = 0x2b0,	//!< Format_5_4: 
	Format_5_5	 = 0x2b4,	//!< Format_5_5: 
	Format_5_6	 = 0x2b8,	//!< Format_5_6: 
	Format_5_7	 = 0x2bc,	//!< Format_5_7: 
	Format_7_0	 = 0x2e0,	//!< Format_7_0: $B%+%a%i5!<o$K0MB8(B
	Format_7_1	 = 0x2e4,	//!< Format_7_1: $B%+%a%i5!<o$K0MB8(B
	Format_7_2	 = 0x2e8,	//!< Format_7_2: $B%+%a%i5!<o$K0MB8(B
	Format_7_3	 = 0x2ec,	//!< Format_7_3: $B%+%a%i5!<o$K0MB8(B
	Format_7_4	 = 0x2f0,	//!< Format_7_4: $B%+%a%i5!<o$K0MB8(B
	Format_7_5	 = 0x2f4,	//!< Format_7_5: $B%+%a%i5!<o$K0MB8(B
	Format_7_6	 = 0x2f8,	//!< Format_7_6: $B%+%a%i5!<o$K0MB8(B
	Format_7_7	 = 0x2fc,	//!< Format_7_7: $B%+%a%i5!<o$K0MB8(B
    };

  //! $B%+%a%i$N%U%l!<%`%l!<%H$rI=$9%S%C%H%^%C%W(B
  /*! $B$I$N$h$&$J%U%l!<%`%l!<%H$,%5%]!<%H$5$l$F$$$k$+$O(B, inquireFrameRate()
      $B$K$h$C$FCN$k$3$H$,$G$-$k(B. */
    enum FrameRate
    {
	FrameRate_1_875	= (0x1u << 31),	//!< 1.875fps
	FrameRate_3_75	= (0x1u << 30),	//!< 3.75fps
	FrameRate_7_5	= (0x1u << 29),	//!< 7.5fps
	FrameRate_15	= (0x1u << 28),	//!< 15fps
	FrameRate_30	= (0x1u << 27),	//!< 30fps
	FrameRate_60	= (0x1u << 26),	//!< 60fps
	FrameRate_120	= (0x1u << 25),	//!< 120fps
	FrameRate_240	= (0x1u << 24),	//!< 240fps
	FrameRate_x	= (0x1u << 23)	//!< $BFC<l$J%U%l!<%`%l!<%H(B
    };
    
  //! $B=PNO2hA|$N2hAG$N7A<0(B
    enum PixelFormat
    {
	MONO_8		= (0x1u << 31),	//!< Y(mono)	  8bit/pixel
	YUV_411		= (0x1u << 30),	//!< YUV(4:1:1)	 12bit/pixel
	YUV_422		= (0x1u << 29),	//!< YUV(4:2:2)	 16bit/pixel
	YUV_444		= (0x1u << 28),	//!< YUV(4:4:4)	 24bit/pixel
	RGB_24		= (0x1u << 27),	//!< RGB	 24bit/pixel
	MONO_16		= (0x1u << 26),	//!< Y(mono16)	 16bit/pixel
	RGB_48		= (0x1u << 25),	//!< RGB	 48bit/pixel
	SIGNED_MONO_16	= (0x1u << 24),	//!< SIGNED MONO 16bit/pixel
	SIGNED_RGB_48	= (0x1u << 23),	//!< SIGNED RGB	 48bit/pixel
	RAW_8		= (0x1u << 22),	//!< RAW	  8bit/pixel
	RAW_16		= (0x1u << 21)	//!< RAW	 16bit/pixel
    };

  //! $BCM$r@_Dj$G$-$k%+%a%i$NB0@-(B
    enum Feature
    {
	BRIGHTNESS	= 0x800,	//!< $BL@$k$5D4@0(B
	AUTO_EXPOSURE	= 0x804,	//!< $B<+F0O*=PD4@0(B
	SHARPNESS	= 0x808,	//!< $BA/L@$5D4@0(B
	WHITE_BALANCE	= 0x80c,	//!< $B%[%o%$%H%P%i%s%9D4@0(B
	HUE		= 0x810,	//!< $B?'$N?'AjD4@0(B
	SATURATION	= 0x814,	//!< $B?'$NK0OBEYD4@0(B
	GAMMA		= 0x818,	//!< $B51EY$N%,%s%^Jd@5D4@0(B
	SHUTTER		= 0x81c,	//!< $B%7%c%C%?!<%9%T!<%ID4@0(B
	GAIN		= 0x820,	//!< $B%2%$%sD4@0(B
	IRIS		= 0x824,	//!< $B%"%$%j%9D4@0(B
	FOCUS		= 0x828,	//!< $B%U%)!<%+%9D4@0(B
	TEMPERATURE	= 0x82c,	//!< $B?'29EYD4@0(B
	TRIGGER_MODE	= 0x830,	//!< $B30It%H%j%,?.9f$N%b!<%I(B
	TRIGGER_DELAY	= 0x834,	//!< $B%H%j%,F~NO$+$iO*8w3+;O$^$G$NCY$l(B
	FRAME_RATE	= 0x83c,	//!< $B%U%l!<%`%l!<%H$N<jF0@_Dj(B
	ZOOM		= 0x880,	//!< $B%:!<%`D4@0(B
	PAN		= 0x884,	//!< $B%Q%s(B($B:81&$N<s?6$j(B)$BD4@0(B
	TILT		= 0x888,	//!< $B%A%k%H(B($B>e2<$N<s?6$j(B)$BD4@0(B
	OPTICAL_FILTER	= 0x88c,
	CAPTURE_SIZE	= 0x8c0,
	CAPTURE_QUALITY	= 0x8c4
    };

  //! $B3FB0@-(B( Feature)$B$K$D$$$F%+%a%i$,%5%]!<%H$7$F$$$k5!G=$rI=$9%S%C%H%^%C%W(B
  /*! $B$I$N$h$&$J5!G=$,%5%]!<%H$5$l$F$$$k$+$O(B, inquireFeatureFunction() $B$K(B
      $B$h$C$FCN$k$3$H$,$G$-$k(B. */
    enum FeatureFunction
    {
	Presence	= (0x1u << 31),	//!< $B$3$NB0@-$=$N$b$N$r%5%]!<%H(B
      //Abs_Control	= (0x1u << 30),	//!< $B$3$NB0@-$rCM$K$h$C$F@)8f2DG=(B
	One_Push	= (0x1u << 28),	//!< one push$B%b!<%I$r%5%]!<%H(B
	ReadOut		= (0x1u << 27),	//!< $B$3$NB0@-$NCM$rFI$_=P$7$,2DG=(B
	OnOff		= (0x1u << 26),	//!< $B$3$NB0@-$N(Bon/off$B$,2DG=(B
	Auto		= (0x1u << 25),	//!< $B$3$NB0@-$NCM$N<+F0@_Dj$,2DG=(B
	Manual		= (0x1u << 24)	//!< $B$3$NB0@-$NCM$N<jF0@_Dj$,2DG=(B
    };
    
  //! $B%+%a%i$N30It%H%j%,!<%b!<%I(B
    enum TriggerMode
    {
	Trigger_Mode0	=  0,	//!< $B%H%j%,(Bon$B$+$i(B #SHUTTER $B$G;XDj$7$?;~4V$@$1C_@Q(B
	Trigger_Mode1	=  1,	//!< $B%H%j%,(Bon$B$+$i%H%j%,(Boff$B$K$J$k$^$GC_@Q(B
	Trigger_Mode2	=  2,
	Trigger_Mode3	=  3,
	Trigger_Mode4	=  4,
	Trigger_Mode5	=  5,
	Trigger_Mode14	= 14
    };

  //! $B%+%a%i$N30It%H%j%,!<?.9f$N6K@-(B
    enum TriggerPolarity
    {
	LowActiveInput	= 0,		//!< low$B$G%H%j%,(Bon
	HighActiveInput	= (0x1u << 24)	//!< high$B$G%H%j%,(Bon
    };

  //! $BK\%+%a%i$,%5%]!<%H$9$k(BFormat_7$B$K4X$9$k>pJs(B(getFormat_7_Info() $B$GF@$i$l$k(B)
    struct Format_7_Info
    {
	u_int		maxWidth;		//!< $B2hA|$N:GBgI}(B
	u_int		maxHeight;		//!< $B2hA|$N:GBg9b$5(B
	u_int		unitWidth;		//!< $B2hA|I}$N:G>.C10L(B
	u_int		unitHeight;		//!< $B2hA|9b$5$N:G>.C10L(B
	u_int		unitU0;			//!< $B86E@?eJ?0LCV;XDj$N:G>.C10L(B
	u_int		unitV0;			//!< $B86E@?bD>0LCV;XDj$N:G>.C10L(B
	u_int		u0;			//!< $B86E@?eJ?0LCV(B
	u_int		v0;			//!< $B86E@?bD>0LCV(B
	u_int		width;			//!< $B2hA|$NI}(B
	u_int		height;			//!< $B2hA|$N9b$5(B
	PixelFormat	pixelFormat;		//!< $B2hA|$N2hAG7A<0(B
	u_int		availablePixelFormats;	//!< $BMxMQ$G$-$k2hAG7A<0(B
    };
    
  //! Dragonfly(Pointgrey Research Inc.)$B$N(BBayer$B%Q%?!<%s(B
    enum Bayer
    {
	RGGB = 0x52474742,	//!< $B:8>e$+$i1&2<$K8~$+$C$F(BR, G, G, B
	BGGR = 0x42474752,	//!< $B:8>e$+$i1&2<$K8~$+$C$F(BB, G, G, R
	GRBG = 0x47524247,	//!< $B:8>e$+$i1&2<$K8~$+$C$F(BG, R, B, G
	GBRG = 0x47425247,	//!< $B:8>e$+$i1&2<$K8~$+$C$F(BG, B, R, G
	YYYY = 0x59595959	//!< $B$9$Y$F(BY(monochrome)
    };

  private:
    struct Mono16
    {
	operator u_char()		const	{return u_char(ntohs(s));}
	operator short()		const	{return ntohs(s);}
	operator u_short()		const	{return u_short(ntohs(s));}
	operator int()			const	{return int(ntohs(s));}
	operator u_int()		const	{return u_int(ntohs(s));}
	operator float()		const	{return float(ntohs(s));}
	operator double()		const	{return double(ntohs(s));}
	      
	short	s;
    };

  public:
    Ieee1394Camera(Type type=Monocular, bool i1394b=false,
		   u_int64_t uniqId=0, u_int delay=0)			;
    ~Ieee1394Camera()							;

  // Basic function stuffs.
    quadlet_t		inquireBasicFunction()			const	;
    Ieee1394Camera&	powerOn()					;
    Ieee1394Camera&	powerOff()					;
    Bayer		bayerTileMapping()			const	;
    bool		isLittleEndian()			const	;
    
  // Format and frame rate stuffs.
    quadlet_t		inquireFrameRate(Format format)		const	;
    Ieee1394Camera&	setFormatAndFrameRate(Format format,
					      FrameRate rate)		;
    Format		getFormat()				const	;
    FrameRate		getFrameRate()				const	;
    u_int		width()					const	;
    u_int		height()				const	;
    PixelFormat		pixelFormat()				const	;

  // Format_7 stuffs.
    Format_7_Info	getFormat_7_Info(Format format7)		;
    Ieee1394Camera&	setFormat_7_ROI(Format format7, 
					u_int u0, u_int v0,
					u_int width, u_int height)	;
    Ieee1394Camera&	setFormat_7_PixelFormat(Format format7, 
						PixelFormat pixelFormat);
    
  // Feature stuffs.
    quadlet_t		inquireFeatureFunction(Feature feature)	const	;
    Ieee1394Camera&	onePush(Feature feature)			;
    Ieee1394Camera&	turnOn(Feature feature)				;
    Ieee1394Camera&	turnOff(Feature feature)			;
    Ieee1394Camera&	setAutoMode(Feature feature)			;
    Ieee1394Camera&	setManualMode(Feature feature)			;
    Ieee1394Camera&	setValue(Feature feature, u_int value)		;
    bool		inOnePushOperation(Feature feature)	const	;
    bool		isTurnedOn(Feature feature)		const	;
    bool		isAuto(Feature feautre)			const	;
    void		getMinMax(Feature feature,
				  u_int& min, u_int& max)	const	;
    u_int		getValue(Feature feature)		const	;

  // White balance stuffs.
    Ieee1394Camera&	setWhiteBalance(u_int ub, u_int vr)		;
    void		getWhiteBalance(u_int& ub, u_int& vr)	const	;
    
  // Temperature stuffs.
    u_int		getAimedTemperature()			const	;
    
  // Trigger stuffs.
    Ieee1394Camera&	setTriggerMode(TriggerMode mode)		;
    TriggerMode		getTriggerMode()			const	;
    Ieee1394Camera&	setTriggerPolarity(TriggerPolarity polarity)	;
    TriggerPolarity	getTriggerPolarity()			const	;
    
  // Shotting stuffs.
    Ieee1394Camera&	continuousShot()				;
    Ieee1394Camera&	stopContinuousShot()				;
    bool		inContinuousShot()			const	;
    Ieee1394Camera&	oneShot()					;
    Ieee1394Camera&	multiShot(u_short nframes)			;

  // Configuration saving/restoring stuffs.
    Ieee1394Camera&	saveConfig(u_int mem_ch)			;
    Ieee1394Camera&	restoreConfig(u_int mem_ch)			;
    u_int		getMemoryChannelMax()			const	;

  // Capture stuffs.
    Ieee1394Camera&	snap()						;
#ifdef HAVE_LIBTUTOOLS__
    template <class T> const Ieee1394Camera&
			operator >>(Image<T>& image)		const	;
    template <class T> const Ieee1394Camera&
			captureRGBImage(Image<T>& image)	const	;
    template <class T> const Ieee1394Camera&
			captureDirectly(Image<T>& image)	const	;
#endif
    const Ieee1394Camera&
			captureRaw(void* image)			const	;
    const Ieee1394Camera&
			captureBayerRaw(void* image)		const	;

  // Utility functions.
    static Format	uintToFormat(u_int format)			;
    static FrameRate	uintToFrameRate(u_int frameRate)		;
    static Feature	uintToFeature(u_int feature)			;
    static TriggerMode	uintToTriggerMode(u_int triggerMode)		;
    static PixelFormat	uintToPixelFormat(u_int pixelFormat)		;

  private:
    nodeaddr_t	getFormat_7_BaseAddr(Format format7)		 const	;
    u_int	setFormat_7_PacketSize(Format format7)			;
    quadlet_t	inquireFrameRate_or_Format_7_Offset(Format format) const;
    bool	unlockAdvancedFeature(u_int64_t featureId,
				      u_int timeout)			;
    void	checkAvailability(Format format, FrameRate rate) const	;
    quadlet_t	checkAvailability(Feature feature, u_int inq)	 const	;
    void	checkAvailability(BasicFunction func)		 const	;
    quadlet_t	readQuadletFromRegister(u_int offset)		 const	;
    void	writeQuadletToRegister(u_int offset, quadlet_t quad)	;
    
    const nodeaddr_t	_cmdRegBase;
    u_int		_w, _h;		// width and height of current image format.
    PixelFormat		_p;		// pixel format of current image format.
    const u_char*	_img;		// currently available image data.
    u_int		_img_size;	// image data size.
    nodeaddr_t		_acr;		// access control register(ACR).
    Bayer		_bayer;		// Bayer pattern supported by this camera.
    bool		_littleEndian;	// true if MONO16 is in little endian format.
};

//! $B$3$N%+%a%i$,%5%]!<%H$9$k(BBayer$B%Q%?!<%s(B(#Bayer)$B$rJV$9(B
inline Ieee1394Camera::Bayer
Ieee1394Camera::bayerTileMapping() const
{
    return _bayer;
}

//! $B$3$N%+%a%i$N(B#MONO_16 $B%U%)!<%^%C%H$,(Blittle endian$B$G$"$k$+$rD4$Y$k(B
inline bool
Ieee1394Camera::isLittleEndian() const
{
    return _littleEndian;
}

//! $B8=:_@_Dj$5$l$F$$$k2hA|%U%)!<%^%C%H(B(#Format)$B$NI}$rJV$9(B
inline u_int
Ieee1394Camera::width() const
{
    return _w;
}

//! $B8=:_@_Dj$5$l$F$$$k2hA|%U%)!<%^%C%H(B(#Format)$B$N9b$5$rJV$9(B
inline u_int
Ieee1394Camera::height() const
{
    return _h;
}

//! $B8=:_@_Dj$5$l$F$$$k2hA|%U%)!<%^%C%H(B(#Format)$B$N2hAG7A<0(B(#PixelFormat)$B$rJV$9(B
inline Ieee1394Camera::PixelFormat
Ieee1394Camera::pixelFormat() const
{
    return _p;
}

//! $B%+%a%i$,%5%]!<%H$7$F$$$k4pK\5!G=$rJV$9(B
/*!
  \return	$B%5%]!<%H$5$l$F$$$k5!G=$r(B #BasicFunction $B7?$NNs5sCM$N(Bor$B$H$7$F(B
		$BJV$9(B
 */
inline quadlet_t
Ieee1394Camera::inquireBasicFunction() const
{
    return readQuadletFromRegister(0x400);
}

//! $B%+%a%i$+$i=PNO$5$l$k:G=i$N2hA|$rJ];}$9$k(B
/*!
  $B%+%a%i$+$i$N2hA|=PNO$O(B, continuousShot(), oneShot(), multiShot() $B$N(B
  $B$$$:$l$+$K$h$C$F9T$o$l$k(B. $B<B:]$K2hA|%G!<%?$,<u?.$5$l$k$^$G(B, $BK\4X?t$O(B
  $B8F$S=P$7B&$K@)8f$rJV$5$J$$(B. 
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H(B
 */
inline Ieee1394Camera&
Ieee1394Camera::snap()
{
    if (_img != 0)
	requeueListenBuffer();
    _img = waitListenBuffer();
    return *this;
}

#ifdef HAVE_LIBTUTOOLS__
//! IEEE1394$B%+%a%i$+$i=PNO$5$l$?2hA|$rD>@\E*$K<h$j9~$`(B
/*!
  #operator >>() $B$H$N0c$$$O(B, $B2hA|7A<0$NJQ49$r9T$o$J$$$3$H$H(B, Image<T> $B9=B$BN(B
  $B$NCf$N%G!<%?NN0h$X$N%]%$%s%?$r(BIEEE1394$BF~NO%P%C%U%!$X$N%]%$%s%?$K=q$-49$($k(B
  $B$3$H$K$h$C$F(B, $B<B:]$K$O%G!<%?$N%3%T!<$r9T$o$J$$$3$H$G$"$k(B. 
  $B%F%s%W%l!<%H%Q%i%a!<%?(BT$B$O(B, $B3JG<@h$N2hA|$N2hAG7A<0$rI=$9(B. $B$J$*(B, $BK\4X?t$r(B
  $B8F$S=P$9A0$K(B snap() $B$K$h$C$F%+%a%i$+$i$N2hA|$rJ];}$7$F$*$+$J$1$l$P$J$i$J$$(B. 
  \param image	$B2hA|%G!<%?$r3JG<$9$k2hA|%*%V%8%'%/%H(B. $B2hA|$NI}$H9b$5$O(B, 
		$B8=:_%+%a%i$K@_Dj$5$l$F$$$k2hA|%5%$%:$K9g$o$;$F<+F0E*$K(B
		$B@_Dj$5$l$k(B. 
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H(B
*/
template <class T> const Ieee1394Camera&
Ieee1394Camera::captureDirectly(Image<T>& image) const
{
    if (_img == 0)
	throw std::runtime_error("TU::Ieee1394Camera::captureDirectly: no images snapped!!");
    image.resize((T*)_img, height(), width());

    return *this;
}
#endif

inline void
Ieee1394Camera::checkAvailability(Format format, FrameRate rate) const
{
    using namespace	std;
    
    quadlet_t	quad = inquireFrameRate(format);
    if (!(quad & rate))
    {
	ostringstream	s;
	
	s << "Ieee1394Camera::checkAvailability: Incompatible combination of format[0x"
	  << hex << format << "] and frame rate[0x" << hex << rate << "]!!";
      	throw runtime_error(s.str().c_str());
    }
}

inline quadlet_t
Ieee1394Camera::checkAvailability(Feature feature, u_int inq) const
{
    using namespace	std;
    
    quadlet_t	quad = inquireFeatureFunction(feature);
    if ((quad & inq) != inq)
    {
	ostringstream	s;
	
	s << "Ieee1394Camera::checkAvailability: This feature[0x"
	  << hex << feature
	  << "] is not present or this field is unavailable (quad: 0x"
	  << hex << quad << ", inq: 0x" << hex << inq << ")!!";
      	throw runtime_error(s.str().c_str());
    }
    return quad;
}

inline void
Ieee1394Camera::checkAvailability(BasicFunction func) const
{
    using namespace	std;

    quadlet_t	quad = inquireBasicFunction();
    if (!(quad & func))
    {
	ostringstream	s;

	s << "Ieee1394Camera::checkAvailabilityOfBasicFuntion: This fucntion is not present (quad: 0x"
	  << hex << quad << ", func: " << hex << func << ")!!";
      	throw runtime_error(s.str().c_str());
    }
}

inline quadlet_t
Ieee1394Camera::readQuadletFromRegister(u_int offset) const
{
    return readQuadlet(_cmdRegBase + offset);
}

inline void
Ieee1394Camera::writeQuadletToRegister(u_int offset, quadlet_t quad)
{
    writeQuadlet(_cmdRegBase + offset, quad);
}
 
/************************************************************************
*  global functions							*
************************************************************************/
std::ostream&	operator <<(std::ostream& out, const Ieee1394Camera& camera);
std::istream&	operator >>(std::istream& in, Ieee1394Camera& camera);

}
#endif	/* !__TUIeee1394PP_h	*/
