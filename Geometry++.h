/*
 *  $Id: Geometry++.h,v 1.13 2007-02-22 23:23:21 ueshiba Exp $
 */
#ifndef __TUGeometryPP_h
#define __TUGeometryPP_h

#include "TU/Minimize++.h"

namespace TU
{
/************************************************************************
*  class CoordBase<T, D>						*
************************************************************************/
template <class T, u_int D>
class CoordBase
{
  public:
    typedef T	ET;
    
    CoordBase()							{*this = 0.0;}
    CoordBase(const CoordBase&)					;
    CoordBase(const Vector<double>&)				;
    CoordBase&		operator =(const CoordBase&)		;
    CoordBase&		operator =(const Vector<double>&)	;
    
    CoordBase&		operator +=(const CoordBase&)		;
    CoordBase&		operator -=(const CoordBase&)		;
    CoordBase&		operator  =(double c)			;
    CoordBase&		operator *=(double c)			;
    CoordBase&		operator /=(double c)			;
    CoordBase		operator  -()			const	{CoordBase
								     r(*this);
								 r *= -1;
								 return r;}
			operator T*()			const	{return(T*)_p;}
			operator Vector<double>()	const	;
    static u_int	dim()					{return D;}
    const T&		operator [](int i)		const	{return _p[i];}
    T&			operator [](int i)			{return _p[i];}
    int			operator ==(const CoordBase&)	const	;
    int			operator !=(const CoordBase& p) const
			{
			    return !(*this == p);
			}
    std::istream&	restore(std::istream&)		;
    std::ostream&	save(std::ostream&)	const	;
    double		square()		const	;
    double		length()		const	{return
							   sqrt(square());}
    CoordBase&		normalize()			{return
							   *this /= length();}
    CoordBase		normal()		const	;
    void		check_dim(u_int d)	const	;

  private:
    T		_p[D];
};

template <class T, u_int D> inline double
CoordBase<T, D>::square() const
{
    return Vector<double>(*this).square();
}

/*
 *  I/O functions
 */
template <class T, u_int D> std::istream&
operator >>(std::istream& in, CoordBase<T, D>& p)		;
template <class T, u_int D> std::ostream&
operator <<(std::ostream& out, const CoordBase<T, D>& p)	;

/*
 *  numerical operators
 */
template <class T, u_int D> T
operator *(const CoordBase<T, D>&, const CoordBase<T, D>&)	;

template <class T, u_int D> inline CoordBase<T, D>
operator +(const CoordBase<T, D>& a, const CoordBase<T, D>& b)
    {CoordBase<T, D> r(a); r += b; return r;}

template <class T, u_int D> inline CoordBase<T, D>
operator -(const CoordBase<T, D>& a, const CoordBase<T, D>& b)
    {CoordBase<T, D> r(a); r -= b; return r;}

template <class T, u_int D> inline CoordBase<T, D>
operator *(double c, const CoordBase<T, D>& a)
    {CoordBase<T, D> r(a); r *= c; return r;}

template <class T, u_int D> inline CoordBase<T, D>
operator *(const Array<T>& a, double c)
    {CoordBase<T, D> r(a); r *= c; return r;}

template <class T, u_int D> inline CoordBase<T, D>
operator /(const Array<T>& a, double c)
    {CoordBase<T, D> r(a); r /= c; return r;}

/************************************************************************
*  class Coordinate<T, D>						*
************************************************************************/
template <class T, u_int D>
class CoordinateP;

template <class T, u_int D>
class Coordinate : public CoordBase<T, D>
{
  public:
    Coordinate()				:CoordBase<T, D>()	{}
    Coordinate(const Coordinate& p)		:CoordBase<T, D>(p)	{}
    Coordinate(const Vector<double>& v)	:CoordBase<T, D>(v)		{}
    Coordinate(const CoordinateP<T, D>&)				;
    Coordinate&		operator =(const Coordinate& p)
			{CoordBase<T, D>::operator =(p); return *this;}
    Coordinate&		operator =(const Vector<double>& v)
			{CoordBase<T, D>::operator =(v); return *this;}
    Coordinate&		operator =(const CoordinateP<T, D>&)		;

    using		CoordBase<T, D>::dim;
    Coordinate&		operator +=(const Coordinate& p)
			{
			    CoordBase<T, D>::operator +=(p);
			    return *this;
			}
    Coordinate&		operator -=(const Coordinate& p)
			{
			    CoordBase<T, D>::operator -=(p);
			    return *this;
			}
    Coordinate&		operator  =(double c)
			{
			    CoordBase<T, D>::operator =(c);
			    return *this;
			}
    Coordinate&		operator *=(double c)
			{
			    CoordBase<T, D>::operator *=(c);
			    return *this;
			}
    Coordinate&		operator /=(double c)
			{
			    CoordBase<T, D>::operator /=(c);
			    return *this;
			}
    Coordinate		operator -() const
			{
			    Coordinate	r(*this);
			    r *= -1;
			    return r;
			}
    double		sqdist(const Coordinate& p)		 const	;
    double		dist(const Coordinate& p) const
			{
			    return sqrt(sqdist(p));
			}
};

/*
 *  numerical operators
 */
template <class T> Coordinate<T, 3u>
operator ^(const Coordinate<T, 3u>&, const Coordinate<T, 3u>&);

template <class T, u_int D> inline Coordinate<T, D>
operator +(const Coordinate<T, D>& a, const Coordinate<T, D>& b)
    {Coordinate<T, D> r(a); r += b; return r;}

template <class T, u_int D> inline Coordinate<T, D>
operator -(const Coordinate<T, D>& a, const Coordinate<T, D>& b)
    {Coordinate<T, D> r(a); r -= b; return r;}

template <class T, u_int D> inline Coordinate<T, D>
operator *(double c, const Coordinate<T, D>& a)
    {Coordinate<T, D> r(a); r *= c; return r;}

template <class T, u_int D> inline Coordinate<T, D>
operator *(const Coordinate<T, D>& a, double c)
    {Coordinate<T, D> r(a); r *= c; return r;}

template <class T, u_int D> inline Coordinate<T, D>
operator /(const Coordinate<T, D>& a, double c)
    {Coordinate<T, D> r(a); r /= c; return r;}

/************************************************************************
*  class CoordinateP<T, D>						*
************************************************************************/
template <class T, u_int D>
class CoordinateP : public CoordBase<T, D+1u>
{
  public:
    CoordinateP()			     :CoordBase<T, D+1u>()	{}
    CoordinateP(const CoordinateP& p)    :CoordBase<T, D+1u>(p)	{}
    CoordinateP(const Vector<double>& v) :CoordBase<T, D+1u>(v)	{}
    CoordinateP(const Coordinate<T, D>&)				;

    using		CoordBase<T, D+1u>::dim;
    CoordinateP&	operator =(const CoordinateP& p)
			{CoordBase<T, D+1u>::operator =(p); return *this;}
    CoordinateP&	operator =(const Vector<double>& v)
			{CoordBase<T, D+1u>::operator =(v); return *this;}
    CoordinateP&	operator =(const Coordinate<T, D>&)		;

    CoordinateP&	operator  =(double c)
			{
			    CoordBase<T, D+1u>::operator =(c);
			    return *this;
			}
    CoordinateP&	operator *=(double c)
			{
			    CoordBase<T, D+1u>::operator *=(c);
			    return *this;
			}
    CoordinateP&	operator /=(double c)
			{
			    CoordBase<T, D+1u>::operator /=(c);
			    return *this;
			}
    CoordinateP		operator -() const
			{
			    CoordinateP	r(*this);
			    r *= -1;
			    return r;
			}
    int			operator ==(const CoordinateP& p)	const	;
};

/*
 *  numerical operators
 */
template <class T> CoordinateP<T, 2u>
operator ^(const CoordinateP<T, 2u>&, const CoordinateP<T, 2u>&);

template <class T, u_int D> inline CoordinateP<T, D>
operator *(double c, const CoordinateP<T, D>& a)
    {CoordinateP<T, D> r(a); r *= c; return r;}

template <class T, u_int D> inline CoordinateP<T, D>
operator *(const CoordinateP<T, D>& a, double c)
    {CoordinateP<T, D> r(a); r *= c; return r;}

template <class T, u_int D> inline CoordinateP<T, D>
operator /(const CoordinateP<T, D>& a, double c)
    {CoordinateP<T, D> r(a); r /= c; return r;}

/************************************************************************
*  class Point2<T>							*
************************************************************************/
template <class T>	class PointP2;
template <class T>
class Point2 : public Coordinate<T, 2u>
{
  public:
    Point2()				:Coordinate<T, 2u>()	{}
    Point2(T, T)						;
    Point2(const Point2<T>& p)
	:Coordinate<T, 2u>((const Coordinate<T, 2u>&)p)	{}
    Point2(const PointP2<T>& p)
	:Coordinate<T, 2u>((const CoordinateP<T, 2u>&)p)	{}
    Point2(const Vector<double>& v)	:Coordinate<T, 2u>(v)	{}
    Point2&	operator =(const Point2<T>& p)
		{
		    Coordinate<T, 2u>::operator =(p);
		    return *this;
		}
    Point2&	operator =(const PointP2<T>& p)
		{
		    Coordinate<T, 2u>::operator =(p);
		    return *this;
		}
    Point2&	operator =(const Vector<double>& v)
		{
		    Coordinate<T, 2u>::operator =(v);
		    return *this;
		}
    Point2&	operator +=(const Point2& p)
		{
		    Coordinate<T, 2u>::operator +=(p);
		    return *this;
		}
    Point2&	operator -=(const Point2& p)
		{
		    Coordinate<T, 2u>::operator -=(p);
		    return *this;
		}
    Point2&	operator  =(double c)
		{
		    Coordinate<T, 2u>::operator =(c);
		    return *this;
		}
    Point2&	operator *=(double c)
		{
		    Coordinate<T, 2u>::operator *=(c);
		    return *this;
		}
    Point2&	operator /=(double c)
		{
		    Coordinate<T, 2u>::operator /=(c);
		    return *this;
		}
    Point2	operator -() const
		{
		    Point2	r(*this);
		    r *= -1;
		    return r;
		}

    Point2	neighbor(int)				const	;
    Point2&	move(int)					;
    int		adj(const Point2&)			const	;
    int		dir(const Point2&)			const	;
    int		angle(const Point2&, const Point2&)	const	;
};

template <class T> inline
Point2<T>::Point2(T u, T v)
    :Coordinate<T, 2u>()
{
    (*this)[0] = u;
    (*this)[1] = v;
}

template <class T> inline Point2<T>
Point2<T>::neighbor(int dir) const
{
    Point2<T>	d(*this);
    return d.move(dir);
}

/************************************************************************
*  class PointP2<T>							*
************************************************************************/
template <class T>
class PointP2 : public CoordinateP<T, 2u>
{
  public:
    PointP2()				:CoordinateP<T, 2u>()		{}
    PointP2(T, T)							;
    PointP2(const PointP2<T>& p)
	:CoordinateP<T, 2u>((const CoordinateP<T, 2u>&)p)		{}
    PointP2(const Point2<T>& p)
	:CoordinateP<T, 2u>((const Coordinate<T, 2u>&)p)		{}
    PointP2(const Vector<double>& v)	:CoordinateP<T, 2u>(v)		{}
    PointP2&	operator =(const PointP2<T>& p)
		{
		    CoordinateP<T, 2u>::operator =(p);
		    return *this;
		}
    PointP2&	operator =(const Point2<T>& p)
		{
		    CoordinateP<T, 2u>::operator =(p);
		    return *this;
		}
    PointP2&	operator =(const Vector<double>& v)
		{
		    CoordinateP<T, 2u>::operator =(v);
		    return *this;
		}
    PointP2&	operator  =(double c)
		{
		    CoordinateP<T, 2u>::operator =(c);
		    return *this;
		}
    PointP2&	operator *=(double c)
		{
		    CoordinateP<T, 2u>::operator *=(c);
		    return *this;
		}
    PointP2&	operator /=(double c)
		{
		    CoordinateP<T, 2u>::operator /=(c);
		    return *this;
		}
    PointP2	operator -() const
		{
		    PointP2	r(*this);
		    r *= -1;
		    return r;
		}
    PointP2	neighbor(int)					const	;
    PointP2&	move(int)						;
    int		adj(const PointP2&)				const	;
    int		dir(const PointP2&)				const	;
    int		angle(const PointP2&, const PointP2&)		const	;
};

template <class T> inline
PointP2<T>::PointP2(T u, T v)
    :CoordinateP<T, 2u>()
{
    (*this)[0] = u;
    (*this)[1] = v;
    (*this)[2] = 1;
}

template <class T> inline PointP2<T>
PointP2<T>::neighbor(int dir) const
{
    Point2<T>	d(*this);
    return d.move(dir);
}

template <class T> inline PointP2<T>&
PointP2<T>::move(int dir)
{
    Point2<T>	d(*this);
    *this = d.move(dir);
    return *this;
}

template <class T> int
PointP2<T>::dir(const PointP2<T>& p) const
{
    Point2<T>	d(*this);
    return d.dir(p);
}

template <class T> int
PointP2<T>::angle(const PointP2<T>& pp, const PointP2<T>& pn) const
{
    Point2<T>	d(*this);
    return d.angle(pp, pn);
}

/************************************************************************
*  class LineP2<T>							*
************************************************************************/
template <class T>
class LineP2 : public CoordinateP<T, 2u>
{
  public:
    LineP2()				:CoordinateP<T, 2u>()		{}
    LineP2(const LineP2<T>& l)
	:CoordinateP<T, 2u>((const CoordinateP<T, 2u>&)l)		{}
    LineP2(const Vector<double>& v)	:CoordinateP<T, 2u>(v)		{}

    CoordinateP<T, 2u>::operator [];	// should not be needed, but due to
					// the bug in SGI C++ compiler....
};

/************************************************************************
*  class PlaneP3<T>							*
************************************************************************/
template <class T>
class PlaneP3 : public CoordinateP<T, 3u>
{
  public:
    PlaneP3()				:CoordinateP<T, 3u>()		{}
    PlaneP3(const PlaneP3<T>& l)
	:CoordinateP<T, 3u>((const CoordinateP<T, 3u>&)l)		{}
    PlaneP3(const Vector<double>& v):CoordinateP<T, 3u>(v)		{}
};

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
    Vector<double>	operator ()(const Vector<double>& x)	const	;
    Vector<double>	normalizeP(const Vector<double>& x)	const	;
    
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
inline Vector<double>
Normalize::operator ()(const Vector<double>& x) const
{
    return (x - _centroid) / _scale;
}

//! $BM?$($i$l$?E@$K@55,2=JQ49$rE,MQ$7$F$=$NF1<!:BI8$rJV$9!%(B
/*!
  \param x	$BE@$NHsF1<!:BI8!J(B#spaceDim()$B<!85!K(B
  \return	$B@55,2=$5$l$?E@$NF1<!:BI8!J(B#spaceDim()+1$B<!85!K(B
*/
inline Vector<double>
Normalize::normalizeP(const Vector<double>& x) const
{
    Vector<double>	val(spaceDim()+1);
    val(0, spaceDim()) = (*this)(x);
    val[spaceDim()] = 1.0;
    return val;
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

/************************************************************************
*  class ProjectiveMapping						*
************************************************************************/
//! $B<M1FJQ49$r9T$&%/%i%9(B
/*!
  \f$\TUvec{T}{} \in \TUspace{R}{(n+1)\times(m+1)}\f$$B$rMQ$$$F(Bm$B<!856u4V$NE@(B
  \f$\TUud{x}{} \in \TUspace{R}{m+1}\f$$B$r(Bn$B<!856u4V$NE@(B
  \f$\TUud{y}{} \simeq \TUvec{T}{}\TUud{x}{} \in \TUspace{R}{n+1}\f$
  $B$K<L$9!J(B\f$m \neq n\f$$B$G$b9=$o$J$$!K!%(B
*/
class ProjectiveMapping
{
  public:
    ProjectiveMapping(u_int inDim=2, u_int outDim=2)			;

  //! $BJQ499TNs$r;XDj$7$F<M1FJQ49%*%V%8%'%/%H$r@8@.$9$k!%(B
  /*!
    \param T			(m+1)x(n+1)$B9TNs!J(Bm, n$B$OF~NO!?=PNO6u4V$N<!85!K(B
  */
    ProjectiveMapping(const Matrix<double>& T)	:_T(T)			{}

    template <class Iterator>
    ProjectiveMapping(Iterator first, Iterator last, bool refine=false)	;

    template <class Iterator>
    void		initialize(Iterator first, Iterator last,
				   bool refine=false)			;

  //! $B$3$N<M1FJQ49$NF~NO6u4V$N<!85$rJV$9!%(B
  /*! 
    \return	$BF~NO6u4V$N<!85(B($BF1<!:BI8$N%Y%/%H%k$H$7$F$N<!85$O(B#inDim()+1)
  */
    u_int		inDim()			const	{return _T.ncol()-1;}

  //! $B$3$N<M1FJQ49$N=PNO6u4V$N<!85$rJV$9!%(B
  /*! 
    \return	$B=PNO6u4V$N<!85(B($BF1<!:BI8$N%Y%/%H%k$H$7$F$N<!85$O(B#outDim()+1)
  */
    u_int		outDim()		const	{return _T.nrow()-1;}

    u_int		ndataMin()		const	;
    
  //! $B$3$N<M1FJQ49$rI=8=$9$k9TNs$rJV$9!%(B
  /*! 
    \return	(#outDim()+1)x(#inDim()+1)$B9TNs(B
  */
    const Matrix<double>&	T()		const	{return _T;}
    
    Vector<double>	operator ()(const Vector<double>& x)	const	;
    Vector<double>	mapP(const Vector<double>& x)		const	;

    template <class In, class Out>
    double		sqdist(const std::pair<In, Out>& pair)	const	;
    template <class In, class Out>
    double		dist(const std::pair<In, Out>& pair)	const	;
    
  protected:
    Matrix<double>	_T;			//!< $B<M1FJQ49$rI=8=$9$k9TNs(B

  private:
  //! $B<M1FJQ499TNs$N:GL`?dDj$N$?$a$N%3%9%H4X?t(B
    class Cost
    {
      public:
	typedef double		ET;
	typedef Matrix<ET>	AT;

	template <class Iterator>
	Cost(Iterator first, Iterator last)				;

	Vector<ET>	operator ()(const AT& T)		const	;
	Matrix<ET>	jacobian(const AT& T)			const	;
	void		update(AT& T, const Vector<ET>& dt)	const	;

	u_int		npoints()		const	{return _X.nrow();}
	     
      private:
	Matrix<ET>	_X, _Y;
    };
};

//! $BM?$($i$l$?E@BPNs$NHsF1<!:BI8$+$i<M1FJQ49%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  \param first			$BE@BPNs$N@hF,$r<($9H?I|;R(B
  \param last			$BE@BPNs$NKvHx$r<($9H?I|;R(B
  \param refine			$BHs@~7?:GE,2=$NM-(B(true)$B!?L5(B(false)$B$r;XDj(B
  \throw std::invalid_argument	$BE@BP$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class Iterator> inline
ProjectiveMapping::ProjectiveMapping(Iterator first, Iterator last,
				     bool refine)
{
    initialize(first, last, refine);
}

//! $BM?$($i$l$?E@BPNs$NHsF1<!:BI8$+$i<M1FJQ49$r7W;;$9$k!%(B
/*!
  \param first			$BE@BPNs$N@hF,$r<($9H?I|;R(B
  \param last			$BE@BPNs$NKvHx$r<($9H?I|;R(B
  \param refine			$BHs@~7?:GE,2=$NM-(B(true)$B!?L5(B(false)$B$r;XDj(B
  \throw std::invalid_argument	$BE@BP$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class Iterator> void
ProjectiveMapping::initialize(Iterator first, Iterator last, bool refine)
{
  // $BE@Ns$N@55,2=(B
    const Normalize	xNormalize(make_const_first_iterator(first),
				   make_const_first_iterator(last)),
			yNormalize(make_const_second_iterator(first),
				   make_const_second_iterator(last));

  // $B=<J,$J8D?t$NE@BP$,$"$k$+!)(B
    const u_int		ndata = std::distance(first, last),
			xdim  = xNormalize.spaceDim() + 1,
			ydim  = yNormalize.spaceDim() + 1;
    if (ndata*(ydim - 1) < xdim*ydim - 1)	// _T$B$N%5%$%:$,L$Dj$J$N$G(B
						// ndataMin()$B$OL58z(B
	throw std::invalid_argument("ProjectiveMapping::initialize(): not enough input data!!");

  // $B%G!<%?9TNs$N7W;;(B
    Matrix<double>	A(xdim*ydim + ndata, xdim*ydim + ndata);
    int			n = xdim*ydim;
    for (Iterator iter = first; iter != last; ++iter)
    {
	const Vector<double>&	x = xNormalize.normalizeP(iter->first);
	const Vector<double>&	y = yNormalize.normalizeP(iter->second);

	A(0, 0, xdim, xdim) += x % x;
	for (int j = 0; j < ydim; ++j)
	    A[n](j*xdim, xdim) = -y[j] * x;
	A[n][n] = y * y;
	++n;
    }
    for (int j = 1; j < ydim; ++j)
	A(j*xdim, j*xdim, xdim, xdim) = A(0, 0, xdim, xdim);
    A.symmetrize();

  // $B%G!<%?9TNs$N:G>.8GM-CM$KBP1~$9$k8GM-%Y%/%H%k$+$iJQ499TNs$r7W;;$7!$(B
  // $B@55,2=$r%-%c%s%;%k$9$k!%(B
    Vector<double>		eval;
    const Matrix<double>&	Ut = A.eigen(eval);
    _T = yNormalize.Tinv()
       * Matrix<double>((double*)Ut[Ut.nrow()-1], ydim, xdim)
       * xNormalize.T();

  // $BJQ499TNs$,@5J}$J$i$P!$$=$N9TNs<0$,#1$K$J$k$h$&$K@55,2=$9$k!%(B
    if (_T.nrow() == _T.ncol())
    {
	double	det = _T.det();
	if (det > 0)
	    _T /= pow(det, 1.0/_T.nrow());
	else
	    _T /= -pow(-det, 1.0/_T.nrow());
    }

  // $BHs@~7?:GE,2=$r9T$&!%(B
    if (refine)
    {
	Cost					cost(first, last);
	ConstNormConstraint<Matrix<double> >	constraint(_T);
	minimizeSquare(cost, constraint, _T);
    }
}

//! $B<M1FJQ49$r5a$a$k$?$a$KI,MW$JE@BP$N:G>.8D?t$rJV$9!%(B
/*!
  $B8=:_@_Dj$5$l$F$$$kF~=PNO6u4V$N<!85$r$b$H$K7W;;$5$l$k!%(B
  \return	$BI,MW$JE@BP$N:G>.8D?t$9$J$o$AF~NO6u4V$N<!85(Bm$B!$=PNO6u4V$N<!85(Bn
		$B$KBP$7$F(B m + 1 + m/n
*/
inline u_int
ProjectiveMapping::ndataMin() const
{
    return inDim() + 1 + u_int(ceil(double(inDim())/double(outDim())));
}
    
//! $BM?$($i$l$?E@$K<M1FJQ49$rE,MQ$7$F$=$NHsF1<!:BI8$rJV$9!%(B
/*!
  \param x	$BE@$NHsF1<!:BI8!J(B#inDim()$B<!85!K$^$?$OF1<!:BI8!J(B#inDim()+1$B<!85!K(B
  \return	$B<M1FJQ49$5$l$?E@$NHsF1<!:BI8!J(B#outDim()$B<!85!K(B
*/
inline Vector<double>
ProjectiveMapping::operator ()(const Vector<double>& x)	const
{
    const Vector<double>&	y = mapP(x);
    return y(0, outDim()) / y[outDim()];
}

//! $BM?$($i$l$?E@$K<M1FJQ49$rE,MQ$7$F$=$NF1<!:BI8$rJV$9!%(B
/*!
  \param x	$BE@$NHsF1<!:BI8!J(B#inDim()$B<!85!K$^$?$OF1<!:BI8!J(B#inDim()+1$B<!85!K(B
  \return	$B<M1FJQ49$5$l$?E@$NF1<!:BI8!J(B#outDim()+1$B<!85!K(B
*/
inline Vector<double>
ProjectiveMapping::mapP(const Vector<double>& x) const
{
    if (x.dim() == inDim())
    {
	Vector<double>	xx(inDim()+1);
	xx(0, inDim()) = x;
	xx[inDim()] = 1.0;
	return _T * xx;
    }
    else
	return _T * x;
}

//! $BF~NOE@$K<M1FJQ49$rE,MQ$7$?E@$H=PNOE@$N5wN%$N(B2$B>h$rJV$9!%(B
/*!
  \param pair	$BF~NOE@$NHsF1<!:BI8!J(B#inDim()$B<!85!K$H=PNOE@$NHsF1<!:BI8(B
		$B!J(B#outDim()+1$B<!85!K$NBP(B
  \return	$BJQ49$5$l$?F~NOE@$H=PNOE@$N5wN%$N(B2$B>h(B
*/
template <class In, class Out> inline double
ProjectiveMapping::sqdist(const std::pair<In, Out>& pair) const
{
    return (*this)(pair.first).sqdist(pair.second);
}
    
//! $BF~NOE@$K<M1FJQ49$rE,MQ$7$?E@$H=PNOE@$N5wN%$rJV$9!%(B
/*!
  \param pair	$BF~NOE@$NHsF1<!:BI8!J(B#inDim()$B<!85!K$H=PNOE@$NHsF1<!:BI8(B
		$B!J(B#outDim()+1$B<!85!K$NBP(B
  \return	$BJQ49$5$l$?F~NOE@$H=PNOE@$N5wN%(B
*/
template <class In, class Out> inline double
ProjectiveMapping::dist(const std::pair<In, Out>& pair) const
{
    return sqrt(sqdist(pair));
}
    
template <class Iterator>
ProjectiveMapping::Cost::Cost(Iterator first, Iterator last)
    :_X(), _Y()
{
    const u_int	ndata = std::distance(first, last);
    _X.resize(ndata, first->first.dim() + 1);
    _Y.resize(ndata, first->second.dim());
    for (int n = 0; first != last; ++first)
    {
	_X[n](0, _X.ncol() - 1) = first->first;
	_X[n][_X.ncol() - 1]	= 1.0;
	_Y[n++]			= first->second;
    }
}
    
/************************************************************************
*  class AffineMapping							*
************************************************************************/
//! $B%"%U%#%sJQ49$r9T$&%/%i%9(B
/*!
  \f$\TUvec{A}{} \in \TUspace{R}{n\times m}\f$$B$H(B
  \f$\TUvec{b}{} \in \TUspace{R}{n}\f$$B$rMQ$$$F(Bm$B<!856u4V$NE@(B
  \f$\TUvec{x}{} \in \TUspace{R}{m}\f$$B$r(Bn$B<!856u4V$NE@(B
  \f$\TUvec{y}{} \simeq \TUvec{A}{}\TUvec{x}{} + \TUvec{b}{}
  \in \TUspace{R}{n}\f$$B$K<L$9!J(B\f$m \neq n\f$$B$G$b9=$o$J$$!K!%(B
*/
class AffineMapping : public ProjectiveMapping
{
  public:
  //! $BF~NO6u4V$H=PNO6u4V$N<!85$r;XDj$7$F%"%U%#%sJQ49%*%V%8%'%/%H$r@8@.$9$k!%(B
  /*!
    $B91EyJQ49$H$7$F=i4|2=$5$l$k!%(B
    \param inDim	$BF~NO6u4V$N<!85(B
    \param outDim	$B=PNO6u4V$N<!85(B
  */
    AffineMapping(u_int inDim=2, u_int outDim=2)
	:ProjectiveMapping(inDim, outDim)				{}

    AffineMapping(const Matrix<double>& T)				;
    template <class Iterator>
    AffineMapping(Iterator first, Iterator last)			;

    template <class Iterator>
    void	initialize(Iterator first, Iterator last)		;
    u_int	ndataMin()					const	;
    
  //! $B$3$N%"%U%#%sJQ49$NJQ7AItJ,$rI=8=$9$k9TNs$rJV$9!%(B
  /*! 
    \return	#outDim() x #inDim()$B9TNs(B
  */
    const Matrix<double>
			A()	const	{return _T(0, 0, outDim(), inDim());}
    
    Vector<double>	b()	const	;
};

//! $BJQ499TNs$r;XDj$7$F%"%U%#%sJQ49%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  $BJQ499TNs$N2<C<9T$O6/@)E*$K(B 0,0,...,0,1 $B$K@_Dj$5$l$k!%(B
  \param T			(m+1)x(n+1)$B9TNs!J(Bm, n$B$OF~NO!?=PNO6u4V$N<!85!K(B
*/
inline
AffineMapping::AffineMapping(const Matrix<double>& T)
    :ProjectiveMapping(T)
{
    _T[outDim()]	  = 0.0;
    _T[outDim()][inDim()] = 1.0;
}

//! $BM?$($i$l$?E@BPNs$NHsF1<!:BI8$+$i%"%U%#%sJQ49%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  \param first			$BE@BPNs$N@hF,$r<($9H?I|;R(B
  \param last			$BE@BPNs$NKvHx$r<($9H?I|;R(B
  \throw std::invalid_argument	$BE@BP$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class Iterator> inline
AffineMapping::AffineMapping(Iterator first, Iterator last)
{
    initialize(first, last);
}

//! $BM?$($i$l$?E@BPNs$NHsF1<!:BI8$+$i%"%U%#%sJQ49$r7W;;$9$k!%(B
/*!
  \param first			$BE@BPNs$N@hF,$r<($9H?I|;R(B
  \param last			$BE@BPNs$NKvHx$r<($9H?I|;R(B
  \throw std::invalid_argument	$BE@BP$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class Iterator> void
AffineMapping::initialize(Iterator first, Iterator last)
{
  // $B=<J,$J8D?t$NE@BP$,$"$k$+!)(B
    const u_int	ndata = std::distance(first, last);
    if (ndata == 0)		// first$B$,M-8z$+!)(B
	throw std::invalid_argument("AffineMapping::initialize(): 0-length input data!!");
    const u_int	xdim = first->first.dim();
    if (ndata < xdim + 1)	// _T$B$N%5%$%:$,L$Dj$J$N$G(BndataMin()$B$OL58z(B
	throw std::invalid_argument("AffineMapping::initialize(): not enough input data!!");

  // $B%G!<%?9TNs$N7W;;(B
    const u_int		ydim = first->second.dim(), xydim2 = xdim*ydim;
    Matrix<double>	M(xdim, xdim);
    Vector<double>	c(xdim), v(xydim2 + ydim);
    for (; first != last; ++first)
    {
	const Vector<double>&	x = first->first;
	const Vector<double>&	y = first->second;

	M += x % x;
	c += x;
	for (int j = 0; j < ydim; ++j)
	    v(j*xdim, xdim) += y[j]*x;
	v(xydim2, ydim) += y;
    }
    Matrix<double>	W(xydim2 + ydim, xydim2 + ydim);
    for (int j = 0; j < ydim; ++j)
    {
	W(j*xdim, j*xdim, xdim, xdim) = M;
	W[xydim2 + j](j*xdim, xdim)   = c;
	W[xydim2 + j][xydim2 + j]     = ndata;
    }
    W.symmetrize();

  // W*u = v$B$r2r$$$FJQ49%Q%i%a!<%?$r5a$a$k!%(B
    v.solve(W);

  // $BJQ499TNs$r%;%C%H$9$k!%(B
    _T.resize(ydim + 1, xdim + 1);
    _T(0, 0, ydim, xdim) = Matrix<double>((double*)v, ydim, xdim);
    for (int j = 0; j < ydim; ++j)
	 _T[j][xdim] = v[xydim2 + j];
    _T[ydim][xdim] = 1.0;
}

//! $B%"%U%#%sJQ49$r5a$a$k$?$a$KI,MW$JE@BP$N:G>.8D?t$rJV$9!%(B
/*!
  $B8=:_@_Dj$5$l$F$$$kF~=PNO6u4V$N<!85$r$b$H$K7W;;$5$l$k!%(B
  \return	$BI,MW$JE@BP$N:G>.8D?t$9$J$o$AF~NO6u4V$N<!85(Bm$B$KBP$7$F(B m + 1
*/
inline u_int
AffineMapping::ndataMin() const
{
    return inDim() + 1;
}
    
/************************************************************************
*  class HyperPlane							*
************************************************************************/
//! d$B<!85<M1F6u4VCf$ND6J?LL$rI=8=$9$k%/%i%9(B
/*!
  d$B<!85<M1F6u4V$NE@(J\(Bf$(J\(BTUud{x}{} (J\(Bin (J\(BTUspace{R}{d+1}(J\(Bf$$B$KBP$7$F(B
  (J\(Bf$(J\(BTUtud{p}{}(J\(BTUud{x}{} = 0,(J~\(BTUud{p}{} (J\(Bin (J\(BTUspace{R}{d+1}(J\(Bf$
  $B$K$h$C$FI=$5$l$k!%(B
*/
class HyperPlane
{
  public:
    HyperPlane(u_int d=2)						;

  //! $BF1<!:BI8%Y%/%H%k$r;XDj$7$FD6J?LL%*%V%8%'%/%H$r@8@.$9$k!%(B
  /*!
    (J\(Bparam p	(d+1)$B<!85%Y%/%H%k!J(Bd$B$O$b$H$N<M1F6h4V$N<!85!K(B
  */
    HyperPlane(const Vector<double>& p)	:_p(p)				{}

    template <class Iterator>
    HyperPlane(Iterator first, Iterator last)				;

    template <class Iterator>
    void	initialize(Iterator first, Iterator last)		;

  //! $B$3$ND6J?LL$,B8:_$9$k<M1F6u4V$N<!85$rJV$9!%(B
  /*! 
    (J\(Breturn	$B<M1F6u4V$N<!85(B($BF1<!:BI8$N%Y%/%H%k$H$7$F$N<!85$O(B#spaceDim()+1)
  */
    u_int	spaceDim()			const	{return _p.dim()-1;}

  //! $BD6J?LL$r5a$a$k$?$a$KI,MW$JE@$N:G>.8D?t$rJV$9!%(B
  /*!
    $B8=:_@_Dj$5$l$F$$$k<M1F6u4V$N<!85$r$b$H$K7W;;$5$l$k!%(B
    (J\(Breturn	$BI,MW$JE@$N:G>.8D?t$9$J$o$AF~NO6u4V$N<!85(B#spaceDim()
  */
    u_int	ndataMin()			const	{return spaceDim();}

  //! $B$3$ND6J?LL$rI=8=$9$kF1<!:BI8%Y%/%H%k$rJV$9!%(B
  /*!
    (J\(Breturn	(#spaceDim()+1)$B<!85%Y%/%H%k(B
  */
    const Vector<double>&
		p()				const	{return _p;}

    double	sqdist(const Vector<double>& x)	const	;
    double	dist(const Vector<double>& x)	const	;
    
  private:
    Vector<double>	_p;	//!> $BD6J?LL$rI=8=$9$k%Y%/%H%k(B
};

//! $B6u4V$N<!85$r;XDj$7$FD6J?LL%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  $BL58B1sD6J?LL(B([0, 0,..., 0, 1])$B$K=i4|2=$5$l$k!%(B
  (J\(Bparam d	$B$3$ND6J?LL$,B8:_$9$k<M1F6u4V$N<!85(B
*/
inline
HyperPlane::HyperPlane(u_int d)
    :_p(d + 1)
{
    _p[d] = 1.0;
}
    
//! $BM?$($i$l$?E@Ns$NHsF1<!:BI8$KEv$F$O$a$i$l$?D6J?LL%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  (J\(Bparam first			$BE@Ns$N@hF,$r<($9H?I|;R(B
  (J\(Bparam last			$BE@Ns$NKvHx$r<($9H?I|;R(B
  (J\(Bthrow std::invalid_argument	$BE@$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class Iterator> inline
HyperPlane::HyperPlane(Iterator first, Iterator last)
{
    initialize(first, last);
}

//! $BM?$($i$l$?E@Ns$NHsF1<!:BI8$KD6J?LL$rEv$F$O$a$k!%(B
/*!
  (J\(Bparam first			$BE@Ns$N@hF,$r<($9H?I|;R(B
  (J\(Bparam last			$BE@Ns$NKvHx$r<($9H?I|;R(B
  (J\(Bthrow std::invalid_argument	$BE@$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class Iterator> void
HyperPlane::initialize(Iterator first, Iterator last)
{
  // $BE@Ns$N@55,2=(B
    const Normalize	normalize(first, last);

  // $B=<J,$J8D?t$NE@$,$"$k$+!)(B
    const u_int		ndata = std::distance(first, last),
			d     = normalize.spaceDim();
    if (ndata < d)	// _p$B$N%5%$%:$,L$Dj$J$N$G(BndataMin()$B$OL58z(B
	throw std::invalid_argument("Hyperplane::initialize(): not enough input data!!");

  // $B%G!<%?9TNs$N7W;;(B
    Matrix<double>	A(d, d);
    while (first != last)
    {
	const Vector<double>&	x = normalize(*first++);
	A += x % x;
    }

  // $B%G!<%?9TNs$N:G>.8GM-CM$KBP1~$9$k8GM-%Y%/%H%k$+$iK!@~%Y%/%H%k$r7W;;$7!$(B
  // $B$5$i$KE@Ns$N=E?4$h$j86E@$+$i$N5wN%$r7W;;$9$k!%(B
    Vector<double>		eval;
    const Matrix<double>&	Ut = A.eigen(eval);
    _p.resize(d+1);
    _p(0, d) = Ut[Ut.nrow()-1];
    _p[d] = -(_p(0, d)*normalize.centroid());
    if (_p[d] > 0.0)
	_p *= -1.0;
}

//! $BM?$($i$l$?E@$HD6J?LL$N5wN%$N(B2$B>h$rJV$9!%(B
/*!
  (J\(Bparam x	$BE@$NHsF1<!:BI8!J(B#spaceDim()$B<!85!K$^$?$OF1<!:BI8(B
		$B!J(B#spaceDim()+1$B<!85!K(B
  (J\(Breturn	$BE@$HD6J?LL$N5wN%$N(B2$B>h(B
*/
inline double
HyperPlane::sqdist(const Vector<double>& x) const
{
    const double	d = dist(x);
    return d*d;
}

//! $BM?$($i$l$?E@$HD6J?LL$N5wN%$rJV$9!%(B
/*!
  (J\(Bparam x			$BE@$NHsF1<!:BI8!J(B#spaceDim()$B<!85!K$^$?$O(B
				$BF1<!:BI8!J(B#spaceDim()+1$B<!85!K(B
  (J\(Breturn			$BE@$HD6J?LL$N5wN%!JHsIi!K(B
  (J\(Bthrow std::invalid_argument	$BE@$N%Y%/%H%k$H$7$F$N<!85$,(B#spaceDim()$B!$(B
				#spaceDim()+1$B$N$$$:$l$G$b$J$$>l9g!$$b$7$/$O(B
				$B$3$NE@$,L58B1sE@$G$"$k>l9g$KAw=P!%(B
*/
inline double
HyperPlane::dist(const Vector<double>& x) const
{
    if (x.dim() == spaceDim())
    {
	Vector<double>	xx(spaceDim()+1);
	xx(0, spaceDim()) = x;
	xx[spaceDim()] = 1.0;
	return fabs(_p * xx);
    }
    else if (x.dim() == spaceDim() + 1)
    {
	if (x[spaceDim()] == 0.0)
	    throw std::invalid_argument("HyperPlane::dist(): point at infinitiy!!");
	return fabs((_p * x)/x[spaceDim()]);
    }
    else
	throw std::invalid_argument("HyperPlane::dist(): dimension mismatch!!");

    return 0;
}

/************************************************************************
*  class CameraBase							*
************************************************************************/
//! $B$9$Y$F$NF);kEj1F%+%a%i$N4pDl$H$J$k%/%i%9(B
class CameraBase
{
  public:
  //! $B%+%a%i$NFbIt%Q%i%a!<%?$rI=$9%/%i%9(B
    class Intrinsic
    {
      public:
	virtual ~Intrinsic()						;
	
      // various operations.
	virtual Point2<double>
	    operator ()(const Point2<double>& xc)		const	;
	virtual Point2<double>
	    xd(const Point2<double>& xc)			const	;
	virtual Matrix<double>
	    jacobianK(const Point2<double>& xc)			const	;
	virtual Matrix<double>
	    jacobianXC(const Point2<double>& xc)		const	;
	virtual Point2<double>
	    xc(const Point2<double>& u)				const	;

      // calibration matrices.    
	virtual Matrix<double>	K()				const	;
	virtual Matrix<double>	Kt()				const	;
	virtual Matrix<double>	Kinv()				const	;
	virtual Matrix<double>	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		k()				const	;
	virtual Point2<double>	principal()			const	;
	virtual double		aspect()			const	;
	virtual double		skew()				const	;
	virtual double		d1()				const	;
	virtual double		d2()				const	;
	virtual Intrinsic&	setFocalLength(double k)		;
	virtual Intrinsic&	setPrincipal(double u0, double v0)	;
	virtual Intrinsic&	setAspect(double aspect)		;
	virtual Intrinsic&	setSkew(double skew)			;
	virtual Intrinsic&	setIntrinsic(const Matrix<double>& K)	;
	virtual Intrinsic&	setDistortion(double d1, double d2)	;

      // parameter updating functions.
	virtual Intrinsic&	update(const Vector<double>& dp)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;
    };
    
  public:
  //! $B0LCV$r86E@$K!$;Q@*$rC10L9TNs$K%;%C%H$7$F=i4|2=(B
    CameraBase()
	:_t(3), _Rt(3, 3)	{_Rt[0][0] = _Rt[1][1] = _Rt[2][2] = 1.0;}
  //! $B0LCV$H;Q@*$rC10L9TNs$K%;%C%H$7$F=i4|2=(B
  /*!
    \param t	$B%+%a%i0LCV$rI=$9(B3$B<!85%Y%/%H%k!%(B
    \param Rt	$B%+%a%i;Q@*$rI=$9(B3x3$B2sE>9TNs!%(B
  */
    CameraBase(const Vector<double>& t, const Matrix<double>& Rt)
	:_t(t), _Rt(Rt)							{}
    virtual ~CameraBase()						;
    
  // various operations in canonical coordinates.
    Point2<double>	xc(const Vector<double>& x)		const	;
    Point2<double>	xc(const Point2<double>& u)		const	;
    Matrix<double>	Pc()					const	;
    Matrix<double>	jacobianPc(const Vector<double>& x)	const	;
    Matrix<double>	jacobianXc(const Vector<double>& x)	const	;

  // various oeprations in image coordinates.
    Point2<double>	operator ()(const Vector<double>& x)	const	;
    Matrix<double>	P()					const	;
    Matrix<double>	jacobianP(const Vector<double>& x)	const	;
    Matrix<double>	jacobianFCC(const Vector<double>& x)	const	;
    Matrix<double>	jacobianX(const Vector<double>& x)	const	;
    Matrix<double>	jacobianK(const Vector<double>& x)	const	;
    Matrix<double>	jacobianXC(const Vector<double>& x)	const	;
    virtual CameraBase& setProjection(const Matrix<double>& P)		=0;

  // parameter updating functions.
    void		update(const Vector<double>& dp)		;
    void		updateFCC(const Vector<double>& dp)		;
    void		updateIntrinsic(const Vector<double>& dp)	;
    
  // calibration matrices.
    Matrix<double>	K()		const	{return intrinsic().K();}
    Matrix<double>	Kt()		const	{return intrinsic().Kt();}
    Matrix<double>	Kinv()		const	{return intrinsic().Kinv();}
    Matrix<double>	Ktinv()		const	{return intrinsic().Ktinv();}

  // extrinsic parameters.
    const Vector<double>&	t()	const	{return _t;}
    const Matrix<double>&	Rt()	const	{return _Rt;}
    CameraBase&		setTranslation(const Vector<double>& t)	;
    CameraBase&		setRotation(const Matrix<double>& Rt)	;

  // intrinsic parameters.
    virtual const Intrinsic&
			intrinsic()	const	= 0;
    virtual Intrinsic&	intrinsic()		= 0;
    u_int		dofIntrinsic()	const	{return intrinsic().dof();}
    double		k()		const	{return intrinsic().k();}
    Point2<double>	principal()	const	{return intrinsic().principal();}
    double		aspect()	const	{return intrinsic().aspect();}
    double		skew()		const	{return intrinsic().skew();}
    double		d1()		const	{return intrinsic().d1();}
    double		d2()		const	{return intrinsic().d2();}
    CameraBase&		setFocalLength(double k)		;
    CameraBase&		setPrincipal(double u0, double v0)	;
    CameraBase&		setAspect(double aspect)		;
    CameraBase&		setSkew(double skew)			;
    CameraBase&		setIntrinsic(const Matrix<double>& K)	;
    CameraBase&		setDistortion(double d1, double d2)	;
    
  // I/O functions.
    std::istream&	get(std::istream& in)			;
    std::ostream&	put(std::ostream& out)		const	;

  private:
    Vector<double>	_t;			// camera center.
    Matrix<double>	_Rt;			// camera orientation.
};

//! 3$B<!856u4VCf$NE@$NA|$N(Bcanonical$B%+%a%i:BI87O$K$*$1$k0LCV$r5a$a$k(B
/*!
  $BA|$O0J2<$N$h$&$K7W;;$5$l$k!%(B
  \f[
    \TUbeginarray{c} x_c \\ y_c \TUendarray = 
    \frac{1}{\TUtvec{r}{z}(\TUvec{x}{} - \TUvec{t}{})}
    \TUbeginarray{c}
      \TUtvec{r}{x}(\TUvec{x}{} - \TUvec{t}{}) \\
      \TUtvec{r}{y}(\TUvec{x}{} - \TUvec{t}{})
    \TUendarray
  \f]
  \param x	3$B<!856u4VCf$NE@$rI=$9(B3$B<!85%Y%/%H%k!%(B
  \return	x$B$NA|$N(Bcanonical$B%+%a%i:BI87O$K$*$1$k0LCV!%(B
*/
inline Point2<double>
CameraBase::xc(const Vector<double>& x) const
{
    const Vector<double>&	xx = _Rt * (x - _t);
    return Point2<double>(xx[0] / xx[2], xx[1] / xx[2]);
}

//! $B2hA|:BI8$K$*$1$kE@$N(B2$B<!850LCV$r(Bcanonical$B%+%a%i:BI87O$KD>$9(B
/*!
  \param u	$B2hA|:BI87O$K$*$1$kE@$N(B2$B<!850LCV!%(B
  \return	canonical$B%+%a%i:BI87O$K$*$1$k(Bu$B$N(B2$B<!850LCV!%(B
*/
inline Point2<double>
CameraBase::xc(const Point2<double>& u) const
{
    return intrinsic().xc(u);
}

//! 3$B<!856u4VCf$NE@$NA|$N2hA|:BI87O$K$*$1$k0LCV$r5a$a$k(B
/*!
  \param x	3$B<!856u4VCf$NE@$rI=$9(B3$B<!85%Y%/%H%k!%(B
  \return	x$B$NA|$N2hA|:BI87O$K$*$1$k0LCV!%(B
*/
inline Point2<double>
CameraBase::operator ()(const Vector<double>& x) const
{
    return intrinsic()(xc(x));
}

//! 3$B<!85%f!<%/%j%C%I6u4V$+$i2hA|J?LL$X$NEj1F9TNs$r5a$a$k(B
/*!
  \return	$BEj1F9TNs!%(B
*/
inline Matrix<double>
CameraBase::P() const
{
    return K() * Pc();
}

//! $B0LCV$r8GDj$7$?$H$-$NFbIt(B/$B30It%Q%i%a!<%?$K4X$9$k(BJacobian$B$r5a$a$k(B
/*!
  \return	
*/
inline Matrix<double>
CameraBase::jacobianFCC(const Vector<double>& x) const
{
    const Matrix<double>&	J = jacobianP(x);
    return Matrix<double>(J, 0, 3, J.nrow(), J.ncol() - 3);
}

inline Matrix<double>
CameraBase::jacobianX(const Vector<double>& x) const
{
    return intrinsic().jacobianXC(xc(x)) * jacobianXc(x);
}

inline Matrix<double>
CameraBase::jacobianK(const Vector<double>& x) const
{
    return intrinsic().jacobianK(xc(x));
}

inline Matrix<double>
CameraBase::jacobianXC(const Vector<double>& x) const
{
    return intrinsic().jacobianXC(xc(x));
}

inline void
CameraBase::updateIntrinsic(const Vector<double>& dp)
{
    intrinsic().update(dp);			// update intrinsic parameters.
}

inline void
CameraBase::updateFCC(const Vector<double>& dp)
{
    _Rt *= Matrix<double>::Rt(dp(0, 3));	// update rotation.
    updateIntrinsic(dp(3, dp.dim() - 3));	// update intrinsic parameters.
}

inline void
CameraBase::update(const Vector<double>& dp)
{
    _t -= dp(0, 3);				// update translation.
    updateFCC(dp(3, dp.dim() - 3));		// update other prameters.
}

inline CameraBase&
CameraBase::setTranslation(const Vector<double>& t)
{
    _t = t;
    return *this;
}

inline CameraBase&
CameraBase::setRotation(const Matrix<double>& Rt)
{
    _Rt = Rt;
    return *this;
}

inline CameraBase&
CameraBase::setFocalLength(double k)
{
    intrinsic().setFocalLength(k);
    return *this;
}

inline CameraBase&
CameraBase::setPrincipal(double u0, double v0)
{
    intrinsic().setPrincipal(u0, v0);
    return *this;
}

inline CameraBase&
CameraBase::setAspect(double aspect)
{
    intrinsic().setAspect(aspect);
    return *this;
}

inline CameraBase&
CameraBase::setSkew(double skew)
{
    intrinsic().setSkew(skew);
    return *this;
}

inline CameraBase&
CameraBase::setIntrinsic(const Matrix<double>& K)
{
    intrinsic().setIntrinsic(K);
    return *this;
}

inline CameraBase&
CameraBase::setDistortion(double d1, double d2)
{
    intrinsic().setDistortion(d1, d2);
    return *this;
}

inline std::istream&
operator >>(std::istream& in, CameraBase& camera)
{
    return camera.get(in);
}

inline std::ostream&
operator <<(std::ostream& out, const CameraBase& camera)
{
    return camera.put(out);
}

inline std::istream&
operator >>(std::istream& in, CameraBase::Intrinsic& intrinsic)
{
    return intrinsic.get(in);
}

inline std::ostream&
operator <<(std::ostream& out, const CameraBase::Intrinsic& intrinsic)
{
    return intrinsic.put(out);
}

/************************************************************************
*  class CanonicalCamera						*
************************************************************************/
class CanonicalCamera : public CameraBase
{
  public:
    CanonicalCamera()	:CameraBase(), _intrinsic()	{}
    CanonicalCamera(const Vector<double>& t, const Matrix<double>& Rt)
	:CameraBase(t, Rt), _intrinsic()		{}
    CanonicalCamera(const Matrix<double>& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&	setProjection(const Matrix<double>& P)		;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;

  private:
    Intrinsic	_intrinsic;
};

/************************************************************************
*  class CameraWithFocalLength						*
************************************************************************/
class CameraWithFocalLength : public CameraBase
{
  public:
    class Intrinsic : public CanonicalCamera::Intrinsic
    {
      public:
	Intrinsic(double k=1.0)	:_k(k)					{}

      // various operations.
	virtual Point2<double>
	    operator ()(const Point2<double>& xc)		const	;
	virtual Matrix<double>
	    jacobianK(const Point2<double>& xc)			const	;
	virtual Matrix<double>
	    jacobianXC(const Point2<double>& xc)		const	;
	virtual Point2<double>
	    xc(const Point2<double>& u)				const	;

      // calibration matrices.
	virtual Matrix<double>		K()			const	;
	virtual Matrix<double>		Kt()			const	;
	virtual Matrix<double>		Kinv()			const	;
	virtual Matrix<double>		Ktinv()			const	;

      // intrinsic parameters.
	virtual u_int			dof()			const	;
	virtual double			k()			const	;
	virtual	CameraBase::Intrinsic&	setFocalLength(double k)	;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
	    update(const Vector<double>& dp)				;

      // I/O functions.
	virtual std::istream&		get(std::istream& in)		;
	virtual std::ostream&		put(std::ostream& out)	const	;

      private:
	double	_k;
    };
    
  public:
    CameraWithFocalLength()	:CameraBase(), _intrinsic()	{}
    CameraWithFocalLength(const Vector<double>& t,
			  const Matrix<double>& Rt,
			  double		k=1.0)
	:CameraBase(t, Rt), _intrinsic(k)			{}
    CameraWithFocalLength(const Matrix<double>& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&		setProjection(const Matrix<double>& P)	;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};

/************************************************************************
*  class CameraWithEuclideanImagePlane					*
************************************************************************/
class CameraWithEuclideanImagePlane : public CameraBase
{
  public:
    class Intrinsic : public CameraWithFocalLength::Intrinsic
    {
      public:
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0)
	    :CameraWithFocalLength::Intrinsic(k), _principal(u0, v0)	{}
	Intrinsic(const CameraWithFocalLength::Intrinsic& intrinsic)
	    :CameraWithFocalLength::Intrinsic(intrinsic),
	     _principal(0.0, 0.0)					{}
	
      // various operations.	
	virtual Point2<double>
	    operator ()(const Point2<double>& xc)		const	;
	virtual Matrix<double>
	    jacobianK(const Point2<double>& xc)			const	;
	virtual Point2<double>
	    xc(const Point2<double>& u)				const	;
    
      // calibration matrices.	
	virtual Matrix<double>		K()			const	;
	virtual Matrix<double>		Kt()			const	;
	virtual Matrix<double>		Kinv()			const	;
	virtual Matrix<double>		Ktinv()			const	;

      // intrinsic parameters.
	virtual u_int			dof()			const	;
	virtual Point2<double>		principal()		const	;
	virtual CameraBase::Intrinsic&	setPrincipal(double u0,
						     double v0)		;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&  update(const Vector<double>& dp);

      // I/O functions.
	virtual std::istream&		get(std::istream& in)		;
	virtual std::ostream&		put(std::ostream& out)	const	;

      private:
	Point2<double>	_principal;
    };
    
  public:
    CameraWithEuclideanImagePlane()	:CameraBase(), _intrinsic()	{}
    CameraWithEuclideanImagePlane(const Vector<double>& t,
				  const Matrix<double>& Rt,
				  double		k=1.0,
				  double		u0=0,
				  double		v0=0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0)			{}
    CameraWithEuclideanImagePlane(const Matrix<double>& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&	setProjection(const Matrix<double>& P)		;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};
    
/************************************************************************
*  class Camera								*
************************************************************************/
class Camera : public CameraBase
{
  public:
    class Intrinsic : public CameraWithEuclideanImagePlane::Intrinsic
    {
      public:
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0,
		  double aspect=1.0, double skew=0.0)
	    :CameraWithEuclideanImagePlane::Intrinsic(k, u0, v0),
	     _k00(aspect * k), _k01(skew * k)				{}
	Intrinsic(const CameraWithEuclideanImagePlane::Intrinsic& intrinsic)
	    :CameraWithEuclideanImagePlane::Intrinsic(intrinsic),
	     _k00(k()), _k01(0.0)					{}
	Intrinsic(const Matrix<double>& K)
	    :CameraWithEuclideanImagePlane::Intrinsic(),
	     _k00(k()), _k01(0.0)			{setIntrinsic(K);}
	
      // various operations.
	virtual Point2<double>
	    operator ()(const Point2<double>& xc)		const	;
	virtual Matrix<double>
	    jacobianK(const Point2<double>& xc)			const	;
	virtual Matrix<double>
	    jacobianXC(const Point2<double>& xc)		const	;
	virtual Point2<double>
	    xc(const Point2<double>& u)				const	;

      // calibration matrices.
	virtual Matrix<double>	K()				const	;
	virtual Matrix<double>	Kt()				const	;
	virtual Matrix<double>	Kinv()				const	;
	virtual Matrix<double>	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		aspect()			const	;
	virtual double		skew()				const	;
	virtual	CameraBase::Intrinsic&
				setFocalLength(double k)		;
	virtual CameraBase::Intrinsic&
				setAspect(double aspect)		;
	virtual CameraBase::Intrinsic&
				setSkew(double skew)			;
	virtual CameraBase::Intrinsic&
				setIntrinsic(const Matrix<double>& K)	;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;
    
      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      protected:
		double		k00()			const	{return _k00;}
		double		k01()			const	{return _k01;}
	
      private:
	double	_k00, _k01;
    };
    
  public:
    Camera()	:CameraBase(), _intrinsic()			{}
    Camera(const Vector<double>& t,
	   const Matrix<double>& Rt,
	   double		 k=1.0,
	   double		 u0=0,
	   double		 v0=0,
	   double		 aspect=1.0,
	   double		 skew=0.0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0, aspect, skew)	{}
    Camera(const Matrix<double>& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&	setProjection(const Matrix<double>& P);
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};

/************************************************************************
*  class CameraWithDistortion						*
************************************************************************/
class CameraWithDistortion : public CameraBase
{
  public:
    class Intrinsic : public Camera::Intrinsic
    {
      public:
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0,
		  double aspect=1.0, double skew=0.0,
		  double d1=0.0, double d2=0.0)
	    :Camera::Intrinsic(k, u0, v0, aspect, skew),
	     _d1(d1), _d2(d2)						{}
	Intrinsic(const Camera::Intrinsic& intrinsic)
	    :Camera::Intrinsic(intrinsic), _d1(0.0), _d2(0.0)		{}
	Intrinsic(const Matrix<double>& K)
	    :Camera::Intrinsic(), _d1(0.0), _d2(0.0)	{setIntrinsic(K);}
	
      // various operations.
	virtual Point2<double>
	    operator ()(const Point2<double>& xc)		const	;
	virtual Point2<double>
	    xd(const Point2<double>& xc)			const	;
	virtual Matrix<double>
	    jacobianXC(const Point2<double>& xc)		const	;
	virtual Matrix<double>
	    jacobianK(const Point2<double>& xc)			const	;
	virtual CameraBase::Intrinsic&
	    update(const Vector<double>& dp)				;
	virtual Point2<double>
	    xc(const Point2<double>& u)				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		d1()				const	;
	virtual double		d2()				const	;
	virtual CameraBase::Intrinsic&	
				setDistortion(double d1, double d2)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      private:
	double	_d1, _d2;
    };
    
  public:
    CameraWithDistortion()	:CameraBase(), _intrinsic()		{}
    CameraWithDistortion(const Vector<double>& t,
			 const Matrix<double>& Rt,
			 double		       k=1.0,
			 double		       u0=0,
			 double		       v0=0,
			 double		       aspect=1.0,
			 double		       skew=0.0,
			 double		       d1=0.0,
			 double		       d2=0.0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0, aspect, skew, d1, d2)	{}
    CameraWithDistortion(const Matrix<double>& P,
			 double d1=0.0, double d2=0.0)			;

    virtual CameraBase&		setProjection(const Matrix<double>& P)	;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};
 
inline
CameraWithDistortion::CameraWithDistortion(const Matrix<double>& P,
					   double d1, double d2)
    :CameraBase(), _intrinsic()
{
    setProjection(P);
    setDistortion(d1,d2);
}

}

#endif	/* !__TUGeometryPP_h */
