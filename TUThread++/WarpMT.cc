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
 *  $Id: WarpMT.cc,v 1.4 2010-07-07 07:33:39 ueshiba Exp $  
 */
#include "TU/Thread++.h"
#include "TU/Warp.h"

namespace TU
{
/************************************************************************
*  class MultiThread2<Warp, Image<T> >					*
************************************************************************/
template <> __PORT void
MultiThread2<Warp, Image<u_char> >::
  operator ()(const Image<u_char>& in, Image<u_char>& out) const
{
    out.resize(height(), width());
    raiseThreads(in, out);
}
    
template <> __PORT void
MultiThread2<Warp, Image<RGBA> >::
  operator ()(const Image<RGBA>& in, Image<RGBA>& out) const
{
    out.resize(height(), width());
    raiseThreads(in, out);
}
    
template <> __PORT void
MultiThread2<Warp, Image<ABGR> >::
  operator ()(const Image<ABGR>& in, Image<ABGR>& out) const
{
    out.resize(height(), width());
    raiseThreads(in, out);
}
    
}
