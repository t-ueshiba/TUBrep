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
 *  $Id: Mapping.h,v 1.4 2008-10-05 23:25:17 ueshiba Exp $
 */
#ifndef __TUMapping_h
#define __TUMapping_h

#include "TU/utility.h"
#include "TU/Normalize.h"
#include "TU/Minimize.h"

namespace TU
{
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
    typedef double	value_type;

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
    void		fit(Iterator first, Iterator last,
			    bool refine=false)				;

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
    Matrix<double>	jacobian(const Vector<S, B>& x)		const	;

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
    template <class AT, class Iterator>
    class Cost
    {
      public:
	typedef double	value_type;

	Cost(Iterator first, Iterator last)				;

	Vector<double>	operator ()(const AT& map)		const	;
	Matrix<double>	jacobian(const AT& map)			const	;
	static void	update(AT& map, const Vector<double>& dm)	;
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
    fit(first, last, refine);
}

//! $BM?$($i$l$?E@BPNs$NHsF1<!:BI8$+$i<M1FJQ49$r7W;;$9$k!%(B
/*!
  \param first			$BE@BPNs$N@hF,$r<($9H?I|;R(B
  \param last			$BE@BPNs$NKvHx$r<($9H?I|;R(B
  \param refine			$BHs@~7?:GE,2=$NM-(B(true)$B!?L5(B(false)$B$r;XDj(B
  \throw std::invalid_argument	$BE@BP$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class Iterator> void
ProjectiveMapping::fit(Iterator first, Iterator last, bool refine)
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
	throw std::invalid_argument("ProjectiveMapping::fit(): not enough input data!!");

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
    return mapP(x).inhomogenize();
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
	return _T * x.homogenize();
    else
	return _T * x;
}

//! $BM?$($i$l$?E@$K$*$1$k%d%3%S9TNs$rJV$9!%(B
/*!
  $B%d%3%S9TNs$H$O<M1FJQ499TNs@.J,$K4X$9$k(B1$B3,HyJ,$N$3$H$G$"$k!%(B
  \param x	$BE@$NHsF1<!:BI8!J(B#inDim()$B<!85!K$^$?$OF1<!:BI8!J(B#inDim()+1$B<!85!K(B
  \return	#outDim() x (#outDim()+1)x(#inDim()+1)$B%d%3%S9TNs(B
*/
template <class S, class B> Matrix<double>
ProjectiveMapping::jacobian(const Vector<S, B>& x) const
{
    Vector<double>		xP;
    if (x.dim() == inDim())
	xP = x.homogenize();
    else
	xP = x;
    const Vector<double>&	y  = mapP(xP);
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
		$B!J(B#outDim()$B<!85!K$NBP(B
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
		$B!J(B#outDim()$B<!85!K$NBP(B
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
    
template <class AT, class Iterator>
ProjectiveMapping::Cost<AT, Iterator>::Cost(Iterator first, Iterator last)
    :_first(first), _last(last), _npoints(std::distance(_first, _last))
{
}
    
template <class AT, class Iterator> Vector<double>
ProjectiveMapping::Cost<AT, Iterator>::operator ()(const AT& map) const
{
    const u_int		outDim = map.outDim();
    Vector<double>	val(_npoints*outDim);
    int			n = 0;
    for (Iterator iter = _first; iter != _last; ++iter)
    {
	val(n, outDim) = map(iter->first) - iter->second;
	n += outDim;
    }
    
    return val;
}
    
template <class AT, class Iterator> Matrix<double>
ProjectiveMapping::Cost<AT, Iterator>::jacobian(const AT& map) const
{
    const u_int		outDim = map.outDim();
    Matrix<double>	J(_npoints*outDim, map.dof()+1);
    int			n = 0;
    for (Iterator iter = _first; iter != _last; ++iter)
    {
	J(n, 0, outDim, J.ncol()) = map.jacobian(iter->first);
	n += outDim;
    }

    return J;
}

template <class AT, class Iterator> inline void
ProjectiveMapping::Cost<AT, Iterator>::update(AT& map, const Vector<double>& dm)
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
    void	fit(Iterator first, Iterator last)		;
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
    fit(first, last);
}

//! $BM?$($i$l$?E@BPNs$NHsF1<!:BI8$+$i%"%U%#%sJQ49$r7W;;$9$k!%(B
/*!
  \param first			$BE@BPNs$N@hF,$r<($9H?I|;R(B
  \param last			$BE@BPNs$NKvHx$r<($9H?I|;R(B
  \throw std::invalid_argument	$BE@BP$N?t$,(B#ndataMin()$B$KK~$?$J$$>l9g$KAw=P(B
*/
template <class Iterator> void
AffineMapping::fit(Iterator first, Iterator last)
{
  // $B=<J,$J8D?t$NE@BP$,$"$k$+!)(B
    const u_int		ndata = std::distance(first, last);
    if (ndata == 0)		// first$B$,M-8z$+!)(B
	throw std::invalid_argument("AffineMapping::fit(): 0-length input data!!");
    const u_int		xdim = first->first.dim();
    if (ndata < xdim + 1)	// _T$B$N%5%$%:$,L$Dj$J$N$G(BndataMin()$B$OL58z(B
	throw std::invalid_argument("AffineMapping::fit(): not enough input data!!");

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
    
}
#endif	/* !__TUMapping_h */
