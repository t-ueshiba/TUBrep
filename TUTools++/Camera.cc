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
 *  $Id: Camera.cc,v 1.13 2009-07-31 07:04:44 ueshiba Exp $
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
//! canonical���W�n�ɂ����ĕ\�����ꂽ���e�_�̉摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	x�̉摜���W�n�ɂ�����ʒu�C���Ȃ킿
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

//! �����p�����[�^�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  �������C�A�X�y�N�g��a�Əœ_����k�̐�ak, �񒼌�c��s�Əœ_����k�̐�sk�����ꂼ��
  ��4, ��5�Ԗڂ̓����p�����[�^�Ƃ��Ĉ����Ck, u0, v0, ak, sk��5�p�����[�^�Ɋւ���
  1�K�����Ƃ��ă��R�r�s����v�Z����D
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���e�_��canonical�摜���W��1�K������\��2x5���R�r�s��C���Ȃ킿
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

//! canonical�摜���W�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���e�_�̉摜���W��1�K������\��2x2���R�r�s��C���Ȃ킿
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

//! �摜���W�ɂ����铊�e�_��2�����ʒu��canonical�摜���W�n�ɒ����D
/*!
  \param u	�摜���W�n�ɂ����铊�e�_��2�����ʒu
  \return	canonical�摜���W�n�ɂ�����u��2�����ʒu�C���Ȃ킿
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

//! �����p�����[�^�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��C���Ȃ킿
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

//! �����p�����[�^�s��̓]�u��Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̓]�u�C���Ȃ킿
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

//! �����p�����[�^�s��̋t�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̋t�s��C���Ȃ킿
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

//! �����p�����[�^�s��̓]�u�̋t�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̓]�u�̋t�s��C���Ȃ킿
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

//! �����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�����p�����[�^�̎��R�x�C���Ȃ킿5
*/
u_int
Camera::Intrinsic::dof() const
{
    return 5;
}

//! �A�X�y�N�g���Ԃ��D
/*!
  \return	�A�X�y�N�g��
*/
double
Camera::Intrinsic::aspect() const
{
    return _k00 / k();
}

//! �񒼌�c�݂�Ԃ��D
/*!
  \return	�񒼌�c��
*/
double
Camera::Intrinsic::skew() const
{
    return _k01 / k();
}

//! �œ_������ݒ肷��D
/*!
  \param kk	�œ_����
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
Camera::Intrinsic::setFocalLength(double kk)
{
    _k00 *= (kk / k());
    _k01 *= (kk / k());
    return CameraWithFocalLength::Intrinsic::setFocalLength(kk);
}

//! �^����ꂽ�����p�����[�^�s�񂩂�����p�����[�^��ݒ肷��D
/*!
  \param K	3x3�����p�����[�^�s��
  \return	���̓����p�����[�^
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

//! �A�X�y�N�g���ݒ肷��D
/*!
  \param aspect	�A�X�y�N�g��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
Camera::Intrinsic::setAspect(double aspect)
{
    _k00 = aspect * k();
    return *this;
}

//! �񒼌𐫘c�݂�ݒ肷��D
/*!
  \param skew	�񒼌𐫘c��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
Camera::Intrinsic::setSkew(double skew)
{
    _k01 = skew * k();
    return *this;
}

//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\��#dof()�����x�N�g��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
Camera::Intrinsic::update(const Vector<double>& dp)
{
    CameraWithEuclideanImagePlane::Intrinsic::update(dp(0, 3));
    _k00 -= dp[3];
    _k01 -= dp[4];
    return *this;
}

//! ���̓X�g���[������J�����̓����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
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

//! �o�̓X�g���[���ɃJ�����̓����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
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
