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
 *  $Id: Heap++.h,v 1.4 2007-11-26 07:28:09 ueshiba Exp $
 */
#ifndef __TUHeapPP_h
#define __TUHeapPP_h

#include "TU/Array++.h"

namespace TU
{
/************************************************************************
*  class Heap<T, Compare>						*
************************************************************************/
//! $BJ#?t$NMWAG$r%=!<%H$9$k%R!<%W$rI=$9%/%i%9(B
/*!
  $B%F%s%W%l!<%H%Q%i%a!<%?(BT$B$OMWAG$N7?$r;XDj$9$k!%%F%s%W%l!<%H%Q%i%a!<%?(BCompare
  $B$O(B2$B$D$N(BT$B7?MWAG$,>:=g$KJB$s$G$$$k;~$K(Btrue$B$rJV$94X?t%*%V%8%'%/%H7?$G$"$k!%(B
  $B$?$H$($P!$(B2$B$D$N(Bint$B7?$rHf3S$9$k>l9g$O(B
  \verbatim
  struct ordered
  {
    bool	operator ()(const int& item0, const int& item1) const
		{
		    return item0 < item1;
		}
  };
  \endverbatim
  $B$HDj5A$9$k!%(B
*/
template <class T, class Compare>
class Heap
{
  public:
    Heap(u_int d, Compare compare)	;
    Heap(Array<T>& a, Compare compare)	;

  //! $B8=:_$NMWAG?t$rJV$9(B
  /*!
    \return	$BMWAG?t(B
  */
    u_int	nelements()	const	{return _n;}

  //! $B%R!<%W@hF,$NMWAG$rJV$9(B
  /*!
    \return	$B@hF,$NMWAG(B
  */
    T		head()		const	{return (_n != 0 ? _array[0] : 0);}

    void	add(T item)		;
    T		detach()		;
    
  private:
    void	upheap(int current)	;
    void	downheap(int current)	;
    
    Array<T>		_array;
    u_int		_n;		// # of items in the heap.
    const Compare	_compare;	// function for comparing two items.
};

/************************************************************************
*  Global functions							*
************************************************************************/
template <class T, class Compare> void
sort(Array<T>& a, Compare compare)	;
 
}

#endif	// !__TUHeapPP_h
