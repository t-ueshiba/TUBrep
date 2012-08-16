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
 *  $Id: BoxFilter.h,v 1.3 2012-08-16 04:00:42 ueshiba Exp $
 */
/*!
  \file		BoxFilter.h
  \brief	box filter�Ɋւ���N���X�̒�`�Ǝ���
*/
#ifndef	__TUBoxFilter_h
#define	__TUBoxFilter_h

#include "TU/iterator.h"
#if defined(USE_TBB)
#  include <tbb/parallel_for.h>
#  include <tbb/blocked_range.h>
#endif

namespace TU
{
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
