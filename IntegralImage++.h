/*
 *  ʿ��9-19ǯ���ȡ˻��ȵ�����縦��� �����ͭ
 *  
 *  �Ϻ�ԡ����ǽ���
 *
 *  �ܥץ����ϡ��ȡ˻��ȵ�����縦���ο����Ǥ��뿢�ǽ��פ��Ϻ��
 *  ���ȡ˻��ȵ�����縦��꤬������ͭ������̩����Ǥ����Ϻ�Ԥˤ�
 *  ����Ĥʤ����ܥץ�������ѡ�ʣ�������ѡ��軰�Ԥس�������������
 *  ��򿯳�����԰٤�ػߤ��ޤ���
 *  
 *  ���Υץ����ˤ�ä������뤤���ʤ�»�����Ф��Ƥ⡤�����ͭ�Ԥ�
 *  ����Ϻ�Ԥ���Ǥ���餤�ޤ���
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
 *  $Id: IntegralImage++.h,v 1.1 2008-08-11 07:09:36 ueshiba Exp $
 */
#ifndef	__TUIntegralImagePP_h
#define	__TUIntegralImagePP_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class IntegralImage<T>						*
************************************************************************/
//! ��ʬ����(integral image)��ɽ�����饹
template <class T>
class IntegralImage : public Image<T>
{
  public:
    IntegralImage()							;
    template <class S, class B>
    IntegralImage(const Image<S, B>& image)				;

    template <class S, class B> IntegralImage&
		initialize(const Image<S, B>& image)			;
    T		crop(int u, int v, int w, int h)		const	;
    T		crossVal(int u, int v, int cropSize)		const	;
    template <class S, class B> const IntegralImage&
		crossVal(Image<S, B>& out, int cropSize)	const	;

    using	Image<T>::width;
    using	Image<T>::height;
};

//! ������ʬ��������
template <class T> inline
IntegralImage<T>::IntegralImage()
{
}
    
//! Ϳ����줿����������ʬ��������
/*!
  \param image		���ϲ���
*/
template <class T> template <class S, class B> inline
IntegralImage<T>::IntegralImage(const Image<S, B>& image)
{
    initialize(image);
}
    
//! Ϳ����줿����������ʬ��������
/*!
  \param image		���ϲ���
  \return		������ʬ����
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

//! �����������ꤷ��Ĺ����������ɥ���β����ͤ����¤��֤�
/*!
  \param u		������ɥ��κ�����β���ɸ
  \param v		������ɥ��κ�����νĺ�ɸ
  \param w		������ɥ�����
  \param h		������ɥ��ι⤵
  \return		������ɥ���β����ͤ�����
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

//! �������������������ͽ����ƥ�ץ졼�Ȥ�Ŭ�Ѥ����ͤ��֤�
/*!
  \param u		�ƥ�ץ졼���濴�β���ɸ
  \param v		�ƥ�ץ졼���濴�νĺ�ɸ
  \param cropSize	�ƥ�ץ졼�Ȥϰ��� 2*cropSize + 1 ��������
  \return		�ƥ�ץ졼�Ȥ�Ŭ�Ѥ�����
*/
template <class T> inline T
IntegralImage<T>::crossVal(int u, int v, int cropSize) const
{
    return crop(u+1,	    v+1,	cropSize, cropSize)
	 - crop(u-cropSize, v+1,	cropSize, cropSize)
	 + crop(u-cropSize, v-cropSize, cropSize, cropSize)
	 - crop(u+1,	    v-cropSize, cropSize, cropSize);
}
    
//! �����������Ƥ����������������ͽ����ƥ�ץ졼�Ȥ�Ŭ�Ѥ������������
/*!
  \param out		�������˥ƥ�ץ졼�Ȥ�Ŭ�Ѥ������ϲ���
  \param cropSize	�ƥ�ץ졼�ȥ���������ꤹ��ѥ�᡼��
			�ƥ�ץ졼�Ȥϰ��� 2*cropSize+1 ��������
  \return		������ʬ����
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
*  class DiagonalIntegralImage<T>					*
************************************************************************/
//! �г���ʬ����(diagonal integral image)��ɽ�����饹
template <class T>
class DiagonalIntegralImage : public Image<T>
{
  public:
    DiagonalIntegralImage()						;
    template <class S, class B>
    DiagonalIntegralImage(const Image<S, B>& image)			;

    template <class S, class B> DiagonalIntegralImage&
		initialize(const Image<S, B>& image)			;
    T		crop(int u, int v, int w, int h)		const	;
    T		crossVal(int u, int v, int cropSize)		const	;
    template <class S, class B> const DiagonalIntegralImage&
		crossVal(Image<S, B>& out, int cropSize)	const	;

    using	Image<T>::width;
    using	Image<T>::height;

  private:
    void	correct(int& u, int& v)				const	;
};

//! �����г���ʬ��������
template <class T> inline
DiagonalIntegralImage<T>::DiagonalIntegralImage()
{
}
    
//! Ϳ����줿���������г���ʬ��������
/*!
  \param image		���ϲ���
*/
template <class T> template <class S, class B> inline
DiagonalIntegralImage<T>::DiagonalIntegralImage(const Image<S, B>& image)
{
    initialize(image);
}
    
//! Ϳ����줿���������г���ʬ��������
/*!
  \param image		���ϲ���
  \return		�����г���ʬ����
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

//! ��������45�ٷ��������ꤷ��Ĺ����������ɥ���β����ͤ����¤��֤�
/*!
  \param u		������ɥ��ξ���β���ɸ
  \param v		������ɥ��ξ���νĺ�ɸ
  \param w		������ɥ�����
  \param h		������ɥ��ι⤵
  \return		������ɥ���β����ͤ�����
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

//! �������������������ͥ����ƥ�ץ졼�Ȥ�Ŭ�Ѥ����ͤ��֤�
/*!
  \param u		�ƥ�ץ졼���濴�β���ɸ
  \param v		�ƥ�ץ졼���濴�νĺ�ɸ
  \param cropSize	�ƥ�ץ졼�Ȥϰ��� 2*cropSize + 1 ��������
  \return		�ƥ�ץ졼�Ȥ�Ŭ�Ѥ�����
*/
template <class T> inline T
DiagonalIntegralImage<T>::crossVal(int u, int v, int cropSize) const
{
    return crop(u+cropSize+1, v-cropSize+1, cropSize, cropSize)
	 - crop(u,	      v+2,	    cropSize, cropSize)
	 + crop(u-cropSize-1, v-cropSize+1, cropSize, cropSize)
	 - crop(u,	      v-2*cropSize, cropSize, cropSize);
}

//! �����������Ƥ����������������ͥ����ƥ�ץ졼�Ȥ�Ŭ�Ѥ������������
/*!
  \param out		�������˥ƥ�ץ졼�Ȥ�Ŭ�Ѥ������ϲ���
  \param cropSize	�ƥ�ץ졼�ȥ���������ꤹ��ѥ�᡼��
			�ƥ�ץ졼�Ȥϰ��� 2*cropSize+1 ��������
  \return		�����г���ʬ����
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

template <class T> inline void
DiagonalIntegralImage<T>::correct(int& u, int& v) const
{
    if (u < 0)
    {
	v += u;
	u  = 0;
    }
    else if (u >= width())
    {
	v += (int(width()) - 1 - u);
	u  = width() - 1;
    }
}

}

#endif	/* !__TUIntegralImagePP_h */
