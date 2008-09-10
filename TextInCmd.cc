/*
 *  $BJ?@.(B14-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n8"=jM-(B
 *  $B<T$K$h$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$k(B
 *  $BEy$N9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
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
 *  $Id: TextInCmd.cc,v 1.5 2008-09-10 05:12:18 ueshiba Exp $  
 */
#include "TextInCmd_.h"
#include "vTextField_.h"

namespace TU
{
namespace v
{
/************************************************************************
*  Default callback for Cmd						*
************************************************************************/
static void
CBtextInCmd(Widget, XtPointer This, XtPointer)
{
    TextInCmd*	vTextInCmd = (TextInCmd*)This;
    vTextInCmd->callback(vTextInCmd->id(), vTextInCmd->getValue());
}

/************************************************************************
*  class TextInCmd							*
************************************************************************/
TextInCmd::TextInCmd(Object& parentObject, const CmdDef& cmd)
    :Cmd(parentObject, cmd.id),
     _widget(parent().widget(), "TUvTextInCmd", cmd)
{
    XtAddCallback(_widget, XtNactivateCallback, CBtextInCmd, this);
}

TextInCmd::~TextInCmd()
{
}

const Object::Widget&
TextInCmd::widget() const
{
    return _widget;
}

void
TextInCmd::setString(const char* str)
{
    XtVaSetValues(_widget, XtNstring, str, NULL);
}

const char*
TextInCmd::getString() const
{
    const char*	str;
    XtVaGetValues(_widget, XtNstring, &str, NULL);
    return str;
}

}
}
