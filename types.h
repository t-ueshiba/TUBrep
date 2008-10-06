/*
 *  $Id: types.h,v 1.9 2008-10-06 01:41:25 ueshiba Exp $
 */
/*!
  \mainpage	libTUTools++ - $BG[Ns!$%Y%/%H%k!$9TNs!$2hA|$J$I4pK\E*$J%G!<%?7?$H$=$l$KIU?o$7$?%"%k%4%j%:%`$r<}$a$?%i%$%V%i%j(B
  \anchor	libTUTools

  \section copyright $BCx:n8"(B
  $BJ?@.(B14-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B

  $BAO:n<T!'?"<G=SIW(B

  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n8"=jM-(B
  $B<T$K$h$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$k(B
  $BEy$N9T0Y$r6X;_$7$^$9!%(B
   
  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B

  Copyright 2002-2007.
  National Institute of Advanced Industrial Science and Technology (AIST)

  Creator: Toshio UESHIBA

  [AIST Confidential and all rights reserved.]
  This program is confidential. Any using, copying, changing or
  giving any information concerning with this program to others
  without permission by the copyright holder are strictly prohibited.

  [No Warranty.]
  The copyright holder or the creator are not responsible for any
  damages caused by using this program.

  \section abstract $B35MW(B
  libTUTools++$B$O!$G[Ns!$%Y%/%H%k!$9TNs!$2hA|Ey$N4pK\E*$J%G!<%?7?$H$=$l(B
  $B$KIU?o$7$?%"%k%4%j%:%`$r<}$a$?%i%$%V%i%j$G$"$k!%(B

  \file		types.h
  \brief	$B4pK\E*$J%G!<%?7?$r%0%m!<%P%k$JL>A06u4V$KDI2C(B
*/
#ifndef __TUtypes_h
#define __TUtypes_h

#ifdef WIN32
typedef unsigned int	size_t;			//!< $BG[NsEy$N%5%$%:$rI=$97?(B
typedef unsigned char	u_char;			//!< $BId9f$J$7(B8bit$B@0?t(B
typedef unsigned short	u_short;		//!< $BId9f$J$7(B16bit$B@0?t(B
typedef unsigned int	u_int;			//!< $BId9f$J$7(B32bit$B@0?t(B
typedef unsigned long	u_long;			//!< $BId9f$J$7(B32/64bit$B@0?t(B
#else
#  include <sys/types.h>
#endif

typedef signed char		s_char;		//!< $BId9fIU$-(B8bit$B@0?t(B
typedef long long		int64;		//!< $BId9fIU$-(B64bit$B@0?t(B
typedef unsigned long long	u_int64;	//!< $BId9f$J$7(B64bit$B@0?t(B

#endif	/*  !__TUtypes_h	*/
