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
 *  $Id: Bezier++.h,v 1.13 2009-07-31 07:04:44 ueshiba Exp $
 */
#ifndef __TUBezierPP_h
#define __TUBezierPP_h

#include "TU/Vector++.h"

namespace TU
{
/************************************************************************
*  class BezierCurve<C>							*
************************************************************************/
template <class C>
class BezierCurve : private Array<C>
{
  public:
    typedef C					value_type;
    typedef typename value_type::value_type	T;
    
    BezierCurve(u_int p=0)	 :Array<C>(p+1)	{}
    BezierCurve(const Array<C>& b) :Array<C>(b)	{}

    static u_int	dim()			{return C::size();}

    u_int	degree()		  const	{return Array<C>::dim()-1;}
    C		operator ()(T t)	  const	;
    Array<C>	deCasteljau(T t, u_int r) const	;
    void	elevateDegree()			;
		operator const T*()	  const	{return (*this)[0];}

    friend	class Array2<BezierCurve<C> >;	// allow access to resize.
    
    using	Array<C>::operator [];
    using	Array<C>::operator ==;
    using	Array<C>::operator !=;
    using	Array<C>::save;
    using	Array<C>::restore;

    friend std::istream&
    operator >>(std::istream& in, BezierCurve<C>& b)
	{return in >> (Array<C>&)b;}
    friend std::ostream&
    operator <<(std::ostream& out, const BezierCurve<C>& b)
	{return out << (const Array<C>&)b;}
};

template <class C> C
BezierCurve<C>::operator ()(T t) const
{
    T		s = 1.0 - t, fact = 1.0;
    int		nCi = 1;
    C		b((*this)[0] * s);
    for (int i = 1; i < degree(); ++i)
    {
	fact *= t;
      /* 
       * Be careful! We cannot use operator "*=" here, because operator "/"
       * must not produce remainder
       */
	nCi = nCi * (degree() - i + 1) / i;
	(b += fact * nCi * (*this)[i]) *= s;
    }
    b += fact * t * (*this)[degree()];
    return b;
}

template <class C> Array<C>
BezierCurve<C>::deCasteljau(T t, u_int r) const
{
    if (r > degree())
	r = degree();

    const T	s = 1.0 - t;
    Array<C>	b_tmp(*this);
    for (int k = 1; k <= r; ++k)
	for (int i = 0; i <= degree() - k; ++i)
	    (b_tmp[i] *= s) += t * b_tmp[i+1];
    b_tmp.resize(degree() - r + 1);
    return b_tmp;
}

template <class C> void
BezierCurve<C>::elevateDegree()
{
    Array<C>	b_tmp(*this);
    Array<C>::resize(degree() + 2);
    (*this)[0] = b_tmp[0];
    for (int i = 1; i < degree(); ++i)
    {
	T	alpha = T(i) / T(degree());
	
	(*this)[i] = alpha * b_tmp[i-1] + (1.0 - alpha) * b_tmp[i];
    }
    (*this)[degree()] = b_tmp[degree()-1];
}

typedef BezierCurve<Vector2f>	BezierCurve2f;
typedef BezierCurve<Vector3f>	RationalBezierCurve2f;
typedef BezierCurve<Vector3f>	BezierCurve3f;
typedef BezierCurve<Vector4f>	RationalBezierCurve3f;
typedef BezierCurve<Vector2d>	BezierCurve2d;
typedef BezierCurve<Vector3d>	RationalBezierCurve2d;
typedef BezierCurve<Vector3d>	BezierCurve3d;
typedef BezierCurve<Vector4d>	RationalBezierCurve3d;

/************************************************************************
*  class BezierSurface<C>						*
************************************************************************/
template <class C>
class BezierSurface : private Array2<BezierCurve<C> >
{
  public:
    typedef C						value_type;
    typedef typename value_type::value_type		T;
    typedef BezierCurve<C>				Curve;

    BezierSurface(u_int p, u_int q) :Array2<Curve>(q+1, p+1)	{}
    BezierSurface(const Array2<Array<C> >& b)			;

    static u_int	dim()				{return C::size();}

    u_int	uDegree()			const	{return ncol()-1;}
    u_int	vDegree()			const	{return nrow()-1;}
    C		operator ()(T u, T v)		const	;
    Array2<Array<C> >
		deCasteljau(T u, T v, u_int r)	const	;
    void	uElevateDegree()			;
    void	vElevateDegree()			;
		operator const T*()		const	{return (*this)[0][0];}

    using	Array2<Curve>::operator [];
    using	Array2<Curve>::nrow;
    using	Array2<Curve>::ncol;
    using	Array2<Curve>::operator ==;
    using	Array2<Curve>::operator !=;
    using	Array2<Curve>::save;
    using	Array2<Curve>::restore;
    
    friend std::istream&
    operator >>(std::istream& in, BezierSurface<C>& b)
	{return in >> (Array2<Curve>&)b;}
    friend std::ostream&
    operator <<(std::ostream& out, const BezierSurface<C>& b)
	{return out << (const Array2<Curve>&)b;}
};

template <class C>
BezierSurface<C>::BezierSurface(const Array2<Array<C> >& b)
    :Array2<Curve>(b.nrow(), b.ncol())
{
    for (int j = 0; j <= vDegree(); ++j)
	for (int i = 0; i <= uDegree(); ++i)
	    (*this)[j][i] = b[j][i];
}

template <class C> C
BezierSurface<C>::operator ()(T u, T v) const
{
    BezierCurve<C>	vCurve(vDegree());
    for (int j = 0; j <= vDegree(); ++j)
	vCurve[j] = (*this)[j](u);
    return vCurve(v);
}
 
typedef BezierSurface<Vector3f>	BezierSurface3f;
typedef BezierSurface<Vector4f>	RationalBezierSurface3f;
typedef BezierSurface<Vector3d>	BezierSurface3d;
typedef BezierSurface<Vector4d>	RationalBezierSurface3d;
 
}

#endif
