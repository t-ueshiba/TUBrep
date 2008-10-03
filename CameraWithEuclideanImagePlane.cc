/*
 *  $BJ?@.(B14-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n8"=jM-(B
 *  $B<T$K$h$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$k(B
 *  $BEy$N9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 2002-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the copyright holder are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holder or the creator are not responsible for any
 *  damages caused by using this program.
 *  
 *  $Id: CameraWithEuclideanImagePlane.cc,v 1.12 2008-10-03 04:23:37 ueshiba Exp $
 */
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class CameraWithEuclideanImagePlane					*
************************************************************************/
CameraBase&
CameraWithEuclideanImagePlane::setProjection(const Matrix34d& PP)
{
    throw std::runtime_error("CameraWithEuclideanImagePlane::setProjection: Not implemented!!");
    return *this;
}

const CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::intrinsic() const
{
    return _intrinsic;
}

CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::intrinsic()
{
    return _intrinsic;
}

/************************************************************************
*  class CameraWithEuclideanImagePlane::Intrinsic			*
************************************************************************/
//! canonical$B2hA|:BI87O$K$*$$$FI=8=$5$l$?Ej1FE@$N2hA|:BI87O$K$*$1$k0LCV$r5a$a$k!%(B
/*!
  \param xc	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	xc$B$N2hA|:BI87O$K$*$1$k(B2$B<!850LCV!$$9$J$o$A(B
		\f$\TUvec{u}{} = k\TUvec{x}{c} + \TUvec{u}{0}\f$
*/
Point2d
CameraWithEuclideanImagePlane::Intrinsic::operator ()(const Point2d& xc) const
{
    return Point2d(k() * xc[0] + _principal[0], k() * xc[1] + _principal[1]);
}

//! $BFbIt%Q%i%a!<%?$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k(B
/*!
  \param xc	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	$BEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x3	Jacobi$B9TNs!$$9$J$o$A(B
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}} =
		\TUbeginarray{ccc} x_c & 1 &  \\ y_c & & 1 \TUendarray
		\f$
*/
Matrix<double>
CameraWithEuclideanImagePlane::Intrinsic::jacobianK(const Point2d& xc) const
{
    Matrix<double>	J(2, 3);
    J[0][0] = xc[0];
    J[1][0] = xc[1];
    J[0][1] = J[1][2] = 1.0;

    return J;
}

//! $B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV$r(Bcanonical$B2hA|:BI87O$KD>$9!%(B
/*!
  \param u	$B2hA|:BI87O$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	canonical$B2hA|:BI87O$K$*$1$k(Bu$B$N(B2$B<!850LCV!$$9$J$o$A(B
		\f$\TUvec{x}{c} = k^{-1}(\TUvec{u}{} - \TUvec{u}{0})\f$
*/
Point2d
CameraWithEuclideanImagePlane::Intrinsic::xcFromU(const Point2d& u) const
{
    return Point2d((u[0] - _principal[0]) / k(), (u[1] - _principal[1]) / k());
}

//! $BFbIt%Q%i%a!<%?9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs!$$9$J$o$A(B
		\f$
		\TUvec{K}{} =
		\TUbeginarray{ccc} k & & u_0 \\ & k & v_0 \\ & & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithEuclideanImagePlane::Intrinsic::K() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = k();
    mat[0][2] = _principal[0];
    mat[1][2] = _principal[1];
    mat[2][2] = 1.0;

    return mat;
}

//! $BFbIt%Q%i%a!<%?9TNs$NE>CV$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV!$$9$J$o$A(B
		\f$
		\TUtvec{K}{} =
		\TUbeginarray{ccc} k & & \\ & k & \\ u_0 & v_0 & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithEuclideanImagePlane::Intrinsic::Kt() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = k();
    mat[2][0] = _principal[0];
    mat[2][1] = _principal[1];
    mat[2][2] = 1.0;

    return mat;
}

//! $BFbIt%Q%i%a!<%?9TNs$N5U9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$N5U9TNs!$$9$J$o$A(B
		\f$
		\TUinv{K}{} =
		\TUbeginarray{ccc}
		k^{-1} & & -k^{-1}u_0 \\ & k^{-1} & -k^{-1}v_0 \\ & & 1
		\TUendarray
		\f$
*/
Matrix33d
CameraWithEuclideanImagePlane::Intrinsic::Kinv() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = 1.0 / k();
    mat[0][2] = -_principal[0] / k();
    mat[1][2] = -_principal[1] / k();
    mat[2][2] = 1.0;

    return mat;
}

//! $BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs!$$9$J$o$A(B
		\f$
		\TUtinv{K}{} =
		\TUbeginarray{ccc}
		k^{-1} & & \\ & k^{-1} & \\ -k^{-1}u_0 & -k^{-1}v_0 & 1
		\TUendarray
		\f$
*/
Matrix33d
CameraWithEuclideanImagePlane::Intrinsic::Ktinv() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = 1.0 / k();
    mat[2][0] = -_principal[0] / k();
    mat[2][1] = -_principal[1] / k();
    mat[2][2] = 1.0;

    return mat;
}

//! $BFbIt%Q%i%a!<%?$N<+M3EY$rJV$9!%(B
/*!
  \return	$BFbIt%Q%i%a!<%?$N<+M3EY!$$9$J$o$A(B3
*/
u_int
CameraWithEuclideanImagePlane::Intrinsic::dof() const
{
    return 3;
}

//! $B2hA|<gE@$rJV$9!%(B
/*!
  \return	$B2hA|<gE@(B
*/
Point2d
CameraWithEuclideanImagePlane::Intrinsic::principal() const
{
    return _principal;
}

//! $B2hA|<gE@$r@_Dj$9$k!%(B
/*!
  \param u0	$B2hA|<gE@$N2#:BI8(B
  \param v0	$B2hA|<gE@$N=D:BI8(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::Intrinsic::setPrincipal(double u0, double v0)
{
    _principal[0] = u0;
    _principal[1] = v0;
    return *this;
}

//! $BFbIt%Q%i%a!<%?$r;XDj$5$l$?NL$@$199?7$9$k!%(B
/*!
  \param dp	$B99?7NL$rI=$9(B#dof()$B<!85%Y%/%H%k(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::Intrinsic::update(const Vector<double>& dp)
{
    CameraWithFocalLength::Intrinsic::update(dp(0, 1));
    _principal[0] -= dp[1];
    _principal[1] -= dp[2];
    return *this;
}

//! $BF~NO%9%H%j!<%`$+$i%+%a%i$NFbIt%Q%i%a!<%?$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
std::istream&
CameraWithEuclideanImagePlane::Intrinsic::get(std::istream& in)
{
    CameraWithFocalLength::Intrinsic::get(in);
    return in >> _principal;
}

//! $B=PNO%9%H%j!<%`$K%+%a%i$NFbIt%Q%i%a!<%?$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
std::ostream&
CameraWithEuclideanImagePlane::Intrinsic::put(std::ostream& out) const
{
    CameraWithFocalLength::Intrinsic::put(out);
    std::cerr << "Principal point:";
    return out << _principal;
}
 
}
