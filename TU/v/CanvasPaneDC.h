/*
 *  平成14-19年（独）産業技術総合研究所 著作権所有
 *  
 *  創作者：植芝俊夫
 *
 *  本プログラムは（独）産業技術総合研究所の職員である植芝俊夫が創作し，
 *  （独）産業技術総合研究所が著作権を所有する秘密情報です．著作権所有
 *  者による許可なしに本プログラムを使用，複製，改変，第三者へ開示する
 *  等の行為を禁止します．
 *  
 *  このプログラムによって生じるいかなる損害に対しても，著作権所有者お
 *  よび創作者は責任を負いません。
 *
 *  Copyright 2002-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the copyright holder are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holder or the creator are not responsible for any
 *  damages caused by using this program.
 *
 *  $Id$  
 */
#ifndef __TU_V_CANVASPANEDC_H
#define __TU_V_CANVASPANEDC_H

#include "TU/v/XDC.h"
#include "TU/v/CanvasPane.h"
#include "TU/v/Menu.h"

namespace TU
{
namespace v
{
/************************************************************************
*  class CanvasPaneDC							*
************************************************************************/
class CanvasPaneDC : public Object, public XDC
{
  public:
    CanvasPaneDC(CanvasPane& parentCanvasPane,
		 size_t width=0, size_t height=0, float zoom=1)		;
    virtual		~CanvasPaneDC()					;
    
    virtual const Widget&	widget()			const	;

    virtual DC&		setSize(size_t width, size_t height, float zoom);
    virtual void	callback(CmdId id, CmdVal val)			;
	    void	grabKeyboard()				const	;

  protected:
    virtual Drawable	drawable()				const	;
    virtual void	initializeGraphics()				;
    virtual DC&		repaintUnderlay()				;
    virtual DC&		repaintOverlay()				;

  private:
    friend void		EVcanvasPaneDC(::Widget,
				       XtPointer client_data,
				       XEvent* event,
				       Boolean*)			;
    friend void		CBcanvasPaneDC(::Widget,
				       XtPointer client_data,
				       XtPointer)			;

    void		setDeviceSize()					;
    virtual size_t	realWidth()				const	;
    virtual size_t	realHeight()				const	;

    const Widget	_widget;		// vCanvasWidget
    Menu		_popup;
    int			_u_last;
    int			_v_last;
};

}
}
#endif	// !__TU_V_CANVASPANEDC_H
