/*
 *  ʿ��14-19ǯ���ȡ˻��ȵ�����縦��� �����ͭ
 *  
 *  �Ϻ�ԡ����ǽ���
 *
 *  �ܥץ����ϡ��ȡ˻��ȵ�����縦���ο����Ǥ��뿢�ǽ��פ��Ϻ��
 *  ���ȡ˻��ȵ�����縦��꤬������ͭ������̩����Ǥ��������ͭ
 *  �Ԥˤ����Ĥʤ����ܥץ�������ѡ�ʣ�������ѡ��軰�Ԥس�������
 *  ���ι԰٤�ػߤ��ޤ���
 *  
 *  ���Υץ����ˤ�ä������뤤���ʤ�»�����Ф��Ƥ⡤�����ͭ�Ԥ�
 *  ����Ϻ�Ԥ���Ǥ���餤�ޤ���
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
 *  $Id: io.h,v 1.2 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		io.h
  \brief	�Ƽ������ϴؿ�������ȼ���
*/
#ifndef __TUio_h
#define __TUio_h

#include <fstream>
#include <string>

namespace TU
{
/************************************************************************
*  global functions							*
************************************************************************/
std::string	openFile(std::ifstream& in, const std::string& name,
			 const std::string& dirs, const char* ext)	;
}

#endif	// !__TUio_h
