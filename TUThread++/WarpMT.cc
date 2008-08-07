/*
 *  ʿ��14-19ǯ���ȡ˻��ȵ�����縦��� �����ͭ
 *  
 *  �Ϻ�ԡ����ǽ���
 *
 *  �ܥץ����ϡ��ȡ˻��ȵ�����縦���ο����Ǥ��뿢�ǽ��פ��Ϻ��
 *  ���ȡ˻��ȵ�����縦��꤬������ͭ������̩����Ǥ����Ϻ�Ԥˤ�
 *  ����Ĥʤ����ܥץ�������ѡ�ʣ�������ѡ��軰�Ԥس�������������
 *  ��򿯳�����԰٤�ػߤ��ޤ���
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
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: WarpMT.cc,v 1.1 2008-08-07 08:11:23 ueshiba Exp $
 */
#include "TU/Thread++.h"
#include "TU/Warp.h"

namespace TU
{
/************************************************************************
*  class MultiThread2<Warp, Image<T> >					*
************************************************************************/
template <> void
MultiThread2<Warp, Image<u_char> >::
  operator ()(const Image<u_char>& in, Image<u_char>& out) const
{
    out.resize(height(), width());
    raiseThreads(in, out);
}
    
template <> void
MultiThread2<Warp, Image<RGBA> >::
  operator ()(const Image<RGBA>& in, Image<RGBA>& out) const
{
    out.resize(height(), width());
    raiseThreads(in, out);
}
    
template <> void
MultiThread2<Warp, Image<ABGR> >::
  operator ()(const Image<ABGR>& in, Image<ABGR>& out) const
{
    out.resize(height(), width());
    raiseThreads(in, out);
}
    
}
