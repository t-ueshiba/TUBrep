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
 *  $Id: Profiler.cc,v 1.1 2008-06-24 00:40:50 ueshiba Exp $
 */
#include <iomanip>
#include "TU/Profiler.h"

namespace TU
{
/************************************************************************
*  clsss Profiler							*
************************************************************************/
//! $BA4$F$N%?%$%^$r%j%;%C%H$9$k!JC_@Q;~4V$r6u$K$7!$%U%l!<%`HV9f$r(B0$B$KLa$9!K!%(B
const Profiler&
Profiler::reset() const
{
    _active = 0;
    for (int n = 0; n < _timers.dim(); ++n)
	_timers[n].reset();
    _nframes = 0;
    return *this;
}

//! 1$B%U%l!<%`$"$?$j$N<B9T;~4V$H(B1$BIC$"$?$j$N=hM}%U%l!<%`?t$rI=<($9$k!%(B
/*!
  $B=hM}B.EY$O!$3F%?%$%^Kh$NC_@Q;~4V$+$i7W;;$5$l$?$b$N$H!$A4%?%$%^$NC_@Q;~4V$N(B
  $BAm7W$+$i7W;;$5$l$?$b$N$NN>J}$,I=<($5$l$k!%(B
  \param out	$B=PNO%9%H%j!<%`(B
 */
std::ostream&
Profiler::print(std::ostream& out) const
{
    using namespace	std;
    timeval		sum_accum;
    sum_accum.tv_sec = sum_accum.tv_usec = 0;

    for (int n = 0; n < _timers.dim(); ++n)
    {
	timeval	accum = _timers[n].print(out, _nframes);
	sum_accum.tv_sec  += accum.tv_sec;
	sum_accum.tv_usec += accum.tv_usec;
    }
    double	tmp = sum_accum.tv_sec * 1.0e6 + sum_accum.tv_usec;
    return out << '|' << setw(8) << tmp / (1.0e3 * _nframes)
	       << "ms(" << setw(7) << 1.0e6 * _nframes / tmp
	       << "fps)" << endl;
}

timeval
Profiler::Timer::print(std::ostream& out, u_int nframes) const
{
    using namespace	std;

    if (nframes > 0)
    {
	double	tmp = _accum.tv_sec * 1.0e6 + _accum.tv_usec;
	out << setw(9) << tmp / (1.0e3 * nframes) << "ms("
	    << setw(7) << 1.0e6 * nframes / tmp << "fps)";
    }
    else
	out << setw(9) << '*' << "ms(" << setw(7) << '*' << "fps)";

    return _accum;
}

}

