/*
 *  平成14-19年（独）産業技術総合研究所 著作権所有
 *  
 *  創作者：植芝俊夫
 *
 *  本プログラムは（独）産業技術総合研究所の職員である植芝俊夫が創作し，
 *  （独）産業技術総合研究所が著作権を所有する秘密情報です．著作権所有
 *  者による許可なしに本プログラムを使用，複製，改変，第三者へ開示する
 *  等の行為を禁止します．
 *  
 *  このプログラムによって生じるいかなる損害に対しても，著作権所有者お
 *  よび創作者は責任を負いません。
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
//! 入力ストリームから画像の画素データを読み込む．
/*!
  \param in	入力ストリーム
  \return	inで指定した入力ストリーム
*/
std::istream&
GenericImage::restoreData(std::istream& in)
{
    for (u_int v = 0; v < height(); )
  	if (!(*this)[v++].restore(in))
	    break;
    return in;
}

//! 出力ストリームに画像の画素データを書き出す．
/*!
  \param out	出力ストリーム
  \return	outで指定した出力ストリーム
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
