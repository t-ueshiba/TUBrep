/*
 *  $BJ?@.(B9$BG/(B $BEE;R5;=QAm9g8&5f=j(B $B?"<G=SIW(B $BCx:n8"=jM-(B
 *
 *  $BCx:n<T$K$h$k5v2D$J$7$K$3$N%W%m%0%i%`$NBh;0<T$X$N3+<(!"J#@=!"2~JQ!"(B
 *  $B;HMQEy$=$NB>$NCx:n?M3J8"$r?/32$9$k9T0Y$r6X;_$7$^$9!#(B
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *
 *  Copyright 1996
 *  Toshio UESHIBA, Electrotechnical Laboratory
 *
 *  All rights reserved.
 *  Any changing, copying or giving information about source programs of
 *  any part of this software and/or documentation without permission of the
 *  authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damage in use of this program.
 */

/*
 *  $Id: Vector++.h,v 1.14 2006-12-22 00:05:55 ueshiba Exp $
 */
#ifndef __TUVectorPP_h
#define __TUVectorPP_h

#include <math.h>
#ifdef WIN32
#  define M_PI	3.14159265358979323846
#endif
#include "TU/Array++.h"

namespace TU
{
/************************************************************************
*  class Rotation							*
************************************************************************/
//! 2$B<!85D6J?LLFb$G$N2sE>$rI=$9%/%i%9(B
/*!
  $B6qBNE*$K$O(B
  \f[
    \TUvec{R}{}(p, q; \theta) \equiv
    \begin{array}{r@{}l}
      & \begin{array}{ccccccccccc}
        & & \makebox[4.0em]{} & p & & & \makebox[3.8em]{} & q & & &
      \end{array} \\
      \begin{array}{l}
        \\ \\ \\ \raisebox{1.5ex}{$p$} \\ \\ \\ \\ \raisebox{1.5ex}{$q$} \\ \\ \\
      \end{array} &
      \TUbeginarray{ccccccccccc}
	1 \\
	& \ddots \\
	& & 1 \\
	& & & \cos\theta & & & & -\sin\theta \\
	& & & & 1 \\
	& & & & & \ddots \\
	& & & & & & 1 \\
	& & & \sin\theta & & & & \cos\theta \\
	& & & & & & & & 1\\
	& & & & & & & & & \ddots \\
	& & & & & & & & & & 1
      \TUendarray
    \end{array}
  \f]
  $B$J$k2sE>9TNs$GI=$5$l$k!%(B
*/
class Rotation
{
  public:
    Rotation(int p, int q, double x, double y)		;
    Rotation(int p, int q, double theta)		;

  //! p$B<4$rJV$9(B
  /*!
    \return	p$B<4$N(Bindex.
  */
    int		p()				const	{return _p;}

  //! q$B<4$rJV$9(B
  /*!
    \return	q$B<4$N(Bindex.
  */
    int		q()				const	{return _q;}

  //! $B2sE>3Q@8@.%Y%/%H%k$ND9$5$rJV$9(B
  /*!
    \return	$B2sE>3Q@8@.%Y%/%H%k(B(x, y)$B$KBP$7$F(B
		\f$\sqrt{x^2 + y^2}\f$$B$rJV$9(B.
  */
    double	length()			const	{return _l;}

  //! $B2sE>3Q$N(Bcos$BCM$rJV$9(B
  /*!
    \return	$B2sE>3Q$N(Bcos$BCM(B.
  */
    double	cos()				const	{return _c;}

  //! $B2sE>3Q$N(Bsin$BCM$rJV$9(B
  /*!
    \return	$B2sE>3Q$N(Bsin$BCM(B.
  */
    double	sin()				const	{return _s;}
    
  private:
    const int	_p, _q;				// rotation axis
    double	_l;				// length of (x, y)
    double	_c, _s;				// cos & sin
};

/************************************************************************
*  class Vector<T>							*
************************************************************************/
template <class T>	class Matrix;

//! T$B7?$NMWAG$r;}$D%Y%/%H%k$rI=$9%/%i%9(B
template <class T>
class Vector : public Array<T>
{
  public:
  //! $B;XDj$5$l$?<!85$N%Y%/%H%k$r@8@.$7!$A4MWAG$r(B0$B$G=i4|2=$9$k(B
  /*!
    \param d	$B%Y%/%H%k$N<!85(B.
  */
    explicit Vector(u_int d=0)			:Array<T>(d)	{*this = 0.0;}

  //! $B30It5-21NN0h$H<!85$r;XDj$7$F%Y%/%H%k$r@8@.$9$k(B
  /*!
    \param p	$B30It5-21NN0h$X$N%]%$%s%?(B.
    \param d	$B%Y%/%H%k$N<!85(B.
  */
    Vector(T* p, u_int d)			:Array<T>(p, d)		{}

  //! $BM?$($i$l$?%Y%/%H%k$H5-21NN0h$r6&M-$9$kItJ,%Y%/%H%k$r@8@.$9$k(B
  /*!
    \param v	$B85$N%Y%/%H%k(B.
    \param i	$BItJ,%Y%/%H%k$NBh(B0$BMWAG$r;XDj$9$k(Bindex.
    \param d	$BItJ,%Y%/%H%k$N<!85(B.
  */
    Vector(const Vector& v, u_int i, u_int d)	:Array<T>(v, i, d)	{}

  //! $B%3%T!<%3%s%9%H%i%/%?(B
  /*!
    \param v	$B%3%T!<85%Y%/%H%k(B.
  */
    Vector(const Vector& v)			:Array<T>(v)		{}
    
  //! $B%3%T!<1i;;;R(B
  /*!
    \param v	$B%3%T!<85%Y%/%H%k(B.
    \return	$B$3$N%Y%/%H%k(B.
  */
    Vector&	operator =(const Vector& v)	{Array<T>::operator =(v);
						 return *this;}

    const Vector	operator ()(u_int, u_int)	const	;
    Vector		operator ()(u_int, u_int)		;

    using	Array<T>::dim;

  //! $B$3$N%Y%/%H%k$NA4$F$NMWAG$KF10l$N?tCM$rBeF~$9$k(B
  /*!
    \param c	$BBeF~$9$k?tCM(B.
    \return	$B$3$N%Y%/%H%k(B.
  */
    Vector&	operator  =(double c)		{Array<T>::operator  =(c);
						 return *this;}

  //! $B$3$N%Y%/%H%k$K;XDj$5$l$??tCM$r3]$1$k(B
  /*!
    \param c	$B3]$1$k?tCM(B.
    \return	$B$3$N%Y%/%H%k!$$9$J$o$A(B\f$\TUvec{u}{}\leftarrow c\TUvec{u}{}\f$.
  */
    Vector&	operator *=(double c)		{Array<T>::operator *=(c);
						 return *this;}

  //! $B$3$N%Y%/%H%k$r;XDj$5$l$??tCM$G3d$k(B
  /*!
    \param c	$B3d$k?tCM(B.
    \return	$B$3$N%Y%/%H%k!$$9$J$o$A(B
		\f$\TUvec{u}{}\leftarrow \frac{\TUvec{u}{}}{c}\f$.
  */
    Vector&	operator /=(double c)		{Array<T>::operator /=(c);
						 return *this;}

  //! $B$3$N%Y%/%H%k$KB>$N%Y%/%H%k$rB-$9(B
  /*!
    \param v	$BB-$9%Y%/%H%k(B.
    \return	$B$3$N%Y%/%H%k!$$9$J$o$A(B
		\f$\TUvec{u}{}\leftarrow \TUvec{u}{} + \TUvec{v}{}\f$.
  */
    Vector&	operator +=(const Vector& v)	{Array<T>::operator +=(v);
						 return *this;}

  //! $B$3$N%Y%/%H%k$+$iB>$N%Y%/%H%k$r0z$/(B
  /*!
    \param v	$B0z$/%Y%/%H%k(B.
    \return	$B$3$N%Y%/%H%k!$$9$J$o$A(B
		\f$\TUvec{u}{}\leftarrow \TUvec{u}{} - \TUvec{v}{}\f$.
  */
    Vector&	operator -=(const Vector& v)	{Array<T>::operator -=(v);
						 return *this;}

    Vector&	operator ^=(const Vector&)	;

  //! $B$3$N%Y%/%H%k$N1&$+$i9TNs$r3]$1$k(B
  /*!
    \param m	$B3]$1$k9TNs(B.
    \return	$B$3$N%Y%/%H%k!$$9$J$o$A(B
		\f$\TUtvec{u}{} \leftarrow \TUtvec{u}{}\TUvec{M}{}\f$.
  */
    Vector&	operator *=(const Matrix<T>& m) {return *this = *this * m;}

  //! $B$3$N%Y%/%H%k$NId9f$rH?E>$7$?%Y%/%H%k$rJV$9(B
  /*!
    \return	$BId9f$rH?E>$7$?%Y%/%H%k!$$9$J$o$A(B\f$-\TUvec{u}{}\f$.
  */
    Vector	operator  -()		const	{Vector r(*this);
						 r *= -1; return r;}

  //! $B$3$N%Y%/%H%k$ND9$5$N(B2$B>h$rJV$9(B
  /*!
    \return	$B%Y%/%H%k$ND9$5$N(B2$B>h!$$9$J$o$A(B\f$\TUnorm{\TUvec{u}{}}^2\f$.
  */
    double	square()		const	{return *this * *this;}

  //! $B$3$N%Y%/%H%k$ND9$5$rJV$9(B
  /*!
    \return	$B%Y%/%H%k$ND9$5!$$9$J$o$A(B\f$\TUnorm{\TUvec{u}{}}\f$.
  */
    double	length()		const	{return sqrt(square());}

  //! $B$3$N%Y%/%H%k$HB>$N%Y%/%H%k$N:9$ND9$5$N(B2$B>h$rJV$9(B
  /*!
    \param v	$BHf3SBP>]$H$J$k%Y%/%H%k(B.
    \return	$B%Y%/%H%k4V$N:9$N(B2$B>h!$$9$J$o$A(B
		\f$\TUnorm{\TUvec{u}{} - \TUvec{v}{}}^2\f$.
  */
    double	sqdist(const Vector& v) const	{return (*this - v).square();}

  //! $B$3$N%Y%/%H%k$HB>$N%Y%/%H%k$N:9$ND9$5$rJV$9(B
  /*!
    \param v	$BHf3SBP>]$H$J$k%Y%/%H%k(B.
    \return	$B%Y%/%H%k4V$N:9!$$9$J$o$A(B
		\f$\TUnorm{\TUvec{u}{} - \TUvec{v}{}}\f$.
  */
    double	dist(const Vector& v)	const	{return sqrt(sqdist(v));}

  //! $B$3$N%Y%/%H%k$ND9$5$r(B1$B$K@55,2=$9$k(B
  /*!
    \return	$B$3$N%Y%/%H%k!$$9$J$o$A(B
		\f$
		  \TUvec{u}{}\leftarrow\frac{\TUvec{u}{}}{\TUnorm{\TUvec{u}{}}}
		\f$.
  */
    Vector&	normalize()			{return *this /= length();}

    Vector	normal()		const	;
    Vector&	solve(const Matrix<T>&)		;
    Matrix<T>	skew()			const	;

  //! $B%Y%/%H%k$N<!85$rJQ99$7!$(B0$B$K=i4|2=$9$k(B
  /*!
    \param d	$B?7$7$$<!85(B.
  */
    void	resize(u_int d)		{Array<T>::resize(d); *this = 0.0;}

  //! $B%Y%/%H%k$NFbIt5-21NN0h$H<!85$rJQ99$9$k(B
  /*!
    \param p	$B?7$7$$FbIt5-21NN0h$X$N%]%$%s%?(B.
    \param d	$B?7$7$$<!85(B.
  */
    void	resize(T* p, u_int d)	{Array<T>::resize(p, d);}
};

//! $BF~NO%9%H%j!<%`$+$i%Y%/%H%k$rFI$_9~$`(B(ASCII)
/*!
  \param in	$BF~NO%9%H%j!<%`(B.
  \param a	$B%Y%/%H%k$NFI$_9~$_@h(B.
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B.
*/
template <class T> inline std::istream&
operator >>(std::istream& in, Vector<T>& v)
{
    return in >> (Array<T>&)v;
}

//! $B=PNO%9%H%j!<%`$X%Y%/%H%k$r=q$-=P$9(B(ASCII)
/*!
  \param out	$B=PNO%9%H%j!<%`(B.
  \param a	$B=q$-=P$9%Y%/%H%k(B.
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B.
*/
template <class T> inline std::ostream&
operator <<(std::ostream& out, const Vector<T>& v)
{
    return out << (const Array<T>&)v;
}

/************************************************************************
*  class Matrix<T>							*
************************************************************************/
//! T$B7?$NMWAG$r;}$D9TNs$rI=$9%/%i%9(B
/*!
  $B3F9T$,(BT$B7?$NMWAG$r;}$D%Y%/%H%k(B#TU::Vector<T>$B$K$J$C$F$$$k!%(B
*/
template <class T>
class Matrix : public Array2<Vector<T> >
{
  public:
  //! $B;XDj$5$l$?%5%$%:$N9TNs$r@8@.$7!$A4MWAG$r(B0$B$G=i4|2=$9$k(B
  /*!
    \param r	$B9TNs$N9T?t(B.
    \param c	$B9TNs$NNs?t(B.
  */
    explicit Matrix(u_int r=0, u_int c=0)
	:Array2<Vector<T> >(r, c)				{*this = 0.0;}

  //! $B30It5-21NN0h$H%5%$%:$r;XDj$7$F9TNs$r@8@.$9$k(B
  /*!
    \param p	$B30It5-21NN0h$X$N%]%$%s%?(B.
    \param r	$B9TNs$N9T?t(B.
    \param c	$B9TNs$NNs?t(B.
  */
    Matrix(T* p, u_int r, u_int c) :Array2<Vector<T> >(p, r, c)	{}

  //! $BM?$($i$l$?9TNs$H5-21NN0h$r6&M-$9$kItJ,9TNs$r@8@.$9$k(B
  /*!
    \param m	$B85$N9TNs(B.
    \param i	$BItJ,9TNs$NBh(B0$B9T$r;XDj$9$k(Bindex.
    \param j	$BItJ,9TNs$NBh(B0$BNs$r;XDj$9$k(Bindex.
    \param r	$BItJ,9TNs$N9T?t(B.
    \param c	$BItJ,9TNs$NNs?t(B.
  */
    Matrix(const Matrix& m, u_int i, u_int j, u_int r, u_int c)
	:Array2<Vector<T> >(m, i, j, r, c)			{}

  //! $B%3%T!<%3%s%9%H%i%/%?(B
  /*!
    \param m	$B%3%T!<859TNs(B.
  */
    Matrix(const Matrix& m)	:Array2<Vector<T> >(m)		{}

  //! $B%3%T!<1i;;;R(B
  /*!
    \param m	$B%3%T!<859TNs(B.
    \return	$B$3$N9TNs(B.
  */
    Matrix&	operator =(const Matrix& m)
			{Array2<Vector<T> >::operator =(m); return *this;}

    const Matrix	operator ()(u_int, u_int, u_int, u_int)	const	;
    Matrix		operator ()(u_int, u_int, u_int, u_int)		;

    using	Array2<Vector<T> >::dim;
    using	Array2<Vector<T> >::nrow;
    using	Array2<Vector<T> >::ncol;
    
    Matrix&	diag(double)			;
    Matrix&	rot(double, int)		;

  //! $B$3$N9TNs$NA4$F$NMWAG$KF10l$N?tCM$rBeF~$9$k(B
  /*!
    \param c	$BBeF~$9$k?tCM(B.
    \return	$B$3$N9TNs(B.
  */
    Matrix&	operator  =(double c)		{Array2<Vector<T> >::
						 operator  =(c); return *this;}
  //! $B$3$N9TNs$K;XDj$5$l$??tCM$r3]$1$k(B
  /*!
    \param c	$B3]$1$k?tCM(B.
    \return	$B$3$N9TNs!$$9$J$o$A(B\f$\TUvec{A}{}\leftarrow c\TUvec{A}{}\f$.
  */
    Matrix&	operator *=(double c)		{Array2<Vector<T> >::
						 operator *=(c); return *this;}

  //! $B$3$N9TNs$r;XDj$5$l$??tCM$G3d$k(B
  /*!
    \param c	$B3d$k?tCM(B.
    \return	$B$3$N9TNs!$$9$J$o$A(B
		\f$\TUvec{A}{}\leftarrow \frac{\TUvec{A}{}}{c}\f$.
  */
    Matrix&	operator /=(double c)		{Array2<Vector<T> >::
						 operator /=(c); return *this;}

  //! $B$3$N9TNs$KB>$N9TNs$rB-$9(B
  /*!
    \param m	$BB-$99TNs(B.
    \return	$B$3$N9TNs!$$9$J$o$A(B
		\f$\TUvec{A}{}\leftarrow \TUvec{A}{} + \TUvec{M}{}\f$.
  */
    Matrix&	operator +=(const Matrix& m)	{Array2<Vector<T> >::
						 operator +=(m); return *this;}

  //! $B$3$N9TNs$+$iB>$N9TNs$r0z$/(B
  /*!
    \param m	$B0z$/9TNs(B.
    \return	$B$3$N9TNs!$$9$J$o$A(B
		\f$\TUvec{A}{}\leftarrow \TUvec{A}{} - \TUvec{M}{}\f$.
  */
    Matrix&	operator -=(const Matrix& m)	{Array2<Vector<T> >::
						 operator -=(m); return *this;}

  //! $B$3$N9TNs$KB>$N9TNs$r3]$1$k(B
  /*!
    \param m	$B3]$1$k9TNs(B.
    \return	$B$3$N9TNs!$$9$J$o$A(B
		\f$\TUvec{A}{}\leftarrow \TUvec{A}{}\TUvec{M}{}\f$.
  */
    Matrix&	operator *=(const Matrix& m)	{return *this = *this * m;}

    Matrix&	operator ^=(const Vector<T>&)	;

  //! $B$3$N9TNs$NId9f$rH?E>$7$?9TNs$rJV$9(B
  /*!
    \return	$BId9f$rH?E>$7$?9TNs!$$9$J$o$A(B\f$-\TUvec{A}{}\f$.
  */
    Matrix	operator  -()			const	{Matrix r(*this);
							 r *= -1; return r;}

    Matrix	trns()				const	;
    Matrix	inv()				const	;
    Matrix&	solve(const Matrix<T>&)			;
    T		det()				const	;
    T		det(u_int, u_int)		const	;
    T		trace()				const	;
    Matrix	adj()				const	;
    Matrix	pinv(double cndnum=1.0e5)	const	;
    Matrix	eigen(Vector<T>&)		const	;
    Matrix	geigen(const Matrix<T>&,
		       Vector<T>&)		const	;
    Matrix	cholesky()			const	;
    Matrix&	normalize()				;
    Matrix&	rotate_from_left(const Rotation&)	;
    Matrix&	rotate_from_right(const Rotation&)	;
    double	square()			const	;

  //! $B$3$N9TNs$N(B2$B>h%N%k%`$rJV$9(B
  /*!
    \return	$B9TNs$N(B2$B>h%N%k%`!$$9$J$o$A(B\f$\TUnorm{\TUvec{A}{}}\f$.
  */
    double	length()		const	{return sqrt(square());}

    Matrix&	symmetrize()				;
    Matrix&	antisymmetrize()			;
    void	rot2angle(double& theta_x,
			  double& theta_y,
			  double& theta_z)	const	;
    Vector<T>	rot2axis(double& c, double& s)	const	;
    Vector<T>	rot2axis()			const	;

  //! $BC10L@5J}9TNs$r@8@.$9$k(B
  /*!
    \param d	$BC10L@5J}9TNs$N<!85(B.
    \return	$BC10L@5J}9TNs(B.
  */
    static Matrix	I(u_int d)	{return Matrix<T>(d, d).diag(1.0);}

    static Matrix	Rt(const Vector<T>& n, double c, double s)	;
    static Matrix	Rt(const Vector<T>& axis)			;

  //! $B9TNs$N%5%$%:$rJQ99$7!$(B0$B$K=i4|2=$9$k(B
  /*!
    \param r	$B?7$7$$9T?t(B.
    \param c	$B?7$7$$Ns?t(B.
  */
    void	resize(u_int r, u_int c)
			{Array2<Vector<T> >::resize(r, c); *this = 0.0;}

  //! $B9TNs$NFbIt5-21NN0h$H%5%$%:$rJQ99$9$k(B
  /*!
    \param p	$B?7$7$$FbIt5-21NN0h$X$N%]%$%s%?(B.
    \param r	$B?7$7$$9T?t(B.
    \param c	$B?7$7$$Ns?t(B.
  */
    void	resize(T* p, u_int r, u_int c)
			{Array2<Vector<T> >::resize(p, r, c);}
};

//! $BF~NO%9%H%j!<%`$+$i9TNs$rFI$_9~$`(B(ASCII)
/*!
  \param in	$BF~NO%9%H%j!<%`(B.
  \param a	$B9TNs$NFI$_9~$_@h(B.
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B.
*/
template <class T> inline std::istream&
operator >>(std::istream& in, Matrix<T>& m)
{
    return in >> (Array2<Vector<T> >&)m;
}

//! $B=PNO%9%H%j!<%`$X9TNs$r=q$-=P$9(B(ASCII)
/*!
  \param out	$B=PNO%9%H%j!<%`(B.
  \param a	$B=q$-=P$99TNs(B.
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B.
*/
template <class T> inline std::ostream&
operator <<(std::ostream& out, const Matrix<T>& m)
{
    return out << (const Array2<Vector<T> >&)m;
}

/************************************************************************
*  numerical operators							*
************************************************************************/
//! 2$B$D$N%Y%/%H%k$NB-$7;;(B
/*!
  \param a	$BBh(B1$B0z?t(B.
  \param b	$BBh(B2$B0z?t(B.
  \return	$B7k2L$r3JG<$7$?%Y%/%H%k!$$9$J$o$A(B\f$\TUvec{a}{}+\TUvec{b}{}\f$.
*/
template <class T> inline Vector<T>
operator +(const Vector<T>& a, const Vector<T>& b)
    {Vector<T> r(a); r += b; return r;}

//! 2$B$D$N%Y%/%H%k$N0z$-;;(B
/*!
  \param a	$BBh(B1$B0z?t(B.
  \param b	$BBh(B2$B0z?t(B.
  \return	$B7k2L$r3JG<$7$?%Y%/%H%k!$$9$J$o$A(B\f$\TUvec{a}{}-\TUvec{b}{}\f$.
*/
template <class T> inline Vector<T>
operator -(const Vector<T>& a, const Vector<T>& b)
    {Vector<T> r(a); r -= b; return r;}

//! $B%Y%/%H%k$KDj?t$r3]$1$k(B
/*!
  \param c	$B3]$1$kDj?t(B.
  \param a	$B%Y%/%H%k(B.
  \return	$B7k2L$r3JG<$7$?%Y%/%H%k!$$9$J$o$A(B\f$c\TUvec{a}{}\f$.
*/
template <class T> inline Vector<T>
operator *(double c, const Vector<T>& a)
    {Vector<T> r(a); r *= c; return r;}

//! $B%Y%/%H%k$KDj?t$r3]$1$k(B
/*!
  \param a	$B%Y%/%H%k(B.
  \param c	$B3]$1$kDj?t(B.
  \return	$B7k2L$r3JG<$7$?%Y%/%H%k!$$9$J$o$A(B\f$c\TUvec{a}{}\f$.
*/
template <class T> inline Vector<T>
operator *(const Vector<T>& a, double c)
    {Vector<T> r(a); r *= c; return r;}

//! $B%Y%/%H%k$N3FMWAG$rDj?t$G3d$k(B
/*!
  \param a	$B%Y%/%H%k(B.
  \param c	$B3d$kDj?t(B.
  \return	$B7k2L$r3JG<$7$?%Y%/%H%k!$$9$J$o$A(B\f$\frac{1}{c}\TUvec{a}{}\f$.
*/
template <class T> inline Vector<T>
operator /(const Vector<T>& a, double c)
    {Vector<T> r(a); r /= c; return r;}

//! 2$B$D$N9TNs$NB-$7;;(B
/*!
  \param a	$BBh(B1$B0z?t(B.
  \param b	$BBh(B2$B0z?t(B.
  \return	$B7k2L$r3JG<$7$?9TNs!$$9$J$o$A(B\f$\TUvec{A}{}+\TUvec{B}{}\f$.
*/
template <class T> inline Matrix<T>
operator +(const Matrix<T>& a, const Matrix<T>& b)
    {Matrix<T> r(a); r += b; return r;}

//! 2$B$D$N9TNs$N0z$-;;(B
/*!
  \param a	$BBh(B1$B0z?t(B.
  \param b	$BBh(B2$B0z?t(B.
  \return	$B7k2L$r3JG<$7$?9TNs!$$9$J$o$A(B\f$\TUvec{A}{}-\TUvec{B}{}\f$.
*/
template <class T> inline Matrix<T>
operator -(const Matrix<T>& a, const Matrix<T>& b)
    {Matrix<T> r(a); r -= b; return r;}

//! $B9TNs$KDj?t$r3]$1$k(B
/*!
  \param c	$B3]$1$kDj?t(B.
  \param a	$B9TNs(B.
  \return	$B7k2L$r3JG<$7$?9TNs!$$9$J$o$A(B\f$c\TUvec{A}{}\f$.
*/
template <class T> inline Matrix<T>
operator *(double c, const Matrix<T>& a)
    {Matrix<T> r(a); r *= c; return r;}

//! $B9TNs$KDj?t$r3]$1$k(B
/*!
  \param a	$B9TNs(B.
  \param c	$B3]$1$kDj?t(B.
  \return	$B7k2L$r3JG<$7$?9TNs!$$9$J$o$A(B\f$c\TUvec{A}{}\f$.
*/
template <class T> inline Matrix<T>
operator *(const Matrix<T>& a, double c)
    {Matrix<T> r(a); r *= c; return r;}

//! $B9TNs$N3FMWAG$rDj?t$G3d$k(B
/*!
  \param a	$B9TNs(B.
  \param c	$B3d$kDj?t(B.
  \return	$B7k2L$r3JG<$7$?9TNs!$$9$J$o$A(B\f$\frac{1}{c}\TUvec{A}{}\f$.
*/
template <class T> inline Matrix<T>
operator /(const Matrix<T>& a, double c)
    {Matrix<T> r(a); r /= c; return r;}

template <class T> extern double
operator *(const Vector<T>&, const Vector<T>&)	;

//! 2$B$D$N(B3$B<!85%Y%/%H%k$N%Y%/%H%k@Q(B
/*!
  \param v	$BBh(B1$B0z?t(B.
  \param w	$BBh(B2$B0z?t(B.
  \return	$B%Y%/%H%k@Q!$$9$J$o$A(B\f$\TUvec{v}{}\times\TUvec{w}{}\f$.
*/
template <class T> inline Vector<T>
operator ^(const Vector<T>& v, const Vector<T>& w)
    {Vector<T> r(v); r ^= w; return r;}

template <class T> extern Vector<T>
operator *(const Vector<T>&, const Matrix<T>&)	;

template <class T> extern Matrix<T>
operator %(const Vector<T>&, const Vector<T>&)	;

template <class T> extern Matrix<T>
operator ^(const Vector<T>&, const Matrix<T>&)	;

template <class T> extern Matrix<T>
operator *(const Matrix<T>&, const Matrix<T>&)	;

template <class T> extern Vector<T>
operator *(const Matrix<T>&, const Vector<T>&)	;

//! ?x3$B9TNs$N3F9T$H(B3$B<!85%Y%/%H%k$N%Y%/%H%k@Q(B
/*!
  \param m	?x3$B9TNs(B.
  \param v	3$B<!85%Y%/%H%k(B.
  \return	$B7k2L$N9TNs!$$9$J$o$A(B\f$(\TUtvec{M}{}\times\TUvec{v}{})^\top\f$.
*/
template <class T> inline Matrix<T>
operator ^(const Matrix<T>& m, const Vector<T>& v)
    {Matrix<T> r(m); r ^= v; return r;}

/************************************************************************
*  class LUDecomposition<T>						*
************************************************************************/
//! $B@5J}9TNs$N(BLU$BJ,2r$rI=$9%/%i%9(B
template <class T>
class LUDecomposition : private Array2<Vector<T> >
{
  public:
    LUDecomposition(const Matrix<T>&)			;

    void	substitute(Vector<T>&)		const	;

  //! $B$b$H$N@5J}9TNs$N9TNs<0$rJV$9(B
  /*!
    \return	$B$b$H$N@5J}9TNs$N9TNs<0(B.
  */
    T		det()				const	{return _det;}
    
  private:
    using	Array2<Vector<T> >::nrow;
    using	Array2<Vector<T> >::ncol;
    
    Array<int>	_index;
    T		_det;
};

/************************************************************************
*  class Householder<T>							*
************************************************************************/
template <class T>	class QRDecomposition;
template <class T>	class TriDiagonal;
template <class T>	class BiDiagonal;

//! Householder$BJQ49$rI=$9%/%i%9(B
template <class T>
class Householder : public Matrix<T>
{
  private:
    Householder(u_int dd, u_int d)
	:Matrix<T>(dd, dd), _d(d), _sigma(Matrix<T>::nrow())	{}
    Householder(const Matrix<T>&, u_int)			;

    using		Matrix<T>::dim;
    using		Matrix<T>::nrow;
    using		Matrix<T>::ncol;
    
    void		apply_from_left(Matrix<T>&, int)	;
    void		apply_from_right(Matrix<T>&, int)	;
    void		apply_from_both(Matrix<T>&, int)	;
    void		make_transformation()			;
    const Vector<T>&	sigma()			const	{return _sigma;}
    Vector<T>&		sigma()				{return _sigma;}
    bool		sigma_is_zero(int, T)	const	;

  private:
    const u_int		_d;		// deviation from diagonal element
    Vector<T>		_sigma;

    friend class	QRDecomposition<T>;
    friend class	TriDiagonal<T>;
    friend class	BiDiagonal<T>;
};

/************************************************************************
*  class QRDecomposition<T>						*
************************************************************************/
//! $B0lHL9TNs$N(BQR$BJ,2r$rI=$9%/%i%9(B
/*!
  $BM?$($i$l$?9TNs(B\f$\TUvec{A}{} \in \TUspace{R}{m\times n}\f$$B$KBP$7$F(B
  \f$\TUvec{A}{} = \TUtvec{R}{}\TUtvec{Q}{}\f$$B$J$k2<H>;03Q9TNs(B
  \f$\TUtvec{R}{} \in \TUspace{R}{m\times n}\f$$B$H2sE>9TNs(B
  \f$\TUtvec{Q}{} \in \TUspace{R}{n\times n}\f$$B$r5a$a$k(B
  $B!J(B\f$\TUvec{A}{}\f$$B$N3F9T$r(B\f$\TUtvec{Q}{}\f$$B$N9T$N@~7?7k9g$GI=8=$9(B
  $B$k!K!%(B
 */
template <class T>
class QRDecomposition : private Matrix<T>
{
  public:
    QRDecomposition(const Matrix<T>&)			;

    using		Matrix<T>::dim;

  //! QR$BJ,2r$N2<H>;03Q9TNs$rJV$9(B
  /*!
    \return	$B2<H>;03Q9TNs(B\f$\TUtvec{R}{}\f$.
  */
    const Matrix<T>&	Rt()			const	{return *this;}

  //! QR$BJ,2r$N2sE>9TNs$rJV$9(B
  /*!
    \return	$B2sE>9TNs(B\f$\TUtvec{Q}{}\f$.
  */
    const Matrix<T>&	Qt()			const	{return _Qt;}
    
  private:
    using		Matrix<T>::nrow;
    using		Matrix<T>::ncol;
    
    Householder<T>	_Qt;			// rotation matrix
};

/************************************************************************
*  class TriDiagonal<T>							*
************************************************************************/
//! $BBP>N9TNs$N(B3$B=EBP3Q2=$rI=$9%/%i%9(B
/*!
  $BM?$($i$l$?BP>N9TNs(B\f$\TUvec{A}{} \in \TUspace{R}{d\times d}\f$$B$KBP$7(B
  $B$F(B\f$\TUtvec{U}{}\TUvec{A}{}\TUvec{U}{}\f$$B$,(B3$B=EBP3Q9TNs$H$J$k$h$&$J2s(B
  $BE>9TNs(B\f$\TUtvec{U}{} \in \TUspace{R}{d\times d}\f$$B$r5a$a$k!%(B
 */
template <class T>
class TriDiagonal
{
  public:
    TriDiagonal(const Matrix<T>&)			;

  //! 3$B=EBP3Q2=$5$l$kBP>N9TNs$N<!85(B(= $B9T?t(B = $BNs?t(B)$B$rJV$9(B
  /*!
    \return	$BBP>N9TNs$N<!85(B.
  */
    u_int		dim()			const	{return _Ut.nrow();}

  //! 3$B=EBP3Q2=$r9T$&2sE>9TNs$rJV$9(B
  /*!
    \return	$B2sE>9TNs(B.
  */
    const Matrix<T>&	Ut()			const	{return _Ut;}

  //! 3$B=EBP3Q9TNs$NBP3Q@.J,$rJV$9(B
  /*!
    \return	$BBP3Q@.J,(B.
  */
    const Vector<T>&	diagonal()		const	{return _diagonal;}

  //! 3$B=EBP3Q9TNs$NHsBP3Q@.J,$rJV$9(B
  /*!
    \return	$BHsBP3Q@.J,(B.
  */
    const Vector<T>&	off_diagonal()		const	{return _Ut.sigma();}

    void		diagonalize()			;
    
  private:
    enum		{NITER_MAX = 30};

    bool		off_diagonal_is_zero(int)		const	;
    void		initialize_rotation(int, int,
					    double&, double&)	const	;
    
    Householder<T>	_Ut;
    Vector<T>		_diagonal;
    Vector<T>&		_off_diagonal;
};

/************************************************************************
*  class BiDiagonal<T>							*
************************************************************************/
//! $B0lHL9TNs$N(B2$B=EBP3Q2=$rI=$9%/%i%9(B
/*!
  $BM?$($i$l$?0lHL9TNs(B\f$\TUvec{A}{} \in \TUspace{R}{m\times n}\f$$B$KBP$7(B
  $B$F(B\f$\TUtvec{V}{}\TUvec{A}{}\TUvec{U}{}\f$$B$,(B2$B=EBP3Q9TNs$H$J$k$h$&$J(B2
  $B$D$N2sE>9TNs(B\f$\TUtvec{U}{} \in \TUspace{R}{n\times n}\f$,
  \f$\TUtvec{V}{} \in \TUspace{R}{m\times m}\f$$B$r5a$a$k!%(B\f$m \le n\f$
  $B$N>l9g$O2<H>;03Q$J(B2$B=EBP3Q9TNs$K!$(B\f$m > n\f$$B$N>l9g$O>eH>;03Q$J(B2$B=EBP3Q(B
  $B9TNs$K$J$k!%(B
 */
template <class T>
class BiDiagonal
{
  public:
    BiDiagonal(const Matrix<T>&)		;

  //! 2$B=EBP3Q2=$5$l$k9TNs$N9T?t$rJV$9(B
  /*!
    \return	$B9TNs$N9T?t(B.
  */
    u_int		nrow()		const	{return _Vt.nrow();}

  //! 2$B=EBP3Q2=$5$l$k9TNs$NNs?t$rJV$9(B
  /*!
    \return	$B9TNs$NNs?t(B.
  */
    u_int		ncol()		const	{return _Ut.nrow();}

  //! 2$B=EBP3Q2=$r9T$&$?$a$K1&$+$i3]$1$k2sE>9TNs$NE>CV$rJV$9(B
  /*!
    \return	$B1&$+$i3]$1$k2sE>9TNs$NE>CV(B.
  */
    const Matrix<T>&	Ut()		const	{return _Ut;}

  //! 2$B=EBP3Q2=$r9T$&$?$a$K:8$+$i3]$1$k2sE>9TNs$rJV$9(B
  /*!
    \return	$B:8$+$i3]$1$k2sE>9TNs(B.
  */
    const Matrix<T>&	Vt()		const	{return _Vt;}

  //! 2$B=EBP3Q9TNs$NBP3Q@.J,$rJV$9(B
  /*!
    \return	$BBP3Q@.J,(B.
  */
    const Vector<T>&	diagonal()	const	{return _Dt.sigma();}

  //! 2$B=EBP3Q9TNs$NHsBP3Q@.J,$rJV$9(B
  /*!
    \return	$BHsBP3Q@.J,(B.
  */
    const Vector<T>&	off_diagonal()	const	{return _Et.sigma();}

    void		diagonalize()		;

  private:
    enum		{NITER_MAX = 30};
    
    bool		diagonal_is_zero(int)			const	;
    bool		off_diagonal_is_zero(int)		const	;
    void		initialize_rotation(int, int,
					    double&, double&)	const	;

    Householder<T>	_Dt;
    Householder<T>	_Et;
    Vector<T>&		_diagonal;
    Vector<T>&		_off_diagonal;
    T			_anorm;
    const Matrix<T>&	_Ut;
    const Matrix<T>&	_Vt;
};

/************************************************************************
*  class SVDecomposition<T>						*
************************************************************************/
//! $B0lHL9TNs$NFC0[CMJ,2r$rI=$9%/%i%9(B
/*!
  $BM?$($i$l$?0lHL9TNs(B\f$\TUvec{A}{} \in \TUspace{R}{m\times n}\f$$B$KBP$7(B
  $B$F(B\f$\TUtvec{V}{}\TUvec{A}{}\TUvec{U}{}\f$$B$,BP3Q9TNs$H$J$k$h$&$J(B2$B$D$N(B
  $B2sE>9TNs(B\f$\TUtvec{U}{} \in \TUspace{R}{n\times n}\f$,
  \f$\TUtvec{V}{} \in \TUspace{R}{m\times m}\f$$B$r5a$a$k!%(B
 */
template <class T>
class SVDecomposition : private BiDiagonal<T>
{
  public:
  //! $BM?$($i$l$?0lHL9TNs$NFC0[CMJ,2r$r5a$a$k(B
  /*!
    \param a	$BFC0[CMJ,2r$9$k0lHL9TNs(B.
  */
    SVDecomposition(const Matrix<T>& a)
	:BiDiagonal<T>(a)			{BiDiagonal<T>::diagonalize();}

    using	BiDiagonal<T>::nrow;
    using	BiDiagonal<T>::ncol;
    using	BiDiagonal<T>::Ut;
    using	BiDiagonal<T>::Vt;
    using	BiDiagonal<T>::diagonal;

  //! $BFC0[CM$r5a$a$k(B
  /*!
    \param i	$B@dBPCM$NBg$-$$=g$KJB$s$@FC0[CM$N(B1$B$D$r;XDj$9$k(Bindex.
    \return	$B;XDj$5$l$?(Bindex$B$KBP1~$9$kFC0[CM(B.
  */
    const T&	operator [](int i)	const	{return diagonal()[i];}
};

/************************************************************************
*  class Minimization1<S>						*
************************************************************************/
static const double	DEFAULT_TOL = 3.0e-8;

template <class S>
class Minimization1
{
  private:
    enum		{DEFAULT_NITER_MAX = 100};

  public:
    Minimization1(S tol = DEFAULT_TOL, int niter_max = DEFAULT_NITER_MAX)
	:_tol(tol), _niter_max(niter_max)				{}
    
    virtual S		operator ()(const S&)			const	= 0;
    S			minimize(S&, S)				const	;

  private:
    const S		_tol;
    const int		_niter_max;
};

/************************************************************************
*  class Minimization<S, T>						*
************************************************************************/
template <class S, class T>
class Minimization
{
  private:
    class LineFunction : public Minimization1<S>
    {
      public:
	LineFunction(const Minimization<S, T>& func,
		     const T& x, const Vector<S>& h,
		     S tol, int niter_max)
	  :Minimization1<S>(tol, niter_max),
	   _func(func), _x(x), _h(h)		{}
    
	S	operator ()(const S& d)	const	{return _func(_func.proceed
							      (_x, d * _h));}
    
      private:
	const Minimization<S, T>&	_func;
	const T&			_x;
	const Vector<S>&		_h;
    };

  private:
    enum		{DEFAULT_NITER_MAX = 1000};
		 
  public:
    Minimization(S tol = DEFAULT_TOL, int niter_max = DEFAULT_NITER_MAX,
		 int pr = 0)
      :_tol(tol), _niter_max(niter_max), _print(pr)			{}
    
    virtual S		operator ()(const T&)			const	= 0;
    virtual Vector<S>	ngrad(const T& x)			const	= 0;
    virtual T		proceed(const T&, const Vector<S>&)	const	= 0;
    S			minimize(T&)					;
    S			steepest_descent(T&)				;
    S			line_minimize(T&, const Vector<S>&)	const	;

  protected:
    virtual void	update(const T&)				;
    virtual void	print(int, S, const T&)			const	;
    
  private:
    int			near_enough(S, S)			const	;
 
    const S		_tol;
    const int		_niter_max;
    const int		_print;
};

template <class S, class T> inline int
Minimization<S, T>::near_enough(S a, S b) const
{
#define EPS	1.0e-10
    return 2.0*fabs(a - b) <= _tol*(fabs(a) + fabs(b) + EPS);
}
 
}

#endif	/* !__TUVectorPP_h	*/
