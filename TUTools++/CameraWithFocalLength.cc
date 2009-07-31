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
 *  $Id: CameraWithFocalLength.cc,v 1.13 2009-07-31 07:04:44 ueshiba Exp $
 */
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class CameraWithFocalLength						*
************************************************************************/
CameraBase&
CameraWithFocalLength::setProjection(const Matrix34d& PP)
{
    Matrix33d			Nt = PP(0, 0, 3, 3);
    Nt[0] *= 0.5;
    Nt[1] *= 0.5;
    SVDecomposition<double>	svd(Nt);
    if (svd[2] < 0.0)
	throw std::invalid_argument("CameraWithFocalLength::setProjection: cannot extract camera rotation due to nevative singular value!");
    const Matrix<double>&	RRt = svd.Vt().trns() * svd.Ut();
    const double		p = RRt[0]*Nt[0] + RRt[1]*Nt[1],
				q = RRt[2]*Nt[2];
    Vector3d			tc;
    tc[0] = -PP[0][3] / p;
    tc[1] = -PP[1][3] / p;
    tc[2] = -PP[2][3] / q;
    setFocalLength(p / q).setRotation(RRt).setTranslation(tc * RRt);

    return *this;
}

const CameraBase::Intrinsic&
CameraWithFocalLength::intrinsic() const
{
    return _intrinsic;
}

CameraBase::Intrinsic&
CameraWithFocalLength::intrinsic()
{
    return _intrinsic;
}

/************************************************************************
*  class CameraWithFocalLength::Intrinsic				*
************************************************************************/
//! canonical�摜���W�n�ɂ����ĕ\�����ꂽ���e�_�̉摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	x�̉摜���W�n�ɂ�����2�����ʒu�C���Ȃ킿
		\f$\TUvec{u}{} = k\TUvec{x}{}\f$
*/
Point2d
CameraWithFocalLength::Intrinsic::operator ()(const Point2d& x) const
{
    return Point2d(_k * x[0], _k * x[1]);
}

//! �����p�����[�^�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���e�_��canonical�摜���W��1�K������\��2x1���R�r�s��C���Ȃ킿
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}} = \TUvec{x}{}
		\f$
*/
Matrix<double>
CameraWithFocalLength::Intrinsic::jacobianK(const Point2d& x) const
{
    Matrix<double>	J(2, 1);
    J[0][0] = x[0];
    J[1][0] = x[1];

    return J;
}

//! canonical�摜���W�Ɋւ��铊�e�_�̉摜���W��1�K���������߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	���e�_��canonical�摜���W��1�K������\��2x2���R�r�s��C���Ȃ킿
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}} =
		k\TUvec{I}{2\times 2}
		\f$
*/
Matrix22d
CameraWithFocalLength::Intrinsic::jacobianXC(const Point2d& x) const
{
    Matrix22d	J;
    return J.diag(_k);
}
    
//! �摜���W�ɂ����铊�e�_��2�����ʒu��canonical�摜���W�n�ɒ����D
/*!
  \param u	�摜���W�n�ɂ����铊�e�_��2�����ʒu
  \return	canonical�摜���W�n�ɂ�����u��2�����ʒu�C���Ȃ킿
		\f$\TUvec{x}{} = k^{-1}\TUvec{u}{}\f$
*/
Point2d
CameraWithFocalLength::Intrinsic::xcFromU(const Point2d& u) const
{
    return Point2d(u[0] / _k, u[1] / _k);
}

//! �����p�����[�^�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��C���Ȃ킿
		\f$
		\TUvec{K}{} =
		\TUbeginarray{ccc} k & & \\ & k & \\ & & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithFocalLength::Intrinsic::K() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = _k;
    mat[2][2] = 1.0;

    return mat;
}
    
//! �����p�����[�^�s��̓]�u��Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̓]�u�C���Ȃ킿
		\f$
		\TUtvec{K}{} =
		\TUbeginarray{ccc} k & & \\ & k & \\ & & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithFocalLength::Intrinsic::Kt() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = _k;
    mat[2][2] = 1.0;

    return mat;
}
    
//! �����p�����[�^�s��̋t�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̋t�s��C���Ȃ킿
		\f$
		\TUinv{K}{} =
		\TUbeginarray{ccc} k^{-1} & & \\ & k^{-1} & \\ & & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithFocalLength::Intrinsic::Kinv() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = 1.0 / _k;
    mat[2][2] = 1.0;

    return mat;
}
    
//! �����p�����[�^�s��̓]�u�̋t�s���Ԃ��D
/*!
  \return	3x3�����p�����[�^�s��̓]�u�̋t�s��C���Ȃ킿
		\f$
		\TUtinv{K}{} =
		\TUbeginarray{ccc} k^{-1} & & \\ & k^{-1} & \\ & & 1 \TUendarray
		\f$
*/
Matrix33d
CameraWithFocalLength::Intrinsic::Ktinv() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = 1.0 / _k;
    mat[2][2] = 1.0;

    return mat;
}

//! �����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�����p�����[�^�̎��R�x�C���Ȃ킿1
*/
u_int
CameraWithFocalLength::Intrinsic::dof() const
{
    return 1;
}

//! �œ_������Ԃ��D
/*!
  \return	�œ_����
*/
double
CameraWithFocalLength::Intrinsic::k() const
{
    return _k;
}

//! �œ_������ݒ肷��D
/*!
  \param k	�œ_����
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraWithFocalLength::Intrinsic::setFocalLength(double k)
{
    _k = k;
    return *this;
}    

//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\��#dof()�����x�N�g��
  \return	���̓����p�����[�^
*/
CameraBase::Intrinsic&
CameraWithFocalLength::Intrinsic::update(const Vector<double>& dp)
{
    _k -= dp[0];
    return *this;
}

//! ���̓X�g���[������J�����̓����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
std::istream&
CameraWithFocalLength::Intrinsic::get(std::istream& in)
{
    CanonicalCamera::Intrinsic::get(in);
    return in >> _k;
}

//! �o�̓X�g���[���ɃJ�����̓����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
std::ostream&
CameraWithFocalLength::Intrinsic::put(std::ostream& out) const
{
    using namespace	std;
    
    CanonicalCamera::Intrinsic::put(out);
    cerr << "Focal length:    ";
    return out << _k << endl;
}
 
}
