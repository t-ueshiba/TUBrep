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
 *  $Id: Geometry++.h,v 1.29 2008-10-03 04:23:37 ueshiba Exp $
 */
#ifndef __TUGeometryPP_h
#define __TUGeometryPP_h

#include "TU/Vector++.h"
#include "TU/Normalize.h"

namespace TU
{
/************************************************************************
*  class Point2<T>							*
************************************************************************/
//! T$B7?$N:BI8@.J,$r;}$D(B2$B<!85E@$rI=$9%/%i%9(B
/*!
  \param T	$B:BI8$N7?(B
 */
template <class T>
class Point2 : public Vector<T, FixedSizedBuf<T, 2> >
{
  private:
    typedef Vector<T, FixedSizedBuf<T, 2> >	array_type;
    
  public:
    Point2(T u=0, T v=0)						;

  //! $BB>$N(B2$B<!85%Y%/%H%k$HF10lMWAG$r;}$D(B2$B<!85E@$r:n$k!%(B
  /*!
    \param v	$B%3%T!<85(B2$B<!85%Y%/%H%k(B
  */
    template <class T2, class B2>
    Point2(const Vector<T2, B2>& v) :array_type(v)			{}

  //! $BB>$N(B2$B<!85%Y%/%H%k$r<+J,$KBeF~$9$k!%(B
  /*!
    \param v	$B%3%T!<85(B2$B<!85%Y%/%H%k(B
    \return	$B$3$N(B2$B<!85E@(B
  */
    template <class T2, class B2>
    Point2&	operator =(const Vector<T2, B2>& v)
		{
		    array_type::operator =(v);
		    return *this;
		}
    Point2	neighbor(int)					const	;
    Point2&	move(int)						;
    int		adj(const Point2&)				const	;
    int		dir(const Point2&)				const	;
    int		angle(const Point2&, const Point2&)		const	;
};

//! $B;XDj$5$l$?:BI8@.J,$r;}$D(B2$B<!85E@$r:n$k!%(B
/*!
  \param u	u$B:BI8(B
  \param v	v$B:BI8(B
*/
template <class T> inline
Point2<T>::Point2(T u, T v)
    :array_type()
{
    (*this)[0] = u;
    (*this)[1] = v;
}

//! $B;XDj$5$l$?J}8~$N(B8$B6aK5E@$rJV$9!%(B
/*!
  \param dir	8$B6aK5E@$NJ}8~(B(mod 8$B$G2r<a!%1&NY$r(B0$B$H$7!$;~7W2s$j$K(B1$B$E$DA}2C(B)
  \return	8$B6aK5E@(B
*/
template <class T> inline Point2<T>
Point2<T>::neighbor(int dir) const
{
    return Point2(*this).move(dir);
}

//! $B;XDj$5$l$?J}8~$N(B8$B6aK5E@$K<+?H$r0\F0$9$k!%(B
/*!
  \param dir	8$B6aK5E@$NJ}8~(B(mod 8$B$G2r<a!%1&NY$r(B0$B$H$7!$;~7W2s$j$K(B1$B$E$DA}2C(B)
  \return	$B0\F08e$N$3$NE@(B
*/
template <class T> Point2<T>&
Point2<T>::move(int dir)
{
    switch (dir % 8)
    {
      case 0:
	++(*this)[0];
	break;
      case 1:
      case -7:
	++(*this)[0];
	++(*this)[1];
	break;
      case 2:
      case -6:
	++(*this)[1];
	break;
      case 3:
      case -5:
	--(*this)[0];
	++(*this)[1];
	break;
      case 4:
      case -4:
	--(*this)[0];
	break;
      case 5:
      case -3:
	--(*this)[0];
	--(*this)[1];
	break;
      case 6:
      case -2:
	--(*this)[1];
	break;
      case 7:
      case -1:
	++(*this)[0];
	--(*this)[1];
	break;
    }
    return *this;
}

//! $B$3$N(B3$B<!85E@$H;XDj$5$l$?(B3$B<!85E@$,(B8$BNY@\$7$F$$$k$+D4$Y$k!%(B
/*!
  \param p	3$B<!85E@(B
  \return	p$B$H0lCW$7$F$$$l$P(B-1$B!$(B8$BNY@\$7$F$$$l$P(B1$B!$$$$:$l$G$b$J$1$l$P(B0
*/
template <class T> int
Point2<T>::adj(const Point2<T>& p) const
{
    const int	du = int(p[0] - (*this)[0]), dv = int(p[1] - (*this)[1]);

    if (du == 0 && dv == 0)
        return -1;
    switch (du)
    {
      case -1:
      case 0:
      case 1:
        switch (dv)
        {
          case -1:
          case 0:
          case 1:
            return 1;
          default:
            return 0;
        }
        break;
    }
    return 0;
}

//! $B$3$N(B3$B<!85E@$+$i;XDj$5$l$?(B3$B<!85E@$X$N8~$-$rJV$9!%(B
/*!
  \param p	3$B<!85E@(B
  \return	-180deg$B$+$i(B180deg$B$^$G$r(B8$BEyJ,$7$?6h4V$rI=$9(B-4$B$+$i(B3$B$^$G$N@0?tCM!%(B
		$BFC$K!$(Bp$B$,$3$NE@$K0lCW$9$k$J$i$P(B4
*/
template <class T> int
Point2<T>::dir(const Point2<T>& p) const
{
    const int	du = int(p[0] - (*this)[0]), dv = int(p[1] - (*this)[1]);

    if (du == 0 && dv == 0)
        return 4;
    if (dv >= 0)
        if (du > dv)
            return 0;
        else if (du > 0)
            return 1;
        else if (du > -dv)
            return 2;
        else if (dv > 0)
            return 3;
        else
            return -4;
    else
        if (du >= -dv)
            return -1;
        else if (du >= 0)
            return -2;
        else if (du >= dv)
            return -3;
        else
            return -4;
}

//! $B$3$N(B3$B<!85E@$H;XDj$5$l$?(B2$B$D$N(B3$B<!85E@$,$J$93QEY$rJV$9!%(B
/*!
  \param pp	3$B<!85E@(B
  \param pn	3$B<!85E@(B
  \return	pp->*this->pn$B$,$J$93QEY$r(B-180deg$B$+$i(B180deg$B$^$G$r(B8$BEyJ,$7$?6h4V$G(B
		$BI=$7$?(B-4$B$+$i(B3$B$^$G$N@0?tCM!%FC$K!$(Bpp, pn$B$N>/$J$/$H$b0lJ}$,$3$NE@$K(B
		$B0lCW$9$k$J$i$P(B4
*/
template <class T> int
Point2<T>::angle(const Point2<T>& pp, const Point2<T>& pn) const
{
    int dp = pp.dir(*this), ang = dir(pn);
    
    if (dp == 4 || ang == 4)
        return 4;
    else if ((ang -= dp) > 3)
        return ang - 8;
    else if (ang < -4)
        return ang + 8;
    else
        return ang;
}

typedef Point2<short>	Point2s;		//!< short$B7?:BI8$r;}$D(B2$B<!85E@(B
typedef Point2<int>	Point2i;		//!< int$B7?:BI8$r;}$D(B2$B<!85E@(B
typedef Point2<float>	Point2f;		//!< float$B7?:BI8$r;}$D(B2$B<!85E@(B
typedef Point2<double>	Point2d;		//!< double$B7?:BI8$r;}$D(B2$B<!85E@(B

/************************************************************************
*  class Point3<T>							*
************************************************************************/
//! T$B7?$N:BI8@.J,$r;}$D(B3$B<!85E@$rI=$9%/%i%9(B
/*!
  \param T	$B:BI8$N7?(B
 */
template <class T>
class Point3 : public Vector<T, FixedSizedBuf<T, 3> >
{
  private:
    typedef Vector<T, FixedSizedBuf<T, 3> >	array_type;
    
  public:
    Point3(T x=0, T y=0, T z=0)						;

  //! $BB>$N(B3$B<!85%Y%/%H%k$HF10lMWAG$r;}$D(B3$B<!85E@$r:n$k!%(B
  /*!
    \param v	$B%3%T!<85(B3$B<!85%Y%/%H%k(B
  */
    template <class T2, class B2>
    Point3(const Vector<T2, B2>& v) :array_type(v)			{}

  //! $BB>$N(B3$B<!85%Y%/%H%k$r<+J,$KBeF~$9$k!%(B
  /*!
    \param v	$B%3%T!<85(B3$B<!85%Y%/%H%k(B
    \return	$B$3$N(B3$B<!85E@(B
  */
    template <class T2, class B2>
    Point3&	operator =(const Vector<T2, B2>& v)
		{
		    array_type::operator =(v);
		    return *this;
		}
};

//! $B;XDj$5$l$?:BI8@.J,$r;}$D(B3$B<!85E@$r:n$k!%(B
/*!
  \param x	x$B:BI8(B
  \param y	y$B:BI8(B
  \param z	z$B:BI8(B
*/
template <class T> inline
Point3<T>::Point3(T x, T y, T z)
    :array_type()
{
    (*this)[0] = x;
    (*this)[1] = y;
    (*this)[2] = z;
}

typedef Point3<short>	Point3s;		//!< short$B7?:BI8$r;}$D(B3$B<!85E@(B
typedef Point3<int>	Point3i;		//!< int$B7?:BI8$r;}$D(B3$B<!85E@(B
typedef Point3<float>	Point3f;		//!< float$B7?:BI8$r;}$D(B3$B<!85E@(B
typedef Point3<double>	Point3d;		//!< double$B7?:BI8$r;}$D(B3$B<!85E@(B

/************************************************************************
*  class HyperPlane							*
************************************************************************/
//! d$B<!85<M1F6u4VCf$ND6J?LL$rI=8=$9$k%/%i%9(B
/*!
  d$B<!85<M1F6u4V$NE@(B\f$\TUud{x}{} \in \TUspace{R}{d+1}\f$$B$KBP$7$F(B
  \f$\TUtud{p}{}\TUud{x}{} = 0,~\TUud{p}{} \in \TUspace{R}{d+1}\f$
  $B$K$h$C$FI=$5$l$k!%(B
*/
template <class T, class B=Buf<T> >
class HyperPlane : public Vector<T, B>
{
  public:
    HyperPlane(u_int d=2)						;

  //! $BF1<!:BI8%Y%/%H%k$r;XDj$7$FD6J?LL%*%V%8%'%/%H$r@8@.$9$k!%(B
  /*!
    \param p	(d+1)$B<!85%Y%/%H%k!J(Bd$B$OD6J?LL$,B8:_$9$k<M1F6u4V$N<!85!K(B
  */
    template <class T2, class B2>
    HyperPlane(const Vector<T2, B2>& p)	:Vector<T, B>(p)		{}

    template <class Iterator>
    HyperPlane(Iterator first, Iterator last)				;

  //! $BD6J?LL%*%V%8%'%/%H$NF1<!:BI8%Y%/%H%k$r;XDj$9$k!%(B
  /*!
    \param v	(d+1)$B<!85%Y%/%H%k!J(Bd$B$OD6J?LL$,B8:_$9$k<M1F6u4V$N<!85!K(B
    \return	$B$3$ND6J?LL%*%V%8%'%/%H(B
  */
    template <class T2, class B2>
    HyperPlane&	operator =(const Vector<T2, B2>& v)
				{Vector<T, B>::operator =(v); return *this;}

    template <class Iterator>
    void	fit(Iterator first, Iterator last)			;

  //! $B$3$ND6J?LL$,B8:_$9$k<M1F6u4V$N<!85$rJV$9!%(B
  /*! 
    \return	$B<M1F6u4V$N<!85(B($BF1<!:BI8$N%Y%/%H%k$H$7$F$N<!85$O(B#spaceDim()+1)
  */
    u_int	spaceDim()		const	{return Vector<T, B>::dim()-1;}

  //! $BD6J?LL$r5a$a$k$?$a$KI,MW$JE@$N:G>.8D?t$rJV$9!%(B
  /*!
    $B8=:_@_Dj$5$l$F$$$k<M1F6u4V$N<!85$r$b$H$K7W;;$5$l$k!%(B
    \return	$BI,MW$JE@$N:G>.8D?t$9$J$o$AF~NO6u4V$N<!85(B#spaceDim()
  */
    u_int	ndataMin()		const	{return spaceDim();}

    template <class T2, class B2> inline T
    sqdist(const Vector<T2, B2>& x)		const	;
    template <class T2, class B2> inline double
    dist(const Vector<T2, B2>& x)		const	;
};

//! $B6u4V$N<!85$r;XDj$7$FD6J?LL%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  $BL58B1sD6J?LL(B([0, 0,..., 0, 1])$B$K=i4|2=$5$l$k!%(B
  \param d	$B$3$ND6J?LL$,B8:_$9$k<M1F6u4V$N<!85(B
*/
template <class T, class B> inline
HyperPlane<T, B>::HyperPlane(u_int d)
    :Vector<T, B>(d + 1)
{
    (*this)[d] = 1;
}
    
//! $BM?$($i$l$?E@Ns$NHsF1<!:BI8$KEv$F$O$a$i$l$?D6J?LL%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  \param first			$BE@Ns$N@hF,$r<($9H?I|;R(B
  \param last			$BE@Ns$NKvHx$r<($9H?I|;R(B
  \throw std::invalid_argument	$BE@$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class T, class B> template <class Iterator> inline
HyperPlane<T, B>::HyperPlane(Iterator first, Iterator last)
{
    fit(first, last);
}

//! $BM?$($i$l$?E@Ns$NHsF1<!:BI8$KD6J?LL$rEv$F$O$a$k!%(B
/*!
  \param first			$BE@Ns$N@hF,$r<($9H?I|;R(B
  \param last			$BE@Ns$NKvHx$r<($9H?I|;R(B
  \throw std::invalid_argument	$BE@$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class T, class B> template <class Iterator> void
HyperPlane<T, B>::fit(Iterator first, Iterator last)
{
  // $BE@Ns$N@55,2=(B
    const Normalize	normalize(first, last);

  // $B=<J,$J8D?t$NE@$,$"$k$+!)(B
    const u_int		ndata = std::distance(first, last),
			d     = normalize.spaceDim();
    if (ndata < d)	// Vector<T, B>$B$N%5%$%:$,L$Dj$J$N$G(BndataMin()$B$OL58z(B
	throw std::invalid_argument("Hyperplane::initialize(): not enough input data!!");

  // $B%G!<%?9TNs$N7W;;(B
    Matrix<T>	A(d, d);
    while (first != last)
    {
	const Vector<T>&	x = normalize(*first++);
	A += x % x;
    }

  // $B%G!<%?9TNs$N:G>.8GM-CM$KBP1~$9$k8GM-%Y%/%H%k$+$iK!@~%Y%/%H%k$r7W;;$7!$(B
  // $B$5$i$KE@Ns$N=E?4$h$j86E@$+$i$N5wN%$r7W;;$9$k!%(B
    Vector<T>		eval;
    const Matrix<T>&	Ut = A.eigen(eval);
    Vector<T, B>::resize(d+1);
    (*this)(0, d) = Ut[Ut.nrow()-1];
    (*this)[d] = -((*this)(0, d)*normalize.centroid());
    if ((*this)[d] > 0.0)
	Vector<T, B>::operator *=(-1.0);
}

//! $BM?$($i$l$?E@$HD6J?LL$N5wN%$N(B2$B>h$rJV$9!%(B
/*!
  \param x	$BE@$NHsF1<!:BI8!J(B#spaceDim()$B<!85!K$^$?$OF1<!:BI8(B
		$B!J(B#spaceDim()+1$B<!85!K(B
  \return	$BE@$HD6J?LL$N5wN%$N(B2$B>h(B
*/
template <class T, class B> template <class T2, class B2> inline T
HyperPlane<T, B>::sqdist(const Vector<T2, B2>& x) const
{
    const double	d = dist(x);
    return d*d;
}

//! $BM?$($i$l$?E@$HD6J?LL$N5wN%$rJV$9!%(B
/*!
  \param x			$BE@$NHsF1<!:BI8!J(B#spaceDim()$B<!85!K$^$?$O(B
				$BF1<!:BI8!J(B#spaceDim()+1$B<!85!K(B
  \return			$BE@$HD6J?LL$N5wN%!JHsIi!K(B
  \throw std::invalid_argument	$BE@$N%Y%/%H%k$H$7$F$N<!85$,(B#spaceDim()$B!$(B
				#spaceDim()+1$B$N$$$:$l$G$b$J$$>l9g!$$b$7$/$O(B
				$B$3$NE@$,L58B1sE@$G$"$k>l9g$KAw=P!%(B
*/
template <class T, class B> template <class T2, class B2> double
HyperPlane<T, B>::dist(const Vector<T2, B2>& x) const
{
    const Vector<T2>&	p = (*this)(0, spaceDim());
    if (x.dim() == spaceDim())
	return (p * x + (*this)[spaceDim()]) / p.length();
    else if (x.dim() == spaceDim() + 1)
    {
	if (x[spaceDim()] == 0.0)
	    throw std::invalid_argument("HyperPlane::dist(): point at infinitiy!!");
	return (*this * x) / (p.length() * x[spaceDim()]);
    }
    else
	throw std::invalid_argument("HyperPlane::dist(): dimension mismatch!!");

    return 0;
}

typedef HyperPlane<float,  FixedSizedBuf<float,  3> >
	LineP2f;			//!< float$B7?:BI8$r;}$D(B2$B<!856u4VCf$ND>@~(B
typedef HyperPlane<double, FixedSizedBuf<double, 3> >
	LineP2d;			//!< double$B7?:BI8$r;}$D(B2$B<!856u4VCf$ND>@~(B
typedef HyperPlane<float,  FixedSizedBuf<float,  4> >
	PlaneP3f;			//!< float$B7?:BI8$r;}$D(B3$B<!856u4VCf$NJ?LL(B
typedef HyperPlane<double, FixedSizedBuf<double, 4> >
	PlaneP3d;			//!< double$B7?:BI8$r;}$D(B3$B<!856u4VCf$NJ?LL(B
}
#endif	/* !__TUGeometryPP_h */
