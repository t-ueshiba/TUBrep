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
 *  $Id: CorrectIntensity.cc,v 1.9 2011-12-07 08:06:31 ueshiba Exp $
 */
#include "TU/CorrectIntensity.h"
#include "TU/mmInstructions.h"

namespace TU
{
/************************************************************************
*  static functions							*
************************************************************************/
#if defined(SSE)
namespace mm
{
template <class T> static inline void
correct(T* p, F32vec a, F32vec b)
{
    const u_int		nelms = F32vec::size;
    const Iu8vec	val = load((const u_char*)p);
#  if defined(SSE2)
    storeu((u_char*)p,
	   cvt<u_char>(
	       cvt<short>(
		   cvt<int>(a + b * cvt<float>(val)),
		   cvt<int>(a + b * cvt<float>(shift_r<nelms>(val)))),
	       cvt<short>(
		   cvt<int>(a + b * cvt<float>(shift_r<2*nelms>(val))),
		   cvt<int>(a + b * cvt<float>(shift_r<3*nelms>(val))))));
#  else
    storeu((u_char*)p,
	   cvt<u_char>(cvt<short>(a + b * cvt<float>(val)),
		       cvt<short>(a + b * cvt<float>(shift_r<nelms>(val)))));
#  endif
}

template <> inline void
correct(short* p, F32vec a, F32vec b)
{
#  if defined(SSE2)
    const u_int		nelms = F32vec::size;
    const Is16vec	val = loadu(p);
    storeu(p, cvt<short>(cvt<int>(a + b * cvt<float>(val)),
			 cvt<int>(a + b * cvt<float>(shift_r<nelms>(val)))));
#  else
    storeu(p, cvt<short>(a + b * cvt<float>(loadu(p))));
#  endif
}

template <> inline void
correct(float* p, F32vec a, F32vec b)
{
    storeu(p, a + b * loadu(p));
}
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
	using namespace	mm;

	const u_int	nelms = vec<T>::size;
	const F32vec	a(_offset), b(_gain);
	for (T* const r = q - nelms; p <= r; p += nelms)
	    correct(p, a, b);
	empty();
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
