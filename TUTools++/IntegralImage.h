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
 *  $Id: IntegralImage.h,v 1.2 2008-09-10 05:10:40 ueshiba Exp $
 */
#ifndef	__TUIntegralImage_h
#define	__TUIntegralImage_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class IntegralImage<T>						*
************************************************************************/
//! $B@QJ,2hA|(B(integral image)$B$rI=$9%/%i%9(B
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

//! $B6u$N@QJ,2hA|$r:n$k(B
template <class T> inline
IntegralImage<T>::IntegralImage()
{
}
    
//! $BM?$($i$l$?2hA|$+$i@QJ,2hA|$r:n$k(B
/*!
  \param image		$BF~NO2hA|(B
*/
template <class T> template <class S, class B> inline
IntegralImage<T>::IntegralImage(const Image<S, B>& image)
{
    initialize(image);
}
    
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

//! $B862hA|$K@5J}7A$NFsCM==;z%F%s%W%l!<%H$rE,MQ$7$?CM$rJV$9(B
/*!
  \param u		$B%F%s%W%l!<%HCf?4$N2#:BI8(B
  \param v		$B%F%s%W%l!<%HCf?4$N=D:BI8(B
  \param cropSize	$B%F%s%W%l!<%H$O0lJU(B 2*cropSize + 1 $B$N@5J}7A(B
  \return		$B%F%s%W%l!<%H$rE,MQ$7$?CM(B
*/
template <class T> inline T
IntegralImage<T>::crossVal(int u, int v, int cropSize) const
{
    return crop(u+1,	    v+1,	cropSize, cropSize)
	 - crop(u-cropSize, v+1,	cropSize, cropSize)
	 + crop(u-cropSize, v-cropSize, cropSize, cropSize)
	 - crop(u+1,	    v-cropSize, cropSize, cropSize);
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
*  class DiagonalIntegralImage<T>					*
************************************************************************/
//! $BBP3Q@QJ,2hA|(B(diagonal integral image)$B$rI=$9%/%i%9(B
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

//! $B6u$NBP3Q@QJ,2hA|$r:n$k(B
template <class T> inline
DiagonalIntegralImage<T>::DiagonalIntegralImage()
{
}
    
//! $BM?$($i$l$?2hA|$+$iBP3Q@QJ,2hA|$r:n$k(B
/*!
  \param image		$BF~NO2hA|(B
*/
template <class T> template <class S, class B> inline
DiagonalIntegralImage<T>::DiagonalIntegralImage(const Image<S, B>& image)
{
    initialize(image);
}
    
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

//! $B862hA|$K@5J}7A$NFsCM%/%m%9%F%s%W%l!<%H$rE,MQ$7$?CM$rJV$9(B
/*!
  \param u		$B%F%s%W%l!<%HCf?4$N2#:BI8(B
  \param v		$B%F%s%W%l!<%HCf?4$N=D:BI8(B
  \param cropSize	$B%F%s%W%l!<%H$O0lJU(B 2*cropSize + 1 $B$N@5J}7A(B
  \return		$B%F%s%W%l!<%H$rE,MQ$7$?CM(B
*/
template <class T> inline T
DiagonalIntegralImage<T>::crossVal(int u, int v, int cropSize) const
{
    return crop(u+cropSize+1, v-cropSize+1, cropSize, cropSize)
	 - crop(u,	      v+2,	    cropSize, cropSize)
	 + crop(u-cropSize-1, v-cropSize+1, cropSize, cropSize)
	 - crop(u,	      v-2*cropSize, cropSize, cropSize);
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

#endif	/* !__TUIntegralImage_h */
