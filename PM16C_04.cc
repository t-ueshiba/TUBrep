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
 *  $Id: PM16C_04.cc,v 1.7 2010-01-14 11:13:09 ueshiba Exp $
 */
#include "TU/PM16C_04.h"
#include "TU/Manip.h"
#include <iomanip>
#include <stdexcept>
#include <cstring>

namespace TU
{
using namespace	std;
    
/************************************************************************
*  static functions							*
************************************************************************/
static void
checkChannel(u_int channel)
{
    if (channel >= 16)
	throw runtime_error("channel# must be less than 16!");
}
    
/************************************************************************
*  class PM16C_04							*
************************************************************************/
//! �w�肳�ꂽtty��open���ăp���X���[�^�R���g���[�������D
/*!
  \param ttyname	tty��
*/
PM16C_04::PM16C_04(const char* ttyname)
    :Serial(ttyname)
{
    i_igncr()
	.o_nl2crnl()
#if !defined(__APPLE__)
	.o_lower2upper()
#endif
	.c_baud(9600).c_csize(8).c_noparity().c_stop1();

    setMode(true);
    usleep(DELAY);

  // padding�Ƃ��ĕ����Ɛ��l�̊Ԃ�'0'���o��
    fill('0');
    setf(ios_base::internal, ios_base::adjustfield);
}
    
//! �t�@�[���E�F�A��ID���o�̓X�g���[���ɏ����o���D
/*!
  \param out	�o�̓X�g���[��
*/
void
PM16C_04::showId(std::ostream& out)
{
    *this << "VER?" << endl;
    for (char c; fdstream::get(c); )
    {
	if (c == '\n')
	    break;
	out << c;
    }
    out << endl;
}

/*
 *  Local/Remote���[�h
 */
//! �R���g���[����LOCAL/REMOTE���[�h��ݒ肷��D
/*!
  \param remote	true�Ȃ�REMOTE���[�h�ɁCfalse�Ȃ�LOCAL���[�h�ɐݒ�
*/
PM16C_04&
PM16C_04::setMode(bool remote)
{
    *this << "S1" << (remote ? 'R' : 'L') << endl;

    return *this;
}

//! �R���g���[����REMOTE���[�h�ł��邩���ׂ�D
/*!
  \return	REMOTE���[�h�Ȃ�true�CLOCAL���[�h�Ȃ�false
*/
bool
PM16C_04::isRemoteMode()
{
    return getHardwareLimitSwitchStatus(Axis_A) & 0x8;
}

/*
 *  �ʒu
 */
//! �w�肳�ꂽ�`�����l���̈ʒu��ݒ肷��D
/*!
  \param channel	�`�����l��
  \param position	�ʒu
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::setPosition(u_int channel, int position)
{
    checkChannel(channel);
    *this << "S5" << std::hex << channel << "PS"
	  << setw(8) << std::showpos << std::dec << position
	  << endl;
    usleep(DELAY);

    return *this;
}

//! �w�肳�ꂽ�`�����l���̌��݈ʒu�𒲂ׂ�D
/*!
  \param channel	�`�����l��
  \return		���݈ʒu
*/
int
PM16C_04::getPosition(u_int channel)
{
    checkChannel(channel);
    *this << "S4" << std::hex << channel << "PS" << endl;
    int	position;
    *this >> std::dec >> position >> skipl;
    
    return position;
}
    
/*
 *  �X�s�[�h
 */
//! ���ׂĂ̎��̃X�s�[�h���[�h(LOW/MEDIUM/HIGH)��ݒ肷��D
/*!
  \param speed		�X�s�[�h���[�h
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::setSpeed(Speed speed)
{
    *this << "S3"
	  << (speed == Speed_Low ? '4' : speed == Speed_Medium ? '5' : '6')
	  << endl;

    return *this;
}

//! �w�肳�ꂽ�`���l���̃X�s�[�h�̒l��ݒ肷��D
/*!
  \param channel	�`�����l��
  \param speed		�X�s�[�h���[�h
  \param val		�X�s�[�h�̒l
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::setSpeedValue(u_int channel, Speed speed, u_int val)
{
    checkChannel(channel);
    *this << "SP"
	  << (speed == Speed_Low ? 'L' : speed == Speed_Medium ? 'M' : 'H')
	  << std::hex << channel
	  << setw(6) << std::noshowpos << std::dec << val
	  << endl;
    
    return *this;
}
    
//! �w�肳�ꂽ�`���l���̃X�s�[�h�̒l�𒲂ׂ�D
/*!
  \param channel	�`�����l��
  \param speed		�X�s�[�h���[�h
  \return		�X�s�[�h�̒l
*/
u_int
PM16C_04::getSpeedValue(u_int channel, Speed speed)
{
    checkChannel(channel);
    *this << "SP"
	  << (speed == Speed_Low ? 'L' : speed == Speed_Medium ? 'M' : 'H')
	  << '?' << std::hex << channel << endl;
    char	c;
    u_int	val;
    *this >> c >> std::dec >> val >> skipl;

    return val;
}

/*
 *  �\�t�g�E�F�A���~�b�g�X�C�b�`
 */
//! �\�t�g�E�F�A���~�b�g�X�C�b�`��L�������D���̈ʒu���w�肷��D
/*!
  \param channel	�`�����l��
  \param PositionP	���������~�b�g�X�C�b�`�̈ʒu
  \param PositionN	���������~�b�g�X�C�b�`�̈ʒu
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::enableSoftwareLimitSwitch(u_int channel,
				    int positionP, int positionN)
{
    setLimitSwitchConf(channel, getLimitSwitchConf(channel) | 0x20);
    *this << "S5" << std::hex << channel << "FL"
	  << setw(8) << std::showpos << std::dec << positionP
	  << endl;
    *this << "S5" << std::hex << channel << "BL"
	  << setw(8) << std::showpos << std::dec << positionN
	  << endl;
    usleep(DELAY);

    return *this;
}
    
//! �\�t�g�E�F�A���~�b�g�X�C�b�`�𖳌�������D
/*!
  \param channel	�`�����l��
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::disableSoftwareLimitSwitch(u_int channel)
{
    return setLimitSwitchConf(channel, getLimitSwitchConf(channel) & 0xdf);
}
    
//! �\�t�g�E�F�A���~�b�g�X�C�b�`���L�������ׂ�D
/*!
  \param channel	�`�����l��
  \return		�L���ł����true, �����ł����false
*/
bool
PM16C_04::isEnabledSoftwareLimitSwitch(u_int channel)
{
    return getLimitSwitchConf(channel) & 0x20;
}

//! �������\�t�g�E�F�A���~�b�g�X�C�b�`�̈ʒu�𒲂ׂ�D
/*!
  \param channel	�`�����l��
  \return		���������~�b�g�X�C�b�`�̈ʒu
*/
int
PM16C_04::getSoftwareLimitSwitchPositionP(u_int channel)
{
    checkChannel(channel);
    *this <<"S4" << std::hex << channel << "FL" << endl;
    int	position;
    *this >> std::dec >> position >> skipl;

    return position;
}
    
//! �������\�t�g�E�F�A���~�b�g�X�C�b�`�̈ʒu�𒲂ׂ�D
/*!
  \param channel	�`�����l��
  \return		���������~�b�g�X�C�b�`�̈ʒu
*/
int
PM16C_04::getSoftwareLimitSwitchPositionN(u_int channel)
{
    checkChannel(channel);
    *this <<"S4" << std::hex << channel << "BL" << endl;
    int	position;
    *this >> std::dec >> position >> skipl;

    return position;
}
    
/*
 *  �n�[�h�E�F�A���~�b�g�X�C�b�`
 */
//! �n�[�h�E�F�A���~�b�g�X�C�b�`��L�������D���̋ɐ���ݒ肷��D
/*!
  \param channel	�`�����l��
  \param dir		���������~�b�g�X�C�b�`�Ȃ�true�C
			���������~�b�g�X�C�b�`�Ȃ�false
  \param normallyClose	�X�C�b�`�������Ă��Ȃ��Ƃ���close�Ȃ�true�Copen�Ȃ�true
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::enableHardwareLimitSwitch(u_int channel, bool dir,
				    bool normallyClose)
{
    u_int	conf = getLimitSwitchConf(channel) | (dir ? 0x08: 0x10);
    if (normallyClose)
	return setLimitSwitchConf(channel, conf | (dir ? 0x01 : 0x02));
    else
	return setLimitSwitchConf(channel, conf & (dir ? 0xfe : 0xfd));
}

//! �n�[�h�E�F�A���~�b�g�X�C�b�`�𖳌�������D
/*!
  \param channel	�`�����l��
  \param dir		���������~�b�g�X�C�b�`�Ȃ�true�C
			���������~�b�g�X�C�b�`�Ȃ�false
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::disableHardwareLimitSwitch(u_int channel, bool dir)
{
    return setLimitSwitchConf(channel, getLimitSwitchConf(channel) &
				       (dir ? 0xf7 : 0xef));
}

//! �n�[�h�E�F�A���~�b�g�X�C�b�`���L���ł��邩���ׂ�D
/*!
  \param channel	�`�����l��
  \param dir		���������~�b�g�X�C�b�`�Ȃ�true�C
			���������~�b�g�X�C�b�`�Ȃ�false
  \return		�L���Ȃ�true, �����Ȃ�false
*/
bool
PM16C_04::isEnabledHardwareLimitSwitch(u_int channel, bool dir)
{
    return getLimitSwitchConf(channel) & (dir ? 0x08 : 0x10);
}

//! �n�[�h�E�F�A���~�b�g�X�C�b�`�̋ɐ��𒲂ׂ�D
/*!
  \param channel	�`�����l��
  \param dir		���������~�b�g�X�C�b�`�Ȃ�true�C
			���������~�b�g�X�C�b�`�Ȃ�false
  \return		�X�C�b�`�������Ă��Ȃ��Ƃ�close�Ȃ�true, open�Ȃ�false
*/
bool
PM16C_04::getHardwareLimitSwitchPolarity(u_int channel, bool dir)
{
    return getLimitSwitchConf(channel) & (dir ? 0x01 : 0x02);
}

//! �n�[�h�E�F�A�z�[���|�W�V�����X�C�b�`�̋ɐ���ݒ肷��D
/*!
  \param channel	�`�����l��
  \param normallyClose	�X�C�b�`�������Ă��Ȃ��Ƃ���close�Ȃ�true�Copen�Ȃ�true
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::setHomeSwitchPolarity(u_int channel, bool normallyClose)
{
    return setLimitSwitchConf(channel, getLimitSwitchConf(channel) | 0x04);
}

//! �n�[�h�E�F�A�z�[���|�W�V�����X�C�b�`�̋ɐ��𒲂ׂ�D
/*!
  \param channel	�`�����l��
  \return		�X�C�b�`�������Ă��Ȃ��Ƃ�close�Ȃ�true, open�Ȃ�false
*/
bool
PM16C_04::getHomeSwitchPolarity(u_int channel)
{
    return getLimitSwitchConf(channel) & 0x04;
}

/*
 *  �o�b�N���b�V���␳
 */
//! �o�b�N���b�V���␳�̃X�e�b�v����ݒ肷��D
/*!
  \param channel	�`�����l��
  \param steps		�X�e�b�v��
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::setBacklashCorrectionStep(u_int channel, u_int steps)
{
    checkChannel(channel);
    *this << 'B' << std::hex << channel
	  << setw(8) << std::showpos << std::dec << steps
	  << endl;

    return *this;
}

//! �o�b�N���b�V���␳�̃X�e�b�v���𒲂ׂ�D
/*!
  \param channel	�`�����l��
  \return		�X�e�b�v��
*/
u_int
PM16C_04::getBacklashCorrectionStep(u_int channel)
{
    checkChannel(channel);
    *this << 'B' << std::hex << channel << '?' << endl;
    char	c;
    u_int	val;
    *this >> c >> std::dec >> val >> skipl;

    return val;
}

/*
 *  Hold off�@�\�i��~���̔�ʓd�j
 */
//! Hold off�@�\�i��~���̔�ʓd�j��L��������D
/*!
  \param channel	�`�����l��
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::enableHoldOff(u_int channel)
{
    return setLimitSwitchConf(channel, getLimitSwitchConf(channel) & 0xbf);
}

//! Hold off�@�\�i��~���̔�ʓd�j�𖳌�������D
/*!
  \param channel	�`�����l��
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::disableHoldOff(u_int channel)
{
    return setLimitSwitchConf(channel, getLimitSwitchConf(channel) | 0x40);
}

//! Hold off�@�\�i��~���̔�ʓd�j���L�������ׂ�D
/*!
  \param channel	�`�����l��
  \return		�L���Ȃ�true, �����Ȃ�false
*/
bool
PM16C_04::isEnabledHoldOff(u_int channel)
{
    return getLimitSwitchConf(channel) & 0x40;
}

/*
 *  �z�[���|�W�V�������o
 */
//! �ǂ��瑤����z�[���|�W�V�������o���s������ݒ肷��D
/*!
  \param channel	�`�����l��
  \param dir		����������ł����true, ����������ł����false
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::setHomeSearchDirection(u_int channel, bool dir)
{
    checkChannel(channel);
    *this << 'D' << (dir ? 'P' : 'N') << std::hex << channel << endl;

    return *this;
}
    
//! �ǂ��瑤����z�[���|�W�V�������o���s���̂����ׂ�D
/*!
  \param channel	�`�����l��
  \return		����������ł����true, ����������ł����false
*/
bool
PM16C_04::getHomeSearchDirection(u_int channel)
{
    return getHomeStatus(channel) & 0x1;
}
    
//! �z�[���|�W�V�������o���̃I�t�Z�b�g�l��ݒ肷��D
/*!
  \param channel	�`�����l��
  \param offset		�I�t�Z�b�g�l
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::setHomeOffset(u_int channel, u_int offset)
{
    checkChannel(channel);
    *this << "GF" << std::hex << channel
	  << setw(4) << std::noshowpos << std::dec << offset
	  << endl;

    return *this;
}
    
//! �z�[���|�W�V�������o���̃I�t�Z�b�g�l�𒲂ׂ�D
/*!
  \param channel	�`�����l��
  \return		�I�t�Z�b�g�l
*/
u_int
PM16C_04::getHomeOffset(u_int channel)
{
    checkChannel(channel);
    *this << "GF?" << std::hex << channel << endl;
    char	c;
    u_int	offset;
    *this >> c >> std::dec >> offset >> skipl;

    return offset;
}
    
//! �z�[���|�W�V���������o�ς݂����ׂ�D
/*!
  \param channel	�`�����l��
  \return		���o�ς݂Ȃ�true, �����o�Ȃ�false
*/
bool
PM16C_04::isHomeFound(u_int channel)
{
    return getHomeStatus(channel) & 0x4;
}
    
//! ���o�ς݂̃z�[���|�W�V�������ǂ��炩�猟�o���ꂽ�̂����ׂ�D
/*!
  \param channel	�`�����l��
  \return		����������Ȃ�true, ����������Ȃ�false
*/
bool
PM16C_04::isHomeFoundFromFront(u_int channel)
{
    return getHomeStatus(channel) & 0x2;
}

//! ���o�ς݂̃z�[���|�W�V�����̈ʒu�𒲂ׂ�D
/*!
  \param channel	�`�����l��
  \return		�z�[���|�W�V�����ʒu
*/
int
PM16C_04::getHomePosition(u_int channel)
{
    checkChannel(channel);
    *this << "HP?" << std::hex << channel << endl;
    int	position;
    *this >> std::dec >> position >> skipl;

    return position;
}

//! �z�[���|�W�V���������o����i�����x���������Ԃ�v���j�D
/*
  \param axis	�z�[���|�W�V�������o�����s���鎲
  \return	���̃R���g���[��
*/
PM16C_04&
PM16C_04::findHome(Axis axis)
{
    const u_int	channel = getChannel(axis);
    if (!isEnabledHardwareLimitSwitch(channel, getHomeSearchDirection(channel)))
	throw runtime_error("PM16C_04::findHome(): hardware limit switch is disabled!");

    *this << "FHP" << (axis == Axis_A ? 'A' :
		       axis == Axis_B ? 'B' :
		       axis == Axis_C ? 'C' : 'D')
	  << endl;

    while (isBusy(axis))
	;

    return *this;
}
    
//! �z�[���|�W�V�����Ɉړ�����D
/*
  \param axis	�z�[���|�W�V�����Ɉړ����鎲
  \return	���̃R���g���[��
*/
PM16C_04&
PM16C_04::goHome(Axis axis)
{
    if (!isHomeFound(getChannel(axis)))
	throw runtime_error("PM16C_04::goHome(): home position is not found yet!");

    *this << "RTHP" << (axis == Axis_A ? 'A' :
			axis == Axis_B ? 'B' :
			axis == Axis_C ? 'C' : 'D')
	  << endl;

    while (isBusy(axis))
	;
    
    return *this;
}
    
/*
 *  ���ƃ`�����l���̊֌W
 */
//! �w�肵�����Ɏw�肵���`�����l�������т���D
/*!
  \param axis		��
  \param channel	�`�����l��
  \return		���̃R���g���[��
*/
PM16C_04&
PM16C_04::setChannel(Axis axis, u_int channel)
{
    checkChannel(channel);
    *this << "S1" << (axis == Axis_A ? '1' :
		      axis == Axis_B ? '2' :
		      axis == Axis_C ? '5' : '6')
	  << std::hex << channel
	  << endl;
    usleep(DELAY);
    
    return *this;
}

//! �w�肵�����ɂǂ̃`�����l�������т����Ă��邩���ׂ�D
/*!
  \param axis		��
  \return		�`�����l��
*/
u_int
PM16C_04::getChannel(Axis axis)
{
    u_int	channel_A, channel_B, channel_C, channel_D;
    getChannel(channel_A, channel_B, channel_C, channel_D);

    return (axis == Axis_A ? channel_A :
	    axis == Axis_B ? channel_B :
	    axis == Axis_C ? channel_C : channel_D);
}
    
//! 4�̎��ɂ��ꂼ��ǂ̃`�����l�������т����Ă��邩���ׂ�D
/*!
  \param channel_A	#Axis_A�Ɍ��т���ꂽ�`�����l�����Ԃ����
  \param channel_B	#Axis_B�Ɍ��т���ꂽ�`�����l�����Ԃ����
  \param channel_C	#Axis_C�Ɍ��т���ꂽ�`�����l�����Ԃ����
  \param channel_D	#Axis_D�Ɍ��т���ꂽ�`�����l�����Ԃ����
*/
void
PM16C_04::getChannel(u_int& channel_A, u_int& channel_B,
		     u_int& channel_C, u_int& channel_D)
{
    *this << "S10" << endl;
    char	c;
    u_int	channel;
    *this >> c >> std::hex >> channel >> skipl;
    channel_A = (channel >> 12) & 0xf;
    channel_B = (channel >>  8) & 0xf;
    channel_C = (channel >>  4) & 0xf;
    channel_D =  channel	& 0xf;
}

/*
 *  ���̏��
 */
//! �w�肵�����̌��݈ʒu�𒲂ׂ�D
/*!
  \param axis	��
  \return	���݈ʒu
*/
int
PM16C_04::where(Axis axis)
{
    *this << "S2" << (axis == Axis_A ? '0' :
		      axis == Axis_B ? '2' :
		      axis == Axis_C ? '4' : '6')
	  << 'D'
	  << endl;
    int	position;
    *this >> std::dec >> position >> skipl;

    return position;
}

//! �w�肵�����ɂ����ĉ��炩�̃R�}���h�����s�������ׂ�D
/*!
  \param axis	��
  \return	���s���Ȃ�true, �����łȂ����false
*/
bool
PM16C_04::isBusy(Axis axis)
{
    return getControllerStatus(axis) & 0x1;
}

//! �w�肵�����ɂ����ăp���X�������������ׂ�D
/*!
  \param axis	��
  \return	�������Ȃ�true, �����łȂ����false
*/
bool
PM16C_04::isPulseEmitted(Axis axis)
{
    return getControllerStatus(axis) & 0x2;
}

//! �w�肵�����ɂ����Ĕ������ł������p���X����~���������ׂ�D
/*!
  \param axis	��
  \return	��~�����Ȃ�true, �����łȂ����false
*/
bool
PM16C_04::isPulseStopped(Axis axis)
{
    return getControllerStatus(axis) & 0x4;
}

//! �w�肵�����ɂ����ă��~�b�g�X�C�b�`��ON�ł��邩���ׂ�D
/*!
  \param axis	��
  \param dir	���������~�b�g�X�C�b�`�Ȃ�true, ���������~�b�g�X�C�b�`�Ȃ�false
  \return	ON�Ȃ�true, OFF�Ȃ�false
*/
bool
PM16C_04::atLimit(Axis axis, bool dir)
{
    return !(getHardwareLimitSwitchStatus(axis) & (dir ? 0x1 : 0x2));
}

//! �w�肵�����ɂ����ăz�[���|�W�V�����X�C�b�`��ON�ł��邩���ׂ�D
/*!
  \param axis	��
  \return	ON�Ȃ�true, OFF�Ȃ�false
*/
bool
PM16C_04::atHome(Axis axis)
{
    return !(getHardwareLimitSwitchStatus(axis) & 0x4);
}

/*
 *  �ړ�
 */
//! �w�肵������������~����D
/*!
  \param axis	��
  \return	���̃R���g���[���@
*/
PM16C_04&
PM16C_04::stop(Axis axis)
{
    return move(axis, "40");
}
    
//! �w�肵������jog���삳����D
/*!
  \param axis	��
  \param dir	�������Ȃ�true, �������Ȃ�false
  \return	���̃R���g���[���@
*/
PM16C_04&
PM16C_04::jog(Axis axis, bool dir)
{
   return move(axis, (dir ? "08" : "09"));
}
    
//! �w�肵��������葬�x�ŃX�L��������D
/*!
  \param axis	��
  \param dir	�������Ȃ�true, �������Ȃ�false
  \return	���̃R���g���[���@
*/
PM16C_04&
PM16C_04::scanWithConstantSpeed(Axis axis, bool dir)
{
    return move(axis, (dir ? "0C" : "0D"));
}
    
//! �w�肵�������`���x�p�^�[���ŃX�L��������D
/*!
  \param axis	��
  \param dir	�������Ȃ�true, �������Ȃ�false
  \return	���̃R���g���[���@
*/
PM16C_04&
PM16C_04::scan(Axis axis, bool dir)
{
    return move(axis, (dir ? "0E" : "0F"));
}
    
//! �w�肵�������ꎞ��~����D
/*!
  \param axis	��
  \param on	
  \return	���̃R���g���[���@
*/
PM16C_04&
PM16C_04::pause(Axis axis, bool on)
{
    return move(axis, (on ? "16" : "17"));
}
    
//! �w�肵������Hold off�@�\�i��~���̔�ʓd�j��ݒ肷��D
/*!
  \param axis	��
  \param set	�@�\���g�p����Ȃ�true, �g�p���Ȃ��Ȃ�falsse	
  \return	���̃R���g���[��
*/
PM16C_04&
PM16C_04::holdOff(Axis axis, bool set)
{
    return move(axis, (set ? "18" : "19"));
}

//! �w�肵�������X�L�������Ȃ���z�[���|�W�V���������o����i���������ᐸ�x�j�D
/*!
  \param axis	��
  \param dir	�������ɃX�L��������Ȃ�true, �������Ȃ�false
  \return	���̃R���g���[��
*/
PM16C_04&
PM16C_04::scanAndStopAtHome(Axis axis, bool dir)
{
    return move(axis, (dir ? "1E" : "1F"));
}

//! �w�肵�������ړ�����D
/*!
  \param axis			��
  \param relative		���ΓI�Ȉړ��Ȃ�true, ��ΓI�Ȉړ��Ȃ�false
  \param val			�ړ��ʁi���ΓI�Ȉړ��j
				�܂��͖ڕW�ʒu�i��ΓI�Ȉړ��j
  \param correctBacklash	��~���Ƀo�b�N���b�V���␳���s���Ȃ�true,
				�s��Ȃ��Ȃ�fase
  \return			���̃R���g���[��
*/
PM16C_04&
PM16C_04::move(Axis axis, bool relative, int val, bool correctBacklash)
{
    *this << "S3" << (axis == Axis_A ? '2' :
		      axis == Axis_B ? '3' :
		      axis == Axis_C ? 'A' : 'B')
	  << (relative ? 'R' : 'A')
	  << setw(8) << std::showpos << std::dec << val;
    if (correctBacklash)
	*this << 'B';
    *this << endl;

    return *this;
}

/*
 *  Parallel I/O�|�[�g
 */
//! �p������I/O�|�[�g��L��������D
/*!
  \return	���̃R���g���[��
*/
PM16C_04&
PM16C_04::enableParallelIO()
{
    *this << "PIO" << endl;

    return *this;
}
    
//! �p������I/O�|�[�g�𖳌�������D
/*!
  \return	���̃R���g���[��
*/
PM16C_04&
PM16C_04::disableParallelIO()
{
    *this << "COM" << endl;

    return *this;
}

//! �p������I/O�|�[�g���L�������ׂ�D
/*!
  \return	�L���Ȃ�true, �����Ȃ�false
*/
bool
PM16C_04::isEnabledParallelIO()
{
    *this << "PIO?" << endl;
    char	response[4];
    getline(response, sizeof(response));

    return !strncmp(response, "PIO", 3);
}

//! �p������I/O�|�[�g����ǂݍ��ށD
/*!
  \return	�ǂݍ��񂾒l
*/
u_int
PM16C_04::readParallelIO()
{
    *this << "RD" << endl;
    char	c;
    u_int	val;
    *this >> c >> std::hex >> val >> skipl;

    return val;
}
    
//! �p������I/O�|�[�g�ɏ����o���D
/*!
  \param val	�����o���l
  \return	���̃R���g���[��
*/
PM16C_04&
PM16C_04::writeParallelIO(u_int val)
{
    *this << 'W' << setw(4) << std::hex << (val & 0xffff)
	  << endl;

    return *this;
}
    
/*
 *  private member functions
 */
u_int
PM16C_04::getLimitSwitchConf(u_int channel)
{
    checkChannel(channel);
    *this << "S4" << std::hex << channel << 'D' << endl;
    char	c;
    u_int	conf;
    *this >> c >> std::hex >> conf >> skipl;

    return conf >> 16;
}

PM16C_04&
PM16C_04::setLimitSwitchConf(u_int channel, u_int conf)
{
    checkChannel(channel);
    *this << "S5" << std::hex << channel << 'D'
	  << setw(2) << std::hex << conf
	  << endl;
    usleep(DELAY);

    return *this;
}

u_int
PM16C_04::getHardwareLimitSwitchStatus(Axis axis)
{
    *this << "S6" << endl;
    char	c;
    u_int	status;
    *this >> c >> std::hex >> status >> skipl;

    return (status >> (axis == Axis_A ?  8 :
		       axis == Axis_B ? 12 :
		       axis == Axis_C ?  0 : 4)) & 0xf;
}
    
u_int
PM16C_04::getHomeStatus(u_int channel)
{
    checkChannel(channel);
    *this << "G?" << std::hex << channel << endl;
    char	c;
    u_int	status;
    *this >> c >> std::hex >> status >> skipl;

    return status;
}
    
u_int
PM16C_04::getControllerStatus(Axis axis)
{
    *this << "S2" << (axis == Axis_A ? '1' :
		      axis == Axis_B ? '3' :
		      axis == Axis_C ? '5' : '7')
	  << endl;
    char	c;
    u_int	status;
    *this >> c >> std::hex >> status >> skipl;

    return status;
}
    
PM16C_04&
PM16C_04::move(Axis axis, const char* motion)
{
    *this << "S3" << (axis == Axis_A ? '0' :
		      axis == Axis_B ? '1' :
		      axis == Axis_C ? '8' : '9')
	  << motion << endl;

    return *this;
}
    
}
