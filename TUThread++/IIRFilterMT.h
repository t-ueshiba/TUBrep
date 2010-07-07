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
 *  $Id: IIRFilterMT.h,v 1.4 2010-07-07 07:33:33 ueshiba Exp $  
 */
#ifndef __TUIIRFilterMT_h
#define __TUIIRFilterMT_h

#include "TU/IIRFilter.h"
#include "TU/Thread++.h"

namespace TU
{
/************************************************************************
*  class BilateralIIRFilterThreadArray<ORD, IN, OUT>			*
************************************************************************/
template <u_int ORD, class IN, class OUT>
class BilateralIIRFilterThreadArray
{
  public:
    enum						{D = ORD};
    typedef BilateralIIRFilter<ORD>			BIIRF;
    typedef typename BIIRF::Order			Order;

  private:
    typedef Array2<typename IN::row_type,  typename IN::buffer_type,
		   typename IN::rowbuffer_type>			InArray2;
    typedef Array2<typename OUT::row_type, typename OUT::buffer_type,
		   typename OUT::rowbuffer_type>		OutArray2;

    class FilterThread : public BIIRF, public Thread
    {
      public:
	FilterThread()	:_in(0), _out(0), _is(0), _ie(0)		{}
    
	void		raise(const InArray2& in, OutArray2& out,
			      u_int is, u_int ie)		const	;

      private:
	virtual void	doJob()						;

	mutable const InArray2*		_in;
	mutable OutArray2*		_out;
	mutable u_int			_is;
	mutable u_int			_ie;
    };
    
  public:
    BilateralIIRFilterThreadArray(u_int nthreads) :_threads(nthreads)	{}
    
    BilateralIIRFilterThreadArray&
	initialize(const float cF[D+D], const float cB[D+D])		;
    BilateralIIRFilterThreadArray&
	initialize(const float cF[D+D], Order order)			;
    const BilateralIIRFilterThreadArray&
	operator ()(const InArray2& in, OutArray2& out)		const	;
    
  private:
    Array<FilterThread>	_threads;
};

template <u_int ORD, class IN, class OUT>
inline BilateralIIRFilterThreadArray<ORD, IN, OUT>&
BilateralIIRFilterThreadArray<ORD, IN, OUT>::
initialize(const float cF[D+D], const float cB[D+D])
{
    for (u_int i = 0; i < _threads.dim(); ++i)
	_threads[i].initialize(cF, cB);

    return *this;
}
    
template <u_int ORD, class IN, class OUT>
inline BilateralIIRFilterThreadArray<ORD, IN, OUT>&
BilateralIIRFilterThreadArray<ORD, IN, OUT>::
initialize(const float cF[D+D], Order order)
{
    for (u_int i = 0; i < _threads.dim(); ++i)
	_threads[i].initialize(cF, order);

    return *this;
}
    
template <u_int ORD, class IN, class OUT>
inline const BilateralIIRFilterThreadArray<ORD, IN, OUT>&
BilateralIIRFilterThreadArray<ORD, IN, OUT>::
operator ()(const InArray2& in, OutArray2& out) const
{
    out.resize(in.ncol(), in.nrow());
    
    const u_int	d = out.ncol() / _threads.dim();
    for (u_int is = 0, n = 0; n < _threads.dim(); ++n)
    {
	const u_int	ie = (n < _threads.dim() - 1 ? is + d : out.ncol());
	_threads[n].raise(in, out, is, ie);
	is = ie;
    }
    for (u_int n = 0; n < _threads.dim(); ++n)
	_threads[n].wait();
    
    return *this;
}
    
template <u_int ORD, class IN, class OUT> inline void
BilateralIIRFilterThreadArray<ORD, IN, OUT>::FilterThread::
raise(const InArray2& in, OutArray2& out, u_int is, u_int ie) const
{
    preRaise();
    _in	     = &in;
    _out     = &out;
    _is	     = is;
    _ie	     = ie;
    postRaise();
}

template <u_int ORD, class IN, class OUT> void
BilateralIIRFilterThreadArray<ORD, IN, OUT>::FilterThread::doJob()
{
    BIIRF::operator ()(*_in, *_out, _is, _ie);
}

}
#endif	// !__TUIIRFilterMT_h
