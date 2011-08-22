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
 *  $Id: Camera++.h,v 1.3 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		Camera++.h
  \brief	�J���������p�����[�^����уJ�������g�Ɋւ���N���X�̒�`�Ǝ���
*/
#ifndef __TUCameraPP_h
#define __TUCameraPP_h

#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  class IntrinsicBase<T>						*
************************************************************************/
//! �S�������Œ肳�ꂽ�J���������p�����[�^��\���N���X
template <class T>
class IntrinsicBase
{
  public:
  //! ���l�v�Z�S�ʂ̃x�[�X�ƂȂ�^
    typedef T						value_type;
  //! 2�����_�̌^
    typedef Point2<value_type>				point2_type;
  //! �x�N�g���̌^
    typedef Vector<value_type>				vector_type;
  //! �s��̌^
    typedef Matrix<value_type>				matrix_type;
  //! 3x3�s��̌^
    typedef Matrix<value_type, FixedSizedBuf<value_type, 9>,
		   FixedSizedBuf<vector_type, 3> >	matrix33_type;

  public:
    IntrinsicBase(value_type k=1,
		  const point2_type& u0=point2_type(0, 0),
		  value_type a=1, value_type s=0)			;

    point2_type		u(const point2_type& x)			const	;
    point2_type		xFromU(const point2_type& u)		const	;

    value_type		k()					const	;
    const point2_type&	u0()					const	;
    value_type		aspect()				const	;
    value_type		skew()					const	;
    value_type		d1()					const	;
    value_type		d2()					const	;
    matrix33_type	K()					const	;
    matrix33_type	Kt()					const	;
    matrix33_type	Kinv()					const	;
    matrix33_type	Ktinv()					const	;
    void		setFocalLength(value_type k)			;
    void		setPrincipal(const point2_type& u0)		;
    void		setAspect(value_type a)				;
    void		setSkew(value_type s)				;
    void		setK(const matrix33_type& K)			;
    
    static u_int	dofIntrinsic()					;
    void		updateIntrinsic(const vector_type& dp)		;

    std::istream&	get(std::istream& in)				;
    std::ostream&	put(std::ostream& out)			const	;

  protected:
    value_type		k00()					const	;
    value_type		k01()					const	;
    point2_type		u(const point2_type& x,
			  matrix_type* J,matrix_type* H)	const	;

  protected:
    value_type		_k;		//!< �œ_����
    point2_type		_u0;		//!< �摜��_
    value_type		_k00;		//!< �œ_�����ƃA�X�y�N�g��̐�
    value_type		_k01;		//!< �œ_�����Ɣ񒼌𐫘c�݂̐�
};
    
//! �����p�����[�^���Z�b�g���ď���������D
/*!
  \param k	�œ_����
  \param u0	�摜��_
  \param a	�A�X�y�N�g��
  \param s	�񒼌𐫘c��
*/
template <class T> inline
IntrinsicBase<T>::IntrinsicBase(value_type k, const point2_type& u0,
				value_type a, value_type s)
    :_k(k), _u0(u0), _k00(a*_k), _k01(s*_k)
{
}

//! canonical�摜���W�n�ɂ����ĕ\�����ꂽ���e�_�̉摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	x�̉摜���W�n�ɂ�����2�����ʒu�C���Ȃ킿
		\f$
		\TUvec{u}{} =
		\TUbeginarray{cc} ak & sk \\ & k \TUendarray \TUvec{x}{}
		+ \TUvec{u}{0}
		\f$
*/
template <class T> inline typename IntrinsicBase<T>::point2_type
IntrinsicBase<T>::u(const point2_type& x) const
{
    return point2_type(_k00*x[0] + _k01*x[1] + _u0[0], _k*x[1] + _u0[1]);
}
    
//! �摜���W�ɂ����铊�e�_��2�����ʒu��canonical�摜���W�n�ɒ����D
/*!
  \param u	�摜���W�n�ɂ����铊�e�_��2�����ʒu
  \return	canonical�摜���W�n�ɂ�����u��2�����ʒu�C���Ȃ킿
		\f$
		\TUvec{x}{} =
		\TUbeginarray{cc}
		a^{-1}k^{-1} & -a^{-1}sk^{-1} \\ & k^{-1}
		\TUendarray
		(\TUvec{u}{} - \TUvec{u}{0})
		\f$
*/
template <class T> inline typename IntrinsicBase<T>::point2_type
IntrinsicBase<T>::xFromU(const point2_type& u) const
{
    return point2_type((u[0] - _u0[0] - (u[1] - _u0[1]) * _k01 / _k) / _k00,
		       (u[1] - _u0[1]) / _k);
}
    
//! �œ_������Ԃ��D
/*!
  \return	�œ_����
*/
template <class T> inline typename IntrinsicBase<T>::value_type
IntrinsicBase<T>::k() const
{
    return _k;
}
    
//! �摜��_��Ԃ��D
/*!
  \return	�摜��_
*/
template <class T>
inline const typename IntrinsicBase<T>::point2_type&
IntrinsicBase<T>::u0() const
{
    return _u0;
}

//! �A�X�y�N�g���Ԃ��D
/*!
  \return	�A�X�y�N�g��
*/
template <class T> inline typename IntrinsicBase<T>::value_type
IntrinsicBase<T>::aspect() const
{
    return _k00 / _k;
}
    
//! �񒼌𐫘c�݂�Ԃ��D
/*!
  \return	�񒼌𐫘c��
*/
template <class T> inline typename IntrinsicBase<T>::value_type
IntrinsicBase<T>::skew() const
{
    return _k01 / _k;
}
    
//! ���˘c�Ȃ̑�1�W����Ԃ��D
/*!
  \return	���˘c�Ȃ̑�1�W��
*/
template <class T> inline typename IntrinsicBase<T>::value_type
IntrinsicBase<T>::d1() const
{
    return 0;
}
    
//! ���˘c�Ȃ̑�2�W����Ԃ��D
/*!
  \return	���˘c�Ȃ̑�2�W��
*/
template <class T> inline typename IntrinsicBase<T>::value_type
IntrinsicBase<T>::d2() const
{
    return 0;
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
template <class T> inline typename IntrinsicBase<T>::matrix33_type
IntrinsicBase<T>::K() const
{
    matrix33_type	mat;
    mat[0][0] = _k00;
    mat[0][1] = _k01;
    mat[0][2] = _u0[0];
    mat[1][1] = _k;
    mat[1][2] = _u0[1];
    mat[2][2] = value_type(1);

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
template <class T> inline typename IntrinsicBase<T>::matrix33_type
IntrinsicBase<T>::Kt() const
{
    matrix33_type	mat;
    mat[0][0] = _k00;
    mat[1][0] = _k01;
    mat[2][0] = _u0[0];
    mat[1][1] = _k;
    mat[2][1] = _u0[1];
    mat[2][2] = value_type(1);

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
template <class T> inline typename IntrinsicBase<T>::matrix33_type
IntrinsicBase<T>::Kinv() const
{
    matrix33_type	mat;
    mat[0][0] = value_type(1) / _k00;
    mat[0][1] = -_k01 / (_k00 * _k);
    mat[0][2] = -_u0[0] * mat[0][0] - _u0[1] * mat[0][1];
    mat[1][1] = value_type(1) / _k;
    mat[1][2] = -_u0[1] / _k;
    mat[2][2] = value_type(1);

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
template <class T> inline typename IntrinsicBase<T>::matrix33_type
IntrinsicBase<T>::Ktinv() const
{
    matrix33_type	mat;
    mat[0][0] = value_type(1) / _k00;
    mat[1][0] = -_k01 / (_k00 * _k);
    mat[2][0] = -_u0[0] * mat[0][0] - _u0[1] * mat[0][1];
    mat[1][1] = value_type(1) / _k;
    mat[2][1] = -_u0[1] / k();
    mat[2][2] = value_type(1);

    return mat;
}

//! �œ_������ݒ肷��D
/*!
  \param k	�œ_����
*/
template <class T> inline void
IntrinsicBase<T>::setFocalLength(value_type k)
{
    _k00 *= (k / _k);
    _k01 *= (k / _k);
    _k = k;
}

//! �摜��_��ݒ肷��D
/*!
  \param u0	�摜��_
*/
template <class T> inline void
IntrinsicBase<T>::setPrincipal(const point2_type& u0)
{
    _u0 = u0;
}

//! �A�X�y�N�g���ݒ肷��D
/*!
  \param a	�A�X�y�N�g��
*/
template <class T> inline void
IntrinsicBase<T>::setAspect(value_type a)
{
    _k00 = a * _k;
}
    
//! �񒼌𐫘c�݂�ݒ肷��D
/*!
  \param s	�񒼌𐫘c��
*/
template <class T> inline void
IntrinsicBase<T>::setSkew(value_type s)
{
    _k01 = s * _k;
}
    
//! ���˘c�ȌW���ȊO�̓����p�����[�^��ݒ肷��D
/*!
  \param K	3x3�����p�����[�^�s��
*/
template <class T> inline void
IntrinsicBase<T>::setK(const matrix33_type& K)
{
    setFocalLength(K[1][1]/K[2][2]);
    setPrincipal(point2_type(K[0][2]/K[2][2], K[1][2]/K[2][2]));
    setAspect(K[0][0] / K[1][1]);
    setSkew(K[0][1] / K[1][1]);
}

//! �ϓ����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�ϓ����p�����[�^�̎��R�x, ���Ȃ킿0
*/
template <class T> inline u_int
IntrinsicBase<T>::dofIntrinsic()
{
    return 0;
}
    
//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\�� dofIntrinsic() �����x�N�g��
*/
template <class T> inline void
IntrinsicBase<T>::updateIntrinsic(const vector_type& dp)
{
}
    
//! ���̓X�g���[������J�����̓����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T> inline std::istream&
IntrinsicBase<T>::get(std::istream& in)
{
    in >> _k >> _u0 >> _k00 >> _k01;
    _k00 *= _k;
    _k01 *= _k;
    return in;
}

//! �o�̓X�g���[���ɃJ�����̓����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T> inline std::ostream&
IntrinsicBase<T>::put(std::ostream& out) const
{
    using namespace	std;
    
    cerr << "Focal length:    ";
    out << _k << endl;
    cerr << "Principal point:";
    out << _u0;
    cerr << "Aspect ratio:    ";
    out << aspect() << endl;
    cerr << "Skew:            ";
    return out << skew() << endl;
}

//! �œ_�����ƃA�X�y�N�g��̐ς�Ԃ��D
/*!
  \return	�œ_�����ƃA�X�y�N�g��̐�
*/
template <class T> inline typename IntrinsicBase<T>::value_type
IntrinsicBase<T>::k00() const
{
    return _k00;
}
    
//! �œ_�����Ɣ񒼌𐫘c�݂̐ς�Ԃ��D
/*!
  \return	�œ_�����Ɣ񒼌𐫘c�݂̐�
*/
template <class T> inline typename IntrinsicBase<T>::value_type
IntrinsicBase<T>::k01() const
{
    return _k01;
}
    
template <class T> typename IntrinsicBase<T>::point2_type
IntrinsicBase<T>::u(const point2_type& x, matrix_type* J, matrix_type* H) const
{
    if (J)
    {
      // [�O���p�����[�^�Ɋւ���1�K����]
      //   (���˘c�Ȃ�t�����ꂽ)canonical�摜���W�n����摜���W�n�ɕϊ�
	((*J)[0](0, 6) *= _k00) += _k01 * (*J)[1](0, 6);
	(*J)[1](0, 6) *= _k;

	if (H)
	{
	  // [�O���p�����[�^�Ɋւ���2�K����]
	  //   (���˘c�Ȃ�t�����ꂽ)canonical�摜���W�n����摜���W�n�ɕϊ�
	    (H[0](0, 0, 6, 6) *= _k00) += _k01 * H[1](0, 0, 6, 6);
	    H[1](0, 0, 6, 6) *= _k;

	    H[0].symmetrize();
	    H[1].symmetrize();
	}
    }

    return u(x);
}
    
/************************************************************************
*  class IntrinsicWithFocalLength<T>					*
************************************************************************/
//! �œ_�����݂̂��ςȃJ���������p�����[�^��\���N���X
template <class T>
class IntrinsicWithFocalLength : public IntrinsicBase<T>
{
  private:
    typedef IntrinsicBase<T>			super;

  public:
    typedef typename super::value_type		value_type;
    typedef typename super::point2_type		point2_type;
    typedef typename super::vector_type		vector_type;
    typedef typename super::matrix_type		matrix_type;
    typedef typename super::matrix33_type	matrix33_type;
    
  public:
    IntrinsicWithFocalLength(value_type k=1,
			     const point2_type& u0=point2_type(0, 0),
			     value_type a=1, value_type s=0)		;
    
    using		super::u;
    static u_int	dofIntrinsic()					;
    void		updateIntrinsic(const vector_type& dp)		;

  protected:
    point2_type		u(const point2_type& x,
			  matrix_type* J, matrix_type* H)	const	;
};

//! �����p�����[�^���Z�b�g���ď���������D
/*!
  \param k	�œ_����
  \param u0	�摜��_
  \param a	�A�X�y�N�g��
  \param s	�񒼌𐫘c��
*/
template <class T> inline
IntrinsicWithFocalLength<T>::IntrinsicWithFocalLength(
    value_type k, const point2_type& u0, value_type a, value_type s)
    :super(k, u0, a, s)
{
}

//! �ϓ����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�ϓ����p�����[�^�̎��R�x, ���Ȃ킿1
*/
template <class T> inline u_int
IntrinsicWithFocalLength<T>::dofIntrinsic()
{
    return 1;
}
    
//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\�� dofIntrinsic() �����x�N�g��
*/
template <class T> inline void
IntrinsicWithFocalLength<T>::updateIntrinsic(const vector_type& dp)
{
    const value_type	a = super::aspect(), s = super::skew();
    super::_k -= dp[0];
    setAspect(a);
    setSkew(s);
}
    
template <class T> typename IntrinsicWithFocalLength<T>::point2_type
IntrinsicWithFocalLength<T>::u(const point2_type& x,
			       matrix_type* J, matrix_type* H) const
{
    if (J)
    {
      // �œ_�����Ɋւ���1�K����
	(*J)[0][6] = super::aspect() * x[0] + super::skew() * x[1];
	(*J)[1][6] =					      x[1];

	if (H)
	{
	  // �œ_�����ƊO���p�����[�^�Ɋւ���2�K����
	    H[0][6](0, 6) = super::aspect() * (*J)[0](0, 6)
			  + super::skew()   * (*J)[1](0, 6);
	    H[1][6](0, 6) =		      (*J)[1](0, 6);
	}
    }
    
    return super::u(x, J, H);
}
    
/************************************************************************
*  class IntrinsicWithEuclideanImagePlane<T>				*
************************************************************************/
//! �œ_�����Ɖ摜��_�݂̂��ςȃJ���������p�����[�^��\���N���X
template <class T>
class IntrinsicWithEuclideanImagePlane : public IntrinsicWithFocalLength<T>
{
  private:
    typedef IntrinsicWithFocalLength<T>		super;

  public:
    typedef typename super::value_type		value_type;
    typedef typename super::point2_type		point2_type;
    typedef typename super::vector_type		vector_type;
    typedef typename super::matrix_type		matrix_type;
    typedef typename super::matrix33_type	matrix33_type;
    
  public:
    IntrinsicWithEuclideanImagePlane(value_type k=1,
				     const point2_type& u0=point2_type(0, 0),
				     value_type a=1, value_type s=0)	;

    using		super::u;
    static u_int	dofIntrinsic()					;
    void		updateIntrinsic(const vector_type& dp)		;

  protected:
    point2_type		u(const point2_type& x,
			  matrix_type* J, matrix_type* H)	const	;
};

//! �����p�����[�^���Z�b�g���ď���������D
/*!
  \param k	�œ_����
  \param u0	�摜��_
  \param a	�A�X�y�N�g��
  \param s	�񒼌𐫘c��
*/
template <class T> inline
IntrinsicWithEuclideanImagePlane<T>::IntrinsicWithEuclideanImagePlane(
    value_type k, const point2_type& u0, value_type a, value_type s)
    :super(k, u0, a, s)
{
}

//! �ϓ����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�ϓ����p�����[�^�̎��R�x, ���Ȃ킿3
*/
template <class T> inline u_int
IntrinsicWithEuclideanImagePlane<T>::dofIntrinsic()
{
    return super::dofIntrinsic() + 2;
}
    
//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\�� dofIntrinsic() �����x�N�g��
*/
template <class T> inline void
IntrinsicWithEuclideanImagePlane<T>::updateIntrinsic(const vector_type& dp)
{
    const u_int	j = super::dofIntrinsic();
    super::updateIntrinsic(dp(0, j));
    super::_u0[0] -= dp[j];
    super::_u0[1] -= dp[j + 1];
}
    
template <class T> typename IntrinsicWithEuclideanImagePlane<T>::point2_type
IntrinsicWithEuclideanImagePlane<T>::u(const point2_type& x,
				       matrix_type* J, matrix_type* H) const
{
    if (J)
    {
      // [�摜��_�Ɋւ���1�K����]
	const u_int	j = 6 + super::dofIntrinsic();
	(*J)[0][j] = (*J)[1][j+1] = 1;
    }
    
    return super::u(x, J, H);
}
    
/************************************************************************
*  class Intrinsic<T>							*
************************************************************************/
//! ���˘c�ȌW���ȊO�̑S�p�����[�^���ςȃJ���������p�����[�^��\���N���X
template <class T>
class Intrinsic : public IntrinsicBase<T>
{
  private:
    typedef IntrinsicBase<T>			super;

  public:
    typedef typename super::value_type		value_type;
    typedef typename super::point2_type		point2_type;
    typedef typename super::vector_type		vector_type;
    typedef typename super::matrix_type		matrix_type;
    typedef typename super::matrix33_type	matrix33_type;
    
  public:
    Intrinsic(value_type k=1, const point2_type& u0=point2_type(0, 0),
	      value_type a=1, value_type s=0)				;

    using		super::u;
    static u_int	dofIntrinsic()					;
    void		updateIntrinsic(const vector_type& dp)		;

  protected:
    point2_type		u(const point2_type& x,
			  matrix_type* J, matrix_type* H)	const	;
};

//! �����p�����[�^���Z�b�g���ď���������D
/*!
  \param k	�œ_����
  \param u0	�摜��_
  \param a	�A�X�y�N�g��
  \param s	�񒼌𐫘c��
*/
template <class T> inline
Intrinsic<T>::Intrinsic(value_type k, const point2_type& u0,
			value_type a, value_type s)
    :super(k, u0, a, s)
{
}

//! �ϓ����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�ϓ����p�����[�^�̎��R�x, ���Ȃ킿5
*/
template <class T> inline u_int
Intrinsic<T>::dofIntrinsic()
{
    return 5;
}
    
//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\�� dofIntrinsic() �����x�N�g��
*/
template <class T> inline void
Intrinsic<T>::updateIntrinsic(const vector_type& dp)
{
    super::_k     -= dp[0];
    super::_u0[0] -= dp[1];
    super::_u0[1] -= dp[2];
    super::_k00   -= dp[3];
    super::_k01   -= dp[4];
}
    
template <class T> typename Intrinsic<T>::point2_type
Intrinsic<T>::u(const point2_type& x, matrix_type* J, matrix_type* H) const
{
    if (J)
    {
      // [�����p�����[�^�Ɋւ���1�K����]
	(*J)[0][7]  = (*J)[1][8] = 1;
	(*J)[0][9]		 = x[0];
	(*J)[0][10] = (*J)[1][6] = x[1];

	if (H)
	{
	  // [�����p�����[�^�ƊO���p�����[�^�Ɋւ���2�K����]
	    H[0](9, 0, 2, 6) = (*J)(0, 0, 2, 6);
	    H[1][6](0, 6)    = (*J)[1](0, 6);
	}
    }
    
    return super::u(x, J, H);
}
    
/************************************************************************
*  class IntrinsicWithDistortion<I>					*
************************************************************************/
//! ���˘c�ȌW�����܂ޑS�p�����[�^���ςȃJ���������p�����[�^��\���N���X
template <class I>
class IntrinsicWithDistortion : public I
{
  private:
    typedef I					super;

  public:
    typedef typename super::value_type		value_type;
    typedef typename super::point2_type		point2_type;
    typedef typename super::vector_type		vector_type;
    typedef typename super::matrix_type		matrix_type;
    typedef typename super::matrix33_type	matrix33_type;
    
  public:
    IntrinsicWithDistortion(value_type k=1,
			    const point2_type& u0=point2_type(0, 0),
			    value_type a=1,  value_type s=0,
			    value_type d1=0, value_type d2=0)		;

    point2_type		u(const point2_type& x)			const	;
    point2_type		xd(const point2_type& x)		const	;
    point2_type		xFromU(const point2_type& u)		const	;
    value_type		d1()					const	;
    value_type		d2()					const	;
    void		setDistortion(value_type d1, value_type d2)	;
    
    static u_int	dofIntrinsic()					;
    void		updateIntrinsic(const vector_type& dp)		;

    std::istream&	get(std::istream& in)				;
    std::ostream&	put(std::ostream& out)			const	;
    
  protected:
    point2_type		u(const point2_type& x,
			  matrix_type* J, matrix_type* H)	const	;

  private:
    vector_type		DDu(const point2_type& x)		const	;
    
  private:
    value_type		_d1;
    value_type		_d2;
};

//! �����p�����[�^���Z�b�g���ď���������D
/*!
  \param k	�œ_����
  \param u0	�摜��_
  \param a	�A�X�y�N�g��
  \param s	�񒼌𐫘c��
  \param d1	���˘c�Ȃ̑�1�W��
  \param d2	���˘c�Ȃ̑�2�W��
*/
template <class I> inline
IntrinsicWithDistortion<I>::IntrinsicWithDistortion(
    value_type k, const point2_type& u0, value_type a, value_type s,
    value_type d1, value_type d2)
    :super(k, u0, a, s), _d1(d1), _d2(d2)
{
}
    
//! canonical�摜���W�n�ɂ����ĕ\�����ꂽ���e�_�̉摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param x	canonical�摜���W�ɂ����铊�e�_��2�����ʒu
  \return	x�̉摜���W�n�ɂ�����2�����ʒu�C���Ȃ킿
		\f$
		\TUvec{u}{} =
		\TUbeginarray{cc} ak & sk \\ & k \TUendarray \TUbreve{x}{}
		+ \TUvec{u}{0},~~
		\TUbreve{x}{} = (1 + d_1 r^2 + d_2 r^4)\TUvec{x}{},~~
		r = \TUnorm{\TUvec{x}{}}
		\f$
*/
template <class I> inline typename IntrinsicWithDistortion<I>::point2_type
IntrinsicWithDistortion<I>::u(const point2_type& x) const
{
    return super::u(xd(x));
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
template <class I> inline typename IntrinsicWithDistortion<I>::point2_type
IntrinsicWithDistortion<I>::xd(const point2_type& x) const
{
    const value_type	sqr = x * x,
			tmp = value_type(1) + sqr*(_d1 + sqr*_d2);
    return point2_type(tmp * x[0], tmp * x[1]);
}
    
//! �摜���W�ɂ����铊�e�_��2�����ʒu��canonical�摜���W�n�ɒ����D
/*!
  \param u	�摜���W�n�ɂ����铊�e�_��2�����ʒu
  \return	canonical�摜�J�������W�n�ɂ�����u��2�����ʒu�C���Ȃ킿
		\f$
		\TUvec{x}{} =
		(1 - d_1 \breve{r}^2 - d_2 \breve{r}^4)\TUbreve{x}{},~~
		\breve{r} = \TUnorm{\TUbreve{x}{}}
		\f$
*/
template <class I> inline typename IntrinsicWithDistortion<I>::point2_type
IntrinsicWithDistortion<I>::xFromU(const point2_type& u) const
{
    const point2_type&	xd  = super::xFromU(u);
    const value_type	sqr = xd * xd,
			tmp = value_type(1) - sqr*(_d1 + sqr*_d2);
    return point2_type(tmp * xd[0], tmp * xd[1]);
}
    
//! ���˘c�Ȃ̑�1�W����Ԃ��D
/*!
  \return	���˘c�Ȃ̑�1�W��
*/
template <class I> inline typename IntrinsicWithDistortion<I>::value_type
IntrinsicWithDistortion<I>::d1() const
{
    return _d1;
}
    
//! ���˘c�Ȃ̑�2�W����Ԃ��D
/*!
  \return	���˘c�Ȃ̑�2�W��
*/
template <class I> inline typename IntrinsicWithDistortion<I>::value_type
IntrinsicWithDistortion<I>::d2() const
{
    return _d2;
}
    
//! ���˘c�ȌW����ݒ肷��D
/*!
  \param d1	���˘c�Ȃ̑�1�W��
  \param d2	���˘c�Ȃ̑�2�W��
*/
template <class I> inline void
IntrinsicWithDistortion<I>::setDistortion(value_type d1, value_type d2)
{
    _d1 = d1;
    _d2 = d2;
}
    
//! �ϓ����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�ϓ����p�����[�^�̎��R�x
*/
template <class I> inline u_int
IntrinsicWithDistortion<I>::dofIntrinsic()
{
    return super::dofIntrinsic() + 2;
}
    
//! �����p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \param dp	�X�V�ʂ�\�� dofIntrinsic() �����x�N�g��
*/
template <class I> inline void
IntrinsicWithDistortion<I>::updateIntrinsic(const vector_type& dp)
{
    const u_int	j = super::dofIntrinsic();
    super::updateIntrinsic(dp(0, j));
    _d1 -= dp[j];
    _d2 -= dp[j + 1];
}

//! ���̓X�g���[������J�����̓����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class I> inline std::istream&
IntrinsicWithDistortion<I>::get(std::istream& in)
{
    super::get(in);
    return in >> _d1 >> _d2;
}

//! �o�̓X�g���[���ɃJ�����̓����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class I> inline std::ostream&
IntrinsicWithDistortion<I>::put(std::ostream& out) const
{
    using namespace	std;
    
    super::put(out);
    cerr << "Distortion-1:    ";
    out << _d1 << endl;
    cerr << "Distortion-2:    ";
    return out << _d2 << endl;
}

template <class I> typename IntrinsicWithDistortion<I>::point2_type
IntrinsicWithDistortion<I>::u(const point2_type& x,
			      matrix_type* J, matrix_type* H) const
{
    if (J)
    {
      // [���˘c�ȃp�����[�^�Ɋւ���1�K����]
	const value_type	sqr = x * x;
	const u_int		i = 6 + super::dofIntrinsic();
	(*J)[0][i]   = sqr * (super::k00() * x[0] + super::k01() * x[1]);
	(*J)[1][i]   = sqr * (			    super::k()   * x[1]);
	(*J)[0][i+1] = sqr * (*J)[0][i];
	(*J)[1][i+1] = sqr * (*J)[1][i];

	matrix_type		Dxd = ((2*_d1 + 4*sqr*_d2) * x) % x;
	const value_type	tmp = value_type(1) + sqr*(_d1 + sqr*_d2);
	Dxd[0][0] += tmp;
	Dxd[1][1] += tmp;

	if (H)
	{
	  // [�O���p�����[�^�Ɋւ���2�K����]
	  //   canonical���W�n������˘c�Ȃ�t�����ꂽcanonical���W�n�ɕϊ�
	    const matrix_type&	tmp = Dxd[1][0] * H[0](0, 0, 6, 6);
	    (H[0](0, 0, 6, 6) *= Dxd[0][0]) += Dxd[0][1] * H[1](0, 0, 6, 6);
	    (H[1](0, 0, 6, 6) *= Dxd[1][1]) += tmp;

	  // [���˘c�ȂƊO���p�����[�^�Ɋւ���2�K����]
	    matrix_type	DDxd(2, 2);
	    DDxd[0][0] = 2*x[0]*x[0] + sqr;
	    DDxd[0][1] = 2*x[0]*x[1];
	    DDxd[1][0] = (4*x[0]*x[0] + sqr)*sqr;
	    DDxd[1][1] = 4*x[0]*x[1]*sqr;
	    matrix_type	DDyd(2, 2);
	    DDyd[0][0] = 2*x[0]*x[1];
	    DDyd[0][1] = 2*x[1]*x[1] + sqr;
	    DDyd[1][0] = 4*x[0]*x[1]*sqr;
	    DDyd[1][1] = (4*x[1]*x[1] + sqr)*sqr;
	    H[0](i, 0, 2, 6) = (super::k00() * DDxd + super::k01() * DDyd)
			     * (*J)(0, 0, 2, 6);
	    H[1](i, 0, 2, 6) = (super::k() * DDyd) * (*J)(0, 0, 2, 6);

	  // [���˘c�ȂƓ����p�����[�^�Ɋւ���2�K����]
	    (H[0][i  ](6, super::dofIntrinsic()) = DDu(x)) *= sqr;
	    (H[0][i+1](6, super::dofIntrinsic()) =
	     H[0][i  ](6, super::dofIntrinsic())	 ) *= sqr;
	    H[1][i  ][6] = sqr * x[1];
	    H[1][i+1][6] = sqr * H[1][i][6];
	}

      // [�O���p�����[�^�Ɋւ���1�K����]
      //   canonical���W�n������˘c�Ȃ�t�����ꂽcanonical���W�n�ɕϊ�
	(*J)(0, 0, 2, 6) = Dxd * (*J)(0, 0, 2, 6);
    }
    
    return super::u(xd(x), J, H);
}

template <class I> inline typename IntrinsicWithDistortion<I>::vector_type
IntrinsicWithDistortion<I>::DDu(const point2_type& x) const
{
    vector_type	val(super::dofIntrinsic());
    val[0] = super::aspect() * x[0] + super::skew() * x[1];

    return val;
}

template <> inline IntrinsicWithDistortion<Intrinsic<float> >::vector_type
IntrinsicWithDistortion<Intrinsic<float> >::DDu(const point2_type& x) const
{
    vector_type	val(super::dofIntrinsic());
    val[3] = x[0];
    val[4] = x[1];

    return val;
}

template <> inline IntrinsicWithDistortion<Intrinsic<double> >::vector_type
IntrinsicWithDistortion<Intrinsic<double> >::DDu(const point2_type& x) const
{
    vector_type	val(super::dofIntrinsic());
    val[3] = x[0];
    val[4] = x[1];

    return val;
}

/************************************************************************
*  class CanonicalCamera<T>						*
************************************************************************/
//! ���ׂĂ̓����p�����[�^���W������l(�œ_�����ƃA�X�y�N�g�䂪1, �񒼌𐫘c�݂ƕ��˘c�ȌW����0, �摜��_�����_�Ɉ�v)�ƂȂ铧�����e�J������\���N���X
template <class T>
class CanonicalCamera
{
  public:
  //! ���l�v�Z�S�ʂ̃x�[�X�ƂȂ�^
    typedef T						value_type;
  //! 2�����_�̌^
    typedef Point2<value_type>				point2_type;
  //! 3�����_�̌^
    typedef Point3<value_type>				point3_type;
  //! �x�N�g���̌^
    typedef Vector<value_type>				vector_type;
  //! �s��̌^
    typedef Matrix<value_type>				matrix_type;
  //! 3x3�s��̌^
    typedef Matrix<value_type, FixedSizedBuf<value_type, 9>,
		   FixedSizedBuf<vector_type, 3> >	matrix33_type;
  //! 3x4�s��̌^
    typedef Matrix<value_type, FixedSizedBuf<value_type, 12>,
		   FixedSizedBuf<vector_type, 3> >	matrix34_type;

  public:
    CanonicalCamera(const point3_type& t=point3_type(0, 0, 0),
		    const matrix33_type& Rt=matrix33_type::I(3))	;

    point2_type		x(const point3_type& X,
			  matrix_type* J=0, matrix_type* H=0)	const	;
    matrix34_type	Pc()					const	;
    const point3_type&	t()					const	;
    const matrix33_type&
			Rt()					const	;
    void		setTranslation(const point3_type& t)		;
    void		setRotation(const matrix33_type& Rt)		;

    static u_int	dof()						;
    void		update(const vector_type& dp)			;
    void		updateFCC(const vector_type& dp)		;

    std::istream&	get(std::istream& in)				;
    std::ostream&	put(std::ostream& out)			const	;

  private:
    point3_type		_t;			//!< �J�����̓��e���S
    matrix33_type	_Rt;			//!< �J�����̉�]
};

//! �ʒu�Ǝp�����Z�b�g���ď���������D
/*!
  \param t	�J�����ʒu��\��3�����x�N�g��
  \param Rt	�J�����p����\��3x3��]�s��
*/
template <class T> inline
CanonicalCamera<T>::CanonicalCamera(const point3_type& t,
				    const matrix33_type& Rt)
    :_t(t), _Rt(Rt)
{
}

//! 3������Ԓ��̓_�̓��e�_��canonical�摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param X	�Ώۓ_��3�����ʒu
  \param J	0�łȂ���΃J�����̈ʒu�E�p���Ɋւ��铊�e�_��1�K�����C
		���Ȃ킿2x6���R�r�s��
		\f$
		\TUbeginarray{cc}
		\TUdisppartial{\TUvec{x}{}}{\TUvec{t}{}} &
		\TUdisppartial{\TUvec{x}{}}{\TUvec{\theta}{}}
		\TUendarray
		\f$
		���Ԃ����
  \param H	0�łȂ���΃J�����̈ʒu�E�p���Ɋւ��铊�e�_��2�K�����C
		���Ȃ킿2��6x6�w�b�Z�s��
		\f$
		\TUbeginarray{cc}
		\frac{\partial^2 x}{\partial\TUvec{t}{}^2} &
		\frac{\partial^2 x}
		     {\partial\TUvec{t}{}\partial\TUvec{\theta}{}} \\
		\frac{\partial^2 x}
		     {\partial\TUvec{\theta}{}\partial\TUvec{t}{}} &
		\frac{\partial^2 x}{\partial\TUvec{\theta}{}^2}
		\TUendarray
		\f$, 
		\f$
		\TUbeginarray{cc}
		\frac{\partial^2 y}{\partial\TUvec{t}{}^2} &
		\frac{\partial^2 y}
		     {\partial\TUvec{t}{}\partial\TUvec{\theta}{}} \\
		\frac{\partial^2 y}
		     {\partial\TUvec{\theta}{}\partial\TUvec{t}{}} &
		\frac{\partial^2 y}{\partial\TUvec{\theta}{}^2}
		\TUendarray
		\f$
		���Ԃ����
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
template <class T> typename CanonicalCamera<T>::point2_type
CanonicalCamera<T>::x(const point3_type& X,
		      matrix_type* J, matrix_type* H) const
{
    typedef Vector<value_type, FixedSizedBuf<value_type, 3> >	vector3_type;

    const vector3_type&	dX = X - _t;
    const vector_type&	Xc = _Rt * dX;
    point2_type		x(Xc[0] / Xc[2], Xc[1] / Xc[2]);

    if (J)	// ���R�r�s����v�Z����Ȃ�...
    {
	if (J->nrow() < 2 || J->ncol() < 6)
	    J->resize(2, 6);				// ���R�r�s���2x6
	
	for (int n = 0; n < 2; ++n)
	{
	    vector_type	Dx = (*J)[n](0, 3);
	    
	    (((Dx = _Rt[2]) *= x[n]) -= _Rt[n]) /= Xc[2];  // �ʒu�Ɋւ������
	    (*J)[n](3, 3) = Dx ^ dX;			   // ��]�Ɋւ������
	
	    if (H)	// �w�b�Z�s����v�Z����Ȃ�...
	    {
		matrix_type&	HH = H[n];		// ��n���̃w�b�Z�s��
		if (HH.nrow() < 6 || HH.ncol() < 6)
		    HH.resize(6, 6);			// �w�b�Z�s���6x6

		(HH(0, 0, 3, 3) = _Rt[2] % Dx + Dx % _Rt[2]) /= Xc[2];
		HH(0, 3, 3, 3) = Dx.skew() + HH(0, 0, 3, 3) ^ dX;
		HH(3, 0, 3, 3) = HH(0, 3, 3, 3).trns();
		HH(3, 3, 3, 3) = -dX ^ HH(0, 3, 3, 3);
		(HH(3, 3, 3, 3) += HH(3, 3, 3, 3).trns()) *= value_type(0.5);
	    }
	}
    }
    
    return x;
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
template <class T> inline typename CanonicalCamera<T>::matrix34_type
CanonicalCamera<T>::Pc() const
{
    matrix34_type	P;
    P(0, 0, 3, 3) = _Rt;
    P[0][3] = -(_Rt[0] * _t);
    P[1][3] = -(_Rt[1] * _t);
    P[2][3] = -(_Rt[2] * _t);

    return P;
}

//! �J�����̈ʒu��Ԃ��D
/*!
  \return	�J������3�����ʒu
*/
template <class T> inline const typename CanonicalCamera<T>::point3_type&
CanonicalCamera<T>::t() const
{
    return _t;
}

//! �J�����̎p����Ԃ��D
/*!
  \return	�J�����̎p����\��3x3��]�s��
*/
template <class T> inline const typename CanonicalCamera<T>::matrix33_type&
CanonicalCamera<T>::Rt() const
{
    return _Rt;
}
    
//! �J�����̈ʒu��ݒ肷��D
/*!
  \param t	�J������3�����ʒu
  \return	���̃J����
*/
template <class T> inline void
CanonicalCamera<T>::setTranslation(const point3_type& t)
{
    _t = t;
}

//! �J�����̎p����ݒ肷��D
/*!
  \param Rt	�J�����̎p����\��3x3��]�s��
  \return	���̃J����
*/
template <class T> inline void
CanonicalCamera<T>::setRotation(const matrix33_type& Rt)
{
    _Rt = Rt;
}

//! �J�����O���p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�O���p�����[�^�̎��R�x, ���Ȃ킿6
*/
template <class T> inline u_int
CanonicalCamera<T>::dof()
{
    return 6;
}
    
//! �J�����̊O���p�����[�^���w�肳�ꂽ�ʂ����X�V����D
/*!
  \f$\Delta\TUvec{p}{} =
  [\Delta\TUtvec{t}{},~\Delta\TUtvec{\theta}{}]^\top\f$�ɑ΂���
  \f{eqnarray*}
  \TUvec{t}{} & \leftarrow & \TUvec{t}{} - \Delta\TUvec{t}{} \\
  \TUtvec{R}{} & \leftarrow &
  \TUtvec{R}{}\TUtvec{R}{}(\Delta\TUvec{\theta}{})
  \f}
  �ƍX�V����D
  \param dp	�X�V�ʂ�\��6�����x�N�g��
*/
template <class T> inline void
CanonicalCamera<T>::update(const vector_type& dp)
{
    _t -= dp(0, 3);
    updateFCC(dp(3, dp.dim() - 3));
}
    
//! �J�����̎p�����w�肳�ꂽ�ʂ����X�V����D
/*!
  \f$\Delta\TUvec{p}{} = \Delta\TUvec{\theta}{}\f$�ɑ΂���
  \f[
  \TUtvec{R}{} \leftarrow \TUtvec{R}{}\TUtvec{R}{}(\Delta\TUvec{\theta}{})
  \f]
  �ƍX�V����D�J�����̈ʒu�͍X�V����Ȃ��D
  \param dp	�X�V�ʂ�\��3�����x�N�g��
*/
template <class T> inline void
CanonicalCamera<T>::updateFCC(const vector_type& dp)
{
    _Rt *= matrix_type::Rt(dp(0, 3));
}
    
//! ���̓X�g���[������J�����̊O���p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T> inline std::istream&
CanonicalCamera<T>::get(std::istream& in)
{
    typedef Vector<value_type, FixedSizedBuf<value_type, 3> >	vector3_type;
    
    const value_type	RAD = value_type(M_PI) / value_type(180);
    vector3_type	axis;
    in >> _t >> axis;
    _Rt = matrix33_type::Rt(RAD*axis);

    return in;
}

//! �o�̓X�g���[���ɃJ�����̊O���p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T> inline std::ostream&
CanonicalCamera<T>::put(std::ostream& out) const
{
    using namespace	std;

    const value_type	DEG = value_type(180) / value_type(M_PI);
    cerr << "Position:       ";
    out << _t;
    cerr << "Rotation(deg.): ";
    out << DEG*_Rt.rot2axis();

    return out;
}

//! ���̓X�g���[������J�����̊O���p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param camera	�O���p�����[�^�̓ǂݍ��ݐ�
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T> inline std::istream&
operator >>(std::istream& in, CanonicalCamera<T>& camera)
{
    return camera.get(in);
}

//! �o�̓X�g���[���ɃJ�����̊O���p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \param camera	�O���p�����[�^�̏����o����
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T> inline std::ostream&
operator <<(std::ostream& out, const CanonicalCamera<T>& camera)
{
    return camera.put(out);
}

/************************************************************************
*  class Camera<I>							*
************************************************************************/
//! �������e�J������\���N���X
template <class I>
class Camera : public CanonicalCamera<typename I::value_type>, public I
{
  public:
  //! �O���p�����[�^�̌^
    typedef CanonicalCamera<typename I::value_type>	extrinsic_type;
    typedef typename extrinsic_type::value_type		value_type;
    typedef typename extrinsic_type::point2_type	point2_type;
    typedef typename extrinsic_type::point3_type	point3_type;
    typedef typename extrinsic_type::vector_type	vector_type;
    typedef typename extrinsic_type::matrix_type	matrix_type;
    typedef typename extrinsic_type::matrix33_type	matrix33_type;
    typedef typename extrinsic_type::matrix34_type	matrix34_type;
  //! �����p�����[�^�̌^
    typedef I						intrinsic_type;
    
  public:
    Camera(const point3_type& t=point3_type(0, 0, 0),
	   const matrix33_type& Rt=matrix33_type::I(3),
	   const intrinsic_type& intrinsic=intrinsic_type())		;
    Camera(const extrinsic_type& extrinsic,
	   const intrinsic_type& intrinsic=intrinsic_type())		;
    Camera(const matrix34_type& P)					;

    point2_type		operator ()(const point3_type& X,
				    matrix_type* J=0,
				    matrix_type* H=0)		const	;
    matrix34_type	P()					const	;
    void		setProjection(const matrix34_type& P)		;

    static u_int	dof()						;
    void		update(const vector_type& dp)			;
    void		updateFCC(const vector_type& dp)		;

    std::istream&	get(std::istream& in)				;
    std::ostream&	put(std::ostream& out)			const	;
};

//! �ʒu�Ǝp������ѓ����p�����[�^���Z�b�g���ď���������D
/*!
  \param t		�J�����ʒu��\��3�����x�N�g��
  \param Rt		�J�����p����\��3x3��]�s��
  \param intrinsic	�����p�����[�^
*/
template <class I> inline
Camera<I>::Camera(const point3_type& t, const matrix33_type& Rt,
		  const intrinsic_type& intrinsic)
    :extrinsic_type(t, Rt), intrinsic_type(intrinsic)
{
}

//! �O���p�����[�^�Ɠ����p�����[�^���Z�b�g���ď���������D
/*!
  \param extrinsic	�O���p�����[�^
  \param intrinsic	�����p�����[�^
*/
template <class I> inline
Camera<I>::Camera(const extrinsic_type& extrinsic,
		  const intrinsic_type& intrinsic)
    :extrinsic_type(extrinsic), intrinsic_type(intrinsic)
{
}

//! ���e�s����Z�b�g���ď���������D
/*!
  \param P	3x4���e�s��
*/
template <class I> inline
Camera<I>::Camera(const matrix34_type& P)
    :extrinsic_type(), intrinsic_type()
{
    setProjection(P);
}

//! 3������Ԓ��̓_�̓��e�_�̉摜���W�n�ɂ�����ʒu�����߂�D
/*!
  \param X	�Ώۓ_��3�����ʒu
  \param J	0�łȂ���΃J�����̈ʒu�E�p������ѓ����p�����^�[�Ɋւ���
		���e�_��1�K�����C
		���Ȃ킿2 x dof() ���R�r�s��
		\f$
		\TUbeginarray{ccc}
		\TUdisppartial{\TUvec{u}{}}{\TUvec{t}{}} &
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\theta}{}} &
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}
		\TUendarray
		\f$
		���Ԃ����
  \param H	0�łȂ���΃J�����̈ʒu�E�p���Ɋւ��铊�e�_��2�K�����C
		���Ȃ킿2�� dof() x dof() �w�b�Z�s��
		\f$
		\TUbeginarray{ccc}
		\frac{\partial^2 u}{\partial\TUvec{t}{}^2} &
		\frac{\partial^2 u}
		     {\partial\TUvec{t}{}\partial\TUvec{\theta}{}} &
		\frac{\partial^2 u}
		     {\partial\TUvec{t}{}\partial\TUvec{\kappa}{}} \\
		\frac{\partial^2 u}
		     {\partial\TUvec{\theta}{}\partial\TUvec{t}{}} &
		\frac{\partial^2 u}{\partial\TUvec{\theta}{}^2} &
		\frac{\partial^2 u}
		     {\partial\TUvec{\theta}{}\partial\TUvec{\kappa}{}} \\
		\frac{\partial^2 u}
		     {\partial\TUvec{\kappa}{}\partial\TUvec{t}{}} &
		\frac{\partial^2 u}
		     {\partial\TUvec{\kappa}{}\partial\TUvec{\theta}{}} &
		\TUvec{O}{}
		\TUendarray
		\f$, 
		\f$
		\TUbeginarray{ccc}
		\frac{\partial^2 v}{\partial\TUvec{t}{}^2} &
		\frac{\partial^2 v}
		     {\partial\TUvec{t}{}\partial\TUvec{\theta}{}} &
		\frac{\partial^2 v}
		     {\partial\TUvec{t}{}\partial\TUvec{\kappa}{}} \\
		\frac{\partial^2 v}
		     {\partial\TUvec{\theta}{}\partial\TUvec{t}{}} &
		\frac{\partial^2 v}{\partial\TUvec{\theta}{}^2} &
		\frac{\partial^2 v}
		     {\partial\TUvec{\theta}{}\partial\TUvec{\kappa}{}} \\
		\frac{\partial^2 v}
		     {\partial\TUvec{\kappa}{}\partial\TUvec{t}{}} &
		\frac{\partial^2 v}
		     {\partial\TUvec{\kappa}{}\partial\TUvec{\theta}{}} &
		\TUvec{O}{}
		\TUendarray
		\f$
		���Ԃ����
  \return	X�̓��e�_�̉摜���W�n�ɂ�����ʒu
*/
template <class I> typename Camera<I>::point2_type
Camera<I>::operator ()(const point3_type& X,
		       matrix_type* J, matrix_type* H) const
{
    if (J)
    {
	J->resize(2, dof());
	
	if (H)
	{
	    H[0].resize(dof(), dof());
	    H[1].resize(dof(), dof());
	}
    }
    
    return intrinsic_type::u(x(X, J, H), J, H);
}

//! 3�������[�N���b�h��Ԃ���摜���ʂւ̓��e�s������߂�D
/*!
  \return	�摜���ʂւ̓��e�s��C���Ȃ킿
		\f$
		\TUvec{P}{} = \TUvec{K}{}\TUtvec{R}{}
		\TUbeginarray{cc}
		\TUvec{I}{3\times 3} & -\TUvec{t}{}
		\TUendarray
		\f$
*/
template <class I> inline typename Camera<I>::matrix34_type
Camera<I>::P() const
{
    return intrinsic_type::K() * extrinsic_type::Pc();
}

//! ���e�s����Z�b�g����D
/*!
  \param P	3x4���e�s��
  \return	���̃J����
*/
template <class I> void
Camera<I>::setProjection(const matrix34_type& P)
{
    typedef Vector<value_type, FixedSizedBuf<value_type, 3> >	vector3_type;

    matrix33_type	K;		// camera intrinsic parameters.
    K[0]    = P[2](0, 3);
    K[1]    = P[1](0, 3);
    K[2]    = P[0](0, 3);
    QRDecomposition<value_type>	qr(K);
    K[0][0] =  qr.Rt()[2][2];
    K[0][1] =  qr.Rt()[2][1];
    K[0][2] = -qr.Rt()[2][0];
    K[1][0] =  0.0;
    K[1][1] =  qr.Rt()[1][1];
    K[1][2] = -qr.Rt()[1][0];
    K[2][0] =  0.0;
    K[2][1] =  0.0;
    K[2][2] = -qr.Rt()[0][0];

    matrix33_type	Rt;		// camera rotation.
    Rt[0]   =  qr.Qt()[2];
    Rt[1]   =  qr.Qt()[1];
    Rt[2]   = -qr.Qt()[0];

    vector3_type	t;		// camera translation.
    t[0]    = -P[0][3];
    t[1]    = -P[1][3];
    t[2]    = -P[2][3];

  // Negate sign of P so that K has positive determinant.
    if (K[0][0] * K[1][1] * K[2][2] < value_type(0))
    {
	K *= -value_type(1);
	t *= -value_type(1);
    }
    
    if (K[0][0] < value_type(0))
    {
	K[0][0] *= -value_type(1);
	Rt[0]   *= -value_type(1);
    }
    if (K[1][1] < value_type(0))
    {
	K[0][1] *= -value_type(1);
	K[1][1] *= -value_type(1);
	Rt[1]   *= -value_type(1);
    }
    if (K[2][2] < value_type(0))
    {
	K[0][2] *= -value_type(1);
	K[1][2] *= -value_type(1);
	K[2][2] *= -value_type(1);
	Rt[2]   *= -value_type(1);
    }
    t = (K.inv() * t) * Rt;

    setTranslation(t);
    setRotation(Rt);
    setK(K);    
}

//! �J�����̊O���^�ϓ����p�����[�^�̎��R�x��Ԃ��D
/*!
  \return	�O���^�ϓ����p�����[�^�̎��R�x, ���Ȃ킿 6 + dofIntrinsic()
*/
template <class I> inline u_int
Camera<I>::dof()
{
    return extrinsic_type::dof() + intrinsic_type::dofIntrinsic();
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
  \param dp	�X�V�ʂ�\��6 + dofIntrinsic() �����x�N�g��
*/
template <class I> inline void
Camera<I>::update(const vector_type& dp)
{
    extrinsic_type::update(dp);
    updateIntrinsic(dp(6, dp.dim() - 6));
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
  \param dp	�X�V�ʂ�\��3 + dofIntrinsic() �����x�N�g��
*/
template <class I> inline void
Camera<I>::updateFCC(const vector_type& dp)
{
    extrinsic_type::updateFCC(dp);
    updateIntrinsic(dp(3, dp.dim() - 3));
}
    
//! ���̓X�g���[������J�����̊O��/�����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class I> inline std::istream&
Camera<I>::get(std::istream& in)
{
    extrinsic_type::get(in);
    return intrinsic_type::get(in);
}

//! �o�̓X�g���[���ɃJ�����̊O��/�����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class I> inline std::ostream&
Camera<I>::put(std::ostream& out) const
{
    extrinsic_type::put(out);
    return intrinsic_type::put(out);
}

//! ���̓X�g���[������J�����̊O���^�����p�����[�^��ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param camera	�O���^�����p�����[�^�̓ǂݍ��ݐ�
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class I> inline std::istream&
operator >>(std::istream& in, Camera<I>& camera)
{
    return camera.get(in);
}

//! �o�̓X�g���[���ɃJ�����̊O���^�����p�����[�^�������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \param camera	�O���^�����p�����[�^�̏����o����
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class I> inline std::ostream&
operator <<(std::ostream& out, const Camera<I>& camera)
{
    return camera.put(out);
}

}
#endif	/* !__TUCameraPP_h */
