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
 *  $Id: plane_motion.cc,v 1.2 2002-07-25 02:38:01 ueshiba Exp $
 */
#include "TU/Calib++.h"

namespace TU
{
void
plane_motion(const Matrix<double>& ldata, const Matrix<double>& rdata,
	     Matrix<double>& Rt, Vector<double>& t, Vector<double>& n)
{
  /* compute A matrix (rotation, translation and plane normal are contained) */
    const int		ndata = ldata.nrow();
    Matrix<double>	P(9, 9);
    for (int i = 0; i < ndata; ++i)
    {
	Matrix<double>	Qt(9, 9);
	
	Qt[0](3, 3) -= rdata[i][2] * ldata[i];
	Qt[0](6, 3) += rdata[i][1] * ldata[i];
	Qt[1](0, 3) += rdata[i][2] * ldata[i];
	Qt[1](6, 3) -= rdata[i][0] * ldata[i];
	Qt[2](0, 3) -= rdata[i][1] * ldata[i];
	Qt[2](3, 3) += rdata[i][0] * ldata[i];

	P += Qt.trns() * Qt;
    }

    Matrix<double>	A(3, 3);
    Vector<double>	a((double*)A, 9);
    Vector<double>	evalue(9);
    a = P.eigen(evalue)[8];

#ifdef DEBUG
	std::cerr << "-----------------" << std::endl
		  << "  Eigen values = " << evalue;
#endif

  /* decompose A into Rt, t and n */
    Matrix<double>	AA = A * A.trns();
    Vector<double>	lambda(3);
    Matrix<double>	Vt = AA.eigen(lambda);

    
}
 
}
