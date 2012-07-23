/*
 *  $Id: BoxFilter.h,v 1.1 2012-07-23 00:45:40 ueshiba Exp $
 */
/*!
  \file		BoxFilter.h
  \brief	box filter�Ɋւ���N���X�̒�`�Ǝ���
*/
#ifndef	__TUBoxFilter_h
#define	__TUBoxFilter_h

#include <iterator>
#include <algorithm>
#include <boost/tuple/tuple.hpp>
#if defined(USE_TBB)
#  include <tbb/parallel_for.h>
#  include <tbb/blocked_range.h>
#endif

namespace TU
{
/************************************************************************
*  class unarize							*
************************************************************************/
//! 2�ϐ��֐���2�̈�����tuple�������Ƃ���1�ϐ��֐��ɒ����֐��I�u�W�F�N�g
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
*  class box_filter_iterator						*
************************************************************************/
//! �R���e�i���̎w�肳�ꂽ�v�f�ɑ΂���box filter��K�p�������ʂ�Ԃ������q
/*!
  \param Iterator	�R���e�i���̗v�f���w���萔�����q�̌^
*/
template <class Iterator>
class box_filter_iterator
    : public std::iterator<std::input_iterator_tag,
			   typename std::iterator_traits<Iterator>::value_type>
{
  private:
    typedef std::iterator<std::input_iterator_tag,
			  typename std::iterator_traits<Iterator>::value_type>
							super;
    
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

/************************************************************************
*  global functions							*
************************************************************************/
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

//! 1�������̓f�[�^���box filter��K�p����
/*!
  \param ib	1�������̓f�[�^��̐擪�����������q
  \param ie	1�������̓f�[�^��̖����̎������������q
  \param out	box filter��K�p�����o�̓f�[�^��̐擪�����������q
  \param w	box filter�̃E�B���h�E��
  \param shift	�o�̓f�[�^�̏������݈ʒu��out�Ŏw�肵���ʒu�������̗ʂ������炷
*/
template <class IN, class OUT> void
boxFilter(IN ib, IN ie, OUT out, size_t w, size_t shift=0)
{
    while (shift-- > 0)
	++out;
    
    for (box_filter_iterator<IN> iter(ib, w), end(ie, 0); iter != end; ++iter)
    {
	*out = *iter;
	++out;
    }
}

namespace detail
{
    template <class IN, class OUT> static void
    boxFilter2Kernel(IN ib, IN ie, OUT out,
		     size_t wrow, size_t wcol, size_t srow, size_t scol)
    {
	while (srow-- > 0)
	    ++out;
	
	for (box_filter_iterator<IN> iter(ib, wrow), end(ie, 0);
	     iter != end; ++iter)
	{
	    boxFilter(iter->begin(), iter->end() + 1 - wcol,
		      out->begin(), wcol, scol);
	    ++out;
	}
    }

# if defined(USE_TBB)
  /**********************************************************************
  *  class BoxFilter2							*
  **********************************************************************/
    template <class IN, class OUT>
    class BoxFilter2
    {
      public:
	BoxFilter2(IN in, OUT out,
		   size_t wrow, size_t wcol, size_t srow, size_t scol)
	    :_in(in), _out(out),
	     _wrow(wrow), _wcol(wcol), _srow(srow), _scol(scol)		{}

	void	operator ()(const tbb::blocked_range<int>& r) const
		{
		    detail::boxFilter2Kernel(_in + r.begin(), _in + r.end(),
					     _out + r.begin(),
					     _wrow, _wcol, _srow, _scol);
		}

      private:
	const IN	_in;
	const OUT	_out;
	const size_t	_wrow;
	const size_t	_wcol;
	const size_t	_srow;
	const size_t	_scol;
    };
# endif
}

//! 2�������̓f�[�^��box filter��K�p����
/*!
  \param ib		2�������̓f�[�^�̐擪�̍s�����������q
  \param ie		2�������̓f�[�^�̖����̎��̍s�����������q
  \param out		box filter��K�p�����o�̓f�[�^�̐擪�̍s�����������q
  \param wrow		box filter�̃E�B���h�E�̍s��
  \param wcol		box filter�̃E�B���h�E�̗�
  \param srow		�o�̓f�[�^�̏������݈ʒu�����̗ʂ����s�����ɂ��炷
  \param scol		�o�̓f�[�^�̏������݈ʒu�����̗ʂ���������ɂ��炷
  \param grainSize	�X���b�h�̗��x(TBB�g�p���̂ݗL��)
*/
template <class IN, class OUT> void
boxFilter2(IN ib, IN ie, OUT out, size_t wrow, size_t wcol,
	   size_t srow=0, size_t scol=0, size_t grainSize=100)
{
#if defined(USE_TBB)
    tbb::parallel_for(tbb::blocked_range<int>(0, ie - ib, grainSize),
		      detail::BoxFilter2<IN, OUT>(ib, out,
						  wrow, wcol, srow, scol));
#else
    detail::boxFilter2Kernel(ib, ie, out, wrow, wcol, srow, scol);
#endif
}

}
#endif	/* !__TUBoxFilter_h	*/
