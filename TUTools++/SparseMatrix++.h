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
 *  $Id: SparseMatrix++.h,v 1.5 2011-09-19 18:25:22 ueshiba Exp $
 */
/*!
  \file		SparseMatrix++.h
  \brief	�N���X TU::SparseMatrix �̒�`�Ǝ���
*/
#ifndef __TUSparseMatrixPP_h
#define __TUSparseMatrixPP_h

#include "TU/Vector++.h"
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <mkl_pardiso.h>

namespace TU
{
/************************************************************************
*  class SparseMatrix<T, SYM>						*
************************************************************************/
//! Intel Math-Kernel Library(MKL)�̃t�H�[�}�b�g�ɂ��a�s��
template <class T, bool SYM=false>
class SparseMatrix
{
  public:
    typedef T		value_type;		//!< �����̌^

  private:
    template <class S>
    struct Assign : public std::binary_function<T, T, S>
    {
	T	operator ()(const T& x, const S& y)	const	{return y;}
    };
    
  public:
  // �\������
    void		beginInit()					;
    void		setRow()					;
    void		setCol(u_int col, value_type val=0)		;
    void		copyRow()					;
    void		endInit()					;
    
  // ��{���
    u_int		dim()					const	;
    u_int		nrow()					const	;
    u_int		ncol()					const	;
    u_int		nelements()				const	;
    u_int		nelements(u_int i)			const	;
    T			operator ()(u_int i, u_int j)		const	;
    T&			operator ()(u_int i, u_int j)			;

  // ��{���Z
    SparseMatrix&	operator  =(value_type c)			;
    SparseMatrix&	operator *=(value_type c)			;
    SparseMatrix&	operator /=(value_type c)			;
    SparseMatrix&	operator +=(const SparseMatrix& A)		;
    SparseMatrix&	operator -=(const SparseMatrix& A)		;
    SparseMatrix	operator  +(const SparseMatrix& A)	const	;
    SparseMatrix	operator  -(const SparseMatrix& A)	const	;
    template <class S, class B>
    Vector<value_type>	operator  *(const Vector<S, B>& v)	const	;
    template <class S, class B, class T2, bool SYM2>
    friend Vector<S>	operator  *(const Vector<S, B>& v,
				    const SparseMatrix<T2, SYM2>& A)	;
    SparseMatrix<T, true>
			compose()				const	;
    SparseMatrix<T, true>
			compose(const SparseMatrix<T, true>& W)	const	;

  // �u���b�N���Z
    Vector<T>		operator ()(u_int i, u_int j, u_int d)	const	;
    Matrix<T>		operator ()(u_int i, u_int j,
				    u_int r, u_int c)		const	;
    template <class S, class B>
    SparseMatrix&	assign(u_int i, u_int j, const Vector<S, B>& v)	;
    template <class S, class B, class R>
    SparseMatrix&	assign(u_int i, u_int j,
			       const Matrix<S, B, R>& M)		;
    template <class OP, class S, class B>
    SparseMatrix&	apply(u_int i, u_int j,
			      OP op, const Vector<S, B>& v)		;
    template <class OP, class S, class B, class R>
    SparseMatrix&	apply(u_int i, u_int j,
			      OP op, const Matrix<S, B, R>& M)		;

  // �A���ꎟ������
    Vector<T>		solve(const Vector<T>& b)		const	;

  // �L�����̃`�F�b�N
    bool		isfinite()				const	;
    
  // ���o��
    std::istream&	get(std::istream& in)				;
    std::ostream&	put(std::ostream& out)			const	;
    
    friend class SparseMatrix<T, !SYM>;

  private:
    template <class OP>
    SparseMatrix	binary_op(const SparseMatrix& B, OP op)	const	;
    template <bool SYM2>
    bool		inner_product(const SparseMatrix<T, SYM2>& B,
				      u_int i, u_int j, T& val)	const	;
    int			index(u_int i, u_int j,
			      bool throwExcept=false)		const	;
    static int		pardiso_precision()				;
    static char		skipws(std::istream& in)			;
    
  private:
    u_int		_ncol;		//!< ��̐�
    std::vector<u_int>	_rowIndex;	//!< �e�s�̐擪�����̒ʂ��ԍ�(1�x�[�X)
    std::vector<u_int>	_columns;	//!< �e�����̗�ԍ�(1�x�[�X)
    std::vector<T>	_values;	//!< �e�����̒l
    std::map<u_int, T>	_rowmap;	//!< 1�s���̗�ԍ��ƒl�̈ꎞ�o�b�t�@
};

/*
 * ----------------------- �\������ -----------------------------
 */
//! ���������J�n����D
template <class T, bool SYM> inline void
SparseMatrix<T, SYM>::beginInit()
{
    _ncol = 0;
    _rowIndex.clear();
    _columns.clear();
    _values.clear();
    _rowmap.clear();
}

//! �s�̐擪�ʒu���Z�b�g����D
template <class T, bool SYM> inline void
SparseMatrix<T, SYM>::setRow()
{
  // �\�[�g�ς݂�1�O�̍s�̗�ԍ������ꂼ��_columns��_value�ɃR�s�[
    if (!_rowIndex.empty())
    {
	for (typename std::map<u_int, T>::const_iterator
		 iter = _rowmap.begin(); iter != _rowmap.end(); ++iter)
	{
	    _columns.push_back(iter->first);
	    _values.push_back(iter->second);
	}
	_rowmap.clear();
    }

  // ���̍s�̐擪�����̒ʂ��ԍ����Z�b�g(1�x�[�X)
    _rowIndex.push_back(nelements() + 1);
}

//! �����̗�ԍ��Ƃ��̒l���Z�b�g����D
/*!
  \param col			��ԍ�
  \param val			�l
  \throw std::logic_error	setRow() ���s���O�ɌĂяo���Ƒ��o
  \throw std::invalid_argument	�Ώ̍s��̏ꍇ�C���݂̍s�ԍ���菬���ȗ�ԍ�
				���w�肷��Ƒ��o
*/
template <class T, bool SYM> inline void
SparseMatrix<T, SYM>::setCol(u_int col, value_type val)
{
    u_int	row = _rowIndex.size();	// ���݂܂łɃZ�b�g���ꂽ�s��
    if (row == 0)
	throw std::logic_error("TU::SparseMatrix<T, SYM>::setCol(): _rowIndex is not set!");
    if (SYM)
    {
	if (col < --row)		// �^����ꂽ��ԍ������݂̍s�ԍ��Ɣ�r
	    throw std::invalid_argument("TU::SparseMatrix<T, SYM>::setCol(): column index must not be less than row index!");
    }

    _rowmap.insert(std::make_pair(col + 1, val));

    if (col + 1 > _ncol)
	_ncol = col + 1;		// �񐔂��X�V

}

//! ���O�̍s�Ɠ����ʒu�ɔ�됬�������悤�ȍs���Z�b�g����D
/*!
  �V���ɃZ�b�g�����s�̐����̒l�͑S��0�ƂȂ�D
  \throw std::logic_error	���O�̍s�����݂��Ȃ��ꍇ�ɑ��o
*/
template <class T, bool SYM> inline void
SparseMatrix<T, SYM>::copyRow()
{
    u_int	row = _rowIndex.size();	// ���݂܂łɃZ�b�g���ꂽ�s��
    if (row == 0)
	throw std::logic_error("TU::SparseMatrix<T, SYM>::copyRow(): no previous rows!");

  // �s�̐擪�����̒ʂ��ԍ����Z�b�g����Drow�����݂̍s�ԍ��ƂȂ�D
    setRow();

  // ���O�̍s��(�Ώ̍s��̏ꍇ��2�Ԗڈȍ~��)�����̗�ԍ������݂̍s�ɃR�s�[����D
    for (u_int n  = _rowIndex[row - 1] - (SYM ? 0 : 1),
	       ne = _rowIndex[row] - 1; n < ne; ++n)
    {
	_columns.push_back(_columns[n]);
	_values.push_back(T(0));
    }
}

//! ����������������D
/*!
  \throw std::logic_error	�������ꂽ�a�s��̍\�������Ă���ꍇ�ɑ��o
*/
template <class T, bool SYM> inline void
SparseMatrix<T, SYM>::endInit()
{
    setRow();				// �_�~�[�̍s���Z�b�g
    
  // �\���`�F�b�N���s���D
    if (SYM)				// �Ώ̍s��Ȃ�...
    {
      // �s���Ɨ񐔂����������H
	if (nrow() != ncol())
	    throw std::logic_error("SparseMatrix<T, true>::endInit(): the numbers of rows and columns must be equal!");
    }
#ifdef LIBTUTOOLS_DEBUG
    if (SYM)
    {
      // �e�s�̐擪�������Ίp�����ł��邩�H
	for (u_int i = 0; i < nrow(); ++i)
	    if (i != _columns[_rowIndex[i] - 1] - 1)
		throw std::logic_error("SparseMatrix<T, true>::endInit(): the first entry of each row must be a diagonal element!");
    }

  // �e�s���ŗ�ԍ������`�P�������ɂȂ��Ă��邩�H
    for (u_int i = 0; i < nrow(); ++i)
	for (int n = _rowIndex[i] - 1; n < _rowIndex[i+1] - 2; ++n)
	    if (_columns[n] >= _columns[n+1])
		throw std::logic_error("SparseMatrix<T, SYM>::endInit(): the column indices must be strictly increasing within each row!");
#endif
}

/*
 * ----------------------- ��{��� ---------------------------------
 */
//! �s��̎������Ȃ킿�s����Ԃ��D
/*!
  \return	�s��̎���(=�s��)
*/
template <class T, bool SYM> inline u_int
SparseMatrix<T, SYM>::dim() const
{
    return nrow();
}
    
//! �s��̍s����Ԃ��D
/*!
  \return	�s��̍s��
*/
template <class T, bool SYM> inline u_int
SparseMatrix<T, SYM>::nrow() const
{
    return _rowIndex.size() - 1;
}
    
//! �s��̗񐔂�Ԃ��D
/*!
  \return	�s��̗�
*/
template <class T, bool SYM> inline u_int
SparseMatrix<T, SYM>::ncol() const
{
    return _ncol;
}
    
//! �s��̔�됬������Ԃ��D
/*!
  \return	�s��̔�됬����
*/
template <class T, bool SYM> inline u_int
SparseMatrix<T, SYM>::nelements() const
{
    return _columns.size();
}
    
//! �w�肳�ꂽ�s�̐�������Ԃ��D
/*!
  \param i	�s�ԍ�
  \return	��i�s�̐�����
*/
template <class T, bool SYM> inline u_int
SparseMatrix<T, SYM>::nelements(u_int i) const
{
    return _rowIndex[i+1] - _rowIndex[i];
}
    
//! �w�肳�ꂽ�s�Ɨ�ɑΉ����鐬����Ԃ��D
/*!
  \param i			�s�ԍ�
  \param j			��ԍ�
  \return			(i, j)����
*/
template <class T, bool SYM> inline T
SparseMatrix<T, SYM>::operator ()(u_int i, u_int j) const
{
    const int	n = index(i, j);
    return (n >= 0 ? _values[n] : T(0));
}

//! �w�肳�ꂽ�s�Ɨ�ɑΉ����鐬����Ԃ��D
/*!
  \param i			�s�ԍ�
  \param j			��ԍ�
  \return			(i, j)����
  \throw std::invalid_argument	���̍s��(i, j)�����������Ȃ��ꍇ�ɑ��o
*/
template <class T, bool SYM> inline T&
SparseMatrix<T, SYM>::operator ()(u_int i, u_int j)
{
    const int	n = index(i, j, true);
    return _values[n];
}
    
/*
 * ----------------------- ��{���Z ---------------------------------
 */
//! ���ׂĂ�0�łȂ������ɒ萔��������D
/*!
  \param c	�������萔
  \return	���̑a�s��
*/
template <class T, bool SYM> inline SparseMatrix<T, SYM>&
SparseMatrix<T, SYM>::operator =(value_type c)
{
    std::fill(_values.begin(), _values.end(), c);
    return *this;
}
    
//! ���̑a�s��ɒ萔���|����D
/*!
  \param c	�|����萔
  \return	���̑a�s��
*/
template <class T, bool SYM> SparseMatrix<T, SYM>&
SparseMatrix<T, SYM>::operator *=(value_type c)
{
    std::transform(_values.begin(), _values.end(), _values.begin(),
		   std::bind2nd(std::multiplies<T>(), c));
    return *this;
}
    
//! ���̑a�s���萔�Ŋ���D
/*!
  \param c	�|����萔
  \return	���̑a�s��
*/
template <class T, bool SYM> SparseMatrix<T, SYM>&
SparseMatrix<T, SYM>::operator /=(value_type c)
{
    std::transform(_values.begin(), _values.end(), _values.begin(),
		   std::bind2nd(std::divides<T>(), c));
    return *this;
}
    
//! ���̑a�s��ɑ��̑a�s��𑫂��D
/*!
  2�̑a�s��͓���̍\���������˂΂Ȃ�Ȃ��D
  \param A			�����a�s��
  \return			���̑a�s��
  \throw std::invalid_argument	2�̑a�s��̍\������v���Ȃ��ꍇ�ɑ��o
*/
template <class T, bool SYM> SparseMatrix<T, SYM>&
SparseMatrix<T, SYM>::operator +=(const SparseMatrix& A)
{
    if (nrow()	    != A.nrow()						||
	nelements() != A.nelements()					||
	!equal(_rowIndex.begin(), _rowIndex.end(), A._rowIndex.begin())	||
	!equal(_columns.begin(),  _columns.end(),  A._columns.begin()))
	throw std::invalid_argument("TU::SparseMatrix<T, SYM>::operator +=(): structure mismatch!");
    std::transform(_values.begin(), _values.end(),
		   A._values.begin(), _values.begin(), std::plus<T>());
    return *this;
}
    
//! ���̑a�s�񂩂瑼�̑a�s��������D
/*!
  2�̑a�s��͓���̍\���������˂΂Ȃ�Ȃ��D
  \param A			�����a�s��
  \return			���̑a�s��
  \throw std::invalid_argument	2�̑a�s��̍\������v���Ȃ��ꍇ�ɑ��o
*/
template <class T, bool SYM> SparseMatrix<T, SYM>&
SparseMatrix<T, SYM>::operator -=(const SparseMatrix& A)
{
    if (nrow()	    != A.nrow()						||
	nelements() != A.nelements()					||
	!equal(_rowIndex.begin(), _rowIndex.end(), A._rowIndex.begin())	||
	!equal(_columns.begin(),  _columns.end(),  A._columns.begin()))
	throw std::invalid_argument("TU::SparseMatrix<T, SYM>::operator -=(): structure mismatch!");
    std::transform(_values.begin(), _values.end(),
		   A._values.begin(), _values.begin(), std::minus<T>());
    return *this;
}

//! ���̑a�s��Ƒ��̑a�s��̘a���v�Z����D
/*!
  2�̑a�s��͓���̃T�C�Y�������˂΂Ȃ�Ȃ��D
  \param A	�����a�s��
  \return	2�̑a�s��̘a
*/
template <class T, bool SYM> inline SparseMatrix<T, SYM>
SparseMatrix<T, SYM>::operator +(const SparseMatrix& A) const
{
    return binary_op(A, std::plus<T>());
}
    
//! ���̑a�s��Ƒ��̑a�s��̍����v�Z����D
/*!
  2�̑a�s��͓���̃T�C�Y�������˂΂Ȃ�Ȃ��D
  \param A	�����a�s��
  \return	2�̑a�s��̍�
*/
template <class T, bool SYM> inline SparseMatrix<T, SYM>
SparseMatrix<T, SYM>::operator -(const SparseMatrix& A) const
{
    return binary_op(A, std::minus<T>());
}
    
//! �a�s��ƃx�N�g���̐�
/*!
  \param v	�x�N�g��
  \return	���ʂ̃x�N�g��
*/
template <class T, bool SYM> template <class S, class B> Vector<T>
SparseMatrix<T, SYM>::operator *(const Vector<S, B>& v) const
{
    v.check_dim(ncol());
    
    Vector<T>	a(nrow());
    for (u_int i = 0; i < nrow(); ++i)
    {
	if (SYM)
	{
	    for (u_int j = 0; j < i; ++j)
	    {
		const int	n = index(i, j);
		if (n >= 0)
		    a[i] += _values[n] * v[j];
	    }
	}
	
	for (u_int n = _rowIndex[i] - 1; n < _rowIndex[i+1] - 1; ++n)
	    a[i] += _values[n] * v[_columns[n] - 1];
    }

    return a;
}

//! ���̑a�s��ɉE���玩�g�̓]�u���|�����s���Ԃ��D
/*!
  \return	���ʂ��i�[�����a�Ώ̍s��
*/
template <class T, bool SYM> SparseMatrix<T, true>
SparseMatrix<T, SYM>::compose() const
{
    SparseMatrix<T, true>	AAt;	// ���ʂ��i�[����a�Ώ̍s��

    AAt.beginInit();
    for (u_int i = 0; i < nrow(); ++i)
    {
	AAt.setRow();
	
	for (u_int j = i; j < nrow(); ++j)
	{
	    T	val;
	    if (inner_product(*this, i, j, val))
		AAt.setCol(j, val);
	}
    }
    AAt.endInit();
    
    return AAt;
}
    
//! ���̑a�s��ɉE����^����ꂽ�a�Ώ̍s��Ǝ��g�̓]�u���|�����s���Ԃ��D
/*!
  \param W			�a�Ώ̍s��
  \return			���ʂ��i�[�����a�Ώ̍s��
  \throw std::invalid_argument	���̑a�s��̗񐔂� W �̍s������v���Ȃ�
				�ꍇ�ɑ��o
*/
template <class T, bool SYM> SparseMatrix<T, true>
SparseMatrix<T, SYM>::compose(const SparseMatrix<T, true>& W) const
{
    if (ncol() != W.nrow())
	throw std::invalid_argument("TU::SparseMatrix<T, SYM>::compose(): mismatched dimension!");

    SparseMatrix<T, false>	AW;
    AW.beginInit();
    for (u_int i = 0; i < nrow(); ++i)
    {
	std::cerr << i << '/' << nrow() << std::endl;
	
	AW.setRow();

	for (u_int j = 0; j < W.nrow(); ++j)
	{
	    T	val;
	    if (inner_product(W, i, j, val))
		AW.setCol(j, val);
	}
    }
    AW.endInit();

    SparseMatrix<T, true>	AWAt;
    AWAt.beginInit();
    for (u_int i = 0; i < AW.nrow(); ++i)
    {
	AWAt.setRow();

	for (u_int j = i; j < nrow(); ++j)
	{
	    T	val;
	    if (AW.inner_product(*this, i, j, val))
		AWAt.setCol(j, val);
	}
    }
    AWAt.endInit();

    return AWAt;
}

/*
 * ----------------------- �u���b�N���Z -----------------------------
 */
//! �a�s��̍s���̖��ȕ������x�N�g���Ƃ��Ď��o���D
/*!
  \param i	�N�_�̍s�ԍ�
  \param j	�N�_�̗�ԍ�
  \param d	���o��������
  \return	���o������������ׂ��x�N�g��
*/
template <class T, bool SYM> Vector<T>
SparseMatrix<T, SYM>::operator ()(u_int i, u_int j, u_int d) const
{
    Vector<T>	v(d);
    const int	n = index(i, j, true);
    const T*	p = &_values[n];
    u_int	dj = 0;
    if (SYM)
    {
	for (u_int je = std::min(i, j + v.dim()); j + dj < je; ++dj)
	{
#ifdef LIBTUTOOLS_DEBUG
	    if (p != &_values[index(i, j+dj, true)])
		throw std::invalid_argument("TU::SparseMatrix<T, SYM>::operator (): accessing non-existent element!");
#endif
	    v[dj] = *p;
	    p += (nelements(j + dj) - 1);
	}
    }
    for (; dj < v.dim(); ++dj)
    {
#ifdef LIBTUTOOLS_DEBUG
	if (p != &_values[index(i, j+dj, true)])
	    throw std::invalid_argument("TU::SparseMatrix<T, SYM>::operator (): accessing non-existent element!");
#endif
	v[dj] = *p++;
    }
    
    return v;
}

//! �a�s�񒆂̖��ȃu���b�N���s��Ƃ��Ď��o���D
/*!
  \param i	�N�_�̍s�ԍ�
  \param j	�N�_�̗�ԍ�
  \param r	���o���s��
  \param c	���o����
  \return	���o������������ׂ��s��
*/
template <class T, bool SYM> Matrix<T>
SparseMatrix<T, SYM>::operator ()(u_int i, u_int j,
				  u_int r, u_int c) const
{
    Matrix<T>	M(r, c);
    for (u_int di = 0; di < M.nrow(); ++di)
	M[di] = (*this)(i + di, j, M.ncol());

    return M;
}

//! �a�s�񒆂̍s�̖��ȕ������x�N�g���Ƃ݂Ȃ��ė^����ꂽ�x�N�g����������D
/*!
  (i, j)�������N�_�Ƃ���A�������ɗ^����ꂽ�x�N�g����������D
  \param i	�N�_�̍s�ԍ�
  \param j	�N�_�̗�ԍ�
  \param v	�������x�N�g��
  \return	���̑a�Ώ̍s��
*/
template <class T, bool SYM> template <class S, class B>
inline SparseMatrix<T, SYM>&
SparseMatrix<T, SYM>::assign(u_int i, u_int j, const Vector<S, B>& v)
{
    return apply(i, j, Assign<S>(), v);
}
    
//! �a�s�񒆂̖��ȃu���b�N���s��Ƃ݂Ȃ��ė^����ꂽ�s���������D
/*!
  (i, j)�������N�_�Ƃ���A�������ɗ^����ꂽ�s���������D
  \param i	�N�_�̍s�ԍ�
  \param j	�N�_�̗�ԍ�
  \param M	�������s��
  \return	���̑a�Ώ̍s��
*/
template <class T, bool SYM> template <class S, class B, class R>
inline SparseMatrix<T, SYM>&
SparseMatrix<T, SYM>::assign(u_int i, u_int j, const Matrix<S, B, R>& M)
{
    return apply(i, j, Assign<S>(), M);
}
    
//! �^����ꂽ�x�N�g���̊e�������w�肳�ꂽ�������N�_�Ƃ��ēK�p����D
/*!
  \param i	�N�_�̍s�ԍ�
  \param j	�N�_�̗�ԍ�
  \param f	T�^�CS�^�̈������Ƃ�T�^�̒l��Ԃ�2�����Z�q
  \param v	���̊e������f�̑�2�����ƂȂ�x�N�g��
  \return	���̑a�Ώ̍s��
*/
template <class T, bool SYM> template <class OP, class S, class B>
SparseMatrix<T, SYM>&
SparseMatrix<T, SYM>::apply(u_int i, u_int j, OP op, const Vector<S, B>& v)
{
    T*		p = &(*this)(i, j);
    u_int	dj = 0;
    if (SYM)
    {
	for (u_int je = std::min(i, j + v.dim()); j + dj < je; ++dj)
	{
#ifdef LIBTUTOOLS_DEBUG
	    if (p != &_values[index(i, j+dj, true)])
		throw std::invalid_argument("TU::SparseMatrix<T, SYM>::apply(): accessing non-existent element!");
#endif
	    *p = op(*p, v[dj]);
	    p += (nelements(j + dj) - 1);
	}
    }
    for (; dj < v.dim(); ++dj)
    {
#ifdef LIBTUTOOLS_DEBUG
	if (p != &_values[index(i, j+dj, true)])
	    throw std::invalid_argument("TU::SparseMatrix<T, SYM>::apply(): accessing non-existent element!");
#endif
	*p = op(*p, v[dj]);
	++p;
    }
    
    return *this;
}
    
//! �^����ꂽ�s��̊e�������w�肳�ꂽ�������N�_�Ƃ��ēK�p����D
/*!
  \param i	�N�_�̍s�ԍ�
  \param j	�N�_�̗�ԍ�
  \param f	T�^�CS�^�̈������Ƃ�T�^�̒l��Ԃ�2�����Z�q
  \param M	���̊e������f�̑�2�����ƂȂ�s��
  \return	���̑a�Ώ̍s��
*/
template <class T, bool SYM> template <class OP, class S, class B, class R>
SparseMatrix<T, SYM>&
SparseMatrix<T, SYM>::apply(u_int i, u_int j, OP op, const Matrix<S, B, R>& M)
{
    if (SYM && i > j)
    {
	const Matrix<S>	Mt = M.trns();
	
	for (u_int dj = 0; dj < Mt.nrow(); ++dj)
	{
	    const u_int	di = std::max(j + dj, i) - i;
	    apply(j + dj, i + di, op, Mt[dj](di, Mt.ncol() - di));
	}
    }
    else
    {
	for (u_int di = 0; di < M.nrow(); ++di)
	{
	    const u_int	dj = (SYM ? std::max(i + di, j) - j : 0);
	    apply(i + di, j + dj, op, M[di](dj, M.ncol() - dj));
	}
    }
    
    return *this;
}
    
/*
 * ----------------------- �A���ꎟ������ -----------------------------
 */
//! ���̍s����W���Ƃ���A���ꎟ�������������D
/*!
  MKL direct sparse solver�ɂ����
  \f$\TUvec{A}{}\TUvec{x}{} = \TUvec{b}{}\f$�������D
  \param b			�x�N�g��
  \return			���x�N�g��
  \throw std::logic_error	���̑a�s�񂪐����łȂ��ꍇ�ɑ��o
  \throw std::runtime_error	MKL direct sparse solver ���G���[��Ԃ���
				�ꍇ�ɑ��o
*/
template <class T, bool SYM> Vector<T>
SparseMatrix<T, SYM>::solve(const Vector<T>& b) const
{
    b.check_dim(ncol());
    
    if (nrow() != ncol())
	throw std::logic_error("TU::SparseMatrix<T, SYM>::solve(): not a square matrix!");

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
    _INTEGER_t		mtype  = (SYM ? -2 : 11);	// ���Ώ́^����Ώ̍s��
    _INTEGER_t		phase  = 13;	// �s��̉�͂��甽���ɂ��ז����܂�
    _INTEGER_t		n      = nrow();// �A���ꎟ�������̎���
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
	throw std::runtime_error("TU::SparseMatrix<T, SYM>::solve(): PARDISO failed!");

  // pardiso ���Ŏg�p�����S���������������D
    phase = -1;
    PARDISO(&pt[0], &maxfct, &mnum, &mtype, &phase, &n, (void*)&_values[0],
	    (_INTEGER_t*)&_rowIndex[0], (_INTEGER_t*)&_columns[0],
	    &perm[0], &nrhs, iparm, &msglvl, (void*)&b[0], &x[0], &error);
    if (error != 0)
	throw std::runtime_error("TU::SparseMatrix<T, SYM>::solve(): PARDISO failed to release memory!");

    return x;
}

/*
 * ----------------------- �L�����̃`�F�b�N -----------------------------
 */
//! ���̍s��̑S�Ă̐������L���ł��邩���ׂ�D
/*!
  \return	 �S�ėL���Ȃ�true, �����łȂ����false
*/
template <class T, bool SYM> bool
SparseMatrix<T, SYM>::isfinite() const
{
    for (u_int n = 0; n < _values.size(); ++n)
#ifdef __INTEL_COMPILER
	if (!::isfinite(_values[n]))
#else
	if (_values[n] != _values[n])
#endif
	    return false;
    return true;
}

/*
 * ----------------------- ���o�� -----------------------------
 */
//! ���̓X�g���[������a�s���ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, bool SYM> std::istream&
SparseMatrix<T, SYM>::get(std::istream& in)
{
    beginInit();

    for (char c; (c = skipws(in)) != '\n'; )
    {
	in.putback(c);
	
	setRow();

	for (u_int col = 0; (c = skipws(in)) != '\n'; ++col)
	    if (isdigit(c) || (c == '+') || (c == '-'))	// �����Ȃ��...
	    {
		in.putback(c);		// �����߂���
		T	val;
		in >> val;		// ���炽�߂Đ����Ƃ��ēǂݒ����D
		setCol(col, val);	// ��ԍ��ƒl���Z�b�g����D
	    }
    }

    endInit();
    
    return in;
}

//! �o�̓X�g���[���֑a�s��������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, bool SYM> std::ostream&
SparseMatrix<T, SYM>::put(std::ostream& out) const
{
    u_int	n = 0;
    for (u_int i = 0; i < nrow(); ++i)
    {
	u_int	j = 0;
	
	if (SYM)
	{
	    for (; j < i; ++j)
		out << " *";
	}

	for (; j < ncol(); ++j)
	{
	    if ((n < _rowIndex[i+1] - 1) && (j == _columns[n] - 1))
		out << ' ' << _values[n++];
	    else
		out << " _";
	}
	out << std::endl;
    }

    return out;
}

/*
 * ----------------------- private members -----------------------------
 */
//! ���̑a�s��Ƒ��̑a�s��̊ԂŐ�������2�����Z���s���D
/*!
  \param B			��������̑a�s��
  \return			2�̑a�s��Ԃ̐�������2�����Z�œ�����a�s��
  \throw std::invalid_argument	2�̑a�s��̃T�C�Y����v���Ȃ��ꍇ�ɑ��o
*/
template <class T, bool SYM> template <class OP> SparseMatrix<T, SYM>
SparseMatrix<T, SYM>::binary_op(const SparseMatrix& B, OP op) const
{
    if ((nrow() != B.nrow()) || (ncol() != B.ncol()))
	throw std::invalid_argument("SparseMatrix<T, SYM>::binary_op(): two matrices must have equal sizes!");

    SparseMatrix	S;

    S.beginInit();
    for (u_int i = 0; i < nrow(); ++i)
    {
	S.setRow();

	for (u_int m = _rowIndex[i], n = B._rowIndex[i]; ; )
	{
	    const u_int	j = (m <   _rowIndex[i+1] ?   _columns[m-1] - 1
						  :   ncol());
	    const u_int	k = (n < B._rowIndex[i+1] ? B._columns[n-1] - 1
						  : B.ncol());
	    
	    if (j == k)
	    {
		if (j == ncol())
		    break;
		
		S.setCol(j, op(_values[m-1], B._values[n-1]));
		++m;
		++n;
	    }
	    else if (j < k)
	    {
		S.setCol(j, op(_values[m-1], T(0)));
		++m;
	    }
	    else
	    {
		S.setCol(k, op(T(0), B._values[n-1]));
		++n;
	    }
	}
    }
    S.endInit();

    return S;
}
    
//! ���̑a�s��Ɨ^����ꂽ�a�s�񂩂炻�ꂼ��1�s�����o���C�����̓��ς����߂�D
/*!
  \param B			����1�̑a�s��
  \param i			���̑a�s��̍s�ԍ�
  \param j			B �̍s�ԍ�
  \param val			���̑a�s��̑�i�s�� B �̑�j�s�̓��ς̒l��
				�Ԃ����
  \return			���̑a�s��̑�i�s�� B �̑�j�s����ԍ���
				���Ȃ��Ƃ�1���L�����true,
				�����łȂ���� false
  \throw std::invalid_argument	���̑a�s��� B �̗񐔂���v���Ȃ��ꍇ�ɑ��o
*/
template <class T, bool SYM> template <bool SYM2> bool
SparseMatrix<T, SYM>::inner_product(const SparseMatrix<T, SYM2>& B,
				    u_int i, u_int j, T& val) const
{
    if (ncol() != B.ncol())
	throw std::invalid_argument("inner_product(): mismatched dimension!");
    
    bool	exist = false;
    val = T(0);

    if (SYM)
    {
	for (u_int col = 0; col < i; ++col)
	{
	    int	m, n;
	    if ((m = index(i, col)) >= 0 && (n = B.index(j, col)) >= 0)
	    {
		exist = true;
		val += _values[m] * B._values[n];
	    }
	}
    }

    for (u_int m = _rowIndex[i] - 1; m < _rowIndex[i+1] - 1; ++m)
    {
	const int	n = B.index(j, _columns[m] - 1);
	if (n >= 0)
	{
	    exist = true;
	    val += _values[m] * B._values[n];
	}
    }

    return exist;
}

//! �w�肳�ꂽ�s�Ɨ�ɑΉ����� #_values ��index��Ԃ��D
/*!
  #_rowIndex �Ɠ��l��1�x�[�X��index��Ԃ��̂ŁC���ۂ� #_values ��
  �A�N�Z�X����ۂɂ͕Ԃ��ꂽ�l����1�������K�v������D
  \param i			�s�ԍ�
  \param j			��ԍ�
  \param throwExcept		(i, j)���������݂��Ȃ��Ƃ��C
				true�Ȃ�� std::invalid_argument �𑗏o�C
				false�Ȃ��-1��Ԃ�
  \return			(i, j)������index(0�x�[�X)
  \throw std::invalid_argument	���̍s��(i, j)������������ throwExcept ��
				true �̏ꍇ�ɑ��o
*/
template <class T, bool SYM> inline int
SparseMatrix<T, SYM>::index(u_int i, u_int j, bool throwExcept) const
{
    if (SYM && i > j)
	std::swap(i, j);

    if (i >= nrow())
	throw std::invalid_argument("TU::SparseMatrix<T, SYM>::index(): invalid row index!");
    
    ++j;				// ��ԍ���1�x�[�X�ɒ����D

  // �w�肳�ꂽ��ԍ��ɑΉ����鐬�������̍s�ɂ��邩2���@�ɂ���Ē��ׂ�D
    for (u_int low = _rowIndex[i]-1, high = _rowIndex[i+1]-1; low != high; )
    {
	u_int	mid = (low + high) / 2;

	if (j < _columns[mid])
	    high = mid;
	else if (j > _columns[mid])
	    low = mid + 1;
	else
	    return mid;
    }

  // �w�肳�ꂽ�������݂���Ȃ������̂ŁC��O�𓊂��邩-1��Ԃ��D
    if (throwExcept)
	throw std::invalid_argument("TU::SparseMatrix<T, SYM>::index(): non-existent element!");
    return -1;
}

template<> inline int
SparseMatrix<float,  false>::pardiso_precision()	{return 1;}
template<> inline int
SparseMatrix<float,  true> ::pardiso_precision()	{return 1;}
template<> inline int
SparseMatrix<double, false>::pardiso_precision()	{return 0;}
template<> inline int
SparseMatrix<double, true> ::pardiso_precision()	{return 0;}

//! ��󔒕����܂��͉��s�ɒB����܂ŃX�g���[����ǂݐi�߂�D
/*!
  ���B������󔒕����������͉��s�̓X�g���[���Ɏc����Ȃ��D
  \param in	���̓X�g���[��
  \return	��󔒕����ɒB�����炻�̕�����Ԃ��C
		���s���X�g���[���I�[�ɒB��������s��Ԃ��D
*/
template <class T, bool SYM> char
SparseMatrix<T, SYM>::skipws(std::istream& in)
{
    for (char c; in.get(c); )
	if (!isspace(c) || (c == '\n'))	// ��󔒕����������͉��s�Ȃ��...
	    return c;			// ���̕�����Ԃ��D

    return '\n';		// �t�@�C���I�[�Ȃ�Ή��s��Ԃ��D
}

/************************************************************************
*  global functions							*
************************************************************************/
//! �x�N�g���Ƒa�s��̐�
/*!
  \param v	�x�N�g��
  \param A	�a�s��
  \return	���ʂ̃x�N�g��
*/
template <class S, class B, class T2, bool SYM2> Vector<S>
operator *(const Vector<S, B>& v, const SparseMatrix<T2, SYM2>& A)
{
    v.check_dim(A.nrow());
    
    Vector<S>	a(A.ncol());
    for (u_int j = 0; j < A.ncol(); ++j)
    {
	for (u_int i = 0; i < (SYM2 ? j : A.nrow()); ++i)
	{
	    const int	n = A.index(i, j);
	    if (n >= 0)
		a[j] += v[i] * A._values[n];
	}
	if (SYM2)
	{
	    for (u_int n = A._rowIndex[j] - 1; n < A._rowIndex[j+1] - 1; ++n)
		a[j] += v[A._columns[n] - 1] * A._values[n];
	}
    }

    return a;
}

//! ���̓X�g���[������a�s���ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param A	�a�s��̓ǂݍ��ݐ�
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, bool SYM> inline std::istream&
operator >>(std::istream& in, SparseMatrix<T, SYM>& A)
{
    return A.get(in >> std::ws);
}

//! �o�̓X�g���[���֑a�s��������o��(ASCII)�C����ɉ��s�R�[�h���o�͂���D
/*!
  \param out	�o�̓X�g���[��
  \param A	�����o���a�s��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, bool SYM> inline std::ostream&
operator <<(std::ostream& out, const SparseMatrix<T, SYM>& A)
{
    return A.put(out) << std::endl;
}

}

#endif	// ! __TUSparseMatrixPP_h
