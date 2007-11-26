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
 *  $Id: PSTree++.cc,v 1.3 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/PSTree++.h"

namespace TU
{
/************************************************************************
*  class PSTree<S, T, CompareX, CompareY>				*
************************************************************************/
template <class S, class T, class CompareX, class CompareY>
PSTree<S, T, CompareX, CompareY>::PSTree(Array<T>& array,
					 CompareX compareX,
					 CompareY compareY)
    :_array(array), _root(0), _compareX(compareX), _compareY(compareY)
{
    if (array.dim() > 0)
    {
	sort(array, _compareY);
	_root = new Node(array, 0, array.dim(), _compareX);
    }
}

template <class S, class T, class CompareX, class CompareY> const T*
PSTree<S, T, CompareX, CompareY>::closestY(const S& q) const
{
    const Node*	node = _root->closestY(q, _compareX, _compareY);
    return (node != 0 ? &node->p() : 0);
}

template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::inRegion(const S& q, List& list) const
{
    _root->inRegion(q, list, _compareX, _compareY);
}

template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::inRegion(const S& p, const S& q,
					   List& list) const
{
    _root->inRegion(p, q, list, _compareX, _compareY);
}

/************************************************************************
*  class PSTree<S, T, CompareX, CompareY>::Node				*
************************************************************************/
template <class S, class T, class CompareX, class CompareY>
PSTree<S, T, CompareX, CompareY>::Node::Node(Array<T>& array,
					     int il, int ir,
					     CompareX compareX)
    :_p(array[shift(array, il, ir, compareX)]), _b(array[middle(il, ir)]),
     _l(0), _r(0)
{
    int	ib = (il + 1 + ir) / 2;
    if (il + 1 < ib)
	_l = new Node(array, il+1, ib, compareX);
    if (ib < ir)
	_r = new Node(array, ib, ir, compareX);
}

template <class S, class T, class CompareX, class CompareY> int
PSTree<S, T, CompareX, CompareY>::Node::shift(Array<T>& array,
					      int il, int ir,
					      CompareX compareX)
{
    int im = il;
    for (int i = il+1; i < ir; ++i)	// Find the left-most element.
	if (compareX(array[i], array[im]))
	    im = i;

    T	tmp = array[im];
    while (im-- > il)	// Shift all the elements left of im to right.
	array[im+1] = array[im];
    array[il] = tmp;
    
    return il;
}

/*
 *  compareX(_p, q) && compareY(_p, q) $B$rK~$?$9%N!<%I(B _p $B$N$&$A!"(B
 *  $BB>$N$I$N(B _p' $B$KBP$7$F$b(B !compareY(_p, _p') $B$H$J$k$b$N$rJV$9!#(B
 */
template <class S, class T, class CompareX, class CompareY>
const typename PSTree<S, T, CompareX, CompareY>::Node*
PSTree<S, T, CompareX, CompareY>::Node::closestY(const S& q,
						 CompareX compareX,
						 CompareY compareY) const
{
    if (!compareX(_p, q))
	return 0;

    const Node*	node = (_r != 0 ? _r->closestY(q, compareX, compareY) : 0);
    if (node == 0 && _l != 0)
	node = _l->closestY(q, compareX, compareY);
    if (compareY(_p, q) && (node == 0 || !compareY(_p, node->p())))
	return this;
    else
	return node;
}

/*
 *  compareX(_p, q) && compareY(_p, q) $B$rK~$?$9%N!<%I(B _p $BA4$F$r(B
 *  $BNs5s$7$F(B list $B$KJV$9!#(B
 */
template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::Node::inRegion(const S& q, List& list,
						 CompareX compareX,
						 CompareY compareY) const
{
    if (!compareX(_p, q))
	return;

    if (_l != 0)
	_l->inRegion(q, list, compareX, compareY);
    if (_r != 0 && compareY(_b, q))
	_r->inRegion(q, list, compareX, compareY);
    if (compareY(_p, q))
	list.push_front(*new ListNode(_p));
}

/*
 *  !compareX(_p, p) && !compareY(_p, p) &&
 *   compareX(_p, q) &&  compareY(_p, q) $B$rK~$?$9%N!<%I(B _p $BA4$F$r(B
 *  $BNs5s$7$F(B list $B$KJV$9!#(B
 */
template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::Node::inRegion(const S& p, const S& q,
						 List& list,
						 CompareX compareX,
						 CompareY compareY) const
{
    if (!compareX(_p, q))
	return;

    if (_l != 0 && !compareY(_b, p))
	_l->inRegion(p, q, list, compareX, compareY);
    if (_r != 0 && compareY(_b, q))
	_r->inRegion(p, q, list, compareX, compareY);
    if (!compareX(_p, p) && !compareY(_p, p) && compareY(_p, q))
	list.push_front(*new ListNode(_p));
}

template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::Node::print(std::ostream& out,
					      char kind) const
{
    static int	indent = 0;
    
    if (this == 0)
	return;
    
    for (int i = 0; i < indent; ++i)
	out << ' ';
    out << kind << ": " << _p;

    ++indent;
    _l->print(out, 'L');
    _r->print(out, 'R');
    --indent;
}
 
}
