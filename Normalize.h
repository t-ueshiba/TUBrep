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
 *  $Id: Normalize.h,v 1.1 2008-09-11 00:14:16 ueshiba Exp $  
 */
#ifndef __TUNormalize_h
#define __TUNormalize_h

#include "TU/Vector++.h"

namespace TU
{
/************************************************************************
*  class Normalize							*
************************************************************************/
//! $BE@$NHsF1<!:BI8$N@55,2=JQ49$r9T$&%/%i%9(B
/*!
  \f$\TUud{x}{}=[\TUtvec{x}{}, 1]^\top~
  (\TUvec{x}{} \in \TUspace{R}{d})\f$$B$KBP$7$F!$0J2<$N$h$&$JJ?9T0\F0$H(B
  $B%9%1!<%j%s%0$r9T$&(B:
  \f[
	\TUud{y}{} =
	\TUbeginarray{c} s^{-1}(\TUvec{x}{} - \TUvec{c}{}) \\ 1	\TUendarray =
	\TUbeginarray{ccc}
	  s^{-1} \TUvec{I}{d} & -s^{-1}\TUvec{c}{} \\ \TUtvec{0}{d} & 1
	\TUendarray
	\TUbeginarray{c} \TUvec{x}{} \\ 1 \TUendarray =
	\TUvec{T}{}\TUud{x}{}
  \f]
  \f$s\f$$B$H(B\f$\TUvec{c}{}\f$$B$O!$?6I}$N(B2$B>hJ?6QCM$,6u4V$N<!85(B\f$d\f$$B$K(B,
  $B=E?4$,86E@$K$J$k$h$&7hDj$5$l$k!%(B
*/
class Normalize
{
  public:
  //! $B6u4V$N<!85$r;XDj$7$F@55,2=JQ49%*%V%8%'%/%H$r@8@.$9$k!%(B
  /*!
    $B91EyJQ49$H$7$F=i4|2=$5$l$k!%(B
    \param d	$B6u4V$N<!85(B
  */
    Normalize(u_int d=2) :_npoints(0), _scale(1.0), _centroid(d)	{}

    template <class Iterator>
    Normalize(Iterator first, Iterator last)				;
    
    template <class Iterator>
    void		update(Iterator first, Iterator last)		;

    u_int		spaceDim()				const	;
    template <class T2, class B2>
    Vector<double>	operator ()(const Vector<T2, B2>& x)	const	;
    template <class T2, class B2>
    Vector<double>	normalizeP(const Vector<T2, B2>& x)	const	;
    
    Matrix<double>		T()				const	;
    Matrix<double>		Tt()				const	;
    Matrix<double>		Tinv()				const	;
    Matrix<double>		Ttinv()				const	;
    double			scale()				const	;
    const Vector<double>&	centroid()			const	;
    
  private:
    u_int		_npoints;	//!< $B$3$l$^$G$KM?$($?E@$NAm?t(B
    double		_scale;		//!< $B$3$l$^$G$KM?$($?E@$N?6I}$N(BRMS$BCM(B
    Vector<double>	_centroid;	//!< $B$3$l$^$G$KM?$($?E@72$N=E?4(B
};

//! $BM?$($i$l$?E@72$NHsF1<!:BI8$+$i@55,2=JQ49%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  $B?6I}$N(B2$B>hJ?6QCM$,(B#spaceDim(), $B=E?4$,86E@$K$J$k$h$&$J@55,2=JQ49$,7W;;$5$l$k!%(B
  \param first	$BE@72$N@hF,$r<($9H?I|;R(B
  \param last	$BE@72$NKvHx$r<($9H?I|;R(B
*/
template <class Iterator> inline
Normalize::Normalize(Iterator first, Iterator last)
    :_npoints(0), _scale(1.0), _centroid()
{
    update(first, last);
}
    
//! $B?7$?$KE@72$rDI2C$7$F$=$NHsF1<!:BI8$+$i8=:_$N@55,2=JQ49$r99?7$9$k!%(B
/*!
  $B?6I}$N(B2$B>hJ?6QCM$,(B#spaceDim(), $B=E?4$,86E@$K$J$k$h$&$J@55,2=JQ49$,7W;;$5$l$k!%(B
  \param first			$BE@72$N@hF,$r<($9H?I|;R(B
  \param last			$BE@72$NKvHx$r<($9H?I|;R(B
  \throw std::invalid_argument	$B$3$l$^$G$KM?$($i$l$?E@$NAm?t$,(B0$B$N>l9g$KAw=P(B
*/
template <class Iterator> void
Normalize::update(Iterator first, Iterator last)
{
    if (_npoints == 0)
    {
	if (first == last)
	    throw std::invalid_argument("Normalize::update(): 0-length input data!!");
	_centroid.resize(first->dim());
    }
    _scale = _npoints * (spaceDim() * _scale * _scale + _centroid * _centroid);
    _centroid *= _npoints;
    while (first != last)
    {
	_scale += first->square();
	_centroid += *first++;
	++_npoints;
    }
    if (_npoints == 0)
	throw std::invalid_argument("Normalize::update(): no input data accumulated!!");
    _centroid /= _npoints;
    _scale = sqrt((_scale / _npoints - _centroid * _centroid) / spaceDim());
}

//! $B$3$N@55,2=JQ49$,E,MQ$5$l$k6u4V$N<!85$rJV$9!%(B
/*! 
  \return	$B6u4V$N<!85(B($BF1<!:BI8$N%Y%/%H%k$H$7$F$N<!85$O(B#spaceDim()+1)
*/
inline u_int
Normalize::spaceDim() const
{
    return _centroid.dim();
}
    
//! $BM?$($i$l$?E@$K@55,2=JQ49$rE,MQ$7$F$=$NHsF1<!:BI8$rJV$9!%(B
/*!
  \param x	$BE@$NHsF1<!:BI8!J(B#spaceDim()$B<!85!K(B
  \return	$B@55,2=$5$l$?E@$NHsF1<!:BI8!J(B#spaceDim()$B<!85!K(B
*/
template <class T2, class B2> inline Vector<double>
Normalize::operator ()(const Vector<T2, B2>& x) const
{
    return (Vector<double>(x) -= _centroid) /= _scale;
}

//! $BM?$($i$l$?E@$K@55,2=JQ49$rE,MQ$7$F$=$NF1<!:BI8$rJV$9!%(B
/*!
  \param x	$BE@$NHsF1<!:BI8!J(B#spaceDim()$B<!85!K(B
  \return	$B@55,2=$5$l$?E@$NF1<!:BI8!J(B#spaceDim()+1$B<!85!K(B
*/
template <class T2, class B2> inline Vector<double>
Normalize::normalizeP(const Vector<T2, B2>& x) const
{
    return (*this)(x).homogenize();
}

//! $B@55,2=JQ49$N%9%1!<%j%s%0Dj?t$rJV$9!%(B
/*!
  \return	$B%9%1!<%j%s%0Dj?t!JM?$($i$l$?E@Ns$N?6I}$N(B2$B>hJ?6QCM!K(B
*/
inline double
Normalize::scale() const
{
    return _scale;
}

//! $B@55,2=JQ49$NJ?9T0\F0@.J,$rJV$9!%(B
/*!
  \return	$BJ?9T0\F0@.J,!JM?$($i$l$?E@Ns$N=E?4!K(B
*/
inline const Vector<double>&
Normalize::centroid() const
{
    return _centroid;
}

}
#endif	/* !__TUNormalize_h */
