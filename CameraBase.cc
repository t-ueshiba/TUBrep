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
 *  $Id: CameraBase.cc,v 1.16 2009-07-31 07:04:44 ueshiba Exp $
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

//! 3�������[�N���b�h��Ԃ���canonical�摜���ʂւ̓��e�s������߂�D
/*!
  \return	canonical�摜���ʂւ̓��e�s��C���Ȃ킿
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

//! �J�����p�����[�^�Ɋւ��铊�e�_��canonical�摜���W��1�K���������߂�D
/*!
  \param X	�Ώۓ_��3�����ʒu
  \return	���e�_��canonical�摜���W��1�K������\��2x6���R�r�s��C���Ȃ킿
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

//! �_��3�����ʒu�Ɋւ��铊�e�_��canonical�摜���W��1�K���������߂�D
/*!
  \param x	�Ώۓ_��3�����ʒu
  \return	���e�_��canonical�摜���W��1�K������\��2x3���R�r�s��C���Ȃ킿
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
 
//! �S�J�����p�����[�^�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  \param x	�Ώۓ_��3�����ʒu
  \return	���e�_�̉摜���W��1�K������\��2x(6+#dofIntrinsic())���R�r�s��C
		���Ȃ킿
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
	
//! ���̓X�g���[������J�����̊O���^�����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
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

//! �o�̓X�g���[���ɃJ�����̊O���^�����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
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

//! canonical�摜���W�n�ɂ����ĕ\�����ꂽ���e�_�̉摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	x�̉摜���W�n�ɂ�����2�����ʒu�C���Ȃ킿
		\f$\TUvec{u}{} = {\cal K}(\TUvec{x}{})\f$
*/
Point2d
CameraBase::Intrinsic::operator ()(const Point2d& x) const
{
    return x;
}

//! canonical�摜���W�n�ɂ����ĕ\�����ꂽ���e�_�̉摜���W�n�ɕ��˘c�Ȃ�t������D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���˘c�ȕt����̓��e�_��2�����ʒu
*/
Point2d
CameraBase::Intrinsic::xd(const Point2d& x) const
{
    return x;
}

//! �����p�����[�^�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���e�_��canonical�摜���W��1�K������\��2x#dofIntrinsic()
		���R�r�s��C���Ȃ킿
		\f$\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}\f$
*/
Matrix<double>
CameraBase::Intrinsic::jacobianK(const Point2d& x) const
{
    return Matrix<double>(2, 0);
}

//! canonical�摜���W�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���e�_��canonical�摜���W��1�K������\��2x2���R�r�s��C���Ȃ킿
		\f$\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}}\f$
*/
Matrix22d
CameraBase::Intrinsic::jacobianXC(const Point2d& x) const
{
    return Matrix22d::I(2);
}
    
//! �摜���W�ɂ����铊�e�_��2�����ʒu��canonical�摜���W�n�ɒ����D
/*!
  \param u	�摜���W�n�ɂ����铊�e�_��2�����ʒu
  \return	canonical�摜���W�n�ɂ�����u��2�����ʒu�C���Ȃ킿
		\f$\TUvec{x}{} = {\cal K}^{-1}(\TUvec{u}{})\f$
*/
Point2d
CameraBase::Intrinsic::xcFromU(const Point2d& u) const
{
    return u;
}

//! �����p�����[�^�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��
*/
Matrix33d
CameraBase::Intrinsic::K() const
{
    return Matrix33d::I(3);
}
    
//! �����p�����[�^�s��̓]�u��Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̓]�u
*/
Matrix33d
CameraBase::Intrinsic::Kt() const
{
    return Matrix33d::I(3);
}
    
//! �����p�����[�^�s��̋t�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̋t�s��
*/
Matrix33d
CameraBase::Intrinsic::Kinv() const
{
    return Matrix33d::I(3);
}
    
//! �����p�����[�^�s��̓]�u�̋t�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̓]�u�̋t�s��
*/
Matrix33d
CameraBase::Intrinsic::Ktinv() const
{
    return Matrix33d::I(3);
}

//! �����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�����p�����[�^�̎��R�x
*/
u_int
CameraBase::Intrinsic::dof() const
{
    return 0;
}

//! �œ_������Ԃ��D
/*!
  \return	�œ_����
*/
double
CameraBase::Intrinsic::k() const
{
    return 1.0;
}

//! �摜��_��Ԃ��D
/*!
  \return	�摜��_
*/
Point2d
CameraBase::Intrinsic::principal() const
{
    return Point2d(0.0, 0.0);
}

//! �A�X�y�N�g���Ԃ��D
/*!
  \return	�A�X�y�N�g��
*/
double
CameraBase::Intrinsic::aspect() const
{
    return 1.0;
}

//! �񒼌�c�݂�Ԃ��D
/*!
  \return	�񒼌�c��
*/
double
CameraBase::Intrinsic::skew() const
{
    return 0.0;
}

//! ���˘c�Ȃ̑�1�W����Ԃ��D
/*!
  \return	���˘c�Ȃ̑�1�W��
*/
double
CameraBase::Intrinsic::d1() const
{
    return 0.0;
}

//! ���˘c�Ȃ̑�2�W����Ԃ��D
/*!
  \return	���˘c�Ȃ̑�2�W��
*/
double
CameraBase::Intrinsic::d2() const
{
    return 0.0;
}

//! �œ_������ݒ肷��D
/*!
  \param k	�œ_����
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setFocalLength(double k)
{
    return *this;				// Do nothing.
}

//! �摜��_��ݒ肷��D
/*!
  \param u0	�摜��_�̉����W
  \param v0	�摜��_�̏c���W
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setPrincipal(double u0, double v0)
{
    return *this;				// Do nothing.
}

//! �A�X�y�N�g���ݒ肷��D
/*!
  \param aspect	�A�X�y�N�g��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setAspect(double aspect)
{
    return *this;				// Do nothing.
}

//! �񒼌𐫘c�݂�ݒ肷��D
/*!
  \param skew	�񒼌𐫘c��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setSkew(double skew)
{
    return *this;				// Do nothing.
}

//! ���˘c�ȌW���ȊO�̓����p�����[�^��ݒ肷��D
/*!
  \param K	3x3�����p�����[�^�s��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setIntrinsic(const Matrix33d& K)
{
    return *this;				// Do nothing.
}

//! ���˘c�ȌW����ݒ肷��D
/*!
  \param d1	���˘c�Ȃ̑�1�W��
  \param d2	���˘c�Ȃ̑�2�W��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::setDistortion(double d1, double d2)
{
    return *this;				// Do nothing.
}
	
//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\��#dof()�����x�N�g��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraBase::Intrinsic::update(const Vector<double>& dp)
{
    return *this;				// Do nothing.
}

//! ���̓X�g���[������J�����̓����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
std::istream&
CameraBase::Intrinsic::get(std::istream& in)
{
    return in;					// Do nothing.
}

//! �o�̓X�g���[���ɃJ�����̓����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
std::ostream&
CameraBase::Intrinsic::put(std::ostream& out) const
{
    return out;					// Do nothing.
}
 
}
