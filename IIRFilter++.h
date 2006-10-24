/*
 *  $Id: IIRFilter++.h,v 1.3 2006-10-24 08:10:50 ueshiba Exp $
 */
#ifndef __TUIIRFilterPP_h
#define __TUIIRFilterPP_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class IIRFilter							*
************************************************************************/
/*!
  $BJRB&(BInfinite Inpulse Response Filter$B$rI=$9%/%i%9!%(B
*/
template <u_int D> class IIRFilter
{
  public:
    IIRFilter&	initialize(const float c[D+D])				;
    template <class S> const IIRFilter&
		forward(const Array<S>& in, Array<float>& out)	const	;
    template <class S> const IIRFilter&
		backward(const Array<S>& in, Array<float>& out)	const	;
    void	limitsF(float& limit0F,
			float& limit1F, float& limit2F)		const	;
    void	limitsB(float& limit0B,
			float& limit1B, float& limit2B)		const	;
    
  private:
    float	_c[D+D];	// coefficients
};

/************************************************************************
*  class BilateralIIRFilter						*
************************************************************************/
/*!
  $BN>B&(BInfinite Inpulse Response Filter$B$rI=$9%/%i%9!%(B
*/
template <u_int D> class BilateralIIRFilter
{
  public:
  //! $BHyJ,$N3,?t(B
    enum Order
    {
	Zeroth,						//!< 0$B3,HyJ,(B
	First,						//!< 1$B3,HyJ,(B
	Second						//!< 2$B3,HyJ,(B
    };
    
    BilateralIIRFilter&	initialize(const float cF[D+D], const float cB[D+D]);
    BilateralIIRFilter&	initialize(const float c[D+D], Order order)	;
    template <class S>
    BilateralIIRFilter&	convolve(const Array<S>& in)			;
    u_int		dim()					const	;
    float		operator [](int i)			const	;
    void		limits(float& limit0,
			       float& limit1,
			       float& limit2)			const	;
    
  private:
    IIRFilter<D>	_iirF;
    Array<float>	_bufF;
    IIRFilter<D>	_iirB;
    Array<float>	_bufB;
};

//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param cF	$BA0?J(Bz$BJQ4978?t(B. z$BJQ49$O(B 
		\f[
		  H^F(z^{-1}) = \frac{c^F_{D-1} + c^F_{D-2}z^{-1}
		  + c^F_{D-3}z^{-2} + \cdots
		  + c^F_{0}z^{-(D-1)}}{1 - c^F_{2D-1}z^{-1}
		  - c^F_{2D-2}z^{-2} - \cdots - c^F_{D}z^{-D}}
		\f]
		$B$H$J$k(B. 
  \param cB	$B8eB`(Bz$BJQ4978?t(B. z$BJQ49$O(B
		\f[
		  H^B(z) = \frac{c^B_{0}z + c^B_{1}z^2 + \cdots + c^B_{D-1}*z^D}
		       {1 - c^B_{D}z - c^B_{D+1}z^2 - \cdots - c^B_{2D-1}*z^D}
		\f]
		$B$H$J$k(B.
*/
template <u_int D> inline BilateralIIRFilter<D>&
BilateralIIRFilter<D>::initialize(const float cF[D+D], const float cB[D+D])
{
    _iirF.initialize(cF);
    _iirB.initialize(cB);
#ifdef DEBUG
    float	limit0, limit1, limit2;
    limits(limit0, limit1, limit2);
    std::cerr << "limit0 = " << limit0 << ", limit1 = " << limit1
	      << ", limit2 = " << limit2 << std::endl;
#endif
    return *this;
}

//! $B%U%#%k%?$K$h$k>v$_9~$_$r9T$&(B. $B=PNO$O(B operator [](int) $B$G<h$j=P$9(B
/*!
  \param in	$BF~NO%G!<%?Ns(B.
  return	$B$3$N%U%#%k%?<+?H(B.
*/
template <u_int D> template <class S> inline BilateralIIRFilter<D>&
BilateralIIRFilter<D>::convolve(const Array<S>& in)
{
    _iirF.forward(in, _bufF);
    _iirB.backward(in, _bufB);

    return *this;
}

//! $B>v$_9~$_$N=PNO%G!<%?Ns$N<!85$rJV$9(B
/*!
  \return	$B=PNO%G!<%?Ns$N<!85(B.
*/
template <u_int D> inline u_int
BilateralIIRFilter<D>::dim() const
{
    return _bufF.dim();
}

//! $B>v$_9~$_$N=PNO%G!<%?$NFCDj$NMWAG$rJV$9(B
/*!
  \param i	$BMWAG$N(Bindex.
  \return	$BMWAG$NCM(B.
*/
template <u_int D> inline float
BilateralIIRFilter<D>::operator [](int i) const
{
    return _bufF[i] + _bufB[i];
}

/************************************************************************
*  class BilateralIIRFilter2						*
************************************************************************/
/*!
  2$B<!85N>B&(BInfinite Inpulse Response Filter$B$rI=$9%/%i%9!%(B
*/
template <u_int D> class BilateralIIRFilter2
{
  public:
    typedef typename BilateralIIRFilter<D>::Order	Order;
    
    BilateralIIRFilter2&
		initialize(float cHF[D+D], float cHB[D+D],
			   float cVF[D+D], float cVB[D+D])		;
    BilateralIIRFilter2&
		initialize(float cHF[D+D], Order orderH,
			   float cVF[D+D], Order orderV)		;
    template <class S, class T> BilateralIIRFilter2&
		convolve(const Image<S>& in, Image<T>& out)		;
    
  private:
    BilateralIIRFilter<D>	_iirH;
    BilateralIIRFilter<D>	_iirV;
    Array2<Array<float> >	_buf;
};
    
//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param cHF	$B2#J}8~A0?J(Bz$BJQ4978?t(B.
  \param cHB	$B2#J}8~8eB`(Bz$BJQ4978?t(B.
  \param cHV	$B=DJ}8~A0?J(Bz$BJQ4978?t(B.
  \param cHV	$B=DJ}8~8eB`(Bz$BJQ4978?t(B.
  \return	$B$3$N%U%#%k%?<+?H(B.
*/
template <u_int D> inline BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::initialize(float cHF[D+D], float cHB[D+D],
				   float cVF[D+D], float cVB[D+D])
{
    _iirH.initialize(cHF, cHB);
    _iirV.initialize(cVF, cVB);

    return *this;
}

//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param cHF	$B2#J}8~A0?J(Bz$BJQ4978?t(B.
  \param orderH $B2#J}8~HyJ,3,?t(B.
  \param cHV	$B=DJ}8~A0?J(Bz$BJQ4978?t(B.
  \param orderV	$B=DJ}8~HyJ,3,?t(B.
  \return	$B$3$N%U%#%k%?<+?H(B.
*/
template <u_int D> inline BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::initialize(float cHF[D+D], Order orderH,
				   float cVF[D+D], Order orderV)
{
    _iirH.initialize(cHF, orderH);
    _iirV.initialize(cVF, orderV);

    return *this;
}

/************************************************************************
*  class DericheConvoler						*
************************************************************************/
/*!
  Canny-Deriche$B3K$K$h$k%9%`!<%7%s%0!$(B1$B<!HyJ,$*$h$S(B2$B<!HyJ,$r4^$`(B
  $B2hA|>v$_9~$_$r9T$&%/%i%9!%(B
*/
class DericheConvolver : private BilateralIIRFilter2<2u>
{
  public:
    using	BilateralIIRFilter2<2u>::Order;
    
    DericheConvolver(float alpha=1.0)		{initialize(alpha);}

    DericheConvolver&	initialize(float alpha)				;
    template <class S, class T>
    DericheConvolver&	smooth(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    DericheConvolver&	gradH(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    DericheConvolver&	gradV(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    DericheConvolver&	laplacian(const Image<S>& in, Image<T>& out)	;

  private:
    float		_c0[4];	// forward coefficients for smoothing
    float		_c1[4];	// forward coefficients for 1st derivatives
    float		_c2[4];	// forward coefficients for 2nd derivatives
    Image<float>	_tmp;	// buffer for storing intermediate values
};

//! Canny-Deriche$B3K$K$h$k%9%`!<%7%s%0(B
/*!
  \param in	$BF~NO2hA|(B.
  \param out	$B=PNO2hA|(B.
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B.
*/
template <class S, class T> inline DericheConvolver&
DericheConvolver::smooth(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k2#J}8~(B1$B<!HyJ,(B
/*!
  \param in	$BF~NO2hA|(B.
  \param out	$B=PNO2hA|(B.
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B.
*/
template <class S, class T> inline DericheConvolver&
DericheConvolver::gradH(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c1, BilateralIIRFilter<2u>::First,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k=DJ}8~(B1$B<!HyJ,(B
/*!
  \param in	$BF~NO2hA|(B.
  \param out	$B=PNO2hA|(B.
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B.
*/
template <class S, class T> inline DericheConvolver&
DericheConvolver::gradV(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c1, BilateralIIRFilter<2u>::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k%i%W%i%7%"%s(B
/*!
  \param in	$BF~NO2hA|(B.
  \param out	$B=PNO2hA|(B.
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B.
*/
template <class S, class T> inline DericheConvolver&
DericheConvolver::laplacian(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c2, BilateralIIRFilter<2u>::Second,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, _tmp).
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c2, BilateralIIRFilter<2u>::Second).convolve(in, out);
    out += _tmp;
    
    return *this;
}

/************************************************************************
*  class GaussianConvoler						*
************************************************************************/
/*!
  Gauss$B3K$K$h$k%9%`!<%7%s%0!$(B1$B<!HyJ,(B(DOG)$B$*$h$S(B2$B<!HyJ,(B(LOG)$B$r4^$`(B
  $B2hA|>v$_9~$_$r9T$&%/%i%9!%(B
*/
class GaussianConvolver : private BilateralIIRFilter2<4u>
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
	typedef double		T;
	typedef Array<Params>	AT;

	EvenConstraint(T sigma) :_sigma(sigma)				{}
	
	Vector<T>	operator ()(const AT& params)		const	;
	Matrix<T>	jacobian(const AT& params)		const	;

      private:
	T		_sigma;
    };

    class CostFunction
    {
      public:
	typedef double		T;
	typedef Array<Params>	AT;
    
	enum			{D = 2};

	CostFunction(int ndivisions, T range)
	    :_ndivisions(ndivisions), _range(range)			{}
    
	Vector<T>	operator ()(const AT& params)		const	;
	Matrix<T>	jacobian(const AT& params)		const	;
	void		update(AT& params, const Vector<T>& dp)	const	;

      private:
	const int	_ndivisions;
	const T		_range;
    };

  public:
    GaussianConvolver(float sigma=1.0)		{initialize(sigma);}

    GaussianConvolver&	initialize(float sigma)				;
    template <class S, class T>
    GaussianConvolver&	smooth(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    GaussianConvolver&	gradH(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    GaussianConvolver&	gradV(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    GaussianConvolver&	laplacian(const Image<S>& in, Image<T>& out)	;

  private:
    float		_c0[8];	// forward coefficients for smoothing
    float		_c1[8];	// forward coefficients for 1st derivatives
    float		_c2[8];	// forward coefficients for 2nd derivatives
    Image<float>	_tmp;	// buffer for storing intermediate values
};

//! Gauss$B3K$K$h$k%9%`!<%7%s%0(B
/*!
  \param in	$BF~NO2hA|(B.
  \param out	$B=PNO2hA|(B.
  \return	$B$3$N(BGauss$B3K<+?H(B.
*/
template <class S, class T> inline GaussianConvolver&
GaussianConvolver::smooth(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k2#J}8~(B1$B<!HyJ,(B(DOG)
/*!
  \param in	$BF~NO2hA|(B.
  \param out	$B=PNO2hA|(B.
  \return	$B$3$N(BGauss$B3K<+?H(B.
*/
template <class S, class T> inline GaussianConvolver&
GaussianConvolver::gradH(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c1, BilateralIIRFilter<4u>::First,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k=DJ}8~(B1$B<!HyJ,(B(DOG)
/*!
  \param in	$BF~NO2hA|(B.
  \param out	$B=PNO2hA|(B.
  \return	$B$3$N(BGauss$B3K<+?H(B.
*/
template <class S, class T> inline GaussianConvolver&
GaussianConvolver::gradV(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c1, BilateralIIRFilter<4u>::First).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k%i%W%i%7%"%s(B(LOG)
/*!
  \param in	$BF~NO2hA|(B.
  \param out	$B=PNO2hA|(B.
  \return	$B$3$N(BGauss$B3K<+?H(B.
*/
template <class S, class T> inline GaussianConvolver&
GaussianConvolver::laplacian(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c2, BilateralIIRFilter<4u>::Second,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, _tmp).
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c2, BilateralIIRFilter<4u>::Second).convolve(in, out);
    out += _tmp;
    
    return *this;
}

}

#endif	// !__TUIIRFilterPP_h
