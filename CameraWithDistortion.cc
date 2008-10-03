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
 *  $Id: CameraWithDistortion.cc,v 1.13 2008-10-03 04:23:37 ueshiba Exp $
 */
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class CameraWithDistortion						*
************************************************************************/
CameraBase&
CameraWithDistortion::setProjection(const Matrix34d& PP)
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
CameraWithDistortion::intrinsic() const
{
    return _intrinsic;
}

CameraBase::Intrinsic&
CameraWithDistortion::intrinsic()
{
    return _intrinsic;
}

/************************************************************************
*  class CameraWithDistortion::Intrinsic				*
************************************************************************/
//! canonical$B:BI87O$K$*$$$FI=8=$5$l$?Ej1FE@$N2hA|:BI87O$K$*$1$k0LCV$r5a$a$k!%(B
/*!
  \param xc	$BEj1FE@$N(Bcanonical$B:BI8$K$*$1$k0LCV$rI=$9(B2$B<!85%Y%/%H%k(B
  \return	xc$B$N2hA|:BI87O$K$*$1$k0LCV!$$9$J$o$A(B
		\f$
		\TUbeginarray{c} \TUvec{u}{} \\ 1 \TUendarray =
		\TUbeginarray{c} {\cal K}(\TUvec{x}{c}) \\ 1 \TUendarray =
		\TUvec{K}{}
		\TUbeginarray{c} \TUbreve{x}{}(\TUvec{x}{c}) \\ 1 \TUendarray
		\f$
*/
Point2d
CameraWithDistortion::Intrinsic::operator ()(const Point2d& xc) const
{
    return Camera::Intrinsic::operator ()(xd(xc));
}

//! canonical$B:BI87O$K$*$$$FI=8=$5$l$?Ej1FE@$KJ|<MOD6J$rIU2C$9$k!%(B
/*!
  \param xc	$BEj1FE@$N(Bcanonical$B:BI8$K$*$1$k0LCV$rI=$9(B2$B<!85%Y%/%H%k(B
  \return	$BJ|<MOD6JIU2C8e$N(Bcanonical$B:BI87O$K$*$1$k0LCV!$$9$J$o$A(B
		\f$
		\TUbreve{x}{} = (1 + d_1 r^2 + d_2 r^4)\TUvec{x}{c},~~
		r = \TUnorm{\TUvec{x}{c}}
		\f$
*/
Point2d
CameraWithDistortion::Intrinsic::xd(const Point2d& xc) const
{
    const double	sqr = xc * xc, tmp = 1.0 + sqr*(_d1 + sqr*_d2);
    return Point2d(tmp * xc[0], tmp * xc[1]);
}

//! canonical$B2hA|:BI8$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k(B
/*!
  \param x	$BBP>]E@$N(B3$B<!850LCV(B
  \return	$BEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x2 Jacobi$B9TNs!$$9$J$o$A(B
		\f$\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{c}}\f$
*/
Matrix22d
CameraWithDistortion::Intrinsic::jacobianXC(const Point2d& xc) const
{
    const double	sqr = xc * xc, tmp = 2.0*(_d1 + 2.0*sqr*_d2);
    Matrix22d		J;
    J[0][0] = J[1][1] = 1.0 + sqr*(_d1 + sqr*_d2);
    J[0][0] += tmp * xc[0] * xc[0];
    J[1][1] += tmp * xc[1] * xc[1];
    J[0][1] = J[1][0] = tmp * xc[0] * xc[1];
    (J[0] *= k00()) += k01() * J[1];
    J[1] *= k();

    return J;
}

//! $BFbIt%Q%i%a!<%?$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k(B
/*!
  $B$?$@$7!$%"%9%Z%/%HHf(Ba$B$H>GE@5wN%(Bk$B$N@Q(Bak, $BHsD>8rOD$_(Bs$B$H>GE@5wN%(Bk$B$N@Q(Bsk$B$r$=$l$>$l(B
  $BBh(B4, $BBh(B5$BHVL\$NFbIt%Q%i%a!<%?$H$7$F07$$!$(Bk, u0, v0, ak, sk, d1, d2$B$N(B7$B%Q%i%a!<%?$K(B
  $B4X$9$k(B1$B3,HyJ,$H$7$F(BJacobian$B$r7W;;$9$k!%(B
  \param xc	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	$BEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x7	Jacobi$B9TNs!$$9$J$o$A(B
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}} =
		\TUbeginarray{ccccccc}
		& 1 & & \breve{x} & \breve{y} &
		r^2(a k x_c + s k y_c) & r^4(a k x_c + s k y_c) \\
		\breve{y} & & 1 & & & r^2 k y_c & r^4 k y_c
		\TUendarray,~~ r = \TUnorm{\TUvec{x}{c}}
		\f$
*/
Matrix<double>
CameraWithDistortion::Intrinsic::jacobianK(const Point2d& xc) const
{
    const Point2d&	xxd = xd(xc);
    Matrix<double>	J(2, 7);
    J[1][0] = J[0][4] = xxd[1];
    J[0][1] = J[1][2] = 1.0;
    J[0][3] = xxd[0];
    const double	sqr = xc * xc;
    J[0][5] = sqr * (k00() * xc[0] + k01() * xc[1]);
    J[1][5] = sqr * (		       k() * xc[1]);
    J[0][6] = sqr * J[0][5];
    J[1][6] = sqr * J[1][5];

    return J;
}

//! $B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV$r(Bcanonical$B2hA|:BI87O$KD>$9!%(B
/*!
  \param u	$B2hA|:BI87O$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	canonical$B2hA|%+%a%i:BI87O$K$*$1$k(Bu$B$N(B2$B<!850LCV!$$9$J$o$A(B
		\f$\TUvec{x}{c} = {\cal K}^{-1}(\TUvec{u}{})\f$
*/
Point2d
CameraWithDistortion::Intrinsic::xcFromU(const Point2d& u) const
{
    const Point2d&	xd = Camera::Intrinsic::xcFromU(u);
    const double	sqr = xd * xd, tmp = 1.0 - sqr*(_d1 + sqr*_d2);
    return Point2d(tmp * xd[0], tmp * xd[1]);
}

//! $BFbIt%Q%i%a!<%?$r;XDj$5$l$?NL$@$199?7$9$k!%(B
/*!
  \param dp	$B99?7NL$rI=$9(B#dof()$B<!85%Y%/%H%k(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraWithDistortion::Intrinsic::update(const Vector<double>& dp)
{
    Camera::Intrinsic::update(dp(0, 5));
    _d1 -= dp[5];
    _d2 -= dp[6];
    return *this;
}

//! $BFbIt%Q%i%a!<%?$N<+M3EY$rJV$9!%(B
/*!
  \return	$BFbIt%Q%i%a!<%?$N<+M3EY!$$9$J$o$A(B7
*/
u_int
CameraWithDistortion::Intrinsic::dof() const
{
    return 7;
}

//! $BJ|<MOD6J$NBh(B1$B78?t$rJV$9!%(B
/*!
  \return	$BJ|<MOD6J$NBh(B1$B78?t(B
*/
double
CameraWithDistortion::Intrinsic::d1() const
{
    return _d1;
}

//! $BJ|<MOD6J$NBh(B2$B78?t$rJV$9!%(B
/*!
  \return	$BJ|<MOD6J$NBh(B2$B78?t(B
*/
double
CameraWithDistortion::Intrinsic::d2() const
{
    return _d2;
}

//! $BJ|<MOD6J78?t$r@_Dj$9$k!%(B
/*!
  \param d1	$BJ|<MOD6J$NBh(B1$B78?t(B
  \param d2	$BJ|<MOD6J$NBh(B2$B78?t(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraWithDistortion::Intrinsic::setDistortion(double d1, double d2)
{
    _d1 = d1;
    _d2 = d2;
    return *this;
}

//! $BF~NO%9%H%j!<%`$+$i%+%a%i$NFbIt%Q%i%a!<%?$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
std::istream&
CameraWithDistortion::Intrinsic::get(std::istream& in)
{
    Camera::Intrinsic::get(in);
    in >> _d1 >> _d2;

    return in;
}

//! $B=PNO%9%H%j!<%`$K%+%a%i$NFbIt%Q%i%a!<%?$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
std::ostream&
CameraWithDistortion::Intrinsic::put(std::ostream& out) const
{
    using namespace	std;
    
    Camera::Intrinsic::put(out);
    cerr << "Distortion-1:    "; out << _d1 << endl;
    cerr << "Distortion-2:    "; out << _d2 << endl;

    return out;
}
 
}
