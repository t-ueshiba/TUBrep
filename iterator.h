/*
 *  ����14-24�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
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
 *  Copyright 2002-2012.
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
 *  $Id: iterator.h,v 1.3 2012-08-16 18:59:59 ueshiba Exp $
 */
/*!
  \file		iterator.h
  \brief	�e�픽���q�̒�`�Ǝ���
*/
#ifndef __TUiterator_h
#define __TUiterator_h

#include <iterator>
#include <boost/iterator/transform_iterator.hpp>
#include "TU/functional.h"

namespace TU
{
//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̔����q���炻�̃����o�ɒ��ڃA�N�Z�X(R/W)���锽���q�����D
template <class Iterator, class S, class T>
inline boost::transform_iterator<mem_var_ref_t<S, T>, Iterator>
make_mbr_iterator(Iterator i, S T::* m)
{
    return boost::make_transform_iterator(i, mem_var_ref(m));
}
    
//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̔����q���炻�̃����o�ɒ��ڃA�N�Z�X(R)���锽���q�����D
template <class Iterator, class S, class T>
inline boost::transform_iterator<const_mem_var_ref_t<S, T>, Iterator>
make_const_mbr_iterator(Iterator i, S const T::* m)
{
    return boost::make_transform_iterator(i, mem_var_ref(m));
}

//! std::pair�ւ̔����q���炻�̑�1�v�f�ɒ��ڃA�N�Z�X(R/W)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline boost::transform_iterator<
    mem_var_ref_t<
	typename std::iterator_traits<Iterator>::value_type::first_type,
	typename std::iterator_traits<Iterator>::value_type>,
    Iterator>
make_first_iterator(Iterator i)
{
    return make_mbr_iterator(
		i, &std::iterator_traits<Iterator>::value_type::first);
}
    
//! std::pair�ւ̔����q���炻�̑�1�v�f�ɒ��ڃA�N�Z�X(R)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline boost::transform_iterator<
    const_mem_var_ref_t<
	typename std::iterator_traits<Iterator>::value_type::first_type,
	typename std::iterator_traits<Iterator>::value_type>,
    Iterator>
make_const_first_iterator(Iterator i)
{
    return make_const_mbr_iterator(
		i, &std::iterator_traits<Iterator>::value_type::first);
}
    
//! std::pair�ւ̔����q���炻�̑�2�v�f�ɒ��ڃA�N�Z�X(R/W)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline boost::transform_iterator<
    mem_var_ref_t<
	typename std::iterator_traits<Iterator>::value_type::second_type,
	typename std::iterator_traits<Iterator>::value_type>,
    Iterator>
make_second_iterator(Iterator i)
{
    return make_mbr_iterator(
		i, &std::iterator_traits<Iterator>::value_type::second);
}
    
//! std::pair�ւ̔����q���炻�̑�2�v�f�ɒ��ڃA�N�Z�X(R)���锽���q�����D
/*!
  \param i	�x�[�X�ƂȂ锽���q
*/
template <class Iterator>
inline boost::transform_iterator<
    const_mem_var_ref_t<
	typename std::iterator_traits<Iterator>::value_type::second_type,
	typename std::iterator_traits<Iterator>::value_type>,
    Iterator>
make_const_second_iterator(Iterator i)
{
    return make_const_mbr_iterator(
		i, &std::iterator_traits<Iterator>::value_type::second);
}
    
/************************************************************************
*  class box_filter_iterator						*
************************************************************************/
//! �R���e�i���̎w�肳�ꂽ�v�f�ɑ΂���box filter��K�p�������ʂ�Ԃ������q
/*!
  \param Iterator	�R���e�i���̗v�f���w���萔�����q�̌^
*/
template <class Iterator>
class box_filter_iterator
    : public std::iterator<
		std::input_iterator_tag,
		typename std::iterator_traits<Iterator>::value_type>
{
  private:
    typedef std::iterator<
		std::input_iterator_tag,
		typename std::iterator_traits<Iterator>::value_type> super;
    
  public:
    typedef typename super::difference_type		difference_type;
    typedef typename super::value_type			value_type;
    typedef typename super::reference			reference;
    typedef typename super::pointer			pointer;

  public:
			box_filter_iterator(Iterator i, size_t w=0)
			    :_head(i), _tail(_head), _valid(true), _val()
			{
			    if (w > 0)
			    {
				_val = *_tail;
				
				while (--w > 0)
				    _val += *++_tail;
			    }
			}

    reference		operator *() const
			{
			    if (!_valid)
			    {
				_val += *_tail;
				_valid = true;
			    }
			    return _val;
			}
    
    const pointer	operator ->() const
			{
			    return &operator *();
			}
    
    box_filter_iterator&
			operator ++()
			{
			    _val -= *_head;
			    if (!_valid)
				_val += *_tail;
			    else
				_valid = false;
			    ++_head;
			    ++_tail;
			    return *this;
			}
    
    box_filter_iterator	operator ++(int)
			{
			    box_filter_iterator	tmp = *this;
			    operator ++();
			    return tmp;
			}
    
    bool		operator ==(const box_filter_iterator& a) const
			{
			    return _head == a._head;
			}
    
    bool		operator !=(const box_filter_iterator& a) const
			{
			    return !operator ==(a);
			}

  private:
    Iterator		_head;
    Iterator		_tail;
    mutable bool	_valid;	//!< _val �� [_head, _tail] �̑��a�Ȃ�true
    mutable value_type	_val;	//!< [_head, _tail) �܂��� [_head, _tail] �̑��a
};

//! box filter�����q�𐶐�����
/*!
  \param iter	�R���e�i���̗v�f���w���萔�����q�̌^
  \return	box filter�����q
*/
template <class Iterator> box_filter_iterator<Iterator>
make_box_filter_iterator(Iterator iter)
{
    return box_filter_iterator<Iterator>(iter);
}

}
#endif
