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
 *  $Id: Heap++.cc,v 1.4 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/Heap++.h"
#include <stdexcept>

namespace TU
{
/************************************************************************
*  class Heap<T, Compare>						*
************************************************************************/
//! $B6u$N%R!<%W$r:n$k(B
/*!
  \param d		$B%R!<%W$N:GBgMWAG?t(B
  \param compare	$BHf3S4X?t%*%V%8%'%/%H(B
*/
template <class T, class Compare>
Heap<T, Compare>::Heap(u_int d, Compare compare)
    :_array(d), _n(0), _compare(compare)
{
}

//! 1$B<!85G[Ns$+$i%R!<%W$r:n$k(B
/*!
  \param a		1$B<!85G[Ns(B
  \param compare	$BHf3S4X?t%*%V%8%'%/%H(B
*/
template <class T, class Compare>
Heap<T, Compare>::Heap(Array<T>& a, Compare compare)
    :_array((T*)a, a.dim()), _n(a.dim()), _compare(compare)
{
    for (int i = _n / 2; --i >= 0; )
	downheap(i);
}

//! $B%R!<%W$KMWAG$rDI2C$9$k(B
/*!
  \param item	$BDI2C$9$kMWAG(B
*/
template <class T, class Compare> void
Heap<T, Compare>::add(T item)
{
    if (_n >= _array.dim())
	throw std::length_error("TU::Heap<T, Compare>::add: Heap is full!!");

    _array[_n] = item;
    upheap(_n++);
}

//! $B%R!<%W@hF,$NMWAG$r<h$j=|$/(B
/*!
  \return	$B<h$j=|$+$l$?MWAG(B
*/
template <class T, class Compare> T
Heap<T, Compare>::detach()
{
    if (_n == 0)
	throw
	    std::out_of_range("TU::Heap<T, Compare>::detach: Heap is empty!!");
    
    T	val = _array[0];			// Store the root value.
    _array[0] = _array[--_n];			// Move the last to the root.
    downheap(0);
    return val;
}

template <class T, class Compare> void
Heap<T, Compare>::upheap(int current)
{
    T	val = _array[current];
    while (current > 0)				// While having a parent...
    {
	int	parent = (current - 1) / 2;	// Index of the parent node.
	if (_compare(val, _array[parent]))
	    break;
	
	_array[current] = _array[parent];	// Pull-down the parent.
	current = parent;			// Go upward.
    }
    _array[current] = val;
}

template <class T, class Compare> void
Heap<T, Compare>::downheap(int current)
{
    T	val = _array[current];
    for (int child; (child = 2 * current + 1) < _n; )
    {
	if (child + 1 < _n && _compare(_array[child], _array[child + 1]))
	    ++child;				// Choose larger child.

	if (_compare(_array[child], val))
	    break;

	_array[current] = _array[child];	// Pull-up the child.
	current = child;			// Go downward.
    }
    _array[current] = val;
}

/************************************************************************
*  Global functions							*
************************************************************************/
//! 1$B<!85G[Ns$N%R!<%W%=!<%H$r9T$&(B
/*!
  \param a		$B%=!<%H$9$kG[Ns(B
  \param compare	$BHf3S4X?t%*%V%8%'%/%H(B
*/
template <class T, class Compare> void
sort(Array<T>& a, Compare compare)
{
    Heap<T, Compare>	heap(a, compare);
    for (int i = a.dim(); i > 0; )
	a[--i] = heap.detach();
}
 
}
