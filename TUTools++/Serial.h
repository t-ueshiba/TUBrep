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
 *  $Id: Serial.h,v 1.6 2009-09-04 04:01:06 ueshiba Exp $
 */
#if !defined(__TUSerial_h)
#define __TUSerial_h

#include <termios.h>
#include <cstdio>
#include "TU/Vector++.h"

namespace TU
{
/************************************************************************
*  class Serial								*
************************************************************************/
class __PORT Serial
{
  public:
			Serial(const char*)			;
    virtual		~Serial()				;

    const Serial&	put(const char* s)		const	;
    const Serial&	get(char* s, u_int size)	const	;
    
    Serial&		i_nl2cr()				;
    Serial&		i_igncr()				;
    Serial&		i_cr2nl()				;
#if !defined(__APPLE__)
    Serial&		i_upper2lower()				;
#endif
    Serial&		i_through()				;
    Serial&		o_nl2crnl()				;
#if !defined(__APPLE__)
    Serial&		o_cr2nl()				;
    Serial&		o_lower2upper()				;
#endif
    Serial&		o_through()				;
    Serial&		c_baud(int)				;
    Serial&		c_csize(int)				;
    Serial&		c_even()				;
    Serial&		c_odd()					;
    Serial&		c_noparity()				;
    Serial&		c_stop1()				;
    Serial&		c_stop2()				;

  protected:
    FILE*		fp()				const	{return _fp;}
    
  private:
    Serial&		set_flag(tcflag_t termios::* flag,
				 unsigned long clearbits,
				 unsigned long setbits)		;

    const int		_fd;
    FILE* const		_fp;
    termios		_termios_bak;
};

/************************************************************************
*  class TriggerGenerator						*
************************************************************************/
class __PORT TriggerGenerator : public Serial
{
  public:
    TriggerGenerator(const char* ttyname)			;

    TriggerGenerator&	showId(std::ostream& out)		;
    TriggerGenerator&	selectChannel(u_int channel)		;
    TriggerGenerator&	setInterval(u_int interval)		;
    TriggerGenerator&	oneShot()				;
    TriggerGenerator&	continuousShot()			;
    TriggerGenerator&	stopContinuousShot()			;
    bool		getStatus(u_int& channel,
				  u_int& interval)	const	;
};
 
}

#endif	/* !__TUSerial_h	*/
