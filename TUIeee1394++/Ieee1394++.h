/*
 *  $Id: Ieee1394++.h,v 1.8 2002-12-09 08:02:30 ueshiba Exp $
 */
#ifndef __TUIeee1394PP_h
#define __TUIeee1394PP_h

#include <libraw1394/raw1394.h>
#include <libraw1394/csr.h>
#include <video1394.h>
#include <netinet/in.h>
#ifdef HAVE_TUToolsPP
#  include "TU/Image++.h"
#else
#  include <iostream>
typedef unsigned long long	u_int64;
#endif

namespace TU
{
/************************************************************************
*  class Ieee1394Port							*
************************************************************************/
class Ieee1394Node;

/*!
  IEEE1394$B%]!<%H(B(PC$B$K%$%s%9%H!<%k$5$l$?(BIEEE1394$B%$%s%?!<%U%'!<%9%+!<%I(B
  $B$,BeI=E*(B)$B$rI=$9%/%i%9!%(B
*/
class Ieee1394Port
{
  public:
    Ieee1394Port(int port_number, u_int delay=1)	;
    ~Ieee1394Port()					;

    u_int		nnodes()		const	;
    nodeid_t		nodeId()		const	;
    u_int		delay()			const	;
    Ieee1394Port&	setDelay(u_int delay)		;

  private:
    Ieee1394Port(const Ieee1394Port&)			;
    Ieee1394Port&	operator =(const Ieee1394Port&)	;

    raw1394handle_t	handle()		const	{return _handle;}
    int			fd()			const	{return _fd;}
    
    void	registerNode(const Ieee1394Node& node)			;
    void	unregisterNode(const Ieee1394Node& node)		;
    bool	isRegisteredNode(const Ieee1394Node& node)	const 	;
    
    const raw1394handle_t	_handle;
    const int			_fd;	  // file desc. for video1394 device.
    const Ieee1394Node*		_node[63];
    u_int			_delay;	  // delay for read/write registers.

    friend class	Ieee1394Node;
};

//! $B%]!<%H$K@\B3$5$l$F$$$k%N!<%I(B($B$3$N%]!<%H<+?H$b4^$`(B)$B$N?t$rJV$9(B
inline u_int
Ieee1394Port::nnodes() const
{
    return raw1394_get_nodecount(_handle);
}

//! $B$3$N%]!<%H$N%N!<%I(BID$B$rJV$9(B
inline nodeid_t
Ieee1394Port::nodeId() const
{
    return raw1394_get_local_id(_handle);
}

//! $B%]!<%H$K@_Dj$5$l$F$$$kCY1d;~4V$rJV$9(B
/*!
  $BCY1d;~4V$K$D$$$F$O(BIeee1394Port()$B$b;2>H!%(B
  \return	$BCY1d;~4V(B($BC10L(B: micro second)
*/
inline u_int
Ieee1394Port::delay()	const
{
    return _delay;
}

//! $B%]!<%H$KCY1d;~4V$r@_Dj$9$k(B
/*!
  $BCY1d;~4V$K$D$$$F$O(BIeee1394Port()$B$b;2>H!%(B
  \param delay	$BCY1d;~4V(B($BC10L(B: micro second)
  \return	$B$3$N(BIEEE1394$B%]!<%H%*%V%8%'%/%H(B
*/
inline Ieee1394Port&
Ieee1394Port::setDelay(u_int delay)
{
    _delay = delay;
    return *this;
}

/************************************************************************
*  class Ieee1394Node							*
************************************************************************/
/*!
  IEEE1394$B$N%N!<%I$rI=$9%/%i%9!%0lHL$K$O!$$h$j6qBNE*$J5!G=$r;}$C$?%N!<%I(B
  (ex. $B%G%8%?%k%+%a%i(B)$B$rI=$9%/%i%9$N4pDl%/%i%9$H$7$FMQ$$$i$l$k!%(B
*/
class Ieee1394Node
{
  protected:
  //! isochronous$BE>Aw$NB.EY(B
    enum Speed
    {
	SPD_100M = 0,				//!< 100Mbps
	SPD_200M = 1,				//!< 200Mbps
	SPD_400M = 2				//!< 400Mbps
    };

  public:
  //! $B$3$N%N!<%I$N(BID(IEEE1394 bus$B>e$N%"%I%l%9(B)$B$rJV$9(B
  /*!
    \return	$B$3$N%N!<%I$N(BID$B!%(B
   */
    nodeid_t	nodeId()			const	{return _nodeId;}
  //! $B$3$N%N!<%I$K3d$jEv$F$i$l$?(Bisochronous$B<u?.MQ%P%C%U%!$N%5%$%:$rJV$9(B
  /*!
    \return	$B<u?.MQ%P%C%U%!(B1$B$D$"$?$j$N%5%$%:(B($BC10L(B: byte)
   */
    u_int	bufferSize()			const	{return _buf_size;}
    u_int64	globalUniqueId()		const	;
    
  protected:
    Ieee1394Node(Ieee1394Port& prt, u_int unit_spec_ID,
		 u_int channel, int sync_tag, int flag, u_int64 uniqId)	;
    virtual		~Ieee1394Node()					;

    u_int		readValueFromUnitDependentDirectory(u_char key)
								const	;
    quadlet_t		readQuadlet(nodeaddr_t addr)		const	;
    void		writeQuadlet(nodeaddr_t addr, quadlet_t quad)	;
    void		mapListenBuffer(size_t packet_size,
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
    
    Ieee1394Port&	_port;
    nodeid_t		_nodeId;
    video1394_mmap	_mmap;		// mmap structure for video1394.
    u_int		_buf_size;	// buffer size excluding header.
    u_char*		_buf;		// addr. of mapped buffer.
    u_int		_current;	// index of current ready buffer.
    u_int		_nready;	// # of ready buffers.
};

inline quadlet_t
Ieee1394Node::readQuadletFromConfigROM(u_int offset) const
{
    return readQuadlet(CSR_REGISTER_BASE + CSR_CONFIG_ROM + offset);
}

/************************************************************************
*  class Ieee1394Camera							*
************************************************************************/
/*!
  1394-based Digital Camera Specification ver. 1.30$B$K=`5r$7$?%G%8%?%k%+%a%i(B
  $B$rI=$9%/%i%9!%(B
*/
class Ieee1394Camera : public Ieee1394Node
{
  public:
  //! $B%+%a%i$,%5%]!<%H$7$F$$$k4pK\5!G=$rI=$9%S%C%H%^%C%W(B
  /*! $B$I$N$h$&$J4pK\5!G=$,%5%]!<%H$5$l$F$$$k$+$O!$(BinquireBasicFunction()$B$K(B
      $B$h$C$FCN$k$3$H$,$G$-$k!%(B*/
    enum BasicFunction
    {
	Advanced_Feature_Inq	= (0x1 << 31),	//!< $B%+%a%i%Y%s%@0MB8$N5!G=(B
	Cam_Power_Cntl_Inq	= (0x1 << 15),	//!< $BEE8;(Bon/off$B$N@)8f(B
	One_Shot_Inq		= (0x1 << 12),	//!< $B2hA|(B1$BKg$@$1$N;#1F(B
	Multi_Shot_Inq		= (0x1 << 11)	//!< $B;XDj$5$l$?Kg?t$N;#1F(B
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
	Format_7_0	 = 0x2e0,	//!< Format_7_0: $B%+%a%i5!<o$K0MB8(B
	Format_7_1	 = 0x2e4,	//!< Format_7_1: $B%+%a%i5!<o$K0MB8(B
	Format_7_2	 = 0x2e8,	//!< Format_7_2: $B%+%a%i5!<o$K0MB8(B
	Format_7_3	 = 0x2ec,	//!< Format_7_3: $B%+%a%i5!<o$K0MB8(B
	Format_7_4	 = 0x2f0,	//!< Format_7_4: $B%+%a%i5!<o$K0MB8(B
	Format_7_5	 = 0x2f4,	//!< Format_7_5: $B%+%a%i5!<o$K0MB8(B
	Format_7_6	 = 0x2f8,	//!< Format_7_6: $B%+%a%i5!<o$K0MB8(B
	Format_7_7	 = 0x2fc	//!< Format_7_7: $B%+%a%i5!<o$K0MB8(B
    };

  //! $B%+%a%i$N%U%l!<%`%l!<%H$rI=$9%S%C%H%^%C%W(B
  /*! $B$I$N$h$&$J%U%l!<%`%l!<%H$,%5%]!<%H$5$l$F$$$k$+$O!$(BinquireFrameRate()
      $B$K$h$C$FCN$k$3$H$,$G$-$k!%(B*/
    enum FrameRate
    {
	FrameRate_1_875	= (0x1 << 31),	//!< 1.875fps
	FrameRate_3_75	= (0x1 << 30),	//!< 3.75fps
	FrameRate_7_5	= (0x1 << 29),	//!< 7.5fps
	FrameRate_15	= (0x1 << 28),	//!< 15fps
	FrameRate_30	= (0x1 << 27),	//!< 30fps
	FrameRate_60	= (0x1 << 26),	//!< 60fps
	FrameRate_x	= (0x1 << 25)	//!< $BFC<l$J%U%l!<%`%l!<%H(B
    };
    
  //! $B=PNO2hA|$N2hAG$N7A<0(B
    enum PixelFormat
    {
	MONO_8		= (0x1 << 31),	//!< Y(mono)	 8bit/pixel
	YUV_411		= (0x1 << 30),	//!< YUV(4:1:1)	12bit/pixel
	YUV_422		= (0x1 << 29),	//!< YUV(4:2:2)	16bit/pixel
	YUV_444		= (0x1 << 28),	//!< YUV(4:4:4)	24bit/pixel
	RGB_24		= (0x1 << 27),	//!< RGB	24bit/pixel
	MONO_16		= (0x1 << 26),	//!< Y(mono16)	16bit/pixel
	RGB_48		= (0x1 << 25)	//!< RGB	48bit/pixel
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
	ZOOM		= 0x880,	//!< $B%:!<%`D4@0(B
	PAN		= 0x884,	//!< $B%Q%s(B($B:81&$N<s?6$j(B)$BD4@0(B
	TILT		= 0x888,	//!< $B%A%k%H(B($B>e2<$N<s?6$j(B)$BD4@0(B
	OPTICAL_FILTER	= 0x88c,
	CAPTURE_SIZE	= 0x8c0,
	CAPTURE_QUALITY	= 0x8c4
    };

  //! $B3FB0@-(B(#Feature)$B$K$D$$$F%+%a%i$,%5%]!<%H$7$F$$$k5!G=$rI=$9%S%C%H%^%C%W(B
  /*! $B$I$N$h$&$J5!G=$,%5%]!<%H$5$l$F$$$k$+$O!$(BinquireFeatureFunction()$B$K$h$C(B
      $B$FCN$k$3$H$,$G$-$k!%(B*/
    enum FeatureFunction
    {
	Presence	= (0x1 << 31),	//!< $B$3$NB0@-$=$N$b$N$r%5%]!<%H(B
      //Abs_Control	= (0x1 << 30),	//!< $B$3$NB0@-$rCM$K$h$C$F@)8f2DG=(B
	One_Push	= (0x1 << 28),	//!< one push$B%b!<%I$r%5%]!<%H(B
	ReadOut		= (0x1 << 27),	//!< $B$3$NB0@-$NCM$rFI$_=P$7$,2DG=(B
	OnOff		= (0x1 << 26),	//!< $B$3$NB0@-$N(Bon/off$B$,2DG=(B
	Auto		= (0x1 << 25),	//!< $B$3$NB0@-$NCM$N<+F0@_Dj$,2DG=(B
	Manual		= (0x1 << 24)	//!< $B$3$NB0@-$NCM$N<jF0@_Dj$,2DG=(B
    };
    
  //! $B%+%a%i$N30It%H%j%,!<%b!<%I(B
    enum TriggerMode
    {
	Trigger_Mode0	= 0,	//!< $B%H%j%,(Bon$B$+$i(B#SHUTTER$B$G;XDj$7$?;~4V$@$1C_@Q(B
	Trigger_Mode1	= 1,	//!< $B%H%j%,(Bon$B$+$i%H%j%,(Boff$B$K$J$k$^$GC_@Q(B
	Trigger_Mode2	= 2,
	Trigger_Mode3	= 3
    };

  //! $B%+%a%i$N30It%H%j%,!<?.9f$N6K@-(B
    enum TriggerPolarity
    {
	LowActiveInput	= 0,		//!< low$B$G%H%j%,(Bon
	HighActiveInput	= (0x1 << 24)	//!< high$B$G%H%j%,(Bon
    };

  //! $BK\%+%a%i$,%5%]!<%H$9$k(BFormat_7$B$K4X$9$k>pJs(B(getFormat_7_Info()$B$GF@$i$l$k(B)
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
    
  private:
    struct Mono16
    {
	operator u_char()		const	{return u_char(ntohs(s));}
	operator short()		const	{return ntohs(s);}
	operator float()		const	{return float(ntohs(s));}
	operator double()		const	{return double(ntohs(s));}
	      
	short	s;
    };

  public:
    Ieee1394Camera(Ieee1394Port& port, u_int channel=0,
		   u_int64 uniqId=0)					;
    virtual ~Ieee1394Camera()						;

  // Power ON/OFF stuffs.
    quadlet_t		inquireBasicFunction()			const	;
    Ieee1394Camera&	powerOn()					;
    Ieee1394Camera&	powerOff()					;

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
    Format_7_Info	getFormat_7_Info(Format format7)	const	;
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
#ifdef HAVE_TUToolsPP
    template <class T> const Ieee1394Camera&
			operator >>(Image<T>& image)		const	;
#endif
    const Ieee1394Camera&
			captureRaw(void* image)			const	;

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
    void	checkAvailability(Format format, FrameRate rate) const	;
    quadlet_t	checkAvailability(Feature feature, u_int inq)	 const	;
    void	checkAvailability(BasicFunction func)		 const	;
    quadlet_t	readQuadletFromRegister(u_int offset)		 const	;
    void	writeQuadletToRegister(u_int offset, quadlet_t quad)	;
    
    const nodeaddr_t	_cmdRegBase;
    u_int		_w, _h;	// width and height of current image format.
    PixelFormat		_p;	// pixel format of current image format.
    const u_char*	_buf;	// currently available image buffer.
};

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
  \return	$B%5%]!<%H$5$l$F$$$k5!G=$r(B#BasicFunction$B7?$NNs5sCM$N(Bor$B$H$7$F(B
		$BJV$9!%(B
 */
inline quadlet_t
Ieee1394Camera::inquireBasicFunction() const
{
    return readQuadletFromRegister(0x400);
}

//! $B%+%a%i$+$i=PNO$5$l$k:G=i$N2hA|$rJ];}$9$k!%(B
/*!
  $B%+%a%i$+$i$N2hA|=PNO$O!$(BcontinuousShot(), oneShot(), multiShot()$B$N$$$:$l$+(B
  $B$K$h$C$F9T$o$l$k!%<B:]$K2hA|%G!<%?$,<u?.$5$l$k$^$G!$K\4X?t$O8F$S=P$7B&$K(B
  $B@)8f$rJV$5$J$$!%(B
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
 */
inline Ieee1394Camera&
Ieee1394Camera::snap()
{
    if (_buf != 0)
	requeueListenBuffer();
    _buf = waitListenBuffer();
    return *this;
}

inline void
Ieee1394Camera::checkAvailability(Format format, FrameRate rate) const
{
    using namespace	std;
    
    quadlet_t	quad = inquireFrameRate(format);
    if (!(quad & rate))
	cerr << "Ieee1394Camera::checkAvailability: Incompatible combination of format[0x"
	     << hex << format << "] and frame rate[0x" << rate << "]!!"
	     << endl;
  //	throw TUExceptionWithMessage("Ieee1394Camera::checkAvailability: Incompatible combination of frame rate and format!!");
}

inline quadlet_t
Ieee1394Camera::checkAvailability(Feature feature, u_int inq) const
{
    using namespace	std;
    
    quadlet_t	quad = inquireFeatureFunction(feature);
    if ((quad & inq) != inq)
	cerr << "Ieee1394Camera::checkAvailability: This feature[0x"
	     << hex << feature
	     << "] is not present or this field is not available (quad: 0x"
	     << quad << ", inq: " << inq << ")!!"
	     << endl;
  //	throw TUExceptionWithMessage("Ieee1394Camera::checkAvailability: This feature is not present or this field is not available!!");
    return quad;
}

inline void
Ieee1394Camera::checkAvailability(BasicFunction func) const
{
    using namespace	std;

    quadlet_t	quad = inquireBasicFunction();
    if (!(quad & func))
	cerr << "Ieee1394Camera::checkAvailabilityOfBasicFuntion: This fucntion is not present (quad: 0x"
	     << hex << quad << ", func: " << func << ")!!"
	     << endl;
  //	throw TUExceptionWithMessage("Ieee1394Camera::checkAvailabilityOfBasicFunction: This function is not present!!");
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
 
}
#endif	/* !__TUIeee1394PP_h	*/
