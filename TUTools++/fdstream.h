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
 *  $Id: fdstream.h,v 1.1 2010-01-12 01:44:55 ueshiba Exp $
 */
#ifndef __TUfdstream_h
#define __TUfdstream_h

#include "TU/types.h"
#include <iostream>
#include <streambuf>

namespace TU
{
/************************************************************************
*  class fdbuf								*
************************************************************************/
//! �t�@�C���L�q�q�����X�g���[���o�b�t�@�N���X
/*!
  #TU::fdistream, #TU::fdostream, #TU::fdstream�̓����Ŏg����D
*/
class __PORT fdbuf : public std::streambuf
{
  public:
    typedef std::streambuf::traits_type			traits_type;
    typedef traits_type::int_type			int_type;

  public:
    fdbuf(int fd, bool closeFdOnClosing)				;
    virtual			~fdbuf()				;

    int				fd()				const	;
    
  protected:
    virtual int_type		underflow()				;
    virtual int_type		overflow(int_type c)			;
    virtual std::streamsize	xsputn(const char* s, std::streamsize n);

  protected:
    enum
    {
	pbSize	= 4,			//!< putback�̈�̍ő啶����
	bufSize	= 1024			//!< �ʏ�ǂݍ��ݗ̈�̍ő啶����
    };
    
    const int	_fd;			//!< �t�@�C���L�q�q
    const bool	_closeFdOnClosing;	//!< ���̃o�b�t�@�̔j�󎞂�_fd��close
    char	_buf[bufSize + pbSize];	//!< �ǂݍ��݃f�[�^�̈�
};

//! �t�@�C���L�q�q��Ԃ��D
/*!
  \return	�t�@�C���L�q�q
*/
inline int
fdbuf::fd() const
{
    return _fd;
}
    
/************************************************************************
*  class fdistream							*
************************************************************************/
//! �t�@�C���L�q�q�������̓X�g���[���N���X
class __PORT fdistream : public std::istream
{
  public:
    fdistream(const char* path)						;
    fdistream(int fd)							;
    
    int		fd()						const	;
    
  protected:
    fdbuf	_buf;
};

//! �w�肵���t�@�C���L�q�q������̓X�g���[�������D
/*!
  ���̃X�g���[�����j�󂳂�Ă��t�@�C���L�q�q��close����Ȃ��D
  \param fd	���͉\�ȃt�@�C���L�q�q
*/
inline
fdistream::fdistream(int fd)
    :std::istream(0), _buf(fd, false)
{
    rdbuf(&_buf);
}

//! �t�@�C���L�q�q��Ԃ��D
/*!
  \return	�t�@�C���L�q�q
*/
inline int
fdistream::fd() const
{
    return _buf.fd();
}

/************************************************************************
*  class fdostream							*
************************************************************************/
//! �t�@�C���L�q�q�����o�̓X�g���[���N���X
class __PORT fdostream : public std::ostream
{
  public:
    fdostream(const char* path)						;
    fdostream(int fd)							;
    
    int		fd()						const	;
    
  protected:
    fdbuf	_buf;
};

//! �w�肵���t�@�C���L�q�q����o�̓X�g���[�������D
/*!
  ���̃X�g���[�����j�󂳂�Ă��t�@�C���L�q�q��close����Ȃ��D
  \param fd	�o�͉\�ȃt�@�C���L�q�q
*/
inline
fdostream::fdostream(int fd)
    :std::ostream(0), _buf(fd, false)
{
    rdbuf(&_buf);
}

//! �t�@�C���L�q�q��Ԃ��D
/*!
  \return	�t�@�C���L�q�q
*/
inline int
fdostream::fd() const
{
    return _buf.fd();
}

/************************************************************************
*  class fdstream							*
************************************************************************/
//! �t�@�C���L�q�q�������o�̓X�g���[���N���X
class __PORT fdstream : public std::iostream
{
  public:
    fdstream(const char* path)						;
    fdstream(int fd)							;
    
    int		fd()						const	;
    
  protected:
    fdbuf	_buf;
};

//! �w�肵���t�@�C���L�q�q������o�̓X�g���[�������D
/*!
  ���̃X�g���[�����j�󂳂�Ă��t�@�C���L�q�q��close����Ȃ��D
  \param fd	���o�͉\�ȃt�@�C���L�q�q
*/
inline
fdstream::fdstream(int fd)
    :std::iostream(0), _buf(fd, false)
{
    rdbuf(&_buf);
}

//! �t�@�C���L�q�q��Ԃ��D
/*!
  \return	�t�@�C���L�q�q
*/
inline int
fdstream::fd() const
{
    return _buf.fd();
}

}

#endif	// !__TUfdstream_h
