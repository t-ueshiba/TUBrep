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
 *  $Id: CorrectIntensity.cc,v 1.8 2009-09-11 05:56:13 ueshiba Exp $
 */
#include "TU/CorrectIntensity.h"
#include "TU/mmInstructions.h"

namespace TU
{
/************************************************************************
*  static functions							*
************************************************************************/
#if defined(SSE)
template <class T> static inline void
mmCorrect(T* p, mmFlt a, mmFlt b)
{
    const mmUInt8	val = mmLoadU((const u_char*)p);
#  if defined(SSE2)
    mmStoreU((u_char*)p,
	     mmCvt<mmUInt8>(
		 mmCvt<mmInt16>(
		     mmCvt<mmInt32>(a + b * mmCvt<mmFlt>(val)),
		     mmCvt<mmInt32>(a + b * mmCvt<mmFlt>(
					mmShiftElmR<mmFlt::NElms>(val)))),
		 mmCvt<mmInt16>(
		     mmCvt<mmInt32>(a + b * mmCvt<mmFlt>(
					mmShiftElmR<2*mmFlt::NElms>(val))),
		     mmCvt<mmInt32>(a + b * mmCvt<mmFlt>(
					mmShiftElmR<3*mmFlt::NElms>(val))))));
#  else
    mmStoreU((u_char*)p,
	     mmCvt<mmUInt8>(
		 mmCvt<mmInt16>(a + b * mmCvt<mmFlt>(val)),
		 mmCvt<mmInt16>(a + b * mmCvt<mmFlt>(
				    mmShiftElmR<mmFlt::NElms>(val)))));
#  endif
}

template <> inline void
mmCorrect(short* p, mmFlt a, mmFlt b)
{
#  if defined(SSE2)
    const mmInt16	val = mmLoadU(p);
    mmStoreU(p, mmCvt<mmInt16>(
		 mmCvt<mmInt32>(a + b * mmCvt<mmFlt>(val)),
		 mmCvt<mmInt32>(a + b * mmCvt<mmFlt>(
				    mmShiftElmR<mmFlt::NElms>(val)))));
#  else
    mmStoreU(p, mmCvt<mmInt16>(a + b * mmCvt<mmFlt>(mmLoadU(p))));
#  endif
}

template <> inline void
mmCorrect(float* p, mmFlt a, mmFlt b)
{
    mmStoreU(p, a + b * mmLoadU(p));
}
#endif

static inline u_char
toUChar(float val)
{
    return (val < 0.0 ? 0 : val > 255.0 ? 255 : u_char(val));
}
    
static inline short
toShort(float val)
{
    return (val < -32768.0 ? -32768 : val > 32767.0 ? 32767 : short(val));
}
    
/************************************************************************
*  class CorrectIntensity						*
************************************************************************/
//! �^����ꂽ�摜�̋P�x��␳����D
/*!
  \param image		���͉摜��^���C�␳���ʂ�����ɕԂ����
  \param vs		�P�x��␳����̈�̍ŏ��̍s���w�肷��index
  \param ve		�P�x��␳����̈�̍Ō�̍s�̎����w�肷��index
*/ 
template <class T> void
CorrectIntensity::operator()(Image<T>& image, u_int vs, u_int ve) const
{
    if (ve == 0)
	ve = image.height();
    
    for (u_int v = vs; v < ve; ++v)
    {
	T*		p = image[v];
	T* const	q = p + image.width();
#if defined(SSE)
	const mmFlt	a = mmSetAll<mmFlt>(_offset),
			b = mmSetAll<mmFlt>(_gain);
	for (T* const r = q - mmNBytes/sizeof(T);
	     p <= r; p += mmNBytes/sizeof(T))
	    mmCorrect(p, a, b);
	mmEmpty();
#endif
	for (; p < q; ++p)
	    *p = val(*p);
    }
}

//! �^����ꂽ��f�l�ɑ΂���P�x�␳���ʂ�Ԃ��D
/*!
  \param pixel	��f�l
  \return	�P�x�␳����
*/
template <class T> inline T
CorrectIntensity::val(T pixel) const
{
    return T(toUChar(_offset + _gain * pixel.r),
	     toUChar(_offset + _gain * pixel.g),
	     toUChar(_offset + _gain * pixel.b));
}

template <> inline u_char
CorrectIntensity::val(u_char pixel) const
{
    return toUChar(_offset + _gain * pixel);
}
    
template <> inline short
CorrectIntensity::val(short pixel) const
{
    return toShort(_offset + _gain * pixel);
}
    
template <> inline float
CorrectIntensity::val(float pixel) const
{
    return _offset + _gain * pixel;
}

template __PORT void
CorrectIntensity::operator ()(Image<u_char>& image, u_int vs, u_int ve) const;
template __PORT void
CorrectIntensity::operator ()(Image<short>& image,  u_int vs, u_int ve) const;
template __PORT void
CorrectIntensity::operator ()(Image<float>& image,  u_int vs, u_int ve) const;
template __PORT void
CorrectIntensity::operator ()(Image<RGBA>& image,   u_int vs, u_int ve) const;
template __PORT void
CorrectIntensity::operator ()(Image<ABGR>& image,   u_int vs, u_int ve) const;
    
}
