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
 *  $Id: Geometry++.cc,v 1.7 2007-11-26 07:55:48 ueshiba Exp $
 */
#include "TU/Geometry++.h"
#include <stdexcept>

namespace TU
{
/************************************************************************
*  class Point2<T>							*
************************************************************************/
template <class T> Point2<T>&
Point2<T>::move(int dir)
{
    switch (dir % 8)
    {
      case 0:
	++(*this)[0];
	break;
      case 1:
      case -7:
	++(*this)[0];
	++(*this)[1];
	break;
      case 2:
      case -6:
	++(*this)[1];
	break;
      case 3:
      case -5:
	--(*this)[0];
	++(*this)[1];
	break;
      case 4:
      case -4:
	--(*this)[0];
	break;
      case 5:
      case -3:
	--(*this)[0];
	--(*this)[1];
	break;
      case 6:
      case -2:
	--(*this)[1];
	break;
      case 7:
      case -1:
	++(*this)[0];
	--(*this)[1];
	break;
    }
    return *this;
}

template <class T> int
Point2<T>::adj(const Point2<T>& p) const
{
    const int	du = int(p[0] - (*this)[0]), dv = int(p[1] - (*this)[1]);

    if (du == 0 && dv == 0)
        return -1;
    switch (du)
    {
      case -1:
      case 0:
      case 1:
        switch (dv)
        {
          case -1:
          case 0:
          case 1:
            return 1;
          default:
            return 0;
        }
        break;
    }
    return 0;
}

template <class T> int
Point2<T>::dir(const Point2<T>& p) const
{
    const int	du = int(p[0] - (*this)[0]), dv = int(p[1] - (*this)[1]);

    if (du == 0 && dv == 0)
        return 4;
    if (dv >= 0)
        if (du > dv)
            return 0;
        else if (du > 0)
            return 1;
        else if (du > -dv)
            return 2;
        else if (dv > 0)
            return 3;
        else
            return -4;
    else
        if (du >= -dv)
            return -1;
        else if (du >= 0)
            return -2;
        else if (du >= dv)
            return -3;
        else
            return -4;
}

template <class T> int
Point2<T>::angle(const Point2<T>& pp, const Point2<T>& pn) const
{
    int dp = pp.dir(*this), ang = dir(pn);
    
    if (dp == 4 || ang == 4)
        return 4;
    else if ((ang -= dp) > 3)
        return ang - 8;
    else if (ang < -4)
        return ang + 8;
    else
        return ang;
}
 
}
