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
 *  $Id: TriggerGenerator.cc,v 1.16 2009-07-31 07:04:45 ueshiba Exp $
 */
#include <cstdlib>
#include "TU/Serial.h"

namespace TU
{
/************************************************************************
*  static functions							*
************************************************************************/
static void
skipl(FILE* fp)
{
    for (int c; (c = fgetc(fp)) != EOF; )
	if (c == '\n')
	    break;
}
    
/************************************************************************
*  class TriggerGenerator						*
************************************************************************/
TriggerGenerator::TriggerGenerator(const char* ttyname)
    :Serial(ttyname)
{
    i_through()
	.o_through()
#if !defined(__APPLE__)
	.o_lower2upper()
#endif
	.c_baud(9600).c_csize(8).c_noparity().c_stop1();
}

TriggerGenerator&
TriggerGenerator::showId(std::ostream& out)
{
    using namespace	std;
    
    fputs("V\n", fp());
    for (int c; (c = fgetc(fp())) != EOF; )
    {
	out << char(c);
	if (c == '\n')
	    break;
    }
    return *this;
}

TriggerGenerator&
TriggerGenerator::selectChannel(u_int channel)
{
    fprintf(fp(), "A%0.8x\n", channel);
    skipl(fp());
    return *this;
}

TriggerGenerator&
TriggerGenerator::setInterval(u_int interval)
{
    if (10 <= interval && interval <= 255)
    {
	fprintf(fp(), "F%d\n", interval);
	skipl(fp());
    }
    return *this;
}

TriggerGenerator&
TriggerGenerator::oneShot()
{
    fputs("T\n", fp());
    skipl(fp());
    return *this;
}

TriggerGenerator&
TriggerGenerator::continuousShot()
{
    fputs("R\n", fp());
    skipl(fp());
    return *this;
}

TriggerGenerator&
TriggerGenerator::stopContinuousShot()
{
    fputs("S\n", fp());
    skipl(fp());
    return *this;
}

bool
TriggerGenerator::getStatus(u_int& channel, u_int& interval) const
{
    fputs("I\n", fp());

    char	token[64], *p = token;
    for (int c; (c = fgetc(fp())) != EOF; )
    {
	if (c == '\n')
	    break;
	else if (c == ',')
	{
	    *p = '\0';
	    if (token[0] == 'A')
		channel = strtoul(token + 1, NULL, 16);
	    else
		interval = strtoul(token + 1, NULL, 10);
	    p = token;
	}
	else
	    *p++ = c;
    }
    *p = '\0';
    
    return !strcmp(token, "RUN");
}

}

