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
 *  $Id: fdstream.cc,v 1.3 2010-01-27 08:09:59 ueshiba Exp $
 */
#include "TU/fdstream.h"
#include <stdexcept>
#include <cstring>	// for memmove()
#include <fcntl.h>
#ifdef WIN32
#  include <io.h>	// for read() and write()
#else
#  include <unistd.h>	// for read() and write()
#endif

namespace TU
{
/************************************************************************
*  class fdbuf								*
************************************************************************/
//! �w�肵���t�@�C���L�q�q����X�g���[���o�b�t�@�����D
/*!
  \param fd			�t�@�C���L�q�q
  \param closeFdOnClosing	true�Ȃ�΂��̃X�g���[���o�b�t�@�̔j�󎞂�
				�t�@�C���L�q�q��close
*/
fdbuf::fdbuf(int fd, bool closeFdOnClosing)
    :_fd(fd), _closeFdOnClosing(closeFdOnClosing)
{
    using namespace	std;
    
    if (_fd < 0)
	throw runtime_error("TU::fdbuf::fdbuf: invalid file descriptor!");
    setg(_buf + pbSize, _buf + pbSize, _buf + pbSize);
}

//! �X�g���[���o�b�t�@��j�󂷂�D
fdbuf::~fdbuf()
{
    if (_closeFdOnClosing && _fd >= 0)
	::close(_fd);
}
    
//! �t�@�C�����當������o�b�t�@�ɓǂݍ��ށD
/*!
  \return	���[�U���ɕԂ���Ă��Ȃ�����������΁C���̍ŏ��̕����D
		�Ȃ����EOF�D
*/
fdbuf::int_type
fdbuf::underflow()
{
#ifndef WIN32
    using std::memmove;
#endif
    if (gptr() < egptr())		// ���݈ʒu�̓o�b�t�@�I�[�����O�H
	return traits_type::to_int_type(*gptr());

    int	numPutback = gptr() - eback();	// �ȑO�ɓǂݍ��܂ꂽ������
    if (numPutback > pbSize)
	numPutback = pbSize;		// putback�̈�̃T�C�Y�ɐ؂�l��

  // �ȑO�ɓǂݍ��܂�Ă������������XpbSize����putback�̈�ɃR�s�[
    memmove(_buf + (pbSize - numPutback), gptr() - numPutback, numPutback);

  // ���XbufSize�̕�����V���ɓǂݍ���
    int	num = read(_fd, _buf + pbSize, bufSize);
    if (num <= 0)
	return traits_type::eof();

  // �o�b�t�@�̃|�C���^���Z�b�g������
    setg(_buf + (pbSize - numPutback), _buf + pbSize, _buf + pbSize + num);

    return traits_type::to_int_type(*gptr());	// ���̕�����Ԃ�
}

//! �t�@�C���ɕ����������o���D
/*!
  \param c	�����o������
  \return	�����o���ɐ�������΂��̕����D���s�����EOF�D
*/
fdbuf::int_type
fdbuf::overflow(int_type c)
{
    if (c != traits_type::eof())
    {
	char	z = c;
	if (write (_fd, &z, 1) != 1)
	    return traits_type::eof();
    }
    return c;
}

//! �t�@�C���ɕ�����������o���D
/*!
  \param s	�����o��������
  \return	�����o����������
*/
std::streamsize
fdbuf::xsputn(const char* s, std::streamsize n)
{
    return write(_fd, s, n);
}

/************************************************************************
*  class fdistream							*
************************************************************************/
//! �w�肵���t�@�C����������̓X�g���[�������D
/*!
  ���̃X�g���[�����j�󂳂��ƃt�@�C����close�����D
  \param path	�t�@�C����
*/
fdistream::fdistream(const char* path)
    :std::istream(0), _buf(::open(path, O_RDONLY), true)
{
    rdbuf(&_buf);
}

/************************************************************************
*  class fdostream							*
************************************************************************/
//! �w�肵���t�@�C��������o�̓X�g���[�������D
/*!
  ���̃X�g���[�����j�󂳂��ƃt�@�C����close�����D
  \param path	�t�@�C����
*/
fdostream::fdostream(const char* path)
    :std::ostream(0), _buf(::open(path, O_WRONLY), true)
{
    rdbuf(&_buf);
}
    
/************************************************************************
*  class fdstream							*
************************************************************************/
//! �w�肵���t�@�C����������o�̓X�g���[�������D
/*!
  ���̃X�g���[�����j�󂳂��ƃt�@�C����close�����D
  \param path	�t�@�C����
*/
fdstream::fdstream(const char* path)
    :std::iostream(0), _buf(::open(path, O_RDWR), true)
{
    rdbuf(&_buf);
}
    
}
