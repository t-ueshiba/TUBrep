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
 *  $Id: io.cc,v 1.1 2011-01-17 00:04:33 ueshiba Exp $
 */
#include "TU/io.h"
#include <algorithm>
#include <stdexcept>

namespace TU
{
/************************************************************************
*  global functions							*
************************************************************************/
//! ���ꤷ�����ϥե�����򥪡��ץ󤹤롥
/*!
  \param in	�����ץ󤵤줿�ե����뤬��ӤĤ��������ϥ��ȥ꡼��
  \param name	�ե�����̾(��ĥ�Ҥ�ޤޤ�)
  \param dirs	':'�Ƕ��ڤ�줿�ե�����õ���ǥ��쥯�ȥ���¤�
  \param ext	�ե�����γ�ĥ�ҡ�0����ꤹ��г�ĥ�Ҥʤ�
  \return	�����ץ󤵤줿�ե������full path̾(��ĥ�Ҥ�ޤޤ�)
*/
std::string
openFile(std::ifstream& in, const std::string& name,
	 const std::string& dirs, const char* ext)
{
    using namespace		std;

    string::const_iterator	p = dirs.begin();
    do
    {
	string::const_iterator	q = find(p, dirs.end(), ':');
	string			fullName = string(p, q) + '/' + name;
	in.open((ext ? fullName + ext : fullName).c_str());
	if (in)
	    return fullName;
	p = q;
    } while (p++ != dirs.end());

    throw runtime_error("Cannot open file \"" + name + ext +
			"\" in \"" + dirs + "\"!!");
    return string();
}

}
