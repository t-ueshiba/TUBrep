/*
 *  $BJ?@.(B14-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$kEy$NCx(B
 *  $B:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
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
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: Profiler.h,v 1.1 2008-06-24 00:40:50 ueshiba Exp $
 */
#include <iostream>
#include <sys/time.h>
#include "TU/Array++.h"

namespace TU
{
/************************************************************************
*  clsss Profiler							*
************************************************************************/
//! $B%W%m%0%i%`$N3F%9%F%C%WKh$K<B9T;~4V$rB,Dj$9$k$?$a$N%/%i%9!%(B
class Profiler
{
  private:
    class Timer
    {
      public:
	Timer()							{reset();}

	Timer&	reset()						;
	Timer&	start()						;
	Timer&	stop()						;
	timeval	print(std::ostream& out, u_int nframes)	const	;
	
      private:
	timeval	_accum;
	timeval	_t0;
    };

  public:
  //! $B;XDj$5$l$?8D?t$N%?%$%^$r;}$D%W%m%U%!%$%i$r:n@.$9$k!%(B
  /*!
    \param ntimers	$B%?%$%^$N8D?t(B
   */
    Profiler(u_int ntimers)
	:_active(0), _timers(ntimers), _nframes(0)		{}

    u_int		nframes()			const	;
    const Profiler&	reset()				const	;
    const Profiler&	start(int n)			const	;
    const Profiler&	stop()				const	;
    const Profiler&	nextFrame()			const	;
    std::ostream&	print(std::ostream& out)	const	;

  private:
    mutable int			_active;
    mutable Array<Timer>	_timers;
    mutable u_int		_nframes;
};
    
//! $B$3$l$^$G$K=hM}$5$l$?%U%l!<%`?t$rJV$9!%(B
/*!
  \return	$B%U%l!<%`?t(B
 */
inline u_int
Profiler::nframes() const
{
    return _nframes;
}

//! $B;XDj$5$l$?%?%$%^$r5/F0$9$k!%(B
/*!
  \param n	$B%?%$%^$NHV9f(B
 */
inline const Profiler&
Profiler::start(int n) const
{
    _active = n;
    _timers[_active].start();
    return *this;
}

//! $B8=:_5/F0Cf$N%?%$%^$rDd;_$9$k!%(B
inline const Profiler&
Profiler::stop() const
{
    _timers[_active].stop();
    return *this;
}

//! $B%U%l!<%`HV9f$r0l$D?J$a$k!%(B
inline const Profiler&
Profiler::nextFrame() const
{
    ++_nframes;
    return *this;
}

inline Profiler::Timer&
Profiler::Timer::reset()
{
    _accum.tv_sec = _accum.tv_usec = 0;
    _t0.tv_sec = _t0.tv_usec = 0;
    return *this;
}

inline Profiler::Timer&
Profiler::Timer::start()
{
    gettimeofday(&_t0, NULL);
    return *this;
}
    
inline Profiler::Timer&
Profiler::Timer::stop()
{
    timeval	t1;
    gettimeofday(&t1, NULL);
    _accum.tv_sec  += (t1.tv_sec  - _t0.tv_sec);
    _accum.tv_usec += (t1.tv_usec - _t0.tv_usec);
    return *this;
}

}
