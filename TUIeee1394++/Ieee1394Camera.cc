/*
 *  $Id: Ieee1394Camera.cc,v 1.6 2002-10-04 01:53:45 ueshiba Exp $
 */
#include "TU/Ieee1394++.h"
#include <stdexcept>

namespace TU
{
/************************************************************************
*  local constants							*
************************************************************************/
static const u_int	unit_spec_ID	= 0x00a02d;

static const u_int	V_FORMAT_INQ	= 0x100;

static const quadlet_t	Format_0	= 0x1 << 31;
static const quadlet_t	Format_1	= 0x1 << 30;
static const quadlet_t	Format_2	= 0x1 << 29;
static const quadlet_t	Format_6	= 0x1 << 25;
static const quadlet_t	Format_7	= 0x1 << 24;

static const u_int	V_MODE_INQ_0	= 0x180;
static const u_int	V_MODE_INQ_1	= 0x184;
static const u_int	V_MODE_INQ_2	= 0x188;
static const u_int	V_MODE_INQ_6	= 0x198;
static const u_int	V_MODE_INQ_7	= 0x19c;

static const quadlet_t	Mode_0		= 0x1 << 31;
static const quadlet_t	Mode_1		= 0x1 << 30;
static const quadlet_t	Mode_2		= 0x1 << 29;
static const quadlet_t	Mode_3		= 0x1 << 28;
static const quadlet_t	Mode_4		= 0x1 << 27;
static const quadlet_t	Mode_5		= 0x1 << 26;
static const quadlet_t	Mode_6		= 0x1 << 25;
static const quadlet_t	Mode_7		= 0x1 << 26;

static const u_int	Feature_Hi_Inq	= 0x404;
static const u_int	Feature_Lo_Inq	= 0x408;

static const u_int	Cur_V_Frm_Rate	= 0x600;
static const u_int	Cur_V_Mode	= 0x604;
static const u_int	Cur_V_Format	= 0x608;
static const u_int	ISO_Channel	= 0x60c;
static const u_int	Camera_Power	= 0x610;
static const u_int	ISO_EN		= 0x614;
static const u_int	Memory_Save	= 0x618;
static const u_int	One_Shot	= 0x61c;
static const u_int	Mem_Save_Ch	= 0x620;
static const u_int	Cur_Mem_Ch	= 0x624;

static const quadlet_t	One_Push	= 0x1 << 26;
static const quadlet_t	ON_OFF		= 0x1 << 25;
static const quadlet_t	A_M_Mode	= 0x1 << 24;

static const quadlet_t	Polarity_Inq	= 0x1 << 25;

// NOTE: Two buffers are not enough under kernel-2.4.6 (2001.8.24).
static const u_int	NBUFFERS	= 4;

/************************************************************************
*  class Ieee1394Camera							*
************************************************************************/
//! IEEE1394$B%+%a%i%N!<%I$r@8@.$9$k(B
/*!
  \param prt		$B$3$N%+%a%i$,@\B3$5$l$F$$$k%]!<%H!%(B
  \param channel	isochronous$BE>AwMQ$N%A%c%M%kHV9f(B(\f$0 \leq
			\f$channel\f$ < 64\f$)
  \param uniqId		$B8D!9$N%+%a%i8GM-$N(B64bit ID$B!%F10l$N(BIEEE1394 bus$B$K(B
			$BJ#?t$N%+%a%i$,@\B3$5$l$F$$$k>l9g!$$3$l$K$h$C$F(B
			$BF1Dj$r9T$&!%(B0$B$,M?$($i$l$k$H!$$^$@(B#Ieee1394Camera
			$B%*%V%8%'%/%H$r3d$jEv$F$i$l$F$$$J$$%+%a%i$N$&$A!$(B
			$B0lHV:G=i$K$_$D$+$C$?$b$N$,$3$N%*%V%8%'%/%H$H7k$S(B
			$B$D$1$i$l$k!%%*%V%8%'%/%H@8@.8e$O!$(BglobalUniqueId()
			$B$K$h$C$F$3$NCM$rCN$k$3$H$,$G$-$k!%(B
*/
Ieee1394Camera::Ieee1394Camera(Ieee1394Port& prt, u_int ch, u_int64 uniqId)
    :Ieee1394Node(prt, unit_spec_ID, ch, 1, VIDEO1394_SYNC_FRAMES, uniqId),
     _cmdRegBase(CSR_REGISTER_BASE
		 + 4 * int64_t(readValueFromUnitDependentDirectory(0x40))),
     _w(0), _h(0), _p(MONO), _buf(0)
{
  // Assign IsoChannel to this camera.
    writeQuadletToRegister(ISO_Channel,
			   (ch << 28)|(Ieee1394Node::SPD_400M << 24));

  // Map video1394 buffer according to current format and frame rate.
    setFormatAndFrameRate(getFormat(), getFrameRate());
}

//! IEEE1394$B%+%a%i%*%V%8%'%/%H$rGK2u$9$k(B
/*!
  $B2hA|%G!<%?=PNOCf$G$"$C$?>l9g$O!$$=$l$rDd;_$9$k!%(B
*/
Ieee1394Camera::~Ieee1394Camera()
{
    stopContinuousShot();
}

//! IEEE1394$B%+%a%i$NEE8;$r(Bon$B$K$9$k(B
/*!
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::powerOn()
{
    checkAvailability(Cam_Power_Cntl_Inq);
    writeQuadletToRegister(Camera_Power, 0x1 << 31);
    return *this;
}

//! IEEE1394$B%+%a%i$NEE8;$r(Boff$B$K$9$k(B
/*!
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::powerOff()
{
    checkAvailability(Cam_Power_Cntl_Inq);
    writeQuadletToRegister(Camera_Power, 0x00000000);
    return *this;
}

//! $B;XDj$5$l$?2hA|%U%)!<%^%C%H$K$*$$$F%5%]!<%H$5$l$F$$$k%U%l!<%`%l!<%H$rD4$Y$k(B
/*!
  \param format	$BBP>]$H$J$k%U%)!<%^%C%H!%(B
  \return	$B%5%]!<%H$5$l$F$$$k%U%l!<%`%l!<%H$r(B#FrameRate$B7?$NNs5sCM(B
		$B$N(Bor$B$H$7$FJV$9!%;XDj$5$l$?%U%)!<%^%C%H<+BN$,$3$N%+%a%i$G%5(B
		$B%]!<%H$5$l$F$$$J$1$l$P!$(B0$B$,JV$5$l$k!%(B
*/
quadlet_t
Ieee1394Camera::inquireFrameRate(Format format) const
{
    quadlet_t	inq = 0;

    switch (format)	// Check presence of format.
    {
      case YUV444_160x120:
      case YUV422_320x240:
      case YUV411_640x480:
      case YUV422_640x480:
      case RGB24_640x480:
      case MONO8_640x480:
      case MONO16_640x480:
	inq = readQuadletFromRegister(V_FORMAT_INQ) & Format_0;
	break;
      case YUV422_800x600:
      case RGB24_800x600:
      case MONO8_800x600:
      case YUV422_1024x768:
      case RGB24_1024x768:
      case MONO8_1024x768:
      case MONO16_800x600:
      case MONO16_1024x768:
	inq = readQuadletFromRegister(V_FORMAT_INQ) & Format_1;
	break;
      case YUV422_1280x960:
      case RGB24_1280x960:
      case MONO8_1280x960:
      case YUV422_1600x1200:
      case RGB24_1600x1200:
      case MONO8_1600x1200:
      case MONO16_1280x960:
      case MONO16_1600x1200:
	inq = readQuadletFromRegister(V_FORMAT_INQ) & Format_2;
	break;
      case Format_7_0:
      case Format_7_1:
      case Format_7_2:
      case Format_7_3:
      case Format_7_4:
      case Format_7_5:
      case Format_7_6:
      case Format_7_7:
	inq = readQuadletFromRegister(V_FORMAT_INQ) & Format_7;
	break;
    }
    if (inq == 0)
	return 0;

    inq = 0;
    switch (format)	// Check presence of mode.
    {
      case YUV444_160x120:
	inq = readQuadletFromRegister(V_MODE_INQ_0) & Mode_0;
	break;
      case YUV422_320x240:
	inq = readQuadletFromRegister(V_MODE_INQ_0) & Mode_1;
	break;
      case YUV411_640x480:
	inq = readQuadletFromRegister(V_MODE_INQ_0) & Mode_2;
	break;
      case YUV422_640x480:
	inq = readQuadletFromRegister(V_MODE_INQ_0) & Mode_3;
	break;
      case RGB24_640x480:
	inq = readQuadletFromRegister(V_MODE_INQ_0) & Mode_4;
	break;
      case MONO8_640x480:
	inq = readQuadletFromRegister(V_MODE_INQ_0) & Mode_5;
	break;
      case MONO16_640x480:
	inq = readQuadletFromRegister(V_MODE_INQ_0) & Mode_6;
	break;
      case YUV422_800x600:
	inq = readQuadletFromRegister(V_MODE_INQ_1) & Mode_0;
	break;
      case RGB24_800x600:
	inq = readQuadletFromRegister(V_MODE_INQ_1) & Mode_1;
	break;
      case MONO8_800x600:
	inq = readQuadletFromRegister(V_MODE_INQ_1) & Mode_2;
	break;
      case YUV422_1024x768:
	inq = readQuadletFromRegister(V_MODE_INQ_1) & Mode_3;
	break;
      case RGB24_1024x768:
	inq = readQuadletFromRegister(V_MODE_INQ_1) & Mode_4;
	break;
      case MONO8_1024x768:
	inq = readQuadletFromRegister(V_MODE_INQ_1) & Mode_5;
	break;
      case MONO16_800x600:
	inq = readQuadletFromRegister(V_MODE_INQ_1) & Mode_6;
	break;
      case MONO16_1024x768:
	inq = readQuadletFromRegister(V_MODE_INQ_1) & Mode_7;
	break;
      case YUV422_1280x960:
	inq = readQuadletFromRegister(V_MODE_INQ_2) & Mode_0;
	break;
      case RGB24_1280x960:
	inq = readQuadletFromRegister(V_MODE_INQ_2) & Mode_1;
	break;
      case MONO8_1280x960:
	inq = readQuadletFromRegister(V_MODE_INQ_2) & Mode_2;
	break;
      case YUV422_1600x1200:
	inq = readQuadletFromRegister(V_MODE_INQ_2) & Mode_3;
	break;
      case RGB24_1600x1200:
	inq = readQuadletFromRegister(V_MODE_INQ_2) & Mode_4;
	break;
      case MONO8_1600x1200:
	inq = readQuadletFromRegister(V_MODE_INQ_2) & Mode_5;
	break;
      case MONO16_1280x960:
	inq = readQuadletFromRegister(V_MODE_INQ_2) & Mode_6;
	break;
      case MONO16_1600x1200:
	inq = readQuadletFromRegister(V_MODE_INQ_2) & Mode_7;
	break;
      case Format_7_0:
	inq = readQuadletFromRegister(V_MODE_INQ_7) & Mode_0;
	break;
      case Format_7_1:
	inq = readQuadletFromRegister(V_MODE_INQ_7) & Mode_1;
	break;
      case Format_7_2:
	inq = readQuadletFromRegister(V_MODE_INQ_7) & Mode_2;
	break;
      case Format_7_3:
	inq = readQuadletFromRegister(V_MODE_INQ_7) & Mode_3;
	break;
      case Format_7_4:
	inq = readQuadletFromRegister(V_MODE_INQ_7) & Mode_4;
	break;
      case Format_7_5:
	inq = readQuadletFromRegister(V_MODE_INQ_7) & Mode_5;
	break;
      case Format_7_6:
	inq = readQuadletFromRegister(V_MODE_INQ_7) & Mode_6;
	break;
      case Format_7_7:
	inq = readQuadletFromRegister(V_MODE_INQ_7) & Mode_7;
	break;
    }
    if (inq == 0)
	return 0;
    
    return readQuadletFromRegister(format);
}

//! $B2hA|%U%)!<%^%C%H$H%U%l!<%`%l!<%H$r@_Dj$9$k(B
/*!
  $B2hA|%G!<%?$r=PNOCf$G$"$C$?>l9g$O$=$l$rDd;_$7$F@_Dj$r9T$&$,!$$=$l$,(B
  $B40N;$9$l$P=PNO$r:F3+$9$k!%(B
  \param format	$B@_Dj$7$?$$2hA|%U%)!<%^%C%H!%(B
  \param rate	$B@_Dj$7$?$$%U%l!<%`%l!<%H!%(B
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::setFormatAndFrameRate(Format format, FrameRate rate)
{
    checkAvailability(format, rate);
    const u_int	fmt  = (u_int(format) - u_int(YUV444_160x120)) / 0x20,
		mode = (u_int(format) - u_int(YUV444_160x120)) % 0x20 / 4;
    const bool	cont = inContinuousShot();
    
    if (cont)
	stopContinuousShot();
    u_int	rt = 0;
    for (u_int bit = FrameRate_1_875; bit != rate; bit >>= 1)
	++rt;
    writeQuadletToRegister(Cur_V_Frm_Rate, rt   << 29);
    writeQuadletToRegister(Cur_V_Mode,	   mode << 29);
    writeQuadletToRegister(Cur_V_Format,   fmt  << 29);

  // Change buffer size and listen to the channel.
    u_int	packet_size = 0;
    switch (format)
    {
      case YUV444_160x120:
	_w = 160;
	_h = 120;
	_p = YUV_444;
	packet_size = 120 * sizeof(quadlet_t);
	break;
      case YUV422_320x240:
	_w = 320;
	_h = 240;
	_p = YUV_422;
	packet_size = 320 * sizeof(quadlet_t);
	break;
      case YUV411_640x480:
	_w = 640;
	_h = 480;
	_p = YUV_411;
	packet_size = 960 * sizeof(quadlet_t);
	break;
      case YUV422_640x480:
	_w = 640;
	_h = 480;
	_p = YUV_422;
	packet_size = 1280 * sizeof(quadlet_t);
	break;
      case RGB24_640x480:
	_w = 640;
	_h = 480;
	_p = RGB_24;
	packet_size = 1920 * sizeof(quadlet_t);
	break;
      case MONO8_640x480:
	_w = 640;
	_h = 480;
	_p = MONO;
	packet_size = 640 * sizeof(quadlet_t);
	break;
      case MONO16_640x480:
	_w = 640;
	_h = 480;
	_p = MONO_16;
	packet_size = 1280 * sizeof(quadlet_t);
	break;
      case YUV422_800x600:
	_w = 800;
	_h = 600;
	_p = YUV_422;
	packet_size = 2000 * sizeof(quadlet_t);
	break;
      case RGB24_800x600:
	_w = 800;
	_h = 600;
	_p = RGB_24;
	packet_size = 3000 * sizeof(quadlet_t);
	break;
      case MONO8_800x600:
	_w = 800;
	_h = 600;
	_p = MONO;
	packet_size = 1000 * sizeof(quadlet_t);
	break;
      case YUV422_1024x768:
	_w = 1024;
	_h = 768;
	_p = YUV_422;
	packet_size = 3072 * sizeof(quadlet_t);
	break;
      case RGB24_1024x768:
	_w = 1024;
	_h = 768;
	_p = RGB_24;
	packet_size = 4608 * sizeof(quadlet_t);
	break;
      case MONO8_1024x768:
	_w = 1024;
	_h = 768;
	_p = MONO;
	packet_size = 1536 * sizeof(quadlet_t);
	break;
      case MONO16_800x600:
	_w = 800;
	_h = 600;
	_p = MONO_16;
	packet_size = 2000 * sizeof(quadlet_t);
	break;
      case MONO16_1024x768:
	_w = 1024;
	_h = 768;
	_p = MONO_16;
	packet_size = 3072 * sizeof(quadlet_t);
	break;
      case YUV422_1280x960:
	_w = 1280;
	_h = 960;
	_p = YUV_422;
	packet_size = 5120 * sizeof(quadlet_t);
	break;
      case RGB24_1280x960:
	_w = 1280;
	_h = 960;
	_p = RGB_24;
	packet_size = 7680 * sizeof(quadlet_t);
	break;
      case MONO8_1280x960:
	_w = 1280;
	_h = 960;
	_p = MONO;
	packet_size = 2560 * sizeof(quadlet_t);
	break;
      case YUV422_1600x1200:
	_w = 1600;
	_h = 1200;
	_p = YUV_422;
	packet_size = 8000 * sizeof(quadlet_t);
	break;
      case RGB24_1600x1200:
	_w = 1600;
	_h = 1200;
	_p = RGB_24;
	packet_size = 12000 * sizeof(quadlet_t);
	break;
      case MONO8_1600x1200:
	_w = 1600;
	_h = 1200;
	_p = MONO;
	packet_size = 4000 * sizeof(quadlet_t);
	break;
      case MONO16_1280x960:
	_w = 1280;
	_h = 960;
	_p = MONO_16;
	packet_size = 5120 * sizeof(quadlet_t);
	break;
      case MONO16_1600x1200:
	_w = 1600;
	_h = 1200;
	_p = MONO_16;
	packet_size = 8000 * sizeof(quadlet_t);
	break;
      default:
	throw std::invalid_argument("Ieee1394Camera::setFormat: not implemented format!!");
	break;
    }
    packet_size >>= (5 - rt);
    u_int	buf_size = _w * _h;
    switch (_p)
    {
      case YUV_444:
      case RGB_24:
	buf_size *= 3;
	break;
      case YUV_422:
      case MONO_16:
	buf_size *= 2;
	break;
      case YUV_411:
	(buf_size *= 3) /= 2;
	break;
    }
    mapListenBuffer(packet_size, buf_size, NBUFFERS);

    if (cont)
	continuousShot();
    
    return *this;
}

//! $B8=:_%+%a%i$K@_Dj$5$l$F$$$k2hA|%U%)!<%^%C%H$rJV$9(B
/*!
  \return	$B@_Dj$5$l$F$$$k2hA|%U%)!<%^%C%H!%(B
*/
Ieee1394Camera::Format
Ieee1394Camera::getFormat() const
{
    return
	uintToFormat(YUV444_160x120 +
		     ((readQuadletFromRegister(Cur_V_Mode)  >>29) & 0x7)*4 + 
		     ((readQuadletFromRegister(Cur_V_Format)>>29) & 0x7)*0x20);
}

//! $B8=:_%+%a%i$K@_Dj$5$l$F$$$k%U%l!<%`%l!<%H$rJV$9(B
/*!
  \return	$B@_Dj$5$l$F$$$k%U%l!<%`%l!<%H!%(B
*/
Ieee1394Camera::FrameRate
Ieee1394Camera::getFrameRate() const
{
    return
	uintToFrameRate(0x1 << 
			(31 - ((readQuadletFromRegister(Cur_V_Frm_Rate) >> 29)
			       & 0x7)));
}

//! $B;XDj$5$l$?B0@-$K$*$$$F%+%a%i$,%5%]!<%H$9$k5!G=$rJV$9(B
/*!
  \param feature	$BBP>]$H$J$kB0@-!%(B
  \return		$B%5%]!<%H$5$l$F$$$k5!G=$r(B#FeatureFunction$B7?$NNs5sCM(B
			$B$N(Bor$B$H$7$FJV$9!%(B
 */
quadlet_t
Ieee1394Camera::inquireFeatureFunction(Feature feature) const
{
    u_int	n = (u_int(feature) - 0x800) >> 2;
    quadlet_t	inq = 0;
    if (n < 32)		// FEATURE_HI
	inq = readQuadletFromRegister(Feature_Hi_Inq) & (0x1 << (31 - n));
    else		// FEATURE_LO
    {
	n -= 32;
	inq = readQuadletFromRegister(Feature_Lo_Inq) & (0x1 << (31 - n));
    }
    if (inq == 0)	// Check presence of feature.
	return 0;

    return readQuadletFromRegister(feature - 0x300);
}

//! $B;XDj$5$l$?B0@-$r(B1$B2s$@$1<+F0@_Dj$9$k!%(B
/*!
  $BK\4X?t$r8F$V$H!$;XDj$7$?B0@-$N<+F0@_Dj$,D>$A$K3+;O$5$l$k!%<+F0@_Dj$,=*N;(B
  $B$7$?$+$I$&$+$O!$(BinOnePushOperation()$B$GCN$k$3$H$,$G$-$k!%(B
  \param feature	$B<+F0@_Dj$7$?$$B0@-!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::onePush(Feature feature)
{
    checkAvailability(feature, One_Push);
    writeQuadletToRegister(feature,
			   readQuadletFromRegister(feature) | One_Push);
    return *this;
}

//! $B;XDj$5$l$?B0@-$r(Bon$B$K$9$k(B
/*!
  \param feature	on$B$K$7$?$$B0@-!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::turnOn(Feature feature)
{
    checkAvailability(feature, OnOff);
    writeQuadletToRegister(feature, readQuadletFromRegister(feature) | ON_OFF);
    return *this;
}

//! $B;XDj$5$l$?B0@-$r(Boff$B$K$9$k(B
/*!
  \param feature	off$B$K$7$?$$B0@-!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::turnOff(Feature feature)
{
    checkAvailability(feature, OnOff);
    writeQuadletToRegister(feature,
			   readQuadletFromRegister(feature) & ~ON_OFF);
    return *this;
}

//! $B;XDj$5$l$?B0@-$r<+F0@_Dj%b!<%I$K$9$k(B
/*!
  $B<+F0@_Dj$K$9$k$H!$$3$NB0@-$NCM$O4D6-$NJQ2=$KDI=>$7$F7QB3E*$K<+F0E*$KD4@0(B
  $B$5$l$k!%(B
  \param feature	$B<+F0@_Dj%b!<%I$K$7$?$$B0@-!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::setAutoMode(Feature feature)
{
    checkAvailability(feature, Auto);
    writeQuadletToRegister(feature,
			   readQuadletFromRegister(feature) | A_M_Mode);
    return *this;
}

//! $B;XDj$5$l$?B0@-$r<jF0@_Dj%b!<%I$K$9$k(B
/*!
  \param feature	$B<jF0@_Dj%b!<%I$K$7$?$$B0@-!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::setManualMode(Feature feature)
{
    checkAvailability(feature, Manual);
    writeQuadletToRegister(feature,
			   readQuadletFromRegister(feature) & ~A_M_Mode);
    return *this;
}

//! $B;XDj$5$l$?B0@-$NCM$r@_Dj$9$k(B
/*!
  #WHITE_BALANCE, #TRIGGER_MODE$B$NCM$r@_Dj$9$k$3$H$O$G$-$J$$!%Be$o$j$K(B
  setWhiteBalance(), setTriggerMode(), setTriggerPolarity()$B$rMQ$$$k$3$H!%(B
  \param feature	$BCM$r@_Dj$7$?$$B0@-!%(B
  \param value		$B@_Dj$9$kCM!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::setValue(Feature feature, u_int value)
{
    if (feature == WHITE_BALANCE || feature == TRIGGER_MODE)
	throw std::invalid_argument("TU::Ieee1394Camera::setValue: cannot set WHITE_BALANCE/TRIGGER_MODE value using this method!!");
    checkAvailability(feature, Manual);
    if (feature == TEMPERATURE)
	writeQuadletToRegister(TEMPERATURE,
			       (readQuadletFromRegister(TEMPERATURE) &
				0xff000fff) | ((value & 0xfff) << 12));
    else
	writeQuadletToRegister(feature,
			       (readQuadletFromRegister(feature) &
				0xfffff000) | (value & 0xfff));
    return *this;
}

//! $B;XDj$5$l$?B0@-$,(B1$B2s$@$1$N<+F0@_Dj$N:GCf$G$"$k$+D4$Y$k(B
/*!
  \param feature	$BBP>]$H$J$kB0@-!%(B
  \return		onePush()$B$r9T$C$?B0@-CM$N<+F0@_Dj$,7QB3Cf$G$"$l$P(B
			true$B$r!$=*N;$7$F$$$l$P(Bfalse$B$rJV$9!%(B
*/
bool
Ieee1394Camera::inOnePushOperation(Feature feature) const
{
    checkAvailability(feature, One_Push);
    return readQuadletFromRegister(feature) & (0x1 << 26);
}

//! $B;XDj$5$l$?B0@-$,(Bon$B$K$J$C$F$$$k$+D4$Y$k(B
/*!
  \param feature	$BBP>]$H$J$kB0@-!%(B
  \return		on$B$K$J$C$F$$$l$P(Btrue$B$r!$$=$&$G$J$1$l$P(Bfalse$B$rJV$9!%(B
*/
bool
Ieee1394Camera::isTurnedOn(Feature feature) const
{
    checkAvailability(feature, OnOff);
    return readQuadletFromRegister(feature) & (0x1 << 25);
}

//! $B;XDj$5$l$?B0@-$,<+F0@_Dj%b!<%I$K$J$C$F$$$k$+D4$Y$k(B
/*!
  \param feature	$BBP>]$H$J$kB0@-!%(B
  \return		$B<+F0@_Dj%b!<%I$K$J$C$F$$$l$P(Btrue$B$r!$$=$&$G$J$1$l$P(B
			false$B$rJV$9!%(B
*/
bool
Ieee1394Camera::isAuto(Feature feature) const
{
    checkAvailability(feature, Auto);
    return readQuadletFromRegister(feature) & (0x1 << 24);
}

//! $B;XDj$5$l$?B0@-$,$H$jF@$kCM$NHO0O$rD4$Y$k(B
/*!
  \param feature	$BBP>]$H$J$kB0@-!%(B
  \param min		$B$H$jF@$kCM$N:G>.CM$,JV$5$l$k!%(B
  \param max		$B$H$jF@$kCM$N:GBgCM$,JV$5$l$k!%(B
*/
void
Ieee1394Camera::getMinMax(Feature feature, u_int& min, u_int& max) const
{
    
    quadlet_t	quad = checkAvailability(feature, Presence);
    min = (quad >> 12) & 0xfff;
    max = quad & 0xfff;
}

//! $B;XDj$5$l$?B0@-$N8=:_$NCM$rD4$Y$k(B
/*!
  feature = #TEMPERATURE$B$N>l9g$O!$(BsetValue()$B$G@_Dj$7$?L\I8CM$G$O$J$/!$(B
  $B<B:]CM$,JV$5$l$k!%L\I8CM$rF@$k$K$O!$(BgetAimedTemperature()$B$rMQ$$$k!%(B
  $B$^$?!$(B#WHITE_BALANCE, #TRIGGER_MODE$B$NCM$rCN$k$3$H$O$G$-$J$$!%Be$o$j(B
  $B$K(BgetWhiteBalance(), getTriggerMode(), getTriggerPolarity()$B$rMQ$$$k(B
  $B$3$H!%(B
  \param	feature $BBP>]$H$J$kB0@-!%(B
  \return	$B8=:_$NCM!%(B
*/
u_int
Ieee1394Camera::getValue(Feature feature) const
{
    if (feature == WHITE_BALANCE || feature == TRIGGER_MODE)
	throw std::invalid_argument("TU::Ieee1394Camera::getValue: cannot get WHITE_BALANCE/TRIGGER_MODE value using this method!!");
    checkAvailability(feature, ReadOut);
    return readQuadletFromRegister(feature) & 0xfff;	// 12bit value.
}

//! $B%[%o%$%H%P%i%s%9$NCM$r@_Dj$9$k(B
/*!
  \param ub		$B@_Dj$9$k(BU/B$BCM!%(B
  \param vr		$B@_Dj$9$k(BV/R$BCM!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::setWhiteBalance(u_int ub, u_int vr)
{
    checkAvailability(WHITE_BALANCE, Manual);
    writeQuadletToRegister(WHITE_BALANCE,
			   (readQuadletFromRegister(WHITE_BALANCE) &
			    0xff000000) | ((ub & 0xfff) << 12) | (vr & 0xfff));
    return *this;
}

//! $B%[%o%$%H%P%i%s%9$NCM$rD4$Y$k(B
/*!
  \param ub		U/B$BCM$,JV$5$l$k!%(B
  \param vr		V/R$BCM$,JV$5$l$k!%(B
*/
void
Ieee1394Camera::getWhiteBalance(u_int &ub, u_int& vr) const
{
    checkAvailability(WHITE_BALANCE, ReadOut);
    quadlet_t	quad = readQuadletFromRegister(WHITE_BALANCE);
    ub = (quad >> 12) & 0xfff;
    vr = quad & 0xfff;
}

//! $B?'29EY$NL\I8CM$rD4$Y$k(B
/*!
  $B?'29EY$N<B:]CM$rCN$k$K$O!$Be$o$j$K(BgetValue()$B$rMQ$$$h!%(B
  \return	$B@_Dj$5$l$F$$$kL\I8CM$,JV$5$l$k!%(B
*/
u_int
Ieee1394Camera::getAimedTemperature() const
{
    checkAvailability(TEMPERATURE, ReadOut);
    return (readQuadletFromRegister(TEMPERATURE) >> 12) & 0xfff;
}

//! $B%H%j%,%b!<%I$r@_Dj$9$k(B
/*!
  $B<B:]$K%+%a%i$,30It%H%j%,$K$h$C$F6nF0$5$l$k$?$a$K$O!$$3$N4X?t$G%b!<%I@_Dj(B
  $B$r9T$C$?8e$K(B#turnOn(#TRIGGER_MODE)$B$r9T$o$J$1$l$P$J$i$J$$!%(B
  \param mode	$B@_Dj$7$?$$%H%j%,%b!<%I!%(B
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::setTriggerMode(TriggerMode mode)
{
    checkAvailability(TRIGGER_MODE, (0x1 << (15 - mode)));
    writeQuadletToRegister(TRIGGER_MODE,
			   (readQuadletFromRegister(TRIGGER_MODE) & ~0xf0000) |
			   ((mode & 0xf) << 16));
    return *this;
}

//! $B8=:_@_Dj$5$l$F$$$k%H%j%,%b!<%I$rD4$Y$k(B
/*!
  \return	$B8=:_@_Dj$5$l$F$$$k%H%j%,%b!<%I!%(B
*/
Ieee1394Camera::TriggerMode
Ieee1394Camera::getTriggerMode() const
{
    checkAvailability(TRIGGER_MODE, ReadOut);
    return uintToTriggerMode((readQuadletFromRegister(TRIGGER_MODE) >> 16)
			     & 0xf);
}

//! $B%H%j%,?.9f$N6K@-$r@_Dj$9$k(B
/*!
  \param polarity	$B@_Dj$7$?$$6K@-!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::setTriggerPolarity(TriggerPolarity polarity)
{
    checkAvailability(TRIGGER_MODE, Polarity_Inq);
    writeQuadletToRegister(TRIGGER_MODE, (readQuadletFromRegister(TRIGGER_MODE)
					  & ~HighActiveInput) | polarity);
    return *this;
}

//! $B8=:_@_Dj$5$l$F$$$k%H%j%,?.9f$N6K@-$rD4$Y$k(B
/*!
  \return	$B8=:_@_Dj$5$l$F$$$k%H%j%,?.9f$N6K@-!%(B
*/
Ieee1394Camera::TriggerPolarity
Ieee1394Camera::getTriggerPolarity() const
{
    checkAvailability(TRIGGER_MODE, ReadOut);
    if (readQuadletFromRegister(TRIGGER_MODE) & HighActiveInput)
	return HighActiveInput;
    else
	return LowActiveInput;
}

//! $B%+%a%i$+$i$N2hA|$NO"B3E*=PNO$r3+;O$9$k(B
/*!
  #TRIGGER_MODE$B$,(Bon$B$G$"$l$P!$;#1F$N%?%$%_%s%0$O30It%H%j%,?.9f$K$h$C$F@)8f$5(B
  $B$l$k!%(B
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::continuousShot()
{
    if (!inContinuousShot())
	writeQuadletToRegister(ISO_EN, 0x1 << 31);
    return *this;
}

//! $B%+%a%i$+$i$N2hA|$NO"B3E*=PNO$rDd;_$9$k(B
/*!
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::stopContinuousShot()
{
    if (inContinuousShot())
    {
	writeQuadletToRegister(ISO_EN, 0x0);
	flushListenBuffer();
	_buf = 0;
    }
    return *this;
}

//! $B%+%a%i$+$i2hA|$,=PNOCf$G$"$k$+D4$Y$k(B
/*!
  \return	$B2hA|=PNOCf$G$"$l$P(Btrue$B$r!$$=$&$G$J$1$l$P(Bfalse$B$rJV$9!%(B
*/
bool
Ieee1394Camera::inContinuousShot() const
{
    return readQuadletFromRegister(ISO_EN) & (0x1 << 31);
}

//! $B2hA|$r(B1$BKg$@$1;#1F$7$F$=$l$r=PNO$9$k(B
/*!
  $B2hA|$rO"B3E*$K=PNOCf$G$"$l$P!$$=$l$rDd;_$7$?8e$K$"$i$?$a$F(B1$BKg$@$1;#1F$9$k!%(B
  #TRIGGER_MODE$B$,(Bon$B$G$"$l$P!$;#1F$N%?%$%_%s%0$O30It%H%j%,?.9f$K$h$C$F@)8f$5(B
  $B$l$k!%(B
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::oneShot()
{
    checkAvailability(One_Shot_Inq);
    stopContinuousShot();
    writeQuadletToRegister(One_Shot, 0x1 << 31);
    return *this;
}

//! $B2hA|$r;XDj$5$l$?Kg?t$@$1;#1F$7$F$=$l$r=PNO$9$k(B
/*!
  $B2hA|$rO"B3E*$K=PNOCf$G$"$l$P!$$=$l$rDd;_$7$?8e$K$"$i$?$a$F;#1F$r3+;O$9$k!%(B
  #TRIGGER_MODE$B$,(Bon$B$G$"$l$P!$;#1F$N%?%$%_%s%0$O30It%H%j%,?.9f$K$h$C$F@)8f$5(B
  $B$l$k!%(B
  \param nframes	$B;#1F$7$?$$Kg?t!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::multiShot(u_short nframes)
{
    checkAvailability(Multi_Shot_Inq);
    stopContinuousShot();
    writeQuadletToRegister(One_Shot, (0x1 << 30) | (nframes & 0xffff));
    return *this;
}

//! $B8=:_$N%+%a%i$N@_Dj$r;XDj$5$l$?%a%b%j%A%c%s%M%k$K5-21$9$k(B
/*!
  IEEE1394$B%+%a%i$N0lIt$K$O!$%+%a%i$K@_Dj$7$?2hA|%U%)!<%^%C%H$dB0@-CM$J$I$r(B
  $B%+%a%iFbIt$N%a%b%j%A%c%s%M%k$K5-21$G$-$k$b$N$,$"$k!%(B
  \param mem_ch		$BCM$r5-21$9$k%a%b%j%A%c%s%M%kHV9f!%(B0$B0J>e$NCM$r$H$j!$(B
			$B:GBgCM$O(BgetMemoryChannelMax$B$GD4$Y$i$l$k!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::saveConfig(u_int mem_ch)
{
    u_int	max = getMemoryChannelMax();
    if (mem_ch == 0 || mem_ch > max)
	throw std::invalid_argument("TU::Ieee1394Camera::saveConfig: invalid memory channel!!");
    writeQuadletToRegister(Mem_Save_Ch, mem_ch << 28);
    writeQuadletToRegister(Memory_Save, 0x1 << 31);
    while ((readQuadletFromRegister(Memory_Save) & (0x1 << 31)) != 0)
	;
    return *this;
}

//! $B;XDj$5$l$?%a%b%j%A%c%s%M%k$K5-21$5$l$?CM$r%+%a%i$K@_Dj$9$k(B
/*!
  IEEE1394$B%+%a%i$N0lIt$K$O!$%+%a%i$K@_Dj$7$?2hA|%U%)!<%^%C%H$dB0@-CM$J$I$r(B
  $B%+%a%iFbIt$N%a%b%j%A%c%s%M%k$K5-21$G$-$k$b$N$,$"$k!%(B
  \param mem_ch		$B@_Dj$7$?$$CM$r5-21$7$F$$$k%a%b%j%A%c%s%M%kHV9f!%(B0$B0J>e(B
			$B$NCM$r$H$j!$:GBgCM$O(BgetMemoryChannelMax$B$GD4$Y$i$l$k!%(B
  \return		$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
Ieee1394Camera::restoreConfig(u_int mem_ch)
{
    u_int	max = getMemoryChannelMax();
    if (mem_ch > max)
	throw std::invalid_argument("TU::Ieee1394Camera::restoreConfig: invalid memory channel!!");
    writeQuadletToRegister(Cur_Mem_Ch, mem_ch << 28);
    return *this;
}

//! $B%a%b%j%A%c%s%M%k$N:GBgCM$rD4$Y$k(B
/*!
  IEEE1394$B%+%a%i$N0lIt$K$O!$%+%a%i$K@_Dj$7$?2hA|%U%)!<%^%C%H$dB0@-CM$J$I$r(B
  $B%+%a%iFbIt$N%a%b%j%A%c%s%M%k$K5-21$G$-$k$b$N$,$"$k!%(B
  \return 	$B%a%b%j%A%c%s%M%kHV9f$N:GBgCM!%(B
*/
u_int
Ieee1394Camera::getMemoryChannelMax() const
{
    return (inquireBasicFunction() & 0xf);
}

/*
 *  Capture stuffs.
 */
#ifdef HAVE_TUToolsPP
//! IEEE1394$B%+%a%i$+$i=PNO$5$l$?2hA|(B1$BKgJ,$N%G!<%?$rE,Ev$J7A<0$KJQ49$7$F<h$j9~$`(B
/*!
  $B%F%s%W%l!<%H%Q%i%a!<%?(BT$B$O!$3JG<@h$N2hA|$N2hAG7A<0$rI=$9!%$J$*!$K\4X?t$r(B
  $B8F$S=P$9A0$K(Bsnap()$B$K$h$C$F%+%a%i$+$i$N2hA|$rJ];}$7$F$*$+$J$1$l$P$J$i$J$$!%(B
  \param image	$B2hA|%G!<%?$r3JG<$9$k2hA|%*%V%8%'%/%H!%2hA|$NI}$H9b$5$O!$(B
		$B8=:_%+%a%i$K@_Dj$5$l$F$$$k2hA|%5%$%:$K9g$o$;$F<+F0E*$K(B
		$B@_Dj$5$l$k!%$^$?!$%+%a%i$K@_Dj$5$l$?%U%)!<%^%C%H$N2hAG7A<0(B
		$B$,2hA|$N$=$l$K0lCW$7$J$$>l9g$O!$<+F0E*$KJQ49$,9T$o$l$k!%(B
		$B%5%]!<%H$5$l$F$$$k2hAG7A<0(BT$B$O!$(Bu_char, short, float, double,
		RGB, RGBA, BGR,	ABGR, YUV444, YUV422, YUV411$B$N$$$:$l$+$G$"$k!%(B
		$B$^$?!$%5%]!<%H$5$l$F$$$kJQ49$O0J2<$N$H$*$j$G$"$j!$%+%a%i$N(B
		$B2hAG7A<0$,$3$l0J30$K@_Dj$5$l$F$$$k>l9g$O(Bstd::domain_error
		$BNc30$,Aw=P$5$l$k!%(B
		    -# #YUV_444 -> T
		    -# #YUV_422 -> T
		    -# #YUV_411 -> T
		    -# #RGB_24 -> T (YUV444, YUV422, YUV411$B$r=|$/(B) 
		    -# #MONO -> T
		    -# #MONO_16 -> T
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
template <class T> const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<T>& image) const
{
    if (_buf == 0)
	throw std::runtime_error("TU::Ieee1394Camera::operator >>: no images snapped!!");
  // Transfer image data from current buffer.
    image.resize(height(), width());
    switch (pixelFormat())
    {
      case YUV_444:
      {
	const YUV444*	src = (const YUV444*)_buf;
	for (u_int v = 0; v < image.height(); ++v)
	    src = image[v].fill(src);
      }
	break;
      case YUV_422:
      {
	const YUV422*	src = (const YUV422*)_buf;
	for (u_int v = 0; v < image.height(); ++v)
	    src = image[v].fill(src);
      }
	break;
      case YUV_411:
      {
	const YUV411*	src = (const YUV411*)_buf;
	for (u_int v = 0; v < image.height(); ++v)
	    src = image[v].fill(src);
      }
	break;
      case RGB_24:
      {
	const RGB*	src = (const RGB*)_buf;
	for (u_int v = 0; v < image.height(); ++v)
	    src = image[v].fill(src);
      }
	break;
      case MONO:
      {
	const u_char*	src = _buf;
	for (u_int v = 0; v < image.height(); ++v)
	    src = image[v].fill(src);
      }
	break;
      case MONO_16:
      {
	const Mono16*	src = (const Mono16*)_buf;
	for (u_int v = 0; v < image.height(); ++v)
	    src = image[v].fill(src);
      }
	break;
      default:
	throw std::domain_error("TU::Ieee1394Camera::operator >>: unknown pixel format!!");
	break;
    }

    return *this;
}
#endif	// HAVE_TUToolsPP

//! IEEE1394$B%+%a%i$+$i=PNO$5$l$?2hA|(B1$BKgJ,$N%G!<%?$r$J$s$iJQ49$r9T$o$:$K<h$j9~$`(B
/*!
  $BK\4X?t$r8F$S=P$9A0$K(Bsnap()$B$K$h$C$F%+%a%i$+$i$N2hA|$rJ];}$7$F$*$+$J$1$l$P(B
  $B$J$i$J$$!%(B
  \param image	$B2hA|%G!<%?$N3JG<NN0h$X$N%]%$%s%?!%(Bwidth(), height()$B$*$h$S(B
		pixelFormat()$B$rMQ$$$F2hA|$N%5%$%:$H2hAG$N7A<0$rD4$Y$F(B
		$B2hA|(B1$BKgJ,$NNN0h$r3NJ]$7$F$*$/$N$O!$%f!<%6$N@UG$$G$"$k!%(B
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
const Ieee1394Camera&
Ieee1394Camera::captureRaw(void* image) const
{
    if (_buf == 0)
	throw std::runtime_error("TU::Ieee1394Camera::captureRaw: no images snapped!!");
  // Transfer image data from current buffer.
    memcpy(image, _buf, bufferSize());

    return *this;
}

//! unsinged int$B$NCM$rF1$8%S%C%H%Q%?!<%s$r;}$D(B#Format$B$KD>$9(B
/*!
  \param format	#Format$B$KD>$7$?$$(Bunsigned int$BCM!%(B
  \return	#Format$B7?$N(Benum$BCM!%(B
 */
Ieee1394Camera::Format
Ieee1394Camera::uintToFormat(u_int format)
{
    switch (format)
    {
      case YUV444_160x120:
	return YUV444_160x120;
      case YUV422_320x240:
	return YUV422_320x240;
      case YUV411_640x480:
	return YUV411_640x480;
      case YUV422_640x480:
	return YUV422_640x480;
      case RGB24_640x480:
	return RGB24_640x480;
      case MONO8_640x480:
	return MONO8_640x480;
      case MONO16_640x480:
	return MONO16_640x480;
      case YUV422_800x600:
	return YUV422_800x600;
      case RGB24_800x600:
	return RGB24_800x600;
      case MONO8_800x600:
	return MONO8_800x600;
      case YUV422_1024x768:
	return YUV422_1024x768;
      case RGB24_1024x768:
	return RGB24_1024x768;
      case MONO8_1024x768:
	return MONO8_1024x768;
      case MONO16_800x600:
	return MONO16_800x600;
      case MONO16_1024x768:
	return MONO16_1024x768;
      case YUV422_1280x960:
	return YUV422_1280x960;
      case RGB24_1280x960:
	return RGB24_1280x960;
      case MONO8_1280x960:
	return MONO8_1280x960;
      case YUV422_1600x1200:
	return YUV422_1600x1200;
      case RGB24_1600x1200:
	return RGB24_1600x1200;
      case MONO8_1600x1200:
	return MONO8_1600x1200;
      case MONO16_1280x960:
	return MONO16_1280x960;
      case MONO16_1600x1200:
	return MONO16_1600x1200;
      case Format_7_0:
	return Format_7_0;
      case Format_7_1:
	return Format_7_1;
      case Format_7_2:
	return Format_7_2;
      case Format_7_3:
	return Format_7_3;
      case Format_7_4:
	return Format_7_4;
      case Format_7_5:
	return Format_7_5;
      case Format_7_6:
	return Format_7_6;
      case Format_7_7:
	return Format_7_7;
    }

    throw std::invalid_argument("Unknown format!!");
    
    return YUV444_160x120;
}

//! unsinged int$B$NCM$rF1$8%S%C%H%Q%?!<%s$r;}$D(B#FrameRate$B$KD>$9(B
/*!
  \param rate	#FrameRate$B$KD>$7$?$$(Bunsigned int$BCM!%(B
  \return	#FrameRate$B7?$N(Benum$BCM!%(B
 */
Ieee1394Camera::FrameRate
Ieee1394Camera::uintToFrameRate(u_int rate)
{
    switch (rate)
    {
      case FrameRate_1_875:
	return FrameRate_1_875;
      case FrameRate_3_75:
	return FrameRate_3_75;
      case FrameRate_7_5:
	return FrameRate_7_5;
      case FrameRate_15:
	return FrameRate_15;
      case FrameRate_30:
	return FrameRate_30;
      case FrameRate_60:
	return FrameRate_60;
    }

    throw std::invalid_argument("Unknown frame rate!!");
    
    return FrameRate_1_875;
}

//! unsinged int$B$NCM$rF1$8%S%C%H%Q%?!<%s$r;}$D(B#Feature$B$KD>$9(B
/*!
  \param feature	#Feature$B$KD>$7$?$$(Bunsigned int$BCM!%(B
  \return		#Feature$B7?$N(Benum$BCM!%(B
 */
Ieee1394Camera::Feature
Ieee1394Camera::uintToFeature(u_int feature)
{
    switch (feature)
    {
      case BRIGHTNESS:
	return BRIGHTNESS;
      case AUTO_EXPOSURE:
	return AUTO_EXPOSURE;
      case SHARPNESS:
	return SHARPNESS;
      case WHITE_BALANCE:
	return WHITE_BALANCE;
      case HUE:
	return HUE;
      case SATURATION:
	return SATURATION;
      case GAMMA:
	return GAMMA;
      case SHUTTER:
	return SHUTTER;
      case GAIN:
	return GAIN;
      case IRIS:
	return IRIS;
      case FOCUS:
	return FOCUS;
      case TEMPERATURE:
	return TEMPERATURE;
      case TRIGGER_MODE:
	return TRIGGER_MODE;
      case ZOOM:
	return ZOOM;
      case PAN:
	return PAN;
      case TILT:
	return TILT;
      case OPTICAL_FILTER:
	return OPTICAL_FILTER;
      case CAPTURE_SIZE:
	return CAPTURE_SIZE;
      case CAPTURE_QUALITY:
	return CAPTURE_QUALITY;
    }

    throw std::invalid_argument("Unknown feature!!");
    
    return BRIGHTNESS;
}

//! unsinged int$B$NCM$rF1$8%S%C%H%Q%?!<%s$r;}$D(B#TriggerMode$B$KD>$9(B
/*!
  \param triggerMode	#TriggerMode$B$KD>$7$?$$(Bunsigned int$BCM!%(B
  \return		#TriggerMode$B7?$N(Benum$BCM!%(B
 */
Ieee1394Camera::TriggerMode
Ieee1394Camera::uintToTriggerMode(u_int triggerMode)
{
    switch (triggerMode)
    {
      case Trigger_Mode0:
	return Trigger_Mode0;
      case Trigger_Mode1:
	return Trigger_Mode1;
      case Trigger_Mode2:
	return Trigger_Mode2;
      case Trigger_Mode3:
	return Trigger_Mode3;
    }

    throw std::invalid_argument("Unknown trigger mode!!");
    
    return Trigger_Mode0;
}
 
}
#ifdef HAVE_TUToolsPP
#  ifdef __GNUG__
#    include "TU/Array++.cc"
#    include "TU/Image++.cc"
namespace TU
{
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<u_char>& image)	const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<short>& image)	const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<float>& image)	const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<double>& image)	const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<RGB>& image)		const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<RGBA>& image)		const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<BGR>& image)		const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<ABGR>& image)		const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<YUV444>& image)	const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<YUV422>& image)	const	;
template const Ieee1394Camera&
Ieee1394Camera::operator >>(Image<YUV411>& image)	const	;
}
#  endif	// __GNUG__
#endif		// HAVE_TUToolsPP
