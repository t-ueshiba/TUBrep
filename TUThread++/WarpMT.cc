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
 *  $Id: WarpMT.cc,v 1.3 2008-09-10 05:11:51 ueshiba Exp $  
 */
#include "TU/Thread++.h"
#include "TU/Warp.h"

namespace TU
{
/************************************************************************
*  class MultiThread2<Warp, Image<T> >					*
************************************************************************/
template <> void
MultiThread2<Warp, Image<u_char> >::
  operator ()(const Image<u_char>& in, Image<u_char>& out) const
{
    out.resize(height(), width());
    raiseThreads(in, out);
}
    
template <> void
MultiThread2<Warp, Image<RGBA> >::
  operator ()(const Image<RGBA>& in, Image<RGBA>& out) const
{
    out.resize(height(), width());
    raiseThreads(in, out);
}
    
template <> void
MultiThread2<Warp, Image<ABGR> >::
  operator ()(const Image<ABGR>& in, Image<ABGR>& out) const
{
    out.resize(height(), width());
    raiseThreads(in, out);
}
    
}
