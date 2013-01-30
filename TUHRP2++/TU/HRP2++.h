/*
 *  $Id$
 */
#ifndef __TUHRP2PP_H
#define __TUHRP2PP_H

#include <pthread.h>
#include <string>
#include <boost/circular_buffer.hpp>
#include "TU/Vector++.h"
#include "HRP2Client.h"

namespace TU
{
/************************************************************************
*  class HRP2								*
************************************************************************/
class HRP2 : public HRP2Client
{
  public:
    typedef Matrix44d	Pose;
    typedef u_int64_t	Time;
    
  private:
    typedef HRP2Client	super;
    
  //! HRP2������δ���λ��������ƻ뤹�륹��å�
  /*!
   *  �������󥰥Хåե�����¸�������饤����Ȥ��׵�˱����ƻ��ꤵ�줿����ˤ�äȤ�
   *  �ᤤ����ˤ�����������֤���
   */
    class GetRealPoseThread
    {
      private:
	struct ChronoPose
	{
	    ChronoPose()			:D(), t(0)		{}
	    ChronoPose(const Pose& D_, Time t_)	:D(D_), t(t_)		{}

	    Pose	D;	//!< ����
	    Time	t;	//!< ���λ�����Ȥä�����(micro sec)
	};

      public:
	GetRealPoseThread(HRP2Client& hrp2,
			  const char* linkName, u_int capacity)		;
	~GetRealPoseThread()						;

	void		run()						;
	bool		operator ()(Time time, Pose& D, Time& t) const	;
	bool		operator ()(Pose& D, Time& t)		 const	;
	void		timeSpan(Time& t0, Time& t1)		 const	;
		
      private:
	void*		mainLoop()					;
	static void*	threadProc(void* thread)			;
	static Time	usec(double sec, double nsec)
			{
			    return Time(1000000*sec + nsec/1000);
			}
	
	HRP2Client&				_hrp2;
	const std::string			_linkName;
	boost::circular_buffer<ChronoPose>	_poses;
	bool					_quit;
	mutable pthread_mutex_t			_mutex;
	pthread_t				_thread;
    };

  //! HRP2�ΰ���������ʤ����ޥ�ɤ�¹Ԥ��륹��å�
  /*!
   *  HRP2����ɸ�ͤ���ã����ޤǸƽ�¦��������֤��ʤ����ޥ�ɤˤĤ��ơ��������Ω����
   *  ����åɤ����餻�뤳�Ȥˤ�ꡤ���ޥ�ɼ¹���˥ۥ���¦���̤κ�Ȥ�Ԥ���褦�ˤ��롥
   */
    class ExecuteCommandThread
    {
      public:
	typedef bool	(HRP2Client::* Command)();
	
      public:
	ExecuteCommandThread(HRP2Client& hrp2)				;
	~ExecuteCommandThread()						;

	void		run()						;
	void		operator ()(Command command)		const	;
	void		wait()					const	;
	bool		poll()					const	;
    
      private:
	void*		mainLoop()					;
	static void*	threadProc(void* thread)			;

	HRP2Client&		_hrp2;
	mutable Command		_command;
	bool			_quit;
	mutable pthread_mutex_t	_mutex;
	mutable pthread_cond_t	_cond;
	pthread_t		_thread;
    };

  public:
    HRP2(int argc, char* argv[],
	 const char* linkName="RARM_JOINT6", u_int capacity=100)	;

    bool	GetRealPose(const char* linkName,
			    Pose& D, Time& t)			const	;
    bool	GetRealPose(const char* linkName, Time time,
			    Pose& D, Time& t)			const	;
    void	PlayMotion(bool blocked=true)				;
    bool	poll()						const	;
    
  private:
    void	setup(bool isLeftHand)					;
    
  private:
    GetRealPoseThread		_getRealPose;
    ExecuteCommandThread	_executeCommand;
};

}
#endif	// !__TUHRP2PP_H
