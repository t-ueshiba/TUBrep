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
 *  $Id: CorrectIntensity.h,v 1.4 2009-07-31 07:04:44 ueshiba Exp $
 */
#ifndef	__TUCorrectIntensity_h
#define	__TUCorrectIntensity_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class CorrectIntensity						*
************************************************************************/
//! �摜�̐��`�P�x�␳���s���N���X
class CorrectIntensity
{
  public:
  //! �I�t�Z�b�g�ƃQ�C�����w�肵�ċP�x�␳�I�u�W�F�N�g�𐶐�����D
  /*!
    \param offset	�I�t�Z�b�g
    \param gain		�Q�C��
  */
    CorrectIntensity(float offset=0.0, float gain=1.0)
	:_offset(offset), _gain(gain) 					{}

    void	initialize(float offset, float gain)			;
    template <class T>
    void	operator()(Image<T>& image, int vs=0, int ve=0)	const	;
    
  private:
    template <class T>
    T		val(T pixel)					const	;
    
    float	_offset;
    float	_gain;
};

//! �I�t�Z�b�g�ƃQ�C�����w�肵�ċP�x�␳�I�u�W�F�N�g������������D
/*!
  \param offset		�I�t�Z�b�g
  \param gain		�Q�C��
*/
inline void
CorrectIntensity::initialize(float offset, float gain)
{
    _offset = offset;
    _gain   = gain;
}

}

#endif	/* !__TUCorrectIntensity_h */
