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
 *  $Id: Thread++.h,v 1.8 2009-09-11 05:57:13 ueshiba Exp $  
 */
#ifndef __TUThreadPP_h
#define __TUThreadPP_h

#include <pthread.h>
#include "TU/Array++.h"

namespace TU
{
/************************************************************************
*  class Thread								*
************************************************************************/
class Thread
{
  public:
    void		wait()					const	;
    
  protected:
    Thread()								;
    virtual ~Thread()							;

    void		preRaise()				const	;
    void		postRaise()				const	;

  private:
    virtual void	doJob()						= 0;
    static void*	threadProc(void* thread)			;
    
    enum State		{Ready, Idle, Exit};

    mutable pthread_mutex_t	_mutex;
    mutable pthread_cond_t	_cond;
    pthread_t			_thread;
    mutable State		_state;
};

inline void
Thread::preRaise() const
{
    pthread_mutex_lock(&_mutex);
}

inline void
Thread::postRaise() const
{
    _state = Ready;
    pthread_cond_signal(&_cond);	// Send Ready signal to the child.
    pthread_mutex_unlock(&_mutex);
}

/************************************************************************
*  class MultiThread<OP, DATA>						*
************************************************************************/
template <class OP, class DATA>
class MultiThread : public OP
{
  private:
    class OperatorThread : public Thread
    {
      public:
	OperatorThread()
	    :Thread(), _op(0), _data(0), _is(0), _ie(0)			{}

	void		raise(const OP& op, DATA& data,
			      u_int is, u_int ie)		const	;

      private:
	virtual void	doJob()						;

	mutable const OP*	_op;
	mutable DATA*		_data;
	mutable u_int		_is, _ie;
    };

  public:
    MultiThread(u_int nthreads=1)	:OP(), _threads(nthreads)	{}

    void	createThreads(u_int nthreads)				;
    void	operator ()(DATA& data)				const	;
    
  private:
    Array<OperatorThread>	_threads;
};

template <class OP, class DATA> inline void
MultiThread<OP, DATA>::createThreads(u_int nthreads)
{
    _threads.resize(nthreads);
}

template <class OP, class DATA> inline void
MultiThread<OP, DATA>::operator ()(DATA& data) const
{
    const u_int	d = data.dim() / _threads.dim();
    for (u_int is = 0, n = 0; n < _threads.dim(); ++n)
    {
	const u_int	ie = (n < _threads.dim() - 1 ? is + d : data.dim());
	_threads[n].raise(*this, data, is, ie);
	is = ie;
    }
    for (u_int n = 0; n < _threads.dim(); ++n)
	_threads[n].wait();
}

template <class OP, class DATA> inline void
MultiThread<OP, DATA>::OperatorThread::raise(const OP& op, DATA& data,
					     u_int is, u_int ie) const
{
    preRaise();
    _op	  = &op;
    _data = &data;
    _is	  = is;
    _ie	  = ie;
    postRaise();
}

template <class OP, class DATA> void
MultiThread<OP, DATA>::OperatorThread::doJob()
{
    (*_op)(*_data, _is, _ie);
}

/************************************************************************
*  class MultiThread2<OP, IN, OUT>					*
************************************************************************/
template <class OP, class IN, class OUT=IN>
class MultiThread2 : public OP
{
  private:
    class OperatorThread : public Thread
    {
      public:
	OperatorThread()
	    :Thread(), _op(0), _in(0), _out(0), _is(0), _ie(0)		{}

	void		raise(const OP& op, const IN& in,
			      OUT& out, u_int is, u_int ie)	const	;

      private:
	virtual void	doJob()						;

	mutable const OP*	_op;
	mutable const IN*	_in;
	mutable OUT*		_out;
	mutable u_int		_is, _ie;
    };

  public:
    MultiThread2(u_int nthreads=1)	:OP(), _threads(nthreads)	{}

    void	createThreads(u_int nthreads)				;
    void	operator ()(const IN& in, OUT& out)		const	;
    
  private:
    void	raiseThreads(const IN& in, OUT& out)		const	;
    
    Array<OperatorThread>	_threads;
};

template <class OP, class IN, class OUT> inline void
MultiThread2<OP, IN, OUT>::createThreads(u_int nthreads)
{
    _threads.resize(nthreads);
}

template <class OP, class IN, class OUT> void
MultiThread2<OP, IN, OUT>::raiseThreads(const IN& in, OUT& out) const
{
    const u_int	d = out.dim() / _threads.dim();
    for (u_int is = 0, n = 0; n < _threads.dim(); ++n)
    {
	const u_int	ie = (n < _threads.dim() - 1 ? is + d : out.dim());
	_threads[n].raise(*this, in, out, is, ie);
	is = ie;
    }
    for (u_int n = 0; n < _threads.dim(); ++n)
	_threads[n].wait();
}

template <class OP, class IN, class OUT> inline void
MultiThread2<OP, IN, OUT>::OperatorThread::raise(const OP& op,
						const IN& in, OUT& out,
						u_int is, u_int ie) const
{
    preRaise();
    _op  = &op;
    _in  = &in;
    _out = &out;
    _is  = is;
    _ie  = ie;
    postRaise();
}

template <class OP, class IN, class OUT> void
MultiThread2<OP, IN, OUT>::OperatorThread::doJob()
{
    (*_op)(*_in, *_out, _is, _ie);
}

}
#endif	// !__TUThreadPP_h
