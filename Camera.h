/*
 *  ����9-19�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
 *  
 *  �n��ҁF�A�ŏr�v
 *
 *  �{�v���O�����́i�Ɓj�Y�ƋZ�p�����������̐E���ł���A�ŏr�v���n�삵�C
 *  �i�Ɓj�Y�ƋZ�p���������������쌠�����L����閧���ł��D�n��҂ɂ�
 *  �鋖�Ȃ��ɖ{�v���O�������g�p�C�����C���ρC��O�҂֊J�����铙�̒�
 *  �쌠��N�Q����s�ׂ��֎~���܂��D
 *  
 *  ���̃v���O�����ɂ���Đ����邢���Ȃ鑹�Q�ɑ΂��Ă��C���쌠���L�҂�
 *  ��ёn��҂͐ӔC�𕉂��܂���B
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
 *  $Id: Camera.h,v 1.1 2008-09-08 08:06:12 ueshiba Exp $
 */
#ifndef __TUCamera_h
#define __TUCamera_h

#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  class CameraBase							*
************************************************************************/
//! ���ׂĂ̓������e�J�����̊��ƂȂ�N���X
class CameraBase
{
  public:
  //! �J�����̓����p�����[�^��\���N���X
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
  //! �ʒu�����_�ɁC�p����P�ʍs��ɃZ�b�g���ď�����
    CameraBase() :_t(), _Rt()	{_Rt[0][0] = _Rt[1][1] = _Rt[2][2] = 1.0;}
  //! �ʒu�Ǝp����P�ʍs��ɃZ�b�g���ď�����
  /*!
    \param t	�J�����ʒu��\��3�����x�N�g���D
    \param Rt	�J�����p����\��3x3��]�s��D
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

//! 3������Ԓ��̓_�̑���canonical�J�������W�n�ɂ�����ʒu�����߂�
/*!
  ���͈ȉ��̂悤�Ɍv�Z�����D
  \f[
    \TUbeginarray{c} x_c \\ y_c \TUendarray = 
    \frac{1}{\TUtvec{r}{z}(\TUvec{x}{} - \TUvec{t}{})}
    \TUbeginarray{c}
      \TUtvec{r}{x}(\TUvec{x}{} - \TUvec{t}{}) \\
      \TUtvec{r}{y}(\TUvec{x}{} - \TUvec{t}{})
    \TUendarray
  \f]
  \param x	3������Ԓ��̓_��\��3�����x�N�g���D
  \return	x�̑���canonical�J�������W�n�ɂ�����ʒu�D
*/
inline Point2d
CameraBase::xc(const Point3d& x) const
{
    const Vector<double>&	xx = _Rt * (x - _t);
    return Point2d(xx[0] / xx[2], xx[1] / xx[2]);
}

//! �摜���W�ɂ�����_��2�����ʒu��canonical�J�������W�n�ɒ���
/*!
  \param u	�摜���W�n�ɂ�����_��2�����ʒu�D
  \return	canonical�J�������W�n�ɂ�����u��2�����ʒu�D
*/
inline Point2d
CameraBase::xcFromU(const Point2d& u) const
{
    return intrinsic().xcFromU(u);
}

//! 3������Ԓ��̓_�̑��̉摜���W�n�ɂ�����ʒu�����߂�
/*!
  \param x	3������Ԓ��̓_��\��3�����x�N�g���D
  \return	x�̑��̉摜���W�n�ɂ�����ʒu�D
*/
inline Point2d
CameraBase::operator ()(const Point3d& x) const
{
    return intrinsic()(xc(x));
}

//! 3�������[�N���b�h��Ԃ���摜���ʂւ̓��e�s������߂�
/*!
  \return	���e�s��D
*/
inline Matrix34d
CameraBase::P() const
{
    return K() * Pc();
}

//! �ʒu���Œ肵���Ƃ��̓���/�O���p�����[�^�Ɋւ���Jacobian�����߂�
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
