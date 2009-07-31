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
 *  $Id: Mapping.cc,v 1.3 2009-07-31 07:04:45 ueshiba Exp $
 */
#include "TU/Mapping.h"

namespace TU
{
/************************************************************************
*  class ProjectiveMapping						*
************************************************************************/
//! ���͋�ԂƏo�͋�Ԃ̎������w�肵�Ďˉe�ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  �P���ϊ��Ƃ��ď����������D
  \param inDim	���͋�Ԃ̎���
  \param outDim	�o�͋�Ԃ̎���
*/
ProjectiveMapping::ProjectiveMapping(u_int inDim, u_int outDim)
    :_T(outDim + 1, inDim + 1)
{
    u_int	n = std::min(inDim, outDim);
    for (u_int i = 0; i < n; ++i)
	_T[i][i] = 1.0;
    _T[outDim][inDim] = 1.0;
}
    
/************************************************************************
*  class AffineMapping							*
************************************************************************/
//! ���̃A�t�B���ϊ��̕��s�ړ�������\������x�N�g����Ԃ��D
/*! 
  \return	#outDim()�����x�N�g��
*/
Vector<double>
AffineMapping::b() const
{
    Vector<double>	bb(outDim());
    for (u_int j = 0; j < bb.dim(); ++j)
	bb[j] = _T[j][inDim()];

    return bb;
}
    
}

