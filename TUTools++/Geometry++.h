/*
 *  $Id: Geometry++.h,v 1.20 2007-10-23 02:27:06 ueshiba Exp $
 */
#ifndef __TUGeometryPP_h
#define __TUGeometryPP_h

#include "TU/utility.h"
#include "TU/Minimize++.h"

namespace TU
{
/************************************************************************
*  class Point2<T>							*
************************************************************************/
template <class T>
class Point2 : public Vector<T, FixedSizedBuf<T, 2> >
{
  private:
    typedef Vector<T, FixedSizedBuf<T, 2> >	array_type;
    
  public:
    Point2(T u=0, T v=0)						;
    template <class T2, class B2>
    Point2(const Vector<T2, B2>& v) :array_type(v)			{}
    template <class T2, class B2>
    Point2&	operator =(const Vector<T2, B2>& v)
		{
		    array_type::operator =(v);
		    return *this;
		}
    Vector<T, FixedSizedBuf<T, 3> >
		hom()						const	;
    Point2	neighbor(int)					const	;
    Point2&	move(int)						;
    int		adj(const Point2&)				const	;
    int		dir(const Point2&)				const	;
    int		angle(const Point2&, const Point2&)		const	;
};

template <class T> inline
Point2<T>::Point2(T u, T v)
    :array_type()
{
    (*this)[0] = u;
    (*this)[1] = v;
}

template <class T> inline Point2<T>
Point2<T>::neighbor(int dir) const
{
    return Point2(*this).move(dir);
}

template <class T> inline Vector<T, FixedSizedBuf<T, 3> >
Point2<T>::hom() const
{
    Vector<T, FixedSizedBuf<T, 3> >	v;
    v[0] = (*this)[0];
    v[1] = (*this)[1];
    v[2] = 1;
    return v;
}

typedef Point2<short>					Point2s;
typedef Point2<int>					Point2i;
typedef Point2<float>					Point2f;
typedef Point2<double>					Point2d;

/************************************************************************
*  class Point3<T>							*
************************************************************************/
template <class T>
class Point3 : public Vector<T, FixedSizedBuf<T, 3> >
{
  private:
    typedef Vector<T, FixedSizedBuf<T, 3> >	array_type;
    
  public:
    Point3(T x=0, T y=0, T z=0)						;
    template <class T2, class B2>
    Point3(const Vector<T2, B2>& v) :array_type(v)			{}
    template <class T2, class B2>
    Point3&	operator =(const Vector<T2, B2>& v)
		{
		    array_type::operator =(v);
		    return *this;
		}
    Vector<T, FixedSizedBuf<T, 4> >
		hom()						const	;
};

template <class T> inline
Point3<T>::Point3(T x, T y, T z)
    :array_type()
{
    (*this)[0] = x;
    (*this)[1] = y;
    (*this)[2] = z;
}

template <class T> inline Vector<T, FixedSizedBuf<T, 4> >
Point3<T>::hom() const
{
    Vector<T, FixedSizedBuf<T, 4> >	v;
    v[0] = (*this)[0];
    v[1] = (*this)[1];
    v[2] = (*this)[2];
    v[3] = 1;
    return v;
}

typedef Point3<short>					Point3s;
typedef Point3<int>					Point3i;
typedef Point3<float>					Point3f;
typedef Point3<double>					Point3d;

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
    typedef double	ET;

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

    template <class S, class B>
    Vector<double>	operator ()(const Vector<S, B>& x)	const	;
    template <class S, class B>
    Vector<double>	mapP(const Vector<S, B>& x)		const	;
    template <class S, class B>
    Matrix<double>	jacobian(const Vector<S, B>& x)	const	;

    template <class In, class Out>
    double		sqdist(const std::pair<In, Out>& pair)	const	;
    template <class In, class Out>
    double		dist(const std::pair<In, Out>& pair)	const	;
    double		square()				const	;
			operator const Vector<double>()		const	;
    u_int		dof()					const	;
    void		update(const Vector<double>& dt)		;
    
  protected:
    Matrix<double>	_T;			//!< $B<M1FJQ49$rI=8=$9$k9TNs(B

  protected:
  //! $B<M1FJQ499TNs$N:GL`?dDj$N$?$a$N%3%9%H4X?t(B
    template <class T, class Iterator>
    class Cost
    {
      public:
	typedef double	ET;
	typedef T	AT;

	Cost(Iterator first, Iterator last)				;

	Vector<ET>	operator ()(const AT& map)		const	;
	Matrix<ET>	jacobian(const AT& map)			const	;
	static void	update(AT& map, const Vector<ET>& dm)		;
	u_int		npoints()				const	;

      private:
	const Iterator	_first, _last;
	const u_int	_npoints;
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
    const u_int		ndata = std::distance(first, last);
    const u_int		xdim1 = xNormalize.spaceDim() + 1,
			ydim  = yNormalize.spaceDim();
    if (ndata*ydim < xdim1*(ydim + 1) - 1)	// _T$B$N%5%$%:$,L$Dj$J$N$G(B
						// ndataMin()$B$OL58z(B
	throw std::invalid_argument("ProjectiveMapping::initialize(): not enough input data!!");

  // $B%G!<%?9TNs$N7W;;(B
    Matrix<double>	A(xdim1*(ydim + 1), xdim1*(ydim + 1));
    for (Iterator iter = first; iter != last; ++iter)
    {
	const Vector<double>&	x  = xNormalize.normalizeP(iter->first);
	const Vector<double>&	y  = yNormalize(iter->second);
	const Matrix<double>&	xx = x % x;
	A(0, 0, xdim1, xdim1) += xx;
	for (int j = 0; j < ydim; ++j)
	    A(ydim*xdim1, j*xdim1, xdim1, xdim1) -= y[j] * xx;
	A(ydim*xdim1, ydim*xdim1, xdim1, xdim1) += (y*y) * xx;
    }
    for (int j = 1; j < ydim; ++j)
	A(j*xdim1, j*xdim1, xdim1, xdim1) = A(0, 0, xdim1, xdim1);
    A.symmetrize();

  // $B%G!<%?9TNs$N:G>.8GM-CM$KBP1~$9$k8GM-%Y%/%H%k$+$iJQ499TNs$r7W;;$7!$(B
  // $B@55,2=$r%-%c%s%;%k$9$k!%(B
    Vector<double>	eval;
    Matrix<double>	Ut = A.eigen(eval);
    _T = yNormalize.Tinv()
       * Matrix<double>((double*)Ut[Ut.nrow()-1], ydim + 1, xdim1)
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
	Cost<ProjectiveMapping, Iterator>	cost(first, last);
	ConstNormConstraint<ProjectiveMapping>	constraint(*this);
	minimizeSquare(cost, constraint, *this);
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
template <class S, class B> inline Vector<double>
ProjectiveMapping::operator ()(const Vector<S, B>& x) const
{
    const Vector<double>&	y = mapP(x);
    return y(0, outDim()) / y[outDim()];
}

//! $BM?$($i$l$?E@$K<M1FJQ49$rE,MQ$7$F$=$NF1<!:BI8$rJV$9!%(B
/*!
  \param x	$BE@$NHsF1<!:BI8!J(B#inDim()$B<!85!K$^$?$OF1<!:BI8!J(B#inDim()+1$B<!85!K(B
  \return	$B<M1FJQ49$5$l$?E@$NF1<!:BI8!J(B#outDim()+1$B<!85!K(B
*/
template <class S, class B> inline Vector<double>
ProjectiveMapping::mapP(const Vector<S, B>& x) const
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

//! $BM?$($i$l$?E@$K$*$1$k(BJacobian$B$rJV$9!%(B
/*!
  Jacobian$B$H$O<M1FJQ499TNs@.J,$K4X$9$k(B1$B3,HyJ,$N$3$H$G$"$k!%(B
  \param x	$BE@$NHsF1<!:BI8!J(B#inDim()$B<!85!K$^$?$OF1<!:BI8!J(B#inDim()+1$B<!85!K(B
  \return	Jacobian$B!J(B#outDim() x (#outDim()+1)x(#inDim()+1)$B9TNs!K(B
*/
template <class S, class B> Matrix<double>
ProjectiveMapping::jacobian(const Vector<S, B>& x) const
{
    Vector<double>		xP(inDim() + 1);
    if (x.dim() == inDim())
    {
	xP(0, inDim()) = x;
	xP[inDim()]    = 1.0;
    }
    else
	xP = x;
    const Vector<double>&	y = mapP(xP);
    Matrix<double>		J(outDim(), (outDim() + 1)*xP.dim());
    for (int i = 0; i < J.nrow(); ++i)
    {
	J[i](i*xP.dim(), xP.dim()) = xP;
	(J[i](outDim()*xP.dim(), xP.dim()) = xP) *= (-y[i]/y[outDim()]);
    }
    J /= y[outDim()];

    return J;
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

//! $B<M1FJQ499TNs$N%N%k%`$N(B2$B>h$rJV$9!%(B
/*!
  \return	$B<M1FJQ499TNs$N%N%k%`$N(B2$B>h(B
*/
inline double
ProjectiveMapping::square() const
{
    return _T.square();
}

//! $B<M1FJQ499TNs$N3F9T$r=gHV$KJB$Y$?%Y%/%H%k$rJV$9!%(B
/*!
  \return	#T()$B$N@.J,$rJB$Y$?%Y%/%H%k!J(B(#outDim()+1)x(#inDim()+1)$B<!85!K(B
*/
inline
ProjectiveMapping::operator const Vector<double>() const
{
    return Vector<double>(const_cast<Matrix<double>&>(_T));
}

//! $B$3$N<M1FJQ49$N<+M3EY$rJV$9!%(B
/*!
  \return	$B<M1FJQ49$N<+M3EY!J(B(#outDim()+1)x(#inDim()+1)-1$B!K(B
*/
inline u_int
ProjectiveMapping::dof() const
{
    return (outDim() + 1)*(inDim() + 1)-1;
}

//! $B<M1FJQ499TNs$rM?$($i$l$?NL$@$1=$@5$9$k!%(B
/*!
  \param dt	$B=$@5NL$rI=$9%Y%/%H%k!J(B(#outDim()+1)x(#inDim()+1)$B<!85!K(B
*/
inline void
ProjectiveMapping::update(const Vector<double>& dt)
{
    Vector<double>	t(_T);
    double		l = t.length();
    (t -= dt).normalize() *= l;
}
    
template <class T, class Iterator>
ProjectiveMapping::Cost<T, Iterator>::Cost(Iterator first, Iterator last)
    :_first(first), _last(last), _npoints(std::distance(_first, _last))
{
}
    
template <class T, class Iterator> Vector<double>
ProjectiveMapping::Cost<T, Iterator>::operator ()(const AT& map) const
{
    const u_int	outDim = map.outDim();
    Vector<ET>	val(_npoints*outDim);
    int	n = 0;
    for (Iterator iter = _first; iter != _last; ++iter)
    {
	val(n, outDim) = map(iter->first) - iter->second;
	n += outDim;
    }
    
    return val;
}
    
template <class T, class Iterator> Matrix<double>
ProjectiveMapping::Cost<T, Iterator>::jacobian(const AT& map) const
{
    const u_int	outDim = map.outDim();
    Matrix<ET>	J(_npoints*outDim, map.dof()+1);
    int	n = 0;
    for (Iterator iter = _first; iter != _last; ++iter)
    {
	J(n, 0, outDim, J.ncol()) = map.jacobian(iter->first);
	n += outDim;
    }

    return J;
}

template <class T, class Iterator> inline void
ProjectiveMapping::Cost<T, Iterator>::update(AT& map, const Vector<ET>& dm)
{
    map.update(dm);
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
    const u_int		ndata = std::distance(first, last);
    if (ndata == 0)		// first$B$,M-8z$+!)(B
	throw std::invalid_argument("AffineMapping::initialize(): 0-length input data!!");
    const u_int		xdim = first->first.dim();
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
template <class T, class B=Buf<T> >
class HyperPlane : public Vector<T, B>
{
  public:
    HyperPlane(u_int d=2)						;

  //! $BF1<!:BI8%Y%/%H%k$r;XDj$7$FD6J?LL%*%V%8%'%/%H$r@8@.$9$k!%(B
  /*!
    (J\(Bparam p	(d+1)$B<!85%Y%/%H%k!J(Bd$B$OD6J?LL$,B8:_$9$k<M1F6u4V$N<!85!K(B
  */
    template <class T2, class B2>
    HyperPlane(const Vector<T2, B2>& p)	:Vector<T, B>(p)		{}

    template <class Iterator>
    HyperPlane(Iterator first, Iterator last)				;

  //! $BD6J?LL%*%V%8%'%/%H$NF1<!:BI8%Y%/%H%k$r;XDj$9$k!%(B
  /*!
    (J\(Bparam v	(d+1)$B<!85%Y%/%H%k!J(Bd$B$OD6J?LL$,B8:_$9$k<M1F6u4V$N<!85!K(B
    (J\(Breturn	$B$3$ND6J?LL%*%V%8%'%/%H(B
  */
    template <class T2, class B2>
    HyperPlane&	operator =(const Vector<T2, B2>& v)
				{Vector<T, B>::operator =(v); return *this;}

    template <class Iterator>
    void	fit(Iterator first, Iterator last)			;

  //! $B$3$ND6J?LL$,B8:_$9$k<M1F6u4V$N<!85$rJV$9!%(B
  /*! 
    (J\(Breturn	$B<M1F6u4V$N<!85(B($BF1<!:BI8$N%Y%/%H%k$H$7$F$N<!85$O(B#spaceDim()+1)
  */
    u_int	spaceDim()		const	{return Vector<T, B>::dim()-1;}

  //! $BD6J?LL$r5a$a$k$?$a$KI,MW$JE@$N:G>.8D?t$rJV$9!%(B
  /*!
    $B8=:_@_Dj$5$l$F$$$k<M1F6u4V$N<!85$r$b$H$K7W;;$5$l$k!%(B
    (J\(Breturn	$BI,MW$JE@$N:G>.8D?t$9$J$o$AF~NO6u4V$N<!85(B#spaceDim()
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
  (J\(Bparam d	$B$3$ND6J?LL$,B8:_$9$k<M1F6u4V$N<!85(B
*/
template <class T, class B> inline
HyperPlane<T, B>::HyperPlane(u_int d)
    :Vector<T, B>(d + 1)
{
    (*this)[d] = 1;
}
    
//! $BM?$($i$l$?E@Ns$NHsF1<!:BI8$KEv$F$O$a$i$l$?D6J?LL%*%V%8%'%/%H$r@8@.$9$k!%(B
/*!
  (J\(Bparam first			$BE@Ns$N@hF,$r<($9H?I|;R(B
  (J\(Bparam last			$BE@Ns$NKvHx$r<($9H?I|;R(B
  (J\(Bthrow std::invalid_argument	$BE@$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class T, class B> template <class Iterator> inline
HyperPlane<T, B>::HyperPlane(Iterator first, Iterator last)
{
    fit(first, last);
}

//! $BM?$($i$l$?E@Ns$NHsF1<!:BI8$KD6J?LL$rEv$F$O$a$k!%(B
/*!
  (J\(Bparam first			$BE@Ns$N@hF,$r<($9H?I|;R(B
  (J\(Bparam last			$BE@Ns$NKvHx$r<($9H?I|;R(B
  (J\(Bthrow std::invalid_argument	$BE@$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
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
  (J\(Bparam x	$BE@$NHsF1<!:BI8!J(B#spaceDim()$B<!85!K$^$?$OF1<!:BI8(B
		$B!J(B#spaceDim()+1$B<!85!K(B
  (J\(Breturn	$BE@$HD6J?LL$N5wN%$N(B2$B>h(B
*/
template <class T, class B> template <class T2, class B2> inline T
HyperPlane<T, B>::sqdist(const Vector<T2, B2>& x) const
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
template <class T, class B> template <class T2, class B2> double
HyperPlane<T, B>::dist(const Vector<T2, B2>& x) const
{
    if (x.dim() == spaceDim())
    {
	Vector<T2>	xx(spaceDim()+1);
	xx(0, spaceDim()) = x;
	xx[spaceDim()] = 1;
	return fabs((*this * xx)/(*this)(0, spaceDim()).length());
    }
    else if (x.dim() == spaceDim() + 1)
    {
	if (x[spaceDim()] == 0.0)
	    throw std::invalid_argument("HyperPlane::dist(): point at infinitiy!!");
	return fabs(((*this) * x)/
		    ((*this)(0, spaceDim()).length() * x[spaceDim()]));
    }
    else
	throw std::invalid_argument("HyperPlane::dist(): dimension mismatch!!");

    return 0;
}

typedef HyperPlane<float,  FixedSizedBuf<float,  3> >	LineP2f;
typedef HyperPlane<double, FixedSizedBuf<double, 3> >	LineP2d;
typedef HyperPlane<float,  FixedSizedBuf<float,  4> >	PlaneP3f;
typedef HyperPlane<double, FixedSizedBuf<double, 4> >	PlaneP3d;

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
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Point2d		xd(const Point2d& xc)		const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianXC(const Point2d& xc)	const	;
	virtual Point2d		xc(const Point2d& u)		const	;

      // calibration matrices.    
	virtual Matrix<double>	K()				const	;
	virtual Matrix<double>	Kt()				const	;
	virtual Matrix<double>	Kinv()				const	;
	virtual Matrix<double>	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		k()				const	;
	virtual Point2d		principal()			const	;
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
    Point2d		xc(const Vector<double>& x)		const	;
    Point2d		xc(const Point2d& u)			const	;
    Matrix<double>	Pc()					const	;
    Matrix<double>	jacobianPc(const Vector<double>& x)	const	;
    Matrix<double>	jacobianXc(const Vector<double>& x)	const	;

  // various oeprations in image coordinates.
    Point2d		operator ()(const Vector<double>& x)	const	;
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
    Point2d		principal()	const	{return intrinsic().principal();}
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
inline Point2d
CameraBase::xc(const Vector<double>& x) const
{
    const Vector<double>&	xx = _Rt * (x - _t);
    return Point2d(xx[0] / xx[2], xx[1] / xx[2]);
}

//! $B2hA|:BI8$K$*$1$kE@$N(B2$B<!850LCV$r(Bcanonical$B%+%a%i:BI87O$KD>$9(B
/*!
  \param u	$B2hA|:BI87O$K$*$1$kE@$N(B2$B<!850LCV!%(B
  \return	canonical$B%+%a%i:BI87O$K$*$1$k(Bu$B$N(B2$B<!850LCV!%(B
*/
inline Point2d
CameraBase::xc(const Point2d& u) const
{
    return intrinsic().xc(u);
}

//! 3$B<!856u4VCf$NE@$NA|$N2hA|:BI87O$K$*$1$k0LCV$r5a$a$k(B
/*!
  \param x	3$B<!856u4VCf$NE@$rI=$9(B3$B<!85%Y%/%H%k!%(B
  \return	x$B$NA|$N2hA|:BI87O$K$*$1$k0LCV!%(B
*/
inline Point2d
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
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianXC(const Point2d& xc)	const	;
	virtual Point2d		xc(const Point2d& u)		const	;

      // calibration matrices.
	virtual Matrix<double>	K()				const	;
	virtual Matrix<double>	Kt()				const	;
	virtual Matrix<double>	Kinv()				const	;
	virtual Matrix<double>	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		k()				const	;
	virtual	CameraBase::Intrinsic&
				setFocalLength(double k)		;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

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
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual Point2d		xc(const Point2d& u)		const	;
    
      // calibration matrices.	
	virtual Matrix<double>	K()				const	;
	virtual Matrix<double>	Kt()				const	;
	virtual Matrix<double>	Kinv()				const	;
	virtual Matrix<double>	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual Point2d		principal()			const	;
	virtual CameraBase::Intrinsic&
				setPrincipal(double u0, double v0)	;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      private:
	Point2d	_principal;
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
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianXC(const Point2d& xc)	const	;
	virtual Point2d		xc(const Point2d& u)		const	;

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
	virtual Point2d		operator ()(const Point2d& xc)	const	;
	virtual Point2d		xd(const Point2d& xc)		const	;
	virtual Matrix<double>	jacobianXC(const Point2d& xc)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& xc)	const	;
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;
	virtual Point2d		xc(const Point2d& u)		const	;

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
