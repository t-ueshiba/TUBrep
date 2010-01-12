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
 *  $Id: TriggerGenerator.h,v 1.2 2010-01-12 01:44:55 ueshiba Exp $
 */
#ifndef __TUTriggerGenerator_h
#define __TUTriggerGenerator_h

#include "TU/Serial.h"

namespace TU
{
/************************************************************************
*  class TriggerGenerator						*
************************************************************************/
//! ���ʎY�Ɛ��g���K�M���������\���N���X
class __PORT TriggerGenerator : public Serial
{
  public:
    TriggerGenerator(const char* ttyname)				;

    void		showId(std::ostream& out)			;
    TriggerGenerator&	selectChannel(u_int channel)			;
    TriggerGenerator&	setInterval(u_int interval)			;
    TriggerGenerator&	oneShot()					;
    TriggerGenerator&	continuousShot()				;
    TriggerGenerator&	stopContinuousShot()				;
    bool		getStatus(u_int& channel, u_int& interval)	;
};

}

#endif	// !__TUTriggerGenerator_h
