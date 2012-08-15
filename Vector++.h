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
 *  $Id: Vector++.h,v 1.50 2012-08-15 07:17:55 ueshiba Exp $
 */
/*!
  \file		Vector++.h
  \brief	�x�N�g���ƍs�񂨂�т���Ɋ֘A����N���X�̒�`�Ǝ���
*/
#ifndef __TUVectorPP_h
#define __TUVectorPP_h

#include "TU/Array++.h"
#include <cmath>
#include <iomanip>

namespace TU
{
/************************************************************************
*  class Rotation<T>							*
************************************************************************/
//! 2���������ʓ��ł̉�]��\���N���X
/*!
  ��̓I�ɂ�
  \f[
    \TUvec{R}{}(p, q; \theta) \equiv
    \begin{array}{r@{}l}
      & \begin{array}{ccccccccccc}
        & & \makebox[4.0em]{} & p & & & \makebox[3.8em]{} & q & & &
      \end{array} \\
      \begin{array}{l}
        \\ \\ \\ \raisebox{1.5ex}{$p$} \\ \\ \\ \\ \raisebox{1.5ex}{$q$} \\ \\ \\
      \end{array} &
      \TUbeginarray{ccccccccccc}
	1 \\
	& \ddots \\
	& & 1 \\
	& & & \cos\theta & & & & -\sin\theta \\
	& & & & 1 \\
	& & & & & \ddots \\
	& & & & & & 1 \\
	& & & \sin\theta & & & & \cos\theta \\
	& & & & & & & & 1\\
	& & & & & & & & & \ddots \\
	& & & & & & & & & & 1
      \TUendarray
    \end{array}
  \f]
  �Ȃ��]�s��ŕ\�����D
*/
template <class T>
class Rotation
{
  public:
    typedef T	value_type;	//!< �����̌^
    
  public:
    Rotation(u_int p, u_int q, value_type x, value_type y)	;
    Rotation(u_int p, u_int q, value_type theta)		;

  //! p����Ԃ��D
  /*!
    \return	p����index
  */
    u_int	p()				const	{return _p;}

  //! q����Ԃ��D
  /*!
    \return	q����index
  */
    u_int	q()				const	{return _q;}

  //! ��]�p�����x�N�g���̒�����Ԃ��D
  /*!
    \return	��]�p�����x�N�g��(x, y)�ɑ΂���\f$\sqrt{x^2 + y^2}\f$
  */
    value_type	length()			const	{return _l;}

  //! ��]�p��cos�l��Ԃ��D
  /*!
    \return	��]�p��cos�l
  */
    value_type	cos()				const	{return _c;}

  //! ��]�p��sin�l��Ԃ��D
  /*!
    \return	��]�p��sin�l
  */
    value_type	sin()				const	{return _s;}
    
  private:
    const u_int	_p, _q;				// rotation axis
    value_type	_l;				// length of (x, y)
    value_type	_c, _s;				// cos & sin
};

//! 2���������ʓ��ł̉�]�𐶐�����
/*!
  \param p	p�����w�肷��index
  \param q	q�����w�肷��index
  \param x	��]�p�𐶐�����ۂ�x�l
  \param y	��]�p�𐶐�����ۂ�y�l
		\f[
		  \cos\theta = \frac{x}{\sqrt{x^2+y^2}},{\hskip 1em}
		  \sin\theta = \frac{y}{\sqrt{x^2+y^2}}
		\f]
*/
template <class T> inline
Rotation<T>::Rotation(u_int p, u_int q, value_type x, value_type y)
    :_p(p), _q(q), _l(1.0), _c(1.0), _s(0.0)
{
    const value_type	absx = std::fabs(x), absy = std::fabs(y);
    _l = (absx > absy ? absx * std::sqrt(1 + (absy*absy)/(absx*absx))
		      : absy * std::sqrt(1 + (absx*absx)/(absy*absy)));
    if (_l != 0.0)
    {
	_c = x / _l;
	_s = y / _l;
    }
}

//! 2���������ʓ��ł̉�]�𐶐�����
/*!
  \param p	p�����w�肷��index
  \param q	q�����w�肷��index
  \param theta	��]�p
*/
template <class T> inline
Rotation<T>::Rotation(u_int p, u_int q, value_type theta)
    :_p(p), _q(q), _l(1.0), _c(std::cos(theta)), _s(std::sin(theta))
{
}

/************************************************************************
*  class Vector<T>							*
************************************************************************/
template <class T, class B, class R>	class Matrix;

//! T�^�̐��������x�N�g����\���N���X
/*!
  \param T	�����̌^
  \param B	�o�b�t�@
*/
template <class T, class B=Buf<T> >
class Vector : public Array<T, B>
{
  private:
    typedef Array<T, B>					super;
    
  public:
    typedef typename super::value_type			value_type;
    typedef typename super::difference_type		difference_type;
    typedef typename super::reference			reference;
    typedef typename super::const_reference		const_reference;
    typedef typename super::pointer			pointer;
    typedef typename super::const_pointer		const_pointer;
    typedef typename super::iterator			iterator;
    typedef typename super::const_iterator		const_iterator;
    typedef typename super::reverse_iterator		reverse_iterator;
    typedef typename super::const_reverse_iterator	const_reverse_iterator;
  //! �����̌^��������3�����x�N�g���̌^
    typedef Vector<T, FixedSizedBuf<T, 3> >		vector3_type;
  //! �����̌^��������3x3�s��̌^
    typedef Matrix<T, FixedSizedBuf<T, 9>,
		   FixedSizedBuf<Vector<T>, 3> >	matrix33_type;
    
  public:
    Vector()								;
    explicit Vector(u_int d)						;
    Vector(T* p, u_int d)						;
    template <class B2>
    Vector(Vector<T, B2>& v, u_int i, u_int d)				;
    template <class T2, class B2>
    Vector(const Vector<T2, B2>& v)					;
    template <class T2, class B2>
    Vector&		operator =(const Vector<T2, B2>& v)		;

    using		super::begin;
    using		super::end;
    using		super::size;
    using		super::dim;
  //using		super::operator pointer;
  //using		super::operator const_pointer;
    using		super::check_dim;
    
    const Vector<T>	operator ()(u_int i, u_int d)		const	;
    Vector<T>		operator ()(u_int i, u_int d)			;
    Vector&		operator  =(const T& c)				;
    Vector&		operator *=(double c)				;
    Vector&		operator /=(double c)				;
    template <class T2, class B2>
    Vector&		operator +=(const Vector<T2, B2>& v)		;
    template <class T2, class B2>
    Vector&		operator -=(const Vector<T2, B2>& v)		;
    template <class T2, class B2>
    Vector&		operator ^=(const Vector<T2, B2>& V)		;
    template <class T2, class B2, class R2>
    Vector&		operator *=(const Matrix<T2, B2, R2>& m)	;
    Vector		operator  -()				const	;
    T			square()				const	;
    double		length()				const	;
    template <class T2, class B2>
    T			sqdist(const Vector<T2, B2>& v)		const	;
    template <class T2, class B2>
    double		dist(const Vector<T2, B2>& v)		const	;
    Vector&		normalize()					;
    Vector		normal()				const	;
    template <class T2, class B2, class R2>
    Vector&		solve(const Matrix<T2, B2, R2>& m)		;
    matrix33_type	skew()					const	;
    Vector<T>		homogeneous()				const	;
    Vector<T>		inhomogeneous()				const	;
    void		resize(u_int d)					;
    void		resize(T* p, u_int d)				;
};

//! �x�N�g���𐶐����C�S������0�ŏ���������D
template <class T, class B>
Vector<T, B>::Vector()
    :super()
{
    *this = 0;
}

//! �w�肳�ꂽ�����̃x�N�g���𐶐����C�S������0�ŏ���������D
/*!
  \param d	�x�N�g���̎���
*/
template <class T, class B> inline
Vector<T, B>::Vector(u_int d)
    :super(d)
{
    *this = 0;
}

//! �O���L���̈�Ǝ������w�肵�ăx�N�g���𐶐�����D
/*!
  \param p	�O���L���̈�ւ̃|�C���^
  \param d	�x�N�g���̎���
*/
template <class T, class B> inline
Vector<T, B>::Vector(T* p, u_int d)
    :super(p, d)
{
}

//! �^����ꂽ�x�N�g���ƋL���̈�����L���镔���x�N�g���𐶐�����D
/*!
  \param v	���̃x�N�g��
  \param i	�����x�N�g���̑�0�������w�肷��index
  \param d	�����x�N�g���̎���
*/
template <class T, class B> template <class B2> inline
Vector<T, B>::Vector(Vector<T, B2>& v, u_int i, u_int d)
    :super(v, i, d)
{
}

//! ���̃x�N�g���Ɠ��ꐬ�������x�N�g�������(�R�s�[�R���X�g���N�^�̊g��)�D
/*!
  \param v	�R�s�[���x�N�g��
*/
template <class T, class B> template <class T2, class B2> inline
Vector<T, B>::Vector(const Vector<T2, B2>& v)
    :super(v)
{
}
    
//! ���̃x�N�g���������ɑ������(������Z�q�̊g��)�D
/*!
  \param v	�R�s�[���x�N�g��
  \return	���̃x�N�g��
*/
template <class T, class B> template <class T2, class B2> inline Vector<T, B>&
Vector<T, B>::operator =(const Vector<T2, B2>& v)
{
    super::operator =(v);
    return *this;
}

//! ���̃x�N�g���ƋL���̈�����L���������x�N�g���𐶐�����D
/*!
    \param i	�����x�N�g���̑�0�������w�肷��index
    \param d	�����x�N�g���̎���
    \return	�������ꂽ�����x�N�g��
*/
template <class T, class B> inline Vector<T>
Vector<T, B>::operator ()(u_int i, u_int d)
{
    return Vector<T>(*this, i, d);
}

//! ���̃x�N�g���ƋL���̈�����L���������x�N�g���𐶐�����D
/*!
    \param i	�����x�N�g���̑�0�������w�肷��index
    \param d	�����x�N�g���̎���
    \return	�������ꂽ�����x�N�g��
*/
template <class T, class B> inline const Vector<T>
Vector<T, B>::operator ()(u_int i, u_int d) const
{
    return Vector<T>(const_cast<Vector<T, B>&>(*this), i, d);
}

//! ���̃x�N�g���̑S�Ă̐����ɓ���̐��l��������D
/*!
  \param c	������鐔�l
  \return	���̃x�N�g��
*/
template <class T, class B> inline Vector<T, B>&
Vector<T, B>::operator =(const T& c)
{
    super::operator =(c);
    return *this;
}

//! ���̃x�N�g���Ɏw�肳�ꂽ���l���|����D
/*!
  \param c	�|���鐔�l
  \return	���̃x�N�g���C���Ȃ킿\f$\TUvec{u}{}\leftarrow c\TUvec{u}{}\f$
*/
template <class T, class B> inline Vector<T, B>&
Vector<T, B>::operator *=(double c)
{
    super::operator *=(c);
    return *this;
}

//! ���̃x�N�g�����w�肳�ꂽ���l�Ŋ���D
/*!
  \param c	���鐔�l
  \return	���̃x�N�g���C���Ȃ킿
		\f$\TUvec{u}{}\leftarrow \frac{\TUvec{u}{}}{c}\f$
*/
template <class T, class B> inline Vector<T, B>&
Vector<T, B>::operator /=(double c)
{
    super::operator /=(c);
    return *this;
}

//! ���̃x�N�g���ɑ��̃x�N�g���𑫂��D
/*!
  \param v	�����x�N�g��
  \return	���̃x�N�g���C���Ȃ킿
		\f$\TUvec{u}{}\leftarrow \TUvec{u}{} + \TUvec{v}{}\f$
*/
template <class T, class B> template <class T2, class B2> inline Vector<T, B>&
Vector<T, B>::operator +=(const Vector<T2, B2>& v)
{
    super::operator +=(v);
    return *this;
}

//! ���̃x�N�g�����瑼�̃x�N�g���������D
/*!
  \param v	�����x�N�g��
  \return	���̃x�N�g���C���Ȃ킿
		\f$\TUvec{u}{}\leftarrow \TUvec{u}{} - \TUvec{v}{}\f$
*/
template <class T, class B> template <class T2, class B2> inline Vector<T, B>&
Vector<T, B>::operator -=(const Vector<T2, B2>& v)
{
    super::operator -=(v);
    return *this;
}

//! ���̃x�N�g���Ƒ���3�����x�N�g���Ƃ̃x�N�g���ς��Ƃ�D
/*!
    \param v	���̃x�N�g��
    \return	���̃x�N�g���C���Ȃ킿
		\f$\TUvec{u}{}\leftarrow \TUvec{u}{}\times\TUvec{v}{}\f$
    \throw std::invalid_argument	���̃x�N�g����v��3�����łȂ��ꍇ�ɑ��o
*/
template <class T, class B> template <class T2, class B2> Vector<T, B>&
Vector<T, B>::operator ^=(const Vector<T2, B2>& v)	// outer product
{
    check_dim(v.dim());
    if (dim() != 3)
	throw std::invalid_argument("TU::Vector<T, B>::operator ^=: dimension must be 3");
    vector3_type	tmp(*this);
    (*this)[0] = tmp[1] * v[2] - tmp[2] * v[1];
    (*this)[1] = tmp[2] * v[0] - tmp[0] * v[2];
    (*this)[2] = tmp[0] * v[1] - tmp[1] * v[0];
    return *this;
}

//! ���̃x�N�g���̉E����s����|����D
/*!
  \param m	�|����s��
  \return	���̃x�N�g���C���Ȃ킿
		\f$\TUtvec{u}{} \leftarrow \TUtvec{u}{}\TUvec{M}{}\f$
*/
template <class T, class B> template <class T2, class B2, class R2>
inline Vector<T, B>&
Vector<T, B>::operator *=(const Matrix<T2, B2, R2>& m)
{
    return *this = *this * m;
}

//! ���̃x�N�g���̕����𔽓]�����x�N�g����Ԃ��D
/*!
  \return	�����𔽓]�����x�N�g���C���Ȃ킿\f$-\TUvec{u}{}\f$
*/
template <class T, class B> inline Vector<T, B>
Vector<T, B>::operator -() const
{
    return Vector(*this) *= -1;
}

//! ���̃x�N�g���̒�����2���Ԃ��D
/*!
  \return	�x�N�g���̒�����2��C���Ȃ킿\f$\TUnorm{\TUvec{u}{}}^2\f$
*/
template <class T, class B> inline T
Vector<T, B>::square() const
{
    return *this * *this;
}

//! ���̃x�N�g���̒�����Ԃ��D
/*!
  \return	�x�N�g���̒����C���Ȃ킿\f$\TUnorm{\TUvec{u}{}}\f$
*/
template <class T, class B> inline double
Vector<T, B>::length() const
{
    return std::sqrt(double(square()));
}

//! ���̃x�N�g���Ƒ��̃x�N�g���̍��̒�����2���Ԃ��D
/*!
  \param v	��r�ΏۂƂȂ�x�N�g��
  \return	�x�N�g���Ԃ̍���2��C���Ȃ킿
		\f$\TUnorm{\TUvec{u}{} - \TUvec{v}{}}^2\f$
*/
template <class T, class B> template <class T2, class B2> inline T
Vector<T, B>::sqdist(const Vector<T2, B2>& v) const
{
    return (*this - v).square();
}

//! ���̃x�N�g���Ƒ��̃x�N�g���̍��̒�����Ԃ��D
/*!
  \param v	��r�ΏۂƂȂ�x�N�g��
  \return	�x�N�g���Ԃ̍��C���Ȃ킿
		\f$\TUnorm{\TUvec{u}{} - \TUvec{v}{}}\f$
*/
template <class T, class B> template <class T2, class B2> inline double
Vector<T, B>::dist(const Vector<T2, B2>& v) const
{
    return std::sqrt(double(sqdist(v)));
}

//! ���̃x�N�g���̒�����1�ɐ��K������D
/*!
  \return	���̃x�N�g���C���Ȃ킿
		\f$
		  \TUvec{u}{}\leftarrow\frac{\TUvec{u}{}}{\TUnorm{\TUvec{u}{}}}
		\f$
*/
template <class T, class B> inline Vector<T, B>&
Vector<T, B>::normalize()
{
    return *this /= length();
}

//! ���̃x�N�g���̒�����1�ɐ��K�������x�N�g����Ԃ��D
/*!
  \return	�����𐳋K�������x�N�g���C���Ȃ킿
		\f$\frac{\TUvec{u}{}}{\TUnorm{\TUvec{u}{}}}\f$
*/
template <class T, class B> inline Vector<T, B>
Vector<T, B>::normal() const
{
    return Vector(*this).normalize();
}

//! ����3�����x�N�g������3x3���Ώ̍s��𐶐�����D
/*!
  \return	�������ꂽ���Ώ̍s��C���Ȃ킿
  \f[
    \TUskew{u}{} \equiv
    \TUbeginarray{ccc}
      & -u_2 & u_1 \\ u_2 & & -u_0 \\ -u_1 & u_0 &
    \TUendarray
  \f]
  \throw std::invalid_argument	3�����x�N�g���łȂ��ꍇ�ɑ��o
*/
template <class T, class B>
Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3> >
Vector<T, B>::skew() const
{
    if (dim() != 3)
	throw std::invalid_argument("TU::Vector<T, B>::skew: dimension must be 3");
    matrix33_type	r;
    r[2][1] = (*this)[0];
    r[0][2] = (*this)[1];
    r[1][0] = (*this)[2];
    r[1][2] = -r[2][1];
    r[2][0] = -r[0][2];
    r[0][1] = -r[1][0];
    return r;
}

//! �񓯎����W��\���x�N�g���ɑ΂��C�l1�����������Ō�ɕt�������������W�x�N�g����Ԃ��D
/*!
  \return	���������ꂽ�x�N�g��
*/
template <class T, class B> inline Vector<T>
Vector<T, B>::homogeneous() const
{
    Vector<T>	v(dim() + 1);
    v(0, dim()) = *this;
    v[dim()]	= 1;
    return v;
}

//! �������W��\���x�N�g���ɑ΂��C�e�������Ō�̐����Ŋ������񓯎����W�x�N�g����Ԃ��D
/*!
  \return	�񓯎������ꂽ�x�N�g��
*/
template <class T, class B> inline Vector<T>
Vector<T, B>::inhomogeneous() const
{
    return (*this)(0, dim()-1) / (*this)[dim()-1];
}

//! �x�N�g���̎�����ύX���C�S������0�ɏ���������D
/*!
  �������C���̃I�u�W�F�N�g�ƋL���̈�����L���Ă���x�N�g���̎�����
  �ύX���邱�Ƃ͂ł��Ȃ��D
  \param d	�V��������
*/
template <class T, class B> inline void
Vector<T, B>::resize(u_int d)
{
    super::resize(d);
    *this = 0;
}

//! �x�N�g���������Ŏg�p����L���̈���w�肵�����̂ɕύX����D
/*!
  \param p	�V�����L���̈�ւ̃|�C���^
  \param d	�V��������
*/
template <class T, class B> inline void
Vector<T, B>::resize(T* p, u_int d)
{
    super::resize(p, d);
}

/************************************************************************
*  class Matrix<T, B, R>						*
************************************************************************/
template <class T>	class TriDiagonal;
template <class T>	class SVDecomposition;

//! T�^�̐��������s���\���N���X
/*!
  �e�s��T�^�̐��������x�N�g��#TU::Vector<T>�ɂȂ��Ă���D
  \param T	�����̌^
  \param B	�o�b�t�@
  \param R	�s�o�b�t�@
*/
template <class T, class B=Buf<T>, class R=Buf<Vector<T> > >
class Matrix : public Array2<Vector<T>, B, R>
{
  private:
    typedef Array2<Vector<T>, B, R>			super;
    
  public:
    typedef typename super::value_type			value_type;
    typedef typename super::difference_type		difference_type;
    typedef typename super::reference			reference;
    typedef typename super::const_reference		const_reference;
    typedef typename super::pointer			pointer;
    typedef typename super::const_pointer		const_pointer;
    typedef typename super::iterator			iterator;
    typedef typename super::const_iterator		const_iterator;
  //! �����̌^��������3�����x�N�g���̌^
    typedef Vector<T, FixedSizedBuf<T, 3> >		vector3_type;
  //! �����̌^��������4�����x�N�g���̌^
    typedef Vector<T, FixedSizedBuf<T, 4> >		vector4_type;
  //! �����̌^��������3x3�s��̌^
    typedef Matrix<T, FixedSizedBuf<T, 9>,
		   FixedSizedBuf<Vector<T>, 3> >	matrix33_type;
    
  public:
    Matrix()								;
    Matrix(u_int r, u_int c)						;
    Matrix(T* p, u_int r, u_int c)					;
    template <class B2, class R2>
    Matrix(Matrix<T, B2, R2>& m, u_int i, u_int j, u_int r, u_int c)	;
    template <class T2, class B2, class R2>
    Matrix(const Matrix<T2, B2, R2>& m)					;
    template <class T2, class B2, class R2>
    Matrix&		operator =(const Matrix<T2, B2, R2>& m)		;

    using		super::begin;
    using		super::end;
    using		super::size;
    using		super::dim;
    using		super::nrow;
    using		super::ncol;
    using		super::check_dim;
    
			operator const Vector<T>()		const	;
    const Matrix<T>	operator ()(u_int i, u_int j,
				    u_int r, u_int c)		const	;
    Matrix<T>		operator ()(u_int i, u_int j,
				    u_int r, u_int c)			;
    Matrix&		operator  =(const T& c)				;
    Matrix&		operator *=(double c)				;
    Matrix&		operator /=(double c)				;
    template <class T2, class B2, class R2>
    Matrix&		operator +=(const Matrix<T2, B2, R2>& m)	;
    template <class T2, class B2, class R2>
    Matrix&		operator -=(const Matrix<T2, B2, R2>& m)	;
    template <class T2, class B2, class R2>
    Matrix&		operator *=(const Matrix<T2, B2, R2>& m)	;
    template <class T2, class B2>
    Matrix&		operator ^=(const Vector<T2, B2>& v)		;
    Matrix		operator  -()				const	;
    Matrix&		diag(T c)					;
    Matrix<T>		trns()					const	;
    Matrix		inv()					const	;
    template <class T2, class B2, class R2>
    Matrix&		solve(const Matrix<T2, B2, R2>& m)		;
    T			det()					const	;
    T			det(u_int p, u_int q)			const	;
    T			trace()					const	;
    Matrix		adj()					const	;
    Matrix<T>		pinv(T cndnum=1.0e5)			const	;
    Matrix<T>		eigen(Vector<T>& eval, bool abs=true)	const	;
    Matrix<T>		geigen(const Matrix<T>& BB,
			       Vector<T>& eval, bool abs=true)	const	;
    Matrix		cholesky()				const	;
    Matrix&		normalize()					;
    Matrix&		rotate_from_left(const Rotation<T>& r)		;
    Matrix&		rotate_from_right(const Rotation<T>& r)		;
    T			square()				const	;
    double		length()				const	;
    Matrix&		symmetrize()					;
    Matrix&		antisymmetrize()				;
    void		rot2angle(T& theta_x,
				  T& theta_y, T& theta_z)	const	;
    vector3_type	rot2axis(T& c, T& s)			const	;
    vector3_type	rot2axis()				const	;
    vector4_type	rot2quaternion()			const	;

    static Matrix	I(u_int d)					;
    template <class T2, class B2>
    static matrix33_type
			Rt(const Vector<T2, B2>& n, T c, T s)		;
    template <class T2, class B2>
    static matrix33_type
			Rt(const Vector<T2, B2>& v)			;

    void		resize(u_int r, u_int c)			;
    void		resize(T* p, u_int r, u_int c)			;
};

//! �s��𐶐����C�S������0�ŏ���������D
template <class T, class B, class R> inline
Matrix<T, B, R>::Matrix()
    :super()
{
    *this = 0;
}

//! �w�肳�ꂽ�T�C�Y�̍s��𐶐����C�S������0�ŏ���������D
/*!
  \param r	�s��̍s��
  \param c	�s��̗�
*/
template <class T, class B, class R> inline
Matrix<T, B, R>::Matrix(u_int r, u_int c)
    :super(r, c)
{
    *this = 0;
}

//! �O���L���̈�ƃT�C�Y���w�肵�čs��𐶐�����D
/*!
  \param p	�O���L���̈�ւ̃|�C���^
  \param r	�s��̍s��
  \param c	�s��̗�
*/
template <class T, class B, class R> inline
Matrix<T, B, R>::Matrix(T* p, u_int r, u_int c)
    :super(p, r, c)
{
}

//! �^����ꂽ�s��ƋL���̈�����L���镔���s��𐶐�����D
/*!
  \param m	���̍s��
  \param i	�����s��̑�0�s���w�肷��index
  \param j	�����s��̑�0����w�肷��index
  \param r	�����s��̍s��
  \param c	�����s��̗�
*/
template <class T, class B, class R> template <class B2, class R2> inline
Matrix<T, B, R>::Matrix(Matrix<T, B2, R2>& m,
			u_int i, u_int j, u_int r, u_int c)
    :super(m, i, j, r, c)
{
}

//! ���̍s��Ɠ��ꐬ�������s������(�R�s�[�R���X�g���N�^�̊g��)�D
/*!
  \param m	�R�s�[���s��
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline
Matrix<T, B, R>::Matrix(const Matrix<T2, B2, R2>& m)
    :super(m)
{
}

//! ���̍s��������ɑ������(������Z�q�̊g��)�D
/*!
  \param m	�R�s�[���s��
  \return	���̍s��
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline Matrix<T, B, R>&
Matrix<T, B, R>::operator =(const Matrix<T2, B2, R2>& m)
{
    super::operator =(m);
    return *this;
}

//! ���̍s��̍s����ׂċL���̈�����L����x�N�g���𐶐�����D
/*!
  �S�s�̋L���̈�͘A�����Ă��Ȃ���΂Ȃ�Ȃ��D
  \return	�L���̈�����L����x�N�g��
*/
template <class T, class B, class R> inline
Matrix<T, B, R>::operator const Vector<T>() const
{
    return Vector<T>(const_cast<T*>((const T*)*this), nrow()*ncol());
}

//! ���̍s��ƋL���̈�����L���������s��𐶐�����D
/*!
    \param i	�����s��̍���������ƂȂ�s���w�肷��index
    \param j	�����s��̍���������ƂȂ����w�肷��index
    \param r	�����s��̍s��
    \param c	�����s��̗�
    \return	�������ꂽ�����s��
*/
template <class T, class B, class R> inline Matrix<T>
Matrix<T, B, R>::operator ()(u_int i, u_int j, u_int r, u_int c)
{
    return Matrix<T>(*this, i, j, r, c);
}

//! ���̍s��ƋL���̈�����L���������s��𐶐�����D
/*!
    \param i	�����s��̍���������ƂȂ�s���w�肷��index
    \param j	�����s��̍���������ƂȂ����w�肷��index
    \param r	�����s��̍s��
    \param c	�����s��̗�
    \return	�������ꂽ�����s��
*/
template <class T, class B, class R> inline const Matrix<T>
Matrix<T, B, R>::operator ()(u_int i, u_int j, u_int r, u_int c) const
{
    return Matrix<T>(const_cast<Matrix<T, B, R>&>(*this), i, j, r, c);
}

//! ���̍s��̑S�Ă̐����ɓ���̐��l��������D
/*!
  \param c	������鐔�l
  \return	���̍s��
*/
template <class T, class B, class R> inline Matrix<T, B, R>&
Matrix<T, B, R>::operator =(const T& c)
{
    super::operator =(c);
    return *this;
}

//! ���̍s��Ɏw�肳�ꂽ���l���|����D
/*!
  \param c	�|���鐔�l
  \return	���̍s��C���Ȃ킿\f$\TUvec{A}{}\leftarrow c\TUvec{A}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>&
Matrix<T, B, R>::operator *=(double c)
{
    super::operator *=(c);
    return *this;
}

//! ���̍s����w�肳�ꂽ���l�Ŋ���D
/*!
  \param c	���鐔�l
  \return	���̍s��C���Ȃ킿
		\f$\TUvec{A}{}\leftarrow \frac{\TUvec{A}{}}{c}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>&
Matrix<T, B, R>::operator /=(double c)
{
    super::operator /=(c);
    return *this;
}

//! ���̍s��ɑ��̍s��𑫂��D
/*!
  \param m	�����s��
  \return	���̍s��C���Ȃ킿
		\f$\TUvec{A}{}\leftarrow \TUvec{A}{} + \TUvec{M}{}\f$
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline Matrix<T, B, R>&
Matrix<T, B, R>::operator +=(const Matrix<T2, B2, R2>& m)
{
    super::operator +=(m);
    return *this;
}

//! ���̍s�񂩂瑼�̍s��������D
/*!
  \param m	�����s��
  \return	���̍s��C���Ȃ킿
		\f$\TUvec{A}{}\leftarrow \TUvec{A}{} - \TUvec{M}{}\f$
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline Matrix<T, B, R>&
Matrix<T, B, R>::operator -=(const Matrix<T2, B2, R2>& m)
{
    super::operator -=(m);
    return *this;
}

//! ���̍s��ɑ��̍s����|����D
/*!
  \param m	�|����s��
  \return	���̍s��C���Ȃ킿
		\f$\TUvec{A}{}\leftarrow \TUvec{A}{}\TUvec{M}{}\f$
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline Matrix<T, B, R>&
Matrix<T, B, R>::operator *=(const Matrix<T2, B2, R2>& m)
{
    return *this = *this * m;
}

//! ����?x3�s��̊e�s��3�����x�N�g���Ƃ̃x�N�g���ς��Ƃ�D
/*!
  \param v	3�����x�N�g��
  \return	���̍s��C���Ȃ킿
		\f$\TUvec{A}{}\leftarrow(\TUtvec{A}{}\times\TUvec{v}{})^\top\f$
*/
template <class T, class B, class R> template <class T2, class B2>
Matrix<T, B, R>&
Matrix<T, B, R>::operator ^=(const Vector<T2, B2>& v)
{
    for (u_int i = 0; i < nrow(); ++i)
	(*this)[i] ^= v;
    return *this;
}

//! ���̍s��̕����𔽓]�����s���Ԃ��D
/*!
  \return	�����𔽓]�����s��C���Ȃ킿\f$-\TUvec{A}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
Matrix<T, B, R>::operator -() const
{
    return Matrix(*this) *= -1;
}

//! ���̐����s���S�ē���̑Ίp�����l�����Ίp�s��ɂ���D
/*!
  \param c	�Ίp�����̒l
  \return	���̍s��C���Ȃ킿\f$\TUvec{A}{} \leftarrow \diag(c,\ldots,c)\f$
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::diag(T c)
{
    check_dim(ncol());
    *this = 0;
    for (u_int i = 0; i < nrow(); ++i)
	(*this)[i][i] = c;
    return *this;
}

//! ���̍s��̓]�u�s���Ԃ��D
/*!
  \return	�]�u�s��C���Ȃ킿\f$\TUtvec{A}{}\f$
*/
template <class T, class B, class R> Matrix<T>
Matrix<T, B, R>::trns() const
{
    Matrix<T> val(ncol(), nrow());
    for (u_int i = 0; i < nrow(); ++i)
	for (u_int j = 0; j < ncol(); ++j)
	    val[j][i] = (*this)[i][j];
    return val;
}

//! ���̍s��̋t�s���Ԃ��D
/*!
  \return	�t�s��C���Ȃ킿\f$\TUinv{A}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
Matrix<T, B, R>::inv() const
{
    return I(nrow()).solve(*this);
}

//! ���̍s��̏��s�񎮂�Ԃ��D
/*!
  \param p	���̍s�񂩂��菜���s���w�肷��index
  \param q	���̍s�񂩂��菜������w�肷��index
  \return	���s�񎮁C���Ȃ킿\f$\det\TUvec{A}{pq}\f$
*/
template <class T, class B, class R> T
Matrix<T, B, R>::det(u_int p, u_int q) const
{
    Matrix<T>		d(nrow()-1, ncol()-1);
    for (u_int i = 0; i < p; ++i)
    {
	for (u_int j = 0; j < q; ++j)
	    d[i][j] = (*this)[i][j];
	for (u_int j = q; j < d.ncol(); ++j)
	    d[i][j] = (*this)[i][j+1];
    }
    for (u_int i = p; i < d.nrow(); ++i)
    {
	for (u_int j = 0; j < q; ++j)
	    d[i][j] = (*this)[i+1][j];
	for (u_int j = q; j < d.ncol(); ++j)
	    d[i][j] = (*this)[i+1][j+1];
    }
    return d.det();
}

//! ���̐����s���trace��Ԃ��D
/*!
  \return			trace, ���Ȃ킿\f$\trace\TUvec{A}{}\f$
  \throw std::invalid_argument	�����s��łȂ��ꍇ�ɑ��o
*/
template <class T, class B, class R> T
Matrix<T, B, R>::trace() const
{
    if (nrow() != ncol())
        throw
	  std::invalid_argument("TU::Matrix<T>::trace(): not square matrix!!");
    T	val = 0.0;
    for (u_int i = 0; i < nrow(); ++i)
	val += (*this)[i][i];
    return val;
}

//! ���̍s��̗]���q�s���Ԃ��D
/*!
  \return	�]���q�s��C���Ȃ킿
		\f$\TUtilde{A}{} = (\det\TUvec{A}{})\TUinv{A}{}\f$
*/
template <class T, class B, class R> Matrix<T, B, R>
Matrix<T, B, R>::adj() const
{
    Matrix<T, B, R>	val(nrow(), ncol());
    for (u_int i = 0; i < val.nrow(); ++i)
	for (u_int j = 0; j < val.ncol(); ++j)
	    val[i][j] = ((i + j) % 2 ? -det(j, i) : det(j, i));
    return val;
}

//! ���̍s��̋^���t�s���Ԃ��D
/*!
  \param cndnum	�ő���ْl�ɑ΂����Βl�̊���������ɒB���Ȃ����͖���
  \return	�^���t�s��C���Ȃ킿�^����ꂽ�s��̓��ْl������
		\f$\TUvec{A}{} = \TUvec{V}{}\diag(\sigma_0,\ldots,\sigma_{n-1})
		\TUtvec{U}{}\f$�Ƃ����
		\f[
		  \TUvec{u}{0}\sigma_0^{-1}\TUtvec{v}{0} + \cdots +
		  \TUvec{u}{r}\sigma_{r-1}^{-1}\TUtvec{v}{r-1},
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \TUabs{\sigma_1} > \epsilon\TUabs{\sigma_0},\ldots,
		  \TUabs{\sigma_{r-1}} > \epsilon\TUabs{\sigma_0}
		\f]
*/
template <class T, class B, class R> Matrix<T>
Matrix<T, B, R>::pinv(T cndnum) const
{
    SVDecomposition<T>	svd(*this);
    Matrix<T>		val(svd.ncol(), svd.nrow());
    
    for (u_int i = 0; i < svd.diagonal().dim(); ++i)
	if (std::fabs(svd[i]) * cndnum > std::fabs(svd[0]))
	    val += (svd.Ut()[i] / svd[i]) % svd.Vt()[i];

    return val;
}

//! ���̑Ώ̍s��̌ŗL�l�ƌŗL�x�N�g����Ԃ��D
/*!
    \param eval	�ŗL�l���Ԃ����
    \param abs	�ŗL�l���Βl�̑傫�����ɕ��ׂ�Ȃ�true, �l�̑傫������
		���ׂ�Ȃ�false
    \return	�e�s���ŗL�x�N�g�����琬���]�s��C���Ȃ킿
		\f[
		  \TUvec{A}{}\TUvec{U}{} =
		  \TUvec{U}{}\diag(\lambda_0,\ldots,\lambda_{n-1}),
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \TUtvec{U}{}\TUvec{U}{} = \TUvec{I}{n},~\det\TUvec{U}{} = 1
		\f]
		�Ȃ�\f$\TUtvec{U}{}\f$
*/
template <class T, class B, class R> Matrix<T>
Matrix<T, B, R>::eigen(Vector<T>& eval, bool abs) const
{
    TriDiagonal<T>	tri(*this);

    tri.diagonalize(abs);
    eval = tri.diagonal();

    return tri.Ut();
}

//! ���̑Ώ̍s��̈�ʌŗL�l�ƈ�ʌŗL�x�N�g����Ԃ��D
/*!
    \param BB	���Ƃ̍s��Ɠ���T�C�Y�̐��l�Ώ̍s��
    \param eval	��ʌŗL�l���Ԃ����
    \param abs	��ʌŗL�l���Βl�̑傫�����ɕ��ׂ�Ȃ�true, �l�̑傫������
		���ׂ�Ȃ�false
    \return	�e�s����ʌŗL�x�N�g�����琬�鐳���s��
		�i�����������s��ł͂Ȃ��j�C���Ȃ킿
		\f[
		  \TUvec{A}{}\TUvec{U}{} =
		  \TUvec{B}{}\TUvec{U}{}\diag(\lambda_0,\ldots,\lambda_{n-1}),
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \TUtvec{U}{}\TUvec{B}{}\TUvec{U}{} = \TUvec{I}{n}
		\f]
		�Ȃ�\f$\TUtvec{U}{}\f$
*/
template <class T, class B, class R> Matrix<T>
Matrix<T, B, R>::geigen(const Matrix<T>& BB, Vector<T>& eval, bool abs) const
{
    Matrix<T>	Ltinv = BB.cholesky().inv(), Linv = Ltinv.trns();
    Matrix<T>	Ut = (Linv * (*this) * Ltinv).eigen(eval, abs);
    
    return Ut * Linv;
}

//! ���̐��l�Ώ̍s���Cholesky�����i�㔼�O�p�s��j��Ԃ��D
/*!
  �v�Z�ɂ����ẮC���Ƃ̍s��̏㔼���������g��Ȃ�
  \return	\f$\TUvec{A}{} = \TUvec{L}{}\TUtvec{L}{}\f$�Ȃ�
		\f$\TUtvec{L}{}\f$�i�㔼�O�p�s��j
  \throw std::invalid_argument	�����s��łȂ��ꍇ�ɑ��o
  \throw std::runtime_error	���l�łȂ��ꍇ�ɑ��o
*/
template <class T, class B, class R> Matrix<T, B, R>
Matrix<T, B, R>::cholesky() const
{
    if (nrow() != ncol())
        throw
	    std::invalid_argument("TU::Matrix<T>::cholesky(): not square matrix!!");

    Matrix<T, B, R>	Lt(*this);
    for (u_int i = 0; i < nrow(); ++i)
    {
	T d = Lt[i][i];
	if (d <= 0)
	    throw std::runtime_error("TU::Matrix<T>::cholesky(): not positive definite matrix!!");
	for (u_int j = 0; j < i; ++j)
	    Lt[i][j] = 0;
	Lt[i][i] = d = std::sqrt(d);
	for (u_int j = i + 1; j < ncol(); ++j)
	    Lt[i][j] /= d;
	for (u_int j = i + 1; j < nrow(); ++j)
	    for (u_int k = j; k < ncol(); ++k)
		Lt[j][k] -= (Lt[i][j] * Lt[i][k]);
    }
    
    return Lt;
}

//! ���̍s��̃m������1�ɐ��K������D
/*!
    \return	���̍s��C���Ȃ킿
		\f$
		  \TUvec{A}{}\leftarrow\frac{\TUvec{A}{}}{\TUnorm{\TUvec{A}{}}}
		\f$
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::normalize()
{
    T	sum = 0.0;
    for (u_int i = 0; i < nrow(); ++i)
	sum += (*this)[i] * (*this)[i];
    return *this /= std::sqrt(sum);
}

//! ���̍s��̍�����i�]�u���ꂽ�j��]�s����|����D
/*!
    \return	���̍s��C���Ȃ킿
		\f$\TUvec{A}{}\leftarrow\TUtvec{R}{}\TUvec{A}{}\f$
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::rotate_from_left(const Rotation<T>& r)
{
    for (u_int j = 0; j < ncol(); ++j)
    {
	const T	tmp = (*this)[r.p()][j];
	
	(*this)[r.p()][j] =  r.cos()*tmp + r.sin()*(*this)[r.q()][j];
	(*this)[r.q()][j] = -r.sin()*tmp + r.cos()*(*this)[r.q()][j];
    }
    return *this;
}

//! ���̍s��̉E�����]�s����|����D
/*!
    \return	���̍s��C���Ȃ킿
		\f$\TUvec{A}{}\leftarrow\TUvec{A}{}\TUvec{R}{}\f$
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::rotate_from_right(const Rotation<T>& r)
{
    for (u_int i = 0; i < nrow(); ++i)
    {
	const T	tmp = (*this)[i][r.p()];
	
	(*this)[i][r.p()] =  tmp*r.cos() + (*this)[i][r.q()]*r.sin();
	(*this)[i][r.q()] = -tmp*r.sin() + (*this)[i][r.q()]*r.cos();
    }
    return *this;
}

//! ���̍s���2��m������2���Ԃ��D
/*!
    \return	�s���2��m������2��C���Ȃ킿\f$\TUnorm{\TUvec{A}{}}^2\f$
*/
template <class T, class B, class R> T
Matrix<T, B, R>::square() const
{
    T	val = 0.0;
    for (u_int i = 0; i < nrow(); ++i)
	val += (*this)[i] * (*this)[i];
    return val;
}

//! ���̍s���2��m������Ԃ��D
/*!
  \return	�s���2��m�����C���Ȃ킿\f$\TUnorm{\TUvec{A}{}}\f$
*/
template <class T, class B, class R> inline double
Matrix<T, B, R>::length() const
{
    return std::sqrt(double(square()));
}

//! ���̍s��̉����O�p�������㔼�O�p�����ɃR�s�[���đΏ̉�����D
/*!
    \return	���̍s��
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::symmetrize()
{
    for (u_int i = 0; i < nrow(); ++i)
	for (u_int j = 0; j < i; ++j)
	    (*this)[j][i] = (*this)[i][j];
    return *this;
}

//! ���̍s��̉����O�p�����̕����𔽓]���C�㔼�O�p�����ɃR�s�[���Ĕ��Ώ̉�����D
/*!
    \return	���̍s��
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::antisymmetrize()
{
    for (u_int i = 0; i < nrow(); ++i)
    {
	(*this)[i][i] = 0.0;
	for (u_int j = 0; j < i; ++j)
	    (*this)[j][i] = -(*this)[i][j];
    }
    return *this;
}

//! ����3������]�s�񂩂�e������̉�]�p�����o���D
/*!
  ���̍s���\f$\TUtvec{R}{}\f$�Ƃ���ƁC
  \f[
    \TUvec{R}{} =
    \TUbeginarray{ccc}
      \cos\theta_z & -\sin\theta_z & \\
      \sin\theta_z &  \cos\theta_z & \\
      & & 1
    \TUendarray
    \TUbeginarray{ccc}
       \cos\theta_y & & \sin\theta_y \\
       & 1 & \\
      -\sin\theta_y & & \cos\theta_y
    \TUendarray
    \TUbeginarray{ccc}
      1 & & \\
      & \cos\theta_x & -\sin\theta_x \\
      & \sin\theta_x &  \cos\theta_x
    \TUendarray
  \f]
  �Ȃ�\f$\theta_x, \theta_y, \theta_z\f$����]�p�ƂȂ�D
 \param theta_x	x������̉�]�p(\f$ -\pi \le \theta_x \le \pi\f$)��Ԃ��D
 \param theta_y	y������̉�]�p
	(\f$ -\frac{\pi}{2} \le \theta_y \le \frac{\pi}{2}\f$)��Ԃ��D
 \param theta_z	z������̉�]�p(\f$ -\pi \le \theta_z \le \pi\f$)��Ԃ��D
 \throw invalid_argument	3���������s��łȂ��ꍇ�ɑ��o
*/
template <class T, class B, class R> void
Matrix<T, B, R>::rot2angle(T& theta_x, T& theta_y, T& theta_z) const
{
    using namespace	std;
    
    if (nrow() != 3 || ncol() != 3)
	throw invalid_argument("TU::Matrix<T>::rot2angle: input matrix must be 3x3!!");

    if ((*this)[0][0] == 0.0 && (*this)[0][1] == 0.0)
    {
	theta_x = std::atan2(-(*this)[2][1], (*this)[1][1]);
	theta_y = ((*this)[0][2] < 0.0 ? M_PI / 2.0 : -M_PI / 2.0);
	theta_z = 0.0;
    }
    else
    {
	theta_x =  std::atan2((*this)[1][2], (*this)[2][2]);
	theta_y = -std::asin((*this)[0][2]);
	theta_z =  std::atan2((*this)[0][1], (*this)[0][0]);
    }
}

//! ����3������]�s�񂩂��]�p�Ɖ�]�������o���D
/*!
  ���̍s���\f$\TUtvec{R}{}\f$�Ƃ���ƁC
  \f[
    \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
    + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
    - \TUskew{n}{}\sin\theta
  \f]
  �Ȃ�\f$\theta~(0 \le \theta \le \pi)\f$��\f$\TUvec{n}{}\f$��
  ���ꂼ���]�p�Ɖ�]���ƂȂ�D
 \param c	��]�p��cos�l�C���Ȃ킿\f$\cos\theta\f$��Ԃ��D
 \param s	��]�p��sin�l�C���Ȃ킿\f$\sin\theta (\ge 0)\f$��Ԃ��D
 \return	��]����\��3�����P�ʃx�N�g���C���Ȃ킿\f$\TUvec{n}{}\f$
 \throw std::invalid_argument	3x3�s��łȂ��ꍇ�ɑ��o
*/
template <class T, class B, class R> Vector<T, FixedSizedBuf<T, 3> >
Matrix<T, B, R>::rot2axis(T& c, T& s) const
{
    if (nrow() != 3 || ncol() != 3)
	throw std::invalid_argument("TU::Matrix<T>::rot2axis: input matrix must be 3x3!!");

  // Compute cosine and sine of rotation angle.
    const T	trace = (*this)[0][0] + (*this)[1][1] + (*this)[2][2];
    c = T(0.5) * (trace - T(1));
    s = T(0.5) * std::sqrt((T(1) + trace)*(T(3) - trace));

  // Compute rotation axis.
    vector3_type	n;
    n[0] = (*this)[1][2] - (*this)[2][1];
    n[1] = (*this)[2][0] - (*this)[0][2];
    n[2] = (*this)[0][1] - (*this)[1][0];
    n.normalize();

    return n;
}

//! ����3������]�s�񂩂��]�p�Ɖ�]�������o���D
/*!
  ���̍s���\f$\TUtvec{R}{}\f$�Ƃ���ƁC
  \f[
    \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
    + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
    - \TUskew{n}{}\sin\theta
  \f]
  �Ȃ�\f$\theta~(0 \le \theta \le \pi)\f$��\f$\TUvec{n}{}\f$��
  ���ꂼ���]�p�Ɖ�]���ƂȂ�D
 \return			��]�p�Ɖ�]����\��3�����x�N�g���C���Ȃ킿
				\f$\theta\TUvec{n}{}\f$
 \throw invalid_argument	3x3�s��łȂ��ꍇ�ɑ��o
*/
template <class T, class B, class R> Vector<T, FixedSizedBuf<T, 3> >
Matrix<T, B, R>::rot2axis() const
{
    if (nrow() != 3 || ncol() != 3)
	throw std::invalid_argument("TU::Matrix<T>::rot2axis: input matrix must be 3x3!!");

    const T	trace = (*this)[0][0] + (*this)[1][1] + (*this)[2][2],
		s2 = std::sqrt((T(1) + trace)*(T(3) - trace));	// 2*sin
    if (s2 + T(1) == T(1))			// sin << 1 ?
	return vector3_type();			// zero vector
    
    vector3_type	axis;
    axis[0] = (*this)[1][2] - (*this)[2][1];
    axis[1] = (*this)[2][0] - (*this)[0][2];
    axis[2] = (*this)[0][1] - (*this)[1][0];

    return axis * (std::atan2(s2, trace - T(1)) / s2);
}

//! ����3������]�s�񂩂�l���������o���D
/*!
  ���̍s���\f$\TUtvec{R}{}\f$�Ƃ���ƁC
  \f[
    \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
    + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
    - \TUskew{n}{}\sin\theta
  \f]
  �Ȃ�\f$\theta~(0 \le \theta \le \pi)\f$��\f$\TUvec{n}{}\f$�ɑ΂��āC�l������
  \f[
    \TUvec{q}{} \equiv
    \TUbeginarray{c}
      \cos\frac{\theta}{2} \\ \TUvec{n}{}\sin\frac{\theta}{2}
    \TUendarray
  \f]
  �ƒ�`�����D
 \return			�l������\��4�����P�ʃx�N�g��
 \throw invalid_argument	3x3�s��łȂ��ꍇ�ɑ��o
*/
template <class T, class B, class R> Vector<T, FixedSizedBuf<T, 4u> >
Matrix<T, B, R>::rot2quaternion() const
{
    if (nrow() != 3 || ncol() != 3)
	throw std::invalid_argument("TU::Matrix<T>::rot2quaternion: input matrix must be 3x3!!");

    vector4_type	q;
    q[0] = T(0.5) * std::sqrt(trace() + T(1));
    if (q[0] + T(1) == T(1))	// q[0] << 1 ?
    {
	Vector<T>	eval;
	q(1, 3) = eigen(eval, false)[0];
    }
    else
    {
	const Matrix<T>&	S = trns() - *this;
	q[1] = T(0.25) * S[2][1] / q[0];
	q[2] = T(0.25) * S[0][2] / q[0];
	q[3] = T(0.25) * S[1][0] / q[0];
    }

    return q;
}

//! �P�ʐ����s��𐶐�����D
/*!
  \param d	�P�ʐ����s��̎���
  \return	�P�ʐ����s��
*/
template <class T, class B, class R> inline Matrix<T, B, R>
Matrix<T, B, R>::I(u_int d)
{
    return Matrix<T, B, R>(d, d).diag(1.0);
}

//! 3������]�s��𐶐�����D
/*!
  \param n	��]����\��3�����P�ʃx�N�g��
  \param c	��]�p��cos�l
  \param s	��]�p��sin�l
  \return	�������ꂽ��]�s��C���Ȃ킿
		\f[
		  \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
		  + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
		  - \TUskew{n}{}\sin\theta
		\f]
*/
template <class T, class B, class R> template <class T2, class B2>
Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3> >
Matrix<T, B, R>::Rt(const Vector<T2, B2>& n, T c, T s)
{
    if (n.dim() != 3)
	throw std::invalid_argument("TU::Matrix<T, B, R>::Rt: dimension of the argument \'n\' must be 3");
    matrix33_type	Qt = n % n;
    Qt *= (1.0 - c);
    Qt[0][0] += c;
    Qt[1][1] += c;
    Qt[2][2] += c;
    Qt[0][1] += n[2] * s;
    Qt[0][2] -= n[1] * s;
    Qt[1][0] -= n[2] * s;
    Qt[1][2] += n[0] * s;
    Qt[2][0] += n[1] * s;
    Qt[2][1] -= n[0] * s;

    return Qt;
}

//! 3������]�s��𐶐�����D
/*!
  \param v	��]�p�Ɖ�]����\��3�����x�N�g���܂��͎l������\��4�����P�ʃx�N�g��
  \return	�������ꂽ��]�s��C���Ȃ킿3�����x�N�g���̏ꍇ��
		\f[
		  \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
		  + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
		  - \TUskew{n}{}\sin\theta,
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \theta \equiv \TUnorm{\TUvec{v}{}},~
		  \TUvec{n}{} \equiv \frac{\TUvec{v}{}}{\TUnorm{\TUvec{v}{}}}
		\f]
		4�����P�ʃx�N�g���̏ꍇ��
		\f[
		  \TUtvec{R}{} \equiv
		  \TUvec{I}{3}(q_0^2 - \TUtvec{q}{}\TUvec{q}{})
		  + 2\TUvec{q}{}\TUtvec{q}{}
		  - 2q_0\TUskew{q}{},
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  q_0 \equiv v_0,~
		  \TUvec{q}{} \equiv [v_1,~v_2,~v_3]^\top
		\f]
*/
template <class T, class B, class R> template <class T2, class B2>
Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3> >
Matrix<T, B, R>::Rt(const Vector<T2, B2>& v)
{
    if (v.dim() == 4)			// quaternion ?
    {
	const T		q0 = v[0];
	vector3_type	q;
	q[0] = v[1];
	q[1] = v[2];
	q[2] = v[3];
	matrix33_type	Qt;
	Qt = (2.0 * q) % q;
	const T		c = q0*q0 - q.square();
	Qt[0][0] += c;
	Qt[1][1] += c;
	Qt[2][2] += c;
	q *= (2.0 * q0);
	Qt[0][1] += q[2];
	Qt[0][2] -= q[1];
	Qt[1][0] -= q[2];
	Qt[1][2] += q[0];
	Qt[2][0] += q[1];
	Qt[2][1] -= q[0];

	return Qt;
    }

    const T	theta = v.length();
    if (theta + T(1) == T(1))		// theta << 1 ?
	return I(3);
    else
    {
	T	c = std::cos(theta), s = std::sin(theta);
	return Rt(v / theta, c, s);
    }
}

//! �s��̃T�C�Y��ύX���C0�ɏ���������D
/*!
  \param r	�V�����s��
  \param c	�V������
*/
template <class T, class B, class R> inline void
Matrix<T, B, R>::resize(u_int r, u_int c)
{
    super::resize(r, c);
    *this = 0;
}

//! �s��̓����L���̈�ƃT�C�Y��ύX����D
/*!
  \param p	�V���������L���̈�ւ̃|�C���^
  \param r	�V�����s��
  \param c	�V������
*/
template <class T, class B, class R> inline void
Matrix<T, B, R>::resize(T* p, u_int r, u_int c)
{
    super::resize(p, r, c);
}

/************************************************************************
*  numerical operators							*
************************************************************************/
//! 2�̃x�N�g���̑����Z
/*!
  \param v	��1����
  \param w	��2����
  \return	���ʂ��i�[�����x�N�g���C���Ȃ킿\f$\TUvec{v}{}+\TUvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2> inline Vector<T1, B1>
operator +(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    return Vector<T1, B1>(v) += w;
}

//! 2�̃x�N�g���̈����Z
/*!
  \param v	��1����
  \param w	��2����
  \return	���ʂ��i�[�����x�N�g���C���Ȃ킿\f$\TUvec{v}{}-\TUvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2> inline Vector<T1, B1>
operator -(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    return Vector<T1, B1>(v) -= w;
}

//! �x�N�g���ɒ萔���|����D
/*!
  \param c	�|����萔
  \param v	�x�N�g��
  \return	���ʂ��i�[�����x�N�g���C���Ȃ킿\f$c\TUvec{v}{}\f$
*/
template <class T, class B> inline Vector<T, B>
operator *(double c, const Vector<T, B>& v)
{
    return Vector<T, B>(v) *= c;
}

//! �x�N�g���ɒ萔���|����D
/*!
  \param v	�x�N�g��
  \param c	�|����萔
  \return	���ʂ��i�[�����x�N�g���C���Ȃ킿\f$c\TUvec{v}{}\f$
*/
template <class T, class B> inline Vector<T, B>
operator *(const Vector<T, B>& v, double c)
{
    return Vector<T, B>(v) *= c;
}

//! �x�N�g���̊e������萔�Ŋ���D
/*!
  \param v	�x�N�g��
  \param c	����萔
  \return	���ʂ��i�[�����x�N�g���C���Ȃ킿\f$\frac{1}{c}\TUvec{v}{}\f$
*/
template <class T, class B> inline Vector<T, B>
operator /(const Vector<T, B>& v, double c)
{
    return Vector<T, B>(v) /= c;
}

//! 2�̍s��̑����Z
/*!
  \param m	��1����
  \param n	��2����
  \return	���ʂ��i�[�����s��C���Ȃ킿\f$\TUvec{M}{}+\TUvec{N}{}\f$
*/
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline Matrix<T1, B1, R1>
operator +(const Matrix<T1, B1, R1>& m, const Matrix<T2, B2, R2>& n)
{
    return Matrix<T1, B1, R1>(m) += n;
}

//! 2�̍s��̈����Z
/*!
  \param m	��1����
  \param n	��2����
  \return	���ʂ��i�[�����s��C���Ȃ킿\f$\TUvec{M}{}-\TUvec{N}{}\f$
*/
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline Matrix<T1, B1, R1>
operator -(const Matrix<T1, B1, R1>& m, const Matrix<T2, B2, R2>& n)
{
    return Matrix<T1, B1, R1>(m) -= n;
}

//! �s��ɒ萔���|����D
/*!
  \param c	�|����萔
  \param m	�s��
  \return	���ʂ��i�[�����s��C���Ȃ킿\f$c\TUvec{M}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
operator *(double c, const Matrix<T, B, R>& m)
{
    return Matrix<T, B, R>(m) *= c;
}

//! �s��ɒ萔���|����D
/*!
  \param m	�s��
  \param c	�|����萔
  \return	���ʂ��i�[�����s��C���Ȃ킿\f$c\TUvec{M}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
operator *(const Matrix<T, B, R>& m, double c)
{
    return Matrix<T, B, R>(m) *= c;
}

//! �s��̊e������萔�Ŋ���D
/*!
  \param m	�s��
  \param c	����萔
  \return	���ʂ��i�[�����s��C���Ȃ킿\f$\frac{1}{c}\TUvec{M}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
operator /(const Matrix<T, B, R>& m, double c)
{
    return Matrix<T, B, R>(m) /= c;
}

//! 2��3�����x�N�g���̃x�N�g����
/*!
  \param v	��1����
  \param w	��2����
  \return	�x�N�g���ρC���Ȃ킿\f$\TUvec{v}{}\times\TUvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2>
inline Vector<T1, FixedSizedBuf<T1, 3> >
operator ^(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    return Vector<T1, FixedSizedBuf<T1, 3> >(v) ^= w;
}

//! 2�̃x�N�g���̓���
/*!
  \param v	��1����
  \param w	��2����
  \return	���ρC���Ȃ킿\f$\TUtvec{v}{}\TUvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2> T1
operator *(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    v.check_dim(w.dim());
    T1	val = 0;
    for (u_int i = 0; i < v.dim(); ++i)
	val += v[i] * w[i];
    return val;
}

//! �x�N�g���ƍs��̐�
/*!
  \param v	�x�N�g��
  \param m	�s��
  \return	���ʂ̃x�N�g���C���Ȃ킿\f$\TUtvec{v}{}\TUvec{M}{}\f$
*/
template <class T1, class B1, class T2, class B2, class R2> Vector<T1>
operator *(const Vector<T1, B1>& v, const Matrix<T2, B2, R2>& m)
{
    v.check_dim(m.nrow());
    Vector<T1> val(m.ncol());
    for (u_int j = 0; j < m.ncol(); ++j)
	for (u_int i = 0; i < m.nrow(); ++i)
	    val[j] += v[i] * m[i][j];
    return val;
}

//! 2�̃x�N�g���̊O��
/*!
  \param v	��1����
  \param w	��2����
  \return	���ʂ̍s��C���Ȃ킿\f$\TUvec{v}{}\TUtvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2> Matrix<T1>
operator %(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    Matrix<T1>	val(v.dim(), w.dim());
    for (u_int i = 0; i < v.dim(); ++i)
	for (u_int j = 0; j < w.dim(); ++j)
	    val[i][j] = v[i] * w[j];
    return val;
}

//! 3�����x�N�g����3x?�s��̊e��Ƃ̃x�N�g����
/*!
  \param v			3�����x�N�g��
  \param m			3x?�s��
  \return			���ʂ�3x?�s��C���Ȃ킿
				\f$\TUvec{v}{}\times\TUvec{M}{}\f$
  \throw std::invalid_argument	v��3�����x�N�g���łȂ���m��3x?�s��łȂ��ꍇ��
				���o
*/
template <class T1, class B1, class T2, class B2, class R2> Matrix<T2, B2, R2>
operator ^(const Vector<T1, B1>& v, const Matrix<T2, B2, R2>& m)
{
    v.check_dim(m.nrow());
    if (v.dim() != 3)
	throw std::invalid_argument("operator ^(const Vecotr<T>&, const Matrix<T, B, R>&): dimension of vector must be 3!!");
    Matrix<T2, B2, R2>	val(m.nrow(), m.ncol());
    for (u_int j = 0; j < val.ncol(); ++j)
    {
	val[0][j] = v[1] * m[2][j] - v[2] * m[1][j];
	val[1][j] = v[2] * m[0][j] - v[0] * m[2][j];
	val[2][j] = v[0] * m[1][j] - v[1] * m[0][j];
    }
    return val;
}

//! 2�̍s��̐�
/*!
  \param m	��1����
  \param n	��2����
  \return	���ʂ̍s��C���Ȃ킿\f$\TUvec{M}{}\TUvec{N}{}\f$
*/
template <class T1, class B1, class R1, class T2, class B2, class R2>
Matrix<T1>
operator *(const Matrix<T1, B1, R1>& m, const Matrix<T2, B2, R2>& n)
{
    n.check_dim(m.ncol());
    Matrix<T1>	val(m.nrow(), n.ncol());
    for (u_int i = 0; i < m.nrow(); ++i)
	for (u_int j = 0; j < n.ncol(); ++j)
	    for (u_int k = 0; k < m.ncol(); ++k)
		val[i][j] += m[i][k] * n[k][j];
    return val;
}

//! �s��ƃx�N�g���̐�
/*!
  \param m	�s��
  \param v	�x�N�g��
  \return	���ʂ̃x�N�g���C���Ȃ킿\f$\TUvec{M}{}\TUvec{v}{}\f$
*/
template <class T1, class B1, class R1, class T2, class B2> Vector<T1>
operator *(const Matrix<T1, B1, R1>& m, const Vector<T2, B2>& v)
{
    Vector<T1>	val(m.nrow());
    for (u_int i = 0; i < m.nrow(); ++i)
	val[i] = m[i] * v;
    return val;
}

//! ?x3�s��̊e�s��3�����x�N�g���̃x�N�g����
/*!
  \param m	?x3�s��
  \param v	3�����x�N�g��
  \return	���ʂ̍s��C���Ȃ킿\f$(\TUtvec{M}{}\times\TUvec{v}{})^\top\f$
*/
template <class T1, class B1, class R1, class T2, class B2>
inline Matrix<T1, B1, R1>
operator ^(const Matrix<T1, B1, R1>& m, const Vector<T2, B2>& v)
{
    return Matrix<T1, B1, R1>(m) ^= v;
}

/************************************************************************
*  class LUDecomposition<T>						*
************************************************************************/
//! �����s���LU������\���N���X
template <class T>
class LUDecomposition : private Array2<Vector<T> >
{
  private:
    typedef T						value_type;
    typedef Array2<Vector<T> >				super;
    
  public:
    template <class T2, class B2, class R2>
    LUDecomposition(const Matrix<T2, B2, R2>&)		;

    template <class T2, class B2>
    void	substitute(Vector<T2, B2>&)	const	;

  //! ���Ƃ̐����s��̍s�񎮂�Ԃ��D
  /*!
    \return	���Ƃ̐����s��̍s��
  */
    T		det()				const	{return _det;}
    
  private:
    using	super::nrow;
    using	super::ncol;
    
    Array<int>	_index;
    T		_det;
};

//! �^����ꂽ�����s���LU�����𐶐�����D
/*!
 \param m			LU�������鐳���s��
 \throw std::invalid_argument	m�������s��łȂ��ꍇ�ɑ��o
*/
template <class T> template <class T2, class B2, class R2>
LUDecomposition<T>::LUDecomposition(const Matrix<T2, B2, R2>& m)
    :super(m), _index(ncol()), _det(1.0)
{
    using namespace	std;
    
    if (nrow() != ncol())
        throw invalid_argument("TU::LUDecomposition<T>::LUDecomposition: not square matrix!!");

    for (u_int j = 0; j < ncol(); ++j)	// initialize column index
	_index[j] = j;			// for explicit pivotting

    Vector<T>	scale(ncol());
    for (u_int j = 0; j < ncol(); ++j)	// find maximum abs. value in each col.
    {					// for implicit pivotting
	T max = 0.0;

	for (u_int i = 0; i < nrow(); ++i)
	{
	    const T tmp = std::fabs((*this)[i][j]);
	    if (tmp > max)
		max = tmp;
	}
	scale[j] = (max != 0.0 ? 1.0 / max : 1.0);
    }

    for (u_int i = 0; i < nrow(); ++i)
    {
	for (u_int j = 0; j < i; ++j)		// left part (j < i)
	{
	    T& sum = (*this)[i][j];
	    for (u_int k = 0; k < j; ++k)
		sum -= (*this)[i][k] * (*this)[k][j];
	}

	u_int	jmax = i;
	T	max = 0.0;
	for (u_int j = i; j < ncol(); ++j)  // diagonal and right part (i <= j)
	{
	    T& sum = (*this)[i][j];
	    for (u_int k = 0; k < i; ++k)
		sum -= (*this)[i][k] * (*this)[k][j];
	    const T tmp = std::fabs(sum) * scale[j];
	    if (tmp >= max)
	    {
		max  = tmp;
		jmax = j;
	    }
	}
	if (jmax != i)			// pivotting required ?
	{
	    for (u_int k = 0; k < nrow(); ++k)	// swap i-th and jmax-th column
		swap((*this)[k][i], (*this)[k][jmax]);
	    swap(_index[i], _index[jmax]);	// swap column index
	    swap(scale[i], scale[jmax]);	// swap colum-wise scale factor
	    _det = -_det;
	}

	_det *= (*this)[i][i];

	if ((*this)[i][i] == 0.0)	// singular matrix ?
	    break;

	for (u_int j = i + 1; j < nrow(); ++j)
	    (*this)[i][j] /= (*this)[i][i];
    }
}

//! ���Ƃ̐����s����W���s��Ƃ����A��1���������������D
/*!
  \param b			���Ƃ̐����s��\f$\TUvec{M}{}\f$�Ɠ�����
				�������x�N�g���D\f$\TUtvec{b}{} =
				\TUtvec{x}{}\TUvec{M}{}\f$�̉��ɕϊ���
				���D
  \throw std::invalid_argument	�x�N�g��b�̎��������Ƃ̐����s��̎����Ɉ�v
				���Ȃ��ꍇ�ɑ��o
  \throw std::runtime_error	���Ƃ̐����s�񂪐����łȂ��ꍇ�ɑ��o
*/
template <class T> template <class T2, class B2> void
LUDecomposition<T>::substitute(Vector<T2, B2>& b) const
{
    if (b.dim() != ncol())
	throw std::invalid_argument("TU::LUDecomposition<T>::substitute: Dimension of given vector is not equal to mine!!");
    
    Vector<T2, B2>	tmp(b);
    for (u_int j = 0; j < b.dim(); ++j)
	b[j] = tmp[_index[j]];

    for (u_int j = 0; j < b.dim(); ++j)		// forward substitution
	for (u_int i = 0; i < j; ++i)
	    b[j] -= b[i] * (*this)[i][j];
    for (u_int j = b.dim(); j-- > 0; )		// backward substitution
    {
	for (u_int i = b.dim(); --i > j; )
	    b[j] -= b[i] * (*this)[i][j];
	if ((*this)[j][j] == 0.0)		// singular matrix ?
	    throw std::runtime_error("TU::LUDecomposition<T>::substitute: singular matrix !!");
	b[j] /= (*this)[j][j];
    }
}

//! �A��1���������������D
/*!
  \param m	�����Ȑ����s��
  \return	\f$\TUtvec{u}{} = \TUtvec{x}{}\TUvec{M}{}\f$
		�̉���[�߂����̃x�N�g���C���Ȃ킿
		\f$\TUtvec{u}{} \leftarrow \TUtvec{u}{}\TUinv{M}{}\f$
*/
template <class T, class B> template <class T2, class B2, class R2>
inline Vector<T, B>&
Vector<T, B>::solve(const Matrix<T2, B2, R2>& m)
{
    LUDecomposition<T2>(m).substitute(*this);
    return *this;
}

//! �A��1���������������D
/*!
  \param m	�����Ȑ����s��
  \return	\f$\TUvec{A}{} = \TUvec{X}{}\TUvec{M}{}\f$
		�̉���[�߂����̍s��C���Ȃ킿
		\f$\TUvec{A}{} \leftarrow \TUvec{A}{}\TUinv{M}{}\f$
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
Matrix<T, B, R>&
Matrix<T, B, R>::solve(const Matrix<T2, B2, R2>& m)
{
    LUDecomposition<T2>	lu(m);
    
    for (u_int i = 0; i < nrow(); ++i)
	lu.substitute((*this)[i]);
    return *this;
}

//! ���̍s��̍s�񎮂�Ԃ��D
/*!
  \return	�s�񎮁C���Ȃ킿\f$\det\TUvec{A}{}\f$
*/
template <class T, class B, class R> inline T
Matrix<T, B, R>::det() const
{
    return LUDecomposition<T>(*this).det();
}

/************************************************************************
*  class Householder<T>							*
************************************************************************/
template <class T>	class QRDecomposition;
template <class T>	class BiDiagonal;

//! Householder�ϊ���\���N���X
template <class T>
class Householder : public Matrix<T>
{
  public:
    typedef T							value_type;
    
  private:
    typedef Matrix<T>						super;
    
  private:
    Householder(u_int dd, u_int d)
	:super(dd, dd), _d(d), _sigma(Matrix<T>::nrow())	{}
    template <class T2, class B2, class R2>
    Householder(const Matrix<T2, B2, R2>& a, u_int d)		;

    using		super::dim;
    
    void		apply_from_left(Matrix<T>& a, u_int m)	;
    void		apply_from_right(Matrix<T>& a, u_int m)	;
    void		apply_from_both(Matrix<T>& a, u_int m)	;
    void		make_transformation()			;
    const Vector<T>&	sigma()				const	{return _sigma;}
    Vector<T>&		sigma()					{return _sigma;}
    bool		sigma_is_zero(u_int m, T comp)	const	;

  private:
    const u_int		_d;		// deviation from diagonal element
    Vector<T>		_sigma;

    friend class	QRDecomposition<T>;
    friend class	TriDiagonal<T>;
    friend class	BiDiagonal<T>;
};

template <class T> template <class T2, class B2, class R2>
Householder<T>::Householder(const Matrix<T2, B2, R2>& a, u_int d)
    :super(a), _d(d), _sigma(dim())
{
    if (a.nrow() != a.ncol())
	throw std::invalid_argument("TU::Householder<T>::Householder: Given matrix must be square !!");
}

template <class T> void
Householder<T>::apply_from_left(Matrix<T>& a, u_int m)
{
    if (a.nrow() < dim())
	throw std::invalid_argument("TU::Householder<T>::apply_from_left: # of rows of given matrix is smaller than my dimension !!");
    
    T	scale = 0.0;
    for (u_int i = m+_d; i < dim(); ++i)
	scale += std::fabs(a[i][m]);
	
    if (scale != 0.0)
    {
	T	h = 0.0;
	for (u_int i = m+_d; i < dim(); ++i)
	{
	    a[i][m] /= scale;
	    h += a[i][m] * a[i][m];
	}

	const T	s = (a[m+_d][m] > 0.0 ? std::sqrt(h) : -std::sqrt(h));
	h	     += s * a[m+_d][m];			// H = u^2 / 2
	a[m+_d][m]   += s;				// m-th col <== u
	    
	for (u_int j = m+1; j < a.ncol(); ++j)
	{
	    T	p = 0.0;
	    for (u_int i = m+_d; i < dim(); ++i)
		p += a[i][m] * a[i][j];
	    p /= h;					// p[j] (p' = u'A / H)
	    for (u_int i = m+_d; i < dim(); ++i)
		a[i][j] -= a[i][m] * p;			// A = A - u*p'
	    a[m+_d][j] = -a[m+_d][j];
	}
	    
	for (u_int i = m+_d; i < dim(); ++i)
	    (*this)[m][i] = scale * a[i][m];		// copy u
	_sigma[m+_d] = scale * s;
    }
}

template <class T> void
Householder<T>::apply_from_right(Matrix<T>& a, u_int m)
{
    if (a.ncol() < dim())
	throw std::invalid_argument("Householder<T>::apply_from_right: # of column of given matrix is smaller than my dimension !!");
    
    T	scale = 0.0;
    for (u_int j = m+_d; j < dim(); ++j)
	scale += std::fabs(a[m][j]);
	
    if (scale != 0.0)
    {
	T	h = 0.0;
	for (u_int j = m+_d; j < dim(); ++j)
	{
	    a[m][j] /= scale;
	    h += a[m][j] * a[m][j];
	}

	const T	s = (a[m][m+_d] > 0.0 ? std::sqrt(h) : -std::sqrt(h));
	h	     += s * a[m][m+_d];			// H = u^2 / 2
	a[m][m+_d]   += s;				// m-th row <== u

	for (u_int i = m+1; i < a.nrow(); ++i)
	{
	    T	p = 0.0;
	    for (u_int j = m+_d; j < dim(); ++j)
		p += a[i][j] * a[m][j];
	    p /= h;					// p[i] (p = Au / H)
	    for (u_int j = m+_d; j < dim(); ++j)
		a[i][j] -= p * a[m][j];			// A = A - p*u'
	    a[i][m+_d] = -a[i][m+_d];
	}
	    
	for (u_int j = m+_d; j < dim(); ++j)
	    (*this)[m][j] = scale * a[m][j];		// copy u
	_sigma[m+_d] = scale * s;
    }
}

template <class T> void
Householder<T>::apply_from_both(Matrix<T>& a, u_int m)
{
    Vector<T>	u = a[m](m+_d, a.ncol()-m-_d);
    T		scale = 0.0;
    for (u_int j = 0; j < u.dim(); ++j)
	scale += std::fabs(u[j]);
	
    if (scale != 0.0)
    {
	u /= scale;

	T		h = u * u;
	const T	s = (u[0] > 0.0 ? std::sqrt(h) : -std::sqrt(h));
	h	     += s * u[0];			// H = u^2 / 2
	u[0]	     += s;				// m-th row <== u

	Matrix<T>	A = a(m+_d, m+_d, a.nrow()-m-_d, a.ncol()-m-_d);
	Vector<T>	p = _sigma(m+_d, dim()-m-_d);
	for (u_int i = 0; i < A.nrow(); ++i)
	    p[i] = (A[i] * u) / h;			// p = Au / H

	const T	k = (u * p) / (h + h);		// K = u*p / 2H
	for (u_int i = 0; i < A.nrow(); ++i)
	{				// m-th col of 'a' is used as 'q'
	    a[m+_d+i][m] = p[i] - k * u[i];		// q = p - Ku
	    for (u_int j = 0; j <= i; ++j)		// A = A - uq' - qu'
		A[j][i] = (A[i][j] -= (u[i]*a[m+_d+j][m] + a[m+_d+i][m]*u[j]));
	}
	for (u_int j = 1; j < A.nrow(); ++j)
	    A[j][0] = A[0][j] = -A[0][j];

	for (u_int j = m+_d; j < a.ncol(); ++j)
	    (*this)[m][j] = scale * a[m][j];		// copy u
	_sigma[m+_d] = scale * s;
    }
}

template <class T> void
Householder<T>::make_transformation()
{
    for (u_int m = dim(); m-- > 0; )
    {
	for (u_int i = m+1; i < dim(); ++i)
	    (*this)[i][m] = 0.0;

	if (_sigma[m] != 0.0)
	{
	    for (u_int i = m+1; i < dim(); ++i)
	    {
		T	g = 0.0;
		for (u_int j = m+1; j < dim(); ++j)
		    g += (*this)[i][j] * (*this)[m-_d][j];
		g /= (_sigma[m] * (*this)[m-_d][m]);	// g[i] (g = Uu / H)
		for (u_int j = m; j < dim(); ++j)
		    (*this)[i][j] -= g * (*this)[m-_d][j];	// U = U - gu'
	    }
	    for (u_int j = m; j < dim(); ++j)
		(*this)[m][j] = (*this)[m-_d][j] / _sigma[m];
	    (*this)[m][m] -= 1.0;
	}
	else
	{
	    for (u_int j = m+1; j < dim(); ++j)
		(*this)[m][j] = 0.0;
	    (*this)[m][m] = 1.0;
	}
    }
}

template <class T> bool
Householder<T>::sigma_is_zero(u_int m, T comp) const
{
    return (T(std::fabs(_sigma[m])) + comp == comp);
}

/************************************************************************
*  class QRDecomposition<T>						*
************************************************************************/
//! ��ʍs���QR������\���N���X
/*!
  �^����ꂽ�s��\f$\TUvec{A}{} \in \TUspace{R}{m\times n}\f$�ɑ΂���
  \f$\TUvec{A}{} = \TUtvec{R}{}\TUtvec{Q}{}\f$�Ȃ鉺���O�p�s��
  \f$\TUtvec{R}{} \in \TUspace{R}{m\times n}\f$�Ɖ�]�s��
  \f$\TUtvec{Q}{} \in \TUspace{R}{n\times n}\f$�����߂�
  �i\f$\TUvec{A}{}\f$�̊e�s��\f$\TUtvec{Q}{}\f$�̍s�̐��^�����ŕ\����
  ��j�D
 */
template <class T>
class QRDecomposition : private Matrix<T>
{
  public:
    typedef T						value_type;
    
  private:
    typedef Matrix<T>					super;
    
  public:
    template <class T2, class B2, class R2>
    QRDecomposition(const Matrix<T2, B2, R2>& m)	;

  //! QR�����̉����O�p�s���Ԃ��D
  /*!
    \return	�����O�p�s��\f$\TUtvec{R}{}\f$
  */
    const Matrix<T>&	Rt()			const	{return *this;}

  //! QR�����̉�]�s���Ԃ��D
  /*!
    \return	��]�s��\f$\TUtvec{Q}{}\f$
  */
    const Matrix<T>&	Qt()			const	{return _Qt;}
    
  private:
    using		super::nrow;
    using		super::ncol;
    
    Householder<T>	_Qt;			// rotation matrix
};

//! �^����ꂽ��ʍs���QR�����𐶐�����D
/*!
 \param m	QR���������ʍs��
*/
template <class T> template <class T2, class B2, class R2>
QRDecomposition<T>::QRDecomposition(const Matrix<T2, B2, R2>& m)
    :super(m), _Qt(m.ncol(), 0)
{
    u_int	n = std::min(nrow(), ncol());
    for (u_int j = 0; j < n; ++j)
	_Qt.apply_from_right(*this, j);
    _Qt.make_transformation();
    for (u_int i = 0; i < n; ++i)
    {
	(*this)[i][i] = _Qt.sigma()[i];
	for (u_int j = i + 1; j < ncol(); ++j)
	    (*this)[i][j] = 0.0;
    }
}

/************************************************************************
*  class TriDiagonal<T>							*
************************************************************************/
//! �Ώ̍s���3�d�Ίp����\���N���X
/*!
  �^����ꂽ�Ώ̍s��\f$\TUvec{A}{} \in \TUspace{R}{d\times d}\f$�ɑ΂�
  ��\f$\TUtvec{U}{}\TUvec{A}{}\TUvec{U}{}\f$��3�d�Ίp�s��ƂȂ�悤�ȉ�
  �]�s��\f$\TUtvec{U}{} \in \TUspace{R}{d\times d}\f$�����߂�D
 */
template <class T>
class TriDiagonal
{
  public:
    typedef T					value_type;	//!< �����̌^
    
  public:
    template <class T2, class B2, class R2>
    TriDiagonal(const Matrix<T2, B2, R2>& a)		;

  //! 3�d�Ίp�������Ώ̍s��̎���(= �s�� = ��)��Ԃ��D
  /*!
    \return	�Ώ̍s��̎���
  */
    u_int		dim()			const	{return _Ut.nrow();}

  //! 3�d�Ίp�����s����]�s���Ԃ��D
  /*!
    \return	��]�s��
  */
    const Matrix<T>&	Ut()			const	{return _Ut;}

  //! 3�d�Ίp�s��̑Ίp������Ԃ��D
  /*!
    \return	�Ίp����
  */
    const Vector<T>&	diagonal()		const	{return _diagonal;}

  //! 3�d�Ίp�s��̔�Ίp������Ԃ��D
  /*!
    \return	��Ίp����
  */
    const Vector<T>&	off_diagonal()		const	{return _Ut.sigma();}

    void		diagonalize(bool abs=true)	;
    
  private:
    enum		{NITER_MAX = 30};

    bool		off_diagonal_is_zero(u_int n)		const	;
    void		initialize_rotation(u_int m, u_int n,
					    T& x, T& y)		const	;
    
    Householder<T>	_Ut;
    Vector<T>		_diagonal;
    Vector<T>&		_off_diagonal;
};

//! �^����ꂽ�Ώ̍s���3�d�Ίp������D
/*!
  \param a			3�d�Ίp������Ώ̍s��
  \throw std::invalid_argument	a�������s��łȂ��ꍇ�ɑ��o
*/
template <class T> template <class T2, class B2, class R2>
TriDiagonal<T>::TriDiagonal(const Matrix<T2, B2, R2>& a)
    :_Ut(a, 1), _diagonal(_Ut.nrow()), _off_diagonal(_Ut.sigma())
{
    if (_Ut.nrow() != _Ut.ncol())
        throw std::invalid_argument("TU::TriDiagonal<T>::TriDiagonal: not square matrix!!");

    for (u_int m = 0; m < dim(); ++m)
    {
	_Ut.apply_from_both(_Ut, m);
	_diagonal[m] = _Ut[m][m];
    }

    _Ut.make_transformation();
}

//! 3�d�Ίp�s���Ίp������i�ŗL�l�C�ŗL�x�N�g���̌v�Z�j�D
/*!
  �Ίp�����͌ŗL�l�ƂȂ�C\f$\TUtvec{U}{}\f$�̊e�s�͌ŗL�x�N�g����^����D
  \throw std::runtime_error	�w�肵���J��Ԃ��񐔂��z�����ꍇ�ɑ��o
  \param abs	�ŗL�l�����̐�Βl�̑傫�����ɕ��ׂ�̂ł����true,
		���̒l�̑傫�����ɕ��ׂ�̂ł����false
*/ 
template <class T> void
TriDiagonal<T>::diagonalize(bool abs)
{
    using namespace	std;
    
    for (u_int n = dim(); n-- > 0; )
    {
	int	niter = 0;
	
#ifdef TUVectorPP_DEBUG
	cerr << "******** n = " << n << " ********" << endl;
#endif
	while (!off_diagonal_is_zero(n))
	{					// n > 0 here
	    if (niter++ > NITER_MAX)
		throw runtime_error("TU::TriDiagonal::diagonalize(): Number of iteration exceeded maximum value!!");

	  /* Find first m (< n) whose off-diagonal element is 0 */
	    u_int	m = n;
	    while (!off_diagonal_is_zero(--m))	// 0 <= m < n < dim() here
	    {
	    }

	  /* Set x and y which determine initial(i = m+1) plane rotation */
	    T	x, y;
	    initialize_rotation(m, n, x, y);
	  /* Apply rotation P(i-1, i) for each i (i = m+1, n+2, ... , n) */
	    for (u_int i = m; ++i <= n; )
	    {
		Rotation<T>	rot(i-1, i, x, y);
		
		_Ut.rotate_from_left(rot);

		if (i > m+1)
		    _off_diagonal[i-1] = rot.length();
		const T w = _diagonal[i] - _diagonal[i-1];
		const T d = rot.sin()*(rot.sin()*w
			       + 2.0*rot.cos()*_off_diagonal[i]);
		_diagonal[i-1]	 += d;
		_diagonal[i]	 -= d;
		_off_diagonal[i] += rot.sin()*(rot.cos()*w
				  - 2.0*rot.sin()*_off_diagonal[i]);
		if (i < n)
		{
		    x = _off_diagonal[i];
		    y = rot.sin()*_off_diagonal[i+1];
		    _off_diagonal[i+1] *= rot.cos();
		}
	    }
#ifdef TUVectorPP_DEBUG
	    cerr << "  niter = " << niter << ": " << off_diagonal();
#endif	    
	}
    }

  // Sort eigen values and eigen vectors.
    if (abs)
    {
	for (u_int m = 0; m < dim(); ++m)
	    for (u_int n = m+1; n < dim(); ++n)
		if (std::fabs(_diagonal[n]) >
		    std::fabs(_diagonal[m]))			// abs. values
		{
		    swap(_diagonal[m], _diagonal[n]);
		    for (u_int j = 0; j < dim(); ++j)
		    {
			const T	tmp = _Ut[m][j];
			_Ut[m][j] = _Ut[n][j];
			_Ut[n][j] = -tmp;
		    }
		}
    }
    else
    {
	for (u_int m = 0; m < dim(); ++m)
	    for (u_int n = m+1; n < dim(); ++n)
		if (_diagonal[n] > _diagonal[m])		// raw values
		{
		    swap(_diagonal[m], _diagonal[n]);
		    for (u_int j = 0; j < dim(); ++j)
		    {
			const T	tmp = _Ut[m][j];
			_Ut[m][j] = _Ut[n][j];
			_Ut[n][j] = -tmp;
		    }
		}
    }
}

template <class T> bool
TriDiagonal<T>::off_diagonal_is_zero(u_int n) const
{
    return (n == 0 || _Ut.sigma_is_zero(n, std::fabs(_diagonal[n-1]) +
					   std::fabs(_diagonal[n])));
}

template <class T> void
TriDiagonal<T>::initialize_rotation(u_int m, u_int n, T& x, T& y) const
{
    const T	g    = (_diagonal[n] - _diagonal[n-1]) / (2.0*_off_diagonal[n]),
		absg = std::fabs(g),
		gg1  = (absg > 1.0 ?
			absg * std::sqrt(1.0 + (1.0/absg)*(1.0/absg)) :
			std::sqrt(1.0 + absg*absg)),
		t    = (g > 0.0 ? g + gg1 : g - gg1);
    x = _diagonal[m] - _diagonal[n] - _off_diagonal[n]/t;
  //x = _diagonal[m];					// without shifting
    y = _off_diagonal[m+1];
}

/************************************************************************
*  class BiDiagonal<T>							*
************************************************************************/
//! ��ʍs���2�d�Ίp����\���N���X
/*!
  �^����ꂽ��ʍs��\f$\TUvec{A}{} \in \TUspace{R}{m\times n}\f$�ɑ΂�
  ��\f$\TUtvec{V}{}\TUvec{A}{}\TUvec{U}{}\f$��2�d�Ίp�s��ƂȂ�悤��2
  �̉�]�s��\f$\TUtvec{U}{} \in \TUspace{R}{n\times n}\f$,
  \f$\TUtvec{V}{} \in \TUspace{R}{m\times m}\f$�����߂�D\f$m \le n\f$
  �̏ꍇ�͉����O�p��2�d�Ίp�s��ɁC\f$m > n\f$�̏ꍇ�͏㔼�O�p��2�d�Ίp
  �s��ɂȂ�D
 */
template <class T>
class BiDiagonal
{
  public:
    typedef T					value_type;	//!< �����̌^
    
  public:
    template <class T2, class B2, class R2>
    BiDiagonal(const Matrix<T2, B2, R2>& a)	;

  //! 2�d�Ίp�������s��̍s����Ԃ��D
  /*!
    \return	�s��̍s��
  */
    u_int		nrow()		const	{return _Vt.nrow();}

  //! 2�d�Ίp�������s��̗񐔂�Ԃ��D
  /*!
    \return	�s��̗�
  */
    u_int		ncol()		const	{return _Ut.nrow();}

  //! 2�d�Ίp�����s�����߂ɉE����|�����]�s��̓]�u��Ԃ��D
  /*!
    \return	�E����|�����]�s��̓]�u
  */
    const Matrix<T>&	Ut()		const	{return _Ut;}

  //! 2�d�Ίp�����s�����߂ɍ�����|�����]�s���Ԃ��D
  /*!
    \return	������|�����]�s��
  */
    const Matrix<T>&	Vt()		const	{return _Vt;}

  //! 2�d�Ίp�s��̑Ίp������Ԃ��D
  /*!
    \return	�Ίp����
  */
    const Vector<T>&	diagonal()	const	{return _Dt.sigma();}

  //! 2�d�Ίp�s��̔�Ίp������Ԃ��D
  /*!
    \return	��Ίp����
  */
    const Vector<T>&	off_diagonal()	const	{return _Et.sigma();}

    void		diagonalize()		;

  private:
    enum		{NITER_MAX = 30};
    
    bool		diagonal_is_zero(u_int n)		const	;
    bool		off_diagonal_is_zero(u_int n)		const	;
    void		initialize_rotation(u_int m, u_int n,
					    T& x, T& y)		const	;

    Householder<T>	_Dt;
    Householder<T>	_Et;
    Vector<T>&		_diagonal;
    Vector<T>&		_off_diagonal;
    T			_anorm;
    const Matrix<T>&	_Ut;
    const Matrix<T>&	_Vt;
};

//! �^����ꂽ��ʍs���2�d�Ίp������D
/*!
  \param a	2�d�Ίp�������ʍs��
*/
template <class T> template <class T2, class B2, class R2>
BiDiagonal<T>::BiDiagonal(const Matrix<T2, B2, R2>& a)
    :_Dt((a.nrow() < a.ncol() ? a.ncol() : a.nrow()), 0),
     _Et((a.nrow() < a.ncol() ? a.nrow() : a.ncol()), 1),
     _diagonal(_Dt.sigma()), _off_diagonal(_Et.sigma()), _anorm(0),
     _Ut(a.nrow() < a.ncol() ? _Dt : _Et),
     _Vt(a.nrow() < a.ncol() ? _Et : _Dt)
{
    if (nrow() < ncol())
	for (u_int i = 0; i < nrow(); ++i)
	    for (u_int j = 0; j < ncol(); ++j)
		_Dt[i][j] = a[i][j];
    else
	for (u_int i = 0; i < nrow(); ++i)
	    for (u_int j = 0; j < ncol(); ++j)
		_Dt[j][i] = a[i][j];

  /* Householder reduction to bi-diagonal (off-diagonal in lower part) form */
    for (u_int m = 0; m < _Et.dim(); ++m)
    {
	_Dt.apply_from_right(_Dt, m);
	_Et.apply_from_left(_Dt, m);
    }

    _Dt.make_transformation();	// Accumulate right-hand transformation: V
    _Et.make_transformation();	// Accumulate left-hand transformation: U

    for (u_int m = 0; m < _Et.dim(); ++m)
    {
	T	anorm = std::fabs(_diagonal[m]) + std::fabs(_off_diagonal[m]);
	if (anorm > _anorm)
	    _anorm = anorm;
    }
}

//! 2�d�Ίp�s���Ίp������i���ْl�����j�D
/*!
  �Ίp�����͓��ْl�ƂȂ�C\f$\TUtvec{U}{}\f$��\f$\TUtvec{V}{}\f$
  �̊e�s�͂��ꂼ��E���كx�N�g���ƍ����كx�N�g����^����D
  \throw std::runtime_error	�w�肵���J��Ԃ��񐔂��z�����ꍇ�ɑ��o
*/ 
template <class T> void
BiDiagonal<T>::diagonalize()
{
    using namespace	std;
    
    for (u_int n = _Et.dim(); n-- > 0; )
    {
	u_int	niter = 0;
	
#ifdef TUVectorPP_DEBUG
	cerr << "******** n = " << n << " ********" << endl;
#endif
	while (!off_diagonal_is_zero(n))	// n > 0 here
	{
	    if (niter++ > NITER_MAX)
		throw runtime_error("TU::BiDiagonal::diagonalize(): Number of iteration exceeded maximum value");
	    
	  /* Find first m (< n) whose off-diagonal element is 0 */
	    u_int m = n;
	    do
	    {
		if (diagonal_is_zero(m-1))
		{ // If _diagonal[m-1] is zero, make _off_diagonal[m] zero.
		    T	x = _diagonal[m], y = _off_diagonal[m];
		    _off_diagonal[m] = 0.0;
		    for (u_int i = m; i <= n; ++i)
		    {
			Rotation<T>	rotD(m-1, i, x, -y);

			_Dt.rotate_from_left(rotD);
			
			_diagonal[i] = -y*rotD.sin()
				     + _diagonal[i]*rotD.cos();
			if (i < n)
			{
			    x = _diagonal[i+1];
			    y = _off_diagonal[i+1]*rotD.sin();
			    _off_diagonal[i+1] *= rotD.cos();
			}
		    }
		    break;	// if _diagonal[n-1] is zero, m == n here.
		}
	    } while (!off_diagonal_is_zero(--m)); // 0 <= m < n < nrow() here.
	    if (m == n)
		break;		// _off_diagonal[n] has been made 0. Retry!

	  /* Set x and y which determine initial(i = m+1) plane rotation */
	    T	x, y;
	    initialize_rotation(m, n, x, y);
#ifdef TUBiDiagonal_DEBUG
	    cerr << "--- m = " << m << ", n = " << n << "---"
		 << endl;
	    cerr << "  diagonal:     " << diagonal();
	    cerr << "  off-diagonal: " << off_diagonal();
#endif
	  /* Apply rotation P(i-1, i) for each i (i = m+1, n+2, ... , n) */
	    for (u_int i = m; ++i <= n; )
	    {
	      /* Apply rotation from left */
		Rotation<T>	rotE(i-1, i, x, y);
		
		_Et.rotate_from_left(rotE);

		if (i > m+1)
		    _off_diagonal[i-1] = rotE.length();
		T	tmp = _diagonal[i-1];
		_diagonal[i-1]	 =  rotE.cos()*tmp
				 +  rotE.sin()*_off_diagonal[i];
		_off_diagonal[i] = -rotE.sin()*tmp
				 +  rotE.cos()*_off_diagonal[i];
		if (diagonal_is_zero(i))
		    break;		// No more Given's rotation needed.
		y		 =  rotE.sin()*_diagonal[i];
		_diagonal[i]	*=  rotE.cos();

		x = _diagonal[i-1];
		
	      /* Apply rotation from right to recover bi-diagonality */
		Rotation<T>	rotD(i-1, i, x, y);

		_Dt.rotate_from_left(rotD);

		_diagonal[i-1] = rotD.length();
		tmp = _off_diagonal[i];
		_off_diagonal[i] =  tmp*rotD.cos() + _diagonal[i]*rotD.sin();
		_diagonal[i]	 = -tmp*rotD.sin() + _diagonal[i]*rotD.cos();
		if (i < n)
		{
		    if (off_diagonal_is_zero(i+1))
			break;		// No more Given's rotation needed.
		    y		        = _off_diagonal[i+1]*rotD.sin();
		    _off_diagonal[i+1] *= rotD.cos();

		    x		        = _off_diagonal[i];
		}
	    }
#ifdef TUVectorPP_DEBUG
	    cerr << "  niter = " << niter << ": " << off_diagonal();
#endif
	}
    }

    for (u_int m = 0; m < _Et.dim(); ++m)  // sort singular values and vectors
	for (u_int n = m+1; n < _Et.dim(); ++n)
	    if (std::fabs(_diagonal[n]) > std::fabs(_diagonal[m]))
	    {
		swap(_diagonal[m], _diagonal[n]);
		for (u_int j = 0; j < _Et.dim(); ++j)
		{
		    const T	tmp = _Et[m][j];
		    _Et[m][j] = _Et[n][j];
		    _Et[n][j] = -tmp;
		}
		for (u_int j = 0; j < _Dt.dim(); ++j)
		{
		    const T	tmp = _Dt[m][j];
		    _Dt[m][j] = _Dt[n][j];
		    _Dt[n][j] = -tmp;
		}
	    }

    u_int l = min(_Dt.dim() - 1, _Et.dim());	// last index
    for (u_int m = 0; m < l; ++m)	// ensure positivity of all singular
	if (_diagonal[m] < 0.0)		// values except for the last one.
	{
	    _diagonal[m] = -_diagonal[m];
	    for (u_int j = 0; j < _Et.dim(); ++j)
		_Et[m][j] = -_Et[m][j];

	    if (l < _Et.dim())
	    {
		_diagonal[l] = -_diagonal[l];
		for (u_int j = 0; j < _Et.dim(); ++j)
		    _Et[l][j] = -_Et[l][j];
	    }
	}
}

template <class T> bool
BiDiagonal<T>::diagonal_is_zero(u_int n) const
{
    return _Dt.sigma_is_zero(n, _anorm);
}

template <class T> bool
BiDiagonal<T>::off_diagonal_is_zero(u_int n) const
{
    return _Et.sigma_is_zero(n, _anorm);
}

template <class T> void
BiDiagonal<T>::initialize_rotation(u_int m, u_int n, T& x, T& y) const
{
    const T	g    = ((_diagonal[n]     + _diagonal[n-1])*
			(_diagonal[n]     - _diagonal[n-1])+
			(_off_diagonal[n] + _off_diagonal[n-1])*
			(_off_diagonal[n] - _off_diagonal[n-1]))
		     / (2.0*_diagonal[n-1]*_off_diagonal[n]),
      // Caution!! You have to ensure that _diagonal[n-1] != 0
      // as well as _off_diagonal[n].
		absg = std::fabs(g),
		gg1  = (absg > 1.0 ?
			absg * std::sqrt(1.0 + (1.0/absg)*(1.0/absg)) :
			std::sqrt(1.0 + absg*absg)),
		t    = (g > 0.0 ? g + gg1 : g - gg1);
    x = ((_diagonal[m] + _diagonal[n])*(_diagonal[m] - _diagonal[n]) -
	 _off_diagonal[n]*(_off_diagonal[n] + _diagonal[n-1]/t)) / _diagonal[m];
  //x = _diagonal[m];				// without shifting
    y = _off_diagonal[m+1];
}

/************************************************************************
*  class SVDecomposition<T>						*
************************************************************************/
//! ��ʍs��̓��ْl������\���N���X
/*!
  �^����ꂽ��ʍs��\f$\TUvec{A}{} \in \TUspace{R}{m\times n}\f$�ɑ΂�
  ��\f$\TUtvec{V}{}\TUvec{A}{}\TUvec{U}{}\f$���Ίp�s��ƂȂ�悤��2��
  ��]�s��\f$\TUtvec{U}{} \in \TUspace{R}{n\times n}\f$,
  \f$\TUtvec{V}{} \in \TUspace{R}{m\times m}\f$�����߂�D
 */
template <class T>
class SVDecomposition : private BiDiagonal<T>
{
  public:
    typedef T					value_type;	//!< �����̌^

  private:
    typedef BiDiagonal<T>			super;
    
  public:
  //! �^����ꂽ��ʍs��̓��ْl���������߂�D
  /*!
    \param a	���ْl���������ʍs��
  */
    template <class T2, class B2, class R2>
    SVDecomposition(const Matrix<T2, B2, R2>& a)
	:super(a)				{super::diagonalize();}

    using	super::nrow;
    using	super::ncol;
    using	super::Ut;
    using	super::Vt;
    using	super::diagonal;

  //! ���ْl�����߂�D
  /*!
    \param i	��Βl�̑傫�����ɕ��񂾓��ْl��1���w�肷��index
    \return	�w�肳�ꂽindex�ɑΉ�������ْl
  */
    const T&	operator [](int i)	const	{return diagonal()[i];}
};

/************************************************************************
*  typedefs								*
************************************************************************/
typedef Vector<short,  FixedSizedBuf<short,   2> >
	Vector2s;			//!< short�^����������2�����x�N�g��
typedef Vector<int,    FixedSizedBuf<int,     2> >
	Vector2i;			//!< int�^����������2�����x�N�g��
typedef Vector<float,  FixedSizedBuf<float,   2> >
	Vector2f;			//!< float�^����������2�����x�N�g��
typedef Vector<double, FixedSizedBuf<double,  2> >
	Vector2d;			//!< double�^����������2�����x�N�g��
typedef Vector<short,  FixedSizedBuf<short,   3> >
	Vector3s;			//!< short�^����������3�����x�N�g��
typedef Vector<int,    FixedSizedBuf<int,     3> >
	Vector3i;			//!< int�^����������3�����x�N�g��
typedef Vector<float,  FixedSizedBuf<float,   3> >
	Vector3f;			//!< float�^����������3�����x�N�g��
typedef Vector<double, FixedSizedBuf<double,  3> >
	Vector3d;			//!< double�^����������3�����x�N�g��
typedef Vector<short,  FixedSizedBuf<short,   4> >
	Vector4s;			//!< short�^����������4�����x�N�g��
typedef Vector<int,    FixedSizedBuf<int,     4> >
	Vector4i;			//!< int�^����������4�����x�N�g��
typedef Vector<float,  FixedSizedBuf<float,   4> >
	Vector4f;			//!< float�^����������4�����x�N�g��
typedef Vector<double, FixedSizedBuf<double,  4> >
	Vector4d;			//!< double�^����������4�����x�N�g��
typedef Matrix<float,  FixedSizedBuf<float,   4>,
	       FixedSizedBuf<Vector<float>,   2> >
	Matrix22f;			//!< float�^����������2x2�s��
typedef Matrix<double, FixedSizedBuf<double,  4>,
	       FixedSizedBuf<Vector<double>,  2> >
	Matrix22d;			//!< double�^����������2x2�s��
typedef Matrix<float,  FixedSizedBuf<float,   6>,
	       FixedSizedBuf<Vector<float>,   2> >
	Matrix23f;			//!< float�^����������2x3�s��
typedef Matrix<double, FixedSizedBuf<double,  6>,
	       FixedSizedBuf<Vector<double>,  2> >
	Matrix23d;			//!< double�^����������2x3�s��
typedef Matrix<float,  FixedSizedBuf<float,   9>,
	       FixedSizedBuf<Vector<float>,   3> >
	Matrix33f;			//!< float�^����������3x3�s��
typedef Matrix<double, FixedSizedBuf<double,  9>,
	       FixedSizedBuf<Vector<double>,  3> >
	Matrix33d;			//!< double�^����������3x3�s��
typedef Matrix<float,  FixedSizedBuf<float,  12>,
	       FixedSizedBuf<Vector<float>,   3> >
	Matrix34f;			//!< float�^����������3x4�s��
typedef Matrix<double, FixedSizedBuf<double, 12>,
	       FixedSizedBuf<Vector<double>,  3> >
	Matrix34d;			//!< double�^����������3x4�s��
typedef Matrix<float,  FixedSizedBuf<float,  16>,
	       FixedSizedBuf<Vector<float>,   4> >
	Matrix44f;			//!< float�^����������4x4�s��
typedef Matrix<double, FixedSizedBuf<double, 16>,
	       FixedSizedBuf<Vector<double>,  4> >
	Matrix44d;			//!< double�^����������4x4�s��
typedef Matrix<float, FixedSizedBuf<float, 12>,
	       FixedSizedBuf<Vector<float>,  2> >
	Matrix26f;			//!< float�^����������2x6�s��
typedef Matrix<double, FixedSizedBuf<double, 12>,
	       FixedSizedBuf<Vector<double>,  2> >
	Matrix26d;			//!< double�^����������2x6�s��
}

#endif	/* !__TUVectorPP_h	*/
