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
 *  $Id: ListCmd.cc,v 1.6 2009-03-03 00:59:47 ueshiba Exp $  
 */
#include "ListCmd_.h"
#include "vViewport_.h"
#include <X11/Xaw3d/List.h>
#include <X11/Xaw3d/Scrollbar.h>

namespace TU
{
namespace v
{
/************************************************************************
*  callbacks for scrollbarWidget					*
************************************************************************/
static void
CBjumpProc(::Widget, XtPointer This, XtPointer percent)
{
    ListCmd*	vListCmd = (ListCmd*)This;
    vListCmd->setPercent(*(float*)percent);
}

static void
CBscrollProc(::Widget widget, XtPointer This, XtPointer position)
{
    ListCmd*	vListCmd = (ListCmd*)This;
    if ((long)position > 0)
	vListCmd->scroll(1);
    else
	vListCmd->scroll(-1);
}

/************************************************************************
*  class ListCmd							*
************************************************************************/
ListCmd::ListCmd(Object& parentObject, const CmdDef& cmd)
    :Cmd(parentObject, cmd.id),
     _widget(parent().widget(), "TUvListCmd", cmd),
     _list(XtVaCreateManagedWidget("TUvListCmd-list",
				   listWidgetClass,
				   _widget,
				   XtNlist,		cmd.prop,
				   XtNdefaultColumns,	1,
				   XtNforceColumns,	TRUE,
				   XtNverticalList,	TRUE,
				   XtNinternalHeight,	0,
				   Null)),
     _top(0),
     _nitems(0),
     _nitemsShown(cmd.size > 0 ? cmd.size : 10)
{
    setProp(cmd.prop);
    setValue(cmd.val);
    setDefaultCallback(_list);

    ::Widget	scrollbar = XtNameToWidget(_widget, "vertical");
    XtAddCallback(scrollbar, XtNjumpProc, CBjumpProc, this);
    XtAddCallback(scrollbar, XtNscrollProc, CBscrollProc, this);
}

ListCmd::~ListCmd()
{
}

const Object::Widget&
ListCmd::widget() const
{
    return _widget;
}

CmdVal
ListCmd::getValue() const
{
    XawListReturnStruct*	item = XawListShowCurrent(_list);
    return (item->list_index != XAW_LIST_NONE ? item->list_index : -1);
}

void
ListCmd::setValue(CmdVal val)
{
    XawListUnhighlight(_list);
    if (val >= 0)
	XawListHighlight(_list, val);
}

void
ListCmd::setProp(void* prop)
{
    if (prop != 0)
    {
	String*	s = (String*)prop;
	for (_nitems = 0; s[_nitems] != 0; )
	    ++_nitems;
	XawListChange(_list, (String*)prop, 0, 0, TRUE);

	Dimension	height;
	XtVaGetValues(_list, XtNheight, &height, Null);
	XtVaSetValues(_widget,
		      XtNheight, (_nitems > _nitemsShown ?
				  height * _nitemsShown / _nitems : height),
		      Null);
    }
}

void
ListCmd::setPercent(float percent)
{
    Dimension	viewportHeight, listHeight;
    XtVaGetValues(_widget, XtNheight, &viewportHeight, Null);
    XtVaGetValues(_list, XtNheight, &listHeight, Null);
    u_int	nshown = _nitems * viewportHeight / listHeight;
    _top = int(_nitems * percent + 0.5);
    if (_top < 0)
	_top = 0;
    else if (_top > _nitems - nshown)
	_top = _nitems - nshown;
    vViewportSetLocation(_widget, 0.0, (float)_top / (float)_nitems);
}

void
ListCmd::scroll(int n)
{
    setPercent((float)(_top + n) / (float)_nitems);
}

}
}
