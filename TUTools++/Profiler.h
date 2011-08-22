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
 *  $Id: Profiler.h,v 1.7 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		Profiler.h
  \brief	�N���X TU::Profiler �̒�`�Ǝ���
*/
#include "TU/Array++.h"
#include <iostream>
#ifdef WIN32
#  include "windows/fakeWindows.h"
#else
#  include <sys/time.h>
#endif

namespace TU
{
/************************************************************************
*  clsss Profiler							*
************************************************************************/
//! �v���O�����̊e�X�e�b�v���Ɏ��s���Ԃ𑪒肷�邽�߂̃N���X�D
class __PORT Profiler
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
  //! �w�肳�ꂽ���̃^�C�}�����v���t�@�C�����쐬����D
  /*!
    \param ntimers	�^�C�}�̌�
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
    
//! ����܂łɏ������ꂽ�t���[������Ԃ��D
/*!
  \return	�t���[����
 */
inline u_int
Profiler::nframes() const
{
    return _nframes;
}

//! �w�肳�ꂽ�^�C�}���N������D
/*!
  \param n	�^�C�}�̔ԍ�
 */
inline const Profiler&
Profiler::start(int n) const
{
    _active = n;
    _timers[_active].start();
    return *this;
}

//! ���݋N�����̃^�C�}���~����D
inline const Profiler&
Profiler::stop() const
{
    _timers[_active].stop();
    return *this;
}

//! �t���[���ԍ�����i�߂�D
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
