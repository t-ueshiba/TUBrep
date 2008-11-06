/*
 *  $BJ?@.(B14-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n8"=jM-(B
 *  $B<T$K$h$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$k(B
 *  $BEy$N9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
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
 *  $Id: CorrectIntensity.cc,v 1.4 2008-11-06 23:21:11 ueshiba Exp $
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
    const mmInt	val = mmLoadU((mmInt*)p);
#  if defined(SSE2)
    mmStoreU((mmInt*)p,
	     mmPackUS(mmPack32(mmToInt32F(mmAddF(a,
						 mmMulF(b, mmToFlt0(val)))),
			       mmToInt32F(mmAddF(a,
						 mmMulF(b, mmToFlt1(val))))),
		      mmPack32(mmToInt32F(mmAddF(a,
						 mmMulF(b, mmToFlt2(val)))),
			       mmToInt32F(mmAddF(a,
						 mmMulF(b, mmToFlt3(val)))))));
#  else
    mmStoreU((mmInt*)p,
	     mmPackUS(mmToIntF(mmAddF(a, mmMulF(b, mmToFlt0(val)))),
		      mmToIntF(mmAddF(a, mmMulF(b, mmToFlt1(val))))));
#  endif
}

template <> inline void
mmCorrect(short* p, mmFlt a, mmFlt b)
{
#  if defined(SSE2)
    const mmInt	val = mmLoadU((mmInt*)p);
    mmStoreU((mmInt*)p,
	     mmPack32(mmToInt32F(mmAddF(a, mmMulF(b, mmToFltL(val)))),
		      mmToInt32F(mmAddF(a, mmMulF(b, mmToFltH(val))))));
#  else
    mmStoreU((mmInt*)p,
	     mmToIntF(mmAddF(a, mmMulF(b, mmToFlt(mmLoadU((mmInt*)p))))));
#  endif
}

template <> inline void
mmCorrect(float* p, mmFlt a, mmFlt b)
{
    mmStoreFU(p, mmAddF(a, mmMulF(b, mmLoadF(p))));
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
//! $BM?$($i$l$?2hA|$N51EY$rJd@5$9$k!%(B
/*!
  \param image		$BF~NO2hA|$rM?$(!$Jd@57k2L$b$3$l$KJV$5$l$k(B
  \param vs		$B51EY$rJd@5$9$kNN0h$N:G=i$N9T$r;XDj$9$k(Bindex
  \param ve		$B51EY$rJd@5$9$kNN0h$N:G8e$N9T$N<!$r;XDj$9$k(Bindex
*/ 
template <class T> void
CorrectIntensity::operator()(Image<T>& image, int vs, int ve) const
{
    if (ve == 0)
	ve = image.height();
    
    for (int v = vs; v < ve; ++v)
    {
	T*		p = image[v];
	T* const	q = p + image.width();
#if defined(SSE)
	const mmFlt	a = mmSetF(_offset), b = mmSetF(_gain);
	for (T* const r = q - mmNBytes/sizeof(T);
	     p <= r; p += mmNBytes/sizeof(T))
	    mmCorrect(p, a, b);
	_mm_empty();
#endif
	for (; p < q; ++p)
	    *p = val(*p);
    }
}

//! $BM?$($i$l$?2hAGCM$KBP$9$k51EYJd@57k2L$rJV$9!%(B
/*!
  \param pixel	$B2hAGCM(B
  \return	$B51EYJd@57k2L(B
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

template void
CorrectIntensity::operator ()(Image<u_char>& image, int vs, int ve) const;
template void
CorrectIntensity::operator ()(Image<short>& image,  int vs, int ve) const;
template void
CorrectIntensity::operator ()(Image<float>& image,  int vs, int ve) const;
template void
CorrectIntensity::operator ()(Image<RGBA>& image,   int vs, int ve) const;
template void
CorrectIntensity::operator ()(Image<ABGR>& image,   int vs, int ve) const;
    
}
