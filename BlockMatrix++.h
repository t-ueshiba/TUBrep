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
 *  $Id: BlockMatrix++.h,v 1.10 2009-07-31 07:04:44 ueshiba Exp $
 */
#ifndef __TUBlockMatrixPP_h
#define __TUBlockMatrixPP_h

#include "TU/Vector++.h"

namespace TU
{
/************************************************************************
*  class BlockMatrix<T>							*
************************************************************************/
//! T�^�̗v�f�������s�񂩂琬��u���b�N�Ίp�s���\���N���X
/*!
  ��̓I�ɂ�d�̏��s��\f$\TUvec{B}{1}, \TUvec{B}{2},\ldots, \TUvec{B}{d}\f$
  (����T�C�Y�Ƃ͌���Ȃ�)���琬��
  \f$
  \TUvec{B}{} =
  \TUbeginarray{cccc}
  \TUvec{B}{1} & & & \\ & \TUvec{B}{2} & & \\ & & \ddots & \\
  & & & \TUvec{B}{d}
  \TUendarray
  \f$
  �Ȃ�`�̍s��D
  \param T	�v�f�̌^
*/
template <class T>
class BlockMatrix : public Array<Matrix<T> >
{
  public:
  //! �w�肳�ꂽ���̏��s�񂩂琬��u���b�N�Ίp�s��𐶐�����D
  /*!
    \param d	���s��̌�
  */
    explicit BlockMatrix(u_int d=0)	:Array<Matrix<T> >(d)	{}
    BlockMatrix(const Array<u_int>& nrows,
		const Array<u_int>& ncols)			;

    using		Array<Matrix<T> >::dim;
    u_int		nrow()				const	;
    u_int		ncol()				const	;
    BlockMatrix		trns()				const	;
    BlockMatrix&	operator  =(const T& c)			;
    BlockMatrix&	operator *=(double c)
			{Array<Matrix<T> >::operator *=(c); return *this;}
    BlockMatrix&	operator /=(double c)
			{Array<Matrix<T> >::operator /=(c); return *this;}

  //! ���̃u���b�N�Ίp�s��ɑ��̃u���b�N�Ίp�s��𑫂��D
  /*!
    \param b	�����u���b�N�Ίp�s��
    \return	���̃u���b�N�Ίp�s��
  */
    BlockMatrix&	operator +=(const BlockMatrix& b)
			{Array<Matrix<T> >::operator +=(b); return *this;}

  //! ���̃u���b�N�Ίp�s�񂩂瑼�̃u���b�N�Ίp�s��������D
  /*!
    \param b	�����u���b�N�Ίp�s��
    \return	���̃u���b�N�Ίp�s��
  */
    BlockMatrix&	operator -=(const BlockMatrix& b)
			{Array<Matrix<T> >::operator -=(b); return *this;}

			operator Matrix<T>()		const	;
};

//! �e���s��̃T�C�Y���w�肵�ău���b�N�Ίp�s��𐶐����C�S�v�f��0�ŏ���������D
/*!
  \param nrows	�e���s��̍s�������Ɏ��߂��z��
  \param ncols	�e���s��̗񐔂����Ɏ��߂��z��
*/
template <class T>
BlockMatrix<T>::BlockMatrix(const Array<u_int>& nrows,
			    const Array<u_int>& ncols)
    :Array<Matrix<T> >(nrows.dim())
{
    if (nrows.dim() != ncols.dim())
	throw std::invalid_argument("TU::BlockMatrix<T>::BlockMatrix: dimension mismatch between nrows and ncols!!");
    for (u_int i = 0; i < dim(); ++i)
    {
	(*this)[i].resize(nrows[i], ncols[i]);
	(*this)[i] = 0.0;
    }
}

//! �u���b�N�Ίp�s��̑��s����Ԃ��D
/*!
  \return	���s��
*/
template <class T> u_int
BlockMatrix<T>::nrow() const
{
    size_t	r = 0;
    for (u_int i = 0; i < dim(); ++i)
	r += (*this)[i].nrow();
    return r;
}

//! �u���b�N�Ίp�s��̑��񐔂�Ԃ��D
/*!
  \return	����
*/
template <class T> u_int
BlockMatrix<T>::ncol() const
{
    size_t	c = 0;
    for (u_int i = 0; i < dim(); ++i)
	c += (*this)[i].ncol();
    return c;
}

//! ���̃u���b�N�Ίp�s��̓]�u�s���Ԃ��D
/*!
  \return	�]�u�s��C���Ȃ킿
  \f$
  \TUtvec{B}{} =
  \TUbeginarray{cccc}
  \TUtvec{B}{1} & & & \\ & \TUtvec{B}{2} & & \\ & & \ddots & \\
  & & & \TUtvec{B}{d}
  \TUendarray
  \f$
*/
template <class T> BlockMatrix<T>
BlockMatrix<T>::trns() const
{
    BlockMatrix	val(dim());
    for (u_int i = 0; i < val.dim(); ++i)
	val[i] = (*this)[i].trns();
    return val;
}

//! ���̃u���b�N�Ίp�s��̑S�Ă̏��s��̑S�v�f�ɓ���̐��l��������D
/*!
  \param c	������鐔�l
  \return	���̃u���b�N�Ίp�s��
*/
template <class T> BlockMatrix<T>&
BlockMatrix<T>::operator =(const T& c)
{
    for (u_int i = 0; i < dim(); ++i)
	(*this)[i] = c;
    return *this;
}

//! ���̃u���b�N�Ίp�s���ʏ�̍s��ɕϊ�����D
/*!
  \return	�ϊ����ꂽ�s��
*/
template <class T>
BlockMatrix<T>::operator Matrix<T>() const
{
    Matrix<T>	val(nrow(), ncol());
    int		r = 0, c = 0;
    for (u_int i = 0; i < dim(); ++i)
    {
	val(r, c, (*this)[i].nrow(), (*this)[i].ncol()) = (*this)[i];
	r += (*this)[i].nrow();
	c += (*this)[i].ncol();
    }
    return val;
}

/************************************************************************
*  numeric operators							*
************************************************************************/
//! 2�̃u���b�N�Ίp�s��̐�
/*!
  \param a	��1����
  \param b	��2����
  \return	���ʂ̃u���b�N�Ίp�s��
*/
template <class T> BlockMatrix<T>
operator *(const BlockMatrix<T>& a, const BlockMatrix<T>& b)
{
    a.check_dim(b.dim());
    BlockMatrix<T>	val(a.dim());
    for (u_int i = 0; i < val.dim(); ++i)
	val[i] = a[i] * b[i];
    return val;
}

//! �u���b�N�Ίp�s��ƒʏ�̍s��̐�
/*!
  \param b	��1����(�u���b�N�Ίp�s��)
  \param m	��2����(�ʏ�s��)
  \return	���ʂ̒ʏ�s��
*/
template <class T> Matrix<T>
operator *(const BlockMatrix<T>& b, const Matrix<T>& m)
{
    Matrix<T>	val(b.nrow(), m.ncol());
    u_int	r = 0, c = 0;
    for (u_int i = 0; i < b.dim(); ++i)
    {
	val(r, 0, b[i].nrow(), m.ncol())
	    = b[i] * m(c, 0, b[i].ncol(), m.ncol());
	r += b[i].nrow();
	c += b[i].ncol();
    }
    if (c != m.nrow())
	throw std::invalid_argument("TU::operaotr *(const BlockMatrix<T>&, const Matrix<T>&): dimension mismatch!!");
    return val;
}

//! �ʏ�̍s��ƃu���b�N�Ίp�s��̐�
/*!
  \param m	��1����(�ʏ�s��)
  \param b	��2����(�u���b�N�Ίp�s��)
  \return	���ʂ̒ʏ�s��
*/
template <class T> Matrix<T>
operator *(const Matrix<T>& m, const BlockMatrix<T>& b)
{
    Matrix<T>	val(m.nrow(), b.ncol());
    u_int	r = 0, c = 0;
    for (u_int i = 0; i < b.dim(); ++i)
    {
	val(0, c, m.nrow(), b[i].ncol())
	    = m(0, r, m.nrow(), b[i].nrow()) * b[i];
	r += b[i].nrow();
	c += b[i].ncol();
    }
    if (r != m.ncol())
	throw std::invalid_argument("TU::operaotr *(const Matrix<T>&, const BlockMatrix<T>&): dimension mismatch!!");
    return val;
}

//! �u���b�N�Ίp�s��ƃx�N�g���̐�
/*!
  \param b	�u���b�N�Ίp�s��
  \param v	�x�N�g��
  \return	���ʂ̃x�N�g��
*/
template <class T> Vector<T>
operator *(const BlockMatrix<T>& b, const Vector<T>& v)
{
    Vector<T>	val(b.nrow());
    u_int	r = 0, c = 0;
    for (u_int i = 0; i < b.dim(); ++i)
    {
	val(r, b[i].nrow()) = b[i] * v(c, b[i].ncol());
	r += b[i].nrow();
	c += b[i].ncol();
    }
    if (c != v.dim())
	throw std::invalid_argument("TU::operaotr *(const BlockMatrix<T>&, const Vector<T>&): dimension mismatch!!");
    return val;
}

//! �x�N�g���ƃu���b�N�Ίp�s��̐�
/*!
  \param v	�x�N�g��
  \param b	�u���b�N�Ίp�s��
  \return	���ʂ̃x�N�g��
*/
template <class T> Vector<T>
operator *(const Vector<T>& v, const BlockMatrix<T>& b)
{
    Vector<T>	val(b.ncol());
    u_int	r = 0, c = 0;
    for (u_int i = 0; i < b.dim(); ++i)
    {
	val(c, b[i].ncol()) = v(r, b[i].nrow()) * b[i];
	r += b[i].nrow();
	c += b[i].ncol();
    }
    if (r != v.dim())
	throw std::invalid_argument("TU::operaotr *(const Vector<T>&, const BlockMatrix<T>&): dimension mismatch!!");
    return val;
}
 
}

#endif	/* !__TUBlockMatrixPP_h	*/
