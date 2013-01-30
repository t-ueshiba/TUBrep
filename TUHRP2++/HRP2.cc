/*
 *  $Id$
 */
#include <boost/foreach.hpp>
#include "TU/HRP2++.h"

namespace TU
{
/************************************************************************
*  class HRP2								*
************************************************************************/
HRP2::HRP2(int argc, char* argv[], const char* linkName, u_int capacity)
    :super(), _getRealPose(*this, linkName, capacity), _executeCommand(*this)
{
    using namespace	std;
    
    if (!init(argc, argv))
	throw runtime_error("HRP2Client::init() failed!");
    setup(false);
}

void
HRP2::setup(bool isLeftHand)
{
    using namespace	std;
    
  // ��«����
    bool	constrained[] = {true, true, true, true, true, true};
    double	weights[]     = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0};
    if (!SelectTaskDofs(isLeftHand, constrained, weights))
	throw runtime_error("HRP2Client::SelectTaskDofs() failed!!");
    
  // ���Ѥ��뼫ͳ�٤�����
    bool	usedDofs[] =
		{
		    true, true, true, true, true, true,		// right leg
		    true, true, true, true, true, true,		// left leg
		    true, false,				// waist
		    false, false,				// neck
		    true, true, true, true, true, true, true,	// right arm
		    false,					// right hand
		    false, false, false, false, false, false, false, // left arm
		    false,					// left hand
		    true, true, true, false, false, false	// base
		};
    if (!SelectUsedDofs(usedDofs))
	throw runtime_error("HRP2Client::SelectUsedDofs() failed!!");
    
  // �١����Ȥʤ��󥯤�����
    if (!SelectBaseLink("RLEG_JOINT5"))
	throw runtime_error("HRP2Client::SelectBaseLink() failed!!");

  // Later�⡼�ɤ�����
    if (!SelectExecutionMode(true))
	throw runtime_error("HRP2Client::SelectExecutionMode() failed!!");

  // ȿ��¦�μ����Ū��deselect������˾�μ��select���롥
    DeSelectArm(!isLeftHand);
    SelectArm(isLeftHand);

  // ����åɤ�ư���롥
    _getRealPose.run();
    _executeCommand.run();
}

bool
HRP2::GetRealPose(const char* linkName, Pose& D, Time& t) const
{
    return _getRealPose(D, t);
}
    
bool
HRP2::GetRealPose(const char* linkName, Time time, Pose& D, Time& t) const
{
    return _getRealPose(time, D, t);
}

void
HRP2::PlayMotion(bool blocked)
{
    if (blocked)
	super::PlayMotion();
    else
	_executeCommand(&super::PlayMotion);
}

bool
HRP2::poll() const
{
    return _executeCommand.poll();
}
    
/************************************************************************
*  class HRP2::GetRealPoseThread					*
************************************************************************/
HRP2::GetRealPoseThread::GetRealPoseThread(HRP2Client& hrp2,
					   const char* linkName,
					   u_int capacity)
    :_hrp2(hrp2), _linkName(linkName), _poses(capacity),
     _quit(false), _mutex(), _thread()
     
{
    pthread_mutex_init(&_mutex, NULL);
}

HRP2::GetRealPoseThread::~GetRealPoseThread()
{
    pthread_mutex_lock(&_mutex);
    _quit = true;			// ��λ�ե饰��Ω�Ƥ�
    pthread_mutex_unlock(&_mutex);

    pthread_join(_thread, NULL);	// �ҥ���åɤν�λ���Ԥ�
    pthread_mutex_destroy(&_mutex);
}

void
HRP2::GetRealPoseThread::run()
{
    pthread_create(&_thread, NULL, threadProc, this);

    usleep(1000);
}
    
bool
HRP2::GetRealPoseThread::operator ()(Time time,
				     Pose& D, Time& t) const
{
  // Ϳ����줿��������Υݡ�����������ޤ��Ԥġ�
    ChronoPose	after;		// time�����λ���Ǽ������줿�ݡ���
    for (;;)			// ��ȯ������ޤ��Ԥġ�
    {
	pthread_mutex_lock(&_mutex);
	if (!_poses.empty() && (after = _poses.back()).t > time)
	    break;
	pthread_mutex_unlock(&_mutex);
    }

  // ��󥰥Хåե�������̤ꡤͿ����줿����������Υݡ�����õ����
    BOOST_REVERSE_FOREACH (const ChronoPose& pose, _poses)
    {
	if (pose.t <= time)	// time��ľ���Υݡ����ʤ�С�����
	{
	  // pose��after�Τ��������λ��郎time�˶ᤤ�����֤���
	    if ((time - pose.t) < (after.t - time))
	    {			// time��after����pose�λ���˶ᤱ��С�����
		D = pose.D;
		t = pose.t;
	    }
	    else		// time��pose����after�λ���˶ᤱ��С�����
	    {
		D = after.D;
		t = after.t;
	    }
	    pthread_mutex_unlock(&_mutex);

	    return true;	// time�򶴤ࣲ�ĤΥݡ�����ȯ������
	}
	after = pose;
    }
    pthread_mutex_unlock(&_mutex);

    return false;		// time��ľ���Υݡ�����ȯ���Ǥ��ʤ��ä�
}

bool
HRP2::GetRealPoseThread::operator ()(Pose& D, Time& t) const
{
    pthread_mutex_lock(&_mutex);
    if (!_poses.empty())
    {
	D = _poses.back().D;
	t = _poses.back().t;
	pthread_mutex_unlock(&_mutex);

	return true;
    }
    pthread_mutex_unlock(&_mutex);

    return false;
}
    
void
HRP2::GetRealPoseThread::timeSpan(Time& t0, Time& t1) const
{
    for (;;)
    {
	pthread_mutex_lock(&_mutex);
	if (!_poses.empty())
	{
	    t0 = _poses.front().t;
	    t1 = _poses.back().t;
	    pthread_mutex_unlock(&_mutex);
	    break;
	}
	pthread_mutex_unlock(&_mutex);
    }
}
    
void*
HRP2::GetRealPoseThread::mainLoop()
{
    for (;;)
    {
	pthread_mutex_lock(&_mutex);
	bool	quit = _quit;		// ̿������
	pthread_mutex_unlock(&_mutex);
	if (quit)			// ��λ̿��ʤ��...
	    break;			// æ��

	Pose	D;
	double	sec, nsec;
	if (_hrp2.GetRealPose(const_cast<char*>(_linkName.c_str()),
			      D.data(), &sec, &nsec))	// �ݡ�������������
	{
	    Time	t = usec(sec, nsec);	// micro second
	    
	    if (_poses.empty() || (t != _poses.back().t))
	    {
		pthread_mutex_lock(&_mutex);
		_poses.push_back(ChronoPose(D, t));	// ��󥰥Хåե��������
		pthread_mutex_unlock(&_mutex);
	    }
	}
    }

    return 0;
}
    
void*
HRP2::GetRealPoseThread::threadProc(void* thread)
{
    GetRealPoseThread*	th = static_cast<GetRealPoseThread*>(thread);

    return th->mainLoop();
}

/************************************************************************
*  class HRP2::ExecuteCommandThread					*
************************************************************************/
HRP2::ExecuteCommandThread::ExecuteCommandThread(HRP2Client& hrp2)
    :_hrp2(hrp2), _command(0), _quit(false), _mutex(), _cond(), _thread()
{
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);
}

HRP2::ExecuteCommandThread::~ExecuteCommandThread()
{
    pthread_mutex_lock(&_mutex);
    _quit = true;			// ��λ̿��򥻥å�
    pthread_cond_signal(&_cond);	// �ҥ���åɤ˽�λ̿�������
    pthread_mutex_unlock(&_mutex);

    pthread_join(_thread, NULL);	// �ҥ���åɤν�λ���Ԥ�
    pthread_cond_destroy(&_cond);
    pthread_mutex_destroy(&_mutex);
}

void
HRP2::ExecuteCommandThread::run()
{
    pthread_create(&_thread, NULL, threadProc, this);

    usleep(1000);
}
    
void
HRP2::ExecuteCommandThread::operator ()(Command command) const
{
    pthread_mutex_lock(&_mutex);
    _command = command;
    pthread_cond_signal(&_cond);	// �ҥ���åɤ˼¹�̿�������
    pthread_mutex_unlock(&_mutex);
}

void
HRP2::ExecuteCommandThread::wait() const
{
    pthread_mutex_lock(&_mutex);
    while (_command != 0)			// �¹Ԥ���λ����ޤ�
	pthread_cond_wait(&_cond, &_mutex);	// �Ԥ�
    pthread_mutex_unlock(&_mutex);
}

bool
HRP2::ExecuteCommandThread::poll() const
{
    pthread_mutex_lock(&_mutex);
    Command	command = _command;
    pthread_mutex_unlock(&_mutex);

    return command == 0;
}
    
void*
HRP2::ExecuteCommandThread::mainLoop()
{
    pthread_mutex_lock(&_mutex);
    for (;;)
    {
	pthread_cond_wait(&_cond, &_mutex);	// �Ƥ����̿����Ԥ�
	if (_quit)				// ����åɽ�λ̿��ʤ��...
	    break;				// �롼�פ�æ��
	else if (_command != 0)			// �¹�̿��ʤ��...
	{
	    pthread_mutex_unlock(&_mutex);
	    (_hrp2.*_command)();		// ���ޥ�ɤ�¹�
	    pthread_mutex_lock(&_mutex);
	    _command = 0;
	    pthread_cond_signal(&_cond);	// �Ƥ˼¹Ԥ���λ�������Ȥ�����
	}
    }
    pthread_mutex_unlock(&_mutex);

    return 0;
}

void*
HRP2::ExecuteCommandThread::threadProc(void* thread)
{
    ExecuteCommandThread* th = static_cast<ExecuteCommandThread*>(thread);

    return th->mainLoop();
}
    
}

