/*
 *  ����21-22�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
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
 *  Copyright 2009-2010.
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
 *  $Id: SparseSymmetricMatrix.h,v 1.1 2010-12-02 07:26:36 ueshiba Exp $
 */
/*!
  \file		SparseMatrix.h
  \brief	�N���X#TU::SparseSymmetricMatrix�̒�`�Ǝ���
*/
#ifndef __SPARSESYMMETRICMATRIX_H
#define __SPARSESYMMETRICMATRIX_H

#include "TU/Vector++.h"
#include <algorithm>
#include <mkl_pardiso.h>

namespace TU
{
/************************************************************************
*  class SparseSymmetricMatrix<T>					*
************************************************************************/
//! Intel Math-Kernel Library(MKL)�̃t�H�[�}�b�g�ɂ��a�Ώ̍s��
template <class T>
class SparseSymmetricMatrix
{
  public:
    typedef T	value_type;		//!< �����̌^
    
  public:
    SparseSymmetricMatrix(u_int d, u_int nelms)				;

    SparseSymmetricMatrix<T>&
		operator =(value_type c)				;
    u_int	dim()						const	;
    u_int	nelements()					const	;
    u_int	nelements(u_int i)				const	;
    T&		operator ()(u_int i)					;
    const T&	operator ()(u_int i)				const	;
    T&		operator ()(u_int i, u_int j)				;
    const T&	operator ()(u_int i, u_int j)			const	;
    template <class F, class S, class B>
    SparseSymmetricMatrix<T>&
		apply(u_int i, u_int j, F f, const Vector<S, B>& v)	;
    template <class F, class S, class B, class R>
    SparseSymmetricMatrix<T>&
		apply(u_int i, u_int j, F f, const Matrix<S, B, R>& M)	;
    Vector<T>	solve(const Vector<T>& b)			const	;

    template <class S> friend std::ostream&
		operator <<(std::ostream& out,
			    const SparseSymmetricMatrix<S>& A)		;
    template <class S> friend std::istream&
		operator >>(std::istream& in,
			    SparseSymmetricMatrix<S>& A)		;
    
  protected:
    u_int	index(u_int i, u_int j)				const	;
    
  private:
    static int	pardiso_precision()					;

  protected:
    Array<u_int>	_rowIndex;	//!< �e�s�̐擪�����̒ʂ��ԍ�(Fortran�`��)
    Array<u_int>	_columns;	//!< �e�����̗�ԍ�(Fortran�`��)
    Vector<T>		_values;	//!< �e�����̒l
};

//! �a�Ώ̍s��𐶐�����D
/*!
  \param d	�s��̎���
  \param nelms	�s��̐�����
*/
template <class T> inline
SparseSymmetricMatrix<T>::SparseSymmetricMatrix(u_int d, u_int nelms)
    :_rowIndex(d + 1), _columns(nelms), _values(_columns.dim())
{
    _rowIndex = 0;
    _rowIndex[dim()] = nelements() + 1;
    _columns = 0;
}
    
//! ���ׂĂ�0�łȂ������ɒ萔��������D
/*!
  \param c	�������萔
  \return	���̑a�Ώ̍s��
*/
template <class T> inline SparseSymmetricMatrix<T>&
SparseSymmetricMatrix<T>::operator =(value_type c)
{
    _values = c;

    return *this;
}
    
//! �s��̎�����Ԃ��D
/*!
  \return	�s��̎���
*/
template <class T> inline u_int
SparseSymmetricMatrix<T>::dim() const
{
    return _rowIndex.dim() - 1;
}
    
//! �s��̑S��������Ԃ��D
/*!
  \return	�s��̑S������
*/
template <class T> inline u_int
SparseSymmetricMatrix<T>::nelements() const
{
    return _columns.dim();
}
    
//! �w�肳�ꂽ�s�̐�������Ԃ��D
/*!
  \param i	�s���w�肷��index
  \return	��i�s�̐�����
*/
template <class T> inline u_int
SparseSymmetricMatrix<T>::nelements(u_int i) const
{
    return _rowIndex[i+1] - _rowIndex[i];
}
    
//! �w�肳�ꂽ�s�̑Ίp������Ԃ��D
/*!
  \param i	�s���w�肷��index
  \return	(i, i)����
*/
template <class T> inline T&
SparseSymmetricMatrix<T>::operator ()(u_int i)
{
    return _values[_rowIndex[i] - 1];
}
    
//! �w�肳�ꂽ�s�̑Ίp������Ԃ��D
/*!
  \param i	�s���w�肷��index
  \return	(i, i)����
*/
template <class T> inline const T&
SparseSymmetricMatrix<T>::operator ()(u_int i) const
{
    return _values[_rowIndex[i] - 1];
}
    
//! �w�肳�ꂽ�s�Ɨ�ɑΉ����鐬����Ԃ��D
/*!
  \param i	�s���w�肷��index
  \param j	����w�肷��index
  \return	(i, j)����
*/
template <class T> inline T&
SparseSymmetricMatrix<T>::operator ()(u_int i, u_int j)
{
    return _values[index(i, j) - 1];
}
    
//! �w�肳�ꂽ�s�Ɨ�ɑΉ����鐬����Ԃ��D
/*!
  \param i	�s���w�肷��index
  \param j	����w�肷��index
  \return	(i, j)����
*/
template <class T> inline const T&
SparseSymmetricMatrix<T>::operator ()(u_int i, u_int j) const
{
    return _values[index(i, j) - 1];
}

//! �^����ꂽ�x�N�g���̊e�������w�肳�ꂽ�������N�_�Ƃ��čs�����ɓK�p����D
/*!
  �Ώ̍s��ł��邩��Ci > j�̏ꍇ��(j, i)�������N�_�Ƃ��ė�����ɓK�p���邱�Ƃɓ����ł���D
  \param i	�s���w�肷��index
  \param j	����w�肷��index
  \param f	T�^�CS�^�̈������Ƃ�T�^�̒l��Ԃ�2�����Z�q
  \param v	���̊e������f�̑�2�����ƂȂ�x�N�g��
  \return	���̑a�Ώ̍s��
*/
template <class T> template <class F, class S, class B>
SparseSymmetricMatrix<T>&
SparseSymmetricMatrix<T>::apply(u_int i, u_int j, F f, const Vector<S, B>& v)
{
    if (i <= j)
    {
	T*	p = &(*this)(i, j);

	for (u_int dj = 0; dj < v.dim(); ++dj)
	{
	    *p = f(*p, v[dj]);
	    ++p;
	}
    }
    else
    {
	T*	p = &(*this)(j, i);

	for (u_int dj = 0; dj < v.dim(); ++dj)
	{
	    *p = f(*p, v[dj]);
	    p += (nelements(j + dj) - 1);
	}
    }
    
    return *this;
}
    
//! �^����ꂽ�s��̊e�������w�肳�ꂽ�������N�_�Ƃ��čs�D�揇�ɓK�p����D
/*!
  �Ώ̍s��ł��邩��Ci > j�̏ꍇ��(j, i)�������N�_�Ƃ��ė�D�揇�ɓK�p���邱�Ƃɓ����ł���D
  \param i	�s���w�肷��index
  \param j	����w�肷��index
  \param f	T�^�CS�^�̈������Ƃ�T�^�̒l��Ԃ�2�����Z�q
  \param M	���̊e������f�̑�2�����ƂȂ�s��
  \return	���̑a�Ώ̍s��
*/
template <class T> template <class F, class S, class B, class R>
SparseSymmetricMatrix<T>&
SparseSymmetricMatrix<T>::apply(u_int i, u_int j, F f, const Matrix<S, B, R>& M)
{
    if (i == j)
    {
	for (u_int di = 0; di < M.nrow(); ++di)
	{
	    T*	p = &(*this)(i + di);
				    
	    for (u_int dj = di; dj < M.ncol(); ++dj)
	    {
		*p = f(*p, M[di][dj]);
		++p;
	    }
	}
    }
    else if (i < j)
    {
	T*	p = &(*this)(i, j);

	for (u_int di = 0; di < M.nrow(); ++di)
	{
	    for (u_int dj = 0; dj < M.ncol(); ++dj)
	    {
		*p = f(*p, M[di][dj]);
		++p;
	    }

	    p += (nelements(i + di) - M.ncol() - 1);
	}
    }
    else
    {
	T*	p = &(*this)(j, i);

	for (u_int dj = 0; dj < M.ncol(); ++dj)
	{
	    for (u_int di = 0; di < M.nrow(); ++di)
	    {
		*p = f(*p, M[di][dj]);
		++p;
	    }

	    p += (nelements(j + dj) - M.nrow() - 1);
	}
    }
    
    return *this;
}
    
//! ���̍s����W���Ƃ���A���ꎟ�������������D
/*!
  MKL direct sparse solver�ɂ����
  \f$\TUvec{A}{}\TUvec{x}{} = \TUvec{b}{}\f$�������D
  \param b	�x�N�g��
  \return	���x�N�g��
*/
template <class T> Vector<T>
SparseSymmetricMatrix<T>::solve(const Vector<T>& b) const
{
    using namespace	std;

    if (b.dim() != dim())
	throw runtime_error("SparseSymmetricMatrix<T>::solve(): input vector with invalid dimension!");

  // pardiso �̊e��p�����[�^��ݒ肷��D
    _MKL_DSS_HANDLE_t	pt[64];		// pardiso�̓����������ւ̃|�C���^
    for (int i = 0; i < 64; ++i)
	pt[i] = 0;
    _INTEGER_t		iparm[64];
    for (int i = 0; i < 64; ++i)
	iparm[i] = 0;
    iparm[0]  =  1;			// iparm[1-] �Ƀf�t�H���g�l�ȊO���w��
    iparm[1]  =  2;			// nested dissection algorithm
    iparm[9]  =  8;			// eps = 1.0e-8
    iparm[17] = -1;			// �����̔�됬���������|�[�g
    iparm[20] =  1;			// Bunch and Kaufman pivoting
    iparm[27] = pardiso_precision();	// float �܂��� double ���w��
    _INTEGER_t		maxfct = 1;	// ���̕�����ێ�����ׂ��s��̐�
    _INTEGER_t		mnum   = 1;	// ���Ԗڂ̍s��ɂ��ĉ��������w��
    _INTEGER_t		mtype  = -2;	// ���Ώ̍s��
    _INTEGER_t		phase  = 13;	// �s��̉�͂��甽���ɂ��ז����܂�
    _INTEGER_t		n      = dim();	// �A���ꎟ�������̎���
    _INTEGER_t		nrhs   = 1;	// Ax = b �ɂ����� b �̗�
    _INTEGER_t		msglvl = 0;	// ���b�Z�[�W���o�͂��Ȃ�
    _INTEGER_t		error  = 0;	// �G���[�R�[�h
    Array<_INTEGER_t>	perm(n);	// permutation�x�N�g��
    Vector<T>		x(n);		// ���x�N�g��

  // �A���ꎟ�������������D
    PARDISO(&pt[0], &maxfct, &mnum, &mtype, &phase, &n, (void*)&_values[0],
	    (_INTEGER_t*)&_rowIndex[0], (_INTEGER_t*)&_columns[0],
	    &perm[0], &nrhs, iparm, &msglvl, (void*)&b[0], &x[0], &error);
    if (error != 0)
	throw runtime_error("SparseSymmetricMatrix<T>::solve(): PARDISO failed!");

  // pardiso ���Ŏg�p�����S���������������D
    phase = -1;
    PARDISO(&pt[0], &maxfct, &mnum, &mtype, &phase, &n, (void*)&_values[0],
	    (_INTEGER_t*)&_rowIndex[0], (_INTEGER_t*)&_columns[0],
	    &perm[0], &nrhs, iparm, &msglvl, (void*)&b[0], &x[0], &error);
    if (error != 0)
	throw runtime_error("SparseSymmetricMatrix<T>::solve(): PARDISO failed to release memory!");

    return x;
}

//! �w�肳�ꂽ�s�Ɨ�ɑΉ�����#_values��index��Ԃ��D
/*!
  #_rowIndex�Ɠ��l��Fortran�`����index��Ԃ��̂ŁC���ۂ�#_values�ɃA�N�Z�X����ۂɂ�
  �Ԃ��ꂽ�l����1�������K�v������D
  \param i			�s���w�肷��index
  \param j			����w�肷��index
  \return			#_values��index(Fortran�`��)
  \throw std::out_of_range	���̍s��(i, j)�����������Ȃ��ꍇ�ɑ��o
*/
template <class T> inline u_int
SparseSymmetricMatrix<T>::index(u_int i, u_int j) const
{
    if (i > j)
	std::swap(i, j);

    ++j;				// ��index��Fortran�`���ɒ���

    u_int	n = _rowIndex[i];	// ��i�s�̐������w��index(Fortran�`��)
    for (; n < _rowIndex[i+1]; ++n)
	if (_columns[n-1] == j)
	    return n;
    if (n == 0)
	throw std::out_of_range("TU::SparseSymmetricMatrix<T>::index(): non-existent element!");
    return 0;
}
    
template<> inline int
SparseSymmetricMatrix<float>::pardiso_precision()	{return 1;}
template<> inline int
SparseSymmetricMatrix<double>::pardiso_precision()	{return 0;}

//! �o�̓X�g���[���֑a�Ώ̍s��������o��(ASCII)�C����ɉ��s�R�[�h���o�͂���D
/*!
  \param out	�o�̓X�g���[��
  \param a	�����o���a�Ώ̍s��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T> std::ostream&
operator <<(std::ostream& out, const SparseSymmetricMatrix<T>& A)
{
    using namespace	std;

    u_int	n = 0;
    for (u_int i = 0; i < A.dim(); ++i)
    {
	for (u_int j = 0; j < i; ++j)
	    out << " *";

	for (u_int j = i; j < A.dim(); ++j)
	{
	    if ((n < A._rowIndex[i+1] - 1) && (j == A._columns[n] - 1))
		out << ' ' << A._values[n++];
	    else
		out << " (0)";
	}
	out << endl;
    }

    return out << endl;
}

}

#endif	// ! __SPARSESYMMETRICMATRIX_H
