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
 *  $Id: Rotation.cc,v 1.8 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/Vector++.h"

namespace TU
{
static inline double	sqr(double x)	{return x * x;}
    
//! 2$B<!85D6J?LLFb$G$N2sE>$r@8@.$9$k(B
/*!
  \param p	p$B<4$r;XDj$9$k(Bindex
  \param q	q$B<4$r;XDj$9$k(Bindex
  \param x	$B2sE>3Q$r@8@.$9$k:]$N(Bx$BCM(B
  \param y	$B2sE>3Q$r@8@.$9$k:]$N(By$BCM(B
		\f[
		  \cos\theta = \frac{x}{\sqrt{x^2+y^2}},{\hskip 1em}
		  \sin\theta = \frac{y}{\sqrt{x^2+y^2}}
		\f]
*/
Rotation::Rotation(int p, int q, double x, double y)
    :_p(p), _q(q), _l(1.0), _c(1.0), _s(0.0)
{
    const double	absx = fabs(x), absy = fabs(y);
    _l = (absx > absy ? absx * sqrt(1.0 + sqr(absy/absx))
		      : absy * sqrt(1.0 + sqr(absx/absy)));
    if (_l != 0.0)
    {
	_c = x / _l;
	_s = y / _l;
    }
}

//! 2$B<!85D6J?LLFb$G$N2sE>$r@8@.$9$k(B
/*!
  \param p	p$B<4$r;XDj$9$k(Bindex
  \param q	q$B<4$r;XDj$9$k(Bindex
  \param theta	$B2sE>3Q(B
*/
Rotation::Rotation(int p, int q, double theta)
    :_p(p), _q(q), _l(1.0), _c(::cos(theta)), _s(::sin(theta))
{
}
 
}
