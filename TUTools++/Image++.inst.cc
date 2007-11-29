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
 *  $Id: Image++.inst.cc,v 1.12 2007-11-29 07:06:36 ueshiba Exp $
 */
#if defined(__GNUG__) || defined(__INTEL_COMPILER)

#include "TU/Image++.cc"

namespace TU
{
template class Image<u_char>;
template class Image<short>;
template class Image<int>;
template class Image<float>;
template class Image<double>;
template class Image<RGB>;
template class Image<BGR>;
template class Image<RGBA>;
template class Image<ABGR>;
template class Image<YUV444>;
template class Image<YUV422>;
template class Image<YUV411>;

template class IntegralImage<int>;
template IntegralImage<int>&
IntegralImage<int>::initialize(const Image<u_char>& image)		;
template const IntegralImage<int>&
IntegralImage<int>::crossVal(Image<int>& out, int cropSize)	const	;
template const IntegralImage<int>&
IntegralImage<int>::crossVal(Image<float>& out, int cropSize)	const	;
    
template class IntegralImage<float>;
template IntegralImage<float>&
IntegralImage<float>::initialize(const Image<u_char>& image)		;
template IntegralImage<float>&
IntegralImage<float>::initialize(const Image<float>& image)		;
template const IntegralImage<float>&
IntegralImage<float>::crossVal(Image<float>& out, int cropSize)	const	;

template class DiagonalIntegralImage<int>;
template DiagonalIntegralImage<int>&
DiagonalIntegralImage<int>::initialize(const Image<u_char>& image)	;
template const DiagonalIntegralImage<int>&
DiagonalIntegralImage<int>::crossVal(Image<int>& out, int cropSize)	const;
template const DiagonalIntegralImage<int>&
DiagonalIntegralImage<int>::crossVal(Image<float>& out, int cropSize)	const;
    
template class DiagonalIntegralImage<float>;
template DiagonalIntegralImage<float>&
DiagonalIntegralImage<float>::initialize(const Image<u_char>& image)	;
template DiagonalIntegralImage<float>&
DiagonalIntegralImage<float>::initialize(const Image<float>& image)	;
template const DiagonalIntegralImage<float>&
DiagonalIntegralImage<float>::crossVal(Image<float>& out, int cropSize)	const;
}

#endif
