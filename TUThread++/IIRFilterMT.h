/*
 *  $BJ?@.(B14-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n8"=jM-(B
 *  $B<T$K$h$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$k(B
 *  $BEy$N9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
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
 *  $Id: IIRFilterMT.h,v 1.2 2008-09-10 05:11:50 ueshiba Exp $  
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
    
	void		raise(const InArray2& in,
			      OutArray2& out, int is, int ie)	const	;

      private:
	virtual void	doJob()						;

	mutable const InArray2*		_in;
	mutable OutArray2*		_out;
	mutable int			_is;
	mutable int			_ie;
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
    for (int i = 0; i < _threads.dim(); ++i)
	_threads[i].initialize(cF, cB);

    return *this;
}
    
template <u_int ORD, class IN, class OUT>
inline BilateralIIRFilterThreadArray<ORD, IN, OUT>&
BilateralIIRFilterThreadArray<ORD, IN, OUT>::
initialize(const float cF[D+D], Order order)
{
    for (int i = 0; i < _threads.dim(); ++i)
	_threads[i].initialize(cF, order);

    return *this;
}
    
template <u_int ORD, class IN, class OUT>
inline const BilateralIIRFilterThreadArray<ORD, IN, OUT>&
BilateralIIRFilterThreadArray<ORD, IN, OUT>::
operator ()(const InArray2& in, OutArray2& out) const
{
    out.resize(in.ncol(), in.nrow());
    
    const int	d = out.ncol() / _threads.dim();
    for (int is = 0, n = 0; n < _threads.dim(); ++n)
    {
	const int	ie = (n < _threads.dim() - 1 ? is + d : out.ncol());
	_threads[n].raise(in, out, is, ie);
	is = ie;
    }
    for (int n = 0; n < _threads.dim(); ++n)
	_threads[n].wait();
    
    return *this;
}
    
template <u_int ORD, class IN, class OUT> inline void
BilateralIIRFilterThreadArray<ORD, IN, OUT>::FilterThread::
raise(const InArray2& in, OutArray2& out, int is, int ie) const
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
