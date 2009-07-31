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
 *  $Id: Profiler.cc,v 1.3 2009-07-31 07:04:45 ueshiba Exp $
 */
#include <iomanip>
#include "TU/Profiler.h"

namespace TU
{
/************************************************************************
*  clsss Profiler							*
************************************************************************/
//! �S�Ẵ^�C�}�����Z�b�g����i�~�ώ��Ԃ���ɂ��C�t���[���ԍ���0�ɖ߂��j�D
const Profiler&
Profiler::reset() const
{
    _active = 0;
    for (u_int n = 0; n < _timers.dim(); ++n)
	_timers[n].reset();
    _nframes = 0;
    return *this;
}

//! 1�t���[��������̎��s���Ԃ�1�b������̏����t���[������\������D
/*!
  �������x�́C�e�^�C�}���̒~�ώ��Ԃ���v�Z���ꂽ���̂ƁC�S�^�C�}�̒~�ώ��Ԃ�
  ���v����v�Z���ꂽ���̗̂������\�������D
  \param out	�o�̓X�g���[��
 */
std::ostream&
Profiler::print(std::ostream& out) const
{
    using namespace	std;
    timeval		sum_accum;
    sum_accum.tv_sec = sum_accum.tv_usec = 0;

    for (u_int n = 0; n < _timers.dim(); ++n)
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

