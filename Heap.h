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
 *  $Id: Heap.h,v 1.2 2008-09-10 05:10:38 ueshiba Exp $
 */
#ifndef __TUHeap_h
#define __TUHeap_h

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

#endif	// !__TUHeap_h
