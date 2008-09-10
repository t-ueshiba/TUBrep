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
 *  $Id: GaussianConvolver.h,v 1.7 2008-09-10 05:10:36 ueshiba Exp $
 */
#ifndef	__TUGaussianConvolver_h
#define	__TUGaussianConvolver_h

#include "TU/Vector++.h"
#include "TU/IIRFilter.h"

namespace TU
{
/************************************************************************
*  class GaussianCoefficients						*
************************************************************************/
class GaussianCoefficients
{
  private:
    struct Params
    {
	void		set(double aa, double bb, double tt, double aaa);
	Params&		operator -=(const Vector<double>& p)		;
    
	double		a, b, theta, alpha;
    };

    class EvenConstraint
    {
      public:
	typedef double		value_type;
	typedef Array<Params>	AT;

	EvenConstraint(double sigma) :_sigma(sigma)				{}
	
	Vector<double>	operator ()(const AT& params)		const	;
	Matrix<double>	jacobian(const AT& params)		const	;

      private:
	double	_sigma;
    };

    class CostFunction
    {
      public:
	typedef double		value_type;
	typedef Array<Params>	AT;
    
	enum			{D = 2};

	CostFunction(int ndivisions, double range)
	    :_ndivisions(ndivisions), _range(range)			{}
    
	Vector<double>	operator ()(const AT& params)		const	;
	Matrix<double>	jacobian(const AT& params)		const	;
	void		update(AT& params,
			       const Vector<double>& dp)	const	;

      private:
	const int	_ndivisions;
	const double	_range;
    };

  public:
    void	initialize(float sigma)			;
    
  protected:
    GaussianCoefficients(float sigma)			{initialize(sigma);}
    
  protected:
    float	_c0[8];		// forward coefficients for smoothing
    float	_c1[8];		// forward coefficients for 1st derivatives
    float	_c2[8];		// forward coefficients for 2nd derivatives
};
    
/************************************************************************
*  class GaussianConvoler						*
************************************************************************/
//! Gauss$B3K$K$h$k(B1$B<!85G[Ns>v$_9~$_$r9T$&%/%i%9(B
class GaussianConvolver
    : public GaussianCoefficients, private BilateralIIRFilter<4u>
{
  public:
    typedef BilateralIIRFilter<4u>		BIIRF;

    GaussianConvolver(float sigma=1.0)	:GaussianCoefficients(sigma)	{}

    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	smooth(const Array<T1, B1>& in, Array<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diff(const Array<T1, B1>& in, Array<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diff2(const Array<T1, B1>& in, Array<T2, B2>& out)		;
};

//! Gauss$B3K$K$h$k%9%`!<%7%s%0(B
/*!
  \param in	$BF~NO(B1$B<!85G[Ns(B
  \param out	$B=PNO(B1$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2>
inline GaussianConvolver&
GaussianConvolver::smooth(const Array<T1, B1>& in, Array<T2, B2>& out)
{
    BIIRF::initialize(_c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k(B1$B3,HyJ,(B
/*!
  \param in	$BF~NO(B1$B<!85G[Ns(B
  \param out	$B=PNO(B1$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2>
inline GaussianConvolver&
GaussianConvolver::diff(const Array<T1, B1>& in, Array<T2, B2>& out)
{
    BIIRF::initialize(_c1, BIIRF::First).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO(B1$B<!85G[Ns(B
  \param out	$B=PNO(B1$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2>
inline GaussianConvolver&
GaussianConvolver::diff2(const Array<T1, B1>& in, Array<T2, B2>& out)
{
    BIIRF::initialize(_c2, BIIRF::Second).convolve(in, out);

    return *this;
}

/************************************************************************
*  class GaussianConvoler2<BIIRH, BIIRV>				*
************************************************************************/
//! Gauss$B3K$K$h$k(B2$B<!85G[Ns>v$_9~$_$r9T$&%/%i%9(B
template <class BIIRH=BilateralIIRFilter<4u>, class BIIRV=BIIRH>
class GaussianConvolver2
    : public GaussianCoefficients, private BilateralIIRFilter2<BIIRH, BIIRV>
{
  public:
    typedef BilateralIIRFilter<4u>		BIIRF;
    typedef BilateralIIRFilter2<BIIRH, BIIRV>	BIIRF2;

    GaussianConvolver2(float sigma=1.0)	:GaussianCoefficients(sigma) 	{}
    GaussianConvolver2(float sigma, u_int nthreads)
    	:GaussianCoefficients(sigma), BIIRF2(nthreads) 			{}

    template <class T1, class B1, class R1, class T2, class B2, class R2>
    GaussianConvolver2&
	smooth(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    GaussianConvolver2&
	diffH(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    GaussianConvolver2&
	diffV(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    GaussianConvolver2&
	diffHH(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    GaussianConvolver2&
	diffHV(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    GaussianConvolver2&
	diffVV(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    GaussianConvolver2&
	laplacian(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out);

  private:
    Array2<Array<float> >	_tmp;	// buffer for computing Laplacian
};

//! Gauss$B3K$K$h$k%9%`!<%7%s%0(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline GaussianConvolver2<BIIRH, BIIRV>&
GaussianConvolver2<BIIRH, BIIRV>::smooth(const Array2<T1, B1, R1>& in,
					 Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c0, BIIRF::Zeroth,
		       _c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k2#J}8~(B1$B3,HyJ,(B(DOG)
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline GaussianConvolver2<BIIRH, BIIRV>&
GaussianConvolver2<BIIRH, BIIRV>::diffH(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c1, BIIRF::First,
		       _c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k=DJ}8~(B1$B3,HyJ,(B(DOG)
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline GaussianConvolver2<BIIRH, BIIRV>&
GaussianConvolver2<BIIRH, BIIRV>::diffV(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c0, BIIRF::Zeroth,
		       _c1, BIIRF::First).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k2#J}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline GaussianConvolver2<BIIRH, BIIRV>&
GaussianConvolver2<BIIRH, BIIRV>::diffHH(const Array2<T1, B1, R1>& in,
					 Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c2, BIIRF::Second,
		       _c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k=D2#N>J}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline GaussianConvolver2<BIIRH, BIIRV>&
GaussianConvolver2<BIIRH, BIIRV>::diffHV(const Array2<T1, B1, R1>& in,
					 Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c1, BIIRF::First,
		       _c1, BIIRF::First).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k=DJ}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline GaussianConvolver2<BIIRH, BIIRV>&
GaussianConvolver2<BIIRH, BIIRV>::diffVV(const Array2<T1, B1, R1>& in,
					 Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c0, BIIRF::Zeroth,
		       _c2, BIIRF::Second).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k%i%W%i%7%"%s(B(LOG)
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline GaussianConvolver2<BIIRH, BIIRV>&
GaussianConvolver2<BIIRH, BIIRV>::laplacian(const Array2<T1, B1, R1>& in,
					    Array2<T2, B2, R2>& out)
{
    diffHH(in, _tmp).diffVV(in, out);
    out += _tmp;
    
    return *this;
}

}

#endif	/* !__TUGaussianConvolver_h */
