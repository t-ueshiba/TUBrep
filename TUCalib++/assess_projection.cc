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
 *  $Id$
 */
#include "TU/Calib++.h"

namespace TU
{
void
assess_projection(const Matrix<double>& data, const Matrix<double>& P)
{
    using namespace	std;
	
    const u_int	ndata = data.nrow();
    double	d_sum = 0.0;
    
    cerr << "=== Errror assessment ===" << endl;
    for (int i = 0; i < ndata; ++i)
    {
	Vector<double>	u = P * data[i](0, 4);
	u /= u[2];
	cerr << " -- Point: " << i << " --" << endl;
	double	d = u(0, 2).dist(data[i](4, 2));
	cerr << "  Distance = " << d << " pixel" << endl;
	d_sum += d;
    }
    cerr << " ** Average distance = " << d_sum / ndata << " pixel **" << endl;
}
 
}
