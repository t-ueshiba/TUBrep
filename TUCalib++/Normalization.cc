/*
 *  $Id$
 */
#include "TU/Calib++.h"

namespace TU
{
/************************************************************************
*  class Normalization							*
************************************************************************/
//! $BM?$($i$l$?E@72$NF1<!:BI8$+$i@55,2=JQ49$r7W;;$9$k(B
/*!
  $B?6I}$NJ?6QCM$,(B1, $B=E?4$,86E@$K$J$k$h$&$J@55,2=JQ49$,7W;;$5$l$k!%(B
  \param points	$BE@72$NF1<!:BI8!%E@$N8D?t$r(BN, $BE@$,B0$9$k6u4V$N<!85$r(Bd
		$B$H$9$k$H!$(BN\f$\times\f$(d+1)$B9TNs$H$7$FM?$($i$l$k!%(B
*/
void
Normalization::initialize(const Matrix<double>& points)
{
    _scale = 0.0;
    _centroid.resize(points.ncol()-1);
    for (int i = 0; i < points.nrow(); ++i)
    {
	_scale += points[i](0, dim()).square();
	_centroid += points[i](0, dim());
    }
    _scale = sqrt(_scale / (points.nrow() * dim()));
    _centroid /= points.nrow();
}

//! $B@55,2=JQ499TNs$rJV$9(B
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
Normalization::T() const
{
    Matrix<double>	TT(dim()+1, dim()+1);
    for (int i = 0; i < dim(); ++i)
    {
	TT[i][i] = 1.0 / _scale;
	TT[i][dim()] = -_centroid[i] / _scale;
    }
    TT[dim()][dim()] = 1.0;

    return TT;
}

//! $B@55,2=JQ49$NE>CV9TNs$rJV$9(B
/*!
  \return	$BJQ49$NE>CV9TNs(B:
		\f$
		\TUtvec{T}{} = 
		\TUbeginarray{ccc}
		 s^{-1} \TUvec{I}{d} & \TUtvec{0}{d} \\ -s^{-1}\TUtvec{c}{} & 1
		\TUendarray
		\f$
*/
Matrix<double>
Normalization::Tt() const
{
    Matrix<double>	TTt(dim()+1, dim()+1);
    for (int i = 0; i < dim(); ++i)
    {
	TTt[i][i] = 1.0 / _scale;
	TTt[dim()][i] = -_centroid[i] / _scale;
    }
    TTt[dim()][dim()] = 1.0;

    return TTt;
}

//! $B@55,2=JQ49$N5U9TNs$rJV$9(B
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
Normalization::Tinv() const
{
    Matrix<double>	TTinv(dim()+1, dim()+1);
    for (int i = 0; i < dim(); ++i)
    {
	TTinv[i][i] = _scale;
	TTinv[i][dim()] = _centroid[i];
    }
    TTinv[dim()][dim()] = 1.0;

    return TTinv;
}

//! $B@55,2=JQ49$N5U9TNs$NE>CV$rJV$9(B
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
Normalization::Ttinv() const
{
    Matrix<double>	TTtinv(dim()+1, dim()+1);
    for (int i = 0; i < dim(); ++i)
    {
	TTtinv[i][i] = _scale;
	TTtinv[dim()][i] = _centroid[i];
    }
    TTtinv[dim()][dim()] = 1.0;

    return TTtinv;
}
 
}
