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
 *  $Id: Normalize.cc,v 1.2 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  class Normalize							*
************************************************************************/
//! $B@55,2=JQ499TNs$rJV$9!%(B
/*!
  \return	$BJQ499TNs(B:
		\f$
		\TUvec{T}{} = 
		\TUbeginarray{ccc}
		 s^{-1} \TUvec{I}{d} & -s^{-1}\TUvec{c}{} \\ \TUtvec{0}{d} & 1
		\TUendarray
		\f$
*/
Matrix<double>
Normalize::T() const
{
    Matrix<double>	TT(spaceDim()+1, spaceDim()+1);
    for (int i = 0; i < spaceDim(); ++i)
    {
	TT[i][i] = 1.0 / _scale;
	TT[i][spaceDim()] = -_centroid[i] / _scale;
    }
    TT[spaceDim()][spaceDim()] = 1.0;

    return TT;
}

//! $B@55,2=JQ49$NE>CV9TNs$rJV$9!%(B
/*!
  \return	$BJQ49$NE>CV9TNs(B:
		\f$
		\TUtvec{T}{} = 
		\TUbeginarray{ccc}
		 s^{-1} \TUvec{I}{d} & \TUvec{0}{d} \\ -s^{-1}\TUtvec{c}{} & 1
		\TUendarray
		\f$
*/
Matrix<double>
Normalize::Tt() const
{
    Matrix<double>	TTt(spaceDim()+1, spaceDim()+1);
    for (int i = 0; i < spaceDim(); ++i)
    {
	TTt[i][i] = 1.0 / _scale;
	TTt[spaceDim()][i] = -_centroid[i] / _scale;
    }
    TTt[spaceDim()][spaceDim()] = 1.0;

    return TTt;
}

//! $B@55,2=JQ49$N5U9TNs$rJV$9!%(B
/*!
  \return	$BJQ49$N5U9TNs(B:
		\f$
		\TUinv{T}{} = 
		\TUbeginarray{ccc}
		 s \TUvec{I}{d} & \TUvec{c}{} \\ \TUtvec{0}{d} & 1
		\TUendarray
		\f$
*/
Matrix<double>
Normalize::Tinv() const
{
    Matrix<double>	TTinv(spaceDim()+1, spaceDim()+1);
    for (int i = 0; i < spaceDim(); ++i)
    {
	TTinv[i][i] = _scale;
	TTinv[i][spaceDim()] = _centroid[i];
    }
    TTinv[spaceDim()][spaceDim()] = 1.0;

    return TTinv;
}

//! $B@55,2=JQ49$N5U9TNs$NE>CV$rJV$9!%(B
/*!
  \return	$BJQ49$N5U9TNs$NE>CV(B:
		\f$
		\TUtinv{T}{} = 
		\TUbeginarray{ccc}
		 s \TUvec{I}{d} & \TUvec{0}{d} \\ \TUtvec{c}{} & 1
		\TUendarray
		\f$
*/
Matrix<double>
Normalize::Ttinv() const
{
    Matrix<double>	TTtinv(spaceDim()+1, spaceDim()+1);
    for (int i = 0; i < spaceDim(); ++i)
    {
	TTtinv[i][i] = _scale;
	TTtinv[spaceDim()][i] = _centroid[i];
    }
    TTtinv[spaceDim()][spaceDim()] = 1.0;

    return TTtinv;
}
 
}
