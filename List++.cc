/*
 *  $Id: List++.cc,v 1.2 2002-07-25 02:38:05 ueshiba Exp $
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
