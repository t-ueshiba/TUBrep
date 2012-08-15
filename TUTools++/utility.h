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
 *  $Id: utility.h,v 1.23 2012-08-15 07:17:55 ueshiba Exp $
 */
/*!
  \file		utility.h
  \brief	�e�탆�[�e�B���e�B�N���X����ъ֐��̒�`�Ǝ���
*/
#ifndef __TUutility_h
#define __TUutility_h

#include <algorithm>
#include <functional>
#include <iterator>
#include <boost/iterator/transform_iterator.hpp>

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
*  class mem_var_t							*
************************************************************************/
//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̃|�C���^���炻�̃����o�ɒ��ڃA�N�Z�X(R/W)����֐��I�u�W�F�N�g
/*
  \param S	T�^�I�u�W�F�N�g�̃����o�ϐ��̌^
  \param T	S�^�����o�ϐ������L����I�u�W�F�N�g�̌^
*/ 
template <class S, class T>
class mem_var_t : public std::unary_function<T*, S>
{
  public:
    typedef S T::*	mem_ptr;
    
    explicit mem_var_t(mem_ptr m)	:_m(m)		{}

    S&		operator ()(T* p)		const	{return p->*_m;}

  private:
    const mem_ptr	_m;
};

//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̃|�C���^���炻�̃����o�ɒ��ڃA�N�Z�X(R/W)����֐��I�u�W�F�N�g�𐶐�����
template <class S, class T> inline mem_var_t<S, T>
mem_var(S T::*m)
{
    return mem_var_t<S, T>(m);
}

/************************************************************************
*  class const_mem_var_t						*
************************************************************************/
//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̃|�C���^���炻�̃����o�ɒ��ڃA�N�Z�X(R)����֐��I�u�W�F�N�g
/*
  \param S	T�^�I�u�W�F�N�g�̃����o�ϐ��̌^
  \param T	S�^�����o�ϐ������L����I�u�W�F�N�g�̌^
*/ 
template <class S, class T>
class const_mem_var_t : public std::unary_function<const T*, S>
{
  public:
    typedef S T::*	mem_ptr;
    
    explicit const_mem_var_t(mem_ptr m)		:_m(m)	{}

    const S&	operator ()(const T* p)		const	{return p->*_m;}

  private:
    const mem_ptr	_m;
};

//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̃|�C���^���炻�̃����o�ɒ��ڃA�N�Z�X(R)����֐��I�u�W�F�N�g�𐶐�����
template <class S, class T> inline const_mem_var_t<S, T>
const_mem_var(S const T::* m)
{
    return const_mem_var_t<S, T>(m);
}

/************************************************************************
*  class mem_var_ref_t							*
************************************************************************/
//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̎Q�Ƃ��炻�̃����o�ɒ��ڃA�N�Z�X(R/W)����֐��I�u�W�F�N�g
/*
  \param S	T�^�I�u�W�F�N�g�̃����o�ϐ��̌^
  \param T	S�^�����o�ϐ������L����I�u�W�F�N�g�̌^
*/ 
template <class S, class T>
class mem_var_ref_t : public std::unary_function<T&, S>
{
  public:
    typedef S T::*	mem_ptr;
    
    explicit mem_var_ref_t(mem_ptr m)		:_m(m)	{}

    S&		operator ()(T& p)		const	{return p.*_m;}

  private:
    const mem_ptr	_m;
};
    
//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̎Q�Ƃ��炻�̃����o�ɒ��ڃA�N�Z�X(R/W)����֐��I�u�W�F�N�g�𐶐�����
template <class S, class T> inline mem_var_ref_t<S, T>
mem_var_ref(S T::*m)
{
    return mem_var_ref_t<S, T>(m);
}

/************************************************************************
*  class const_mem_var_ref_t						*
************************************************************************/
//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̎Q�Ƃ��炻�̃����o�ɒ��ڃA�N�Z�X(R)����֐��I�u�W�F�N�g
/*
  \param S	T�^�I�u�W�F�N�g�̃����o�ϐ��̌^
  \param T	S�^�����o�ϐ������L����I�u�W�F�N�g�̌^
*/ 
template <class S, class T>
class const_mem_var_ref_t : public std::unary_function<const T&, S>
{
  public:
    typedef S const T::*	mem_ptr;
    
    explicit const_mem_var_ref_t(mem_ptr m)	:_m(m)	{}

    const S&	operator ()(const T& p)		const	{return p.*_m;}

  private:
    const mem_ptr	_m;
};
    
//! S�^�̃����o�ϐ�������T�^�I�u�W�F�N�g�ւ̎Q�Ƃ��炻�̃����o�ɒ��ڃA�N�Z�X(R)����֐��I�u�W�F�N�g�𐶐�����
template <class S, class T> inline const_mem_var_ref_t<S, T>
mem_var_ref(S const T::* m)
{
    return const_mem_var_ref_t<S, T>(m);
}

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
    mem_var_ref_t<typename std::iterator_traits<Iterator>::value_type
							 ::second_type,
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
    
}

#endif	/* __TUutility_h */
