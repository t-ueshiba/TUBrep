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
 *  $Id: CameraWithEuclideanImagePlane.cc,v 1.14 2009-07-31 07:04:44 ueshiba Exp $
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
//! canonical�摜���W�n�ɂ����ĕ\�����ꂽ���e�_�̉摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	x�̉摜���W�n�ɂ�����2�����ʒu�C���Ȃ킿
		\f$\TUvec{u}{} = k\TUvec{x}{} + \TUvec{u}{0}\f$
*/
Point2d
CameraWithEuclideanImagePlane::Intrinsic::operator ()(const Point2d& x) const
{
    return Point2d(k() * x[0] + _principal[0], k() * x[1] + _principal[1]);
}

//! �����p�����[�^�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���e�_��canonical�摜���W��1�K������\��2x3���R�r�s��C���Ȃ킿
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}} =
		\TUbeginarray{ccc} x & 1 &  \\ y & & 1 \TUendarray
		\f$
*/
Matrix<double>
CameraWithEuclideanImagePlane::Intrinsic::jacobianK(const Point2d& x) const
{
    Matrix<double>	J(2, 3);
    J[0][0] = x[0];
    J[1][0] = x[1];
    J[0][1] = J[1][2] = 1.0;

    return J;
}

//! �摜���W�ɂ����铊�e�_��2�����ʒu��canonical�摜���W�n�ɒ����D
/*!
  \param u	�摜���W�n�ɂ����铊�e�_��2�����ʒu
  \return	canonical�摜���W�n�ɂ�����u��2�����ʒu�C���Ȃ킿
		\f$\TUvec{x}{} = k^{-1}(\TUvec{u}{} - \TUvec{u}{0})\f$
*/
Point2d
CameraWithEuclideanImagePlane::Intrinsic::xcFromU(const Point2d& u) const
{
    return Point2d((u[0] - _principal[0]) / k(), (u[1] - _principal[1]) / k());
}

//! �����p�����[�^�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��C���Ȃ킿
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

//! �����p�����[�^�s��̓]�u��Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̓]�u�C���Ȃ킿
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

//! �����p�����[�^�s��̋t�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̋t�s��C���Ȃ킿
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

//! �����p�����[�^�s��̓]�u�̋t�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̓]�u�̋t�s��C���Ȃ킿
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

//! �����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�����p�����[�^�̎��R�x�C���Ȃ킿3
*/
u_int
CameraWithEuclideanImagePlane::Intrinsic::dof() const
{
    return 3;
}

//! �摜��_��Ԃ��D
/*!
  \return	�摜��_
*/
Point2d
CameraWithEuclideanImagePlane::Intrinsic::principal() const
{
    return _principal;
}

//! �摜��_��ݒ肷��D
/*!
  \param u0	�摜��_�̉����W
  \param v0	�摜��_�̏c���W
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::Intrinsic::setPrincipal(double u0, double v0)
{
    _principal[0] = u0;
    _principal[1] = v0;
    return *this;
}

//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\��#dof()�����x�N�g��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::Intrinsic::update(const Vector<double>& dp)
{
    CameraWithFocalLength::Intrinsic::update(dp(0, 1));
    _principal[0] -= dp[1];
    _principal[1] -= dp[2];
    return *this;
}

//! ���̓X�g���[������J�����̓����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
std::istream&
CameraWithEuclideanImagePlane::Intrinsic::get(std::istream& in)
{
    CameraWithFocalLength::Intrinsic::get(in);
    return in >> _principal;
}

//! �o�̓X�g���[���ɃJ�����̓����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
std::ostream&
CameraWithEuclideanImagePlane::Intrinsic::put(std::ostream& out) const
{
    CameraWithFocalLength::Intrinsic::put(out);
    std::cerr << "Principal point:";
    return out << _principal;
}
 
}
