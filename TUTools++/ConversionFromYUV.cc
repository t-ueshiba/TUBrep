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
 *  $Id: ConversionFromYUV.cc,v 1.10 2009-09-04 04:01:05 ueshiba Exp $
 */
#include "TU/Image++.h"

namespace TU
{
static inline int	flt2fix(float flt)	{return int(flt * (1 << 10));}

/************************************************************************
*  global variables							*
************************************************************************/
__PORT const ConversionFromYUV	conversionFromYUV;

/************************************************************************
*  class ConversionFromYUV						*
************************************************************************/
ConversionFromYUV::ConversionFromYUV()
{
    for (int i = 0; i < 256; ++i)
    {
	_r [i] = int(1.4022f * (i - 128));
	_g0[i] = flt2fix(0.7144f * (i - 128));
	_g1[i] = flt2fix(0.3457f * (i - 128));
	_b [i] = int(1.7710f * (i - 128));
    }
}
 
}
