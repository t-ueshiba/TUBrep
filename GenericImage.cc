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
 *  $Id: GenericImage.cc,v 1.8 2010-01-28 08:16:14 ueshiba Exp $
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
__PORT std::istream&
GenericImage::restoreData(std::istream& in)
{
    u_int	npads = 0;
    switch (_typeInfo.type)
    {
      case BMP_8:
	npads = 4 * ((width() + 3) / 4) - width();
	_colormap.resize(_typeInfo.ncolors);
	_colormap.restore(in);
	break;
      case BMP_24:
	npads = 4 * ((3*width() + 3) / 4) - 3*width();
      // fall back to the next case.
      default:
	_colormap.resize(0);
	break;
    }

    if (_typeInfo.bottomToTop)
    {
	for (u_int v = height(); v > 0; )
	    if (!(*this)[--v].restore(in) || !in.ignore(npads))
		break;
    }
    else
    {
	for (u_int v = 0; v < height(); )
	    if (!(*this)[v++].restore(in) || !in.ignore(npads))
		break;
    }

    return in;
}

//! �o�̓X�g���[���ɉ摜�̉�f�f�[�^�������o���D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
__PORT std::ostream&
GenericImage::saveData(std::ostream& out) const
{
    Array<u_char>	pad(0);
    switch (_typeInfo.type)
    {
      case BMP_8:
	pad.resize(4 * ((width() + 3) / 4) - width());
	_colormap.save(out);
	for (u_int i = _colormap.dim(); i < 256; ++i)
	    out.put(0).put(0).put(0).put(0);
	break;
      case BMP_24:
	pad.resize(4 * ((3*width() + 3) / 4) - 3*width());
	break;
    }
    pad = 0;

    if (_typeInfo.bottomToTop)
    {
	for (u_int v = height(); v > 0; )
	    if (!(*this)[--v].save(out) || !pad.save(out))
		break;
    }
    else
    {
	for (u_int v = 0; v < height(); )
	    if (!(*this)[v++].save(out) || !pad.save(out))
		break;
    }
    
    return out;
}

__PORT u_int
GenericImage::_width() const
{
    return (ncol()*8) / type2depth(_typeInfo.type);
}

__PORT u_int
GenericImage::_height() const
{
    return nrow();
}

__PORT ImageBase::Type
GenericImage::_defaultType() const
{
    return _typeInfo.type;
}

__PORT void
GenericImage::_resize(u_int h, u_int w, const TypeInfo& typeInfo)
{
    _typeInfo = typeInfo;
    w = (type2depth(_typeInfo.type)*w + 7) / 8;
    Array2<Array<u_char> >::resize(h, w);
}

}
