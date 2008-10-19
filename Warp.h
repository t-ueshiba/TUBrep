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
 *  $Id: Warp.h,v 1.5 2008-10-19 23:29:34 ueshiba Exp $
 */
#ifndef	__TUWarp_h
#define	__TUWarp_h

#include "TU/Image++.h"
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class Warp								*
************************************************************************/
//! $B2hA|$rJQ7A$9$k$?$a$N%/%i%9(B
class Warp
{
  private:
    struct FracArray
    {
	FracArray(u_int d=0)
	    :us(d), vs(d), du(d), dv(d), lmost(0)	{}

	u_int		width()			const	{return us.dim();}
	void		resize(u_int d)			;

	Array<short>				us, vs;
#ifdef __INTEL_COMPILER	
	Array<u_char, AlignedBuf<u_char> >	du, dv;
#else
	Array<u_char>				du, dv;
#endif
	int					lmost;
    };
    
  public:
  //! $B2hA|JQ7A%*%V%8%'%/%H$r@8@.$9$k!%(B
    Warp()	:_fracs(), _width(0)			{}

  //! $B=PNO2hA|$NI}$rJV$9!%(B
  /*!
    return	$B=PNO2hA|$NI}(B
  */
    u_int	width()				const	{return _width;}

  //! $B=PNO2hA|$N9b$5$rJV$9!%(B
  /*!
    return	$B=PNO2hA|$N9b$5(B
  */
    u_int	height()			const	{return _fracs.dim();}
    
    int		lmost(int v)			const	;
    int		rmost(int v)			const	;

    void	initialize(const Matrix33d& Htinv,
			   u_int inWidth,  u_int inHeight,
			   u_int outWidth, u_int outHeight)		;
    void	initialize(const Matrix33d& Htinv,
			   const CameraBase::Intrinsic& intrinsic,
			   u_int inWidth,  u_int inHeight,
			   u_int outWidth, u_int outHeight)		;
    template <class T>
    void	operator ()(const Image<T>& in, Image<T>& out,
			    int vs=0, int ve=0)			const	;
    Vector2f	operator ()(int u, int v)			const	;

  private:
    Array<FracArray>	_fracs;
    u_int		_width;
};

inline void
Warp::FracArray::resize(u_int d)
{
    us.resize(d);
    vs.resize(d);
    du.resize(d);
    dv.resize(d);
}

//! $B=PNO2hA|$K$*$1$k;XDj$5$l$?9T$NM-8z:8C<0LCV$rJV$9!%(B
/*!
  $BF~NO2hA|$,6k7A$G$b=PNO2hA|$b6k7A$H$O8B$i$J$$$N$G!$=PNO2hA|$N0lIt$7$+(B
  $BF~NO2hA|$NCM0h(B($BM-8zNN0h(B)$B$H$J$i$J$$!%K\4X?t$O!$=PNO2hA|$N;XDj$5$l$?9T(B
  $B$K$D$$$F!$$=$NM-8zNN0h$N:8C<$H$J$k2hAG0LCV$rJV$9!%(B
  \param v	$B9T$r;XDj$9$k(Bintex
  \return	$B:8C<0LCV(B
*/
inline int
Warp::lmost(int v) const
{
    return _fracs[v].lmost;
}

//! $B=PNO2hA|$K$*$1$k;XDj$5$l$?9T$NM-8z1&C<0LCV$N<!$rJV$9!%(B
/*!
  $BF~NO2hA|$,6k7A$G$b=PNO2hA|$b6k7A$H$O8B$i$J$$$N$G!$=PNO2hA|$N0lIt$7$+(B
  $BF~NO2hA|$NCM0h(B($BM-8zNN0h(B)$B$H$J$i$J$$!%K\4X?t$O!$=PNO2hA|$N;XDj$5$l$?9T(B
  $B$K$D$$$F!$$=$NM-8zNN0h$N1&C<$N1&NY$H$J$k2hAG0LCV$rJV$9!%(B
  \param v	$B9T$r;XDj$9$k(Bintex
  \return	$B1&C<0LCV$N<!(B
*/
inline int
Warp::rmost(int v) const
{
    return _fracs[v].lmost + _fracs[v].width();
}

//! $B=PNO2hA|E@$r;XDj$7$F$=$l$K%^%C%W$5$l$kF~NO2hA|E@$N(B2$B<!85:BI8$rJV$9!%(B
/*!
  \param u	$B=PNO2hA|E@$N2#:BI8(B
  \param v	$B=PNO2hA|E@$N=D:BI8(B
  \return	$B=PNO2hA|E@(B(u, v)$B$K%^%C%W$5$l$kF~NO2hA|E@$N(B2$B<!85:BI8(B
*/
inline Vector2f
Warp::operator ()(int u, int v) const
{
    Vector2f		val;
    const FracArray&	fracs = _fracs[v];
    val[0] = float(fracs.us[u]) + float(fracs.du[u]) / 128.0;
    val[1] = float(fracs.vs[u]) + float(fracs.dv[u]) / 128.0;
    return val;
}

}

#endif	/* !__TUWarp_h */
