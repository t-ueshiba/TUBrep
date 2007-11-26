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
 *  $Id: ToggleButtonCmd.cc,v 1.3 2007-11-26 08:11:50 ueshiba Exp $
 */
#include "ToggleButtonCmd_.h"
#include <X11/Xaw3d/Toggle.h>

namespace TU
{
namespace v
{
/************************************************************************
*  class ToggleButtonCmd						*
************************************************************************/
ToggleButtonCmd::ToggleButtonCmd(Object& parentObject, const CmdDef& cmd)
    :Cmd(parentObject, cmd.id),
     _widget(parent().widget(), "TUvToggleButtonCmd", cmd),
     _bitmap(cmd.prop == noProp ? 0 :
	     new Bitmap(window().colormap(), (u_char*)cmd.prop))
{
    setValue(cmd.val);
    setDefaultCallback(_widget);
    if (_bitmap != 0)
	XtVaSetValues(_widget, XtNbitmap, _bitmap->xpixmap(), NULL);
}

ToggleButtonCmd::~ToggleButtonCmd()
{
    delete _bitmap;
}

const Object::Widget&
ToggleButtonCmd::widget() const
{
    return _widget;
}

CmdVal
ToggleButtonCmd::getValue() const
{
    Boolean	state;
    XtVaGetValues(_widget, XtNstate, &state, NULL);
    return (state == TRUE ? 1 : 0);
}

void
ToggleButtonCmd::setValue(CmdVal val)
{
    XtVaSetValues(_widget, XtNstate, (val != 0 ? TRUE : FALSE), NULL);
}

}
}
