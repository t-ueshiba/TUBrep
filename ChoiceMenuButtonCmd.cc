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
 *  $Id: ChoiceMenuButtonCmd.cc,v 1.3 2007-11-26 08:11:50 ueshiba Exp $
 */
#include "ChoiceMenuButtonCmd_.h"

namespace TU
{
namespace v
{
/************************************************************************
*  class ChoiceMenuButtonCmd						*
************************************************************************/
ChoiceMenuButtonCmd::ChoiceMenuButtonCmd(Object& parentObject,
					 const CmdDef& cmd)
    :Cmd(parentObject, cmd.id),
     _widget(parent().widget(), "TUvChoiceMenuButtonCmd", cmd),
     _menu(*this, (MenuDef*)cmd.prop)
{
    addCmd(&_menu);
    setValue(getValue());
}

ChoiceMenuButtonCmd::~ChoiceMenuButtonCmd()
{
    detachCmd();
}

const Object::Widget&
ChoiceMenuButtonCmd::widget() const
{
    return _widget;
}

void
ChoiceMenuButtonCmd::callback(CmdId nid, CmdVal nval)
{
    if (nid == _menu.id())
    {
	setValue(nval);
	parent().callback(id(), getValue());
    }
    else
	parent().callback(nid, nval);
}

CmdVal
ChoiceMenuButtonCmd::getValue() const
{
    return _menu.getValue();
}

void
ChoiceMenuButtonCmd::setValue(CmdVal val)
{
    _menu.setValue(val);
}

}
}
