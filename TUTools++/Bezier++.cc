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
 *  $Id: Bezier++.cc,v 1.6 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/Bezier++.h"

namespace TU
{
/************************************************************************
*  class BezierCurve<C>							*
************************************************************************/
template <class C> C
BezierCurve<C>::operator ()(T t) const
{
    T		s = 1.0 - t, fact = 1.0;
    int		nCi = 1;
    C		b((*this)[0] * s);
    for (int i = 1; i < degree(); ++i)
    {
	fact *= t;
      /* 
       * Be careful! We cannot use operator "*=" here, because operator "/"
       * must not produce remainder
       */
	nCi = nCi * (degree() - i + 1) / i;
	(b += fact * nCi * (*this)[i]) *= s;
    }
    b += fact * t * (*this)[degree()];
    return b;
}

template <class C> Array<C>
BezierCurve<C>::deCasteljau(T t, u_int r) const
{
    if (r > degree())
	r = degree();

    const T	s = 1.0 - t;
    Array<C>	b_tmp(*this);
    for (int k = 1; k <= r; ++k)
	for (int i = 0; i <= degree() - k; ++i)
	    (b_tmp[i] *= s) += t * b_tmp[i+1];
    b_tmp.resize(degree() - r + 1);
    return b_tmp;
}

template <class C> void
BezierCurve<C>::elevateDegree()
{
    Array<C>	b_tmp(*this);
    Array<C>::resize(degree() + 2);
    (*this)[0] = b_tmp[0];
    for (int i = 1; i < degree(); ++i)
    {
	T	alpha = T(i) / T(degree());
	
	(*this)[i] = alpha * b_tmp[i-1] + (1.0 - alpha) * b_tmp[i];
    }
    (*this)[degree()] = b_tmp[degree()-1];
}

/************************************************************************
*  class BezierSurface<C>						*
************************************************************************/
template <class C>
BezierSurface<C>::BezierSurface(const Array2<Array<C> >& b)
    :Array2<Curve>(b.nrow(), b.ncol())
{
    for (int j = 0; j <= vDegree(); ++j)
	for (int i = 0; i <= uDegree(); ++i)
	    (*this)[j][i] = b[j][i];
}

template <class C> C
BezierSurface<C>::operator ()(T u, T v) const
{
    BezierCurve<C>	vCurve(vDegree());
    for (int j = 0; j <= vDegree(); ++j)
	vCurve[j] = (*this)[j](u);
    return vCurve(v);
}
 
}
