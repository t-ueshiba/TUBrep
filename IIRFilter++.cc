/*
 *  $Id: IIRFilter++.cc,v 1.1 2006-10-23 01:22:00 ueshiba Exp $
 */
#include <algorithm>
#include "TU/IIRFilter++.h"
#include "TU/mmInstructions.h"

namespace TU
{
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
//! �t�B���^��z�ϊ��W�����Z�b�g����
/*!
  \param c	z�ϊ��W��. z�ϊ��֐��́C�O�i�t�B���^�̏ꍇ��
		  H(w) = (c[D-1] + c[D-2]*w + c[D-3]*w^2 + ... + c[0]*w^(D-1))
		       / (1 - c[D+D-1]*w - c[D+D-2]*w^2... - c[D]*w^D)
		������ w = z^(-1). ��ރt�B���^�̏ꍇ��
		  H(z) = (c[0]*z + c[1]*z^2 + ... + c[D-1]*z^D)
		       / (1 - c[D]*z - c[D+1]*z^2... - c[D+D-1]*z^D)
  \return	���̃t�B���^���g.
*/
template <u_int D> IIRFilter<D>&
IIRFilter<D>::initialize(const float c[D+D])
{
    for (int i = 0; i < D+D; ++i)
	_c[i] = c[i];

    return *this;
}

//! �O�i�����Ƀt�B���^��K�p����
/*!
  \param in	���̓f�[�^��.
  \param out	�o�̓f�[�^��.
  \return	���̃t�B���^���g.
*/
template <u_int D> template <class S> const IIRFilter<D>&
IIRFilter<D>::forward(const Array<S>& in, Array<float>& out) const
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
    
//! ��ޕ����Ƀt�B���^��K�p����
/*!
  \param in	���̓f�[�^��.
  \param out	�o�̓f�[�^��.
  \return	���̃t�B���^���g.
*/
template <u_int D> template <class S> const IIRFilter<D>&
IIRFilter<D>::backward(const Array<S>& in, Array<float>& out) const
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

template <> template <class S> const IIRFilter<2u>&
IIRFilter<2u>::forward(const Array<S>& in, Array<float>& out) const
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
    
template <> template <class S> const IIRFilter<2u>&
IIRFilter<2u>::backward(const Array<S>& in, Array<float>& out) const
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

template <> template <class S> const IIRFilter<4u>&
IIRFilter<4u>::forward(const Array<S>& in, Array<float>& out) const
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
    
template <> template <class S> const IIRFilter<4u>&
IIRFilter<4u>::backward(const Array<S>& in, Array<float>& out) const
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

//! ����̓��̓f�[�^��ɑ΂��đO�i�����Ƀt�B���^��K�p�����ꍇ�̋Ɍ��l�����߂�
/*!
  \param limit0F	������ in(n) = 1 ��^�����Ƃ��̏o�͋Ɍ��l��Ԃ�.
  \param limit1F	�X�������� in(n) = n ��^�����Ƃ��̏o�͋Ɍ��l��Ԃ�.
  \param limit1F	2������ in(n) = n^2 ��^�����Ƃ��̏o�͋Ɍ��l��Ԃ�.
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

//! ����̓��̓f�[�^��ɑ΂��Č�ޕ����Ƀt�B���^��K�p�����ꍇ�̋Ɍ��l�����߂�
/*!
  \param limit0B	������ in(n) = 1 ��^�����Ƃ��̏o�͋Ɍ��l��Ԃ�.
  \param limit1B	�X�������� in(n) = n ��^�����Ƃ��̏o�͋Ɍ��l��Ԃ�.
  \param limit1B	2������ in(n) = n^2 ��^�����Ƃ��̏o�͋Ɍ��l��Ԃ�.
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
//! �����t�B���^��z�ϊ��W�����Z�b�g����
/*!
  \param c	�O�i����z�ϊ��W��. 
		  H(w) = (c[D-1] + c[D-2]*w + c[D-3]*w^2 + ... + c[0]*w^(D-1))
		       / (1 - c[D+D-1]*w - c[D+D-2]*w^2... - c[D]*w^D)
		������ w = z^(-1).
  \param order	�t�B���^�̔����K��. Zeroth�܂���Second�Ȃ�ΑΏ̃t�B���^
		�Ƃ��āCFirst�Ȃ�Δ��Ώ̃t�B���^�Ƃ��Ď����I�Ɍ�ޕ�����
		z�ϊ��W�����v�Z����DZeroth, First, Second�̂Ƃ��ɁC����
		���� in(n) = 1, in(n) = n, in(n) = n^2 �ɑ΂���o�͂�
		1, 1, 2�ɂȂ�悤�C�S�̂̃X�P�[�������������D
  \return	���̃t�B���^���g.
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
    
//! ����̓��̓f�[�^��ɑ΂��ăt�B���^��K�p�����ꍇ�̋Ɍ��l�����߂�
/*!
  \param limit0		������ in(n) = 1 ��^�����Ƃ��̏o�͋Ɍ��l��Ԃ�.
  \param limit1		�X�������� in(n) = n ��^�����Ƃ��̏o�͋Ɍ��l��Ԃ�.
  \param limit1		2������ in(n) = n^2 ��^�����Ƃ��̏o�͋Ɍ��l��Ԃ�.
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
//! �^����ꂽ�摜�Ƃ��̃t�B���^�̏�ݍ��݂��s��
/*!
  \param in	���͉摜.
  \param out	�o�͉摜.
  \return	���̃t�B���^���g.
*/
template <u_int D> template <class S, class T> BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::convolve(const Image<S>& in, Image<T>& out)
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
      	ImageLine<T>*	col = &out[0];
	for (int v = 0; v < _iirV.dim(); ++v)
	    (*col++)[u] = _iirV[v];
    }

    return *this;
}

}
