/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$kEy$NCx(B
 *  $B:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: IIRFilter++.h,v 1.6 2008-08-08 08:03:43 ueshiba Exp $
 */
#ifndef	__TUIIRFilterPP_h
#define	__TUIIRFilterPP_h

#include "TU/Array++.h"
#include "TU/mmInstructions.h"

namespace TU
{
/************************************************************************
*  static functions							*
************************************************************************/
#if defined(SSE2)
static inline mmFlt
mmForward2(mmFlt in3210, mmFlt c0123, mmFlt& tmp)
{
    tmp = mmAddF(mmShiftRF(tmp),
		 mmMulF(c0123, _mm_shuffle_ps(tmp, in3210,
					      _MM_SHUFFLE(0,0,0,0))));
    mmFlt	out0123 = tmp;
    tmp = mmAddF(mmShiftRF(tmp),
		 mmMulF(c0123, _mm_shuffle_ps(tmp, in3210,
					      _MM_SHUFFLE(1,1,0,0))));
    out0123 = mmReplaceRMostF(mmShiftLF(out0123), tmp);
    tmp = mmAddF(mmShiftRF(tmp),
		 mmMulF(c0123, _mm_shuffle_ps(tmp, in3210,
					      _MM_SHUFFLE(2,2,0,0))));
    out0123 = mmReplaceRMostF(mmShiftLF(out0123), tmp);
    tmp = mmAddF(mmShiftRF(tmp),
		 mmMulF(c0123, _mm_shuffle_ps(tmp, in3210,
					      _MM_SHUFFLE(3,3,0,0))));
    return mmReverseF(mmReplaceRMostF(mmShiftLF(out0123), tmp));
}

static inline mmFlt
mmBackward2(mmFlt in3210, mmFlt c1032, mmFlt& tmp)
{
    tmp = mmAddF(mmShiftRF(tmp),
		 mmMulF(c1032, _mm_shuffle_ps(tmp, in3210,
					      _MM_SHUFFLE(3,3,0,0))));
    mmFlt	out3210 = tmp;
    tmp = mmAddF(mmShiftRF(tmp),
		 mmMulF(c1032, _mm_shuffle_ps(tmp, in3210,
					      _MM_SHUFFLE(2,2,0,0))));
    out3210 = mmReplaceRMostF(mmShiftLF(out3210), tmp);
    tmp = mmAddF(mmShiftRF(tmp),
		 mmMulF(c1032, _mm_shuffle_ps(tmp, in3210,
					      _MM_SHUFFLE(1,1,0,0))));
    out3210 = mmReplaceRMostF(mmShiftLF(out3210), tmp);
    tmp = mmAddF(mmShiftRF(tmp),
		 mmMulF(c1032, _mm_shuffle_ps(tmp, in3210,
					      _MM_SHUFFLE(0,0,0,0))));
    return mmReplaceRMostF(mmShiftLF(out3210), tmp);
}

template <class S> static void
mmForward2(const S*& src, float*& dst, mmFlt c0123, mmFlt& tmp);

template <> inline void
mmForward2(const u_char*& src, float*& dst, mmFlt c0123, mmFlt& tmp)
{
    mmInt	in8 = mmLoadU((const mmInt*)src);
    mmStoreFU(dst, mmForward2(mmToFlt0(in8), c0123, tmp));
    dst += 4;
    mmStoreFU(dst, mmForward2(mmToFlt1(in8), c0123, tmp));
    dst += 4;
    mmStoreFU(dst, mmForward2(mmToFlt2(in8), c0123, tmp));
    dst += 4;
    mmStoreFU(dst, mmForward2(mmToFlt3(in8), c0123, tmp));
    dst += 4;
    src += mmNBytes;
}

template <> inline void
mmForward2(const float*& src, float*& dst, mmFlt c0123, mmFlt& tmp)
{
    mmStoreFU(dst, mmForward2(mmLoadFU(src), c0123, tmp));
    dst += 4;
    src += 4;
}

template <class S> static void
mmBackward2(const S*& src, float*& dst, mmFlt c1032, mmFlt& tmp);

template <> inline void
mmBackward2(const u_char*& src, float*& dst, mmFlt c1032, mmFlt& tmp)
{
    src -= mmNBytes;
    mmInt	in8 = mmLoadU((const mmInt*)src);
    dst -= 4;
    mmStoreFU(dst, mmBackward2(mmToFlt3(in8), c1032, tmp));
    dst -= 4;
    mmStoreFU(dst, mmBackward2(mmToFlt2(in8), c1032, tmp));
    dst -= 4;
    mmStoreFU(dst, mmBackward2(mmToFlt1(in8), c1032, tmp));
    dst -= 4;
    mmStoreFU(dst, mmBackward2(mmToFlt0(in8), c1032, tmp));
}

template <> inline void
mmBackward2(const float*& src, float*& dst, mmFlt c1032, mmFlt& tmp)
{
    src -= 4;
    dst -= 4;
    mmStoreFU(dst, mmBackward2(mmLoadFU(src), c1032, tmp));
}

static inline mmFlt
mmForward4(mmFlt in3210, mmFlt c0123, mmFlt c4567, mmFlt& tmp)
{
    tmp = mmAddF(mmShiftRF(tmp), mmAddF(mmMulF(c0123, mmSet0F(in3210)),
					mmMulF(c4567, mmSet0F(tmp))));
    mmFlt	out0123 = tmp;
    tmp = mmAddF(mmShiftRF(tmp), mmAddF(mmMulF(c0123, mmSet1F(in3210)),
					mmMulF(c4567, mmSet0F(tmp))));
    out0123 = mmReplaceRMostF(mmShiftLF(out0123), tmp);
    tmp = mmAddF(mmShiftRF(tmp), mmAddF(mmMulF(c0123, mmSet2F(in3210)),
					mmMulF(c4567, mmSet0F(tmp))));
    out0123 = mmReplaceRMostF(mmShiftLF(out0123), tmp);
    tmp = mmAddF(mmShiftRF(tmp), mmAddF(mmMulF(c0123, mmSet3F(in3210)),
					mmMulF(c4567, mmSet0F(tmp))));
    return mmReverseF(mmReplaceRMostF(mmShiftLF(out0123), tmp));
}

static inline mmFlt
mmBackward4(mmFlt in3210, mmFlt c3210, mmFlt c7654, mmFlt& tmp)
{
    tmp = mmAddF(mmShiftRF(tmp), mmAddF(mmMulF(c3210, mmSet3F(in3210)),
					mmMulF(c7654, mmSet0F(tmp))));
    mmFlt	out3210 = tmp;
    tmp = mmAddF(mmShiftRF(tmp), mmAddF(mmMulF(c3210, mmSet2F(in3210)),
					mmMulF(c7654, mmSet0F(tmp))));
    out3210 = mmReplaceRMostF(mmShiftLF(out3210), tmp);
    tmp = mmAddF(mmShiftRF(tmp), mmAddF(mmMulF(c3210, mmSet1F(in3210)),
					mmMulF(c7654, mmSet0F(tmp))));
    out3210 = mmReplaceRMostF(mmShiftLF(out3210), tmp);
    tmp = mmAddF(mmShiftRF(tmp), mmAddF(mmMulF(c3210, mmSet0F(in3210)),
					mmMulF(c7654, mmSet0F(tmp))));
    return mmReplaceRMostF(mmShiftLF(out3210), tmp);
}

template <class S> static void
mmForward4(const S*& src, float*& dst, mmFlt c0123, mmFlt c4567, mmFlt& tmp);

template <> inline void
mmForward4(const u_char*& src, float*& dst,
	   mmFlt c0123, mmFlt c4567, mmFlt& tmp)
{
    mmInt	in8 = mmLoadU((const mmInt*)src);
    mmStoreFU(dst, mmForward4(mmToFlt0(in8), c0123, c4567, tmp));
    dst += 4;
    mmStoreFU(dst, mmForward4(mmToFlt1(in8), c0123, c4567, tmp));
    dst += 4;
    mmStoreFU(dst, mmForward4(mmToFlt2(in8), c0123, c4567, tmp));
    dst += 4;
    mmStoreFU(dst, mmForward4(mmToFlt3(in8), c0123, c4567, tmp));
    dst += 4;
    src += mmNBytes;
}

template <> inline void
mmForward4(const float*& src, float*& dst,
	   mmFlt c0123, mmFlt c4567, mmFlt& tmp)
{
    mmStoreFU(dst, mmForward4(mmLoadFU(src), c0123, c4567, tmp));
    dst += 4;
    src += 4;
}

template <class S> static void
mmBackward4(const S*& src, float*& dst, mmFlt c3210, mmFlt c7654, mmFlt& tmp);

template <> inline void
mmBackward4(const u_char*& src, float*& dst,
	    mmFlt c3210, mmFlt c7654, mmFlt& tmp)
{
    src -= mmNBytes;
    mmInt	in8 = mmLoadU((const mmInt*)src);
    dst -= 4;
    mmStoreFU(dst, mmBackward4(mmToFlt3(in8), c3210, c7654, tmp));
    dst -= 4;
    mmStoreFU(dst, mmBackward4(mmToFlt2(in8), c3210, c7654, tmp));
    dst -= 4;
    mmStoreFU(dst, mmBackward4(mmToFlt1(in8), c3210, c7654, tmp));
    dst -= 4;
    mmStoreFU(dst, mmBackward4(mmToFlt0(in8), c3210, c7654, tmp));
}

template <> inline void
mmBackward4(const float*& src, float*& dst,
	    mmFlt c3210, mmFlt c7654, mmFlt& tmp)
{
    src -= 4;
    dst -= 4;
    mmStoreFU(dst, mmBackward4(mmLoadFU(src), c3210, c7654, tmp));
}
#endif

/************************************************************************
*  class IIRFilter							*
************************************************************************/
//! $BJRB&(BInfinite Inpulse Response Filter$B$rI=$9%/%i%9(B
template <u_int D> class IIRFilter
{
  public:
    IIRFilter&	initialize(const float c[D+D])				;
    template <class T, class B, class B2> const IIRFilter&
		forward(const Array<T, B>& in,
			Array<float, B2>& out)			const	;
    template <class T, class B, class B2> const IIRFilter&
		backward(const Array<T, B>& in,
			 Array<float, B2>& out)			const	;
    void	limitsF(float& limit0F,
			float& limit1F, float& limit2F)		const	;
    void	limitsB(float& limit0B,
			float& limit1B, float& limit2B)		const	;
    
  private:
    float	_c[D+D];	// coefficients
};

//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param c	z$BJQ4978?t(B. z$BJQ494X?t$O!$A0?J%U%#%k%?$N>l9g$O(B
		\f[
		  H(z^{-1}) = \frac{c_{D-1} + c_{D-2}z^{-1} + c_{D-3}z^{-2} +
		  \cdots
		  + c_{0}z^{-(D-1)}}{1 - c_{2D-1}z^{-1} - c_{2D-2}z^{-2} -
		  \cdots - c_{D}z^{-D}}
		\f]
		$B8eB`%U%#%k%?$N>l9g$O(B
		\f[
		  H(z) = \frac{c_{0}z + c_{1}z^2 + \cdots + c_{D-1}z^D}
		       {1 - c_{D}z - c_{D+1}z^2 - \cdots - c_{2D-1}z^D}
		\f]
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> IIRFilter<D>&
IIRFilter<D>::initialize(const float c[D+D])
{
    for (int i = 0; i < D+D; ++i)
	_c[i] = c[i];

    return *this;
}

//! $BA0?JJ}8~$K%U%#%k%?$rE,MQ$9$k(B
/*!
  \param in	$BF~NO%G!<%?Ns(B
  \param out	$B=PNO%G!<%?Ns(B
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> template <class T, class B, class B2> const IIRFilter<D>&
IIRFilter<D>::forward(const Array<T, B>& in, Array<float, B2>& out) const
{
    out.resize(in.dim());

    const T*	src = in;
    float*	dst = out;
    for (const T* const	tail = &in[in.dim()]; src < tail; ++src)
    {
	const int	d = std::min(int(D), src - (const T*)in);
	
	if (d == 0)
	    *dst = _c[1] * src[0];
	else
	{
	    *dst = 0;
	    for (int i = -d; i++ < 0; )
		*dst += (_c[D-1+i]*src[i] + _c[D+D-1+i]*dst[i-1]);
	}
	++dst;
    }

    return *this;
}
    
//! $B8eB`J}8~$K%U%#%k%?$rE,MQ$9$k(B
/*!
  \param in	$BF~NO%G!<%?Ns(B
  \param out	$B=PNO%G!<%?Ns(B
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> template <class T, class B, class B2> const IIRFilter<D>&
IIRFilter<D>::backward(const Array<T, B>& in, Array<float, B2>& out) const
{
    out.resize(in.dim());

    const T*	src = &in[in.dim()];
    float*	dst = &out[out.dim()];
    for (const T* const	head = in; src > head; )
    {
	const int	d = std::min(int(D), &in[in.dim()] - src--);

      	*--dst = 0;
	for (int i = 0; i < d; ++i)
	    *dst += (_c[i]*src[i+1] + _c[D+i]*dst[i+1]);
    }

    return *this;
}

template <> template <class T, class B, class B2> const IIRFilter<2u>&
IIRFilter<2u>::forward(const Array<T, B>& in, Array<float, B2>& out) const
{
    if (in.dim() < 2)
	return *this;

    out.resize(in.dim());
    const T*		src = in;
    float*		dst = out;
    const T* const	tail = &in[in.dim()];
#if defined(SSE2)
    const mmFlt	c0123 = mmSetF(_c[0], _c[1], _c[2], _c[3]);
    mmFlt	tmp = mmSetF(_c[0]*src[1], _c[0]*src[0] + _c[1]*src[1],
			     _c[0]*src[0] + _c[1]*src[0], 0.0);
    src += 2;
    for (const T* const tail2 = tail - mmNBytes/sizeof(T); src <= tail2; )
	mmForward2(src, dst, c0123, tmp);
    _mm_empty();
#else
    *dst = (_c[0] + _c[1])*src[0];
    ++src;
    ++dst;
    *dst = _c[0]*src[-1] + _c[1]*src[0] + (_c[2] + _c[3])*dst[-1];
    ++src;
    ++dst;
#endif
    for (; src < tail; ++src)
    {
	*dst = _c[0]*src[-1] + _c[1]*src[0] + _c[2]*dst[-2] + _c[3]*dst[-1];
	++dst;
    }

    return *this;
}
    
template <> template <class T, class B, class B2> const IIRFilter<2u>&
IIRFilter<2u>::backward(const Array<T, B>& in, Array<float, B2>& out) const
{
    if (in.dim() < 2)
	return *this;

    out.resize(in.dim());
    float*		dst = &out[out.dim()];
    const T*		src = &in[in.dim()];
    const T* const	head = &in[0];
#if defined(SSE2)
    const mmFlt		c1032 = mmSetF(_c[1], _c[0], _c[3], _c[2]);
    mmFlt		tmp   = mmSetF(_c[1]*src[-1], (_c[0] + _c[1])*src[-1],
				       (_c[0] + _c[1])*src[-1], 0.0);
    src -= 2;
    for (const T* const head2 = head + mmNBytes/sizeof(T); src >= head2; )
	mmBackward2(src, dst, c1032, tmp);
    _mm_empty();
#else
    --src;
    --dst;
    *dst = (_c[0] + _c[1])*src[0];
    --src;
    --dst;
    *dst = (_c[0] + _c[1])*src[1] + (_c[2] + _c[3])*dst[1];
#endif
    for (const T* const head = in; --src >= head; )
    {
	--dst;
	*dst = _c[0]*src[1] + _c[1]*src[2] + _c[2]*dst[1] + _c[3]*dst[2];
    }

    return *this;
}

template <> template <class T, class B, class B2> const IIRFilter<4u>&
IIRFilter<4u>::forward(const Array<T, B>& in, Array<float, B2>& out) const
{
    if (in.dim() < 4)
	return *this;

    out.resize(in.dim());
    const T*		src = in;
    float*		dst = out;
    const T* const	tail = &in[in.dim()];
#if defined(SSE2)
    const mmFlt		c0123 = mmSetF(_c[0], _c[1], _c[2], _c[3]),
			c4567 = mmSetF(_c[4], _c[5], _c[6], _c[7]);
    mmFlt		tmp   = mmSetF(_c[0]*src[0], (_c[0] + _c[1])*src[0],
				       (_c[0] + _c[1] + _c[2])*src[0], 0.0);
    for (const T* const tail2 = tail - mmNBytes/sizeof(T); src <= tail2; )
	mmForward4(src, dst, c0123, c4567, tmp);
    _mm_empty();
#else
    *dst = (_c[0] + _c[1] + _c[2] + _c[3])*src[0];
    ++src;
    ++dst;
    *dst = (_c[0] + _c[1] + _c[2])*src[-1] + _c[3]*src[0]
					   + _c[7]*dst[-1];
    ++src;
    ++dst;
    *dst = (_c[0] + _c[1])*src[-2] + _c[2]*src[-1] + _c[3]*src[0]
				   + _c[6]*dst[-2] + _c[7]*dst[-1];
    ++src;
    ++dst;
    *dst = _c[0]*src[-3] + _c[1]*src[-2] + _c[2]*src[-1] + _c[3]*src[0]
			 + _c[5]*dst[-3] + _c[6]*dst[-2] + _c[7]*dst[-1];
    ++src;
    ++dst;
#endif
    for (; src < tail; ++src)
	*dst++ = _c[0]*src[-3] + _c[1]*src[-2] + _c[2]*src[-1] + _c[3]*src[0]
	       + _c[4]*dst[-4] + _c[5]*dst[-3] + _c[6]*dst[-2] + _c[7]*dst[-1];

    return *this;
}
    
template <> template <class T, class B, class B2> const IIRFilter<4u>&
IIRFilter<4u>::backward(const Array<T, B>& in, Array<float, B2>& out) const
{
    if (in.dim() < 4)
	return *this;

    out.resize(in.dim());
    float*		dst = &out[out.dim()];
    const T*		src = &in[in.dim()];
    const T* const	head = &in[0];
#if defined(SSE2)
    const mmFlt		c3210 = mmSetF(_c[3], _c[2], _c[1], _c[0]),
			c7654 = mmSetF(_c[7], _c[6], _c[5], _c[4]);
    mmFlt		tmp   = mmSetF(_c[3]*src[-1], (_c[3] + _c[2])*src[-1],
				       (_c[3] + _c[2] + _c[1])*src[-1], 0.0);
    for (const T* const head2 = head + mmNBytes/sizeof(T); src >= head2; )
	mmBackward4(src, dst, c3210, c7654, tmp);
    _mm_empty();
#else
    --src;
    --dst;
    *dst = (_c[0] + _c[1] + _c[2] + _c[3])*src[0];
    --src;
    --dst;
    *dst = (_c[0] + _c[1] + _c[2] + _c[3])*src[1];
	 + _c[4]*dst[1];
    --src;
    --dst;
    *dst = _c[0]*src[1] + (_c[1] + _c[2] + _c[3])*src[2]
	 + _c[4]*dst[1] + _c[5]*dst[2];
    --src;
    --dst;
    *dst = _c[0]*src[1] + _c[1]*src[2] + (_c[2] + _c[3])*src[3]
	 + _c[4]*dst[1] + _c[5]*dst[2] + _c[6]*dst[3];
#endif
    for (const T* const head = in; --src >= head; )
    {
	--dst;
	*dst = _c[0]*src[1] + _c[1]*src[2] + _c[2]*src[3] + _c[3]*src[4]
	     + _c[4]*dst[1] + _c[5]*dst[2] + _c[6]*dst[3] + _c[7]*dst[4];
    }

    return *this;
}

//! $BFCDj$NF~NO%G!<%?Ns$KBP$7$FA0?JJ}8~$K%U%#%k%?$rE,MQ$7$?>l9g$N6K8BCM$r5a$a$k(B
/*!
  \param limit0F	$B0lDjF~NO(B in(n) = 1 $B$rM?$($?$H$-$N=PNO6K8BCM$rJV$9!%(B
  \param limit1F	$B79$-0lDjF~NO(B in(n) = n $B$rM?$($?$H$-$N=PNO6K8BCM$rJV$9!%(B
  \param limit1F	2$B<!F~NO(B in(n) = n^2 $B$rM?$($?$H$-$N=PNO6K8BCM$rJV$9!%(B
*/
template <u_int D> void
IIRFilter<D>::limitsF(float& limit0F, float& limit1F, float& limit2F) const
{
    float	n0 = 0.0, d0 = 1.0, n1 = 0.0, d1 = 0.0, n2 = 0.0, d2 = 0.0;
    for (int i = 0; i < D; ++i)
    {
	n0 +=	      _c[i];
	d0 -=	      _c[D+i];
	n1 +=	    i*_c[D-1-i];
	d1 -=	(i+1)*_c[D+D-1-i];
	n2 += (i-1)*i*_c[D-1-i];
	d2 -= i*(i+1)*_c[D+D-1-i];
    }
    const float	x0 = n0/d0, x1 = (n1 - x0*d1)/d0,
		x2 = (n2 - 2.0*x1*d1 - x0*d2)/d0;
    limit0F =  x0;
    limit1F = -x1;
    limit2F =  x1 + x2;
}

//! $BFCDj$NF~NO%G!<%?Ns$KBP$7$F8eB`J}8~$K%U%#%k%?$rE,MQ$7$?>l9g$N6K8BCM$r5a$a$k(B
/*!
  \param limit0B	$B0lDjF~NO(B in(n) = 1 $B$rM?$($?$H$-$N=PNO6K8BCM$rJV$9!%(B
  \param limit1B	$B79$-0lDjF~NO(B in(n) = n $B$rM?$($?$H$-$N=PNO6K8BCM$rJV$9!%(B
  \param limit1B	2$B<!F~NO(B in(n) = n^2 $B$rM?$($?$H$-$N=PNO6K8BCM$rJV$9!%(B
*/
template <u_int D> void
IIRFilter<D>::limitsB(float& limit0B, float& limit1B, float& limit2B) const
{
    float	n0 = 0.0, d0 = 1.0, n1 = 0.0, d1 = 0.0, n2 = 0.0, d2 = 0.0;
    for (int i = 0; i < D; ++i)
    {
	n0 +=	      _c[i];
	d0 -=	      _c[D+i];
	n1 +=	(i+1)*_c[i];
	d1 -=	(i+1)*_c[D+i];
	n2 += i*(i+1)*_c[i];
	d2 -= i*(i+1)*_c[D+i];
    }
    const float	x0 = n0/d0, x1 = (n1 - x0*d1)/d0,
		x2 = (n2 - 2.0*x1*d1 - x0*d2)/d0;
    limit0B = x0;
    limit1B = x1;
    limit2B = x1 + x2;
}

/************************************************************************
*  class BilateralIIRFilter						*
************************************************************************/
//! $BN>B&(BInfinite Inpulse Response Filter$B$rI=$9%/%i%9(B
template <u_int D> class BilateralIIRFilter
{
  public:
  //! $BHyJ,$N3,?t(B
    enum Order
    {
	Zeroth,						//!< 0$B3,HyJ,(B
	First,						//!< 1$B3,HyJ,(B
	Second						//!< 2$B3,HyJ,(B
    };
    
    BilateralIIRFilter&	initialize(const float cF[D+D], const float cB[D+D]);
    BilateralIIRFilter&	initialize(const float c[D+D], Order order)	;
    template <class T, class B> const BilateralIIRFilter&
			convolve(const Array<T, B>& in)		const	;
    template <class T1, class B1, class T2, class B2>
    const BilateralIIRFilter&
			operator ()(const Array2<T1, B1>& in,
				    Array2<T2, B2>& out,
				    int is=0, int ie=0)		const	;
    u_int		dim()					const	;
    float		operator [](int i)			const	;
    void		limits(float& limit0,
			       float& limit1,
			       float& limit2)			const	;
    
  private:
    IIRFilter<D>		_iirF;
    mutable Array<float>	_bufF;
    IIRFilter<D>		_iirB;
    mutable Array<float>	_bufB;
};

//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param cF	$BA0?J(Bz$BJQ4978?t(B. z$BJQ49$O(B 
		\f[
		  H^F(z^{-1}) = \frac{c^F_{D-1} + c^F_{D-2}z^{-1}
		  + c^F_{D-3}z^{-2} + \cdots
		  + c^F_{0}z^{-(D-1)}}{1 - c^F_{2D-1}z^{-1}
		  - c^F_{2D-2}z^{-2} - \cdots - c^F_{D}z^{-D}}
		\f]
		$B$H$J$k(B. 
  \param cB	$B8eB`(Bz$BJQ4978?t(B. z$BJQ49$O(B
		\f[
		  H^B(z) = \frac{c^B_{0}z + c^B_{1}z^2 + \cdots + c^B_{D-1}z^D}
		       {1 - c^B_{D}z - c^B_{D+1}z^2 - \cdots - c^B_{2D-1}z^D}
		\f]
		$B$H$J$k(B.
*/
template <u_int D> inline BilateralIIRFilter<D>&
BilateralIIRFilter<D>::initialize(const float cF[D+D], const float cB[D+D])
{
    _iirF.initialize(cF);
    _iirB.initialize(cB);
#ifdef DEBUG
    float	limit0, limit1, limit2;
    limits(limit0, limit1, limit2);
    std::cerr << "limit0 = " << limit0 << ", limit1 = " << limit1
	      << ", limit2 = " << limit2 << std::endl;
#endif
    return *this;
}

//! $BN>B&%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param c	$BA0?JJ}8~(Bz$BJQ4978?t(B. z$BJQ494X?t$O(B
		\f[
		  H(z^{-1}) = \frac{c_{D-1} + c_{D-2}z^{-1} + c_{D-3}z^{-2} +
		  \cdots
		  + c_{0}z^{-(D-1)}}{1 - c_{2D-1}z^{-1} - c_{2D-2}z^{-2} -
		  \cdots - c_{D}z^{-D}}
		\f]
  \param order	$B%U%#%k%?$NHyJ,3,?t(B. #Zeroth$B$^$?$O(B#Second$B$J$i$PBP>N%U%#%k%?(B
		$B$H$7$F!$(B#First$B$J$i$PH?BP>N%U%#%k%?$H$7$F<+F0E*$K8eB`J}8~$N(B
		z$BJQ4978?t$r7W;;$9$k!%(B#Zeroth, #First, #Second$B$N$H$-$K!$$=$l(B
		$B$>$l(B in(n) = 1, in(n) = n, in(n) = n^2 $B$KBP$9$k=PNO$,(B
		1, 1, 2$B$K$J$k$h$&!$A4BN$N%9%1!<%k$bD4@0$5$l$k!%(B
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> BilateralIIRFilter<D>&
BilateralIIRFilter<D>::initialize(const float c[D+D], Order order)
{
  // Compute 0th, 1st and 2nd derivatives of the forward z-transform
  // functions at z = 1.
    float	n0 = 0.0, d0 = 1.0, n1 = 0.0, d1 = 0.0, n2 = 0.0, d2 = 0.0;
    for (int i = 0; i < D; ++i)
    {
	n0 +=	      c[i];
	d0 -=	      c[D+i];
	n1 +=	    i*c[D-1-i];
	d1 -=	(i+1)*c[D+D-1-i];
	n2 += (i-1)*i*c[D-1-i];
	d2 -= i*(i+1)*c[D+D-1-i];
    }
    const float	x0 = n0/d0, x1 = (n1 - x0*d1)/d0,
		x2 = (n2 - 2.0*x1*d1 - x0*d2)/d0;
    
  // Compute denominators.
    float	cF[D+D], cB[D+D];
    for (int i = 0; i < D; ++i)
	cB[D+D-1-i] = cF[D+i] = c[D+i];

  // Compute nominators.
    if (order == First)	// Antisymmetric filter
    {
	const float	k = -0.5/x1;
	cF[D-1] = cB[D-1] = 0.0;			// i(n), i(n+D)
	for (int i = 0; i < D-1; ++i)
	{
	    cF[i]     = k*c[i];				// i(n-D+1+i)
	    cB[D-2-i] = -cF[i];				// i(n+D-1-i)
	}
    }
    else		// Symmetric filter
    {
	const float	k = (order == Second ? 1.0 / (x1 + x2)
					     : 1.0 / (2.0*x0 - c[D-1]));
	cF[D-1] = k*c[D-1];				// i(n)
	cB[D-1] = cF[D-1] * c[D];			// i(n+D)
	for (int i = 0; i < D-1; ++i)
	{
	    cF[i]     = k*c[i];				// i(n-D+1+i)
	    cB[D-2-i] = cF[i] + cF[D-1] * cF[D+1+i];	// i(n+D-1-i)
	}
    }

    return initialize(cF, cB);
}
    
//! $B%U%#%k%?$K$h$k>v$_9~$_$r9T$&(B. $B=PNO$O(B operator [](int) $B$G<h$j=P$9(B
/*!
  \param in	$BF~NO%G!<%?Ns(B
  return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> template <class T, class B>
inline const BilateralIIRFilter<D>&
BilateralIIRFilter<D>::convolve(const Array<T, B>& in) const
{
    _iirF.forward(in, _bufF);
    _iirB.backward(in, _bufB);

    return *this;
}

//! 2$B<!85G[Ns$N3F9T$KBP$7$F%U%#%k%?$K$h$k>v$_9~$_$r9T$&(B.
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \param is	$B=hM}$r3+;O$9$k9T(B
  \param ie	$B=hM}$r=*N;$9$k<!$N9T!%(B0$B$rM?$($k$H:G8e$N9T$^$G=hM}$9$k!%(B
  return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> template <class T1, class B1, class T2, class B2>
const BilateralIIRFilter<D>&
BilateralIIRFilter<D>::operator ()(const Array2<T1, B1>& in,
				   Array2<T2, B2>& out, int is, int ie) const
{
    out.resize(in.ncol(), in.nrow());
    if (ie == 0)
	ie = in.nrow();

    for (int i = is; i < ie; ++i)
    {
	convolve(in[i]);
	T2*	col = &out[0];
	for (int j = 0; j < dim(); ++j)
	    (*col++)[i] = (*this)[j];
    }

    return *this;
}

//! $B>v$_9~$_$N=PNO%G!<%?Ns$N<!85$rJV$9(B
/*!
  \return	$B=PNO%G!<%?Ns$N<!85(B
*/
template <u_int D> inline u_int
BilateralIIRFilter<D>::dim() const
{
    return _bufF.dim();
}

//! $B>v$_9~$_$N=PNO%G!<%?$NFCDj$NMWAG$rJV$9(B
/*!
  \param i	$BMWAG$N(Bindex
  \return	$BMWAG$NCM(B
*/
template <u_int D> inline float
BilateralIIRFilter<D>::operator [](int i) const
{
    return _bufF[i] + _bufB[i];
}

//! $BFCDj$NF~NO%G!<%?Ns$KBP$7$F%U%#%k%?$rE,MQ$7$?>l9g$N6K8BCM$r5a$a$k(B
/*!
  \param limit0		$B0lDjF~NO(B in(n) = 1 $B$rM?$($?$H$-$N=PNO6K8BCM$rJV$9!%(B
  \param limit1		$B79$-0lDjF~NO(B in(n) = n $B$rM?$($?$H$-$N=PNO6K8BCM$rJV$9!%(B
  \param limit2		2$B<!F~NO(B in(n) = n^2 $B$rM?$($?$H$-$N=PNO6K8BCM$rJV$9!%(B
*/
template <u_int D> void
BilateralIIRFilter<D>::limits(float& limit0, float& limit1, float& limit2) const
{
    float	limit0F, limit1F, limit2F;
    _iirF.limitsF(limit0F, limit1F, limit2F);

    float	limit0B, limit1B, limit2B;
    _iirB.limitsB(limit0B, limit1B, limit2B);

    limit0 = limit0F + limit0B;
    limit1 = limit1F + limit1B;
    limit2 = limit2F + limit2B;
}

/************************************************************************
*  class BilateralIIRFilter2						*
************************************************************************/
//! 2$B<!85N>B&(BInfinite Inpulse Response Filter$B$rI=$9%/%i%9(B
template <u_int D> class BilateralIIRFilter2
{
  public:
    typedef typename BilateralIIRFilter<D>::Order	Order;
    
    BilateralIIRFilter2&
		initialize(float cHF[D+D], float cHB[D+D],
			   float cVF[D+D], float cVB[D+D])		;
    BilateralIIRFilter2&
		initialize(float cHF[D+D], Order orderH,
			   float cVF[D+D], Order orderV)		;
    template <class T1, class B1, class T2, class B2>
    const BilateralIIRFilter2&
		convolve(const Array2<T1, B1>& in,
			 Array2<T2, B2>& out)			const	;
    
  private:
    BilateralIIRFilter<D>		_iirH;
    BilateralIIRFilter<D>		_iirV;
    mutable Array2<Array<float> >	_buf;
};
    
//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param cHF	$B2#J}8~A0?J(Bz$BJQ4978?t(B
  \param cHB	$B2#J}8~8eB`(Bz$BJQ4978?t(B
  \param cHV	$B=DJ}8~A0?J(Bz$BJQ4978?t(B
  \param cHV	$B=DJ}8~8eB`(Bz$BJQ4978?t(B
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> inline BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::initialize(float cHF[D+D], float cHB[D+D],
				   float cVF[D+D], float cVB[D+D])
{
    _iirH.initialize(cHF, cHB);
    _iirV.initialize(cVF, cVB);

    return *this;
}

//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param cHF	$B2#J}8~A0?J(Bz$BJQ4978?t(B
  \param orderH $B2#J}8~HyJ,3,?t(B
  \param cHV	$B=DJ}8~A0?J(Bz$BJQ4978?t(B
  \param orderV	$B=DJ}8~HyJ,3,?t(B
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> inline BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::initialize(float cHF[D+D], Order orderH,
				   float cVF[D+D], Order orderV)
{
    _iirH.initialize(cHF, orderH);
    _iirV.initialize(cVF, orderV);

    return *this;
}

//! $BM?$($i$l$?(B2$B<!85G[Ns$H$3$N%U%#%k%?$N>v$_9~$_$r9T$&(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> template <class T1, class B1, class T2, class B2>
inline const BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::convolve(const Array2<T1, B1>& in,
				 Array2<T2, B2>& out) const
{
    _iirH(in, _buf);
    _iirV(_buf, out);

    return *this;
}

}

#endif	/* !__TUIIRFilterPP_h */
