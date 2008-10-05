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
 *  $Id: CameraWithFocalLength.cc,v 1.12 2008-10-05 23:25:17 ueshiba Exp $
 */
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class CameraWithFocalLength						*
************************************************************************/
CameraBase&
CameraWithFocalLength::setProjection(const Matrix34d& PP)
{
    Matrix33d			Nt = PP(0, 0, 3, 3);
    Nt[0] *= 0.5;
    Nt[1] *= 0.5;
    SVDecomposition<double>	svd(Nt);
    if (svd[2] < 0.0)
	throw std::invalid_argument("CameraWithFocalLength::setProjection: cannot extract camera rotation due to nevative singular value!");
    const Matrix<double>&	RRt = svd.Vt().trns() * svd.Ut();
    const double		p = RRt[0]*Nt[0] + RRt[1]*Nt[1],
				q = RRt[2]*Nt[2];
    Vector3d			tc;
    tc[0] = -PP[0][3] / p;
    tc[1] = -PP[1][3] / p;
    tc[2] = -PP[2][3] / q;
    setFocalLength(p / q).setRotation(RRt).setTranslation(tc * RRt);

    return *this;
}

const CameraBase::Intrinsic&
CameraWithFocalLength::intrinsic() const
{
    return _intrinsic;
}

CameraBase::Intrinsic&
CameraWithFocalLength::intrinsic()
{
    return _intrinsic;
}

/************************************************************************
*  class CameraWithFocalLength::Intrinsic				*
************************************************************************/
//! canonical$B2hA|:BI87O$K$*$$$FI=8=$5$l$?Ej1FE@$N2hA|:BI87O$K$*$1$k0LCV$r5a$a$k!%(B
/*!
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	x$B$N2hA|:BI87O$K$*$1$k(B2$B<!850LCV!$$9$J$o$A(B
		\f$\TUvec{u}{} = k\TUvec{x}{}\f$
*/
Point2d
CameraWithFocalLength::Intrinsic::operator ()(const Point2d& x) const
{
    return Point2d(_k * x[0], _k * x[1]);
}

//! $BFbIt%Q%i%a!<%?$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	$BEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x1$B%d%3%S9TNs!$$9$J$o$A(B
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}} = \TUvec{x}{}
		\f$
*/
Matrix<double>
CameraWithFocalLength::Intrinsic::jacobianK(const Point2d& x) const
{
    Matrix<double>	J(2, 1);
    J[0][0] = x[0];
    J[1][0] = x[1];

    return J;
}

//! canonical$B2hA|:BI8$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	$BEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x2$B%d%3%S9TNs!$$9$J$o$A(B
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}} =
		k\TUvec{I}{2\times 2}
		\f$
*/
Matrix22d
CameraWithFocalLength::Intrinsic::jacobianXC(const Point2d& x) const
{
    Matrix22d	J;
    return J.diag(_k);
}
    
//! $B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV$r(Bcanonical$B2hA|:BI87O$KD>$9!%(B
/*!
  \param u	$B2hA|:BI87O$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	canonical$B2hA|:BI87O$K$*$1$k(Bu$B$N(B2$B<!850LCV!$$9$J$o$A(B
		\f$\TUvec{x}{} = k^{-1}\TUvec{u}{}\f$
*/
Point2d
CameraWithFocalLength::Intrinsic::xcFromU(const Point2d& u) const
{
    return Point2d(u[0] / _k, u[1] / _k);
}

//! $BFbIt%Q%i%a!<%?9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs!$$9$J$o$A(B
		\f$
		\TUvec{K}{} =
		\TUbeginarray{ccc} k & & \\ & k & \\ & & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithFocalLength::Intrinsic::K() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = _k;
    mat[2][2] = 1.0;

    return mat;
}
    
//! $BFbIt%Q%i%a!<%?9TNs$NE>CV$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV!$$9$J$o$A(B
		\f$
		\TUtvec{K}{} =
		\TUbeginarray{ccc} k & & \\ & k & \\ & & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithFocalLength::Intrinsic::Kt() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = _k;
    mat[2][2] = 1.0;

    return mat;
}
    
//! $BFbIt%Q%i%a!<%?9TNs$N5U9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$N5U9TNs!$$9$J$o$A(B
		\f$
		\TUinv{K}{} =
		\TUbeginarray{ccc} k^{-1} & & \\ & k^{-1} & \\ & & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithFocalLength::Intrinsic::Kinv() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = 1.0 / _k;
    mat[2][2] = 1.0;

    return mat;
}
    
//! $BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs!$$9$J$o$A(B
		\f$
		\TUtinv{K}{} =
		\TUbeginarray{ccc} k^{-1} & & \\ & k^{-1} & \\ & & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithFocalLength::Intrinsic::Ktinv() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = 1.0 / _k;
    mat[2][2] = 1.0;

    return mat;
}

//! $BFbIt%Q%i%a!<%?$N<+M3EY$rJV$9!%(B
/*!
  \return	$BFbIt%Q%i%a!<%?$N<+M3EY!$$9$J$o$A(B1
*/
u_int
CameraWithFocalLength::Intrinsic::dof() const
{
    return 1;
}

//! $B>GE@5wN%$rJV$9!%(B
/*!
  \return	$B>GE@5wN%(B
*/
double
CameraWithFocalLength::Intrinsic::k() const
{
    return _k;
}

//! $B>GE@5wN%$r@_Dj$9$k!%(B
/*!
  \param k	$B>GE@5wN%(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraWithFocalLength::Intrinsic::setFocalLength(double k)
{
    _k = k;
    return *this;
}    

//! $BFbIt%Q%i%a!<%?$r;XDj$5$l$?NL$@$199?7$9$k!%(B
/*!
  \param dp	$B99?7NL$rI=$9(B#dof()$B<!85%Y%/%H%k(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraWithFocalLength::Intrinsic::update(const Vector<double>& dp)
{
    _k -= dp[0];
    return *this;
}

//! $BF~NO%9%H%j!<%`$+$i%+%a%i$NFbIt%Q%i%a!<%?$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
std::istream&
CameraWithFocalLength::Intrinsic::get(std::istream& in)
{
    CanonicalCamera::Intrinsic::get(in);
    return in >> _k;
}

//! $B=PNO%9%H%j!<%`$K%+%a%i$NFbIt%Q%i%a!<%?$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
std::ostream&
CameraWithFocalLength::Intrinsic::put(std::ostream& out) const
{
    using namespace	std;
    
    CanonicalCamera::Intrinsic::put(out);
    cerr << "Focal length:    ";
    return out << _k << endl;
}
 
}
