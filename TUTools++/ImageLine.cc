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
 *  $Id: ImageLine.cc,v 1.7 2009-07-31 07:04:44 ueshiba Exp $
 */
#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class ImageLine<YUV422>						*
************************************************************************/
const YUV444*
ImageLine<YUV422>::fill(const YUV444* src)
{
    register YUV422* dst = *this;
    for (register int n = dim(); n > 0; n -= 2)
    {
	dst->x = src->u;
	dst->y = src->y;
	++dst;
	dst->x = src->v;
	++src;
	dst->y = src->y;
	++dst;
	++src;
    }
    return src;
}

const YUV411*
ImageLine<YUV422>::fill(const YUV411* src)
{
    register YUV422* dst = *this;
    for (register int n = dim(); n > 0; n -= 4)
    {
	dst->x = src[0].x;
	dst->y = src[0].y0;
	++dst;
	dst->x = src[1].x;
	dst->y = src[0].y1;
	++dst;
	dst->x = src[0].x;
	dst->y = src[1].y0;
	++dst;
	dst->x = src[1].x;
	dst->y = src[1].y1;
	++dst;
	src += 2;
    }
    return src;
}

/************************************************************************
*  class ImageLine<YUV411>						*
************************************************************************/
const YUV444*
ImageLine<YUV411>::fill(const YUV444* src)
{
    register YUV411* dst = *this;
    for (register int n = dim(); n > 0; n -= 2)
    {
	dst->x  = src[0].u;
	dst->y0 = src[0].y;
	dst->y1 = src[1].y;
	++dst;
	dst->x  = src[0].v;
	dst->y0 = src[2].y;
	dst->y1 = src[3].y;
	++dst;
	src += 4;
    }
    return src;
}

const YUV422*
ImageLine<YUV411>::fill(const YUV422* src)
{
    register YUV411* dst = *this;
    for (register int n = dim(); n > 0; n -= 2)
    {
	dst->x  = src[0].x;
	dst->y0 = src[0].y;
	dst->y1 = src[1].y;
	++dst;
	dst->x  = src[1].x;
	dst->y0 = src[2].y;
	dst->y1 = src[3].y;
	++dst;
	src += 4;
    }
    return src;
}
 
}
