/*
 *  $Id: utility.h,v 1.8 2007-02-28 00:16:06 ueshiba Exp $
 */
#ifndef __TUutility_h
#define __TUutility_h

#include <algorithm>
#include <iterator>

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
  \param first	�f�[�^��̐擪�����������q
  \param last	�f�[�^��̖��������������q
  \param pred	�������w�肷��P�����Z�q
  \return	�����𖞂����Ȃ��v�f�̐擪�����������q
*/
template <class Iter, class Pred> Iter
pull_if(Iter first, Iter last, Pred pred)
{
    for (Iter iter = first; iter != last; ++iter)
	if (pred(*iter))
	    std::iter_swap(first++, iter);
    return first;
}

//! 2�̈����̍��̐�Βl��Ԃ��D
template <class T> inline T
diff(const T& a, const T& b)
{
    return (a > b ? a - b : b - a);
}

/************************************************************************
*  class mem_iterator							*
************************************************************************/
//! �R���e�i�̗v�f�̓���̃����o�ɃA�N�Z�X���锽���q
/*!
  \param Iterator	�{�����q�̃x�[�X�ƂȂ锽���q
*/
template <class Iterator, class T>
class mem_iterator
    : public std::iterator<typename Iterator::iterator_category, T,
			   typename Iterator::difference_type, T*, T&>
{
  public:
    typedef typename Iterator::iterator_category	iterator_categoty;
    typedef T						value_type;
    typedef typename Iterator::difference_type		difference_type;
    typedef value_type*					pointer;
    typedef value_type&					reference;
    typedef value_type Iterator::value_type::*		mem_pointer;
    
    mem_iterator(const Iterator& i, mem_pointer m)
	:_i(i), _m(m)						{}

    bool		operator ==(const mem_iterator& i) const
			{
			    return _i == i._i;
			}
    bool		operator !=(const mem_iterator& i) const
			{
			    return !(*this == i);
			}
    reference		operator * () const
			{
			    return (*_i).*_m;
			}
    pointer		operator ->() const
			{
			    return &(operator *());
			}
    mem_iterator&	operator ++()
			{
			    ++_i;
			    return *this;
			}
    mem_iterator	operator ++(int)
			{
			    mem_iterator	tmp = *this;
			    ++_i;
			    return tmp;
			}
    mem_iterator&	operator --()
			{
			    --_i;
			    return *this;
			}
    mem_iterator	operator --(int)
			{
			    mem_iterator	tmp = *this;
			    --_i;
			    return tmp;
			}
	
  private:
    Iterator		_i;
    const mem_pointer	_m;
};

//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�1�v�f�ɃA�N�Z�X(R/W)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline mem_iterator<Iterator, typename Iterator::value_type::first_type>
make_first_iterator(const Iterator& i)
{
    return mem_iterator<Iterator, typename Iterator::value_type::first_type>
		(i, &Iterator::value_type::first);
}
    
//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�1�v�f�ɃA�N�Z�X(R)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline mem_iterator<Iterator, const typename Iterator::value_type::first_type>
make_const_first_iterator(const Iterator& i)
{
    return mem_iterator<Iterator,
			const typename Iterator::value_type::first_type>
		(i, &Iterator::value_type::first);
}
    
//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�2�v�f�ɃA�N�Z�X(R/W)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline mem_iterator<Iterator, typename Iterator::value_type::second_type>
make_second_iterator(const Iterator& i)
{
    return mem_iterator<Iterator, typename Iterator::value_type::second_type>
		(i, &Iterator::value_type::second);
}

//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�2�v�f�ɃA�N�Z�X(R)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline mem_iterator<Iterator, const typename Iterator::value_type::second_type>
make_const_second_iterator(const Iterator& i)
{
    return mem_iterator<Iterator,
			const typename Iterator::value_type::second_type>
		(i, &Iterator::value_type::second);
}
    
}

#endif	/* __TUutility_h */
