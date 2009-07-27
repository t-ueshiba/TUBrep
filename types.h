/*
 *  $Id: types.h,v 1.11 2009-07-27 07:32:05 ueshiba Exp $
 */
/*!
  \mainpage	libTUTools++ - $BG[Ns!$%Y%/%H%k!$9TNs!$2hA|Ey$N4pK\E*$J%G!<%?7?$H$=$l$KIU?o$7$?%"%k%4%j%:%`$r<}$a$?%i%$%V%i%j(B
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
  $B$KIU?o$7$?%"%k%4%j%:%`$r<}$a$?%i%$%V%i%j$G$"$k!%8=:_<BAu$5$l$F$$$k<g(B
  $BMW$J%/%i%9$*$h$S4X?t$O$*$*$^$+$K0J2<$NJ,Ln$KJ,N`$5$l$k!%(B

  <b>1$B<!85$*$h$S(B2$B<!85G[Ns(B</b>
  - #TU::Array
  - #TU::Array2

  <b>$B%Y%/%H%k$H9TNs$*$h$S@~7A7W;;(B</b>
  - #TU::Vector
  - #TU::Matrix
  - #TU::LUDecomposition
  - #TU::Householder
  - #TU::QRDecomposition
  - #TU::TriDiagonal
  - #TU::BiDiagonal
  - #TU::SVDecomposition
  - #TU::BlockMatrix

  <b>$BHs@~7A:GE,2=(B</b>
  - #TU::NullConstraint
  - #TU::ConstNormConstraint
  - #Matrix<typename F::value_type> TU::minimizeSquare(const F&, const G&, AT&, int, double)
  - #Matrix<typename F::value_type> TU::minimizeSquareSparse(const F&, const G&, ATA&, IB, IB, int, double)

  <b>RANSAC</b>
  - #typename Pointset::Container TU::ransac(const PointSet&, Model&, Conform, double)

  <b>$BE@!$D>@~!$J?LLEy$N4v2?MWAG$H$=$NJQ49(B</b>
  - #TU::Point2
  - #TU::Point3
  - #TU::HyperPlane
  - #TU::Normalize
  - #TU::ProjectiveMapping
  - #TU::AffineMapping

  <b>$BEj1F$N4v2?(B</b>
  - #TU::CanonicalCamera
  - #TU::CameraWithFocalLength
  - #TU::CameraWithEuclideanImagePlane
  - #TU::Camera
  - #TU::CameraWithDistortion
  
  <b>$B2hAG$H2hA|(B</b>
  - #TU::RGB
  - #TU::BGR
  - #TU::RGBA
  - #TU::ABGR
  - #TU::YUV444
  - #TU::YUV422
  - #TU::YUV411
  - #TU::ImageLine
  - #TU::Image
  - #TU::GenericImage
  - #TU::Movie

  <b>$B2hA|=hM}(B</b>
  - #TU::EdgeDetector
  - #TU::CorrectIntensity
  - #TU::Warp
  - #TU::IntegralImage
  - #TU::DiagonalIntegralImage
  
  <b>$B2hA|$K8B$i$J$$?.9f=hM}(B</b>
  - #TU::IIRFilter
  - #TU::BilateralIIRFilter
  - #TU::BilateralIIRFilter2
  - #TU::DericheConvolver
  - #TU::DericheConvolver2
  - #TU::GaussianConvolver
  - #TU::GaussianConvolver2
  
  <b>$BFC<l%G!<%?9=B$(B</b>
  - #TU::List
  - #TU::Heap
  - #TU::PSTree

  <b>Bezier$B6J@~$H(BBezier$B6JLL(B</b>
  - #TU::BezierCurve
  - #TU::BezierSurface
  
  <b>B-Spline$B6J@~$H(BB-Spline$B6JLL(B</b>
  - #TU::BSplineCurve
  - #TU::BSplineSurface
  
  <b>$B%a%C%7%e(B</b>
  - #TU::Mesh

  <b>$BI8=`%i%$%V%i%j$NJd6/(B</b>
  - #const T& std::min(const T&, const T&, const T&)
  - #const T& std::min(const T&, const T&, const T&, const T&)
  - #const T& std::max(const T&, const T&, const T&)
  - #const T& std::max(const T&, const T&, const T&, const T&)
  - #Iter TU::pull_if(Iter, Iter, Pred)
  - #T TU::diff(const T&, const T&)
  - #TU::mbr_iterator
  - #std::istream& TU::ign(std::istream&)
  - #std::istream& TU::skipl(std::istream&)
  - #TU::IOManip
  - #TU::IManip1
  - #TU::OManip1
  - #TU::IOManip1
  - #TU::IManip2
  - #TU::OManip2

  <b>$B%a%b%j4IM}(B</b>
  - #TU::Allocator

  <b>$B%7%j%"%k%$%s%?%U%'!<%9(B</b>
  - #TU::Serial
  - #TU::Puma
  - #TU::Pata
  - #TU::Microscope
  - #TU::TriggerGenerator
  
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
