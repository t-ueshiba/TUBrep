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
 *  $Id: TUTools++.sa.cc,v 1.7 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/Image++.h"
#include "TU/Serial++.h"

namespace TU
{
/************************************************************************
*  Color space converter form YUVxxx					*
************************************************************************/
const ConversionFromYUV	conversionFromYUV;

/************************************************************************
*  Manipulators for Serial						*
************************************************************************/
IOManip<Serial>	nl2cr	  (&Serial::i_nl2cr, &Serial::o_nl2crnl);
#ifndef __APPLE__
IOManip<Serial>	cr2nl	  (&Serial::i_cr2nl, &Serial::o_cr2nl);
IOManip<Serial>	upperlower(&Serial::i_upper2lower, &Serial::o_lower2upper);
#endif
IOManip<Serial>	through	  (&Serial::i_through, &Serial::o_through);
}

