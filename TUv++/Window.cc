/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$;HMQ!$Bh;0<T$X3+<($9$k(B
 *  $BEy$NCx:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  Confidential and all rights reserved.
 *  This program is confidential. Any using, copying, changing, giving
 *  information about the source program of any part of this software
 *  to others without permission by the creators are prohibited.
 *
 *  No Warranty.
 *  Copyright holders or creators are not responsible for any damages
 *  in the use of this program.
 *  
 *  $Id: Window.cc,v 1.4 2007-11-26 08:11:50 ueshiba Exp $
 */
#include "TU/v/App.h"
#include <stdexcept>

namespace TU
{
namespace v
{
/************************************************************************
*  class Window							*
************************************************************************/
Window::Window(Window& parentWindow)
    :Object(parentWindow), _windowList(), _paneList()
{
    if (&parent() != this)
	parent().window().addWindow(*this);
}

Window::~Window()
{
    if (&parent() != this)
	parent().window().detachWindow(*this);
}

Colormap&
Window::colormap()
{
    return app().colormap();
}

void
Window::show()
{
    XtPopup(widget(), XtGrabNone);
}

void
Window::hide()
{
    XtPopdown(widget());
}

/*
 *  protected member funtions
 */
Window&
Window::window()
{
    return *this;
}

CanvasPane&
Window::canvasPane()
{
    throw std::domain_error("TU::v::Window::canvasPane(): No canvasPane found!!");
    return canvasPane();
}

Object&
Window::paned()
{
    return *this;
}

}
}

