/*
 *  平成14-19年（独）産業技術総合研究所 著作権所有
 *  
 *  創作者：植芝俊夫
 *
 *  本プログラムは（独）産業技術総合研究所の職員である植芝俊夫が創作し，
 *  （独）産業技術総合研究所が著作権を所有する秘密情報です．著作権所有
 *  者による許可なしに本プログラムを使用，複製，改変，第三者へ開示する
 *  等の行為を禁止します．
 *  
 *  このプログラムによって生じるいかなる損害に対しても，著作権所有者お
 *  よび創作者は責任を負いません。
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
 *  $Id$
 */
#include "TU/Profiler.h"
#include <iomanip>

namespace TU
{
/************************************************************************
*  clsss Profiler							*
************************************************************************/
//! 全てのタイマをリセットする（蓄積時間を空にし，フレーム番号を0に戻す）．
const Profiler&
Profiler::reset() const
{
    _active = 0;
    for (size_t n = 0; n < _timers.size(); ++n)
	_timers[n].reset();
    _nframes = 0;
    return *this;
}

//! 1フレームあたりの実行時間と1秒あたりの処理フレーム数を表示する．
/*!
  処理速度は，各タイマ毎の蓄積時間から計算されたものと，全タイマの蓄積時間の
  総計から計算されたものの両方が表示される．
  \param out	出力ストリーム
 */
std::ostream&
Profiler::print(std::ostream& out) const
{
    using namespace	std;
    timeval		sum_accum;
    sum_accum.tv_sec = sum_accum.tv_usec = 0;

    for (size_t n = 0; n < _timers.size(); ++n)
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

std::ostream&
Profiler::printTabSeparated(std::ostream& out) const
{
    using namespace	std;
    timeval		sum_accum;
    sum_accum.tv_sec = sum_accum.tv_usec = 0;

    for (size_t n = 0; n < _timers.size(); ++n)
    {
	timeval	accum = _timers[n].printTabSeparated(out, _nframes);
	sum_accum.tv_sec  += accum.tv_sec;
	sum_accum.tv_usec += accum.tv_usec;
	out << '\t';
    }
    double	tmp = sum_accum.tv_sec * 1.0e6 + sum_accum.tv_usec;
    return out << "| " << tmp / (1.0e3 * _nframes) << endl;
}

timeval
Profiler::Timer::print(std::ostream& out, size_t nframes) const
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

timeval
Profiler::Timer::printTabSeparated(std::ostream& out, size_t nframes) const
{
    if (nframes > 0)
    {
	double	tmp = _accum.tv_sec * 1.0e6 + _accum.tv_usec;
	out << tmp / (1.0e3 * nframes);
    }

    return _accum;
}

}

