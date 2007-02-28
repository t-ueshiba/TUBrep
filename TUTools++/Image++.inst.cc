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
 *  $Id: Image++.inst.cc,v 1.8 2007-02-28 00:16:06 ueshiba Exp $
 */
#if defined(__GNUG__) || defined(__INTEL_COMPILER)

#include "TU/Image++.cc"

namespace TU
{
template class Image<u_char>;
template class Image<short>;
template class Image<long>;
template class Image<float>;
template class Image<double>;
template class Image<RGB>;
template class Image<BGR>;
template class Image<RGBA>;
template class Image<ABGR>;
template class Image<YUV444>;
template class Image<YUV422>;
template class Image<YUV411>;

template class IntegralImage<long>;
template IntegralImage<long>&
IntegralImage<long>::initialize(const Image<u_char>& image)		;
template const IntegralImage<long>&
IntegralImage<long>::crossVal(Image<long>& out, int cropSize)	const	;
template const IntegralImage<long>&
IntegralImage<long>::crossVal(Image<float>& out, int cropSize)	const	;
    
template class IntegralImage<float>;
template IntegralImage<float>&
IntegralImage<float>::initialize(const Image<u_char>& image)		;
template IntegralImage<float>&
IntegralImage<float>::initialize(const Image<float>& image)		;
template const IntegralImage<float>&
IntegralImage<float>::crossVal(Image<float>& out, int cropSize)	const	;

template class DiagonalIntegralImage<long>;
template DiagonalIntegralImage<long>&
DiagonalIntegralImage<long>::initialize(const Image<u_char>& image)	;
template const DiagonalIntegralImage<long>&
DiagonalIntegralImage<long>::crossVal(Image<long>& out, int cropSize)	const;
template const DiagonalIntegralImage<long>&
DiagonalIntegralImage<long>::crossVal(Image<float>& out, int cropSize)	const;
    
template class DiagonalIntegralImage<float>;
template DiagonalIntegralImage<float>&
DiagonalIntegralImage<float>::initialize(const Image<u_char>& image)	;
template DiagonalIntegralImage<float>&
DiagonalIntegralImage<float>::initialize(const Image<float>& image)	;
template const DiagonalIntegralImage<float>&
DiagonalIntegralImage<float>::crossVal(Image<float>& out, int cropSize)	const;
}

#endif
