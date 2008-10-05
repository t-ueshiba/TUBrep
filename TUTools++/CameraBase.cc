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
 *  $Id: CameraBase.cc,v 1.15 2008-10-05 23:25:16 ueshiba Exp $
 */
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class CameraBase							*
************************************************************************/
CameraBase::~CameraBase()
{
}

//! 3$B<!85%f!<%/%j%C%I6u4V$+$i(Bcanonical$B2hA|J?LL$X$NEj1F9TNs$r5a$a$k!%(B
/*!
  \return	canonical$B2hA|J?LL$X$NEj1F9TNs!$$9$J$o$A(B
		\f$
		\TUvec{P}{c} = \TUtvec{R}{}
		\TUbeginarray{cc}
		\TUvec{I}{3\times 3} & -\TUvec{t}{}
		\TUendarray
		\f$
*/
Matrix34d
CameraBase::Pc() const
{
    Matrix34d	PP;
    PP(0, 0, 3, 3) = _Rt;
    PP[0][3] = -(_Rt[0] * _t);
    PP[1][3] = -(_Rt[1] * _t);
    PP[2][3] = -(_Rt[2] * _t);

    return PP;
}

//! $B%+%a%i%Q%i%a!<%?$K4X$9$kEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param X	$BBP>]E@$N(B3$B<!850LCV(B
  \return	$BEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x6$B%d%3%S9TNs!$$9$J$o$A(B
		\f$
		\TUbeginarray{cc}
		\TUdisppartial{\TUvec{x}{}}{\TUvec{t}{}} &
		\TUdisppartial{\TUvec{x}{}}{\TUvec{\theta}{}}
		\TUendarray
		\f$
*/
Matrix<double>
CameraBase::jacobianPc(const Point3d& X) const
{
    const Vector3d&		dX = X - _t;
    const Vector<double>&	x  = _Rt * dX;
    Matrix<double>		J(2, 6);
    (J[0](0, 3) = (x[0] / x[2] * _Rt[2] - _Rt[0])) /= x[2];
    (J[1](0, 3) = (x[1] / x[2] * _Rt[2] - _Rt[1])) /= x[2];
    J[0](3, 3) = J[0](0, 3) ^ dX;
    J[1](3, 3) = J[1](0, 3) ^ dX;

    return J;
}

//! $BE@$N(B3$B<!850LCV$K4X$9$kEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param x	$BBP>]E@$N(B3$B<!850LCV(B
  \return	$BEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x3$B%d%3%S9TNs!$$9$J$o$A(B
		\f$\TUdisppartial{\TUvec{x}{}}{\TUvec{X}{}}\f$
*/
Matrix23d
CameraBase::jacobianXc(const Point3d& X) const
{
    const Vector<double>&	x = _Rt * (X - _t);
    Matrix23d			J;
    (J[0] = (_Rt[0] - x[0] / x[2] * _Rt[2])) /= x[2];
    (J[1] = (_Rt[1] - x[1] / x[2] * _Rt[2])) /= x[2];

    return J;
}
 
//! $BA4%+%a%i%Q%i%a!<%?$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param x	$BBP>]E@$N(B3$B<!850LCV(B
  \return	$BEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x(6+#dofIntrinsic())$B%d%3%S9TNs!$(B
		$B$9$J$o$A(B
		\f$
		\TUbeginarray{ccc}
		\TUdisppartial{\TUvec{u}{}}{\TUvec{t}{}} &
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\theta}{}} &
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}
		\TUendarray =
		\TUbeginarray{cc}
		\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}}
		\TUbeginarray{cc}
		\TUdisppartial{\TUvec{x}{}}{\TUvec{t}{}} &
		\TUdisppartial{\TUvec{x}{}}{\TUvec{\theta}{}}
		\TUendarray &
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}
		\TUendarray
		\f$
*/
Matrix<double>
CameraBase::jacobianP(const Point3d& X) const
{
    const Point2d&		x  = xc(X);
    const Matrix<double>&	JK = intrinsic().jacobianK(x);
    Matrix<double>		J(2, 6 + JK.ncol());
    J(0, 0, 2, 6) = intrinsic().jacobianXC(x) * jacobianPc(X);
    J(0, 6, 2, JK.ncol()) = JK;

    return J;
}
	
//! $BF~NO%9%H%j!<%`$+$i%+%a%i$N30It!?FbIt%Q%i%a!<%?$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
std::istream&
CameraBase::get(std::istream& in)
{
    const double	RAD = M_PI / 180.0;
    Vector3d		axis;
    in >> _t >> axis;
    _Rt = Matrix33d::Rt(RAD*axis);
    return intrinsic().get(in);
}

//! $B=PNO%9%H%j!<%`$K%+%a%i$N30It!?FbIt%Q%i%a!<%?$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
std::ostream&
CameraBase::put(std::ostream& out) const
{
    const double	DEG = 180.0 / M_PI;
    std::cerr << "Position:       ";    out << _t;
    std::cerr << "Rotation(deg.): ";    out << DEG*_Rt.rot2axis();
    return intrinsic().put(out);
}

/************************************************************************
*  class CameraBase::Intrinsic						*
************************************************************************/
CameraBase::Intrinsic::~Intrinsic()
{
}

//! canonical$B2hA|:BI87O$K$*$$$FI=8=$5$l$?Ej1FE@$N2hA|:BI87O$K$*$1$k0LCV$r5a$a$k!%(B
/*!
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	x$B$N2hA|:BI87O$K$*$1$k(B2$B<!850LCV!$$9$J$o$A(B
		\f$\TUvec{u}{} = {\cal K}(\TUvec{x}{})\f$
*/
Point2d
CameraBase::Intrinsic::operator ()(const Point2d& x) const
{
    return x;
}

//! canonical$B2hA|:BI87O$K$*$$$FI=8=$5$l$?Ej1FE@$N2hA|:BI87O$KJ|<MOD6J$rIU2C$9$k!%(B
/*!
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	$BJ|<MOD6JIU2C8e$NEj1FE@$N(B2$B<!850LCV(B
*/
Point2d
CameraBase::Intrinsic::xd(const Point2d& x) const
{
    return x;
}

//! $BFbIt%Q%i%a!<%?$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	$BEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x#dofIntrinsic()
		$B%d%3%S9TNs!$$9$J$o$A(B
		\f$\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}\f$
*/
Matrix<double>
CameraBase::Intrinsic::jacobianK(const Point2d& x) const
{
    return Matrix<double>(2, 0);
}

//! canonical$B2hA|:BI8$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param x	canonical$B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	$BEj1FE@$N(Bcanonical$B2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x2$B%d%3%S9TNs!$$9$J$o$A(B
		\f$\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}}\f$
*/
Matrix22d
CameraBase::Intrinsic::jacobianXC(const Point2d& x) const
{
    return Matrix22d::I(2);
}
    
//! $B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV$r(Bcanonical$B2hA|:BI87O$KD>$9!%(B
/*!
  \param u	$B2hA|:BI87O$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	canonical$B2hA|:BI87O$K$*$1$k(Bu$B$N(B2$B<!850LCV!$$9$J$o$A(B
		\f$\TUvec{x}{} = {\cal K}^{-1}(\TUvec{u}{})\f$
*/
Point2d
CameraBase::Intrinsic::xcFromU(const Point2d& u) const
{
    return u;
}

//! $BFbIt%Q%i%a!<%?9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs(B
*/
Matrix33d
CameraBase::Intrinsic::K() const
{
    return Matrix33d::I(3);
}
    
//! $BFbIt%Q%i%a!<%?9TNs$NE>CV$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV(B
*/
Matrix33d
CameraBase::Intrinsic::Kt() const
{
    return Matrix33d::I(3);
}
    
//! $BFbIt%Q%i%a!<%?9TNs$N5U9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$N5U9TNs(B
*/
Matrix33d
CameraBase::Intrinsic::Kinv() const
{
    return Matrix33d::I(3);
}
    
//! $BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs$rJV$9!%(B
/*!
  \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs(B
*/
Matrix33d
CameraBase::Intrinsic::Ktinv() const
{
    return Matrix33d::I(3);
}

//! $BFbIt%Q%i%a!<%?$N<+M3EY$rJV$9!%(B
/*!
  \return	$BFbIt%Q%i%a!<%?$N<+M3EY(B
*/
u_int
CameraBase::Intrinsic::dof() const
{
    return 0;
}

//! $B>GE@5wN%$rJV$9!%(B
/*!
  \return	$B>GE@5wN%(B
*/
double
CameraBase::Intrinsic::k() const
{
    return 1.0;
}

//! $B2hA|<gE@$rJV$9!%(B
/*!
  \return	$B2hA|<gE@(B
*/
Point2d
CameraBase::Intrinsic::principal() const
{
    return Point2d(0.0, 0.0);
}

//! $B%"%9%Z%/%HHf$rJV$9!%(B
/*!
  \return	$B%"%9%Z%/%HHf(B
*/
double
CameraBase::Intrinsic::aspect() const
{
    return 1.0;
}

//! $BHsD>8rOD$_$rJV$9!%(B
/*!
  \return	$BHsD>8rOD$_(B
*/
double
CameraBase::Intrinsic::skew() const
{
    return 0.0;
}

//! $BJ|<MOD6J$NBh(B1$B78?t$rJV$9!%(B
/*!
  \return	$BJ|<MOD6J$NBh(B1$B78?t(B
*/
double
CameraBase::Intrinsic::d1() const
{
    return 0.0;
}

//! $BJ|<MOD6J$NBh(B2$B78?t$rJV$9!%(B
/*!
  \return	$BJ|<MOD6J$NBh(B2$B78?t(B
*/
double
CameraBase::Intrinsic::d2() const
{
    return 0.0;
}

//! $B>GE@5wN%$r@_Dj$9$k!%(B
/*!
  \param k	$B>GE@5wN%(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setFocalLength(double k)
{
    return *this;				// Do nothing.
}

//! $B2hA|<gE@$r@_Dj$9$k!%(B
/*!
  \param u0	$B2hA|<gE@$N2#:BI8(B
  \param v0	$B2hA|<gE@$N=D:BI8(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setPrincipal(double u0, double v0)
{
    return *this;				// Do nothing.
}

//! $B%"%9%Z%/%HHf$r@_Dj$9$k!%(B
/*!
  \param aspect	$B%"%9%Z%/%HHf(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setAspect(double aspect)
{
    return *this;				// Do nothing.
}

//! $BHsD>8r@-OD$_$r@_Dj$9$k!%(B
/*!
  \param skew	$BHsD>8r@-OD$_(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setSkew(double skew)
{
    return *this;				// Do nothing.
}

//! $BJ|<MOD6J78?t0J30$NFbIt%Q%i%a!<%?$r@_Dj$9$k!%(B
/*!
  \param K	3x3$BFbIt%Q%i%a!<%?9TNs(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setIntrinsic(const Matrix33d& K)
{
    return *this;				// Do nothing.
}

//! $BJ|<MOD6J78?t$r@_Dj$9$k!%(B
/*!
  \param d1	$BJ|<MOD6J$NBh(B1$B78?t(B
  \param d2	$BJ|<MOD6J$NBh(B2$B78?t(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setDistortion(double d1, double d2)
{
    return *this;				// Do nothing.
}
	
//! $BFbIt%Q%i%a!<%?$r;XDj$5$l$?NL$@$199?7$9$k!%(B
/*!
  \param dp	$B99?7NL$rI=$9(B#dof()$B<!85%Y%/%H%k(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::update(const Vector<double>& dp)
{
    return *this;				// Do nothing.
}

//! $BF~NO%9%H%j!<%`$+$i%+%a%i$NFbIt%Q%i%a!<%?$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
std::istream&
CameraBase::Intrinsic::get(std::istream& in)
{
    return in;					// Do nothing.
}

//! $B=PNO%9%H%j!<%`$K%+%a%i$NFbIt%Q%i%a!<%?$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
std::ostream&
CameraBase::Intrinsic::put(std::ostream& out) const
{
    return out;					// Do nothing.
}
 
}
