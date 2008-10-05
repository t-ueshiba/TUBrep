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
 *  $Id: Camera.cc,v 1.12 2008-10-05 23:25:16 ueshiba Exp $
 */
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class Camera								*
************************************************************************/
CameraBase&
Camera::setProjection(const Matrix34d& PP)
{
    Matrix33d	KK;		// camera intrinsic parameters.
    KK[0]    = PP[2](0, 3);
    KK[1]    = PP[1](0, 3);
    KK[2]    = PP[0](0, 3);
    QRDecomposition<double>	qr(KK);
    KK[0][0] =  qr.Rt()[2][2];
    KK[0][1] =  qr.Rt()[2][1];
    KK[0][2] = -qr.Rt()[2][0];
    KK[1][0] =  0.0;
    KK[1][1] =  qr.Rt()[1][1];
    KK[1][2] = -qr.Rt()[1][0];
    KK[2][0] =  0.0;
    KK[2][1] =  0.0;
    KK[2][2] = -qr.Rt()[0][0];

    Matrix33d	RRt;		// camera rotation.
    RRt[0]   =  qr.Qt()[2];
    RRt[1]   =  qr.Qt()[1];
    RRt[2]   = -qr.Qt()[0];

    Vector3d	tt;		// camera translation.
    tt[0]    = -PP[0][3];
    tt[1]    = -PP[1][3];
    tt[2]    = -PP[2][3];

  // Negate sign of PP so that KK has positive determinant.
    if (KK[0][0] * KK[1][1] * KK[2][2] < 0.0)
    {
	KK *= -1.0;
	tt *= -1.0;
    }
    
    if (KK[0][0] < 0.0)
    {
	KK[0][0] *= -1.0;
	RRt[0] *= -1.0;
    }
    if (KK[1][1] < 0.0)
    {
	KK[0][1] *= -1.0;
	KK[1][1] *= -1.0;
	RRt[1] *= -1.0;
    }
    if (KK[2][2] < 0.0)
    {
	KK[0][2] *= -1.0;
	KK[1][2] *= -1.0;
	KK[2][2] *= -1.0;
	RRt[2] *= -1.0;
    }
    tt = (KK.inv() * tt) * RRt;

    setIntrinsic(KK).setTranslation(tt).setRotation(RRt);

    return *this;
}

const CameraBase::Intrinsic&
Camera::intrinsic() const
{
    return _intrinsic;
}

CameraBase::Intrinsic&
Camera::intrinsic()
{
    return _intrinsic;
}

/************************************************************************
*  class Camera::Intrinsic						*
************************************************************************/
//! canonical$B:BI87O$K$*$$$FI=8=$5$l$?Ej1FE@$N2hA|:BI87O$K$*$1$k0LCV$r5a$a$k!%(B
/*!
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	x$B$N2hA|:BI87O$K$*$1$k0LCV!$$9$J$o$A(B
		\f$
		\TUbeginarray{c} \TUvec{u}{} \\ 1 \TUendarray =
		\TUvec{K}{}
		\TUbeginarray{c} \TUvec{x}{} \\ 1 \TUendarray
		\f$
*/
Point2d
Camera::Intrinsic::operator ()(const Point2d& x) const
{
    return Point2d(_k00 * x[0] + _k01 * x[1] + principal()[0],
		   k() * x[1] + principal()[1]);
}

//! $BFbIt%Q%i%a!<%?$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  $B$?$@$7!$%"%9%Z%/%HHf(Ba$B$H>GE@5wN%(Bk$B$N@Q(Bak, $BHsD>8rOD$_(Bs$B$H>GE@5wN%(Bk$B$N@Q(Bsk$B$r$=$l$>$l(B
  $BBh(B4, $BBh(B5$BHVL\$NFbIt%Q%i%a!<%?$H$7$F07$$!$(Bk, u0, v0, ak, sk$B$N(B5$B%Q%i%a!<%?$K4X$9$k(B
  1$B3,HyJ,$H$7$F%d%3%S9TNs$r7W;;$9$k!%(B
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	$BEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x5$B%d%3%S9TNs!$$9$J$o$A(B
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}} =
		\TUbeginarray{ccccc}
		& 1 & & x & y \\ y & & 1 & &
		\TUendarray
		\f$
*/
Matrix<double>
Camera::Intrinsic::jacobianK(const Point2d& x) const
{
    Matrix<double>	J(2, 5);
    J[1][0] = J[0][4] = x[1];
    J[0][1] = J[1][2] = 1.0;
    J[0][3] = x[0];

    return J;
}

//! canonical$B2hA|:BI8$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	$BEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x2$B%d%3%S9TNs!$$9$J$o$A(B
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}} =
		\TUbeginarray{cc} ak & sk \\ & k \TUendarray
		\f$
*/
Matrix22d
Camera::Intrinsic::jacobianXC(const Point2d& x) const
{
    Matrix22d	J;
    J[0][0] = _k00;
    J[0][1] = _k01;
    J[1][1] = k();

    return J;
}

//! $B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV$r(Bcanonical$B2hA|:BI87O$KD>$9!%(B
/*!
  \param u	$B2hA|:BI87O$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	canonical$B2hA|:BI87O$K$*$1$k(Bu$B$N(B2$B<!850LCV!$$9$J$o$A(B
		\f$
		\TUbeginarray{c} \TUvec{x}{c} \\ 1 \TUendarray =
		\TUinv{K}{}
		\TUbeginarray{c} \TUvec{u}{}  \\ 1 \TUendarray
		\f$
*/
Point2d
Camera::Intrinsic::xcFromU(const Point2d& u) const
{
    return Point2d((u[0] - principal()[0] -
		    (u[1] - principal()[1]) * _k01 / k()) / _k00,
		   (u[1] - principal()[1]) / k());
}

//! $BFbIt%Q%i%a!<%?9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs!$$9$J$o$A(B
		\f$
		\TUvec{K}{} =
		\TUbeginarray{ccc}
		ak & sk & u_0 \\ & k & v_0 \\ & & 1
		\TUendarray
		\f$
*/
Matrix33d
Camera::Intrinsic::K() const
{
    Matrix33d	mat;
    mat[0][0] = _k00;
    mat[0][1] = _k01;
    mat[0][2] = principal()[0];
    mat[1][1] = k();
    mat[1][2] = principal()[1];
    mat[2][2] = 1.0;

    return mat;
}

//! $BFbIt%Q%i%a!<%?9TNs$NE>CV$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV!$$9$J$o$A(B
		\f$
		\TUtvec{K}{} =
		\TUbeginarray{ccc}
		ak & & \\ sk & k & \\ u_0 & v_0 & 1
		\TUendarray
		\f$
*/
Matrix33d
Camera::Intrinsic::Kt() const
{
    Matrix33d	mat;
    mat[0][0] = _k00;
    mat[1][0] = _k01;
    mat[2][0] = principal()[0];
    mat[1][1] = k();
    mat[2][1] = principal()[1];
    mat[2][2] = 1.0;

    return mat;
}

//! $BFbIt%Q%i%a!<%?9TNs$N5U9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$N5U9TNs!$$9$J$o$A(B
		\f$
		\TUinv{K}{} =
		\TUbeginarray{ccc}
		a^{-1}k^{-1} & -a^{-1}k^{-1}s &
		-a^{-1}k^{-1}(u_0 - s v_0) \\ & k^{-1} & -k^{-1}v_0 \\ & & 1
		\TUendarray
		\f$
*/
Matrix33d
Camera::Intrinsic::Kinv() const
{
    Matrix33d	mat;
    mat[0][0] = 1.0 / _k00;
    mat[0][1] = -_k01 / (_k00 * k());
    mat[0][2] = -principal()[0] * mat[0][0] - principal()[1] * mat[0][1];
    mat[1][1] = 1.0 / k();
    mat[1][2] = -principal()[1] / k();
    mat[2][2] = 1.0;

    return mat;
}

//! $BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs!$$9$J$o$A(B
		\f$
		\TUtinv{K}{} =
		\TUbeginarray{ccc}
		a^{-1}k^{-1} & & \\ -a^{-1}k^{-1}s & k^{-1} & \\
		-a^{-1}k^{-1}(u_0 - s v_0) & -k^{-1}v_0 & 1
		\TUendarray
		\f$
*/
Matrix33d
Camera::Intrinsic::Ktinv() const
{
    Matrix33d	mat;
    mat[0][0] = 1.0 / _k00;
    mat[1][0] = -_k01 / (_k00 * k());
    mat[2][0] = -principal()[0] * mat[0][0] - principal()[1] * mat[1][0];
    mat[1][1] = 1.0 / k();
    mat[2][1] = -principal()[1] / k();
    mat[2][2] = 1.0;

    return mat;
}

//! $BFbIt%Q%i%a!<%?$N<+M3EY$rJV$9!%(B
/*!
  \return	$BFbIt%Q%i%a!<%?$N<+M3EY!$$9$J$o$A(B5
*/
u_int
Camera::Intrinsic::dof() const
{
    return 5;
}

//! $B%"%9%Z%/%HHf$rJV$9!%(B
/*!
  \return	$B%"%9%Z%/%HHf(B
*/
double
Camera::Intrinsic::aspect() const
{
    return _k00 / k();
}

//! $BHsD>8rOD$_$rJV$9!%(B
/*!
  \return	$BHsD>8rOD$_(B
*/
double
Camera::Intrinsic::skew() const
{
    return _k01 / k();
}

//! $B>GE@5wN%$r@_Dj$9$k!%(B
/*!
  \param kk	$B>GE@5wN%(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
Camera::Intrinsic::setFocalLength(double kk)
{
    _k00 *= (kk / k());
    _k01 *= (kk / k());
    return CameraWithFocalLength::Intrinsic::setFocalLength(kk);
}

//! $BM?$($i$l$?FbIt%Q%i%a!<%?9TNs$+$iFbIt%Q%i%a!<%?$r@_Dj$9$k!%(B
/*!
  \param K	3x3$BFbIt%Q%i%a!<%?9TNs(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
Camera::Intrinsic::setIntrinsic(const Matrix33d& K)
{
    setAspect(K[0][0] / K[1][1])
	.setSkew(K[0][1] / K[1][1])
	.setPrincipal(K[0][2]/K[2][2], K[1][2]/K[2][2])
	.setFocalLength(K[1][1]/K[2][2]);

    return *this;
}

//! $B%"%9%Z%/%HHf$r@_Dj$9$k!%(B
/*!
  \param aspect	$B%"%9%Z%/%HHf(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
Camera::Intrinsic::setAspect(double aspect)
{
    _k00 = aspect * k();
    return *this;
}

//! $BHsD>8r@-OD$_$r@_Dj$9$k!%(B
/*!
  \param skew	$BHsD>8r@-OD$_(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
Camera::Intrinsic::setSkew(double skew)
{
    _k01 = skew * k();
    return *this;
}

//! $BFbIt%Q%i%a!<%?$r;XDj$5$l$?NL$@$199?7$9$k!%(B
/*!
  \param dp	$B99?7NL$rI=$9(B#dof()$B<!85%Y%/%H%k(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
Camera::Intrinsic::update(const Vector<double>& dp)
{
    CameraWithEuclideanImagePlane::Intrinsic::update(dp(0, 3));
    _k00 -= dp[3];
    _k01 -= dp[4];
    return *this;
}

//! $BF~NO%9%H%j!<%`$+$i%+%a%i$NFbIt%Q%i%a!<%?$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
std::istream&
Camera::Intrinsic::get(std::istream& in)
{
    CameraWithEuclideanImagePlane::Intrinsic::get(in);
    in >> _k00 >> _k01;
    _k00 *= k();
    _k01 *= k();

    return in;
}

//! $B=PNO%9%H%j!<%`$K%+%a%i$NFbIt%Q%i%a!<%?$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
std::ostream&
Camera::Intrinsic::put(std::ostream& out) const
{
    using namespace	std;
    
    CameraWithEuclideanImagePlane::Intrinsic::put(out);
    cerr << "Aspect ratio:    "; out << aspect() << endl;
    cerr << "Skew:            "; out << skew() << endl;

    return out;
}
 
}
