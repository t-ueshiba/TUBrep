/*
 *  $BJ?@.(B9$BG/(B $BEE;R5;=QAm9g8&5f=j(B $B?"<G=SIW(B $BCx:n8"=jM-(B
 *
 *  $BCx:n<T$K$h$k5v2D$J$7$K$3$N%W%m%0%i%`$NBh;0<T$X$N3+<(!"J#@=!"2~JQ!"(B
 *  $B;HMQEy$=$NB>$NCx:n?M3J8"$r?/32$9$k9T0Y$r6X;_$7$^$9!#(B
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *
 *  Copyright 1996
 *  Toshio UESHIBA, Electrotechnical Laboratory
 *
 *  All rights reserved.
 *  Any changing, copying or giving information about source programs of
 *  any part of this software and/or documentation without permission of the
 *  authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damage in use of this program.
 */

/*
 *  $Id: Rotation.cc,v 1.3 2003-03-14 02:26:07 ueshiba Exp $
 */
#include "TU/Vector++.h"

namespace TU
{
Rotation::Rotation(int p, int q, double x, double y)
    :_p(p), _q(q), _c(1.0), _s(0.0)
{
    const double	absy = fabs(y);
    if (fabs(x) > absy)
    {
	const double	t = y / x;
	_c = 1 / sqrt(t*t + 1);
	_s = t * _c;
    }
    else if (absy != 0.0)
    {
	const double	t = x / y;
	_s = 1 / sqrt(t*t + 1);
	_c = t * _s;
    }
}

Rotation::Rotation(int p, int q, double theta)
    :_p(p), _q(q), _c(::cos(theta)), _s(::sin(theta))
{
}
 
}
