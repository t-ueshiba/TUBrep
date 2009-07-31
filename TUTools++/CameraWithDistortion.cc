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
 *  $Id: CameraWithDistortion.cc,v 1.15 2009-07-31 07:04:44 ueshiba Exp $
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
//! canonical���W�n�ɂ����ĕ\�����ꂽ���e�_�̉摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	x�̉摜���W�n�ɂ�����ʒu�C���Ȃ킿
		\f$
		\TUbeginarray{c} \TUvec{u}{} \\ 1 \TUendarray =
		\TUbeginarray{c} {\cal K}(\TUvec{x}{}) \\ 1 \TUendarray =
		\TUvec{K}{}
		\TUbeginarray{c} \TUbreve{x}{}(\TUvec{x}{}) \\ 1 \TUendarray
		\f$
*/
Point2d
CameraWithDistortion::Intrinsic::operator ()(const Point2d& x) const
{
    return Camera::Intrinsic::operator ()(xd(x));
}

//! canonical���W�n�ɂ����ĕ\�����ꂽ���e�_�ɕ��˘c�Ȃ�t������D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���˘c�ȕt�����canonical���W�n�ɂ�����ʒu�C���Ȃ킿
		\f$
		\TUbreve{x}{} = (1 + d_1 r^2 + d_2 r^4)\TUvec{x}{},~~
		r = \TUnorm{\TUvec{x}{}}
		\f$
*/
Point2d
CameraWithDistortion::Intrinsic::xd(const Point2d& x) const
{
    const double	sqr = x * x, tmp = 1.0 + sqr*(_d1 + sqr*_d2);
    return Point2d(tmp * x[0], tmp * x[1]);
}

//! canonical�摜���W�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���e�_�̉摜���W��1�K������\��2x2���R�r�s��C���Ȃ킿
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}} = 
		\TUdisppartial{\TUvec{u}{}}{\TUbreve{x}{}}
		\TUdisppartial{\TUbreve{x}{}}{\TUvec{x}{}}
		\f$
*/
Matrix22d
CameraWithDistortion::Intrinsic::jacobianXC(const Point2d& x) const
{
    const double	sqr = x * x, tmp = 2.0*(_d1 + 2.0*sqr*_d2);
    Matrix22d		J;
    J[0][0] = J[1][1] = 1.0 + sqr*(_d1 + sqr*_d2);
    J[0][0] += tmp * x[0] * x[0];
    J[1][1] += tmp * x[1] * x[1];
    J[0][1] = J[1][0] = tmp * x[0] * x[1];
    (J[0] *= k00()) += k01() * J[1];
    J[1] *= k();

    return J;
}

//! �����p�����[�^�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  �������C�A�X�y�N�g��a�Əœ_����k�̐�ak, �񒼌�c��s�Əœ_����k�̐�sk�����ꂼ��
  ��4, ��5�Ԗڂ̓����p�����[�^�Ƃ��Ĉ����Ck, u0, v0, ak, sk, d1, d2��7�p�����[�^��
  �ւ���1�K�����Ƃ��ă��R�r�s����v�Z����D
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���e�_��canonical�摜���W��1�K������\��2x7���R�r�s��C���Ȃ킿
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}} =
		\TUbeginarray{ccccccc}
		& 1 & & \breve{x} & \breve{y} &
		r^2(a k x + s k y) & r^4(a k x + s k y) \\
		\breve{y} & & 1 & & & r^2 k y & r^4 k y
		\TUendarray,~~ r = \TUnorm{\TUvec{x}{}}
		\f$
*/
Matrix<double>
CameraWithDistortion::Intrinsic::jacobianK(const Point2d& x) const
{
    const Point2d&	xxd = xd(x);
    Matrix<double>	J(2, 7);
    J[1][0] = J[0][4] = xxd[1];
    J[0][1] = J[1][2] = 1.0;
    J[0][3] = xxd[0];
    const double	sqr = x * x;
    J[0][5] = sqr * (k00() * x[0] + k01() * x[1]);
    J[1][5] = sqr * (		      k() * x[1]);
    J[0][6] = sqr * J[0][5];
    J[1][6] = sqr * J[1][5];

    return J;
}

//! �摜���W�ɂ����铊�e�_��2�����ʒu��canonical�摜���W�n�ɒ����D
/*!
  \param u	�摜���W�n�ɂ����铊�e�_��2�����ʒu
  \return	canonical�摜�J�������W�n�ɂ�����u��2�����ʒu�C���Ȃ킿
		\f$\TUvec{x}{} = {\cal K}^{-1}(\TUvec{u}{})\f$
*/
Point2d
CameraWithDistortion::Intrinsic::xcFromU(const Point2d& u) const
{
    const Point2d&	xd = Camera::Intrinsic::xcFromU(u);
    const double	sqr = xd * xd, tmp = 1.0 - sqr*(_d1 + sqr*_d2);
    return Point2d(tmp * xd[0], tmp * xd[1]);
}

//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\��#dof()�����x�N�g��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraWithDistortion::Intrinsic::update(const Vector<double>& dp)
{
    Camera::Intrinsic::update(dp(0, 5));
    _d1 -= dp[5];
    _d2 -= dp[6];
    return *this;
}

//! �����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�����p�����[�^�̎��R�x�C���Ȃ킿7
*/
u_int
CameraWithDistortion::Intrinsic::dof() const
{
    return 7;
}

//! ���˘c�Ȃ̑�1�W����Ԃ��D
/*!
  \return	���˘c�Ȃ̑�1�W��
*/
double
CameraWithDistortion::Intrinsic::d1() const
{
    return _d1;
}

//! ���˘c�Ȃ̑�2�W����Ԃ��D
/*!
  \return	���˘c�Ȃ̑�2�W��
*/
double
CameraWithDistortion::Intrinsic::d2() const
{
    return _d2;
}

//! ���˘c�ȌW����ݒ肷��D
/*!
  \param d1	���˘c�Ȃ̑�1�W��
  \param d2	���˘c�Ȃ̑�2�W��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraWithDistortion::Intrinsic::setDistortion(double d1, double d2)
{
    _d1 = d1;
    _d2 = d2;
    return *this;
}

//! ���̓X�g���[������J�����̓����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
std::istream&
CameraWithDistortion::Intrinsic::get(std::istream& in)
{
    Camera::Intrinsic::get(in);
    in >> _d1 >> _d2;

    return in;
}

//! �o�̓X�g���[���ɃJ�����̓����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
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
