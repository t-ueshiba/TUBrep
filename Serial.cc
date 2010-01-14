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
 *  $Id: Serial.cc,v 1.21 2010-01-14 11:13:09 ueshiba Exp $
 */
#include "TU/Serial.h"
#include <stdexcept>
#include <cstring>
#include <errno.h>

namespace TU
{
/************************************************************************
*  Public member functions						*
************************************************************************/
//! �w�肳�ꂽtty��open���ăV���A���|�[�g�����D
/*!
  \param ttyname	tty��
*/
Serial::Serial(const char* ttyname)
    :fdstream(ttyname)
{
    using namespace	std;

  // Flush everything in the buffer.
    if (::tcflush(fd(), TCIOFLUSH))
	throw runtime_error(string("TU::Serial::Serial: cannot flush tty; ")
			    + strerror(errno));

  // Keep the original termios settings.
    termios	termios;
    if (::tcgetattr(fd(), &termios) == -1)
	throw runtime_error(string("TU::Serial::Serial: tcgetattr; ")
			    + strerror(errno));
    _termios_bak = termios;		// backup termios structure

  // Set local modes and control characters.
    termios.c_lflag &= ~(ICANON | ECHO | ISIG);
    termios.c_cc[VMIN]  = 1;
    termios.c_cc[VTIME] = 0;
    if (::tcsetattr(fd(), TCSANOW, &termios) == -1)
	throw runtime_error(string("TU::Serial::Serial: tcsetattr; ")
			    + strerror(errno));
}

//! �V���A���|�[�g��j�󂷂�D
/*!
  �V���A���|�[�g�n�[�h�E�F�A�̓|�[�g�����O�̏�Ԃɖ߂����D
*/
Serial::~Serial()
{
    if (fd() >= 0)
    {
	::tcsetattr(fd(), TCSANOW, &_termios_bak);
	::close(fd());
    }
}

//! NL��CR�ɕϊ����ē��͂���D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::i_nl2cr()		// '\n' -> '\r'
{
    return set_flag(&termios::c_iflag, ICRNL, INLCR);
}

//! ���͂����CR�𖳎�����D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::i_igncr()		// don't read '\r'
{
    return set_flag(&termios::c_iflag, INLCR|ICRNL, IGNCR);
}

//! CR��NL�ɕϊ����ē��͂���D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::i_cr2nl()		// '\r' -> '\n'
{
    return set_flag(&termios::c_iflag, INLCR, ICRNL);
}

#if !defined(__APPLE__)
//! �啶�����������ɕϊ����ē��͂���D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::i_upper2lower()		// upper -> lower
{
    return set_flag(&termios::c_iflag, 0, IUCLC);
}
#endif

//! �����ϊ������ɓ��͂���D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::i_through()		// read transparently
{
#if !defined(__APPLE__)
    return set_flag(&termios::c_iflag, INLCR|IGNCR|ICRNL|IUCLC, 0);
#else
    return set_flag(&termios::c_iflag, INLCR|IGNCR|ICRNL, 0);
#endif
}

//! NL��CR+NL�ɕϊ����ďo�͂���D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::o_nl2crnl()		// '\r\n' <- "\n"
{
#if !defined(__APPLE__)
    return set_flag(&termios::c_oflag, OCRNL, OPOST|ONLCR);
#else
    return set_flag(&termios::c_oflag, 0, OPOST|ONLCR);
#endif
}

#if !defined(__APPLE__)
//! NL��CR+NL�ɕϊ����ďo�͂���D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::o_cr2nl()		// '\n' <- '\r'
{
    return set_flag(&termios::c_oflag, ONLCR, OPOST|OCRNL);
}

//! ��������啶���ɕϊ����ďo�͂���D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::o_lower2upper()		// upper <- lower
{
    return set_flag(&termios::c_oflag, 0, OPOST|OLCUC);
}
#endif

//! �����ϊ������ɏo�͂���D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::o_through()		// write transparently
{
    return set_flag(&termios::c_oflag, OPOST, 0);
}

//! �{�[���[�g��ݒ肷��D
/*!
  \param baud	�{�[���[�g�D50, 75, 110, 134, 150, 200, 300, 600,
		1200, 1800, 2400, 4800. 9600, 19200, 38400�̂����ꂩ�D
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::c_baud(int baud)	// set baud rate
{
    using namespace	std;

#if !defined(__APPLE__)
    switch (baud)
    {
      case 50:
	return set_flag(&termios::c_cflag, CBAUD, B50);
      case 75:
	return set_flag(&termios::c_cflag, CBAUD, B75);
      case 110:
	return set_flag(&termios::c_cflag, CBAUD, B110);
      case 134:
	return set_flag(&termios::c_cflag, CBAUD, B134);
      case 150:
	return set_flag(&termios::c_cflag, CBAUD, B150);
      case 200:
	return set_flag(&termios::c_cflag, CBAUD, B200);
      case 300:
	return set_flag(&termios::c_cflag, CBAUD, B300);
      case 600:
	return set_flag(&termios::c_cflag, CBAUD, B600);
      case 1200:
	return set_flag(&termios::c_cflag, CBAUD, B1200);
      case 1800:
	return set_flag(&termios::c_cflag, CBAUD, B1800);
      case 2400:
	return set_flag(&termios::c_cflag, CBAUD, B2400);
      case 4800:
	return set_flag(&termios::c_cflag, CBAUD, B4800);
      case 9600:
	return set_flag(&termios::c_cflag, CBAUD, B9600);
      case 19200:
	return set_flag(&termios::c_cflag, CBAUD, B19200);
      case 38400:
	return set_flag(&termios::c_cflag, CBAUD, B38400);
      default:
	throw runtime_error("TU::Serial::c_baud: unknown baud rate!");
    }
#else
    termios		termios;
    if (::tcgetattr(fd(), &termios) == -1)
	throw runtime_error(string("TU::Serial::c_baud: tcgetattr; ")
			    + strerror(errno));
    termios.c_ispeed = termios.c_ospeed = baud;
    if (::tcsetattr(fd(), TCSANOW, &termios) == -1)
	throw runtime_error(string("TU::Serial::c_baud: tcsetattr; ")
			    + strerror(errno));
#endif
    return *this;
}

//! �����T�C�Y��ݒ肷��D
/*!
  \param csize	�����T�C�Y�D5, 6, 7, 8�̂����ꂩ�D
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::c_csize(int csize)	// set character size
{
    using namespace	std;
    
    switch (csize)
    {
      case 5:
	return set_flag(&termios::c_cflag, CSIZE, CS5);
      case 6:
	return set_flag(&termios::c_cflag, CSIZE, CS6);
      case 7:
	return set_flag(&termios::c_cflag, CSIZE, CS7);
      case 8:
	return set_flag(&termios::c_cflag, CSIZE, CS8);
      default:
	throw runtime_error("TU::Serial::c_baud: unknown character size!");
    }
    return *this;
}

//! �p���e�B�`�F�b�N�������ɐݒ肷��D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::c_even()		// even parity
{
    return set_flag(&termios::c_cflag, PARODD, PARENB);
}

//! �p���e�B�`�F�b�N����ɐݒ肷��D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::c_odd()			// odd parity
{
    return set_flag(&termios::c_cflag, 0, PARENB|PARODD);
}

//! �p���e�B�`�F�b�N�Ȃ��ɐݒ肷��D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::c_noparity()		// no parity
{
    return set_flag(&termios::c_cflag, PARENB, 0);
}

//! �X�g�b�v�r�b�g��1�ɐݒ肷��D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::c_stop1()		// 1 stop bit
{
    return set_flag(&termios::c_cflag, CSTOPB, 0);
}

//! �X�g�b�v�r�b�g��2�ɐݒ肷��D
/*!
  \return	���̃V���A���|�[�g
*/
Serial&
Serial::c_stop2()		// 2 stop bits
{
    return set_flag(&termios::c_cflag, 0, CSTOPB);
}

/************************************************************************
*  Private member functions						*
************************************************************************/ 
Serial&
Serial::set_flag(tcflag_t termios::* flag,
		 unsigned long clearbits, unsigned long setbits)
{
    using namespace	std;
    
    termios		termios;
    if (::tcgetattr(fd(), &termios) == -1)
	throw runtime_error(string("TU::Serial::set_flag: tcgetattr; ")
			    + strerror(errno));
    termios.*flag &= ~clearbits;
    termios.*flag |= setbits;
    if (::tcsetattr(fd(), TCSANOW, &termios) == -1)
	throw runtime_error(string("TU::Serial::set_flag: tcsetattr; ")
			    + strerror(errno));
    return *this;
}

}

