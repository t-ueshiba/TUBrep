/*
 *  $Id: utility.h,v 1.5 2007-02-05 23:24:03 ueshiba Exp $
 */
#ifndef __TUutility_h
#define __TUutility_h

#include <algorithm>
#include <iterator>

/*!
  \namespace	TU
  \brief	�{���C�u�����Œ�`���ꂽ�N���X����ъ֐���[�߂閼�O���
*/
namespace TU
{
/************************************************************************
*  generic algorithms							*
************************************************************************/
//! 3�̈����̂����ŏ��̂��̂�Ԃ��D
template <class T> inline const T&
min(const T& a, const T& b, const T& c)
{
    return std::min(std::min(a, b), c);
}

//! 3�̈����̂����ő�̂��̂�Ԃ��D
template <class T> inline const T&
max(const T& a, const T& b, const T& c)
{
    return std::max(std::max(a, b), c);
}
    
//! 4�̈����̂����ŏ��̂��̂�Ԃ��D
template <class T> inline const T&
min(const T& a, const T& b, const T& c, const T& d)
{
    return std::min(min(a, b, c), d);
}

//! 4�̈����̂����ő�̂��̂�Ԃ��D
template <class T> inline const T&
max(const T& a, const T& b, const T& c, const T& d)
{
    return std::max(max(a, b, c), d);
}

//! 2�̈����̍��̐�Βl��Ԃ��D
template <class T> inline T
diff(const T& a, const T& b)
{
    return (a > b ? a - b : b - a);
}

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

/************************************************************************
*  class first_iterator							*
************************************************************************/
//! std::pair��v�f�Ƃ���R���e�i�ɂ�����pair�̑�1�v�f�ɃA�N�Z�X���锽���q
/*!
  \param Iterator	�{�����q�̃x�[�X�ƂȂ锽���q
*/
template <class Iterator>
class first_iterator : public std::iterator_traits<Iterator>
{
  public:
    typedef typename Iterator::value_type::first_type	value_type;
    typedef value_type*					pointer_type;
    typedef value_type&					reference_type;
	
    first_iterator(const Iterator& i) :_i(i)		{}

    bool		operator ==(const first_iterator& i) const
			{
			    return _i == i._i;
			}
    bool		operator !=(const first_iterator& i) const
			{
			    return !(*this == i);
			}
    reference_type	operator * () const
			{
			    return _i->first;
			}
    pointer_type	operator ->() const
			{
			    return &(operator *());
			}
    first_iterator&	operator ++()
			{
			    ++_i;
			    return *this;
			}
    first_iterator	operator ++(int)
			{
			    first_iterator	tmp = *this;
			    ++_i;
			    return tmp;
			}
    first_iterator&	operator --()
			{
			    --_i;
			    return *this;
			}
    first_iterator	operator --(int)
			{
			    first_iterator	tmp = *this;
			    --_i;
			    return tmp;
			}
	
  private:
    Iterator	_i;
};

//! std::pair��v�f�Ƃ���R���e�i�ɂ�����pair�̑�2�v�f�ɃA�N�Z�X���锽���q
/*!
  \param Iterator	�{�����q�̃x�[�X�ƂȂ锽���q
*/
template <class Iterator>
class second_iterator : public std::iterator_traits<Iterator>
{
  public:
    typedef typename Iterator::value_type::second_type	value_type;
    typedef value_type*					pointer_type;
    typedef value_type&					reference_type;
	
    second_iterator(const Iterator& i) :_i(i)		{}

    bool		operator ==(const second_iterator& i) const
			{
			    return _i == i._i;
			}
    bool		operator !=(const second_iterator& i) const
			{
			    return !(*this == i);
			}
    reference_type	operator * () const
			{
			    return _i->second;
			}
    pointer_type	operator ->() const
			{
			    return &(operator *());
			}
    second_iterator&	operator ++()
			{
			    ++_i;
			    return *this;
			}
    second_iterator	operator ++(int)
			{
			    second_iterator	tmp = *this;
			    ++_i;
			    return tmp;
			}
    second_iterator&	operator --()
			{
			    --_i;
			    return *this;
			}
    second_iterator	operator --(int)
			{
			    second_iterator	tmp = *this;
			    --_i;
			    return tmp;
			}
	
  private:
    Iterator	_i;
};

//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�1�v�f�ɃA�N�Z�X���锽���q�����
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator> inline first_iterator<Iterator>
make_first_iterator(const Iterator& i)
{
    return first_iterator<Iterator>(i);
}
    
//! std::pair��v�f�Ƃ���R���e�i�ɂ���pair�̑�2�v�f�ɃA�N�Z�X���锽���q�����
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator> inline second_iterator<Iterator>
make_second_iterator(const Iterator& i)
{
    return second_iterator<Iterator>(i);
}
    
}

#endif	/* __TUutility_h */
