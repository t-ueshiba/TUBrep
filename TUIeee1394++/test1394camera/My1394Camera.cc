/*
 *  $Id: My1394Camera.cc,v 1.7 2003-02-27 03:48:13 ueshiba Exp $
 */
#include <sys/time.h>
#include <stdexcept>
#include "My1394Camera.h"

namespace TU
{
/************************************************************************
*  YUV -> RGB conversion stuffs						*
************************************************************************/
static int	tbl_r [256];
static int	tbl_g0[256];
static int	tbl_g1[256];
static int	tbl_b [256];

inline int	flt2fix(float flt)	{return int(flt * (1 << 10));}
inline int	fix2int(int fix)	{return fix >> 10;}

//! YUV -> RGB $BJQ49%F!<%V%k$N=i4|2=!%(B
static void
initialize_tbl()
{
    for (int i = 0; i < 256; ++i)
    {
	tbl_r [i] = int(1.4022f * (i - 128));
	tbl_g0[i] = flt2fix(0.7144f * (i - 128));
	tbl_g1[i] = flt2fix(0.3457f * (i - 128));
	tbl_b [i] = int(1.7710f * (i - 128));
    }
}
    
//! YUV -> RGB $BJQ49$r9T$&!%(B
inline MyRGB
yuv2rgb(u_char y, u_char u, u_char v)
{
    int		tmp;
    MyRGB	val;
    tmp   = y + tbl_r[v];
    val.r = (tmp > 255 ? 255 : tmp < 0 ? 0 : tmp);
    tmp   = y - fix2int(tbl_g0[v] + tbl_g1[u]);
    val.g = (tmp > 255 ? 255 : tmp < 0 ? 0 : tmp);
    tmp   = y + tbl_b[u];
    val.b = (tmp > 255 ? 255 : tmp < 0 ? 0 : tmp);
    
    return val;
}

/************************************************************************
*  static functions							*
************************************************************************/
//! $B%k!<%W(B10$B2s$KMW$7$?;~4V$NJ?6Q$r$H$k$3$H$K$h$j!$%U%l!<%`%l!<%H$rB,Dj$9$k!%(B
static void
countTime(int& nframes, struct timeval& start)
{
    if (nframes == 10)
    {
	struct timeval	end;
	gettimeofday(&end, NULL);
	double	interval = (end.tv_sec  - start.tv_sec) +
	    (end.tv_usec - start.tv_usec) / 1.0e6;
	std::cerr << nframes / interval << " frames/sec" << std::endl;
	nframes = 0;
    }
    if (nframes++ == 0)
	gettimeofday(&start, NULL);
}

//! $B:FIA2h$N$?$a$N%3!<%k%P%C%/4X?t!%(B
/*!
  \param widget		DrawingArea widget
  \param event		$B%$%Y%s%H(B
  \param userdata	My1394Camera (IEEE1394$B%+%a%i(B)
  \return		TRUE$B$rJV$9(B
*/
static gboolean
CBexpose(GtkWidget* widget, GdkEventExpose* event, gpointer userdata)
{
    My1394Camera*	camera = (My1394Camera*)userdata;
    camera->draw();
    return TRUE;
}

/************************************************************************
*  class My1394Camera							*
************************************************************************/
//! IEEE1394$B%+%a%i%N!<%I$r@8@.$9$k(B
/*!
  \param port	$B$3$N%+%a%i$,@\B3$5$l$F$$$k%]!<%H!%(B
  \param uniqId	$B8D!9$N%+%a%i8GM-$N(B64bit ID$B!%F10l$N(BIEEE1394 bus$B$K(B
		$BJ#?t$N%+%a%i$,@\B3$5$l$F$$$k>l9g!$$3$l$K$h$C$F(B
		$BF1Dj$r9T$&!%(B
*/
My1394Camera::My1394Camera(Ieee1394Port& port, u_int64 uniqId)
    :Ieee1394Camera(port, 0, uniqId),
     _canvas(gtk_drawing_area_new()),
     _buf(0),
     _rgb(0)
{
    initialize_tbl();			// YUV -> RGB $BJQ49%F!<%V%k$N=i4|2=!%(B
    gdk_rgb_init();
    gtk_signal_connect(GTK_OBJECT(_canvas), "expose_event",
		       GTK_SIGNAL_FUNC(CBexpose), (gpointer)this);
    
  // $B8=:_$N%+%a%i$N%U%)!<%^%C%H$K9g$o$;$F%P%C%U%!$N3NJ]$r9T$&!%(B
    setFormatAndFrameRate(getFormat(), getFrameRate());
}

//! IEEE1394$B%+%a%i%*%V%8%'%/%H$rGK2u$9$k(B
My1394Camera::~My1394Camera()
{
    delete [] _rgb;
    delete [] _buf;
}

//! $B2hA|%U%)!<%^%C%H$H%U%l!<%`%l!<%H$r;XDj$9$k!%(B
/*!
  $B$5$i$KF~NO2hA|%P%C%U%!$H(BRGB$B%P%C%U%!$r3NJ]$7D>$7!$(Bcanvas$B$NBg$-$5$rJQ99$9$k!%(B
  \param format	$B@_Dj$7$?$$2hA|%U%)!<%^%C%H!%(B
  \param rate	$B@_Dj$7$?$$%U%l!<%`%l!<%H!%(B
  \return	$B$3$N(BIEEE1394$B%+%a%i%*%V%8%'%/%H!%(B
*/
Ieee1394Camera&
My1394Camera::setFormatAndFrameRate(Format format, FrameRate rate)
{
  // IEEE1394$B%+%a%i$KBP$7$F2hA|%U%)!<%^%C%H$H%U%l!<%`%l!<%H$r;XDj$9$k!%(B
    Ieee1394Camera::setFormatAndFrameRate(format, rate);

  // $B;XDj$7$?%U%)!<%^%C%H$K9g$o$;$FF~NO2hA|%P%C%U%!$H(BRGB$B%P%C%U%!$r:F3NJ]$9$k!%(B
    u_int	buffSize;
    switch (pixelFormat())
    {
      case YUV_444:
      case RGB_24:
	buffSize = width() * height() * 3;
	break;
      case YUV_422:
      case MONO_16:
	buffSize = width() * height() * 2;
	break;
      case YUV_411:
	buffSize = width() * height() * 3 / 2;
	break;
      case MONO_8:
	buffSize = width() * height();
	break;
      default:
	throw std::invalid_argument("Unsupported camera format!!");
	break;
    }
    delete [] _rgb;
    delete [] _buf;
    _buf = new u_char[buffSize];
    _rgb = new MyRGB[width() * height()];

  // $B;XDj$7$?%U%)!<%^%C%H$K9g$o$;$F(Bcanvas$B$NBg$-$5$rJQ99$9$k!%(B
    gtk_drawing_area_size(GTK_DRAWING_AREA(_canvas), width(), height());
    gtk_widget_show(_canvas);
    
    return *this;
}

//! $B%+%a%i$+$i2hA|$rFI$_9~$_!$(Bcanvas$B$KI=<($9$k!%(B
/*!
  $B2hA|%-%c%W%A%c%\%?%s$,2!$5$l$F$$$k4V$O!$(Bidle$B4X?t$H$7$FB>$K$d$k$3$H$,(B
  $B$J$$;~$K$/$j$+$($78F$P$l$k!%(B
*/
void
My1394Camera::idle()
{
  // $B%U%l!<%`%l!<%H$NB,Dj!%(B
    static int			nframes = 0;
    static struct timeval	start;
    countTime(nframes, start);

  // IEEE1394Camera $B$+$i2hA|%G!<%?$rFI$_9~$`!%(B
    if (bayerTileMapping() != Ieee1394Camera::YYYY &&
	(pixelFormat() == MONO_8 || pixelFormat() == MONO_16))
	snap().captureBayerRaw(_rgb);
    else
	snap().captureRaw(_buf);
    draw();			// canvas$B$KI=<($9$k!%(B
}

//! $B%P%C%U%!Cf$N2hA|2hA|$r(Bcanvas$B$KI=<($9$k!%(B
/*!
  idle(), CBexpose()$B$+$i8F$P$l$k!%(B
*/
void
My1394Camera::draw()
{
  // $BI,MQ$J$i(B YUV -> RGB $B$NJQ49$r9T$C$F$+$i2hA|$rI=<(!%(B
    switch (pixelFormat())
    {
      case YUV_444:
      {
	const u_char*	p = _buf;
	MyRGB*		q = _rgb;
	for (u_int y = 0; y < height(); ++y)
	    for (u_int x = 0; x < width(); ++x)
	    {
		*q++ = yuv2rgb(p[1], p[0], p[2]);	// Y, U, V
		p += 3;
	    }
	gdk_draw_rgb_image(_canvas->window,
			   _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			   0, 0, width(), height(),
			   GDK_RGB_DITHER_NONE, (guchar*)_rgb, 3*width());
      }
	break;
      case YUV_422:
      {
	const u_char*	p = _buf;
	MyRGB*		q = _rgb;
	for (u_int y = 0; y < height(); ++y)
	    for (u_int x = 0; x < width(); x += 2)
	    {
		*q++ = yuv2rgb(p[1], p[0], p[2]);	// Y0, U, V
		*q++ = yuv2rgb(p[3], p[0], p[2]);	// Y1, U, V
		p += 4;
	    }
	gdk_draw_rgb_image(_canvas->window,
			   _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			   0, 0, width(), height(),
			   GDK_RGB_DITHER_NONE, (guchar*)_rgb, 3*width());
      }
	break;
      case YUV_411:
      {
	const u_char*	p = _buf;
	MyRGB*		q = _rgb;
	for (u_int y = 0; y < height(); ++y)
	    for (u_int x = 0; x < width(); x += 4)
	    {
		*q++ = yuv2rgb(p[1], p[0], p[3]);	// Y0, U, V
		*q++ = yuv2rgb(p[2], p[0], p[3]);	// Y1, U, V
		*q++ = yuv2rgb(p[4], p[0], p[3]);	// Y2, U, V
		*q++ = yuv2rgb(p[5], p[0], p[3]);	// Y3, U, V
		p += 6;
	    }
	gdk_draw_rgb_image(_canvas->window,
			   _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			   0, 0, width(), height(),
			   GDK_RGB_DITHER_NONE, (guchar*)_rgb, 3*width());
      }
	break;
      case RGB_24:
	gdk_draw_rgb_image(_canvas->window,
			   _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			   0, 0, width(), height(),
			   GDK_RGB_DITHER_NONE, (guchar*)_buf, 3*width());
	break;
      case MONO_8:
	if (bayerTileMapping() != Ieee1394Camera::YYYY)
	    gdk_draw_rgb_image(_canvas->window,
			       _canvas->style
				      ->fg_gc[GTK_WIDGET_STATE(_canvas)],
			       0, 0, width(), height(),
			       GDK_RGB_DITHER_NONE, (guchar*)_rgb, 3*width());
	else
	    gdk_draw_gray_image(_canvas->window,
				_canvas->style
				       ->fg_gc[GTK_WIDGET_STATE(_canvas)],
				0, 0, width(), height(),
				GDK_RGB_DITHER_NONE, (guchar*)_buf, width());
	break;
      case MONO_16:
	if (bayerTileMapping() != Ieee1394Camera::YYYY)
	    gdk_draw_rgb_image(_canvas->window,
			       _canvas->style
				      ->fg_gc[GTK_WIDGET_STATE(_canvas)],
			       0, 0, width(), height(),
			       GDK_RGB_DITHER_NONE, (guchar*)_rgb, 3*width());
	else
	{
	    const u_short*	p = (u_short*)_buf;
	    u_char*		q = _buf;
	    for (u_int y = 0; y < height(); ++y)
		for (u_int x = 0; x < width(); ++x)
		    *q++ = htons(*p++);
	    gdk_draw_gray_image(_canvas->window,
				_canvas->style
				       ->fg_gc[GTK_WIDGET_STATE(_canvas)],
				0, 0, width(), height(),
				GDK_RGB_DITHER_NONE, (guchar*)_buf, width());
	}
	break;
    }
}
 
//! $B%P%C%U%!Cf$N2hA|$r(Bsave$B$9$k!%(B
/*!
  $B%b%N%/%m2hA|$O(BPGM$B7A<0$G!"%+%i!<2hA|$O(BPPM$B7A<0$G(Bsave$B$5$l$k!%(B
  \param out	$B2hA|$r=q$-=P$9=PNO%9%H%j!<%`!%(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`!%(B
*/
std::ostream&
My1394Camera::save(std::ostream& out) const
{
    using namespace	std;
    
    switch (pixelFormat())
    {
      case YUV_444:
      case YUV_422:
      case YUV_411:
	out << "P6" << '\n' << width() << ' ' << height() << '\n' << 255
	    << endl;
	out.write((const char*)_rgb, 3*width()*height());
	break;

      case RGB_24:
	out << "P6" << '\n' << width() << ' ' << height() << '\n' << 255
	    << endl;
	out.write((const char*)_buf, 3*width()*height());
	break;

      case MONO_8:
      case MONO_16:
	if (bayerTileMapping() != Ieee1394Camera::YYYY)
	{
	    out << "P6" << '\n' << width() << ' ' << height() << '\n' << 255
		<< endl;
	    out.write((const char*)_rgb, 3*width()*height());
	}
	else
	{
	    out << "P5" << '\n' << width() << ' ' << height() << '\n' << 255
		<< endl;
	    out.write((const char*)_buf, width()*height());
	}
	break;
    }
    
    return out;
}
 
}
