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
 *  $Id: SHOT602.cc,v 1.2 2010-12-17 00:53:28 ueshiba Exp $
 */
#include "TU/SHOT602.h"
#include "TU/Manip.h"
#include <stdexcept>
#include <cstdlib>

namespace TU
{
using namespace	std;
    
/************************************************************************
*  class SHOT602							*
************************************************************************/
//! �w�肳�ꂽtty��open���ăp���X���[�^�R���g���[�������D
/*!
  \param ttyname	tty��
*/
SHOT602::SHOT602(const char* ttyname)
    :Serial(ttyname)
{
    i_igncr()
	.o_nl2crnl()
#if !defined(__APPLE__)
	.o_lower2upper()
#endif
	.c_baud(9600).c_csize(8).c_noparity().c_stop1();

    setSpeed(HighSpeed);
}
    
//! �t�@�[���E�F�A��ID���o�̓X�g���[���ɏ����o���D
/*!
  \param out	�o�̓X�g���[��
*/
void
SHOT602::showId(std::ostream& out)
{
    *this << "?:" << endl;
    for (char c; fdstream::get(c); )
    {
	if (c == '\n')
	    break;
	out << c;
    }
    out << endl;
}

/*
 *  �z�[���|�W�V�������o
 */
//! �z�[���|�W�V���������o����D
/*
  �z�[���|�W�V���������o�����܂Ńz�X�g���ɐ����Ԃ��Ȃ��D
  \param axis	�z�[���|�W�V�������o�����s���鎲
  \param dir	����������ł����true, ����������ł����false
  \param dir2	axis��#Axis_Both�̏ꍇ�̑�2���ɂ��āC
		����������ł����true, ����������ł����false
  \return	���̃R���g���[��
*/
SHOT602&
SHOT602::findHome(Axis axis, bool dir, bool dir2)
{
    putCommand(axis, 'H', (dir ? "+" : "-"), (dir2 ? "+" : "-"));
    while (isBusy())
	;

    return *this;
}
    
//! ���݈ʒu�����W���_�ɐݒ肷��D
/*
  \param axis	�z�[���|�W�V�����Ɉړ����鎲
  \return	���̃R���g���[��
*/
SHOT602&
SHOT602::setOrigin(Axis axis)
{
    return putCommand(axis, 'R', "", "");
}
    
/*
 *  ��Ԍ��o
 */
//! ���炩�̃R�}���h�����s�������ׂ�D
/*!
  \return	���s���Ȃ�true, �����łȂ����false
*/
bool
SHOT602::isBusy()
{
    *this << "!:" << endl;
    char	c;
    *this >> c >> skipl;
    
    return (c == 'B');
}

//! ���݈ʒu�𒲂ׂ�D
/*!
  \param axis	��
  \return	���݈ʒu
*/
int
SHOT602::where(Axis axis)
{
    int		position1, position2;
    bool	limit1, limit2;
    getStatus(position1, position2, limit1, limit2);

    switch (axis)
    {
      case Axis_1:
	return position1;
      case Axis_2:
	return position2;
    }

    throw runtime_error("SHOT602::where(): unknown axis!");

    return 0;
}

//! �w�肵�����ɂ����ă��~�b�g�X�C�b�`��ON�ł��邩���ׂ�D
/*!
  \param axis	��
  \return	ON�Ȃ�true, OFF�Ȃ�false
*/
bool
SHOT602::atLimit(Axis axis)
{
    int		position1, position2;
    bool	limit1, limit2;
    getStatus(position1, position2, limit1, limit2);

    switch (axis)
    {
      case Axis_1:
	return limit1;
      case Axis_2:
	return limit2;
    }

    throw runtime_error("SHOT602::atLimit(): unknown axis!");

    return false;
}

//! �R���g���[���̏�Ԃ𒲂ׂ�D
/*!
  \param position1	��1���̌��݈ʒu���Ԃ����
  \param position2	��2���̌��݈ʒu���Ԃ����
  \param limit1		��1���̃��~�b�g�X�C�b�`��ON�Ȃ�true, OFF�Ȃ�false���Ԃ����
  \param limit2		��2���̃��~�b�g�X�C�b�`��ON�Ȃ�true, OFF�Ȃ�false���Ԃ����
  \return		���̃R���g���[��
 */
bool
SHOT602::getStatus(int& position1, int& position2, bool& limit1, bool& limit2)
{
    *this << "Q:" << endl;

    char	s[256];
  // ��1���̈ʒu
    getline(s, sizeof(s), ',');
    position1 = (s[0] != '-' ? atoi(s) : -atoi(s + 1));

  // ��2���̈ʒu
    getline(s, sizeof(s), ',');
    position2 = (s[0] != '-' ? atoi(s) : -atoi(s + 1));

  // �R�}���h�܂��̓p�����[�^�̃G���[
    getline(s, sizeof(s), ',');
    if (s[0] == 'X')
	throw runtime_error("SHOT602::getStatus(): command/parameter error!");

  // ���~�b�g�X�C�b�`�̏��
    getline(s, sizeof(s), ',');
    switch (s[0])
    {
      case 'L':
	limit1 = true;
	limit2 = false;
	break;
      case 'M':
	limit1 = false;
	limit2 = true;
	break;
      case 'W':
	limit1 = limit2 = true;
	break;
      default:
	limit1 = limit2 = false;
	break;
    }

  // BUSY/READY���
    getline(s, sizeof(s));
    return (s[0] == 'B');
}

/*
 *  ���x�ݒ�
 */
//! ���x���[�h�ƃp�����[�^��ݒ肷��D
/*!
  \param speed		�ᑬ/�������[�h
  \param bottom1	��1���̋N�����x�C0��^����ƃf�t�H���g�l�ɐݒ�
  \param top1		��1���̏��q���x�C0��^����ƃf�t�H���g�l�ɐݒ�
  \param duration1	��1���̉��������ԁC0��^����ƃf�t�H���g�l�ɐݒ�
  \param bottom2	��2���̋N�����x�C0��^����ƃf�t�H���g�l�ɐݒ�
  \param top2		��2���̏��q���x�C0��^����ƃf�t�H���g�l�ɐݒ�
  \param duration2	��2���̉��������ԁC0��^����ƃf�t�H���g�l�ɐݒ�
*/
SHOT602&
SHOT602::setSpeed(Speed speed,
		  u_int bottom1, u_int top1, u_int duration1,
		  u_int bottom2, u_int top2, u_int duration2)
{
    const u_int	LowSpeedMin		= 1,
		LowSpeedMax		= 200,
		HighSpeedMin		= 50,
		HighSpeedMax		= 20000,
		DefaultBottomSpeed	= 500,
		DefaultTopSpeed		= 5000,
		DefaultDuration		= 200;

    if (speed == LowSpeed)
    {
	bottom1 = (bottom1 == 0		 ? DefaultBottomSpeed :
		   bottom1 < LowSpeedMin ? LowSpeedMin :
		   bottom1 > LowSpeedMax ? LowSpeedMax :
		   bottom1);
	top1	= (top1 == 0	      ? DefaultTopSpeed :
		   top1 < LowSpeedMin ? LowSpeedMin :
		   top1 > LowSpeedMax ? LowSpeedMax :
		   top1);
	bottom2 = (bottom2 == 0		 ? DefaultBottomSpeed :
		   bottom2 < LowSpeedMin ? LowSpeedMin :
		   bottom2 > LowSpeedMax ? LowSpeedMax :
		   bottom2);
	top2	= (top2 == 0	      ? DefaultTopSpeed :
		   top2 < LowSpeedMin ? LowSpeedMin :
		   top2 > LowSpeedMax ? LowSpeedMax :
		   top2);
    }
    else
    {
	bottom1 = (bottom1 == 0		  ? DefaultBottomSpeed :
		   bottom1 < HighSpeedMin ? HighSpeedMin :
		   bottom1 > HighSpeedMax ? HighSpeedMax :
		   bottom1);
	top1	= (top1 == 0	       ? DefaultTopSpeed :
		   top1 < HighSpeedMin ? HighSpeedMin :
		   top1 > HighSpeedMax ? HighSpeedMax :
		   top1);
	bottom2 = (bottom2 == 0		  ? DefaultBottomSpeed :
		   bottom2 < HighSpeedMin ? HighSpeedMin :
		   bottom2 > HighSpeedMax ? HighSpeedMax :
		   bottom2);
	top2	= (top2 == 0	       ? DefaultTopSpeed :
		   top2 < HighSpeedMin ? HighSpeedMin :
		   top2 > HighSpeedMax ? HighSpeedMax :
		   top2);
    }

    if (bottom1 > top1 || bottom2 > top2)
	throw runtime_error("SHOT602::setSpeed(): bottom speed must be lower than top speed!");
    
    if (duration1 == 0)
	duration1 = DefaultDuration;
    if (duration2 == 0)
	duration2 = DefaultDuration;

    *this << "D:" << (speed == LowSpeed ? 1 : 2)
	  << 'S' << bottom1 << 'F' << top1 << 'R' << duration1
	  << 'S' << bottom2 << 'F' << top2 << 'R' << duration2
	  << endl;

    return *this;
}

/*
 *  �ړ�
 */
//! �w�肵������������~����D
/*!
  \param axis	��
  \return	���̃R���g���[���@
*/
SHOT602&
SHOT602::stop(Axis axis)
{
    return putCommand(axis, 'L', "", "");
}
    
//! �S�������~����D
/*!
  \return	���̃R���g���[���@
*/
SHOT602&
SHOT602::emergencyStop()
{
    *this << "L:E" << endl;

    return *this;
}
    
//! �w�肵������jog���삳����D
/*!
  \param axis	��
  \param dir	�������Ȃ�true, �������Ȃ�false
  \param dir2	axis��#Axis_Both�̏ꍇ�̑�2���ɂ��āC
		�������Ȃ�true, �������Ȃ�false
  \return	���̃R���g���[���@
*/
SHOT602&
SHOT602::jog(Axis axis, bool dir, bool dir2)
{
    putCommand(axis, 'J', (dir ? "+" : "-"), (dir2 ? "+" : "-"));
    *this << 'G' << endl;

    return *this;
}
    
//! �w�肵�������ړ�����D
/*!
  \param axis	��
  \param val	�ړ���
  \param val2	axis��#Axis_Both�̏ꍇ�̑�2���̈ړ���
  \param block	�ړ�����������܂Ń��^�[�����Ȃ��Ȃ�true, �����Ƀ��^�[������Ȃ�false
  \return	���̃R���g���[��
*/
SHOT602&
SHOT602::move(Axis axis, int val, int val2, bool block)
{
    putCommand(axis, 'M', "", "", false);

    if (val >= 0)
	*this << "+P" << val;
    else
	*this << "-P" << -val;

    if (axis == Axis_Both)
    {
	if (val2 >= 0)
	    *this << "+P" << val2;
	else
	    *this << "-P" << -val2;
    }

    *this << endl << 'G' << endl;
    if (block)
	while (isBusy())
	    ;
    
    return *this;
}

/*
 *  �㎥
 */
//! �w�肵������hold�i�㎥�j/free�i��㎥�j��ݒ肷��D
/*!
  \param axis	��
  \param on	�㎥����Ȃ�true, �㎥���Ȃ��Ȃ�false
  \param on2	axis��#Axis_Both�̏ꍇ�̑�2���ɂ��āC
		�㎥����Ȃ�true, �㎥���Ȃ��Ȃ�false
  \return	���̃R���g���[��
*/
SHOT602&
SHOT602::setHold(Axis axis, bool on, bool on2)
{
    return putCommand(axis, 'C', (on ? "1" : "0"), (on2 ? "1" : "0"));
}

/*
 *  private member functions
 */
SHOT602&
SHOT602::putCommand(Axis axis, char command,
		    const char* arg, const char* arg2, bool putDelimiter)
{
    *this << command;

    switch (axis)
    {
      case Axis_1:
	*this << ":1" << arg;
	break;
      case Axis_2:
	*this << ":2" << arg;
	break;
      default:
	*this << ":W" << arg << arg2;
	break;
    }

    if (putDelimiter)
	*this << endl;
    
    return *this;
}
    
}
