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
 *  $Id: ChoiceFrameCmd.cc,v 1.5 2008-09-10 05:12:00 ueshiba Exp $  
 */
#include "ChoiceFrameCmd_.h"

namespace TU
{
namespace v
{
/************************************************************************
*  class ChoiceFrameCmd							*
************************************************************************/
ChoiceFrameCmd::ChoiceFrameCmd(Object& parentObject, const CmdDef& cmd)
    :FrameCmd(parentObject, cmd)
{
    setValue(cmd.val);
}

ChoiceFrameCmd::~ChoiceFrameCmd()
{
}

void
ChoiceFrameCmd::callback(CmdId nid, CmdVal nval)
{
    if (findChild(nid) != 0)
    {
	setValue(nid);
	parent().callback(id(), getValue());
    }
    else
	parent().callback(nid, nval);
}

CmdVal
ChoiceFrameCmd::getValue() const
{
    const Cmd*	vcmd = findChildWithNonZeroValue();
    return (vcmd != 0 ? vcmd->id() : M_None);
}

void
ChoiceFrameCmd::setValue(CmdVal val)
{
    Cmd*	cvcmd = findChild(getValue());	// current command chosen
    Cmd*	nvcmd = findChild(val);		// next command chosen
    if (nvcmd != cvcmd)
    {
	if (cvcmd != 0)
	    cvcmd->setValue(0);
	if (nvcmd != 0)
	    nvcmd->setValue(1);
    }
}

}
}
