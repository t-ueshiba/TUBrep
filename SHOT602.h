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
 *  $Id: SHOT602.h,v 1.1 2010-12-15 03:55:57 ueshiba Exp $
 */
#ifndef __TUSHOT602_h
#define __TUSHOT602_h

#include "TU/Serial.h"

namespace TU
{
/************************************************************************
*  class SHOT602							*
************************************************************************/
//! �V�O�}���@���p���X���[�^�R���g���[��SHOT-604�𐧌䂷��N���X
class __PORT SHOT602 : public Serial
{
  public:
  //! ��
    enum Axis
    {
	Axis_1,		//!< ��1��
	Axis_2,		//!< ��2��
	Axis_Both	//!< ��1,2���̗���
    };

  //! �X�s�[�h
    enum Speed
    {
	LowSpeed,	//!< �ᑬ
	HighSpeed	//!< ����
    };

  public:
    SHOT602(const char* ttyname)					;

  // �t�@�[���E�F�A�o�[�W����
    void	showId(std::ostream& out)				;

  // �z�[���|�W�V�������o
    SHOT602&	findHome(Axis axis, bool dir, bool dir2=true)		;
    SHOT602&	setOrigin(Axis axis)					;
    
  // ��Ԍ��o
    bool	isBusy()						;
    int		where(Axis axis)					;
    bool	atLimit(Axis axis)					;
    bool	getStatus(int& position1, int& position2,
			  bool& limit1, bool& limit2)			;

  // ���x�ݒ�
    SHOT602&	setSpeed(Speed speed,
			 u_int bottom1=0,   u_int top1=0,
			 u_int duration1=0, u_int bottom2=0,
			 u_int top2=0,	    u_int duration2=0)		;
    
  // �ړ�
    SHOT602&	stop(Axis axis)						;
    SHOT602&	emergencyStop()						;
    SHOT602&	jog(Axis axis, bool dir, bool dir2=true)		;
    SHOT602&	move(Axis axis, int val, int val2=0)			;

  // �㎥
    SHOT602&	setHold(Axis axis, bool on1, bool on2=true)		;

  private:
    SHOT602&	putCommand(Axis axis, char command,
			   const char* arg, const char* arg2,
			   bool putDelimieter=true)			;
};
    
}

#endif	// !__TUSHOT602_h
