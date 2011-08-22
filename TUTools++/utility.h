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
 *  $Id: utility.h,v 1.22 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		utility.h
  \brief	�e�탆�[�e�B���e�B�N���X����ъ֐��̒�`�Ǝ���
*/
#ifndef __TUutility_h
#define __TUutility_h

#include <algorithm>
#include <iterator>

/*!
  \namespace	std
  \brief	�������̊�{�I�Ȋ֐��𖼑O���std�ɒǉ�
*/
namespace std
{
/************************************************************************
*  generic algorithms							*
************************************************************************/
//! 3�̈����̂����ŏ��̂��̂�Ԃ��D
template <class T> inline const T&
min(const T& a, const T& b, const T& c)
{
    return min(min(a, b), c);
}

//! 3�̈����̂����ő�̂��̂�Ԃ��D
template <class T> inline const T&
max(const T& a, const T& b, const T& c)
{
    return max(max(a, b), c);
}
    
//! 4�̈����̂����ŏ��̂��̂�Ԃ��D
template <class T> inline const T&
min(const T& a, const T& b, const T& c, const T& d)
{
    return min(min(a, b, c), d);
}

//! 4�̈����̂����ő�̂��̂�Ԃ��D
template <class T> inline const T&
max(const T& a, const T& b, const T& c, const T& d)
{
    return max(max(a, b, c), d);
}

}

/*!
  \namespace	TU
  \brief	�{���C�u�����Œ�`���ꂽ�N���X����ъ֐���[�߂閼�O���
*/
namespace TU
{
/************************************************************************
*  generic algorithms							*
************************************************************************/
//! �����𖞂����v�f���O���ɁC�����łȂ����̂��㔼�ɂȂ�悤�ɕ��בւ���D
/*!
  \param begin	�f�[�^��̐擪�����������q
  \param end	�f�[�^��̖��������������q
  \param pred	�������w�肷��P�����Z�q
  \return	�����𖞂����Ȃ��v�f�̐擪�����������q
*/
template <class Iter, class Pred> Iter
pull_if(Iter begin, Iter end, Pred pred)
{
    for (Iter iter = begin; iter != end; ++iter)
	if (pred(*iter))
	    std::iter_swap(begin++, iter);
    return begin;
}

//! 2�̈����̍��̐�Βl��Ԃ��D
template <class T> inline T
diff(const T& a, const T& b)
{
    return (a > b ? a - b : b - a);
}

//! 2�����f�[�^�ɑ΂���3x3�E�B���h�E�𑖍����ċߖT�������s���D
/*!
  \param begin	�ŏ��̍s�����������q
  \param end	�Ō�̍s�̎������������q
  \param op	3x3�E�B���h�E���`��Ƃ��鉉�Z�q
*/
template <class Iterator, class OP> void
op3x3(Iterator begin, Iterator end, OP op)
{
    typedef typename std::iterator_traits<Iterator>::value_type	Row;
    typedef typename Row::iterator				RowIter;
    typedef typename std::iterator_traits<RowIter>::value_type	RowVal;
    
    Row	buf = *begin;			// ��O�̍s
    --end;
    for (Iterator iter = ++begin; iter != end; )
    {
	RowIter	p    = buf.begin();	// �����f
	RowIter	q    = iter->begin();	// ����f	
	RowVal	val  = *q;		// ����f�ɂ����錋��
	RowIter	rend = (++iter)->end();
	--rend;
	--rend;				// ������f�̉E�[
	for (RowIter r = iter->begin(); r != rend; )  // ������f�ɂ��ă��[�v
	{
	    RowVal tmp = op(p, q, r++);	// ���ډ�f�ɂ����錋��
	    *p++ = *q;			// ���s�̍����f = ����f
	    *q++ = val;			// ����f�ɂ����錋�ʂ���������
	    val	 = tmp;			// ���E�B���h�E�̍���f�ɂ����錋�ʂ�ۑ�
	}
	*p++ = *q;			// ���s�̍����f = ����f
	*q++ = val;			// ����f�ɂ����錋�ʂ���������
	*p   = *q;			// ���s�̏��f = ���ډ�f
    }
}

/************************************************************************
*  class mbr_iterator							*
************************************************************************/
//! �R���e�i�̗v�f�̓���̃����o�ɃA�N�Z�X���锽���q
/*!
  \param Iterator	�{�����q�̃x�[�X�ƂȂ锽���q
*/
template <class Iterator, class T>
class mbr_iterator
  : public
      std::iterator<typename std::iterator_traits<Iterator>::iterator_category,
		    T,
		    typename std::iterator_traits<Iterator>::difference_type,
		    T*, T&>
{
  public:
    typedef typename std::iterator_traits<Iterator>::iterator_category
							iterator_categoty;
    typedef T						value_type;
    typedef typename std::iterator_traits<Iterator>::difference_type
							difference_type;
    typedef value_type*					pointer;
    typedef value_type&					reference;
    typedef value_type std::iterator_traits<Iterator>::value_type::*
							mbr_pointer;
    
    mbr_iterator(Iterator i, mbr_pointer m)	:_i(i), _m(m)	{}

    Iterator		base() const
			{
			    return _i;
			}
    reference		operator * () const
			{
			    return (*_i).*_m;
			}
    pointer		operator ->() const
			{
			    return &(operator *());
			}
    mbr_iterator&	operator ++()
			{
			    ++_i;
			    return *this;
			}
    mbr_iterator	operator ++(int)
			{
			    mbr_iterator	tmp = *this;
			    ++_i;
			    return tmp;
			}
    mbr_iterator&	operator --()
			{
			    --_i;
			    return *this;
			}
    mbr_iterator	operator --(int)
			{
			    mbr_iterator	tmp = *this;
			    --_i;
			    return tmp;
			}
    mbr_iterator&	operator +=(difference_type n)
			{
			    _i += n;
			    return *this;
			}
    mbr_iterator&	operator -=(difference_type n)
			{
			    _i -= n;
			    return *this;
			}
    mbr_iterator	operator +(difference_type n) const
			{
			    mbr_iterator	tmp = *this;
			    return tmp += n;
			}
    mbr_iterator	operator -(difference_type n) const
			{
			    mbr_iterator	tmp = *this;
			    return tmp -= n;
			}
    reference		operator [](difference_type n) const
			{
			    return *(*this + n);
			}
	
  private:
    Iterator		_i;
    const mbr_pointer	_m;
};

template <class Iterator, class T> inline bool 
operator ==(const mbr_iterator<Iterator, T>& x,
	    const mbr_iterator<Iterator, T>& y) 
{
    return x.base() == y.base();
}

template<class Iterator, class T> inline bool 
operator !=(const mbr_iterator<Iterator, T>& x, 
	    const mbr_iterator<Iterator, T>& y) 
{
    return !(x == y);
}

template<class Iterator, class T> inline bool 
operator <(const mbr_iterator<Iterator, T>& x, 
	   const mbr_iterator<Iterator, T>& y) 
{
    return x.base() < y.base();
}

template<class Iterator, class T> inline bool 
operator >(const mbr_iterator<Iterator, T>& x, 
	   const mbr_iterator<Iterator, T>& y) 
{
    return y < x;
}

template<class Iterator, class T> inline bool 
operator <=(const mbr_iterator<Iterator, T>& x, 
	    const mbr_iterator<Iterator, T>& y) 
{
    return !(y < x);
}

template<class Iterator, class T> inline bool 
operator >=(const mbr_iterator<Iterator, T>& x, 
	    const mbr_iterator<Iterator, T>& y) 
{
    return !(x < y);
}

template<class Iterator, class T>
inline typename mbr_iterator<Iterator, T>::difference_type
operator -(const mbr_iterator<Iterator, T>& x, 
	   const mbr_iterator<Iterator, T>& y) 
{
    return x.base() - y.base();
}

template<class Iterator, class T> inline mbr_iterator<Iterator, T> 
operator +(typename mbr_iterator<Iterator, T>::difference_type n,
	   const mbr_iterator<Iterator, T>& x) 
{
    return x + n;
}

//! T�^�̃����o�����I�u�W�F�N�g��v�f�Ƃ���R���e�i�ɂ��Ă��̃����o�ɃA�N�Z�X(R/W)���锽���q�����D
template <class Iterator, class T> inline mbr_iterator<Iterator, T>
make_mbr_iterator(Iterator i, T std::iterator_traits<Iterator>::value_type::* m)
{
    return mbr_iterator<Iterator, T>(i, m);
}
    
//! T�^�̃����o�����I�u�W�F�N�g��v�f�Ƃ���R���e�i�ɂ��Ă��̃����o�ɃA�N�Z�X(R)���锽���q�����D
template <class Iterator, class T> inline mbr_iterator<Iterator, const T>
make_const_mbr_iterator(Iterator i,
			const T std::iterator_traits<Iterator>::value_type::* m)
{
    return mbr_iterator<Iterator, const T>(i, m);
}

//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�1�v�f�ɃA�N�Z�X(R/W)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline mbr_iterator<Iterator, typename std::iterator_traits<Iterator>
					  ::value_type::first_type>
make_first_iterator(Iterator i)
{
    return make_mbr_iterator(i, &std::iterator_traits<Iterator>
				    ::value_type::first);
}
    
//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�1�v�f�ɃA�N�Z�X(R)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline mbr_iterator<Iterator, const typename std::iterator_traits<Iterator>
						::value_type::first_type>
make_const_first_iterator(Iterator i)
{
    return make_const_mbr_iterator(i, &std::iterator_traits<Iterator>
					  ::value_type::first);
}
    
//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�2�v�f�ɃA�N�Z�X(R/W)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline mbr_iterator<Iterator, typename std::iterator_traits<Iterator>
					  ::value_type::second_type>
make_second_iterator(Iterator i)
{
    return make_mbr_iterator(i, &std::iterator_traits<Iterator>
				    ::value_type::second);
}

//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�2�v�f�ɃA�N�Z�X(R)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline mbr_iterator<Iterator, const typename std::iterator_traits<Iterator>
						::value_type::second_type>
make_const_second_iterator(Iterator i)
{
    return make_const_mbr_iterator(i, &std::iterator_traits<Iterator>
					  ::value_type::second);
}
    
}

#endif	/* __TUutility_h */
