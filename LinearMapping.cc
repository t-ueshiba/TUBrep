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
 *  $Id: LinearMapping.cc,v 1.4 2007-11-26 07:55:48 ueshiba Exp $
 */
#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  class ProjectiveMapping						*
************************************************************************/
//! $BF~NO6u4V$H=PNO6u4V$N<!85$r;XDj$7$F<M1FJQ49%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  $B91EyJQ49$H$7$F=i4|2=$5$l$k!%(B
  \param inDim	$BF~NO6u4V$N<!85(B
  \param outDim	$B=PNO6u4V$N<!85(B
*/
ProjectiveMapping::ProjectiveMapping(u_int inDim, u_int outDim)
    :_T(outDim + 1, inDim + 1)
{
    u_int	n = std::min(inDim, outDim);
    for (int i = 0; i < n; ++i)
	_T[i][i] = 1.0;
    _T[outDim][inDim] = 1.0;
}
    
/************************************************************************
*  class AffineMapping							*
************************************************************************/
//! $B$3$N%"%U%#%sJQ49$NJB9T0\F0ItJ,$rI=8=$9$k%Y%/%H%k$rJV$9!%(B
/*! 
  \return	#outDim()$B<!85%Y%/%H%k(B
*/
Vector<double>
AffineMapping::b() const
{
    Vector<double>	bb(outDim());
    for (int j = 0; j < bb.dim(); ++j)
	bb[j] = _T[j][inDim()];

    return bb;
}
    
}

