/*
 *  $BJ?@.(B19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $BF1=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n<T$K$h$k5v2D$J$7$K$3$N%W%m(B
 *  $B%0%i%`$rBh;0<T$X3+<(!$J#@=!$2~JQ!$;HMQ$9$kEy$NCx:n8"$r?/32$9$k9T0Y(B
 *  $B$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *  Copyright 2007
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Author: Toshio UESHIBA
 *
 *  Confidentail and all rights reserved.
 *  This program is confidential. Any changing, copying or giving
 *  information about the source code of any part of this software
 *  and/or documents without permission by the authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damages in the use of this program.
 *  
 *  $Id: List++.cc,v 1.3 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/List++.h"

namespace TU
{
/************************************************************************
*  class List<T>							*
************************************************************************/
/*
 *  'i' $B$N0LCV$K(B 'x' $B$rA^F~$7!"$=$NA^F~$5$l$?0LCV$rJV$9!#(B
 */
template <class T> typename List<T>::Iterator
List<T>::insert(Iterator i, T& x)
{
    if (i == end())				// $BKvHx$KA^F~!)(B
	_back = &x;

    if (i == begin())				// $B%j%9%H$N@hF,!)(B
    {
	x._next = _front;			// $B@hF,$KA^F~(B
	_front = &x;
    }
    else
	i._prev->insertNext(&x);		// $B!V<jA0$N<!!W$KA^F~(B

    return i;
}

/*
 *  'i' $B$N0LCV$K$"$kMWAG$r:o=|$7!":o=|$5$l$?MWAG$X$N;2>H$rJV$9!#(B
 */
template <class T> typename List<T>::reference
List<T>::erase(Iterator i)
{
    T&	x = *i;
    if (&x == _back)				// $B%j%9%H$NKvHx!)(B
	_back = i._prev;			// $BKvHx$NMWAG$r:o=|(B
    if (&x == _front)				// $B%j%9%H$N@hF,!)(B
	_front = _front->_next;			// $B@hF,$NMWAG$r:o=|(B
    else
	i._prev->eraseNext();			// $B!V<jA0$N<!!W$r:o=|(B
    
    return x;
}

/*
 *  'x' $B$HF1$8%*%V%8%'%/%H!J9b!9#1$D$7$+$J$$$O$:!K$r:o=|$9$k!#(B
 */
template <class T> void
List<T>::remove(const T& x)
{
    for (Iterator i = begin(); i != end(); )
    {
	Iterator next = i;
	++next;
	if (i.operator ->() == &x)
	{
	    erase(i);
	    return;
	}
	i = next;
    }
}
 
}
