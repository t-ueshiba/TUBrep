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
 *  $Id: Timer.cc,v 1.3 2007-11-26 08:11:50 ueshiba Exp $
 */
#include "TU/v/Timer.h"
#include "TU/v/App.h"

namespace TU
{
namespace v
{
/************************************************************************
*  callback for class Timer						*
************************************************************************/
void
CBtimer(XtPointer vTimer, XtIntervalId*)
{
    ((Timer*)vTimer)->tick();
}

/************************************************************************
*  class Timer							*
************************************************************************/
/*
 *  public member functions.
 */
Timer::Timer(Object& vObject, u_long interval)
    :_vObject(vObject), _interval(interval), _id(0)
{
    if (_interval != 0)
	start();
}

Timer::~Timer()
{
    stop();
}

void
Timer::start(u_long interval)
{
    stop();
    _interval = interval;
    if (_interval != 0)
	start();
}

void
Timer::stop()
{
    if (_id != 0)
    {
	XtRemoveTimeOut(_id);
	_id = 0;
    }
}

/*
 *  private member functions.
 */
void
Timer::start()
{
    _id = XtAppAddTimeOut(XtWidgetToApplicationContext(_vObject.widget()),
			  _interval, CBtimer, (XtPointer)this);
}

void
Timer::tick()
{
    _vObject.tick();
    start();
}

}
}
