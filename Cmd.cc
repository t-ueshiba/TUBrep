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
 *  $Id: Cmd.cc,v 1.6 2009-03-03 00:59:47 ueshiba Exp $  
 */
#include "TU/v/TUv++.h"
#include "LabelCmd_.h"
#include "SliderCmd_.h"
#include "FrameCmd_.h"
#include "ButtonCmd_.h"
#include "ToggleButtonCmd_.h"
#include "MenuButtonCmd_.h"
#include "ChoiceMenuButtonCmd_.h"
#include "RadioButtonCmd_.h"
#include "ChoiceFrameCmd_.h"
#include "ListCmd_.h"
#include "TextInCmd_.h"
#include <stdexcept>

namespace TU
{
namespace v
{
/************************************************************************
*  Default callback for Cmd						*
************************************************************************/
static void
CBvCmd(Widget, XtPointer This, XtPointer)
{
    Cmd*	vcmd = (Cmd*)This;
    vcmd->callback(vcmd->id(), vcmd->getValue());
}

/************************************************************************
*  class Cmd								*
************************************************************************/
Cmd::Cmd(Object& parentObject, CmdId id)
    :Object(parentObject), _id(id)
{
}

Cmd::~Cmd()
{
}

Cmd*
Cmd::newCmd(Object& parentObject, const CmdDef& cmd)
{
    switch (cmd.type)
    {
      case C_Button:		// Button
	return new ButtonCmd(parentObject, cmd);

      case C_ToggleButton:	// Toggle button
	return new ToggleButtonCmd(parentObject, cmd);

      case C_RadioButton:	// Radio button
	return new RadioButtonCmd(parentObject, cmd);

      case C_Frame:		// General purpose frame
	return new FrameCmd(parentObject, cmd);

      case C_ChoiceFrame:	// Choice frame
	return new ChoiceFrameCmd(parentObject, cmd);

      case C_MenuButton:	// Menu button
	return new MenuButtonCmd(parentObject, cmd);

      case C_ChoiceMenuButton:	// Choie menu button
	return new ChoiceMenuButtonCmd(parentObject, cmd);

      case C_Icon:		// Display only icon
      case C_Label:		// Regular text label
	return new LabelCmd(parentObject, cmd);
	    
      case C_Slider:		// Slider
	return new SliderCmd(parentObject, cmd);

      case C_List:		// List
	return new ListCmd(parentObject, cmd);

      case C_TextIn:		// TextIn
	return new TextInCmd(parentObject, cmd);
    }

    throw std::domain_error("TU::v::Cmd::newCmd: Unknown command type!!");

    return 0;
}

CmdVal
Cmd::getValue() const
{
    return 0;
}

void
Cmd::setValue(CmdVal)
{
}

const char*
Cmd::getString() const
{
    const char*	str;
    XtVaGetValues(widget(), XtNlabel, &str, Null);
    return str;
}

void
Cmd::setString(const char* str)
{
    XtVaSetValues(widget(), XtNlabel, str, Null);
}

void
Cmd::setProp(void*)
{
}

void
Cmd::setDefaultCallback(const Widget& widget)
{
    XtAddCallback(widget, XtNcallback, CBvCmd, (XtPointer)this);
}

}
}
