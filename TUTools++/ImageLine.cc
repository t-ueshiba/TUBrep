/*
 *  $BJ?@.(B19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $BF1=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n<T$K$h$k5v2D$J$7$K$3$N%W%m(B
 *  $B%0%i%`$rBh;0<T$X3+<(!$J#@=!$2~JQ!$;HMQ$9$kEy$NCx:n8"$r?/32$9$k9T0Y(B
 *  $B$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *  Copyright 2007
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Author: Toshio UESHIBA
 *
 *  Confidentail and all rights reserved.
 *  This program is confidential. Any changing, copying or giving
 *  information about the source code of any part of this software
 *  and/or documents without permission by the authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damages in the use of this program.
 *  
 *  $Id: ImageLine.cc,v 1.3 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class ImageLine<YUV422>						*
************************************************************************/
const YUV444*
ImageLine<YUV422>::fill(const YUV444* src)
{
    register YUV422* dst = *this;
    for (register int n = dim(); n > 0; n -= 2)
    {
	dst->x = src->u;
	dst->y = src->y;
	++dst;
	dst->x = src->v;
	++src;
	dst->y = src->y;
	++dst;
	++src;
    }
    return src;
}

const YUV411*
ImageLine<YUV422>::fill(const YUV411* src)
{
    register YUV422* dst = *this;
    for (register int n = dim(); n > 0; n -= 4)
    {
	dst->x = src[0].x;
	dst->y = src[0].y0;
	++dst;
	dst->x = src[1].x;
	dst->y = src[0].y1;
	++dst;
	dst->x = src[0].x;
	dst->y = src[1].y0;
	++dst;
	dst->x = src[1].x;
	dst->y = src[1].y1;
	++dst;
	src += 2;
    }
    return src;
}

/************************************************************************
*  class ImageLine<YUV411>						*
************************************************************************/
const YUV444*
ImageLine<YUV411>::fill(const YUV444* src)
{
    register YUV411* dst = *this;
    for (register int n = dim(); n > 0; n -= 2)
    {
	dst->x  = src[0].u;
	dst->y0 = src[0].y;
	dst->y1 = src[1].y;
	++dst;
	dst->x  = src[0].v;
	dst->y0 = src[2].y;
	dst->y1 = src[3].y;
	++dst;
	src += 4;
    }
    return src;
}

const YUV422*
ImageLine<YUV411>::fill(const YUV422* src)
{
    register YUV411* dst = *this;
    for (register int n = dim(); n > 0; n -= 2)
    {
	dst->x  = src[0].x;
	dst->y0 = src[0].y;
	dst->y1 = src[1].y;
	++dst;
	dst->x  = src[1].x;
	dst->y0 = src[2].y;
	dst->y1 = src[3].y;
	++dst;
	src += 4;
    }
    return src;
}
 
}
