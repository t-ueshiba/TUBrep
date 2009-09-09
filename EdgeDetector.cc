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
 *  $Id: EdgeDetector.cc,v 1.15 2009-09-09 07:06:30 ueshiba Exp $
 */
#include "TU/EdgeDetector.h"
#include "TU/mmInstructions.h"

namespace TU
{
static const float	slant = 0.414214;	// tan(M_PI/8)
    
/************************************************************************
*  static functions							*
************************************************************************/
#if defined(SSE2)
static inline mmInt32
mmDir4(mmFlt eH, mmFlt eV)
{
    mmInt32	l0 = mmCast<mmInt32>(eH < eV),
		l1 = mmCast<mmInt32>(eH < -eV);
    return ((l0 ^ l1) & mmSetAll<mmInt32>(0x2)) |
	   (l1 & mmSetAll<mmInt32>(0x4));
}

static inline mmInt32
mmDir8(mmFlt eH, mmFlt eV)
{
    mmFlt	sH = mmSetAll<mmFlt>(slant) * eH,
		sV = mmSetAll<mmFlt>(slant) * eV;
    mmInt32	l0 = mmCast<mmInt32>(sH < eV),
		l1 = mmCast<mmInt32>(eH < sV),
		l2 = mmCast<mmInt32>(eH < -sV),
		l3 = mmCast<mmInt32>(sH < -eV);
    return (((l0 ^ l1) | (l2 ^ l3)) & mmSetAll<mmInt32>(0x1)) |
	   ((l1 ^ l3) & mmSetAll<mmInt32>(0x2)) |
	   (l3 & mmSetAll<mmInt32>(0x4));
}
#endif

//! ����G�b�W�_�Ǝw�肳�ꂽ�����̋ߖT�_���ڑ����Ă��邩���ׂ�
/*!
  \param edge	�G�b�W�摜
  \param p	�G�b�W�_
  \param dir	�ߖT�_�̕���
  \return	�ڑ����Ă����true�C�����łȂ����false
*/
static inline bool
isLink(const Image<u_char>& edge, const Point2i& p, int dir)
{
  // (1) �ߖT�_�����Ȃ��Ƃ���/��G�b�W�_�ł���C���C(2a) 4�ߖT�_�ł��邩�C
  // (2b) ���ׂ̋ߖT�_����/��G�b�W�_�łȂ��ꍇ�ɐڑ����Ă���Ɣ��肷��D
    return (edge(p.neighbor(dir)) &&
	    (!(dir & 0x1) ||
	     (!edge(p.neighbor(dir-1)) && !edge(p.neighbor(dir+1)))));
}
    
//! ����G�b�W�_���N�_�ɂ��āC�ڑ�����G�b�W�_��ǐՂ���
/*!
  \param edge	�G�b�W�摜
  \param p	�G�b�W�_
*/
static void
trace(Image<u_char>& edge, const Point2i& p)
{
    u_char&	e = edge(p);		// ���̓_p�̉�f�l
    
    if (e & EdgeDetector::TRACED)	// ���ɂ��̓_���K��ς݂Ȃ�΁C
	return;				// �����ɖ߂�D

    e |= (EdgeDetector::TRACED | EdgeDetector::EDGE);	// �K��ς݂��G�b�W�_
    for (int dir = 0; dir < 8; ++dir)	// p��8�̋ߖT�_���ꂼ��ɂ���
	if (isLink(edge, p, dir))	// p�Ɛڑ����Ă����
	    trace(edge, p.neighbor(dir));	// ����ɒǐՂ𑱂���D
}

//! ����_��łĂ�EDGE���x�����t���Ă���_�Ƃ����łȂ��_�����ׂ邩���ׂ�
/*!
  \param edge	�G�b�W�摜
  \param p	�łƂ��Ƃ���_
  \return	���ׂ�̂ł����true�C�����łȂ����false
*/
static bool
canInterpolate(const Image<u_char>& edge, const Point2i& p)
{
    int	nedges = 0, nweaks = 0;
    
    for (int dir = 0; dir < 8; ++dir)	// p��8�̋ߖT�_���ꂼ��ɂ���
    {
	u_char	e = edge(p.neighbor(dir));
	
	if (e & EdgeDetector::EDGE)
	    ++nedges;			// EDGE���x�����t���Ă���_
	else if (e & EdgeDetector::WEAK)
	    ++nweaks;			// EDGE���x�����t���Ă��Ȃ��ア�G�b�W�_
    }

  // p�̋ߖT�ɁC����EDGE���x�����t���Ă���_�����Ȃ��Ƃ�1�C�����
  // �t���Ă��Ȃ��ア�G�b�W�_��1��������΁Ctrue��Ԃ��D
    return (nedges != 0 && nweaks == 1);
}
    
/************************************************************************
*  class EdgeDetector							*
************************************************************************/
//! �G�b�W���x�����߂�
/*!
  \param edgeH	������1�K�������͉摜
  \param edgeV	�c����1�K�������͉摜
  \param out	�G�b�W���x�o�͉摜
  \return	���̃G�b�W���o�펩�g
*/
const EdgeDetector&
EdgeDetector::strength(const Image<float>& edgeH,
		       const Image<float>& edgeV, Image<float>& out) const
{
    out.resize(edgeH.height(), edgeH.width());
    for (u_int v = 0; v < out.height(); ++v)
    {
	const float		*eH = edgeH[v], *eV = edgeV[v];
	float*			dst = out[v];
	const float* const	end = dst + out.width();
#if defined(SSE)
	for (const float* const end2 = dst + 4*(out.width()/4); dst < end2; )
	{
	    const mmFlt	fH = mmLoadU(eH), fV = mmLoadU(eV);
	    
	    mmStoreU(dst, mmSqrt(fH * fH + fV * fV));
	    eH  += mmFlt::NElms;
	    eV  += mmFlt::NElms;
	    dst += mmFlt::NElms;
	}
#endif
	while (dst < end)
	{
	    *dst++ = sqrtf(*eH * *eH + *eV * *eV);
	    ++eH;
	    ++eV;
	}
    }

    return *this;
}
    
//! 4�ߖT�ɂ��G�b�W���������߂�
/*!
  \param edgeH	������1�K�������͉摜
  \param edgeV	�c����1�K�������͉摜
  \param out	�G�b�W�����o�͉摜
  \return	���̃G�b�W���o�펩�g
*/
const EdgeDetector&
EdgeDetector::direction4(const Image<float>& edgeH,
			 const Image<float>& edgeV, Image<u_char>& out) const
{
    out.resize(edgeH.height(), edgeH.width());
    for (u_int v = 0; v < out.height(); ++v)
    {
	const float		*eH = edgeH[v], *eV = edgeV[v];
	u_char*			dst = out[v];
	const u_char* const	end = dst + out.width();
#if defined(SSE2)
	for (const u_char* const end2 = dst + mmUInt8::floor(out.width());
	     dst < end2; dst += mmUInt8::NElms)
	{
	    const mmInt32	d0 = mmDir4(mmLoadU(eH), mmLoadU(eV));
	    eH  += mmFlt::NElms;
	    eV  += mmFlt::NElms;
	    const mmInt32	d1 = mmDir4(mmLoadU(eH), mmLoadU(eV));
	    eH  += mmFlt::NElms;
	    eV  += mmFlt::NElms;
	    const mmInt32	d2 = mmDir4(mmLoadU(eH), mmLoadU(eV));
	    eH  += mmFlt::NElms;
	    eV  += mmFlt::NElms;
	    const mmInt32	d3 = mmDir4(mmLoadU(eH), mmLoadU(eV));
	    eH  += mmFlt::NElms;
	    eV  += mmFlt::NElms;
	    mmStoreU(dst, mmCvt<mmUInt8>(mmCvt<mmInt16>(d0, d1),
					 mmCvt<mmInt16>(d2, d3)));
	}
#endif
	while (dst < end)
	{
	    *dst++ = (*eH <= *eV ? (*eH <= -*eV ? 4 : 2)
				 : (*eH <= -*eV ? 6 : 0));
	    ++eH;
	    ++eV;
	}
    }
    
    return *this;
}
    
//! 8�ߖT�ɂ��G�b�W���������߂�
/*!
  \param edgeH	������1�K�������͉摜
  \param edgeV	�c����1�K�������͉摜
  \param out	�G�b�W�����o�͉摜
  \return	���̃G�b�W���o�펩�g
*/
const EdgeDetector&
EdgeDetector::direction8(const Image<float>& edgeH,
			 const Image<float>& edgeV, Image<u_char>& out) const
{
    out.resize(edgeH.height(), edgeH.width());
    for (u_int v = 0; v < out.height(); ++v)
    {
	const float		*eH = edgeH[v], *eV = edgeV[v];
	u_char*			dst = out[v];
	const u_char* const	end = dst + out.width();
#if defined(SSE2)
	for (const u_char* const end2 = dst + mmUInt8::floor(out.width());
	     dst < end2; dst += mmUInt8::NElms)
	{
	    const mmInt32	d0 = mmDir8(mmLoadU(eH), mmLoadU(eV));
	    eH  += mmFlt::NElms;
	    eV  += mmFlt::NElms;
	    const mmInt32	d1 = mmDir8(mmLoadU(eH), mmLoadU(eV));
	    eH  += mmFlt::NElms;
	    eV  += mmFlt::NElms;
	    const mmInt32	d2 = mmDir8(mmLoadU(eH), mmLoadU(eV));
	    eH  += mmFlt::NElms;
	    eV  += mmFlt::NElms;
	    const mmInt32	d3 = mmDir8(mmLoadU(eH), mmLoadU(eV));
	    eH  += mmFlt::NElms;
	    eV  += mmFlt::NElms;
	    mmStoreU(dst, mmCvt<mmUInt8>(mmCvt<mmInt16>(d0, d1),
					 mmCvt<mmInt16>(d2, d3)));
	}
#endif
	while (dst < end)
	{
	    const float	sH = slant * *eH, sV = slant * *eV;
	    
	    *dst++ = (sH <= *eV ?
		      (*eH <= sV ?
		       (*eH <= -sV ?
			(sH <= -*eV ? 4 : 3) : 2) : 1) :
		      (sH <= -*eV ?
		       (*eH <= -sV ?
			(*eH <=  sV ? 5 : 6) : 7) : 0));
	    ++eH;
	    ++eV;
	}
    }
    
    return *this;
}
    
//! ��ɑ�l�}�������ɂ��א������s��
/*!
  \param strength	�G�b�W���x���͉摜
  \param direction	�G�b�W�������͉摜
  \param out		�����G�b�W�_�Ǝア�G�b�W�_�ɂ��ꂼ��#EDGE���x����
			#WEAK���x����t�����o�͉摜
  \return		���̃G�b�W���o�펩�g
*/
const EdgeDetector&
EdgeDetector::suppressNonmaxima(const Image<float>& strength,
				const Image<u_char>& direction,
				Image<u_char>& out) const
{
    out.resize(strength.height(), strength.width());

  // �o�͉摜�̊O����0�ɂ���D
    if (out.height() > 0)
	for (u_int u = 0; u < out.width(); ++u)
	    out[0][u] = out[out.height()-1][u] = 0;
    if (out.width() > 0)
	for (u_int v = 0; v < out.height(); ++v)
	    out[v][0] = out[v][out.width()-1] = 0;

  // �e�_�̃G�b�W���x�� (1) ���̓_�̃G�b�W�����ɉ��������ׂƔ�r���ċɑ��
  // �Ȃ��Ă���C���C(2a) ����臒l�ȏ�Ȃ��EDGE���x�����C(2b) �ア臒l
  // �ȏ�Ȃ��WEAK���x�������ꂼ�ꏑ�����ށD�����łȂ����0���������ށD
    for (u_int v = 0; ++v < out.height() - 1; )
    {
	const float		*prv = strength[v-1],
				*str = strength[v],
				*nxt = strength[v+1];
	const u_char		*dir = direction[v];
	const u_char* const	end  = &out[v][out.width() - 1];
	for (u_char* dst = out[v]; ++dst < end; )
	{
	    ++prv;
	    ++str;
	    ++nxt;
	    ++dir;
	    
	    if (*str >= _th_low)	// �ア臒l�ȏ�Ȃ�
		switch (*dir)		// �G�b�W����������D
		{
		  case 0:
		  case 4:
		    *dst = (*str > *(str-1) && *str > *(str+1) ?
			    (*str >= _th_high ? EDGE : WEAK) : 0);
		    break;
		  case 1:
		  case 5:
		    *dst = (*str > *(prv-1) && *str > *(nxt+1) ?
			    (*str >= _th_high ? EDGE : WEAK) : 0);
		    break;
		  case 2:
		  case 6:
		    *dst = (*str > *prv && *str > *nxt ?
			    (*str >= _th_high ? EDGE : WEAK) : 0);
		    break;
		  default:
		    *dst = (*str > *(prv+1) && *str > *(nxt-1) ?
			    (*str >= _th_high ? EDGE : WEAK) : 0);
		    break;
		}
	    else
		*dst = 0;		// �ア臒l�����Ȃ� 0
	}
    }

    return *this;
}

//! 2�������摜�̃[�������_�����o����
/*!
  \param in		����2�������摜
  \param out		�[�������_��255�C�����łȂ��_��0�Ƃ����o�͉摜
  \return		���̃G�b�W���o�펩�g
*/
const EdgeDetector&
EdgeDetector::zeroCrossing(const Image<float>& in, Image<u_char>& out) const
{
    out.resize(in.height(), in.width());

  // �o�͉摜�̉��[�ƉE�[��0�ɂ���D
    if (out.height() > 0)
	for (u_int u = 0; u < out.width(); ++u)
	    out[out.height()-1][u] = 0;
    if (out.width() > 0)
	for (u_int v = 0; v < out.height(); ++v)
	    out[v][out.width()-1] = 0;

  // ���ݓ_��������Ƃ���2x2�E�B���h�E���̉�f���ٕ����Ȃ�G�b�W�_�Ƃ���D
    for (u_int v = 0; v < out.height() - 1; ++v)
    {
	const float		*cur = in[v],
				*nxt = in[v+1];
	const u_char* const	end  = &out[v][out.width() - 1];
	for (u_char* dst = out[v]; dst < end; )
	{
	    if ((*cur >= 0.0 && *(cur+1) >= 0.0 &&
		 *nxt >= 0.0 && *(nxt+1) >= 0.0) ||
		(*cur <= 0.0 && *(cur+1) <= 0.0 &&
		 *nxt <= 0.0 && *(nxt+1) <= 0.0))
		*dst++ = 0;
	    else
		*dst++ = 255;
	    ++cur;
	    ++nxt;
	}
    }

    return *this;
}
    
//! 2�������摜�̃[�������_�����o���C�G�b�W���x�ɂ���ĕ��ނ���
/*!
  \param in		����2�������摜
  \param strength	���̓G�b�W���x�摜
  \param out		�����G�b�W�_�Ǝア�G�b�W�_�ɂ��ꂼ��#EDGE���x����
			#WEAK���x����t�����o�͉摜
  \return		���̃G�b�W���o�펩�g
*/
const EdgeDetector&
EdgeDetector::zeroCrossing(const Image<float>& in, const Image<float>& strength,
			   Image<u_char>& out) const
{
    out.resize(in.height(), in.width());

  // �o�͉摜�̊O����0�ɂ���D
    if (out.height() > 0)
	for (u_int u = 0; u < out.width(); ++u)
	    out[0][u] = out[out.height()-1][u] = 0;
    if (out.width() > 0)
	for (u_int v = 0; v < out.height(); ++v)
	    out[v][0] = out[v][out.width()-1] = 0;

  // ���ݓ_��������Ƃ���2x2�E�B���h�E���̉�f���ٕ����Ȃ�G�b�W�_�Ƃ���D
    for (u_int v = 0; ++v < out.height() - 1; )
    {
	const float		*cur = in[v],
				*nxt = in[v+1],
				*str = strength[v];
	const u_char* const	end  = &out[v][out.width() - 1];
	for (u_char* dst = out[v]; ++dst < end; )
	{
	    ++cur;
	    ++nxt;
	    ++str;

	    if ((*str < _th_low) ||
		(*cur >= 0.0 && *(cur+1) >= 0.0 &&
		 *nxt >= 0.0 && *(nxt+1) >= 0.0) ||
		(*cur <= 0.0 && *(cur+1) <= 0.0 &&
		 *nxt <= 0.0 && *(nxt+1) <= 0.0))
		*dst = 0;
	    else
		*dst = (*str >= _th_high ? EDGE : WEAK);
	}
    }

    return *this;
}

//! �����G�b�W�_���N�_�Ɏア�G�b�W��ǐՂ��邱�Ƃɂ��q�X�e���V�X臒l�������s��
/*!
  \param edge		�����G�b�W�_�Ǝア�G�b�W�_�ɂ��ꂼ��#EDGE���x����
			#WEAK���x����t�����摜�D�������I���ƍŏI�I�ȃG�b�W
			�_��255���C�����łȂ��_�ɂ�0����������ŕԂ����D
  \return		���̃G�b�W���o�펩�g
*/
const EdgeDetector&
EdgeDetector::hysteresisThresholding(Image<u_char>& edge) const
{
  // �����G�b�W�_���N�_�ɂ��āC�ڑ�����ア�G�b�W�_��ǐՂ�EDGE���x����t����D
    for (u_int v = 0; ++v < edge.height() - 1; )
	for (u_int u = 0; ++u < edge.width() - 1; )
	    if (edge[v][u] & EDGE)
		trace(edge, Point2i(u, v));

  // EDGE���x�����t���Ă��炸�C���t���Ă���_�ƕt���Ă��Ȃ��ア�G�b�W�_��
  // ���n���ɂȂ��_�ɐV����EDGE���x����t���ĒǐՂ��s���D
    for (u_int v = 0; ++v < edge.height() - 1; )
	for (u_int u = 0; ++u < edge.width() - 1; )
	{
	    Point2i	p(u, v);
	    
	    if (!(edge(p) & EDGE) && canInterpolate(edge, p))
		trace(edge, p);
	}

  // EDGE�_�ɂ�255���C�����łȂ��_�ɂ�0���������ށD
    for (u_int v = 0; v < edge.height(); )
    {
	u_char*	dst = edge[v++];
	for (const u_char* const end = dst + edge.width(); dst < end; ++dst)
	    *dst = (*dst & EDGE ? 255 : 0);
    }
 
    return *this;
}
    
}
