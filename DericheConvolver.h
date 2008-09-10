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
 *  $Id: DericheConvolver.h,v 1.6 2008-09-10 05:10:35 ueshiba Exp $
 */
#ifndef	__TUDericheConvolver_h
#define	__TUDericheConvolver_h

#include "TU/IIRFilter.h"

namespace TU
{
/************************************************************************
*  class DericheCoefficients						*
************************************************************************/
class DericheCoefficients
{
  public:
    void	initialize(float alpha)			;
    
  protected:
    DericheCoefficients(float alpha)			{initialize(alpha);}
    
  protected:
    float	_c0[4];		// forward coefficients for smoothing
    float	_c1[4];		// forward coefficients for 1st derivatives
    float	_c2[4];		// forward coefficients for 2nd derivatives
};

//! Canny-Deriche$B3K$N=i4|2=$r9T$&(B
/*!
  \param alpha	$B%U%#%k%?%5%$%:$rI=$9@5?t!J>.$5$$$[$I9-$,$j$,Bg$-$$!K(B
*/
inline void
DericheCoefficients::initialize(float alpha)
{
    const float	e  = expf(-alpha), beta = sinhf(alpha);
    _c0[0] =  (alpha - 1.0) * e;		// i(n-1)
    _c0[1] =  1.0;				// i(n)
    _c0[2] = -e * e;				// oF(n-2)
    _c0[3] =  2.0 * e;				// oF(n-1)

    _c1[0] = -1.0;				// i(n-1)
    _c1[1] =  0.0;				// i(n)
    _c1[2] = -e * e;				// oF(n-2)
    _c1[3] =  2.0 * e;				// oF(n-1)

    _c2[0] =  (1.0 + beta) * e;			// i(n-1)
    _c2[1] = -1.0;				// i(n)
    _c2[2] = -e * e;				// oF(n-2)
    _c2[3] =  2.0 * e;				// oF(n-1)
}

/************************************************************************
*  class DericheConvoler						*
************************************************************************/
//! Canny-Deriche$B3K$K$h$k(B1$B<!85G[Ns>v$_9~$_$r9T$&%/%i%9(B
class DericheConvolver
    : public DericheCoefficients, private BilateralIIRFilter<2u>
{
  public:
    typedef BilateralIIRFilter<2u>		BIIRF;
    
    DericheConvolver(float alpha=1.0)	:DericheCoefficients(alpha)	{}

    template <class T1, class B1, class T2, class B2> DericheConvolver&
	smooth(const Array<T1, B1>& in, Array<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diff(const Array<T1, B1>& in, Array<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diff2(const Array<T1, B1>& in, Array<T2, B2>& out)		;
};

//! Canny-Deriche$B3K$K$h$k%9%`!<%7%s%0(B
/*!
  \param in	$BF~NO(B1$B<!85G[Ns(B
  \param out	$B=PNO(B1$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2>
inline DericheConvolver&
DericheConvolver::smooth(const Array<T1, B1>& in, Array<T2, B2>& out)
{
    BIIRF::initialize(_c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k(B1$B3,HyJ,(B
/*!
  \param in	$BF~NO(B1$B<!85G[Ns(B
  \param out	$B=PNO(B1$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2>
inline DericheConvolver&
DericheConvolver::diff(const Array<T1, B1>& in, Array<T2, B2>& out)
{
    BIIRF::initialize(_c1, BIIRF::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO(B1$B<!85G[Ns(B
  \param out	$B=PNO(B1$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2>
inline DericheConvolver&
DericheConvolver::diff2(const Array<T1, B1>& in, Array<T2, B2>& out)
{
    BIIRF::initialize(_c2, BIIRF::Second).convolve(in, out);

    return *this;
}

/************************************************************************
*  class DericheConvoler2<BIIRH, BIIRV>					*
************************************************************************/
//! Canny-Deriche$B3K$K$h$k(B2$B<!85G[Ns>v$_9~$_$r9T$&%/%i%9(B
template <class BIIRH=BilateralIIRFilter<2u>, class BIIRV=BIIRH>
class DericheConvolver2
    : public DericheCoefficients, private BilateralIIRFilter2<BIIRH, BIIRV>
{
  public:
    typedef BilateralIIRFilter<2u>		BIIRF;
    typedef BilateralIIRFilter2<BIIRH, BIIRV>	BIIRF2;
    
    DericheConvolver2(float alpha=1.0)	:DericheCoefficients(alpha)	{}
    DericheConvolver2(float alpha, u_int nthreads)
    	:DericheCoefficients(alpha), BIIRF2(nthreads)			{}

    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	smooth(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffH(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffV(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffHH(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffHV(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffVV(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	laplacian(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out);

  private:
    Array2<Array<float> >	_tmp;	// buffer for computing Laplacian
};

//! Canny-Deriche$B3K$K$h$k%9%`!<%7%s%0(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::smooth(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c0, BIIRF::Zeroth,
		       _c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k2#J}8~(B1$B3,HyJ,(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffH(const Array2<T1, B1, R1>& in,
				       Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c1, BIIRF::First,
		       _c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k=DJ}8~(B1$B3,HyJ,(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffV(const Array2<T1, B1, R1>& in,
				       Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c0, BIIRF::Zeroth,
		       _c1, BIIRF::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k2#J}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffHH(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c2, BIIRF::Second,
		       _c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k=D2#N>J}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffHV(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c1, BIIRF::First,
		       _c1, BIIRF::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k=DJ}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffVV(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c0, BIIRF::Zeroth,
		       _c2, BIIRF::Second).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k%i%W%i%7%"%s(B
/*!
  \param in	$BF~NO(B2$B<!85G[Ns(B
  \param out	$B=PNO(B2$B<!85G[Ns(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::laplacian(const Array2<T1, B1, R1>& in,
					   Array2<T2, B2, R2>& out)
{
    diffHH(in, _tmp).diffVV(in, out);
    out += _tmp;
    
    return *this;
}

}

#endif	/* !__TUDericheConvolver_h */
