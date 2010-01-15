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
 *  $Id: TriggerGenerator.cc,v 1.22 2010-01-15 01:21:12 ueshiba Exp $
 */
#include "TU/TriggerGenerator.h"
#include "TU/Manip.h"
#include <iomanip>
#include <cstring>

namespace TU
{
/************************************************************************
*  class TriggerGenerator						*
************************************************************************/
//! �w�肳�ꂽtty��open���ăg���K�M������������D
/*!
  \param ttyname	tty��
*/
TriggerGenerator::TriggerGenerator(const char* ttyname)
    :Serial(ttyname)
{
    i_through()
	.o_through()
#if !defined(__APPLE__)
	.o_lower2upper()
#endif
	.c_baud(9600).c_csize(8).c_noparity().c_stop1();

    fill('0');
    setf(ios_base::internal, ios_base::adjustfield);
}

//! �t�@�[���E�F�A��ID���o�̓X�g���[���ɏ����o���D
/*!
  \param out	�o�̓X�g���[��
*/
void
TriggerGenerator::showId(std::ostream& out)
{
    using namespace	std;
    
    *this << 'V' << endl;
    for (char c; get(c); )
    {
	if (c == '\n')
	    break;
	out << c;
    }
    out << endl;

    *this >> skipl;
}

//! �g���K�M�����o�͂���`�����l�����w�肷��D
/*!
  \param channel	�o�̓`�����l���ɑΉ�����r�b�g��1�𗧂Ă��r�b�g�}�b�v
  \return		���̃g���K�M��������
*/
TriggerGenerator&
TriggerGenerator::selectChannel(u_int channel)
{
    using namespace	std;

    *this << 'A' << setw(8) << std::hex << channel << endl;
    *this >> skipl >> skipl;
    return *this;
}

//! �g���K�M���̏o�͊Ԋu���w�肷��D
/*!
  \param interval	�o�͊Ԋu(msec)�D10 <= interval <=
  \return		���̃g���K�M��������
*/
TriggerGenerator&
TriggerGenerator::setInterval(u_int interval)
{
    using namespace	std;
    
    if (10 <= interval && interval <= 255)
    {
	*this << 'F' << std::dec << interval << endl;
	*this >> skipl >> skipl;
    }
    return *this;
}

//! �g���K�M����1�����o�͂���D
/*!
  \return		���̃g���K�M��������
*/
TriggerGenerator&
TriggerGenerator::oneShot()
{
    using namespace	std;
    
    *this << 'T' << endl;
    *this >> skipl >> skipl;
    return *this;
}

//! �g���K�M����A���I�ɏo�͂���D
/*!
  \return		���̃g���K�M��������
*/
TriggerGenerator&
TriggerGenerator::continuousShot()
{
    using namespace	std;
    
    *this << 'R' << endl;
    *this >> skipl >> skipl;
    return *this;
}

//! �g���K�M�����~����D
/*!
  \return		���̃g���K�M��������
*/
TriggerGenerator&
TriggerGenerator::stopContinuousShot()
{
    using namespace	std;
    
    *this << 'S' << endl;
    *this >> skipl >> skipl;
    return *this;
}

//! �g���K�M��������̏�Ԃ��擾����D
/*!
  \param channel	�g���K�M�����o�͂���`�����l����1�𗧂Ă��r�b�g�}�b�v
			���Ԃ����
  \param interval	�g���K�M���̏o�͊Ԋu(msec)���Ԃ����
  \return		�g���K�M�����o�͒��Ȃ��true�C�����łȂ����false
*/
bool
TriggerGenerator::getStatus(u_int& channel, u_int& interval)
{
    using namespace	std;
    
    *this << 'I' << endl;

    char	c, token[5];	// token��"STOP"�܂���"RUN"�̂����ꂩ
    *this >> c >> std::hex >> channel >> c;
    *this >> c >> std::dec >> interval >> c >> token >> skipl >> skipl;
    
    return !strcmp(token, "RUN");
}

}

