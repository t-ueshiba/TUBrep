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
 *  $Id: Thread.cc,v 1.5 2010-07-07 07:33:39 ueshiba Exp $  
 */
#include "TU/Thread++.h"
#include <unistd.h>

namespace TU
{
/************************************************************************
*  class Thread								*
************************************************************************/
Thread::Thread()
    :_mutex(), _cond(), _thread(), _state(Idle)
{
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);

    pthread_create(&_thread, NULL, threadProc, this);
  // The Exit signal may be sent to the child thread from ~Thread()
  // before the child reaches pthread_cond_wait() when ~Thread() is
  // called immediately after Thread(). This results in the child
  // failing to catch the signal and wait it forever! So, we need some
  // delay here.
    usleep(1000);
}

Thread::~Thread()
{
    pthread_mutex_lock(&_mutex);
    _state = Exit;
    pthread_cond_signal(&_cond);	// Send Exit signal to the child.
    pthread_mutex_unlock(&_mutex);

    pthread_join(_thread, NULL);	// Wait the child terminating.
    pthread_cond_destroy(&_cond);
    pthread_mutex_destroy(&_mutex);
}

void
Thread::wait() const
{
    pthread_mutex_lock(&_mutex);
    while (_state != Idle)
	pthread_cond_wait(&_cond, &_mutex);
    pthread_mutex_unlock(&_mutex);
}

void*
Thread::threadProc(void* t)
{
    Thread*	thread = (Thread*)t;

    pthread_mutex_lock(&thread->_mutex);
    for (;;)
    {
	pthread_cond_wait(&thread->_cond, &thread->_mutex);
      	if (thread->_state == Ready)
	{
	    thread->doJob();				// Do main job.
	    thread->_state = Idle;
	    pthread_cond_signal(&thread->_cond);	// Send Idle signal
	}						// to the parent.
	else if (thread->_state == Exit)
	    break;
    }
    pthread_mutex_unlock(&thread->_mutex);

    return 0;
}

}
