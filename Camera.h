/*
 *  ����14-19�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
 *  
 *  �n��ҁF�A�ŏr�v
 *
 *  �{�v���O�����́i�Ɓj�Y�ƋZ�p�����������̐E���ł���A�ŏr�v���n�삵�C
 *  �i�Ɓj�Y�ƋZ�p���������������쌠�����L����閧���ł��D���쌠���L
 *  �҂ɂ�鋖�Ȃ��ɖ{�v���O�������g�p�C�����C���ρC��O�҂֊J������
 *  ���̍s�ׂ��֎~���܂��D
 *  
 *  ���̃v���O�����ɂ���Đ����邢���Ȃ鑹�Q�ɑ΂��Ă��C���쌠���L�҂�
 *  ��ёn��҂͐ӔC�𕉂��܂���B
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
 *  $Id: Camera.h,v 1.6 2009-09-04 04:01:05 ueshiba Exp $
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
class __PORT CameraBase
{
  public:
  //! �J�����̓����p�����[�^��\���N���X
    class __PORT Intrinsic
    {
      public:
	virtual ~Intrinsic()						;
	
      // various operations.
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Point2d		xd(const Point2d& x)		const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& x)	const	;
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
  //! �ʒu�����_�ɁC�p����P�ʍs��ɃZ�b�g���ď���������D
    CameraBase() :_t(), _Rt()	{_Rt[0][0] = _Rt[1][1] = _Rt[2][2] = 1.0;}

  //! �ʒu�Ǝp�����Z�b�g���ď���������D
  /*!
    \param t	�J�����ʒu��\��3�����x�N�g��
    \param Rt	�J�����p����\��3x3��]�s��
  */
    CameraBase(const Point3d& t, const Matrix33d& Rt)	:_t(t), _Rt(Rt)	{}
    virtual ~CameraBase()						;
    
  // various operations in canonical coordinates.
    Point2d		xc(const Point3d& X)			const	;
    Point2d		xcFromU(const Point2d& u)		const	;
    Matrix34d		Pc()					const	;
    Matrix<double>	jacobianPc(const Point3d& X)		const	;
    Matrix23d		jacobianXc(const Point3d& X)		const	;

  // various oeprations in image coordinates.
    Point2d		operator ()(const Point3d& X)		const	;
    Matrix34d		P()					const	;
    Matrix<double>	jacobianP(const Point3d& X)		const	;
    Matrix<double>	jacobianFCC(const Point3d& X)		const	;
    Matrix23d		jacobianX(const Point3d& X)		const	;
    Matrix<double>	jacobianK(const Point3d& X)		const	;
    Matrix22d		jacobianXC(const Point3d& X)		const	;

  //! ���e�s����Z�b�g����D
  /*!
    \param P	3x4���e�s��
    \return	���̃J����
  */
    virtual CameraBase& setProjection(const Matrix34d& P)		=0;

  // parameter updating functions.
    void		update(const Vector<double>& dp)		;
    void		updateFCC(const Vector<double>& dp)		;
    void		updateIntrinsic(const Vector<double>& dp)	;
    
  // calibration matrices.
  //! �����p�����[�^�s���Ԃ��D
  /*!
    \return	3x3�����p�����[�^�s��
  */
    Matrix33d		K()		const	{return intrinsic().K();}

  //! �����p�����[�^�s��̓]�u��Ԃ��D
  /*!
    \return	3x3�����p�����[�^�s��̓]�u
  */
    Matrix33d		Kt()		const	{return intrinsic().Kt();}

  //! �����p�����[�^�s��̋t�s���Ԃ��D
  /*!
    \return	3x3�����p�����[�^�s��̋t�s��
  */
    Matrix33d		Kinv()		const	{return intrinsic().Kinv();}

  //! �����p�����[�^�s��̓]�u�̋t�s���Ԃ��D
  /*!
    \return	3x3�����p�����[�^�s��̓]�u�̋t�s��
  */
    Matrix33d		Ktinv()		const	{return intrinsic().Ktinv();}

  // extrinsic parameters.
  //! �J�����̈ʒu��Ԃ��D
  /*!
    \return	�J������3�����ʒu
  */
    const Point3d&	t()		const	{return _t;}

  //! �J�����̎p����Ԃ��D
  /*!
    \return	�J�����̎p����\��3x3��]�s��
  */
    const Matrix33d&	Rt()		const	{return _Rt;}
    CameraBase&		setTranslation(const Point3d& t)	;
    CameraBase&		setRotation(const Matrix33d& Rt)	;

  // intrinsic parameters.
  //! �����p�����[�^��Ԃ��D
  /*!
    \return	�����p�����[�^
  */
    virtual const Intrinsic&
			intrinsic()	const	= 0;

  //! �����p�����[�^��Ԃ��D
  /*!
    \return	�����p�����[�^
  */
    virtual Intrinsic&	intrinsic()		= 0;

  //! �����p�����[�^�̎��R�x��Ԃ��D
  /*!
    \return	�����p�����[�^�̎��R�x
  */
    u_int		dofIntrinsic()	const	{return intrinsic().dof();}

  //! �œ_������Ԃ��D
  /*!
    \return	�œ_����
  */
    double		k()		const	{return intrinsic().k();}

  //! �摜��_��Ԃ��D
  /*!
    \return	�摜��_
  */
    Point2d		principal()	const	{return intrinsic().principal();}

  //! �A�X�y�N�g���Ԃ��D
  /*!
    \return	�A�X�y�N�g��
  */
    double		aspect()	const	{return intrinsic().aspect();}

  //! �񒼌�c�݂�Ԃ��D
  /*!
    \return	�񒼌�c��
  */
    double		skew()		const	{return intrinsic().skew();}

  //! ���˘c�Ȃ̑�1�W����Ԃ��D
  /*!
    \return	���˘c�Ȃ̑�1�W��
  */
    double		d1()		const	{return intrinsic().d1();}

  //! ���˘c�Ȃ̑�2�W����Ԃ��D
  /*!
    \return	���˘c�Ȃ̑�2�W��
  */
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

//! 3������Ԓ��̓_�̓��e�_��canonical�摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param X	�Ώۓ_��3�����ʒu
  \return	canonical�摜���W�n�ɂ�����x�̓��e�_�̈ʒu�C���Ȃ킿
		\f$
		\TUvec{x}{} = 
		\frac{1}{\TUtvec{r}{z}(\TUvec{X}{} - \TUvec{t}{})}
		\TUbeginarray{c}
		\TUtvec{r}{x}(\TUvec{X}{} - \TUvec{t}{}) \\
		\TUtvec{r}{y}(\TUvec{X}{} - \TUvec{t}{})
		\TUendarray
		\f$
*/
inline Point2d
CameraBase::xc(const Point3d& X) const
{
    const Vector<double>&	x = _Rt * (X - _t);
    return Point2d(x[0] / x[2], x[1] / x[2]);
}

//! �摜���W�ɂ����铊�e�_��2�����ʒu��canonical�摜���W�n�ɒ����D
/*!
  \param u	�摜���W�n�ɂ����铊�e�_��2�����ʒu
  \return	canonical�摜�J�������W�n�ɂ�����u��2�����ʒu�C���Ȃ킿
		\f$\TUvec{x}{} = {\cal K}^{-1}(\TUvec{u}{})\f$
*/
inline Point2d
CameraBase::xcFromU(const Point2d& u) const
{
    return intrinsic().xcFromU(u);
}

//! 3������Ԓ��̓_�̓��e�_�̉摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param X	�Ώۓ_��3�����ʒu
  \return	X�̓��e�_�̉摜���W�n�ɂ�����ʒu�C���Ȃ킿
		\f$\TUvec{u}{} = {\cal K}(\TUvec{x}{}(\TUvec{X}{}))\f$
*/
inline Point2d
CameraBase::operator ()(const Point3d& X) const
{
    return intrinsic()(xc(X));
}

//! 3�������[�N���b�h��Ԃ���摜���ʂւ̓��e�s������߂�D
/*!
  \return	�摜���ʂւ̓��e�s��C���Ȃ킿
  \f$
    \TUvec{P}{} = \TUvec{K}{}\TUtvec{R}{}
    \TUbeginarray{cc} \TUvec{I}{3\times 3} & -\TUvec{t}{} \TUendarray
  \f$
*/
inline Matrix34d
CameraBase::P() const
{
    return K() * Pc();
}

//! �J�����ʒu�ȊO�̑S�J�����p�����[�^�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  \param X	�Ώۓ_��3�����ʒu
  \return	���e�_�̉摜���W��1�K������\��2x(3+#dofIntrinsic())���R�r�s��C
		���Ȃ킿
		\f$
		\TUbeginarray{ccc}
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\theta}{}} &
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}
		\TUendarray =
		\TUbeginarray{ccc}
		\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}}
		\TUdisppartial{\TUvec{x}{}}{\TUvec{\theta}{}} &
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}
		\TUendarray
		\f$
*/
inline Matrix<double>
CameraBase::jacobianFCC(const Point3d& X) const
{
    const Matrix<double>&	J = jacobianP(X);
    return Matrix<double>(J, 0, 3, J.nrow(), J.ncol() - 3);
}

//! �_��3�����ʒu�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  \param X	�Ώۓ_��3�����ʒu
  \return	���e�_�̉摜���W��1�K������\��2x3���R�r�s��C���Ȃ킿
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{X}{}} =
		\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}}
		\TUdisppartial{\TUvec{x}{}}{\TUvec{X}{}}
		\f$
*/
inline Matrix23d
CameraBase::jacobianX(const Point3d& X) const
{
    return intrinsic().jacobianXC(xc(X)) * jacobianXc(X);
}

//! �����p�����[�^�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�
/*!
  \param X	�Ώۓ_��3�����ʒu
  \return	���e�_�̉摜���W��1�K������\��2x#dofIntrinsic()���R�r�s��C
		���Ȃ킿
		\f$\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}\f$
*/
inline Matrix<double>
CameraBase::jacobianK(const Point3d& X) const
{
    return intrinsic().jacobianK(xc(X));
}

//! canonical�摜���W�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�
/*!
  \param X	�Ώۓ_��3�����ʒu
  \return	���e�_�̉摜���W��1�K������\��2x2���R�r�s��C���Ȃ킿
		\f$\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}}\f$
*/
inline Matrix22d
CameraBase::jacobianXC(const Point3d& X) const
{
    return intrinsic().jacobianXC(xc(X));
}

//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \f$\Delta\TUvec{p}{} = \Delta\TUvec{\kappa}{}\f$�ɑ΂���
  \f$\TUvec{\kappa}{} \leftarrow \TUvec{\kappa}{} - \Delta\TUvec{\kappa}{}\f$
  �ƍX�V����D
  \param dp	�X�V�ʂ�\��#dofIntrinsic()�����x�N�g��
  \return	���̓����p�����[�^
*/
inline void
CameraBase::updateIntrinsic(const Vector<double>& dp)
{
    intrinsic().update(dp);			// update intrinsic parameters.
}

//! �J�����̎p���Ɠ����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \f$\Delta\TUvec{p}{} = [\Delta\TUtvec{\theta}{},
  ~\Delta\TUtvec{\kappa}{}]^\top\f$�ɑ΂���
  \f{eqnarray*}
  \TUtvec{R}{} & \leftarrow &
  \TUtvec{R}{}\TUtvec{R}{}(\Delta\TUvec{\theta}{}) \\
  \TUvec{\kappa}{} & \leftarrow & \TUvec{\kappa}{} - \Delta\TUvec{\kappa}{}
  \f}
  �ƍX�V����D�J�����̈ʒu�͍X�V����Ȃ��D
  \param dp	�X�V�ʂ�\��3+#dofIntrinsic()�����x�N�g��
  \return	���̓����p�����[�^
*/
inline void
CameraBase::updateFCC(const Vector<double>& dp)
{
    _Rt *= Matrix<double>::Rt(dp(0, 3));	// update rotation.
    updateIntrinsic(dp(3, dp.dim() - 3));	// update intrinsic parameters.
}

//! �J�����̊O���^�����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \f$\Delta\TUvec{p}{} = [\Delta\TUtvec{t}{},~\Delta\TUtvec{\theta}{},
  ~\Delta\TUtvec{\kappa}{}]^\top\f$�ɑ΂���
  \f{eqnarray*}
  \TUvec{t}{} & \leftarrow & \TUvec{t}{} - \Delta\TUvec{t}{} \\
  \TUtvec{R}{} & \leftarrow &
  \TUtvec{R}{}\TUtvec{R}{}(\Delta\TUvec{\theta}{}) \\
  \TUvec{\kappa}{} & \leftarrow & \TUvec{\kappa}{} - \Delta\TUvec{\kappa}{}
  \f}
  �ƍX�V����D
  \param dp	�X�V�ʂ�\��6+#dofIntrinsic()�����x�N�g��
  \return	���̓����p�����[�^
*/
inline void
CameraBase::update(const Vector<double>& dp)
{
    _t -= dp(0, 3);				// update translation.
    updateFCC(dp(3, dp.dim() - 3));		// update other prameters.
}

//! �J�����̈ʒu��ݒ肷��D
/*!
  \param t	�J������3�����ʒu
  \return	���̃J����
*/
inline CameraBase&
CameraBase::setTranslation(const Point3d& t)
{
    _t = t;
    return *this;
}

//! �J�����̎p����ݒ肷��D
/*!
  \param Rt	�J�����̎p����\��3x3��]�s��
  \return	���̃J����
*/
inline CameraBase&
CameraBase::setRotation(const Matrix33d& Rt)
{
    _Rt = Rt;
    return *this;
}

//! �œ_������ݒ肷��D
/*!
  \param k	�œ_����
  \return	���̃J����
*/
inline CameraBase&
CameraBase::setFocalLength(double k)
{
    intrinsic().setFocalLength(k);
    return *this;
}

//! �摜��_��ݒ肷��D
/*!
  \param u0	�摜��_�̉����W
  \param v0	�摜��_�̏c���W
  \return	���̃J����
*/
inline CameraBase&
CameraBase::setPrincipal(double u0, double v0)
{
    intrinsic().setPrincipal(u0, v0);
    return *this;
}

//! �A�X�y�N�g���ݒ肷��D
/*!
  \param aspect	�A�X�y�N�g��
  \return	���̃J����
*/
inline CameraBase&
CameraBase::setAspect(double aspect)
{
    intrinsic().setAspect(aspect);
    return *this;
}

//! �񒼌𐫘c�݂�ݒ肷��D
/*!
  \param skew	�񒼌𐫘c��
  \return	���̃J����
*/
inline CameraBase&
CameraBase::setSkew(double skew)
{
    intrinsic().setSkew(skew);
    return *this;
}

//! ���˘c�ȌW���ȊO�̓����p�����[�^��ݒ肷��D
/*!
  \param K	3x3�����p�����[�^�s��
  \return	���̃J����
*/
inline CameraBase&
CameraBase::setIntrinsic(const Matrix33d& K)
{
    intrinsic().setIntrinsic(K);
    return *this;
}

//! ���˘c�ȌW����ݒ肷��D
/*!
  \param d1	���˘c�Ȃ̑�1�W��
  \param d2	���˘c�Ȃ̑�2�W��
  \return	���̃J����
*/
inline CameraBase&
CameraBase::setDistortion(double d1, double d2)
{
    intrinsic().setDistortion(d1, d2);
    return *this;
}

//! ���̓X�g���[������J�����̊O���^�����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param camera	�O���^�����p�����[�^�̓ǂݍ��ݐ�
  \return	in�Ŏw�肵�����̓X�g���[��
*/
inline std::istream&
operator >>(std::istream& in, CameraBase& camera)
{
    return camera.get(in);
}

//! �o�̓X�g���[���ɃJ�����̊O���^�����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \param camera	�O���^�����p�����[�^�̏����o����
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
inline std::ostream&
operator <<(std::ostream& out, const CameraBase& camera)
{
    return camera.put(out);
}

//! ���̓X�g���[������J�����̓����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param camera	�����p�����[�^�̓ǂݍ��ݐ�
  \return	in�Ŏw�肵�����̓X�g���[��
*/
inline std::istream&
operator >>(std::istream& in, CameraBase::Intrinsic& intrinsic)
{
    return intrinsic.get(in);
}

//! �o�̓X�g���[���ɃJ�����̓����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \param camera	�����p�����[�^�̏����o����
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
inline std::ostream&
operator <<(std::ostream& out, const CameraBase::Intrinsic& intrinsic)
{
    return intrinsic.put(out);
}

/************************************************************************
*  class CanonicalCamera						*
************************************************************************/
//! ���ׂĂ̓����p�����[�^���W������l(�œ_�����ƃA�X�y�N�g�䂪1, �񒼌�c�݂ƕ��˘c�ȌW����0, �摜��_�����_�Ɉ�v)�ƂȂ铧�����e�J������\���N���X
class __PORT CanonicalCamera : public CameraBase
{
  public:
  //! �ʒu�����_�ɁC�p����P�ʍs��ɃZ�b�g���ď���������D
    CanonicalCamera()	:CameraBase(), _intrinsic()	{}

  //! �ʒu�Ǝp�����Z�b�g���ď���������D
  /*!
    \param t	�J������3�����ʒu
    \param Rt	�J�����̎p����\��3x3��]�s��
  */
    CanonicalCamera(const Point3d& t, const Matrix33d& Rt)
	:CameraBase(t, Rt), _intrinsic()		{}

  //! ���e�s����Z�b�g���ď���������D
  /*!
    \param P	3x4���e�s��
  */
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
//! �œ_�����ȊO�̓����p�����[�^���W������l�ƂȂ铧�����e�J������\���N���X
class __PORT CameraWithFocalLength : public CameraBase
{
  public:
  //! �œ_�����݂̂��琬������p�����[�^��\���N���X
    class __PORT Intrinsic : public CanonicalCamera::Intrinsic
    {
      public:
      //! �����p�����[�^���Z�b�g���ď���������D
      /*!
	\param k	�œ_����
      */
	Intrinsic(double k=1.0)	:_k(k)					{}

      // various operations.
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& x)	const	;
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
  //! �ʒu�����_�ɁC�p����P�ʍs��ɁC�����p�����[�^���f�t�H���g�l�ɃZ�b�g���ď���������D
    CameraWithFocalLength()	:CameraBase(), _intrinsic()	{}

  //! �O���^�����p�����[�^���Z�b�g���ď���������D
  /*!
    \param t	�J������3�����ʒu
    \param Rt	�J�����̎p����\��3x3��]�s��
    \param k	�œ_����
  */
    CameraWithFocalLength(const Point3d& t,
			  const Matrix33d& Rt, double k=1.0)
	:CameraBase(t, Rt), _intrinsic(k)			{}

  //! ���e�s����Z�b�g���ď���������D
  /*!
    \param P	3x4���e�s��
  */
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
//! �œ_�����Ɖ摜��_�ȊO�̓����p�����[�^���W������l�ƂȂ铧�����e�J������\���N���X
class __PORT CameraWithEuclideanImagePlane : public CameraBase
{
  public:
  //! �œ_�����Ɖ摜��_���琬������p�����[�^��\���N���X
    class __PORT Intrinsic : public CameraWithFocalLength::Intrinsic
    {
      public:
      //! �����p�����[�^���Z�b�g���ď���������D
      /*!
	\param k	�œ_����
	\param u0	�摜��_�̉����W
	\param v0	�摜��_�̏c���W
      */
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0)
	    :CameraWithFocalLength::Intrinsic(k), _principal(u0, v0)	{}

      //! #CameraWithFocalLength�^�J�����̓����p�����[�^���Z�b�g���ď���������D
      /*!
	�摜��_��(0, 0)�ɏ����������D
	\param intrinsic	#CameraWithFocalLength�̓����p�����[�^
      */
	Intrinsic(const CameraWithFocalLength::Intrinsic& intrinsic)
	    :CameraWithFocalLength::Intrinsic(intrinsic),
	     _principal(0.0, 0.0)					{}
	
      // various operations.	
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
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
  //! �ʒu�����_�ɁC�p����P�ʍs��ɁC�����p�����[�^���f�t�H���g�l�ɃZ�b�g���ď���������D
    CameraWithEuclideanImagePlane()	:CameraBase(), _intrinsic()	{}

  //! �O���^�����p�����[�^���Z�b�g���ď���������D
  /*!
    \param t	�J������3�����ʒu
    \param Rt	�J�����̎p����\��3x3��]�s��
    \param k	�œ_����
    \param u0	�摜��_�̉����W
    \param v0	�摜��_�̏c���W
  */
    CameraWithEuclideanImagePlane(const Point3d&	t,
				  const Matrix33d&	Rt,
				  double		k=1.0,
				  double		u0=0,
				  double		v0=0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0)			{}

  //! ���e�s����Z�b�g���ď���������D
  /*!
    \param P	3x4���e�s��
  */
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
//! ���˘c�ȌW���݂̂��W������l(0)�ƂȂ铧�����e�J������\���N���X
class __PORT Camera : public CameraBase
{
  public:
  //! ���˘c�ȌW���ӊO�̑S�����p�����[�^��\���N���X
    class __PORT Intrinsic : public CameraWithEuclideanImagePlane::Intrinsic
    {
      public:
      //! �����p�����[�^���Z�b�g���ď���������D
      /*!
	\param k	�œ_����
	\param u0	�摜��_�̉����W
	\param v0	�摜��_�̏c���W
	\param aspect	�A�X�y�N�g��
	\param skew	�񒼌𐫘c��
      */
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0,
		  double aspect=1.0, double skew=0.0)
	    :CameraWithEuclideanImagePlane::Intrinsic(k, u0, v0),
	     _k00(aspect * k), _k01(skew * k)				{}

      //! #CameraWithEuclideanImagePlane�^�J�����̓����p�����[�^���Z�b�g���ď���������D
      /*!
	�A�X�y�N�g��Ɣ񒼌�c�݂�0�ɏ����������D
	\param intrinsic	#CameraWithEuclideanImagePlane�̓����p�����[�^
      */
	Intrinsic(const CameraWithEuclideanImagePlane::Intrinsic& intrinsic)
	    :CameraWithEuclideanImagePlane::Intrinsic(intrinsic),
	     _k00(k()), _k01(0.0)					{}

      //! �����p�����[�^���Z�b�g���ď���������D
      /*!
	\param K	3x3�����p�����[�^�s��
      */
	Intrinsic(const Matrix33d& K)
	    :CameraWithEuclideanImagePlane::Intrinsic(),
	     _k00(k()), _k01(0.0)			{setIntrinsic(K);}
	
      // various operations.
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& x)	const	;
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
      //! �œ_�����ƃA�X�y�N�g��̐ς�Ԃ��D
      /*!
	\return		�œ_����k�ƃA�X�y�N�g��a�̐�ak
      */
		double		k00()			const	{return _k00;}

      //! �œ_�����Ɣ񒼌�c�݂̐ς�Ԃ��D
      /*!
	\return		�œ_����k�Ɣ񒼌�c��s�̐�sk
      */
		double		k01()			const	{return _k01;}
	
      private:
	double	_k00, _k01;
    };
    
  public:
  //! �ʒu�����_�ɁC�p����P�ʍs��ɁC�����p�����[�^���f�t�H���g�l�ɃZ�b�g���ď���������D
    Camera()	:CameraBase(), _intrinsic()			{}

  //! �O���^�����p�����[�^���Z�b�g���ď���������D
  /*!
    \param t	�J������3�����ʒu
    \param Rt	�J�����̎p����\��3x3��]�s��
    \param k	�œ_����
    \param u0	�摜��_�̉����W
    \param v0	�摜��_�̏c���W
    \param a	�A�X�y�N�g��
    \param s	�񒼌�c��
  */
    Camera(const Point3d&	t,
	   const Matrix33d&	Rt,
	   double		k=1.0,
	   double		u0=0,
	   double		v0=0,
	   double		aspect=1.0,
	   double		skew=0.0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0, aspect, skew)	{}

  //! ���e�s����Z�b�g���ď���������D
  /*!
    \param P	3x4���e�s��
  */
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
//! ���˘c�ȌW�����܂ޑS�����p�����[�^���ςƂȂ铧�����e�J������\���N���X
class __PORT CameraWithDistortion : public CameraBase
{
  public:
  //! ���˘c�ȌW�����܂ޑS�����p�����[�^��\���N���X
    class __PORT Intrinsic : public Camera::Intrinsic
    {
      public:
      //! �����p�����[�^���Z�b�g���ď���������D
      /*!
	\param k	�œ_����
	\param u0	�摜��_�̉����W
	\param v0	�摜��_�̏c���W
	\param aspect	�A�X�y�N�g��
	\param skew	�񒼌𐫘c��
	\param d1	���˘c�Ȃ̑�1�W��
	\param d2	���˘c�Ȃ̑�2�W��
      */
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0,
		  double aspect=1.0, double skew=0.0,
		  double d1=0.0, double d2=0.0)
	    :Camera::Intrinsic(k, u0, v0, aspect, skew),
	     _d1(d1), _d2(d2)						{}

      //! #Camera�^�J�����̓����p�����[�^���Z�b�g���ď���������D
      /*!
	2�̕��˘c�ȌW����0�ɏ����������D
	\param intrinsic	#Camera�̓����p�����[�^
      */
	Intrinsic(const Camera::Intrinsic& intrinsic)
	    :Camera::Intrinsic(intrinsic), _d1(0.0), _d2(0.0)		{}

      //! �����p�����[�^���Z�b�g���ď���������D
      /*!
	2�̕��˘c�ȌW����0�ɏ����������D
	\param K	3x3�����p�����[�^�s��
      */
	Intrinsic(const Matrix33d& K)
	    :Camera::Intrinsic(), _d1(0.0), _d2(0.0)	{setIntrinsic(K);}
	
      // various operations.
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Point2d		xd(const Point2d& x)		const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& x)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;

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
  //! �ʒu�����_�ɁC�p����P�ʍs��ɁC�����p�����[�^���f�t�H���g�l�ɃZ�b�g���ď���������D
    CameraWithDistortion()	:CameraBase(), _intrinsic()		{}

  //! �O���^�����p�����[�^���Z�b�g���ď���������D
  /*!
    \param t	�J������3�����ʒu
    \param Rt	�J�����̎p����\��3x3��]�s��
    \param k	�œ_����
    \param u0	�摜��_�̉����W
    \param v0	�摜��_�̏c���W
    \param a	�A�X�y�N�g��
    \param s	�񒼌�c��
    \param d1	���˘c�Ȃ̑�1�W��
    \param d2	���˘c�Ȃ̑�2�W��
  */
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
 
//! ���e�s��ƕ��˘c�ȌW�����Z�b�g���ď���������D
/*!
  \param P	3x4���e�s��
  \param d1	���˘c�Ȃ̑�1�W��
  \param d2	���˘c�Ȃ̑�2�W��
*/
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
