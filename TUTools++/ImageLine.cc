/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$;HMQ!$Bh;0<T$X3+<($9$k(B
 *  $BEy$NCx:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  Confidential and all rights reserved.
 *  This program is confidential. Any using, copying, changing, giving
 *  information about the source program of any part of this software
 *  to others without permission by the creators are prohibited.
 *
 *  No Warranty.
 *  Copyright holders or creators are not responsible for any damages
 *  in the use of this program.
 *  
 *  $Id: ImageLine.cc,v 1.4 2007-11-26 07:55:48 ueshiba Exp $
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
