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
 *  $Id: Minimize.h,v 1.4 2008-10-03 04:23:37 ueshiba Exp $
 */
#ifndef __TUMinimize_h
#define __TUMinimize_h

#include "TU/Vector++.h"
#include <algorithm>
#include <stdexcept>

namespace TU
{
/************************************************************************
*  class NullConstraint							*
************************************************************************/
//! $B2?$N94B+$bM?$($J$$$H$$$&%@%_!<$N94B+>r7o$rI=$9%/%i%9(B
/*!
  $B<BBN$OG$0U$N0z?t$KBP$7$F(B0$B<!85$N%Y%/%H%k$r=PNO$9$k%Y%/%H%kCM4X?t$G$"$j!$(B
  #minimizeSquare()$B$d(B#minimizeSquareSparce()$B$N%F%s%W%l!<%H%Q%i%a!<%?(BG$B$H$7$F(B
  $BMxMQ$9$k$3$H$rA[Dj$7$F$$$k!%(B
  \param ET $B=PNO%Y%/%H%k$NMWAG$N7?(B
*/
template <class ET>
class NullConstraint
{
  public:
  //! $BG$0U$N0z?t$KBP$7$F(B0$B<!85%Y%/%H%k$r=PNO$9$k!%(B
    template <class AT>
    Vector<ET>	operator ()(const AT&)	const	{return Vector<ET>(0);}
  //! $BG$0U$N0z?t$KBP$7$F(B0x0$B9TNs$r=PNO$9$k!%(B
    template <class AT>
    Matrix<ET>	jacobian(const AT&)	const	{return Matrix<ET>(0, 0);}
};

/************************************************************************
*  class ConstNormConstraint						*
************************************************************************/
//! $B0z?t$N(B2$B>h%N%k%`CM$,0lDj$H$$$&94B+>r7o$rI=$9%/%i%9(B
/*!
  $B<BBN$OM?$($i$l$?0z?t$N(B2$B>h%N%k%`CM$HL\I8CM$H$N:9$r(B1$B<!85%Y%/%H%k$H$7$FJV$9%Y%/%H%kCM(B
  $B4X?t$G$"$j!$(B#minimizeSquare()$B$d(B#minimizeSquareSparce()$B$N%F%s%W%l!<%H%Q%i%a!<%?(BG
  $B$H$7$FMxMQ$9$k$3$H$rA[Dj$7$F$$$k!%(B
  \param AT	$B0z?t$N7?!%0J2<$N>r7o$rK~$?$9$3$H!'(B
  \verbatim
  1. $B%Y%/%H%k$d9TNs$G$"$k>l9g!$$=$NMWAG$N7?$r(BAT::value_type$B$H$$$&L>A0$G(Btypedef$B$7$F$$$k!%(B
  2. $B%a%s%P4X?t(B
	AT::value_type	AT::square() const
     $B$K$h$C$F!$$=$N(B2$B>h%N%k%`CM$rCN$k$3$H$,$G$-$k!%(B
  3. Vector<AT::value_type>$B7?$KJQ49$G$-$k(B($BNc!'(BMatrix<AT::value_type>$B7?$O(B
     $B$=$NMWAG$r9TM%@h=g$K(B1$BNs$KJB$Y$?%Y%/%H%k$KJQ492DG=(B)$B!%(B
  \endverbatim  
*/
template <class AT>
class ConstNormConstraint
{
  private:
    typedef typename AT::value_type	ET;
    
  public:
  //! $B?7$?$J94B+>r7o$r@8@.$7!$$=$N(B2$B>h%N%k%`$NL\I8CM$r@_Dj$9$k!%(B
  /*!
    \param x	$B0z?t(B($B$3$N(B2$B>h%N%k%`CM$,L\I8CM$H$J$k(B)
  */
    ConstNormConstraint(const AT& x) :_sqr(x.square())			{}

  //! $BM?$($i$l$?0z?t$N(B2$B>h%N%k%`CM$HL\I8CM$N:9$r=PNO$9$k!%(B
  /*!
    \param x	$B0z?t(B
    \return	x$B$N(B2$B>h%N%k%`CM$HL\I8CM$N:9$r<}$a$?(B1$B<!85%Y%/%H%k(B
  */
    Vector<ET>	operator ()(const AT& x) const
		{
		    Vector<ET>	val(1);
		    val[0] = x.square() - _sqr;
		    return val;
		}

  //! $BM?$($i$l$?0z?t$N(B2$B>h%N%k%`CM$K$D$$$F!$$3$N0z?t<+?H$K$h$k(B1$B3,HyJ,CM$r=PNO$9$k!%(B
  /*!
    \param x	$B0z?t(B
    \return	1$B3,HyJ,CM$r<}$a$?(B1xd$B9TNs(B(d$B$O%Y%/%H%k2=$5$l$?0z?t$N<!85(B)
  */
    Matrix<ET>	jacobian(const AT& x) const
		{
		    const Vector<ET>	y(x);
		    Matrix<ET>		L(1, y.dim());
		    (L[0] = y) *= 2.0;
		    return L;
		}
	    
  private:
    const ET	_sqr;
};

/************************************************************************
*  function minimizeSquare						*
*    -- Compute x st. ||f(x)||^2 -> min under g(x) = 0.			*
************************************************************************/
//! $BM?$($i$l$?%Y%/%H%kCM4X?t$N(B2$B>h%N%k%`$rM?$($i$l$?94B+>r7o$N2<$G:G>.2=$9$k0z?t$r5a$a$k!%(B
/*!
  $BK\4X?t$O!$(B2$B$D$N%Y%/%H%k4X?t(B\f$\TUvec{f}{}(\TUvec{x}{})\f$,
  \f$\TUvec{g}{}(\TUvec{x}{})\f$$B$*$h$S=i4|CM(B\f$\TUvec{x}{0}\f$$B$,M?$($i$l$?$H$-!$(B
  \f$\TUvec{g}{}(\TUvec{x}{}) = \TUvec{0}{}\f$$B$J$k94B+$N$b$H$G(B
  \f$\TUnorm{\TUvec{f}{}(\TUvec{x}{})}^2 \rightarrow \min\f$$B$H$9$k(B
  \f$\TUvec{x}{}\f$$B$r5a$a$k!%(B
  
  $B%F%s%W%l!<%H%Q%i%a!<%?(BAT$B$O!$%Y%/%H%kCM4X?t$*$h$S94B+>r7o4X?t$N0z?t$rI=$97?$G$"$j!$(B
  $B0J2<$N>r7o$rK~$?$9$3$H!'(B
  \verbatim
  $B0z?t$,%Y%/%H%k$d9TNs$G$"$k>l9g!$$=$NMWAG$N7?$r(BAT::value_type$B$H$$$&L>A0$G(Btypedef$B$7$F$$$k!%(B
  \endverbatim  
  $B%F%s%W%l!<%H%Q%i%a!<%?(BF$B$O!$(BAT$B7?$N0z?t$rF~NO$7$F%Y%/%H%kCM$r=PNO$9$k4X?t$rI=$97?$G$"$j!$(B
  $B0J2<$N>r7o$rK~$?$9$3$H!'(B
  \verbatim
  1. $B=PNO%Y%/%H%k$NMWAG$N7?$r(BF::value_type$B$H$$$&L>A0$G(Btypedef$B$7$F$$$k!%(B
  2. $B0z?t(Bx$B$rM?$($?$H$-$N4X?tCM$O!$%a%s%P4X?t(B
	Vector<F:value_type>	F::operator ()(const AT& x) const
     $B$K$h$C$FM?$($i$l$k!%(B
  3. $B0z?t(Bx$B$rM?$($?$H$-$N%d%3%S%"%s$O!$%a%s%P4X?t(B
	Matrix<F:value_type>	F::jacobian(const AT& x) const
     $B$K$h$C$FM?$($i$l$k!%(B
  4. $B%a%s%P4X?t(B
	void	F::update(const AT& x, const Vector<F::value_type>& dx) const
     $B$K$h$C$F0z?t(Bx$B$rHy>/NL(Bdx$B$@$199?7$9$k$3$H$,$G$-$k!%(B
  \endverbatim
  $B%F%s%W%l!<%H%Q%i%a!<%?(BG$B$O!$(BAT$B7?$N0z?t$rF~NO$7$F%Y%/%H%kCM$r=PNO$9$k4X?t$rI=$97?$G$"$j!$(B
  $B0J2<$N>r7o$rK~$?$9$3$H!'(B
  \verbatim
  1. $B=PNO%Y%/%H%k$NMWAG$N7?$r(BG::value_type$B$H$$$&L>A0$G(Btypedef$B$7$F$$$k!%(B
  2. $B0z?t(Bx$B$rM?$($?$H$-$N4X?tCM$O!$%a%s%P4X?t(B
	Vector<G:value_type>	G::operator ()(const AT& x) const
     $B$K$h$C$FM?$($i$l$k!%(B
  3. $B0z?t(Bx$B$rM?$($?$H$-$N%d%3%S%"%s$O!$%a%s%P4X?t(B
	Matrix<G::value_type>	G::jacobian(const AT& x) const
     $B$K$h$C$FM?$($i$l$k!%(B
  \endverbatim
  \param f		$B$=$N(B2$B>h%N%k%`$r:G>.2=$9$Y$-%Y%/%H%kCM4X?t(B
  \param g		$B94B+>r7o$rI=$9%Y%/%H%kCM4X?t(B
  \param x		$B=i4|CM$rM?$($k$H!$(Bg$B$,Nm%Y%/%H%k$H$J$k$H$$$&94B+>r7o$N2<$G(B
			f$B$N(B2$B>h%N%k%`$r:G>.2=$9$k0z?t$NCM$,JV$5$l$k!%(B
  \param niter_max	$B:GBg7+$jJV$72s?t(B
  \param tol		$B<}B+H=Dj>r7o$rI=$9ogCM(B($B99?7NL$,$3$NCM0J2<$K$J$l$P<}B+$H8+$J$9(B)
  \return		x$B$N?dDjCM$N6&J,;69TNs(B
*/
template <class F, class G, class AT> Matrix<typename F::value_type>
minimizeSquare(const F& f, const G& g, AT& x,
	       int niter_max=100, double tol=1.5e-8)
{
    using namespace			std;
    typedef typename F::value_type	ET;	// element type.

    Vector<ET>	fval   = f(x);			// function value.
    ET		sqr    = fval * fval;		// square value.
    ET		lambda = 1.0e-4;		// L-M parameter.

    for (int n = 0; n++ < niter_max; )
    {
	const Matrix<ET>&	J    = f.jacobian(x);	// Jacobian.
	const Vector<ET>&	Jtf  = fval * J;
	const Vector<ET>&	gval = g(x);		// constraint residual.
	const u_int		xdim = J.ncol(), gdim = gval.dim();
	Matrix<ET>		A(xdim + gdim, xdim + gdim);

	A(0, 0, xdim, xdim) = J.trns() * J;
	A(xdim, 0, gdim, xdim) = g.jacobian(x);
	A(0, xdim, xdim, gdim) = A(xdim, 0, gdim, xdim).trns();

	Vector<ET>		diagA(xdim);
	for (int i = 0; i < xdim; ++i)
	    diagA[i] = A[i][i];			// Keep diagonal elements.

	for (;;)
	{
	  // Compute dx: update for parameters x to be estimated.
	    for (int i = 0; i < xdim; ++i)
		A[i][i] = (1.0 + lambda) * diagA[i];	// Augument diagonals.
	    Vector<ET>	dx(xdim + gdim);
	    dx(0, xdim) = Jtf;
	    dx(xdim, gdim) = gval;
	    dx.solve(A);

	  // Compute updated parameters and function value to it.
	    AT			x_new(x);
	    f.update(x_new, dx(0, xdim));
	    const Vector<ET>&	fval_new = f(x_new);
	    const ET		sqr_new  = fval_new * fval_new;
#ifdef TUMinimizePP_DEBUG
	    cerr << "val^2 = " << sqr << ", gval = " << gval
		 << "  (update: val^2 = " << sqr_new
		 << ", lambda = " << lambda << ")" << endl;
#endif
	    if (fabs(sqr_new - sqr) <=
		tol * (fabs(sqr_new) + fabs(sqr) + 1.0e-10))
	    {
		for (int i = 0; i < xdim; ++i)
		    A[i][i] = diagA[i];
		return A(0, 0, xdim, xdim).pinv(1.0e8);
	    }

	    if (sqr_new < sqr)
	    {
		x	= x_new;		// Update parameters.
		fval	= fval_new;		// Update function value.
		sqr	= sqr_new;		// Update square value.
		lambda *= 0.1;			// Decrease L-M parameter.
		break;
	    }
	    else
		lambda *= 10.0;			// Increase L-M parameter.
	}
    }
    throw std::runtime_error("minimizeSquare: maximum iteration limit exceeded!");
    return Matrix<ET>(0, 0);
}

/************************************************************************
*  function minimizeSquareSparse					*
*    -- Compute a and b st. sum||f(a, b[j])||^2 -> min under g(a) = 0.	*
************************************************************************/
//! $BM?$($i$l$?%Y%/%H%kCM4X?t$N(B2$B>h%N%k%`$rM?$($i$l$?94B+>r7o$N2<$G:G>.2=$9$k0z?t$r5a$a$k!%(B
/*!
  $BK\4X?t$O!$(B\f$\TUvec{x}{} = [\TUtvec{a}{}, \TUtvec{b}{1},
  \TUtvec{b}{2}, \ldots, \TUtvec{b}{J}]^\top\f$$B$rF~NO$H$9$k(B2$B$D$N%Y%/%H(B
  $B%k4X?t(B\f$\TUvec{f}{}(\TUvec{x}{}) = [\TUtvec{f}{1}(\TUvec{a}{},
  \TUvec{b}{1}), \TUtvec{f}{2}(\TUvec{a}{}, \TUvec{b}{2}),\ldots,
  \TUtvec{f}{J}(\TUvec{a}{}, \TUvec{b}{J})]^\top\f$,
  \f$\TUvec{g}{}(\TUvec{x}{})\f$$B$*$h$S=i4|CM(B\f$\TUvec{x}{0}\f$$B$,M?$($i(B
  $B$l$?$H$-!$(B\f$\TUvec{g}{}(\TUvec{x}{}) = \TUvec{0}{}\f$$B$J$k94B+$N$b$H(B
  $B$G(B\f$\TUnorm{\TUvec{f}{}(\TUvec{x}{})}^2 \rightarrow \min\f$$B$H$9$k(B
  \f$\TUvec{x}{}\f$$B$r5a$a$k!%8D!9$N(B\f$\TUvec{f}{j}(\cdot)\f$$B$O(B
  \f$\TUvec{a}{}\f$$B$H(B\f$\TUvec{b}{j}\f$$B$N$_$K0MB8$7!$(B
  \f$\TUvec{g}{}(\cdot)\f$$B$O(B\f$\TUvec{a}{}\f$$B$N$_$K0MB8$9$k(B($B$9$J$o$A(B
  \f$\TUvec{g}{}(\TUvec{x}{}) = \TUvec{g}{}(\TUvec{a}{})\f$)$B$b$N$H$9$k!%(B
  
  $B%F%s%W%l!<%H%Q%i%a!<%?(BATA$B$O!$%Y%/%H%kCM4X?t(Bf$B$NBh(B1$B0z?t$*$h$S94B+>r7o4X?t(Bg$B$N0z?t(Ba$B$r(B
  $BI=$97?$G$"$j!$0J2<$N>r7o$rK~$?$9$3$H!'(B
  \verbatim
  $B0z?t$,%Y%/%H%k$d9TNs$G$"$k>l9g!$$=$NMWAG$N7?$r(BATA::value_type$B$H$$$&L>A0$G(Btypedef$B$7$F$$$k!%(B
  \endverbatim  
  $B%F%s%W%l!<%H%Q%i%a!<%?(BIB$B$O!$8D!9$N%Y%/%H%kCM4X?t(Bf_j$B$NBh(B2$B0z?t(Bb_j$B$r;X$9H?I|;R$rI=$97?(B
  $B$G$"$j!$0J2<$N>r7o$rK~$?$9$3$H!'(B
  \verbatim
  iterator_traits<IB>::value_type$B$G$3$NH?I|;R$,;X$90z?t$N7?(B($B0J2<!$(BATB$B$H$9$k(B)$B$rCN$k$3$H$,$G$-$k!%(B
  \endverbatim  
  $B%F%s%W%l!<%H%Q%i%a!<%?(BF$B$O!$(BATA$B7?$N0z?t(Ba$B$H(BATB$B7?$N0z?t(Bb_j$B$rF~NO$7$F%Y%/%H%kCM$r=PNO$9$k(B
  $B4X?t$rI=$97?$G$"$j!$0J2<$N>r7o$rK~$?$9$3$H!'(B
  \verbatim
  1. $B=PNO%Y%/%H%k$NMWAG$N7?$r(BF::value_type$B$H$$$&L>A0$G(Btypedef$B$7$F$$$k!%(B
  2. $B%d%3%S%"%s$N7?$r(BF::jacobian_type$B$H$$$&L>A0$G(Btypedef$B$7$F$$$k!%(B
  3. ATA$B7?$N0z?t(Ba$B$,;}$D<+M3EY$r(B
	u_int	F::adim() const
     $B$K$h$C$FCN$k$3$H$,$G$-$k!%(B
  4. $B0z?t(Ba$B$r(Ba_1, a_2,..., a_I$B$KJ,3d$7$?>l9g$N3F(Ba_i$B$,;}$D<+M3EY$r(B
	const Array<u_int>&	F::adims() const;
     $B$K$h$C$FCN$k$3$H$,$G$-$k!%$3$NG[Ns$NMWAG$NAmOB$O(BF::adim()$B$KEy$7$$!%(Ba$B$,J,3d$G$-$J$$>l9gD9$5(B1$B$NG[Ns$,JV$5$l!$$=$NM#0l$NMWAG$NCM$O(BF::adim()$B$KEy$7$$!%(B
  5. $B0z?t(Ba, b_j$B$rM?$($?$H$-$N(Bf_j$B$N4X?tCM$O!$%a%s%P4X?t(B
	Vector<F:value_type>	F::operator ()(const ATA& a, const ATB& b, int j) const
     $B$K$h$C$FM?$($i$l$k!%(B
  6. $B0z?t(Ba, b_j$B$rM?$($?$H$-$N(Ba$B$GHyJ,$7$?%d%3%S%"%s$O!$%a%s%P4X?t(B
	F::jacobian_type	F::jacobianA(const ATA& a, const ATB& b, int j) const
     $B$K$h$C$FM?$($i$l$k!%(B
  7. $B%a%s%P4X?t(B
	void	F::updateA(const ATA& a, const Vector<F::value_type>& da) const
     $B$K$h$C$F0z?t(Ba$B$rHy>/NL(Bda$B$@$199?7$9$k$3$H$,$G$-$k!%(B
  8. $B%a%s%P4X?t(B
	void	F::updateB(const ATB& b_j, const Vector<F::value_type>& db_j) const
     $B$K$h$C$F0z?t(Bb$B$rHy>/NL(Bdb_j$B$@$199?7$9$k$3$H$,$G$-$k!%(B
  \endverbatim
  $B%F%s%W%l!<%H%Q%i%a!<%?(BG$B$O!$(BATA$B7?$N0z?t$rF~NO$7$F%Y%/%H%kCM$r=PNO$9$k4X?t$rI=$97?$G$"$j!$(B
  $B0J2<$N>r7o$rK~$?$9$3$H!'(B
  \verbatim
  1. $B=PNO%Y%/%H%k$NMWAG$N7?$r(BG::value_type$B$H$$$&L>A0$G(Btypedef$B$7$F$$$k!%(B
  2. $B0z?t(Ba$B$rM?$($?$H$-$N4X?tCM$O!$%a%s%P4X?t(B
	Vector<G:value_type>	G::operator ()(const ATA& a) const
     $B$K$h$C$FM?$($i$l$k!%(B
  3. $B0z?t(Ba$B$rM?$($?$H$-$N%d%3%S%"%s$O!$%a%s%P4X?t(B
	Matrix<G::value_type>	G::jacobian(const ATA& a) const
     $B$K$h$C$FM?$($i$l$k!%(B
  \endverbatim
  \param f		$B$=$N(B2$B>h%N%k%`$r:G>.2=$9$Y$-%Y%/%H%kCM4X?t(B
  \param g		$B94B+>r7o$rI=$9%Y%/%H%kCM4X?t(B
  \param a		$B3F(Bf_j$B$NBh(B1$B0z?t$G$"$j!$$+$D(Bg$B$N0z?t!%=i4|CM$rM?$($k$H:GE,2r$,(B
			$BJV$5$l$k!%(B
  \param bbegin		$B3F(Bf_j$B$KM?$($kBh(B2$B0z?t$NJB$S$N@hF,$r;X$9H?I|;R(B
  \param bend		$B3F(Bf_j$B$KM?$($kBh(B2$B0z?t$NJB$S$NKvHx$N<!$r;X$9H?I|;R(B
  \param niter_max	$B:GBg7+$jJV$72s?t(B
  \param tol		$B<}B+H=Dj>r7o$rI=$9ogCM(B($B99?7NL$,$3$NCM0J2<$K$J$l$P<}B+$H8+$J$9(B)
  \return		a, b_1, b_2,..., b_J$B$N?dDjCM$N6&J,;69TNs(B
*/
template <class F, class G, class ATA, class IB> Matrix<typename F::value_type>
minimizeSquareSparse(const F& f, const G& g, ATA& a, IB bbegin, IB bend,
		     int niter_max=100, double tol=1.5e-8)
{
    using namespace					std;
    typedef typename F::value_type			ET;  // element type.
    typedef typename F::jacobian_type			JT;  // Jacobian type.
    typedef typename iterator_traits<IB>::value_type	ATB; // arg. b type.
    
    const u_int			nb = distance(bbegin, bend);
    Array<Vector<ET> >		fval(nb);	// function values.
    ET				sqr = 0;	// sum of squares.
    int				j = 0;
    for (IB b = bbegin; b != bend; ++b, ++j)
    {
	fval[j] = f(a, *b, j);
	sqr    += fval[j] * fval[j];
    }
    ET	lambda = 1.0e-7;			// L-M parameter.

    for (int n = 0; n++ < niter_max; )
    {
	const u_int		adim = f.adim();
	JT			U(f.adims(), f.adims());
	Vector<ET>		Jtf(adim);
	Array<Matrix<ET> >	V(nb);
	Array<Matrix<ET> >	W(nb);
	Array<Vector<ET> >	Ktf(nb);
	j = 0;
	for (IB b = bbegin; b != bend; ++b, ++j)
	{
	    const JT&		J  = f.jacobianA(a, *b, j);
	    const JT&		Jt = J.trns();
	    const Matrix<ET>&	K  = f.jacobianB(a, *b, j);

	    U     += Jt * J;
	    Jtf   += fval[j] * J;
	    V[j]   = K.trns() * K;
	    W[j]   = Jt * K;
	    Ktf[j] = fval[j] * K;
	}

      	const Vector<ET>&	gval = g(a);
	const u_int		gdim = gval.dim();
	Matrix<ET>		A(adim + gdim, adim + gdim);
	
	A(adim, 0, gdim, adim) = g.jacobian(a);
	A(0, adim, adim, gdim) = A(adim, 0, gdim, adim).trns();

	for (;;)
	{
	  // Compute da: update for parameters a to be estimated.
	    A(0, 0, adim, adim) = U;
	    for (int i = 0; i < adim; ++i)
		A[i][i] *= (1.0 + lambda);		// Augument diagonals.

	    Vector<ET>		da(adim + gdim);
	    da(0, adim) = Jtf;
	    da(adim, gdim) = gval;
	    Array<Matrix<ET> >	VinvWt(nb);
	    Array<Vector<ET> >	VinvKtf(nb);
	    for (int j = 0; j < nb; ++j)
	    {
		Matrix<ET>	Vinv = V[j];
		for (int k = 0; k < Vinv.dim(); ++k)
		    Vinv[k][k] *= (1.0 + lambda);	// Augument diagonals.
		Vinv = Vinv.inv();
		VinvWt[j]  = Vinv * W[j].trns();
		VinvKtf[j] = Vinv * Ktf[j];
		A(0, 0, adim, adim) -= W[j] * VinvWt[j];
		da(0, adim) -= W[j] * VinvKtf[j];
	    }
	    da.solve(A);

	  // Compute updated parameters and function value to it.
	    ATA			a_new(a);
	    f.updateA(a_new, da(0, adim));
	    Array<ATB>		b_new(nb);
	    copy(bbegin, bend, b_new.begin());
	    Array<Vector<ET> >	fval_new(nb);
	    ET			sqr_new = 0;
	    for (int j = 0; j < nb; ++j)
	    {
		const Vector<ET>& db = VinvKtf[j] - VinvWt[j] * da(0, adim);
		f.updateB(b_new[j], db);
		fval_new[j] = f(a_new, b_new[j], j);
		sqr_new	   += fval_new[j] * fval_new[j];
	    }
#ifdef TUMinimizePP_DEBUG
	    cerr << "val^2 = " << sqr << ", gval = " << gval
		 << "  (update: val^2 = " << sqr_new
		 << ", lambda = " << lambda << ")" << endl;
#endif
	    if (fabs(sqr_new - sqr) <=
		tol * (fabs(sqr_new) + fabs(sqr) + 1.0e-10))
	    {
		u_int		bdim = 0;
		for (int j = 0; j < nb; ++j)
		    bdim += V[j].dim();
		Matrix<ET>	S(adim + bdim, adim + bdim);
		Matrix<ET>	Sa(S, 0, 0, adim, adim);
		Sa = U;
		for (int j = 0; j < nb; ++j)
		{
		    VinvWt[j] = V[j].inv() * W[j].trns();
		    Sa -= W[j] * VinvWt[j];
		}
		for (int jj = adim, j = 0; j < nb; ++j)
		{
		    const Matrix<ET>&	VinvWtSa = VinvWt[j] * Sa;
		    for (int kk = adim, k = 0; k <= j; ++k)
		    {
			S(jj, kk, VinvWtSa.nrow(), VinvWt[k].nrow())
			     = VinvWtSa * VinvWt[k].trns();
			kk += VinvWt[k].nrow();
		    }
		    S(jj, jj, V[j].nrow(), V[j].nrow()) += V[j].inv();
		    jj += VinvWt[j].nrow();
		}
		Sa = Sa.pinv(1.0e8);
		for (int jj = adim, j = 0; j < nb; ++j)
		{
		    S(jj, 0, VinvWt[j].nrow(), adim) = -VinvWt[j] * Sa;
		    jj += VinvWt[j].nrow();
		}
		    
		return S.symmetrize() *= sqr;
	    }
	    
	    if (sqr_new < sqr)
	    {
		a = a_new;			// Update parameters.
		copy(b_new.begin(), b_new.end(), bbegin);
		fval = fval_new;		// Update function values.
		sqr = sqr_new;			// Update residual.
		lambda *= 0.1;			// Decrease L-M parameter.
		break;
	    }
	    else
		lambda *= 10.0;			// Increase L-M parameter.
	}
    }
    throw std::runtime_error("minimizeSquareSparse: maximum iteration limit exceeded!");

    return Matrix<ET>(0, 0);
}

/************************************************************************
*  function minimizeSquareSparseDebug					*
*    -- Compute a and b st. sum||f(a, b[j])||^2 -> min under g(a) = 0.	*
************************************************************************/
template <class F, class G, class ATA, class IB>  Matrix<typename F::ET>
minimizeSquareSparseDebug(const F& f, const G& g, ATA& a, IB bbegin, IB bend,
			  int niter_max=100, double tol=1.5e-8)
{
    using namespace					std;
    typedef typename F::value_type			ET;  // element type.
    typedef typename iterator_traits<IB>::value_type	ATB; // arg. b type.

    const u_int			nb = distance(bbegin, bend);
    Array<Vector<ET> >		fval(nb);	// function values.
    ET				sqr = 0;	// sum of squares.
    int				j = 0;
    for (IB b = bbegin; b != bend; ++b, ++j)
    {
	fval[j] = f(a, *b, j);
	sqr    += fval[j] * fval[j];
    }
    ET	lambda = 1.0e-7;			// L-M parameter.

    for (int n = 0; n++ < niter_max; )
    {
	const u_int		adim = f.adim();
	const u_int		bdim = f.bdim() * nb;
      	const Vector<ET>&	gval = g(a);
	const u_int		gdim = gval.dim();
	Matrix<ET>		U(adim, adim);
	Vector<ET>		Jtf(adim);
	Array<Matrix<ET> >	V(nb);
	Array<Matrix<ET> >	W(nb);
	Array<Vector<ET> >	Ktf(nb);
	Matrix<ET>		A(adim + bdim + gdim, adim + bdim + gdim);
	j = 0;
	for (IB b = bbegin; b != bend; ++b, ++j)
	{
	    const Matrix<ET>&	J  = f.jacobianA(a, *b, j);
	    const Matrix<ET>&	Jt = J.trns();
	    const Matrix<ET>&	K  = f.jacobianB(a, *b, j);

	    U     += Jt * J;
	    Jtf   += fval[j] * J;
	    V[j]   = K.trns() * K;
	    W[j]   = Jt * K;
	    Ktf[j] = fval[j] * K;

	    A(0, adim + j*f.bdim(), adim, f.bdim()) = W[j];
	    A(adim + j*f.bdim(), 0, f.bdim(), adim) = W[j].trns();
	}
	A(adim + bdim, 0, gdim, adim) = g.jacobian(a);
	A(0, adim + bdim, adim, gdim) = A(adim + bdim, 0, gdim, adim).trns();

	for (;;)
	{
	  // Compute da: update for parameters a to be estimated.
	    A(0, 0, adim, adim) = U;
	    for (int i = 0; i < adim; ++i)
		A[i][i] *= (1.0 + lambda);
	    for (int j = 0; j < nb; ++j)
	    {
		A(adim + j*f.bdim(), adim + j*f.bdim(), f.bdim(), f.bdim())
		    = V[j];
		for (int k = 0; k < f.bdim(); ++k)
		    A[adim + j*f.bdim() + k][adim + j*f.bdim() + k]
			*= (1.0 + lambda);
	    }

	    Vector<ET>	dx(adim + bdim + gdim);
	    dx(0, adim) = Jtf;
	    for (int j = 0; j < nb; ++j)
		dx(adim + j*f.bdim(), f.bdim()) = Ktf[j];
	    dx(adim + bdim, gdim) = gval;
	    dx.solve(A);
	    
	  // Compute updated parameters and function value to it.
	    ATA			a_new(a);
	    f.updateA(a_new, dx(0, adim));
	    Array<ATB>		b_new(nb);
	    copy(bbegin, bend, b_new.begin());
	    Array<Vector<ET> >	fval_new(nb);
	    ET			sqr_new = 0;
	    for (int j = 0; j < nb; ++j)
	    {
		const Vector<ET>& db = dx(adim + j*f.bdim(), f.bdim());
	      /*		cerr << "*** check:  "
				<< (dx(0, adim) * W[j] + V[j] * db - Ktf[j]);*/
		f.updateB(b_new[j], db);
		fval_new[j] = f(a_new, b_new[j], j);
		sqr_new	   += fval_new[j] * fval_new[j];
	    }
#ifdef TUMinimizePP_DEBUG
	    cerr << "val^2 = " << sqr << ", gval = " << gval
		 << "  (update: val^2 = " << sqr_new
		 << ", lambda = " << lambda << ")" << endl;
#endif
	    if (fabs(sqr_new - sqr) <=
		tol * (fabs(sqr_new) + fabs(sqr) + 1.0e-10))
	    {
		A(0, 0, adim, adim) = U;
		for (int j = 0; j < nb; ++j)
		    A(adim + j*f.bdim(), adim + j*f.bdim(), f.bdim(), f.bdim())
			= V[j];
		Vector<ET>	evalue;
		A(0, 0, adim + bdim, adim + bdim).eigen(evalue);
		cerr << evalue;
		return A(0, 0, adim + bdim, adim + bdim).pinv(1.0e8) *= sqr;
	    }

	    if (sqr_new < sqr)
	    {
		a = a_new;			// Update parameters.
		copy(b_new.begin(), b_new.end(), bbegin);
		fval = fval_new;		// Update function values.
		sqr = sqr_new;			// Update residual.
		lambda *= 0.1;			// Decrease L-M parameter.
		break;
	    }
	    else
		lambda *= 10.0;			// Increase L-M parameter.
	}
    }
    throw std::runtime_error("minimizeSquareSparseDebug: maximum iteration limit exceeded!");

    return Matrix<ET>(0, 0);
}
 
}
#endif	/* !__TUMinimize_h	*/
