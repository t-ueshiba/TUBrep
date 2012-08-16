/*
 *  $Id: functional.h,v 1.1 2012-08-16 01:30:37 ueshiba Exp $
 */
/*!
  \file		functional.h
  \brief	�֐��I�u�W�F�N�g�̒�`�Ǝ���
*/
#ifndef __TUfunctional_h
#define __TUfunctional_h

#include <functional>
#include <boost/tuple/tuple.hpp>

namespace TU
{
/************************************************************************
*  class unarize							*
************************************************************************/
//! 2�ϐ��֐���2�ϐ�tuple�������Ƃ���1�ϐ��֐��ɒ����֐��I�u�W�F�N�g
/*!
  \param OP	2�ϐ��֐��I�u�W�F�N�g�̌^
*/
template <class OP>
class unarize
{
  public:
    typedef typename OP::result_type				result_type;

    unarize(const OP& op=OP())	:_op(op)			{}
    
    template <class TUPLE>
    result_type	operator ()(const TUPLE& t) const
		{
		    return _op(boost::get<0>(t), boost::get<1>(t));
		}

  private:
    const OP	_op;
};

/************************************************************************
*  class seq_transform							*
************************************************************************/
//! 1�܂���2�g�̃f�[�^��̊e�v�f�ɑ΂���1�܂���2�ϐ��֐���K�p����1�g�̃f�[�^����o�͂���֐��I�u�W�F�N�g
/*!
  \param RESULT	�ϊ����ꂽ�v�f���o�͂���f�[�^��̌^
  \param OP	�X�̗v�f�ɓK�p�����1�ϐ�/2�ϐ��֐��I�u�W�F�N�g�̌^
*/
template <class RESULT, class OP>
class seq_transform
{
  public:
    typedef const RESULT&					result_type;

    seq_transform(const OP& op=OP())	:_op(op), _result()	{}

    template <class ARG>
    result_type	operator ()(const ARG& x) const
		{
		    _result.resize(x.size());
		    std::transform(x.begin(), x.end(), _result.begin(), _op);
		    return _result;
		}
    
    template <class ARG0, class ARG1>
    result_type	operator ()(const ARG0& x, const ARG1& y) const
		{
		    _result.resize(x.size());
		    std::transform(x.begin(), x.end(), y.begin(),
				   _result.begin(), _op);
		    return _result;
		}
    
  private:
    const OP		_op;
    mutable RESULT	_result;
};

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

}
#endif

