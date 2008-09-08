/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$kEy$NCx(B
 *  $B:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: CameraBase.cc,v 1.12 2008-09-08 08:06:12 ueshiba Exp $
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

//! 3$B<!85%f!<%/%j%C%I6u4V$+$i(Bcanonical$B2hA|J?LL$X$NEj1F9TNs$r5a$a$k(B
/*!
  \return	canonical$B2hA|J?LL$X$NEj1F9TNs!%(B
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

//! $B%+%a%i%Q%i%a!<%?$K4X$9$kEj1FE@$N(Bcanonical$B:BI8$N(B1$B<!HyJ,$r5a$a$k(B
/*!
  \param x	$BBP>]E@$N(B3$B<!850LCV$rI=$9(B3$B<!85%Y%/%H%k!%(B
  \return	x$B$K$*$1$kEj1FE@$N(Bcanonical$B:BI8$N(B1$B<!HyJ,!%(B
*/
Matrix<double>
CameraBase::jacobianPc(const Point3d& x) const
{
    const Vector3d&		dx = x - _t;
    const Vector<double>&	xx = _Rt * dx;
    Matrix<double>		J(2, 6);
    (J[0](0, 3) = (xx[0] / xx[2] * _Rt[2] - _Rt[0])) /= xx[2];
    (J[1](0, 3) = (xx[1] / xx[2] * _Rt[2] - _Rt[1])) /= xx[2];
    J[0](3, 3) = J[0](0, 3) ^ dx;
    J[1](3, 3) = J[1](0, 3) ^ dx;

    return J;
}

//! $BE@$N(B3$B<!850LCV$K4X$9$kEj1FE@$N(Bcanonical$B:BI8$N(B1$B<!HyJ,$r5a$a$k(B
Matrix23d
CameraBase::jacobianXc(const Point3d& x) const
{
    const Vector<double>&	xx = _Rt * (x - _t);
    Matrix23d			J;
    (J[0] = (_Rt[0] - xx[0] / xx[2] * _Rt[2])) /= xx[2];
    (J[1] = (_Rt[1] - xx[1] / xx[2] * _Rt[2])) /= xx[2];

    return J;
}
 
//! $B%+%a%i%Q%i%a!<%?$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B<!HyJ,$r5a$a$k(B
Matrix<double>
CameraBase::jacobianP(const Point3d& x) const
{
    const Point2d&		xx = xc(x);
    const Matrix<double>&	JK = intrinsic().jacobianK(xx);
    Matrix<double>		J(2, 6 + JK.ncol());
    J(0, 0, 2, 6) = intrinsic().jacobianXC(xx) * jacobianPc(x);
    J(0, 6, 2, JK.ncol()) = JK;

    return J;
}
	
std::istream&
CameraBase::get(std::istream& in)
{
    const double	RAD = M_PI / 180.0;
    Vector3d		axis;
    in >> _t >> axis;
    _Rt = Matrix33d::Rt(RAD*axis);
    return intrinsic().get(in);
}

std::ostream&
CameraBase::put(std::ostream& out) const
{
    const double	DEG = 180.0 / M_PI;
    std::cerr << "Position:       ";    out << _t;
    std::cerr << "Rotation(deg.): ";    out << DEG*_Rt.rot2axis() << std::endl;
    return intrinsic().put(out);
}

/************************************************************************
*  class CameraBase::Intrinsic						*
************************************************************************/
CameraBase::Intrinsic::~Intrinsic()
{
}

Point2d
CameraBase::Intrinsic::operator ()(const Point2d& xc) const
{
    return xc;
}

Point2d
CameraBase::Intrinsic::xd(const Point2d& xc) const
{
    return xc;
}

Matrix<double>
CameraBase::Intrinsic::jacobianK(const Point2d& xc) const
{
    return Matrix<double>(2, 0);
}

Matrix22d
CameraBase::Intrinsic::jacobianXC(const Point2d& xc) const
{
    return Matrix22d::I(2);
}
    
Point2d
CameraBase::Intrinsic::xcFromU(const Point2d& u) const
{
    return u;
}

Matrix33d
CameraBase::Intrinsic::K() const
{
    return Matrix33d::I(3);
}
    
Matrix33d
CameraBase::Intrinsic::Kt() const
{
    return Matrix33d::I(3);
}
    
Matrix33d
CameraBase::Intrinsic::Kinv() const
{
    return Matrix33d::I(3);
}
    
Matrix33d
CameraBase::Intrinsic::Ktinv() const
{
    return Matrix33d::I(3);
}

u_int
CameraBase::Intrinsic::dof() const
{
    return 0;
}

double
CameraBase::Intrinsic::k() const
{
    return 1.0;
}

Point2d
CameraBase::Intrinsic::principal() const
{
    return Point2d(0.0, 0.0);
}

double
CameraBase::Intrinsic::aspect() const
{
    return 1.0;
}

double
CameraBase::Intrinsic::skew() const
{
    return 0.0;
}

double
CameraBase::Intrinsic::d1() const
{
    return 0.0;
}

double
CameraBase::Intrinsic::d2() const
{
    return 0.0;
}

CameraBase::Intrinsic&
CameraBase::Intrinsic::setFocalLength(double k)
{
    return *this;				// Do nothing.
}

CameraBase::Intrinsic&
CameraBase::Intrinsic::setPrincipal(double u0, double v0)
{
    return *this;				// Do nothing.
}

CameraBase::Intrinsic&
CameraBase::Intrinsic::setAspect(double aspect)
{
    return *this;				// Do nothing.
}

CameraBase::Intrinsic&
CameraBase::Intrinsic::setSkew(double skew)
{
    return *this;				// Do nothing.
}

CameraBase::Intrinsic&
CameraBase::Intrinsic::setIntrinsic(const Matrix33d& K)
{
    return *this;				// Do nothing.
}

CameraBase::Intrinsic&
CameraBase::Intrinsic::setDistortion(double d1, double d2)
{
    return *this;				// Do nothing.
}
	
CameraBase::Intrinsic&
CameraBase::Intrinsic::update(const Vector<double>& dp)
{
    return *this;				// Do nothing.
}

std::istream&
CameraBase::Intrinsic::get(std::istream& in)
{
    return in;					// Do nothing.
}

std::ostream&
CameraBase::Intrinsic::put(std::ostream& out) const
{
    return out;					// Do nothing.
}
 
}
