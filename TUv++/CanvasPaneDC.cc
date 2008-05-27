/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$kEy$NCx(B
 *  $B:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: CanvasPaneDC.cc,v 1.6 2008-05-27 11:38:25 ueshiba Exp $
 */
#include "TU/v/CanvasPaneDC.h"
#include "vCanvas_.h"
#include "vViewport_.h"

namespace TU
{
namespace v
{
/************************************************************************
*  static data								*
************************************************************************/
enum	{m_X025 = 32400, m_X05, m_X10, m_X15, m_X20, m_X40};

static MenuDef zoomMenu[] =
{
    {"x0.25", m_X025, false, noSub},
    {"x0.5",  m_X05,  false, noSub},
    {"x1",    m_X10,  false, noSub},
    {"x1.5",  m_X15,  false, noSub},
    {"x2",    m_X20,  false, noSub},
    {"x4",    m_X40,  false, noSub},
    EndOfMenu
};

/************************************************************************
*  Xt event handlers							*
************************************************************************/
void
EVcanvasPaneDC(::Widget widget, XtPointer client_data, XEvent* event,
	       Boolean* boolean)
{
    extern void		EVkeyPress(::Widget, XtPointer, XEvent*, Boolean*);
    CanvasPaneDC&	canvasPaneDC = *(CanvasPaneDC*)client_data;
    switch (event->type)
    {
      case ButtonPress:
	switch (event->xbutton.button)
	{
	  case Button1:
	    canvasPaneDC.callback(Id_MouseButton1Press,
				  CmdVal(event->xbutton.x, event->xbutton.y));
	    break;
	  case Button2:
	    canvasPaneDC.callback(Id_MouseButton2Press,
				  CmdVal(event->xbutton.x, event->xbutton.y));
	    break;
	  case Button3:
	    canvasPaneDC._u_last = canvasPaneDC.dev2logU(event->xbutton.x);
	    canvasPaneDC._v_last = canvasPaneDC.dev2logU(event->xbutton.y);
	    canvasPaneDC.callback(Id_MouseButton3Press,
				  CmdVal(event->xbutton.x, event->xbutton.y));
	    break;
	}
	break;
	
      case ButtonRelease:
	switch (event->xbutton.button)
	{
	  case Button1:
	    canvasPaneDC.callback(Id_MouseButton1Release,
				  CmdVal(event->xbutton.x, event->xbutton.y));
	    break;
	  case Button2:
	    canvasPaneDC.callback(Id_MouseButton2Release,
				  CmdVal(event->xbutton.x, event->xbutton.y));
	    break;
	  case Button3:
	    canvasPaneDC.callback(Id_MouseButton3Release,
				  CmdVal(event->xbutton.x, event->xbutton.y));
	    break;
	}
	break;
	
      case MotionNotify:
	if (event->xmotion.state & Button1Mask)
	    canvasPaneDC.callback(Id_MouseButton1Drag,
				  CmdVal(event->xbutton.x, event->xbutton.y));
	else if (event->xmotion.state & Button2Mask)
	    canvasPaneDC.callback(Id_MouseButton2Drag,
				  CmdVal(event->xbutton.x, event->xbutton.y));
	else if (event->xmotion.state & Button3Mask)
	    canvasPaneDC.callback(Id_MouseButton3Drag,
				  CmdVal(event->xbutton.x, event->xbutton.y));
	else
	    canvasPaneDC.callback(Id_MouseMove,
				  CmdVal(event->xbutton.x, event->xbutton.y));
        break;
      
      case EnterNotify:
	canvasPaneDC.callback(Id_MouseEnterFocus, 0);
	break;

      case LeaveNotify:
	canvasPaneDC.callback(Id_MouseLeaveFocus, 0);
	break;

      case Expose:
	while (XCheckWindowEvent(XtDisplay(widget), XtWindow(widget),
				 ExposureMask, event) == True)
	    ;		// Consume all the remained exposure events.
	canvasPaneDC.repaintAll();
	break;

      case KeyPress:
	EVkeyPress(widget, client_data, event, boolean);
	break;
    }
}

void
CBcanvasPaneDC(::Widget, XtPointer client_data, XtPointer)
{
    CanvasPaneDC&	canvasPaneDC = *(CanvasPaneDC*)client_data;
  // Invoke initialization routine specific to each DC.
    canvasPaneDC.initializeGraphics();
  // For some DC types, setting plane masks is only possible after realization.
    canvasPaneDC.setLayer(canvasPaneDC.getLayer());
  // Invoke initialization routine overwritten by the user.
    canvasPaneDC.canvasPane().initializeGraphics();
}

/************************************************************************
*  class CanvasPaneDC						*
************************************************************************/
/*
 *  Public member functions
 */
CanvasPaneDC::CanvasPaneDC(CanvasPane& parentCanvasPane,
			   u_int w, u_int h)
    :Object(parentCanvasPane),
     XDC(w != 0 ? w : parentCanvasPane.widget().width(),
	 h != 0 ? h : parentCanvasPane.widget().height(),
	 window().colormap(),
	 XtAllocateGC(window().widget(), 0, 0, 0, 0, 0)),
     _widget(XtVaCreateManagedWidget("TUvCanvasPaneDC",	   // widget name
				     vCanvasWidgetClass,   // widget class 
				     parent().widget(),    // parent widget
				     XtNwidth,		deviceWidth(),
				     XtNheight,		deviceHeight(),
				     XtNborderWidth,	0,
				   // Set window background.
				     XtNbackground,
				         colormap().getUnderlayPixel(u_char(0),
								     0, 0),
				     NULL)),
     _popup(*this, zoomMenu),
     _u_last(0), _v_last(0)
{
    setDeviceSize();
    
    XtAddEventHandler(_widget,			// get mouse events
		      ButtonPressMask	|
		      ButtonReleaseMask	|
		      PointerMotionMask	|
		      EnterWindowMask	|
		      LeaveWindowMask	|
		      ExposureMask      |
		      KeyPressMask,
		      FALSE, EVcanvasPaneDC, (XtPointer)this);
    XtAddCallback(_widget, XtNginitCallback, CBcanvasPaneDC, (XtPointer)this);
}

CanvasPaneDC::~CanvasPaneDC()
{
}

const Object::Widget&
CanvasPaneDC::widget() const
{
    return _widget;
}

DC&
CanvasPaneDC::setSize(u_int width, u_int height, u_int mul, u_int div)
{
    XDC::setSize(width, height, mul, div);
  // Viewport $B$NCf$G$3$N(B widget $B$r>.$5$/$9$k$H$-(B, $B0JA0IA2h$7$?$b$N$N;D(B
  // $B3<$,M>Gr$K;D$k$N$O8+6l$7$$$N$G!"(Bwidget $BA4BN$r%/%j%"$7$F$*$/!#$^$?!"(B
  // $BD>@\(B graphic hardware $B$K%"%/%;%9$9$k(B API $B!J(BXIL $B$J$I!K$H<B9T=g=x$,(B
  // $BF~$lBX$o$k$3$H$rKI$0$?$a!"(BXSync() $B$r8F$V!J(BXDC.cc $B;2>H!K!#(B
    XClearWindow(colormap().display(), drawable());
    XSync(colormap().display(), False);
    setDeviceSize();
    return *this;
}

void
CanvasPaneDC::callback(CmdId id, CmdVal val)
{
    switch (id)
    {
      default:
	parent().callback(id, val);
	return;

      case m_X025:
	*this << x0_25;
	break;
      case m_X05:
	*this << x0_5;
	break;
      case m_X10:
	*this << x1;
	break;
      case m_X15:
	*this << x1_5;
	break;
      case m_X20:
	*this << x2;
	break;
      case m_X40:
	*this << x4;
	break;
    }

    canvasPane().moveDC(log2devU(_u_last), log2devV(_v_last));
    *this << TU::v::repaintAll;
}

/*
 *  Protected member functions
 */
Drawable
CanvasPaneDC::drawable() const
{
    return XtWindow(_widget);
}

void
CanvasPaneDC::initializeGraphics()
{
}

DC&
CanvasPaneDC::repaintUnderlay()
{
    canvasPane().repaintUnderlay();
    return *this;
}

DC&
CanvasPaneDC::repaintOverlay()
{
    canvasPane().repaintOverlay();
    return *this;
}

/*
 *  Private member functions
 */
void
CanvasPaneDC::setDeviceSize()
{
    XtVaSetValues(canvasPane().widget(),
		  XtNchildMinWidth,	deviceWidth(),
		  XtNchildMinHeight,	deviceHeight(),
		  NULL);
}

u_int
CanvasPaneDC::realWidth() const		// widget width: for XDC::clear()
{
    return widget().width();
}

u_int
CanvasPaneDC::realHeight() const	// widget height: for XDC::clear()
{
    return widget().height();
}

}
}
