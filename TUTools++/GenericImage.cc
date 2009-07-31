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
 *  $Id: GenericImage.cc,v 1.6 2009-07-31 07:04:44 ueshiba Exp $
 */
#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class GenericImage							*
************************************************************************/
//! ���̓X�g���[������摜�̉�f�f�[�^��ǂݍ��ށD
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
std::istream&
GenericImage::restoreData(std::istream& in)
{
    for (u_int v = 0; v < height(); )
  	if (!(*this)[v++].restore(in))
	    break;
    return in;
}

//! �o�̓X�g���[���ɉ摜�̉�f�f�[�^�������o���D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
std::ostream&
GenericImage::saveData(std::ostream& out) const
{
    for (u_int v = 0; v < height(); )
	if (!(*this)[v++].save(out))
	    break;
    return out;
}

u_int
GenericImage::_width() const
{
    return (ncol()*8) / type2depth(_type);
}

u_int
GenericImage::_height() const
{
    return nrow();
}

ImageBase::Type
GenericImage::_defaultType() const
{
    return type();
}

void
GenericImage::_resize(u_int h, u_int w, ImageBase::Type type)
{
    _type = type;
    w = (type2depth(_type)*w - 1)/8 + 1;
    Array2<Array<u_char> >::resize(h, w);
}

}
