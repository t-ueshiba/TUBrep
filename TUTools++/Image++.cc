/*
 *  $BJ?@.(B9$BG/(B $BEE;R5;=QAm9g8&5f=j(B $B?"<G=SIW(B $BCx:n8"=jM-(B
 *
 *  $BCx:n<T$K$h$k5v2D$J$7$K$3$N%W%m%0%i%`$NBh;0<T$X$N3+<(!"J#@=!"2~JQ!"(B
 *  $B;HMQEy$=$NB>$NCx:n?M3J8"$r?/32$9$k9T0Y$r6X;_$7$^$9!#(B
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *
 *  Copyright 1996
 *  Toshio UESHIBA, Electrotechnical Laboratory
 *
 *  All rights reserved.
 *  Any changing, copying or giving information about source programs of
 *  any part of this software and/or documentation without permission of the
 *  authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damage in use of this program.
 */
/*
 *  $Id: Image++.cc,v 1.17 2007-05-23 01:36:27 ueshiba Exp $
 */
#include "TU/utility.h"
#include "TU/Image++.h"
#include "TU/mmInstructions.h"

namespace TU
{
/************************************************************************
*  class ImageLine<T>							*
************************************************************************/
template <class T> template <class S> const S*
ImageLine<T>::fill(const S* src)
{
    T* dst = *this;
    for (int n = dim() + 1; --n; )
	*dst++ = T(*src++);
    return src;
}

template <class T> const YUV422*
ImageLine<T>::fill(const YUV422* src)
{
    register T* dst = *this;
    for (register u_int u = 0; u < dim(); u += 2)
    {
	*dst++ = fromYUV<T>(src[0].y, src[0].x, src[1].x);
	*dst++ = fromYUV<T>(src[1].y, src[0].x, src[1].x);
	src += 2;
    }
    return src;
}

template <class T> const YUV411*
ImageLine<T>::fill(const YUV411* src)
{
    register T*  dst = *this;
    for (register u_int u = 0; u < dim(); u += 4)
    {
	*dst++ = fromYUV<T>(src[0].y0, src[0].x, src[1].x);
	*dst++ = fromYUV<T>(src[0].y1, src[0].x, src[1].x);
	*dst++ = fromYUV<T>(src[1].y0, src[0].x, src[1].x);
	*dst++ = fromYUV<T>(src[1].y1, src[0].x, src[1].x);
	src += 2;
    }
    return src;
}

template <class S> const S*
ImageLine<YUV422>::fill(const S* src)
{
    YUV422* dst = *this;
    for (int n = dim() + 1; --n; )
	*dst++ = YUV422(*src++);
    return src;
}

template <class S> const S*
ImageLine<YUV411>::fill(const S* src)
{
    YUV411* dst = *this;
    for (int n = dim() + 1; --n; )
	*dst++ = YUV411(*src++);
    return src;
}

/************************************************************************
*  class Image<T, B>							*
************************************************************************/
template <class T, class B> std::istream&
Image<T, B>::restoreData(std::istream& in, Type type)
{
    switch (type)
    {
      case U_CHAR:
	return restoreRows<u_char>(in);
      case SHORT:
	return restoreRows<short>(in);
      case INT:
	return restoreRows<int>(in);
      case FLOAT:
	return restoreRows<float>(in);
      case DOUBLE:
	return restoreRows<double>(in);
      case RGB_24:
	return restoreRows<RGB>(in);
      case YUV_444:
	return restoreRows<YUV444>(in);
      case YUV_422:
	return restoreRows<YUV422>(in);
      case YUV_411:
	return restoreRows<YUV411>(in);
    }
    return in;
}

template <class T, class B> std::ostream&
Image<T, B>::saveData(std::ostream& out, Type type) const
{
    switch (type)
    {
      case U_CHAR:
	return saveRows<u_char>(out);
      case SHORT:
	return saveRows<short>(out);
      case INT:
	return saveRows<int>(out);
      case FLOAT:
	return saveRows<float>(out);
      case DOUBLE:
	return saveRows<double>(out);
      case RGB_24:
	return saveRows<RGB>(out);
      case YUV_444:
	return saveRows<YUV444>(out);
      case YUV_422:
	return saveRows<YUV422>(out);
      case YUV_411:
	return saveRows<YUV411>(out);
    }
    return out;
}

template <class T, class B> template <class S> std::istream&
Image<T, B>::restoreRows(std::istream& in)
{
    ImageLine<S>	buf(width());
    for (int v = 0; v < height(); )
    {
	if (!buf.restore(in))
	    break;
	(*this)[v++].fill((S*)buf);
    }
    return in;
}

template <class T, class B> template <class D> std::ostream&
Image<T, B>::saveRows(std::ostream& out) const
{
    ImageLine<D>	buf(width());
    for (int v = 0; v < height(); )
    {
	buf.fill((const T*)(*this)[v++]);
	if (!buf.save(out))
	    break;
    }
    return out;
}

template <class T, class B> u_int
Image<T, B>::_width() const
{
    return Image<T, B>::width();	// Don't call ImageBase::width!
}

template <class T, class B> u_int
Image<T, B>::_height() const
{
    return Image<T, B>::height();	// Don't call ImageBase::height!
}

template <class T, class B> void
Image<T, B>::_resize(u_int h, u_int w, Type)
{
    Image<T, B>::resize(h, w);		// Don't call ImageBase::resize!
}

/************************************************************************
*  static functions							*
************************************************************************/
#ifdef SSE3
static inline mmFlt	mmIIR2(const float* src, const float* dst,
			       const float* c)
			{
			    return mmInpro4F(mmUnpackLF2(mmLoadFU(src),
							 mmLoadFU(dst)),
					     mmLoadFU(c));
			}
static inline mmFlt	mmIIR2(const u_char* src, const float* dst,
			       const float* c)
			{
			    return mmInpro4F(
				       mmUnpackLF2(
					   mmToFlt32(
					       _mm_set_epi8(0, 0, 0, 0,
							    0, 0, 0, 0,
							    0, 0, 0, src[1],
							    0, 0, 0, src[0])),
					   mmLoadFU(dst)),
				       mmLoadFU(c));
			}
static inline mmFlt	mmIIR4(const float* src, const float* dst,
			       const float* c)
			{
			    return mmAddF(mmInpro4F(mmLoadFU(src),
						    mmLoadFU(c)), 
					  mmInpro4F(mmLoadFU(dst),
						    mmLoadFU(c + 4)));
			}
static inline mmFlt	mmIIR4(const u_char* src, const float* dst,
			       const float* c)
			{
			    return mmAddF(mmInpro4F(mmToFlt32(
							_mm_set_epi8(
							    0, 0, 0, src[3],
							    0, 0, 0, src[2],
							    0, 0, 0, src[1],
							    0, 0, 0, src[0])),
						    mmLoadFU(c)), 
					  mmInpro4F(mmLoadFU(dst),
						    mmLoadFU(c + 4)));
			}
#endif

/************************************************************************
*  class IIRFilter							*
************************************************************************/
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
template <u_int D> template <class S, class B, class B2> const IIRFilter<D>&
IIRFilter<D>::forward(const Array<S, B>& in, Array<float, B2>& out) const
{
    out.resize(in.dim());

    const S*	src = in;
    float*	dst = out;
    for (const S* const	tail = &in[in.dim()]; src < tail; ++src)
    {
	const int	d = min(D, src - (S*)in);
	
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
template <u_int D> template <class S, class B, class B2> const IIRFilter<D>&
IIRFilter<D>::backward(const Array<S, B>& in, Array<float, B2>& out) const
{
    out.resize(in.dim());

    const S*	src = &in[in.dim()];
    float*	dst = &out[out.dim()];
    for (const S* const	head = in; src > head; )
    {
	const int	d = min(D, &in[in.dim()] - src--);

      	*--dst = 0;
	for (int i = 0; i < d; ++i)
	    *dst += (_c[i]*src[i+1] + _c[D+i]*dst[i+1]);
    }

    return *this;
}

template <> template <class S, class B, class B2> const IIRFilter<2u>&
IIRFilter<2u>::forward(const Array<S, B>& in, Array<float, B2>& out) const
{
    if (in.dim() < 2)
	return *this;

    out.resize(in.dim());
    const S*	src = in;
    float*	dst = out;
    *dst = (_c[0] + _c[1])*src[0];
    ++src;
    ++dst;
    *dst = _c[0]*src[-1] + _c[1]*src[0] + (_c[2] + _c[3])*dst[-1];
    ++src;
    ++dst;

    const S* const	tail = &in[in.dim()];
#if defined(SSE3)
    for (const S* const	tail2 = tail - 2; src < tail2; ++src)
    {
	mmStoreRMostF(dst, mmIIR2(src - 1, dst - 2, _c));
	++dst;
    }
#endif
    for (; src < tail; ++src)
    {
	*dst = _c[0]*src[-1] + _c[1]*src[0] + _c[2]*dst[-2] + _c[3]*dst[-1];
	++dst;
    }

    return *this;
}
    
template <> template <class S, class B, class B2> const IIRFilter<2u>&
IIRFilter<2u>::backward(const Array<S, B>& in, Array<float, B2>& out) const
{
    if (in.dim() < 2)
	return *this;

    out.resize(in.dim());
    float*	dst = &out[out.dim()];
    const S*	src = &in[in.dim()];
    --src;
    --dst;
    *dst = (_c[0] + _c[1])*src[0];
    --src;
    --dst;
    *dst = (_c[0] + _c[1])*src[1] + (_c[2] + _c[3])*dst[1];
#ifdef SSE3
    --src;
    --dst;
    *dst = _c[0]*src[1] + _c[1]*src[2] + _c[2]*dst[1] + _c[3]*dst[2];
    --src;
    --dst;
    *dst = _c[0]*src[1] + _c[1]*src[2] + _c[2]*dst[1] + _c[3]*dst[2];
#endif
    for (const S* const head = in; --src >= head; )
    {
	--dst;
#if defined(SSE3)
	mmStoreRMostF(dst, mmIIR2(src + 1, dst + 1, _c));
#else
	*dst = _c[0]*src[1] + _c[1]*src[2] + _c[2]*dst[1] + _c[3]*dst[2];
#endif
    }

    return *this;
}

template <> template <class S, class B, class B2> const IIRFilter<4u>&
IIRFilter<4u>::forward(const Array<S, B>& in, Array<float, B2>& out) const
{
    if (in.dim() < 4)
	return *this;

    out.resize(in.dim());
    const S*	src = in;
    float*	dst = out;
    *dst = (_c[0] + _c[1] + _c[2] + _c[3])*src[0];
    ++src;
    ++dst;
    *dst = (_c[0] + _c[1] + _c[2])*src[-1] + _c[3]*src[0]
	 + (_c[4] + _c[5] + _c[6] + _c[7])*dst[-1];
    ++src;
    ++dst;
    *dst = (_c[0] + _c[1])*src[-2] + _c[2]*src[-1] + _c[3]*src[0]
	 + (_c[4] + _c[5] + _c[6])*dst[-2]	   + _c[7]*dst[-1];
    ++src;
    ++dst;
    *dst = _c[0]*src[-3] + _c[1]*src[-2] + _c[2]*src[-1] + _c[3]*src[0]
	 + (_c[4] + _c[5])*dst[-3]	 + _c[6]*dst[-2] + _c[7]*dst[-1];
    ++src;
    ++dst;

    for (const S* const	tail = &in[in.dim()]; src < tail; ++src)
    {
#if defined(SSE3)
	mmStoreRMostF(dst, mmIIR4(src - 3, dst - 4, _c));
#else
	*dst = _c[0]*src[-3] + _c[1]*src[-2] + _c[2]*src[-1] + _c[3]*src[0]
	     + _c[4]*dst[-4] + _c[5]*dst[-3] + _c[6]*dst[-2] + _c[7]*dst[-1];
#endif
	++dst;
    }

    return *this;
}
    
template <> template <class S, class B, class B2> const IIRFilter<4u>&
IIRFilter<4u>::backward(const Array<S, B>& in, Array<float, B2>& out) const
{
    if (in.dim() < 4)
	return *this;

    out.resize(in.dim());
    float*	dst = &out[out.dim()];
    const S*	src = &in[in.dim()];
    --src;
    --dst;
    *dst = (_c[0] + _c[1] + _c[2] + _c[3])*src[0];
    --src;
    --dst;
    *dst = (_c[0] + _c[1] + _c[2] + _c[3])*src[1];
	 + (_c[4] + _c[5] + _c[6] + _c[7])*dst[1];
    --src;
    --dst;
    *dst = _c[0]*src[1] + (_c[1] + _c[2] + _c[3])*src[2]
	 + _c[4]*dst[1] + (_c[5] + _c[6] + _c[7])*dst[2];
    --src;
    --dst;
    *dst = _c[0]*src[1] + _c[1]*src[2] + (_c[2] + _c[3])*src[3]
	 + _c[4]*dst[1] + _c[5]*dst[2] + (_c[6] + _c[7])*dst[3];
    for (const S* const head = in; --src >= head; )
    {
	--dst;
#if defined(SSE3)
	mmStoreRMostF(dst, mmIIR4(src + 1, dst + 1, _c));
#else
	*dst = _c[0]*src[1] + _c[1]*src[2] + _c[2]*src[3] + _c[3]*src[4]
	     + _c[4]*dst[1] + _c[5]*dst[2] + _c[6]*dst[3] + _c[7]*dst[4];
#endif
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
//! $BM?$($i$l$?2hA|$H$3$N%U%#%k%?$N>v$_9~$_$r9T$&(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> template <class T1, class B1, class T2, class B2>
BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::convolve(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    _buf.resize(in.width(), in.height());
    for (int v = 0; v < in.height(); ++v)
    {
	_iirH.convolve(in[v]);
      	Array<float>*	col = &_buf[0];
	for (int u = 0; u < _iirH.dim(); ++u)
	    (*col++)[v] = _iirH[u];
    }

    out.resize(in.height(), in.width());
    for (int u = 0; u < _buf.nrow(); ++u)
    {
	_iirV.convolve(_buf[u]);
      	ImageLine<T2>*	col = &out[0];
	for (int v = 0; v < _iirV.dim(); ++v)
	    (*col++)[u] = _iirV[v];
    }

    return *this;
}

/************************************************************************
*  class IntegralImage							*
************************************************************************/
//! $BM?$($i$l$?2hA|$+$i@QJ,2hA|$r:n$k(B
/*!
  \param image		$BF~NO2hA|(B
  \return		$B$3$N@QJ,2hA|(B
*/
template <class T> template <class S, class B> IntegralImage<T>&
IntegralImage<T>::initialize(const Image<S, B>& image)
{
    resize(image.height(), image.width());
    
    for (int v = 0; v < height(); ++v)
    {
	const S*	src = image[v];
	T*		dst = (*this)[v];
	T		val = 0;

	if (v == 0)
	    for (const T* const end = dst + width(); dst < end; )
		*dst++ = (val += *src++);
	else
	{
	    const T*	prv = (*this)[v-1];
	    for (const T* const end = dst + width(); dst < end; )
		*dst++ = (val += *src++) + *prv++;
	}
    }

    return *this;
}

//! $B862hA|$K@_Dj$7$?D9J}7A%&%#%s%I%&Fb$N2hAGCM$NAmOB$rJV$9(B
/*!
  \param u		$B%&%#%s%I%&$N:8>e6y$N2#:BI8(B
  \param v		$B%&%#%s%I%&$N:8>e6y$N=D:BI8(B
  \param w		$B%&%#%s%I%&$NI}(B
  \param h		$B%&%#%s%I%&$N9b$5(B
  \return		$B%&%#%s%I%&Fb$N2hAGCM$NAmOB(B
*/
template <class T> T
IntegralImage<T>::crop(int u, int v, int w, int h) const
{
    --u;
    --v;
    const int	u1 = std::min(u+w, int(width())-1),
		v1 = std::min(v+h, int(height())-1);
    if (u >= int(width()) || v >= int(height()) || u1 < 0 || v1 < 0)
	return 0;
    
    T	a = 0, b = 0, c = 0;
    if (u >= 0)
    {
	c = (*this)[v1][u];
	if (v >= 0)
	{
	    a = (*this)[v][u];
	    b = (*this)[v][u1];
	}
    }
    else if (v >= 0)
	b = (*this)[v][u1];
    
    return (*this)[v1][u1] + a - b - c;
}

//! $B862hA|$NA4$F$NE@$K@5J}7A$NFsCM==;z%F%s%W%l!<%H$rE,MQ$7$?2hA|$r5a$a$k(B
/*!
  \param out		$B862hA|$K%F%s%W%l!<%H$rE,MQ$7$?=PNO2hA|(B
  \param cropSize	$B%F%s%W%l!<%H%5%$%:$r;XDj$9$k%Q%i%a!<%?(B
			$B%F%s%W%l!<%H$O0lJU(B 2*cropSize+1 $B$N@5J}7A(B
  \return		$B$3$N@QJ,2hA|(B
*/
template <class T> template <class S, class B> const IntegralImage<T>&
IntegralImage<T>::crossVal(Image<S, B>& out, int cropSize) const
{
    out.resize(height(), width());
    for (int v = 0; v < out.height(); ++v)
	for (int u = 0; u < out.width(); ++u)
	    out[v][u] = crossVal(u, v, cropSize);

    return *this;
}

/************************************************************************
*  class DiagonalIntegralImage						*
************************************************************************/
//! $BM?$($i$l$?2hA|$+$iBP3Q@QJ,2hA|$r:n$k(B
/*!
  \param image		$BF~NO2hA|(B
  \return		$B$3$NBP3Q@QJ,2hA|(B
*/
template <class T> template <class S, class B> DiagonalIntegralImage<T>&
DiagonalIntegralImage<T>::initialize(const Image<S, B>& image)
{
    resize(image.height(), image.width());
    
    Array<T>	K(width() + height() - 1), L(width() + height() - 1);
    for (int i = 0; i < K.dim(); ++i)
	K[i] = L[i] = 0;
    
    for (int v = 0; v < height(); ++v)
    {
	const S*	src = image[v];
	T		*dst = (*this)[v],
			*kp = &K[height() - 1 - v], *lp = &L[v];
	if (v == 0)
	    for (const T* const end = dst + width(); dst < end; )
		*dst++ = *kp++ = *lp++ = *src++;
	else
	{
	    const T*	prv = (*this)[v-1];
	    for (const T* const end = dst + width(); dst < end; )
	    {
		*dst++ = *src + *kp + *lp + *prv++;
		*kp++ += *src;
		*lp++ += *src++;
	    }
	}
    }

    return *this;
}

//! $B862hA|$K(B45$BEY79$1$F@_Dj$7$?D9J}7A%&%#%s%I%&Fb$N2hAGCM$NAmOB$rJV$9(B
/*!
  \param u		$B%&%#%s%I%&$N>e6y$N2#:BI8(B
  \param v		$B%&%#%s%I%&$N>e6y$N=D:BI8(B
  \param w		$B%&%#%s%I%&$NI}(B
  \param h		$B%&%#%s%I%&$N9b$5(B
  \return		$B%&%#%s%I%&Fb$N2hAGCM$NAmOB(B
*/
template <class T> T
DiagonalIntegralImage<T>::crop(int u, int v, int w, int h) const
{
    --v;
    int		ul = u - h, vl = v + h, ur = u + w, vr = v + w,
		ut = u + w - h, vt = v + w + h;
    correct(u,  v);
    correct(ul, vl);
    correct(ur, vr);
    correct(ut, vt);
    if (vt >= height())
	return 0;
    return (v  >= 0 ? (*this)[v][u]   : 0)
	 + (vt >= 0 ? (*this)[vt][ut] : 0)
	 - (vl >= 0 ? (*this)[vl][ul] : 0)
	 - (vr >= 0 ? (*this)[vr][ur] : 0);
}

//! $B862hA|$NA4$F$NE@$K@5J}7A$NFsCM%/%m%9%F%s%W%l!<%H$rE,MQ$7$?2hA|$r5a$a$k(B
/*!
  \param out		$B862hA|$K%F%s%W%l!<%H$rE,MQ$7$?=PNO2hA|(B
  \param cropSize	$B%F%s%W%l!<%H%5%$%:$r;XDj$9$k%Q%i%a!<%?(B
			$B%F%s%W%l!<%H$O0lJU(B 2*cropSize+1 $B$N@5J}7A(B
  \return		$B$3$NBP3Q@QJ,2hA|(B
*/
template <class T> template <class S, class B> const DiagonalIntegralImage<T>&
DiagonalIntegralImage<T>::crossVal(Image<S, B>& out, int cropSize) const
{
    out.resize(height(), width());
    for (int v = 0; v < out.height() - 2*cropSize - 1; ++v)
	for (int u = 0; u < out.width(); ++u)
	    out[v][u] = crossVal(u, v, cropSize);

    return *this;
}

}
