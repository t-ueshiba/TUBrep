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
 *  $Id: Array++.h,v 1.35 2011-07-07 07:51:04 ueshiba Exp $
 */
#ifndef __TUArrayPP_h
#define __TUArrayPP_h

#include <iterator>
#include <iostream>
#include <stdexcept>
#include "TU/types.h"
#ifdef __INTEL_COMPILER
  #include <mmintrin.h>
#endif

namespace TU
{
/************************************************************************
*  class Buf<T>								*
************************************************************************/
//! �ϒ��o�b�t�@�N���X
/*!
  �P�ƂŎg�p���邱�Ƃ͂Ȃ��C#TU::Array<T, B>�܂���#TU::Array2<T, B, R>��
  ��2�e���v���[�g�����Ɏw�肷�邱�Ƃɂ���āC�����̊��N���X�Ƃ��Ďg���D
  \param T	�v�f�̌^
*/
template <class T>
class Buf
{
  public:
    typedef T			value_type;	//!< �v�f�̌^
    typedef value_type&		reference;	//!< �v�f�ւ̎Q��
    typedef const value_type&	const_reference;//!< �萔�v�f�ւ̎Q��
    typedef value_type*		pointer;	//!< �v�f�ւ̃|�C���^
    typedef const value_type*	const_pointer;	//!< �萔�v�f�ւ̃|�C���^
    
  public:
    explicit Buf(u_int siz=0)					;
    Buf(pointer p, u_int siz)					;
    Buf(const Buf& b)						;
    Buf&		operator =(const Buf& b)		;
    ~Buf()							;

    pointer		ptr()					;
    const_pointer	ptr()				const	;
    size_t		size()				const	;
    bool		resize(u_int siz)			;
    void		resize(pointer p, u_int siz)		;
    static u_int	stride(u_int siz)			;
    std::istream&	get(std::istream& in, u_int m=0)	;
    std::ostream&	put(std::ostream& out)		const	;
    
  private:
    u_int	_size;		// the number of elements in the buffer
    pointer	_p;		// pointer to the buffer area
    u_int	_shared	  :  1;	// buffer area is shared with other object
    u_int	_capacity : 31;	// buffer capacity (unit: element, >= _size)
};

//! �w�肵���v�f���̃o�b�t�@�𐶐�����D
/*!
  \param siz	�v�f��
*/
template <class T> inline
Buf<T>::Buf(u_int siz)
    :_size(siz), _p(new T[_size]), _shared(0), _capacity(_size)
{
}

//! �O���̗̈�Ɨv�f�����w�肵�ăo�b�t�@�𐶐�����D
/*!
  \param p	�O���̈�ւ̃|�C���^
  \param siz	�v�f��
*/
template <class T> inline
Buf<T>::Buf(pointer p, u_int siz)
    :_size(siz), _p(p), _shared(1), _capacity(_size)
{
}
    
//! �R�s�[�R���X�g���N�^
template <class T>
Buf<T>::Buf(const Buf<T>& b)
    :_size(b._size), _p(new T[_size]), _shared(0), _capacity(_size)
{
    for (u_int i = 0; i < _size; ++i)
	_p[i] = b._p[i];
}

//! �W��������Z�q
template <class T> Buf<T>&
Buf<T>::operator =(const Buf<T>& b)
{
    if (this != &b)
    {
	resize(b._size);
	for (u_int i = 0; i < _size; ++i)
	    _p[i] = b._p[i];
    }
    return *this;
}

//! �f�X�g���N�^
template <class T> inline
Buf<T>::~Buf()
{
    if (!_shared)
	delete [] _p;
}
    
//! �o�b�t�@���g�p��������L���̈�ւ̃|�C���^��Ԃ��D
template <class T> inline typename Buf<T>::pointer
Buf<T>::ptr()
{
    return _p;
}

//! �o�b�t�@���g�p��������L���̈�ւ̃|�C���^��Ԃ��D
template <class T> inline typename Buf<T>::const_pointer
Buf<T>::ptr() const
{
    return _p;
}
    
//! �o�b�t�@�̗v�f����Ԃ��D
template <class T> inline size_t
Buf<T>::size() const
{
    return _size;
}
    
//! �o�b�t�@�̗v�f����ύX����D
/*!
  �������C���̃I�u�W�F�N�g�ƋL���̈�����L���Ă���o�b�t�@�̗v�f����
  �ύX���邱�Ƃ͂ł��Ȃ��D
  \param siz			�V�����v�f��
  \return			siz�����̗v�f�������傫�����true�C����
				�łȂ����false
  \throw std::logic_error	�L���̈�𑼂̃I�u�W�F�N�g�Ƌ��L���Ă���ꍇ
				�ɑ��o
*/
template <class T> bool
Buf<T>::resize(u_int siz)
{
    if (_size == siz)
	return false;
    
    if (_shared)
	throw std::logic_error("Buf<T>::resize: cannot change size of shared buffer!");

    const u_int	old_size = _size;
    _size = siz;
    if (_capacity < _size)
    {
	delete [] _p;
	_p = new T[_size];
	_capacity = _size;
    }
    return _size > old_size;
}

//! �o�b�t�@�������Ŏg�p����L���̈���w�肵�����̂ɕύX����D
/*!
  \param p	�V�����L���̈�ւ̃|�C���^
  \param siz	�V�����v�f��
*/
template <class T> inline void
Buf<T>::resize(pointer p, u_int siz)
{
    _size = siz;
    if (!_shared)
	delete [] _p;
    _p = p;
    _shared = 1;
    _capacity = _size;
}

//! �L���̈��align���邽�߂ɕK�v�ȗv�f����Ԃ��D
/*!
  �K�v�ȋL���e�ʂ��o�b�t�@�ɂ���Č��܂����̒l�̔{���ɂȂ�悤�C�^����ꂽ
  �v�f�����J��グ��D
  \param siz	�v�f��
  \return	align���ꂽ�v�f��
*/
template <class T> inline u_int
Buf<T>::stride(u_int siz)
{
    return siz;
}
    
//! ���̓X�g���[������w�肵���ӏ��ɔz���ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param m	�ǂݍ��ݐ�̐擪���w�肷��index
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T> std::istream&
Buf<T>::get(std::istream& in, u_int m)
{
    const u_int	BufSiz = (sizeof(T) < 2048 ? 2048 / sizeof(T) : 1);
    T* const	tmp = new T[BufSiz];
    u_int	n = 0;
    
    while (n < BufSiz)
    {
	char	c;
	while (in.get(c))		// skip white spaces other than '\n'
	    if (!isspace(c) || c == '\n')
		break;

	if (!in || c == '\n')
	{
	    resize(m + n);
	    break;
	}

	in.putback(c);
	in >> tmp[n++];
    }
    if (n == BufSiz)
	get(in, m + n);

    for (u_int i = 0; i < n; ++i)
	_p[m + i] = tmp[i];

    delete [] tmp;
    
    return in;
}

//! �o�̓X�g���[���ɔz��������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T> std::ostream&
Buf<T>::put(std::ostream& out) const
{
    for (u_int i = 0; i < _size; )
	out << ' ' << _p[i++];
    return out;
}

#ifdef __INTEL_COMPILER
/************************************************************************
*  class AlignedBuf<T>							*
************************************************************************/
//! �L���̈�̃A�h���X��16byte�̔{���ɂȂ�悤align���ꂽ�ϒ��o�b�t�@�N���X
/*!
  �P�ƂŎg�p���邱�Ƃ͂Ȃ��C#TU::Array<T, B>�܂���#TU::Array2<T, B>��
  ��2�e���v���[�g�����Ɏw�肷�邱�Ƃɂ���āC�����̊��N���X�Ƃ��Ďg���D
  \param T	�v�f�̌^
*/
template <class T>
class AlignedBuf : public Buf<T>
{
  private:
    typedef Buf<T>				super;

  public:
  //! �v�f�̌^
    typedef typename super::value_type		value_type;
  //! �v�f�ւ̎Q��
    typedef typename super::reference		reference;
  //! �萔�v�f�ւ̎Q��
    typedef typename super::const_reference	const_reference;
  //! �v�f�ւ̃|�C���^
    typedef typename super::pointer		pointer;
  //! �萔�v�f�ւ̃|�C���^    
    typedef typename super::const_pointer	const_pointer;

  public:
    explicit AlignedBuf(u_int siz=0)				;
    AlignedBuf(const AlignedBuf& b)				;
    AlignedBuf&		operator =(const AlignedBuf& b)		;
    ~AlignedBuf()						;

    using		super::ptr;
    using		super::size;
    
    bool		resize(u_int siz)			;
    static u_int	stride(u_int siz)			;
    
  private:
    static pointer	memalloc(u_int siz)			;
    static void		memfree(pointer p)			;
    
    enum		{ALIGN = 16};
    class LCM		//! sizeof(T)��ALIGN�̍ŏ����{��
    {
      public:
	LCM()							;
			operator u_int()		const	{return _val;}
      private:
	u_int		_val;
    };
    static const LCM	_lcm;
};

//! �w�肵���v�f���̃o�b�t�@�𐶐�����D
/*!
  \param siz	�v�f��
*/
template <class T> inline
AlignedBuf<T>::AlignedBuf(u_int siz)
    :super(memalloc(siz), siz)
{
}

//! �R�s�[�R���X�g���N�^
template <class T>
AlignedBuf<T>::AlignedBuf(const AlignedBuf<T>& b)
    :super(memalloc(b.size()), b.size())
{
    super::operator =(b);
}

//! �W��������Z�q
template <class T> AlignedBuf<T>&
AlignedBuf<T>::operator =(const AlignedBuf<T>& b)
{
    resize(b.size());		// Buf<T>::resize(u_int)�͎g���Ȃ��D
    super::operator =(b);
    return *this;
}

//! �f�X�g���N�^
template <class T> inline
AlignedBuf<T>::~AlignedBuf()
{
    memfree(ptr());
}
    
//! �o�b�t�@�̗v�f����ύX����D
/*!
  \param siz	�V�����v�f��
  \return	siz�����̗v�f���Ɠ��������true�C�����łȂ����false
*/
template <class T> inline bool
AlignedBuf<T>::resize(u_int siz)
{
    if (siz == size())
	return false;

    memfree(ptr());
    super::resize(memalloc(siz), siz);
    return true;
}

//! �L���̈��align���邽�߂ɕK�v�ȗv�f����Ԃ��D
/*!
  �K�v�ȋL���e�ʂ�16byte�̔{���ɂȂ�悤�C�^����ꂽ�v�f�����J��グ��D
  \param siz	�v�f��
  \return	align���ꂽ�v�f��
*/
template <class T> inline u_int
AlignedBuf<T>::stride(u_int siz)
{
  // _lcm * m >= sizeof(T) * siz �Ȃ�ŏ��� m �����߂�D
    const u_int	m = (sizeof(T)*siz + _lcm - 1) / _lcm;
    return (_lcm * m) / sizeof(T);
}

template <class T> inline typename AlignedBuf<T>::pointer
AlignedBuf<T>::memalloc(u_int siz)
{
    void*	p = _mm_malloc(sizeof(T)*siz, ALIGN);
    if (p == 0)
	throw std::runtime_error("AlignedBuf<T>::memalloc(): failed to allocate memory!!");
    return pointer(p);
}

template <class T> inline void
AlignedBuf<T>::memfree(pointer p)
{
    if (p != 0)
	_mm_free(p);
}

template <class T>
AlignedBuf<T>::LCM::LCM()
    :_val(ALIGN * sizeof(T))
{
  // sizeof(T)��ALIGN�̍ő����(GCD)�����߂�D
    u_int	gcd = ALIGN;
    for (u_int m = sizeof(T); m > 0; m -= gcd)
	if (m < gcd)
	    std::swap(m, gcd);

  // sizeof(T)��ALIGN�̍ŏ����{��(LCM)
    _val /= gcd;
}

template <class T> const AlignedBuf<T>::LCM	AlignedBuf<T>::_lcm;

#endif	// __INTEL_COMPILER
/************************************************************************
*  class FixedSizedBuf<T, D>						*
************************************************************************/
//! �萔�T�C�Y�̃o�b�t�@�N���X
/*!
  �P�ƂŎg�p���邱�Ƃ͂Ȃ��C#TU::Array<T, B>�̑�2�e���v���[�g�����Ɏw�肷��
  ���Ƃɂ����#TU::Array<T, B>�̊��N���X�Ƃ��Ďg���D
  \param T	�v�f�̌^
  \param D	�o�b�t�@���̗v�f��
*/
template <class T, size_t D>
class FixedSizedBuf
{
  public:
    typedef T			value_type;	//!< �v�f�̌^
    typedef value_type&		reference;	//!< �v�f�ւ̎Q��
    typedef const value_type&	const_reference;//!< �萔�v�f�ւ̎Q��
    typedef value_type*		pointer;	//!< �v�f�ւ̃|�C���^
    typedef const value_type*	const_pointer;	//!< �萔�v�f�ւ̃|�C���^

  public:
    explicit FixedSizedBuf(u_int siz=D)				;
    FixedSizedBuf(pointer p, u_int siz)				;
    FixedSizedBuf(const FixedSizedBuf& b)			;
    FixedSizedBuf&	operator =(const FixedSizedBuf& b)	;
    
    pointer		ptr()					;
    const_pointer	ptr()				const	;
    static size_t	size()					;
    static bool		resize(u_int siz)			;
    void		resize(pointer p, u_int siz)		;
    static u_int	stride(u_int siz)			;
    std::istream&	get(std::istream& in)			;
    std::ostream&	put(std::ostream& out)		const	;

  private:
    T			_p[D];				// D-sized buffer
};

//! �o�b�t�@�𐶐�����D
/*!
  \param siz			�v�f��
  \throw std::logic_error	siz���e���v���[�g�p�����[�^D�Ɉ�v���Ȃ��ꍇ��
				���o
*/
template <class T, size_t D> inline
FixedSizedBuf<T, D>::FixedSizedBuf(u_int siz)
{
    resize(siz);
}

//! �O���̗̈�Ɨv�f�����w�肵�ăo�b�t�@�𐶐�����i�_�~�[�֐��j�D
/*!
  ���ۂ̓o�b�t�@���g�p����L���̈�͌Œ肳��Ă��ĕύX�ł��Ȃ��̂ŁC
  ���̊֐��͏�ɗ�O�𑗏o����D
  \throw std::logic_error	���̊֐����Ă΂ꂽ��K�����o
*/
template <class T, size_t D> inline
FixedSizedBuf<T, D>::FixedSizedBuf(pointer p, u_int siz)
{
    throw std::logic_error("FixedSizedBuf<T, D>::FixedSizedBuf(pointer, u_int): cannot specify a pointer to external storage!!");
}

//! �R�s�[�R���X�g���N�^
template <class T, size_t D>
FixedSizedBuf<T, D>::FixedSizedBuf(const FixedSizedBuf<T, D>& b)
{
    for (u_int i = 0; i < D; ++i)
	_p[i] = b._p[i];
}

//! �W��������Z�q
template <class T, size_t D> FixedSizedBuf<T, D>&
FixedSizedBuf<T, D>::operator =(const FixedSizedBuf<T, D>& b)
{
    if (this != &b)
	for (u_int i = 0; i < D; ++i)
	    _p[i] = b._p[i];
    return *this;
}

//! �o�b�t�@���g�p��������L���̈�ւ̃|�C���^��Ԃ��D
template <class T, size_t D> inline typename FixedSizedBuf<T, D>::pointer
FixedSizedBuf<T, D>::ptr()
{
    return _p;
}

//! �o�b�t�@���g�p��������L���̈�ւ̃|�C���^��Ԃ��D
template <class T, size_t D> inline typename FixedSizedBuf<T, D>::const_pointer
FixedSizedBuf<T, D>::ptr() const
{
    return _p;
}
    
//! �o�b�t�@�̗v�f����Ԃ��D
template <class T, size_t D> inline size_t
FixedSizedBuf<T, D>::size()
{
    return D;
}
    
//! �o�b�t�@�̗v�f����ύX����D
/*!
  ���ۂɂ̓o�b�t�@�̗v�f����ύX���邱�Ƃ͂ł��Ȃ��̂ŁC�^����ꂽ�v�f����
  ���̃o�b�t�@�̗v�f���ɓ������ꍇ�̂݁C�ʏ�ǂ���ɂ��̊֐����琧�䂪�Ԃ�D
  \param siz			�V�����v�f��
  \return			���false
  \throw std::logic_error	siz���e���v���[�g�p�����[�^D�Ɉ�v���Ȃ��ꍇ��
				���o
*/
template <class T, size_t D> inline bool
FixedSizedBuf<T, D>::resize(u_int siz)
{
    if (siz != D)
	throw std::logic_error("FixedSizedBuf<T, D>::resize(u_int): cannot change buffer size!!");
    return false;
}
    
//! �o�b�t�@�������Ŏg�p����L���̈���w�肵�����̂ɕύX����D
/*!
  ���ۂɂ̓o�b�t�@�̋L���̈��ύX���邱�Ƃ͂ł��Ȃ��̂ŁC�^����ꂽ�|�C���^
  �Ɨv�f�������̃o�b�t�@�̂����ɓ������ꍇ�̂݁C�ʏ�ǂ���ɂ��̊֐����琧��
  ���Ԃ�D
  \param p			�V�����L���̈�ւ̃|�C���^
  \param siz			�V�����v�f��
  \throw std::logic_error	p�����̃o�b�t�@�̓����L���̈�Ɉ�v���Ȃ����C
				siz���e���v���[�g�p�����[�^D�Ɉ�v���Ȃ��ꍇ��
				���o
*/
template <class T, size_t D> inline void
FixedSizedBuf<T, D>::resize(pointer p, u_int siz)
{
    if (p != _p || siz != D)
	throw std::logic_error("FixedSizedBuf<T, D>::resize(pointer, u_int): cannot specify a potiner to external storage!!");
}
    
//! �L���̈��align���邽�߂ɕK�v�ȗv�f����Ԃ��D
/*!
  �K�v�ȋL���e�ʂ��o�b�t�@�ɂ���Č��܂����̒l�̔{���ɂȂ�悤�C�^����ꂽ
  �v�f�����J��グ��D
  \param siz	�v�f��
  \return	align���ꂽ�v�f��
*/
template <class T, size_t D> inline u_int
FixedSizedBuf<T, D>::stride(u_int siz)
{
    return siz;
}
    
//! ���̓X�g���[������z���ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, size_t D> std::istream&
FixedSizedBuf<T, D>::get(std::istream& in)
{
    for (u_int i = 0; i < D; ++i)
	in >> _p[i];
    return in;
}
    
//! �o�̓X�g���[���ɔz��������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, size_t D> std::ostream&
FixedSizedBuf<T, D>::put(std::ostream& out) const
{
    for (u_int i = 0; i < D; ++i)
	out << ' ' << _p[i];
    return out;
}

/************************************************************************
*  class Array<T, B>							*
************************************************************************/
//! B�^�o�b�t�@�ɂ���Ď��������T�^�I�u�W�F�N�g��1�����z��N���X
/*!
  \param T	�v�f�̌^
  \param B	�o�b�t�@
*/
template <class T, class B=Buf<T> >
class Array : public B
{
  public:
  //! �o�b�t�@�̌^
    typedef B						buf_type;
  //! �v�f�̌^    
    typedef typename buf_type::value_type		value_type;
  //! �v�f�ւ̎Q��
    typedef typename buf_type::reference		reference;
  //! �萔�v�f�ւ̎Q��
    typedef typename buf_type::const_reference		const_reference;
  //! �v�f�ւ̃|�C���^
    typedef typename buf_type::pointer			pointer;
  //! �萔�v�f�ւ̃|�C���^
    typedef typename buf_type::const_pointer		const_pointer;
  //! �����q
    typedef pointer					iterator;
  //! �萔�����q
    typedef const_pointer				const_iterator;
  //! �t�����q    
    typedef std::reverse_iterator<iterator>		reverse_iterator;
  //! �萔�t�����q    
    typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
  //! �|�C���^�Ԃ̍�
    typedef ptrdiff_t					difference_type;
    
  public:
    Array()								;
    explicit Array(u_int d)						;
    Array(pointer p, u_int d)						;
    template <class T2, class B2>
    Array(const Array<T2, B2>& a)					;
    template <class B2>
    Array(Array<T, B2>& a, u_int i, u_int d)				;
    template <class T2, class B2>
    Array&		operator =(const Array<T2, B2>& a)		;
    Array&		operator =(const value_type& c)			;

    iterator			begin()					;
    const_iterator		begin()				const	;
    iterator			end()					;
    const_iterator		end()				const	;
    reverse_iterator		rbegin()				;
    const_reverse_iterator	rbegin()			const	;
    reverse_iterator		rend()					;
    const_reverse_iterator	rend()				const	;

    using		buf_type::size;

			operator pointer()				;
  			operator const_pointer()		const	;
    u_int		dim()					const	;
    reference		operator [](int i)				;
    const_reference	operator [](int i)			const	;
    Array&		operator *=(double c)				;
    Array&		operator /=(double c)				;
    template <class T2, class B2>
    Array&		operator +=(const Array<T2, B2>& a)		;
    template <class T2, class B2>
    Array&		operator -=(const Array<T2, B2>& a)		;
    template <class T2, class B2>
    bool		operator ==(const Array<T2, B2>& a)	const	;
    template <class T2, class B2>
    bool		operator !=(const Array<T2, B2>& a)	const	;
    std::istream&	restore(std::istream& in)			;
    std::ostream&	save(std::ostream& out)			const	;
    void		check_dim(u_int d)			const	;

  protected:
    static u_int	partial_dim(u_int i, u_int d, u_int a)		;
};

//! �z��𐶐�����D
template <class T, class B> inline
Array<T, B>::Array()
    :buf_type()
{
}

//! �w�肵���v�f���̔z��𐶐�����D
/*!
  \param d	�z��̗v�f��
*/
template <class T, class B> inline
Array<T, B>::Array(u_int d)
    :buf_type(d)
{
}

//! �O���̗̈�Ɨv�f�����w�肵�Ĕz��𐶐�����D
/*!
  \param p	�O���̈�ւ̃|�C���^
  \param d	�z��̗v�f��
*/
template <class T, class B> inline
Array<T, B>::Array(pointer p, u_int d)
    :buf_type(p, d)
{
}

//! ���̔z��Ɠ���v�f�����z������i�R�s�[�R���X�g���N�^�̊g���j�D
/*!
  �R�s�[�R���X�g���N�^�͕ʌ����I�ɐ��������D
  \param a	�R�s�[���̔z��
*/
template <class T, class B> template <class T2, class B2>
Array<T, B>::Array(const Array<T2, B2>& a)
    :buf_type(a.dim())
{
    for (u_int i = 0; i < dim(); ++i)
	(*this)[i] = a[i];
}
	
//! �L���̈�����̔z��Ƌ��L���������z������D
/*!
  \param a	�z��
  \param i	�����z��̑�0�v�f���w�肷��index
  \param d	�����z��̎���(�v�f��)
*/
template <class T, class B> template <class B2> inline
Array<T, B>::Array(Array<T, B2>& a, u_int i, u_int d)
    :buf_type(i < a.dim() ? pointer(&a[i]) : pointer((value_type*)0),
	      partial_dim(i, d, a.dim()))
{
}

//! ���̔z��������ɑ������i�W��������Z�q�̊g���j�D
/*!
  �W��������Z�q�͕ʌ����I�ɐ��������D
  \param a	�R�s�[���̔z��
  \return	���̔z��
*/
template <class T, class B> template <class T2, class B2> Array<T, B>&
Array<T, B>::operator =(const Array<T2, B2>& a)
{
    resize(a.dim());
    for (u_int i = 0; i < dim(); ++i)
	(*this)[i] = a[i];
    return *this;
}
    
//! �S�Ă̗v�f�ɓ���̒l��������D
/*!
  \param c	�������l
  \return	���̔z��
*/
template <class T, class B> Array<T, B>&
Array<T, B>::operator =(const value_type& c)
{
    for (u_int i = 0; i < dim(); )
	(*this)[i++] = c;
    return *this;
}

//! �z��̐擪�v�f���w�������q��Ԃ��D
/*!
  \return	�擪�v�f���w�������q
*/
template <class T, class B> inline typename Array<T, B>::iterator
Array<T, B>::begin()
{
    return buf_type::ptr();
}

//! �z��̐擪�v�f���w���萔�����q��Ԃ��D
/*!
  \return	�擪�v�f���w���萔�����q
*/
template <class T, class B> inline typename Array<T, B>::const_iterator
Array<T, B>::begin() const
{
    return buf_type::ptr();
}

//! �z��̖������w�������q��Ԃ��D
/*!
  \return	�������w�������q
*/
template <class T, class B> inline typename Array<T, B>::iterator
Array<T, B>::end()
{
    return begin() + size();
}

//! �z��̖������w���萔�����q��Ԃ��D
/*!
  \return	�������w���萔�����q
*/
template <class T, class B> inline typename Array<T, B>::const_iterator
Array<T, B>::end() const
{
    return begin() + size();
}

//! �z��̖����v�f���w���t�����q��Ԃ��D
/*!
  \return	�����v�f���w���t�����q
*/
template <class T, class B> inline typename Array<T, B>::reverse_iterator
Array<T, B>::rbegin()
{
    return reverse_iterator(end());
}

//! �z��̖����v�f���w���t�����q��Ԃ��D
/*!
  \return	�����v�f���w���t�����q
*/
template <class T, class B> inline typename Array<T, B>::const_reverse_iterator
Array<T, B>::rbegin() const
{
    return const_reverse_iterator(end());
}

//! �z��̐擪���w���t�����q��Ԃ��D
/*!
  \return	�擪���w���t�����q
*/
template <class T, class B> inline typename Array<T, B>::reverse_iterator
Array<T, B>::rend()
{
    return reverse_iterator(begin());
}

//! �z��̐擪���w���t�����q��Ԃ��D
/*!
  \return	�擪���w���t�����q
*/
template <class T, class B> inline typename Array<T, B>::const_reverse_iterator
Array<T, B>::rend() const
{
    return const_reverse_iterator(begin());
}

//! �z��̓����L���̈�ւ̃|�C���^��Ԃ��D
/*!
  \return	�����L���̈�ւ̃|�C���^
*/
template <class T, class B> inline
Array<T, B>::operator pointer()
{
    return buf_type::ptr();
}

//! �z��̓����L���̈�ւ̃|�C���^��Ԃ��D
/*!
  \return	�����L���̈�ւ̃|�C���^
*/
template <class T, class B> inline
Array<T, B>::operator const_pointer() const
{
    return buf_type::ptr();
}

//! �z��̎����i�v�f���j��Ԃ��D
template <class T, class B> inline u_int
Array<T, B>::dim() const
{
    return size();
}
    
//! �z��̗v�f�փA�N�Z�X����iLIBTUTOOLS_DEBUG���w�肷���index�̃`�F�b�N����j�D
/*!
  \param i			�v�f���w�肷��index
  \return			index�ɂ���Ďw�肳�ꂽ�v�f
  \throw std::out_of_range	0 <= i < dim()�łȂ��ꍇ�ɑ��o
*/
template <class T, class B> inline typename Array<T, B>::reference
Array<T, B>::operator [](int i)
{
#ifdef LIBTUTOOLS_DEBUG
    if (i < 0 || u_int(i) >= dim())
	throw std::out_of_range("TU::Array<T, B>::operator []: invalid index!");
#endif
    return begin()[i];
}

//! �z��̗v�f�փA�N�Z�X����iLIBTUTOOLS_DEBUG���w�肷���index�̃`�F�b�N����j
/*!
  \param i			�v�f���w�肷��index
  \return			index�ɂ���Ďw�肳�ꂽ�v�f
  \throw std::out_of_range	0 <= i < dim()�łȂ��ꍇ�ɑ��o
*/
    template <class T, class B> inline typename Array<T, B>::const_reference
Array<T, B>::operator [](int i) const
{
#ifdef LIBTUTOOLS_DEBUG
    if (i < 0 || u_int(i) >= dim())
	throw std::out_of_range("TU::Array<T, B>::operator []: invalid index!");
#endif
    return begin()[i];
}

//! �S�Ă̗v�f�ɓ���̐��l���|����D
/*!
  \param c	�|���鐔�l
  \return	���̔z��
*/
template <class T, class B> Array<T, B>&
Array<T, B>::operator *=(double c)
{
    for (u_int i = 0; i < dim(); )
	(*this)[i++] *= c;
    return *this;
}

//! �S�Ă̗v�f�𓯈�̐��l�Ŋ���D
/*!
  \param c	���鐔�l
  \return	���̔z��
*/
template <class T, class B> inline Array<T, B>&
Array<T, B>::operator /=(double c)
{
    return operator *=(1.0 / c);
}

//! ���̔z��ɑ��̔z��𑫂��D
/*!
  \param a	�����z��
  \return	���̔z��
*/
template <class T, class B> template <class T2, class B2> Array<T, B>&
Array<T, B>::operator +=(const Array<T2, B2>& a)
{
    check_dim(a.dim());
    for (u_int i = 0; i < dim(); ++i)
	(*this)[i] += a[i];
    return *this;
}

//! ���̔z�񂩂瑼�̔z��������D
/*!
  \param a	�����z��
  \return	���̔z��
*/
template <class T, class B> template <class T2, class B2> Array<T, B>&
Array<T, B>::operator -=(const Array<T2, B2>& a)
{
    check_dim(a.dim());
    for (u_int i = 0; i < dim(); ++i)
	(*this)[i] -= a[i];
    return *this;
}

//! 2�̔z���v�f���ɔ�r���C�����ł��邩���ׂ�D
/*!
  \param a	��r�ΏۂƂȂ�z��
  \return	�S�Ă̗v�f�������Ȃ��true�C�����łȂ����false
*/
template <class T, class B> template <class T2, class B2> bool
Array<T, B>::operator ==(const Array<T2, B2>& a) const
{
    if (dim() != a.dim())
	return false;
    for (u_int i = 0; i < dim(); ++i)
	if ((*this)[i] != a[i])
	    return false;
    return true;
}

//! 2�̔z���v�f���ɔ�r���C�قȂ���̂����݂��邩���ׂ�D
/*!
  \param a	��r�ΏۂƂȂ�z��
  \return	�قȂ�v�f�����݂����true�C�����łȂ����false
*/
template <class T, class B> template <class T2, class B2> inline bool
Array<T, B>::operator !=(const Array<T2, B2>& a) const
{
    return !(*this == a);
}

//! ���̓X�g���[������z���ǂݍ���(binary)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, class B> inline std::istream&
Array<T, B>::restore(std::istream& in)
{
    in.read((char*)pointer(*this), sizeof(value_type) * dim());
    return in;
}

//! �o�̓X�g���[���ɔz��������o��(binary)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, class B> inline std::ostream&
Array<T, B>::save(std::ostream& out) const
{
    out.write((const char*)const_pointer(*this), sizeof(T) * dim());
    return out;
}

//! �w�肵���l�����̔z��̗v�f���Ɉ�v���邩���ׂ�D
/*!
  \param d			���ׂ����l
  \throw std::invalid_argument	d != dim()�̏ꍇ�ɑ��o
*/
template <class T, class B> inline void
Array<T, B>::check_dim(u_int d) const
{
    if (d != dim())
	throw std::invalid_argument("Array<T, B>::check_dim: dimension mismatch!");
}

template <class T, class B> inline u_int
Array<T, B>::partial_dim(u_int i, u_int d, u_int a)
{
    return (i+d <= a ? d : i < a ? a-i : 0);
}

//! ���̓X�g���[������z���ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param a	�z��̓ǂݍ��ݐ�
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, class B> inline std::istream&
operator >>(std::istream& in, Array<T, B>& a)
{
    return a.get(in >> std::ws);
}

//! �o�̓X�g���[���֔z��������o��(ASCII)�C����ɉ��s�R�[�h���o�͂���D
/*!
  \param out	�o�̓X�g���[��
  \param a	�����o���z��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, class B> inline std::ostream&
operator <<(std::ostream& out, const Array<T, B>& a)
{
    return a.put(out) << std::endl;
}

/************************************************************************
*  class Array2<T, B, R>						*
************************************************************************/
//! 1�����z��T��1�����z��Ƃ��Ē�`���ꂽ2�����z��N���X
/*!
  \param T	1�����z��̌^
  \param B	�o�b�t�@
  \param R	�s�o�b�t�@
*/
template <class T, class B=Buf<typename T::value_type>, class R=Buf<T> >
class Array2 : public Array<T, R>
{
  private:
    typedef Array<T, R>				super;
    
  public:
  //! �s�o�b�t�@�̌^
    typedef R					row_buf_type;
  //! �s�̌^    
    typedef typename super::value_type		row_type;
  //! �s�ւ̎Q��    
    typedef typename super::reference		row_reference;
  //! �萔�s�ւ̎Q��    
    typedef typename super::const_reference	row_const_reference;
  //! �s�ւ̃|�C���^    
    typedef typename super::pointer		row_pointer;
  //! �萔�s�ւ̃|�C���^    
    typedef typename super::const_pointer	row_const_pointer;
  //! �s�̔����q    
    typedef typename super::iterator		row_iterator;
  //! �s�̒萔�����q    
    typedef typename super::const_iterator	row_const_iterator;
  //! �s�̋t�����q    
    typedef typename super::reverse_iterator	row_reverse_iterator;
  //! �s�̒萔�t�����q    
    typedef typename super::const_reverse_iterator
						row_const_reverse_iterator;
  //! �o�b�t�@�̌^    
    typedef B					buf_type;
  //! �v�f�̌^    
    typedef typename row_type::value_type	value_type;
  //! �v�f�ւ̎Q��    
    typedef typename row_type::reference	reference;
  //! �萔�v�f�ւ̎Q��    
    typedef typename row_type::const_reference	const_reference;
  //! �v�f�ւ̃|�C���^    
    typedef typename row_type::pointer		pointer;
  //! �萔�v�f�ւ̃|�C���^    
    typedef typename row_type::const_pointer	const_pointer;
  //! �|�C���^�Ԃ̍�    
    typedef ptrdiff_t				difference_type;

  public:
    Array2()								;
    Array2(u_int r, u_int c)						;
    Array2(pointer p, u_int r, u_int c)					;
    Array2(const Array2& a)						;
    template <class T2, class B2, class R2>
    Array2(const Array2<T2, B2, R2>& a)					;
    template <class B2, class R2>
    Array2(Array2<T, B2, R2>& a, u_int i, u_int j, u_int r, u_int c)	;
    Array2&		operator =(const Array2& a)			;
    template <class T2, class B2, class R2>
    Array2&		operator =(const Array2<T2, B2, R2>& a)		;
    Array2&		operator =(const value_type& c)			;

    using		super::begin;
    using		super::end;
    using		super::rbegin;
    using		super::rend;
    using		super::size;
    using		super::dim;
    
			operator pointer()				;
			operator const_pointer()		const	;
    u_int		nrow()					const	;
    u_int		ncol()					const	;
    u_int		stride()				const	;
    bool		resize(u_int r, u_int c)			;
    void		resize(pointer p, u_int r, u_int c)		;
    std::istream&	restore(std::istream& in)			;
    std::ostream&	save(std::ostream& out)			const	;
    std::istream&	get(std::istream& in,
			    u_int i=0, u_int j=0, u_int jmax=0)		;

  private:
    void		set_rows()					;
    
    u_int		_ncol;
    buf_type		_buf;
};

//! 2�����z��𐶐�����D
template <class T, class B, class R> inline
Array2<T, B, R>::Array2()
    :super(), _ncol(0), _buf()
{
    if (nrow() > 0)
	_ncol = _buf.size() / nrow();
    set_rows();
}

//! �s���Ɨ񐔂��w�肵��2�����z��𐶐�����D
/*!
  \param r	�s��
  \param c	��
*/
template <class T, class B, class R> inline
Array2<T, B, R>::Array2(u_int r, u_int c)
    :super(r), _ncol(c), _buf(nrow()*buf_type::stride(ncol()))
{
    set_rows();
}

//! �O���̗̈�ƍs������ї񐔂��w�肵��2�����z��𐶐�����D
/*!
  \param p	�O���̈�ւ̃|�C���^
  \param r	�s��
  \param c	��
*/
template <class T, class B, class R> inline
Array2<T, B, R>::Array2(pointer p, u_int r, u_int c)
    :super(r), _ncol(c), _buf(p, nrow()*buf_type::stride(ncol()))
{
    set_rows();
}

//! �R�s�[�R���X�g���N�^
/*!
  \param a	�R�s�[���̔z��
*/
template <class T, class B, class R> inline
Array2<T, B, R>::Array2(const Array2& a)
    :super(a.nrow()), _ncol(a.ncol()), _buf(nrow()*buf_type::stride(ncol()))
{
    set_rows();
    super::operator =(a);
}    

//! ���̔z��Ɠ���v�f�����z������i�R�s�[�R���X�g���N�^�̊g���j�D
/*!
  �R�s�[�R���X�g���N�^���`���Ȃ��Ǝ����I�ɍ���Ă��܂��̂ŁC
  ���̃R���X�g���N�^�������Ă��R�s�[�R���X�g���N�^��ʌɒ�`
  ���Ȃ���΂Ȃ�Ȃ��D
  \param a	�R�s�[���̔z��
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline
Array2<T, B, R>::Array2(const Array2<T2, B2, R2>& a)
    :super(a.nrow()), _ncol(a.ncol()), _buf(nrow()*buf_type::stride(ncol()))
{
    set_rows();
    super::operator =(a);
}    

//! �L���̈�����̔z��Ƌ��L���������z������
/*!
  \param a	�z��
  \param i	�����z��̍�����v�f�̍s���w�肷��index
  \param j	�����z��̍�����v�f�̗���w�肷��index
  \param r	�����z��̍s��
  \param c	�����z��̗�
*/
template <class T, class B, class R> template <class B2, class R2>
Array2<T, B, R>::Array2(Array2<T, B2, R2>& a,
			u_int i, u_int j, u_int r, u_int c)
    :super(super::partial_dim(i, r, a.nrow())),
     _ncol(super::partial_dim(j, c, a.ncol())),
     _buf((nrow() > 0 && ncol() > 0 ? pointer(&a[i][j])
				    : pointer((value_type*)0)),
	  nrow()*buf_type::stride(ncol()))
{
    for (u_int ii = 0; ii < nrow(); ++ii)
	(*this)[ii].resize(pointer(&a[i+ii][j]), ncol());
}    

//! �W��������Z�q
/*!
  \param a	�R�s�[���̔z��
  \return	���̔z��
*/
template <class T, class B, class R> inline Array2<T, B, R>&
Array2<T, B, R>::operator =(const Array2& a)
{
    resize(a.nrow(), a.ncol());
    super::operator =(a);
    return *this;
}

//! ���̔z��������ɑ������i�W��������Z�q�̊g���j�D
/*!
  �W��������Z�q���`���Ȃ��Ǝ����I�ɍ���Ă��܂��̂ŁC���̑�����Z�q��
  �����Ă��W��������Z�q��ʌɒ�`���Ȃ���΂Ȃ�Ȃ��D
  \param a	�R�s�[���̔z��
  \return	���̔z��
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline Array2<T, B, R>&
Array2<T, B, R>::operator =(const Array2<T2, B2, R2>& a)
{
    resize(a.nrow(), a.ncol());
    super::operator =(a);
    return *this;
}

//! �S�Ă̗v�f�ɓ���̒l��������D
/*!
  \param c	�������l
  \return	���̔z��
*/
template <class T, class B, class R> Array2<T, B, R>&
Array2<T, B, R>::operator =(const value_type& c)
{
    for (u_int i = 0; i < nrow(); )
	(*this)[i++] = c;
    return *this;
}

//! 2�����z��̓����L���̈�ւ̃|�C���^��Ԃ��D
/*!
  \return	�����L���̈�ւ̃|�C���^
*/
template <class T, class B, class R> inline
Array2<T, B, R>::operator pointer()
{
    return _buf.ptr();
}

//! 2�����z��̓����L���̈�ւ̃|�C���^��Ԃ��D
/*!
  \return	�����L���̈�ւ̃|�C���^
*/
template <class T, class B, class R> inline
Array2<T, B, R>::operator const_pointer() const
{
    return _buf.ptr();
}

//! 2�����z��̍s����Ԃ��D
/*!
  \return	�s��
*/
template <class T, class B, class R> inline u_int
Array2<T, B, R>::nrow() const
{
    return size();
}

//! 2�����z��̗񐔂�Ԃ��D
/*!
  \return	��
*/
template <class T, class B, class R> inline u_int
Array2<T, B, R>::ncol() const
{
    return _ncol;
}

//! 2�����z��̗אڂ���s�̊Ԋu��v�f���P�ʂŕԂ��D
/*!
  \return	�Ԋu
*/
template <class T, class B, class R> inline u_int
Array2<T, B, R>::stride() const
{
    return (nrow() > 1 ? const_pointer((*this)[1]) - const_pointer((*this)[0])
		       : _ncol);
}

//! �z��̃T�C�Y��ύX����D
/*!
  \param r	�V�����s��
  \param c	�V������
  \return	r�����̍s�����傫������c�����̗񐔂ƈقȂ��true�C
		�����łȂ����false
*/
template <class T, class B, class R> bool
Array2<T, B, R>::resize(u_int r, u_int c)
{
    if (!super::resize(r) && ncol() == c)
	return false;

    _ncol = c;
    _buf.resize(nrow()*buf_type::stride(ncol()));
    set_rows();
    return true;
}

//! �z�񂪓����Ŏg�p����L���̈���w�肵�����̂ɕύX����D
/*!
  \param p	�V�����L���̈�ւ̃|�C���^
  \param r	�V�����s��
  \param c	�V������
*/
template <class T, class B, class R> void
Array2<T, B, R>::resize(pointer p, u_int r, u_int c)
{
    super::resize(r);
    _ncol = c;
    _buf.resize(p, nrow()*buf_type::stride(ncol()));
    set_rows();
}

//! ���̓X�g���[������z���ǂݍ���(binary)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, class B, class R> std::istream&
Array2<T, B, R>::restore(std::istream& in)
{
    for (u_int i = 0; i < nrow(); )
	(*this)[i++].restore(in);
    return in;
}

//! �o�̓X�g���[���ɔz��������o��(binary)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, class B, class R> std::ostream&
Array2<T, B, R>::save(std::ostream& out) const
{
    for (u_int i = 0; i < nrow(); )
	(*this)[i++].save(out);
    return out;
}

//! ���̓X�g���[������w�肵���ӏ���2�����z���ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param i	�ǂݍ��ݐ�̐擪�s���w�肷��index
  \param j	�ǂݍ��ݐ�̐擪����w�肷��index
  \param jmax	����܂łɓǂ�ł����v�f�̗�ԍ��̍ő�l
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, class B, class R> std::istream&
Array2<T, B, R>::get(std::istream& in, u_int i, u_int j, u_int jmax)
{
    char	c;

    while (in.get(c))			// Skip white spaces other than '\n'.
	if (!isspace(c) || c == '\n')
	    break;

    if (!in || c == '\n')
    {
	++i;				// Proceed to the next row.
	if (j > jmax)
	    jmax = j;
	j = 0;				// Return to the first column.

	while (in.get(c))		// Skip white spaces other than '\n'.
	    if (!isspace(c) || c == '\n')
		break;

	if (!in || c == '\n')
	{
	    if (jmax > 0)
		resize(i, jmax);
	    return in;
	}
    }
    in.putback(c);
    value_type	val;
    in >> val;
    get(in, i, j + 1, jmax);
    (*this)[i][j] = val;
    return in;
}    

template <class T, class B, class R> void
Array2<T, B, R>::set_rows()
{
    const u_int	stride = buf_type::stride(ncol());
    for (u_int i = 0; i < nrow(); ++i)
	(*this)[i].resize(_buf.ptr() + i*stride, ncol());
}
    
//! ���̓X�g���[������z���ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param a	�z��̓ǂݍ��ݐ�
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, class B, class R> inline std::istream&
operator >>(std::istream& in, Array2<T, B, R>& a)
{
    return a.get(in >> std::ws);
}

}

#endif	/* !__TUArrayPP_h */
