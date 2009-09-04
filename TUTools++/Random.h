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
 *  $Id: Random.h,v 1.8 2009-09-04 04:01:06 ueshiba Exp $
 */
#include "TU/types.h"

namespace TU
{
/************************************************************************
*  class Random								*
************************************************************************/
class __PORT Random
{
  public:
    Random()						;
    
    double	uniform()				;
    double	gaussian()				;
    double	uniform48()				;
    double	gaussian48()				;
    
  private:
    double	gaussian(double (Random::*uni)())	;
    
    int		_seed;
    long	_x1, _x2, _x3;
    double	_r[97];
    int		_ff;
    int		_has_extra;	// flag showing existence of _extra.
    double	_extra;		// extra gaussian noise value.
};

inline double
Random::gaussian()
{
    return gaussian(&Random::uniform);
}

inline double
Random::gaussian48()
{
    return gaussian(&Random::uniform48);
}
 
}
