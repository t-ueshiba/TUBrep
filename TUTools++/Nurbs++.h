/*
 *  ����14-19�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
 *  
 *  �n��ҁF�A�ŏr�v
 *
 *  �{�v���O�����́i�Ɓj�Y�ƋZ�p�����������̐E���ł���A�ŏr�v���n�삵�C
 *  �i�Ɓj�Y�ƋZ�p���������������쌠�����L����閧���ł��D���쌠���L
 *  �҂ɂ�鋖�Ȃ��ɖ{�v���O�������g�p�C�����C���ρC��O�҂֊J������
 *  ���̍s�ׂ��֎~���܂��D
 *  
 *  ���̃v���O�����ɂ���Đ����邢���Ȃ鑹�Q�ɑ΂��Ă��C���쌠���L�҂�
 *  ��ёn��҂͐ӔC�𕉂��܂���B
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
 *  $Id: Nurbs++.h,v 1.14 2009-07-31 07:04:45 ueshiba Exp $
 */
#ifndef __TUNurbsPP_h
#define __TUNurbsPP_h

#include "TU/Vector++.h"
#include "TU/utility.h"

namespace TU
{
/************************************************************************
*  class BSplineKnots<T>						*
************************************************************************/
template <class T>
class BSplineKnots : private Array<T>
{
  public:
    BSplineKnots(u_int degree, T us, T ue)	;
    
    u_int	degree()		const	{return _degree;}
    u_int	M()			const	{return dim()-1;}
    u_int	L()			const	{return M()-degree()-degree();}
    int		findSpan(T u)		const	;
    int		leftmost(int k)		const	;
    int		rightmost(int k)	const	;
    u_int	multiplicity(int k)	const	;

    Array<T>	basis(T u, int& I)	const	;
    Array2<Array<T> >
      derivatives(T u, u_int K, int& I)	const	;

    int		insertKnot(T u)			;
    int		removeKnot(int k)		;
    void	elevateDegree()			{++_degree;}
		operator const T*()	const	{return
						  Array<T>::operator const T*();}

    using	Array<T>::dim;
    using	Array<T>::operator [];		// knots
    
  private:
    u_int	_degree;
};

/*
 *  BSplineKnots<T>::BSplineKnots(u_int deg, T us=0.0, T ue=1.0)
 *
 *    Create a knot sequence of {us, ..., us, ue, ..., ue},
 *				 ^^^^^^^^^^^  ^^^^^^^^^^^
 *			         deg+1 times  deg+1 times
 */
template <class T>
BSplineKnots<T>::BSplineKnots(u_int deg, T us, T ue)
    :Array<T>(deg+1+deg+1), _degree(deg)
{
    for (int i = 0; i <= degree(); ++i)
	(*this)[i] = us;
    for (int i = M() - degree(); i <= M(); ++i)
	(*this)[i] = ue;
}

/*
 *  int BSplineKnots<T>::findSpan(T u) const
 *
 *    Find span such that u_{span} <= u < u_{span+1} for given 'u'.
 */
template <class T> int
BSplineKnots<T>::findSpan(T u) const
{
    if (u == (*this)[M()-degree()])	// special case
	return M()-degree()-1;

  // binary search
    for (int low = degree(), high = M()-degree(); low != high; )
    {
	int	mid = (low + high) / 2;

	if (u < (*this)[mid])
	    high = mid;
	else if (u >= (*this)[mid+1])
	    low = mid;
	else
	    return mid;
    }

    throw std::out_of_range("TU::BSplineKnots<T>::findSpan: given parameter is out of range!");
    
    return 0;
}

/*
 *   int BSplineKnots<T>::leftmost(int k) const
 *
 *     Return index of the leftmost knot with same value as k-th knot.
 */
template <class T> int
BSplineKnots<T>::leftmost(int k) const
{
    while (k-1 >= 0 && (*this)[k-1] == (*this)[k])
	--k;
    return k;
}

/*
 *   int BSplineKnots<T>::rightmost(int k) const
 *
 *     Return index of the rightmost knot with same value as k-th knot.
 */
template <class T> int
BSplineKnots<T>::rightmost(int k) const
{
    while (k+1 <= M() && (*this)[k+1] == (*this)[k])
	++k;
    return k;
}

/*
 *   u_int BSplineKnots<T>::multiplicity(int k) const
 *
 *     Return multiplicity of k-th knot.
 */
template <class T> u_int
BSplineKnots<T>::multiplicity(int k) const
{
    return rightmost(k) - leftmost(k) + 1;
}

/*
 *  Array<T> BSplineKnots<T>::basis(T u, int& I) const
 *
 *    Compute 'I' such that u_{I} <= u < u_{I} and return an array with
 *    values of basis:
 *      array[i] = N_{I-p+i}(u) where 0 <= i <= degree.
 */
template <class T> Array<T>
BSplineKnots<T>::basis(T u, int& I) const
{
    I = findSpan(u);
    
    Array<T>	Npi(degree()+1);
    Array<T>	left(degree()), right(degree());
    Npi[0] = 1.0;
    for (int i = 0; i < degree(); ++i)
    {
	left[i]	 = u - (*this)[I-i];
	right[i] = (*this)[I+i+1] - u;
	T  saved = 0.0;
	for (int j = 0; j <= i; ++j)
	{
	    const T tmp	= Npi[j] / (right[j] + left[i-j]);
	    Npi[j]	= saved + right[j]*tmp;
	    saved	= left[i-j]*tmp;
	}
	Npi[i+1] = saved;
    }
    return Npi;
}

/*
 *  Array2<Array<T> >
 *  BSplineKnots<T>::derivatives(T u, u_int K, int& I) const
 *
 *    Compute 'I' such that u_{I} <= u < u_{I} and return an 2D array with
 *    derivative values of basis:
 *      array[k][i] = "k-th derivative of N_{I-p+i}(u)"
 *	  where 0 <= k <= K and 0 <= i <= degree.
 */
template <class T> Array2<Array<T> >
BSplineKnots<T>::derivatives(T u, u_int K, int& I) const
{
    I = findSpan(u);
    
    Array2<Array<T> >	ndu(degree()+1, degree()+1);
    Array<T>		left(degree()), right(degree());
    ndu[0][0] = 1.0;
    for (int i = 0; i < degree(); ++i)
    {
	left[i]  = u - (*this)[I-i];
	right[i] = (*this)[I+i+1] - u;
	T  saved = 0.0;
	for (int j = 0; j <= i; ++j)
	{
	    ndu[j][i+1] = right[j] + left[i-j];		// upper triangle

	    const T tmp = ndu[i][j] / ndu[j][i+1];
	    ndu[i+1][j] = saved + right[j]*tmp;		// lower triangle
	    saved	= left[i-j]*tmp;
	}
	ndu[i+1][i+1] = saved;				// diagonal elements
    }
    
    Array2<Array<T> >	N(K+1, degree()+1);
    N[0] = ndu[degree()];				// values of basis
    for (int i = 0; i <= degree(); ++i)
    {
	Array2<Array<T> >	a(2, degree()+1);
	int			previous = 0, current = 1;
	a[previous][0] = 1.0;
	for (int k = 1; k <= K; ++k)			// k-th derivative
	{
	    N[k][i] = 0.0;
	    for (int j = std::max(0, k-i); j <= std::min(k, degree()-i); ++j)
	    {
		a[current][j] = ((j != k ? a[previous][j]   : 0.0) -
				 (j != 0 ? a[previous][j-1] : 0.0))
			      / ndu[i-k+j][degree()-k+1];
		N[k][i] += a[current][j] * ndu[degree()-k][i-k+j];
	    }
	    std::swap(current, previous);
	}
    }

  // Multiply factors    
    int	fact = degree();
    for (int k = 1; k <= K; ++k)
    {
	for (int i = 0; i <= degree(); ++i)
	    N[k][i] *= fact;
	fact *= (degree() - k);
    }
    
    return N;
}

/*
 *  int BSplineKnots<T>::insertKnot(T u)
 *
 *    Insert a knot with value 'u' and return its index of location.
 */
template <class T> int
BSplineKnots<T>::insertKnot(T u)
{
    int		l = findSpan(u) + 1;	// insertion point for the new knot
    Array<T>	tmp(*this);
    resize(dim() + 1);
    for (int i = 0; i < l; ++i)		// copy unchanged knots
	(*this)[i] = tmp[i];
    (*this)[l] = u;			// insert a new knot
    for (int i = M(); i > l; --i)	// shift unchanged knots
	(*this)[i] = tmp[i-1];
    return rightmost(l);		// index of the new knot
}

/*
 *  int BSplineKnots<T>::removeKnot(int k)
 *
 *    Remove k-th knot and return its right-most index.
 */
template <class T> int
BSplineKnots<T>::removeKnot(int k)
{
    k = rightmost(k);			// index of the knot to be removed
    Array<T>	tmp(*this);
    Array<T>::resize(dim() - 1);
    for (int i = 0; i < k; ++i)		// copy unchanged knots
	(*this)[i] = tmp[i];
    for (int i = M(); i >= k; --i)	// shift unchanged knots
	(*this)[i] = tmp[i+1];
    return k;				// index of the new knot
}

/************************************************************************
*  class BSplineCurve<C>						*
************************************************************************/
template <class C>
class BSplineCurve : private Array<C>
{
  public:
    typedef C					value_type;
    typedef typename value_type::value_type	T;
    
    BSplineCurve(u_int degree, T us=0.0, T ue=1.0)	;

    static u_int	dim()			{return C::size();}

    u_int	degree()		  const	{return _knots.degree();}
    u_int	M()			  const	{return _knots.M();}
    u_int	L()			  const	{return _knots.L();}
    u_int	N()			  const	{return Array<C>::dim()-1;}
    T		knots(int i)		  const {return _knots[i];}
    u_int	multiplicity(int k)	  const {return
						     _knots.multiplicity(k);}
    const BSplineKnots<T>&
		knots()			  const	{return _knots;}

    C		operator ()(T u)	  const	;
    Array<C>	derivatives(T u, u_int K) const	;

    int		insertKnot(T u)			;
    int		removeKnot(int k)		;
    void	elevateDegree()			;
		operator const T*()	  const	{return (*this)[0];}

    using	Array<C>::operator [];
    using	Array<C>::operator ==;
    using	Array<C>::operator !=;
    using	Array<C>::save;
    using	Array<C>::restore;

  private:
    BSplineKnots<T>	_knots;
};

template <class C>
BSplineCurve<C>::BSplineCurve(u_int degree, T us, T ue)
    :Array<C>(degree + 1), _knots(degree, us, ue)
{
}

/*
 *  C BSplineCurve<C>::operator ()(T u) const
 *
 *    Evaluate the coodinate of the curve at 'u'.
 */
template <class C> C
BSplineCurve<C>::operator ()(T u) const
{
    int		span;
    Array<T>	N = _knots.basis(u, span);
    C		c;
    for (int i = 0; i <= degree(); ++i)
	c += N[i] * (*this)[span-degree()+i];
    return c;
}

/*
 *  Array<C> BSplineCurve<C>::derivatives(T u, u_int K) const
 *
 *    Evaluate up to K-th derivatives of the curve at 'u':
 *      array[k] = "k-th derivative of the curve at 'u'" where 0 <= k <= K.
 */
template <class C> Array<C>
BSplineCurve<C>::derivatives(T u, u_int K) const
{
    int			I;
    Array2<Array<T> >	dN = _knots.derivatives(u, min(K,degree()), I);
    Array<C>		ders(K+1);
    for (int k = 0; k < dN.nrow(); ++k)
	for (int i = 0; i <= degree(); ++i)
	    ders[k] += dN[k][i] * (*this)[I-degree()+i];
    return ders;
}

/*
 *  int BSplineCurve<C>::insertKnot(T u)
 *
 *    Insert a knot at 'u', recompute control points and return the index
 *    of the new knot.
 */
template <class C> int
BSplineCurve<C>::insertKnot(T u)
{
    int		l = _knots.insertKnot(u);
    Array<C>	tmp(*this);
    resize(Array<C>::dim() + 1);	// cannot omit Array<C>:: specifier
    for (int i = 0; i < l-degree(); ++i)
	(*this)[i] = tmp[i];		// copy unchanged control points
    for (int i = l-degree(); i < l; ++i)
    {
      //  Note that we have already inserted a new knot at l. So, old
      //  knots(i+degree()) must be accressed as knots(i+degree()+1).

	T	alpha = (u - knots(i)) / (knots(i+degree()+1) - knots(i));

	(*this)[i] = (1.0 - alpha) * tmp[i-1] + alpha * tmp[i];
    }
    for (int i = N(); i >= l; --i)
	(*this)[i] = tmp[i-1];		// copy unchanged control points

    return l;
}

/*
 *  int BSplineCurve<C>::removeKnot(int k)
 *
 *    Remove k-th knot, recompute control points and return the index of
 *    the removed knot.
 */
template <class C> int
BSplineCurve<C>::removeKnot(int k)
{
    u_int	s = multiplicity(k);
    T		u = knots(k);
    k = _knots.removeKnot(k);
    Array<C>	tmp(*this);
    resize(Array<C>::dim() - 1);	// cannot omit Array<C>:: specifier
    int		i, j;
    for (i = 0; i < k - degree(); ++i)
	(*this)[i] = tmp[i];		// copy unchanged control points
    for (j = N(); j >= k - s; --j)
	(*this)[j] = tmp[j+1];		// copy unchanged control points
    for (i = k - degree(), j = k - s; i < j - 1; ++i, --j)
    {
	T	alpha_i = (u - knots(i)) / (knots(i+degree()) - knots(i)),
		alpha_j = (u - knots(j)) / (knots(j+degree()) - knots(j));
	(*this)[i]   = (tmp[i] - (1.0 - alpha_i) * (*this)[i-1]) / alpha_i;
	(*this)[j-1] = (tmp[j] - alpha_j * (*this)[j]) / (1.0 - alpha_j);
    }
    if (i == j - 1)
    {
	T	alpha_i = (u - knots(i)) / (knots(i+degree()) - knots(i)),
		alpha_j = (u - knots(j)) / (knots(j+degree()) - knots(j));
	(*this)[i] = ((tmp[i] - (1.0 - alpha_i) * (*this)[i-1]) / alpha_i + 
		      (tmp[j] - alpha_j * (*this)[j]) / (1.0 - alpha_j)) / 2.0;
    }
    
    return k;
}

/*
 *  void BSplineCurve<C>::elevateDegree()
 *
 *    Elevate degree of the curve by one.
 */
template <class C> void
BSplineCurve<C>::elevateDegree()
{
  // Convert to Bezier segments.
    Array<u_int>	mul(L());
    u_int		nsegments = 1;
    for (int k = degree() + 1; k < M() - degree(); k += degree())
    {
      // Elevate multiplicity of each internal knot to degree().
	mul[nsegments-1] = multiplicity(k);
	for (u_int n = mul[nsegments-1]; n < degree(); ++n)
	    insertKnot(knots(k));
	++nsegments;
    }
    Array<C>	tmp(*this);	// Save control points of Bezier segments.

  // Set knots and allocate area for control points.
    for (int k = 0; k <= M(); )	// Elevate multiplicity of each knot by one.
	k = _knots.insertKnot(knots(k)) + 1;
    _knots.elevateDegree();
    resize(Array<C>::dim() + nsegments);

  // Elevate degree of each Bezier segment.
    for (int n = 0; n < nsegments; ++n)
    {
	(*this)[n*degree()] = tmp[n*(degree()-1)];
	for (int i = 1; i < degree(); ++i)
	{
	    T	alpha = T(i) / T(degree());
	    
	    (*this)[n*degree()+i] = alpha	  * tmp[n*(degree()-1)+i-1]
				  + (1.0 - alpha) * tmp[n*(degree()-1)+i];
	}
    }
    (*this)[nsegments*degree()] = tmp[nsegments*(degree()-1)];

  // Remove redundant internal knots.
    for (int k = degree() + 1, n = 0; k < M() - degree(); k += mul[n]+1, ++n)
	for (int r = degree(); --r > mul[n]; )
	    removeKnot(k);
}

typedef BSplineCurve<Vector<float, FixedSizedBuf<float, 2> > >
BSplineCurve2f;
typedef BSplineCurve<Vector<float, FixedSizedBuf<float, 3> > >
RationalBSplineCurve2f;
typedef BSplineCurve<Vector<float, FixedSizedBuf<float, 3> > >
BSplineCurve3f;
typedef BSplineCurve<Vector<float, FixedSizedBuf<float, 4> > >
RationalBSplineCurve3f;
typedef BSplineCurve<Vector<double, FixedSizedBuf<double, 2> > >
BSplineCurve2d;
typedef BSplineCurve<Vector<double, FixedSizedBuf<double, 3> > >
RationalBSplineCurve2d;
typedef BSplineCurve<Vector<double, FixedSizedBuf<double, 3> > >
BSplineCurve3d;
typedef BSplineCurve<Vector<double, FixedSizedBuf<double, 4> > >
RationalBSplineCurve3d;
    
/************************************************************************
*  class BSplineSurface<C>						*
************************************************************************/
template <class C>
class BSplineSurface : private Array2<Array<C> >
{
  public:
    typedef C					value_type;
    typedef typename value_type::value_type	T;
    
    BSplineSurface(u_int uDegree, u_int vDegree,
		   T us=0.0, T ue=1.0, T vs=0.0, T ve=1.0)	;

    static u_int	dim()			{return C::size();}

    u_int	uDegree()		const	{return _uKnots.degree();}
    u_int	uM()			const	{return _uKnots.M();}
    u_int	uL()			const	{return _uKnots.L();}
    u_int	uN()			const	{return ncol()-1;}
    u_int	vDegree()		const	{return _vKnots.degree();}
    u_int	vM()			const	{return _vKnots.M();}
    u_int	vL()			const	{return _vKnots.L();}
    u_int	vN()			const	{return nrow()-1;}
    T		uKnots(int i)		const	{return _uKnots[i];}
    T		vKnots(int j)		const	{return _vKnots[j];}
    u_int	uMultiplicity(int k)	const	{return
						     _uKnots.multiplicity(k);}
    u_int	vMultiplicity(int l)	const	{return
						     _vKnots.multiplicity(l);}
    const BSplineKnots<T>&
		uKnots()		const	{return _uKnots;}
    const BSplineKnots<T>&
		vKnots()		const	{return _vKnots;}

    C		operator ()(T u, T v)	const	;
    Array2<Array<C> >
	derivatives(T u, T v, u_int D)	const	;

    int		uInsertKnot(T u)		;
    int		vInsertKnot(T v)		;
    int		uRemoveKnot(int k)		;
    int		vRemoveKnot(int l)		;
    void	uElevateDegree()		;
    void	vElevateDegree()		;
		operator const T*()	const	{return (*this)[0][0];}

    using	Array2<Array<C> >::operator [];
    using	Array2<Array<C> >::ncol;
    using	Array2<Array<C> >::nrow;
    using	Array2<Array<C> >::operator ==;
    using	Array2<Array<C> >::operator !=;
    using	Array2<Array<C> >::save;
    using	Array2<Array<C> >::restore;
    
    friend std::istream&
    operator >>(std::istream& in, BSplineSurface<C>& b)
	{return in >> (Array2<Array<C> >&)b;}
    friend std::ostream&
    operator <<(std::ostream& out, const BSplineSurface<C>& b)
	{return out << (const Array2<Array<C> >&)b;}

  private:
    BSplineKnots<T>	_uKnots, _vKnots;
};

template <class C>
BSplineSurface<C>::BSplineSurface(u_int uDeg, u_int vDeg,
				  T us, T ue, T vs, T ve)
    :Array2<Array<C> >(vDeg + 1, uDeg + 1),
     _uKnots(uDeg, us, ue), _vKnots(vDeg, vs, ve)
{
}

/*
 *  C BSplineSurface<C>::operator ()(T u, T v) const
 *
 *    Evaluate the coodinate of the surface at (u, v).
 */
template <class C> C
BSplineSurface<C>::operator ()(T u, T v) const
{
    int		uSpan, vSpan;
    Array<T>	Nu = _uKnots.basis(u, uSpan);
    Array<T>	Nv = _vKnots.basis(v, vSpan);
    C		c;
    for (int j = 0; j <= vDegree(); ++j)
    {
	C	tmp;
	for (int i = 0; i <= uDegree(); ++i)
	    tmp += Nu[i] * (*this)[vSpan-vDegree()+j][uSpan-uDegree()+i];
	c += Nv[j] * tmp;
    }
    return c;
}

/*
 *  Array2<Array<C> >
 *  BSplineSurface<C>::derivatives(T u, T v, u_int D) const
 *
 *    Evaluate up derivatives of the surface at (u, v):
 *      array[l][k] = "derivative of order k w.r.t u and order l w.r.t v"
 *        where 0 <= k <= D and 0 <= l <= D.
 */
template <class C> Array2<Array<C> >
BSplineSurface<C>::derivatives(T u, T v, u_int D) const
{
    int			I, J;
    Array2<Array<T> >	udN = _uKnots.derivatives(u, min(D,uDegree()), I),
			vdN = _vKnots.derivatives(v, min(D,vDegree()), J);
    Array2<Array<C> >	ders(D+1, D+1);
    for (int k = 0; k < udN.nrow(); ++k)		// derivatives w.r.t u
    {
	Array<C>	tmp(vDegree()+1);
	for (int j = 0; j <= vDegree(); ++j)
	    for (int i = 0; i <= uDegree(); ++i)
		tmp[j] += udN[k][i] * (*this)[J-vDegree()+j][I-uDegree()+i];
	for (int l = 0; l < min(vdN.nrow(), D-k); ++l)	// derivatives w.r.t v
	    for (int j = 0; j <= vDegree(); ++j)
		ders[l][k] += vdN[l][j] * tmp[j];
    }
    return ders;
}

/*
 *  int BSplineSurface<C>::uInsertKnot(T u)
 *
 *    Insert a knot in u-direction at 'u', recompute control points and return
 *    the index of the new knot.
 */
template <class C> int
BSplineSurface<C>::uInsertKnot(T u)
{
    int			l = _uKnots.insertKnot(u);
    Array2<Array<C> >	tmp(*this);
    resize(nrow(), ncol()+1);
    Array<T>		alpha(uDegree());
    for (int i = l-uDegree(); i < l; ++i)
	alpha[i-l+uDegree()] =
	    (u - uKnots(i)) / (uKnots(i+uDegree()+1) - uKnots(i));
    for (int j = 0; j <= vN(); ++j)
    {
	for (int i = 0; i < l-uDegree(); ++i)
	    (*this)[j][i] = tmp[j][i];
	for (int i = l-uDegree(); i < l; ++i)
	    (*this)[j][i] = (1.0 - alpha[i-l+uDegree()]) * tmp[j][i-1]
				 + alpha[i-l+uDegree()]  * tmp[j][i];
	for (int i = uN(); i >= l; --i)
	    (*this)[j][i] = tmp[j][i-1];
    }

    return l;
}

/*
 *  int BSplineSurface<C>::vInsertKnot(T v)
 *
 *    Insert a knot in v-direction at 'v', recompute control points and return
 *    the index of the new knot.
 */
template <class C> int
BSplineSurface<C>::vInsertKnot(T v)
{
    int			l = _vKnots.insertKnot(v);
    Array2<Array<C> >	tmp(*this);
    resize(nrow()+1, ncol());
    Array<T>		alpha(vDegree());
    for (int j = l-vDegree(); j < l; ++j)
	alpha[j-l+vDegree()] =
	    (v - vKnots(j)) / (vKnots(j+vDegree()+1) - vKnots(j));
    for (int i = 0; i <= uN(); ++i)
    {
	for (int j = 0; j < l-vDegree(); ++j)
	    (*this)[j][i] = tmp[j][i];
	for (int j = l-vDegree(); j < l; ++j)
	    (*this)[j][i] = (1.0 - alpha[j-l+vDegree()]) * tmp[j-1][i]
				 + alpha[j-l+vDegree()]  * tmp[j]  [i];
	for (int j = vN(); j >= l; --j)
	    (*this)[j][i] = tmp[j-1][i];
    }

    return l;
}

/*
 *  int BSplineSurface<C>::uRemoveKnot(int k)
 *
 *    Remove k-th knot in u-derection, recompute control points and return 
 *    the index of the removed knot.
 */
template <class C> int
BSplineSurface<C>::uRemoveKnot(int k)
{
    u_int	s = uMultiplicity(k);
    T		u = uKnots(k);
    k = _uKnots.removeKnot(k);
    Array2<Array<C> >	tmp(*this);
    resize(nrow(), ncol()-1);
    for (int j = 0; j <= vN(); ++j)
    {
	int	is, ie;
	for (is = 0; is < k - uDegree(); ++is)
	    (*this)[j][is] = tmp[j][is];    // copy unchanged control points
	for (ie = uN(); ie >= k - s; --ie)
	    (*this)[j][ie] = tmp[j][ie+1];  // copy unchanged control points
	for (is = k - uDegree(), ie = k - s; is < ie - 1; ++is, --ie)
	{
	    T	alpha_s	     = (u - uKnots(is))
			     / (uKnots(is+uDegree()) - uKnots(is)),
		alpha_e	     = (u - uKnots(ie))
			     / (uKnots(ie+uDegree()) - uKnots(ie));
	    (*this)[j][is]   = (tmp[j][is] - (1.0 - alpha_s)*(*this)[j][is-1])
			     / alpha_s;
	    (*this)[j][ie-1] = (tmp[j][ie] - alpha_e * (*this)[j][ie])
			     / (1.0 - alpha_e);
	}
	if (is == ie - 1)
	{
	    T	alpha_s	   = (u - uKnots(is))
			   / (uKnots(is+uDegree()) - uKnots(is)),
		alpha_e	   = (u - uKnots(ie))
			   / (uKnots(ie+uDegree()) - uKnots(ie));
	    (*this)[j][is] = ((tmp[j][is] - (1.0 - alpha_s)*(*this)[j][is-1]) /
			      alpha_s +
			      (tmp[j][ie] - alpha_e * (*this)[j][ie]) /
			      (1.0 - alpha_e)
			     ) / 2.0;
	}
    }
    
    return k;
}

/*
 *  int BSplineSurface<C>::vRemoveKnot(int l)
 *
 *    Remove l-th knot in v-derection, recompute control points and return 
 *    the index of the removed knot.
 */
template <class C> int
BSplineSurface<C>::vRemoveKnot(int l)
{
    u_int	s = vMultiplicity(l);
    T		v = vKnots(l);
    l = _vKnots.removeKnot(l);
    Array2<Array<C> >	tmp(*this);
    resize(nrow()-1, ncol());
    for (int i = 0; i <= uN(); ++i)
    {
	int	js, je;
	for (js = 0; js < l - vDegree(); ++js)
	    (*this)[js][i] = tmp[js][i];    // copy unchanged control points
	for (je = vN(); je >= l - s; --je)
	    (*this)[je][i] = tmp[je+1][i];  // copy unchanged control points
	for (js = l - vDegree(), je = l - s; js < je - 1; ++js, --je)
	{
	    T	alpha_s      = (v - vKnots(js))
			     / (vKnots(js+vDegree()) - vKnots(js)),
		alpha_e	     = (v - vKnots(je))
			     / (vKnots(je+vDegree()) - vKnots(je));
	    (*this)[js][i]   = (tmp[js][i] - (1.0 - alpha_s)*(*this)[js-1][i])
			     / alpha_s;
	    (*this)[je-1][i] = (tmp[je][i] - alpha_e * (*this)[je][i])
			     / (1.0 - alpha_e);
	}
	if (js == je - 1)
	{
	    T	alpha_s	   = (v - vKnots(js))
			   / (vKnots(js+vDegree()) - vKnots(js)),
		alpha_e	   = (v - vKnots(je))
			   / (vKnots(je+vDegree()) - vKnots(je));
	    (*this)[js][i] = ((tmp[js][i] - (1.0 - alpha_s)*(*this)[js-1][i]) /
			      alpha_s +
			      (tmp[je][i] - alpha_e * (*this)[je][i]) /
			      (1.0 - alpha_e)
			     ) / 2.0;
	}
    }
    
    return l;
}

/*
 *  void BSplineSurface<C>::uElevateDegree()
 *
 *    Elevate degree of the surface by one in u-direction.
 */
template <class C> void
BSplineSurface<C>::uElevateDegree()
{
  // Convert to Bezier segments.
    Array<u_int>	mul(uL());
    u_int		nsegments = 1;
    for (int k = uDegree() + 1; k < uM() - uDegree(); k += uDegree())
    {
      // Elevate multiplicity of each internal knot to uDegree().
	mul[nsegments-1] = uMultiplicity(k);
	for (u_int n = mul[nsegments-1]; n < uDegree(); ++n)
	    uInsertKnot(uKnots(k));
	++nsegments;
    }
    Array2<Array<C> >	tmp(*this);	// Save Bezier control points.

  // Set knots and allocate area for control points.
    for (int k = 0; k <= uM(); )
	k = _uKnots.insertKnot(uKnots(k)) + 1;
    _uKnots.elevateDegree();
    resize(nrow(), ncol() + nsegments);
    
  // Elevate degree of each Bezier segment.
    for (int j = 0; j <= vN(); ++j)
    {
	for (int n = 0; n < nsegments; ++n)
	{
	    (*this)[j][n*uDegree()] = tmp[j][n*(uDegree()-1)];
	    for (int i = 1; i < uDegree(); ++i)
	    {
		T	alpha = T(i) / T(uDegree());
	    
		(*this)[j][n*uDegree()+i]
		    = alpha	    * tmp[j][n*(uDegree()-1)+i-1]
		    + (1.0 - alpha) * tmp[j][n*(uDegree()-1)+i];
	    }
	    (*this)[j][nsegments*uDegree()] = tmp[j][nsegments*(uDegree()-1)];
	}
    }

  // Remove redundant internal knots.
    for (int k = uDegree() + 1, j = 0;
	 k < uM() - uDegree(); k += mul[j]+1, ++j)
	for (int r = uDegree(); --r > mul[j]; )
	    uRemoveKnot(k);
}

/*
 *  void BSplineSurface<C>::vElevateDegree()
 *
 *    Elevate degree of the surface by one in v-direction.
 */
template <class C> void
BSplineSurface<C>::vElevateDegree()
{
  // Convert to Bezier segments.
    Array<u_int>	mul(vL());
    u_int		nsegments = 1;
    for (int l = vDegree() + 1; l < vM() - vDegree(); l += vDegree())
    {
      // Elevate multiplicity of each internal knot to vDegree().
	mul[nsegments-1] = vMultiplicity(l);
	for (u_int n = mul[nsegments-1]; n < vDegree(); ++n)
	    vInsertKnot(vKnots(l));
	++nsegments;
    }
    Array2<Array<C> >	tmp(*this);	// Save Bezier control points.

  // Set knots and allocate area for control points.
    for (int l = 0; l <= vM(); )
	l = _vKnots.insertKnot(vKnots(l)) + 1;
    _vKnots.elevateDegree();
    resize(nrow() + nsegments, ncol());
    
  // Elevate degree of each Bezier segment.
    for (int i = 0; i <= uN(); ++i)
    {
	for (int n = 0; n < nsegments; ++n)
	{
	    (*this)[n*vDegree()][i] = tmp[n*(vDegree()-1)][i];
	    for (int j = 1; j < vDegree(); ++j)
	    {
		T	alpha = T(j) / T(vDegree());
	    
		(*this)[n*vDegree()+j][i]
		    = alpha	    * tmp[n*(vDegree()-1)+j-1][i]
		    + (1.0 - alpha) * tmp[n*(vDegree()-1)+j][i];
	    }
	    (*this)[nsegments*vDegree()][i] = tmp[nsegments*(vDegree()-1)][i];
	}
    }

  // Remove redundant internal knots.
    for (int l = vDegree() + 1, i = 0;
	 l < vM() - vDegree(); l += mul[i]+1, ++i)
	for (int r = vDegree(); --r > mul[i]; )
	    vRemoveKnot(l);
}

typedef BSplineSurface<Vector<float, FixedSizedBuf<float, 3> > >
BSplineSurface3f;
typedef BSplineSurface<Vector<float, FixedSizedBuf<float, 4> > >
RationalBSplineSurface3f;
typedef BSplineSurface<Vector<double, FixedSizedBuf<double, 3> > >
BSplineSurface3d;
typedef BSplineSurface<Vector<double, FixedSizedBuf<double, 4> > >
RationalBSplineSurface3d;

}
#endif
