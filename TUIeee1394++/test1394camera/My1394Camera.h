/*
 *  $Id: My1394Camera.h,v 1.3 2003-02-27 03:48:13 ueshiba Exp $
 */
#include <gtk/gtk.h>
#include "TU/Ieee1394++.h"

namespace TU
{
/*!
  RGB $B%+%i!<2hA|$N2hAG$rI=$99=B$BN!%(B
*/
struct MyRGB
{
    u_char	r;	//!< $B@V(B
    u_char	g;	//!< $B@D(B
    u_char	b;	//!< $BNP(B
};

/************************************************************************
*  class My1394Camera							*
************************************************************************/
/*!
  IEEE1394$B%G%8%?%k%+%a%i$rI=$9%/%i%9!%$5$i$K!$(BGTK+ $B$rMQ$$$?2hA|I=<($N$?$a$N(B
  canvas (GTK+ $B$N(B drawing area widget)$B!$F~NO2hA|%P%C%U%!!$(BRGB $B%+%i!<2hA|(B
  $BI=<(MQ$N%P%C%U%!$r3NJ]$9$k5!G=$r;}$D!%(B
*/
class My1394Camera : public Ieee1394Camera
{
  public:
    My1394Camera(Ieee1394Port& port, u_int64 uniqId)			;
    ~My1394Camera()							;

  //! $B2hA|$NI=<(NN0h$H$J$k%-%c%s%P%9$rJV$9!%(B
    GtkWidget*		canvas()				const	;
    Ieee1394Camera&	setFormatAndFrameRate(Format format,
					      FrameRate rate)		;
    void		idle()						;
    void		draw()						;
    std::ostream&	save(std::ostream& out)			const	;
    
  private:
    GtkWidget* const	_canvas;	// $B2hA|$rI=<($9$kNN0h(B
    u_char*		_buf;		// $BF~NO2hA|MQ%P%C%U%!(B
    MyRGB*		_rgb;		// RGB $B%+%i!<2hA|(B($BI=<(MQ(B)$B%P%C%U%!(B
};

inline GtkWidget*
My1394Camera::canvas() const
{
    return _canvas;
}

}
