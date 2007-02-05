/*
 *  $Id
 */
#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  class Normalize							*
************************************************************************/
//! $B@55,2=JQ499TNs$rJV$9!%(B
/*!
  (J\(Breturn	$BJQ499TNs(B:
		(J\(Bf$
		(J\(BTUvec{T}{} = 
		(J\(BTUbeginarray{ccc}
		 s^{-1} (J\(BTUvec{I}{d} & -s^{-1}(J\(BTUvec{c}{} (J\\(B (J\(BTUtvec{0}{d} & 1
		(J\(BTUendarray
		(J\(Bf$
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
  (J\(Breturn	$BJQ49$NE>CV9TNs(B:
		(J\(Bf$
		(J\(BTUtvec{T}{} = 
		(J\(BTUbeginarray{ccc}
		 s^{-1} (J\(BTUvec{I}{d} & (J\(BTUvec{0}{d} (J\\(B -s^{-1}(J\(BTUtvec{c}{} & 1
		(J\(BTUendarray
		(J\(Bf$
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
  (J\(Breturn	$BJQ49$N5U9TNs(B:
		(J\(Bf$
		(J\(BTUinv{T}{} = 
		(J\(BTUbeginarray{ccc}
		 s (J\(BTUvec{I}{d} & (J\(BTUvec{c}{} (J\\(B (J\(BTUtvec{0}{d} & 1
		(J\(BTUendarray
		(J\(Bf$
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
  (J\(Breturn	$BJQ49$N5U9TNs$NE>CV(B:
		(J\(Bf$
		(J\(BTUtinv{T}{} = 
		(J\(BTUbeginarray{ccc}
		 s (J\(BTUvec{I}{d} & (J\(BTUvec{0}{d} (J\\(B (J\(BTUtvec{c}{} & 1
		(J\(BTUendarray
		(J\(Bf$
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
