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
 *  $Id: ConversionFromYUV.cc,v 1.4 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/Image++.h"

namespace TU
{
static inline int	flt2fix(float flt)	{return int(flt * (1 << 10));}

/************************************************************************
*  class ConversionFromYUV						*
************************************************************************/
ConversionFromYUV::ConversionFromYUV()
{
    for (int i = 0; i < 256; ++i)
    {
	_r [i] = int(1.4022f * (i - 128));
	_g0[i] = flt2fix(0.7144f * (i - 128));
	_g1[i] = flt2fix(0.3457f * (i - 128));
	_b [i] = int(1.7710f * (i - 128));
    }
}
 
}
