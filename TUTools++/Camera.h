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
 *  $Id: Camera.h,v 1.2 2008-09-10 05:10:32 ueshiba Exp $
 */
#ifndef __TUCamera_h
#define __TUCamera_h

#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  class CameraBase							*
************************************************************************/
//! $B$9$Y$F$NF);kEj1F%+%a%i$N4pDl$H$J$k%/%i%9(B
class CameraBase
{
  public:
  //! $B%+%a%i$NFbIt%Q%i%a!<%?$rI=$9%/%i%9(B
    class Intrinsic
    {
      public:
	virtual ~Intrinsic()						;
	
      // various operations.
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Point2d		xd(const Point2d& xc)		const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& xc)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;

      // calibration matrices.    
	virtual Matrix33d	K()				const	;
	virtual Matrix33d	Kt()				const	;
	virtual Matrix33d	Kinv()				const	;
	virtual Matrix33d	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		k()				const	;
	virtual Point2d		principal()			const	;
	virtual double		aspect()			const	;
	virtual double		skew()				const	;
	virtual double		d1()				const	;
	virtual double		d2()				const	;
	virtual Intrinsic&	setFocalLength(double k)		;
	virtual Intrinsic&	setPrincipal(double u0, double v0)	;
	virtual Intrinsic&	setAspect(double aspect)		;
	virtual Intrinsic&	setSkew(double skew)			;
	virtual Intrinsic&	setIntrinsic(const Matrix33d& K)	;
	virtual Intrinsic&	setDistortion(double d1, double d2)	;

      // parameter updating functions.
	virtual Intrinsic&	update(const Vector<double>& dp)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;
    };
    
  public:
  //! $B0LCV$r86E@$K!$;Q@*$rC10L9TNs$K%;%C%H$7$F=i4|2=(B
    CameraBase() :_t(), _Rt()	{_Rt[0][0] = _Rt[1][1] = _Rt[2][2] = 1.0;}
  //! $B0LCV$H;Q@*$rC10L9TNs$K%;%C%H$7$F=i4|2=(B
  /*!
    \param t	$B%+%a%i0LCV$rI=$9(B3$B<!85%Y%/%H%k!%(B
    \param Rt	$B%+%a%i;Q@*$rI=$9(B3x3$B2sE>9TNs!%(B
  */
    CameraBase(const Point3d& t, const Matrix33d& Rt)	:_t(t), _Rt(Rt)	{}
    virtual ~CameraBase()						;
    
  // various operations in canonical coordinates.
    Point2d		xc(const Point3d& x)			const	;
    Point2d		xcFromU(const Point2d& u)		const	;
    Matrix34d		Pc()					const	;
    Matrix<double>	jacobianPc(const Point3d& x)		const	;
    Matrix23d		jacobianXc(const Point3d& x)		const	;

  // various oeprations in image coordinates.
    Point2d		operator ()(const Point3d& x)		const	;
    Matrix34d		P()					const	;
    Matrix<double>	jacobianP(const Point3d& x)		const	;
    Matrix<double>	jacobianFCC(const Point3d& x)		const	;
    Matrix23d		jacobianX(const Point3d& x)		const	;
    Matrix<double>	jacobianK(const Point3d& x)		const	;
    Matrix22d		jacobianXC(const Point3d& x)		const	;
    virtual CameraBase& setProjection(const Matrix34d& P)		=0;

  // parameter updating functions.
    void		update(const Vector<double>& dp)		;
    void		updateFCC(const Vector<double>& dp)		;
    void		updateIntrinsic(const Vector<double>& dp)	;
    
  // calibration matrices.
    Matrix33d		K()		const	{return intrinsic().K();}
    Matrix33d		Kt()		const	{return intrinsic().Kt();}
    Matrix33d		Kinv()		const	{return intrinsic().Kinv();}
    Matrix33d		Ktinv()		const	{return intrinsic().Ktinv();}

  // extrinsic parameters.
    const Point3d&	t()		const	{return _t;}
    const Matrix33d&	Rt()		const	{return _Rt;}
    CameraBase&		setTranslation(const Point3d& t)	;
    CameraBase&		setRotation(const Matrix33d& Rt)	;

  // intrinsic parameters.
    virtual const Intrinsic&
			intrinsic()	const	= 0;
    virtual Intrinsic&	intrinsic()		= 0;
    u_int		dofIntrinsic()	const	{return intrinsic().dof();}
    double		k()		const	{return intrinsic().k();}
    Point2d		principal()	const	{return intrinsic().principal();}
    double		aspect()	const	{return intrinsic().aspect();}
    double		skew()		const	{return intrinsic().skew();}
    double		d1()		const	{return intrinsic().d1();}
    double		d2()		const	{return intrinsic().d2();}
    CameraBase&		setFocalLength(double k)		;
    CameraBase&		setPrincipal(double u0, double v0)	;
    CameraBase&		setAspect(double aspect)		;
    CameraBase&		setSkew(double skew)			;
    CameraBase&		setIntrinsic(const Matrix33d& K)	;
    CameraBase&		setDistortion(double d1, double d2)	;
    
  // I/O functions.
    std::istream&	get(std::istream& in)			;
    std::ostream&	put(std::ostream& out)		const	;

  private:
    Point3d		_t;			// camera center.
    Matrix33d		_Rt;			// camera orientation.
};

//! 3$B<!856u4VCf$NE@$NA|$N(Bcanonical$B%+%a%i:BI87O$K$*$1$k0LCV$r5a$a$k(B
/*!
  $BA|$O0J2<$N$h$&$K7W;;$5$l$k!%(B
  \f[
    \TUbeginarray{c} x_c \\ y_c \TUendarray = 
    \frac{1}{\TUtvec{r}{z}(\TUvec{x}{} - \TUvec{t}{})}
    \TUbeginarray{c}
      \TUtvec{r}{x}(\TUvec{x}{} - \TUvec{t}{}) \\
      \TUtvec{r}{y}(\TUvec{x}{} - \TUvec{t}{})
    \TUendarray
  \f]
  \param x	3$B<!856u4VCf$NE@$rI=$9(B3$B<!85%Y%/%H%k!%(B
  \return	x$B$NA|$N(Bcanonical$B%+%a%i:BI87O$K$*$1$k0LCV!%(B
*/
inline Point2d
CameraBase::xc(const Point3d& x) const
{
    const Vector<double>&	xx = _Rt * (x - _t);
    return Point2d(xx[0] / xx[2], xx[1] / xx[2]);
}

//! $B2hA|:BI8$K$*$1$kE@$N(B2$B<!850LCV$r(Bcanonical$B%+%a%i:BI87O$KD>$9(B
/*!
  \param u	$B2hA|:BI87O$K$*$1$kE@$N(B2$B<!850LCV!%(B
  \return	canonical$B%+%a%i:BI87O$K$*$1$k(Bu$B$N(B2$B<!850LCV!%(B
*/
inline Point2d
CameraBase::xcFromU(const Point2d& u) const
{
    return intrinsic().xcFromU(u);
}

//! 3$B<!856u4VCf$NE@$NA|$N2hA|:BI87O$K$*$1$k0LCV$r5a$a$k(B
/*!
  \param x	3$B<!856u4VCf$NE@$rI=$9(B3$B<!85%Y%/%H%k!%(B
  \return	x$B$NA|$N2hA|:BI87O$K$*$1$k0LCV!%(B
*/
inline Point2d
CameraBase::operator ()(const Point3d& x) const
{
    return intrinsic()(xc(x));
}

//! 3$B<!85%f!<%/%j%C%I6u4V$+$i2hA|J?LL$X$NEj1F9TNs$r5a$a$k(B
/*!
  \return	$BEj1F9TNs!%(B
*/
inline Matrix34d
CameraBase::P() const
{
    return K() * Pc();
}

//! $B0LCV$r8GDj$7$?$H$-$NFbIt(B/$B30It%Q%i%a!<%?$K4X$9$k(BJacobian$B$r5a$a$k(B
/*!
  \return	
*/
inline Matrix<double>
CameraBase::jacobianFCC(const Point3d& x) const
{
    const Matrix<double>&	J = jacobianP(x);
    return Matrix<double>(J, 0, 3, J.nrow(), J.ncol() - 3);
}

inline Matrix23d
CameraBase::jacobianX(const Point3d& x) const
{
    return intrinsic().jacobianXC(xc(x)) * jacobianXc(x);
}

inline Matrix<double>
CameraBase::jacobianK(const Point3d& x) const
{
    return intrinsic().jacobianK(xc(x));
}

inline Matrix22d
CameraBase::jacobianXC(const Point3d& x) const
{
    return intrinsic().jacobianXC(xc(x));
}

inline void
CameraBase::updateIntrinsic(const Vector<double>& dp)
{
    intrinsic().update(dp);			// update intrinsic parameters.
}

inline void
CameraBase::updateFCC(const Vector<double>& dp)
{
    _Rt *= Matrix<double>::Rt(dp(0, 3));	// update rotation.
    updateIntrinsic(dp(3, dp.dim() - 3));	// update intrinsic parameters.
}

inline void
CameraBase::update(const Vector<double>& dp)
{
    _t -= dp(0, 3);				// update translation.
    updateFCC(dp(3, dp.dim() - 3));		// update other prameters.
}

inline CameraBase&
CameraBase::setTranslation(const Point3d& t)
{
    _t = t;
    return *this;
}

inline CameraBase&
CameraBase::setRotation(const Matrix33d& Rt)
{
    _Rt = Rt;
    return *this;
}

inline CameraBase&
CameraBase::setFocalLength(double k)
{
    intrinsic().setFocalLength(k);
    return *this;
}

inline CameraBase&
CameraBase::setPrincipal(double u0, double v0)
{
    intrinsic().setPrincipal(u0, v0);
    return *this;
}

inline CameraBase&
CameraBase::setAspect(double aspect)
{
    intrinsic().setAspect(aspect);
    return *this;
}

inline CameraBase&
CameraBase::setSkew(double skew)
{
    intrinsic().setSkew(skew);
    return *this;
}

inline CameraBase&
CameraBase::setIntrinsic(const Matrix33d& K)
{
    intrinsic().setIntrinsic(K);
    return *this;
}

inline CameraBase&
CameraBase::setDistortion(double d1, double d2)
{
    intrinsic().setDistortion(d1, d2);
    return *this;
}

inline std::istream&
operator >>(std::istream& in, CameraBase& camera)
{
    return camera.get(in);
}

inline std::ostream&
operator <<(std::ostream& out, const CameraBase& camera)
{
    return camera.put(out);
}

inline std::istream&
operator >>(std::istream& in, CameraBase::Intrinsic& intrinsic)
{
    return intrinsic.get(in);
}

inline std::ostream&
operator <<(std::ostream& out, const CameraBase::Intrinsic& intrinsic)
{
    return intrinsic.put(out);
}

/************************************************************************
*  class CanonicalCamera						*
************************************************************************/
class CanonicalCamera : public CameraBase
{
  public:
    CanonicalCamera()	:CameraBase(), _intrinsic()	{}
    CanonicalCamera(const Point3d& t, const Matrix33d& Rt)
	:CameraBase(t, Rt), _intrinsic()		{}
    CanonicalCamera(const Matrix34d& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}
    
    virtual CameraBase&	setProjection(const Matrix34d& P)		;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;

  private:
    Intrinsic	_intrinsic;
};

/************************************************************************
*  class CameraWithFocalLength						*
************************************************************************/
class CameraWithFocalLength : public CameraBase
{
  public:
    class Intrinsic : public CanonicalCamera::Intrinsic
    {
      public:
	Intrinsic(double k=1.0)	:_k(k)					{}

      // various operations.
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& xc)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;

      // calibration matrices.
	virtual Matrix33d	K()				const	;
	virtual Matrix33d	Kt()				const	;
	virtual Matrix33d	Kinv()				const	;
	virtual Matrix33d	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		k()				const	;
	virtual	CameraBase::Intrinsic&
				setFocalLength(double k)		;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      private:
	double	_k;
    };
    
  public:
    CameraWithFocalLength()	:CameraBase(), _intrinsic()	{}
    CameraWithFocalLength(const Point3d& t,
			  const Matrix33d& Rt, double k=1.0)
	:CameraBase(t, Rt), _intrinsic(k)			{}
    CameraWithFocalLength(const Matrix34d& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&		setProjection(const Matrix34d& P)	;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};

/************************************************************************
*  class CameraWithEuclideanImagePlane					*
************************************************************************/
class CameraWithEuclideanImagePlane : public CameraBase
{
  public:
    class Intrinsic : public CameraWithFocalLength::Intrinsic
    {
      public:
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0)
	    :CameraWithFocalLength::Intrinsic(k), _principal(u0, v0)	{}
	Intrinsic(const CameraWithFocalLength::Intrinsic& intrinsic)
	    :CameraWithFocalLength::Intrinsic(intrinsic),
	     _principal(0.0, 0.0)					{}
	
      // various operations.	
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;
    
      // calibration matrices.	
	virtual Matrix33d	K()				const	;
	virtual Matrix33d	Kt()				const	;
	virtual Matrix33d	Kinv()				const	;
	virtual Matrix33d	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual Point2d		principal()			const	;
	virtual CameraBase::Intrinsic&
				setPrincipal(double u0, double v0)	;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      private:
	Point2d	_principal;
    };
    
  public:
    CameraWithEuclideanImagePlane()	:CameraBase(), _intrinsic()	{}
    CameraWithEuclideanImagePlane(const Point3d&	t,
				  const Matrix33d&	Rt,
				  double		k=1.0,
				  double		u0=0,
				  double		v0=0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0)			{}
    CameraWithEuclideanImagePlane(const Matrix34d& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&	setProjection(const Matrix34d& P)		;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};
    
/************************************************************************
*  class Camera								*
************************************************************************/
class Camera : public CameraBase
{
  public:
    class Intrinsic : public CameraWithEuclideanImagePlane::Intrinsic
    {
      public:
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0,
		  double aspect=1.0, double skew=0.0)
	    :CameraWithEuclideanImagePlane::Intrinsic(k, u0, v0),
	     _k00(aspect * k), _k01(skew * k)				{}
	Intrinsic(const CameraWithEuclideanImagePlane::Intrinsic& intrinsic)
	    :CameraWithEuclideanImagePlane::Intrinsic(intrinsic),
	     _k00(k()), _k01(0.0)					{}
	Intrinsic(const Matrix33d& K)
	    :CameraWithEuclideanImagePlane::Intrinsic(),
	     _k00(k()), _k01(0.0)			{setIntrinsic(K);}
	
      // various operations.
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& xc)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;

      // calibration matrices.
	virtual Matrix33d	K()				const	;
	virtual Matrix33d	Kt()				const	;
	virtual Matrix33d	Kinv()				const	;
	virtual Matrix33d	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		aspect()			const	;
	virtual double		skew()				const	;
	virtual	CameraBase::Intrinsic&
				setFocalLength(double k)		;
	virtual CameraBase::Intrinsic&
				setAspect(double aspect)		;
	virtual CameraBase::Intrinsic&
				setSkew(double skew)			;
	virtual CameraBase::Intrinsic&
				setIntrinsic(const Matrix33d& K)	;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;
    
      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      protected:
		double		k00()			const	{return _k00;}
		double		k01()			const	{return _k01;}
	
      private:
	double	_k00, _k01;
    };
    
  public:
    Camera()	:CameraBase(), _intrinsic()			{}
    Camera(const Point3d&	t,
	   const Matrix33d&	Rt,
	   double		k=1.0,
	   double		u0=0,
	   double		v0=0,
	   double		aspect=1.0,
	   double		skew=0.0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0, aspect, skew)	{}
    Camera(const Matrix34d& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&	setProjection(const Matrix34d& P)		;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};

/************************************************************************
*  class CameraWithDistortion						*
************************************************************************/
class CameraWithDistortion : public CameraBase
{
  public:
    class Intrinsic : public Camera::Intrinsic
    {
      public:
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0,
		  double aspect=1.0, double skew=0.0,
		  double d1=0.0, double d2=0.0)
	    :Camera::Intrinsic(k, u0, v0, aspect, skew),
	     _d1(d1), _d2(d2)						{}
	Intrinsic(const Camera::Intrinsic& intrinsic)
	    :Camera::Intrinsic(intrinsic), _d1(0.0), _d2(0.0)		{}
	Intrinsic(const Matrix33d& K)
	    :Camera::Intrinsic(), _d1(0.0), _d2(0.0)	{setIntrinsic(K);}
	
      // various operations.
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Point2d		xd(const Point2d& xc)		const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& xc)	const	;
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		d1()				const	;
	virtual double		d2()				const	;
	virtual CameraBase::Intrinsic&	
				setDistortion(double d1, double d2)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      private:
	double	_d1, _d2;
    };
    
  public:
    CameraWithDistortion()	:CameraBase(), _intrinsic()		{}
    CameraWithDistortion(const Point3d&		t,
			 const Matrix33d&	Rt,
			 double			k=1.0,
			 double			u0=0,
			 double			v0=0,
			 double			aspect=1.0,
			 double			skew=0.0,
			 double			d1=0.0,
			 double			d2=0.0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0, aspect, skew, d1, d2)	{}
    CameraWithDistortion(const Matrix34d& P,
			 double d1=0.0, double d2=0.0)			;

    virtual CameraBase&		setProjection(const Matrix34d& P)	;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};
 
inline
CameraWithDistortion::CameraWithDistortion(const Matrix34d& P,
					   double d1, double d2)
    :CameraBase(), _intrinsic()
{
    setProjection(P);
    setDistortion(d1, d2);
}

}
#endif	/* !__TUCamera_h */
