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
 *  $Id: Vector++.cc,v 1.13 2006-12-19 07:09:24 ueshiba Exp $
 */
#include "TU/Vector++.h"
#include <stdexcept>
#include <iomanip>

namespace TU
{
template <class T> inline void
swap(T& a, T& b)
{
    const T tmp = a;
    a = b;
    b = tmp;
}

/************************************************************************
*  class Vector<T>							*
************************************************************************/
//! $B$3$N%Y%/%H%k$H5-21NN0h$r6&M-$7$?ItJ,%Y%/%H%k$r@8@.$9$k(B
/*!
    \param i	$BItJ,%Y%/%H%k$NBh(B0$BMWAG$r;XDj$9$k(Bindex.
    \param d	$BItJ,%Y%/%H%k$N<!85(B.
    \return	$B@8@.$5$l$?ItJ,%Y%/%H%k(B.
*/
template <class T> const Vector<T>
Vector<T>::operator ()(u_int i, u_int dd) const	// partial vector
{
    return Vector<T>(*this, i, dd);
}

//! $B$3$N%Y%/%H%k$H5-21NN0h$r6&M-$7$?ItJ,%Y%/%H%k$r@8@.$9$k(B
/*!
    \param i	$BItJ,%Y%/%H%k$NBh(B0$BMWAG$r;XDj$9$k(Bindex.
    \param d	$BItJ,%Y%/%H%k$N<!85(B.
    \return	$B@8@.$5$l$?ItJ,%Y%/%H%k(B.
*/
template <class T> Vector<T>
Vector<T>::operator ()(u_int i, u_int dd)	// partial vector
{
    return Vector<T>(*this, i, dd);
}

//! $B$3$N%Y%/%H%k$HB>$N(B3$B<!85%Y%/%H%k$H$N%Y%/%H%k@Q$r$H$k(B
/*!
    \param v	$BB>$N%Y%/%H%k(B.
    \return	$B$3$N%Y%/%H%k!$$9$J$o$A(B
		\f$\TUvec{u}{}\leftarrow \TUvec{u}{}\times\TUvec{v}{}\f$.
*/
template <class T> Vector<T>&
Vector<T>::operator ^=(const Vector<T>& v)	// outer product
{
    check_dim(v.dim());
    if (dim() != 3)
	throw std::invalid_argument("TU::Vector<T>::operator ^=: dimension must be 3");
    Vector<T> tmp(*this);
    (*this)[0] = tmp[1] * v[2] - tmp[2] * v[1];
    (*this)[1] = tmp[2] * v[0] - tmp[0] * v[2];
    (*this)[2] = tmp[0] * v[1] - tmp[1] * v[0];
    return *this;
}

//! $B$3$N%Y%/%H%k$ND9$5$r(B1$B$K@55,2=$7$?%Y%/%H%k$rJV$9(B
/*!
  \return	$BD9$5$r@55,2=$7$?%Y%/%H%k!$$9$J$o$A(B
		\f$\frac{\TUvec{u}{}}{\TUnorm{\TUvec{u}{}}}\f$.
*/
template <class T> Vector<T>
Vector<T>::normal() const			// return normalized vector
{
    Vector<T> r(*this);
    r.normalize();
    return r;
}

//! $BO"N)(B1$B<!J}Dx<0$r2r$/(B
/*!
  \param m	$B@5B'$J@5J}9TNs(B.
  \return	\f$\TUtvec{u}{} = \TUtvec{x}{}\TUvec{M}{}\f$
		$B$N2r$rG<$a$?$3$N%Y%/%H%k!$$9$J$o$A(B
		\f$\TUtvec{u}{} \leftarrow \TUtvec{u}{}\TUinv{M}{}\f$.
*/
template <class T> Vector<T>&
Vector<T>::solve(const Matrix<T>& m)
{
    LUDecomposition<T>(m).substitute(*this);
    return *this;
}

//! $B$3$N(B3$B<!85%Y%/%H%k$+$i(B3x3$BH?BP>N9TNs$r@8@.$9$k(B
/*!
  \return	$B@8@.$5$l$?H?BP>N9TNs!$$9$J$o$A(B
  \f[
    \TUskew{u}{} \equiv
    \TUbeginarray{ccc}
      & -u_2 & u_1 \\ u_2 & & -u_0 \\ -u_1 & u_0 &
    \TUendarray
  \f]
*/
template <class T> Matrix<T>
Vector<T>::skew() const
{
    if (dim() != 3)
	throw std::invalid_argument("TU::Vector<T>::skew: dimension must be 3");
    Matrix<T>	r(3, 3);
    r[2][1] = (*this)[0];
    r[0][2] = (*this)[1];
    r[1][0] = (*this)[2];
    r[1][2] = -r[2][1];
    r[2][0] = -r[0][2];
    r[0][1] = -r[1][0];
    return r;
}

/************************************************************************
*  class Matrix<T>							*
************************************************************************/
//! $B$3$N9TNs$H5-21NN0h$r6&M-$7$?ItJ,9TNs$r@8@.$9$k(B
/*!
    \param i	$BItJ,9TNs$N:8>e6yMWAG$H$J$k9T$r;XDj$9$k(Bindex.
    \param j	$BItJ,9TNs$N:8>e6yMWAG$H$J$kNs$r;XDj$9$k(Bindex.
    \param r	$BItJ,9TNs$N9T?t(B.
    \param c	$BItJ,9TNs$NNs?t(B.
    \return	$B@8@.$5$l$?ItJ,9TNs(B.
*/
template <class T> const Matrix<T>
Matrix<T>::operator ()(u_int i, u_int j, u_int r, u_int c) const
{
    return Matrix<T>(*this, i, j, r, c);
}

//! $B$3$N9TNs$H5-21NN0h$r6&M-$7$?ItJ,9TNs$r@8@.$9$k(B
/*!
    \param i	$BItJ,9TNs$N:8>e6yMWAG$H$J$k9T$r;XDj$9$k(Bindex.
    \param j	$BItJ,9TNs$N:8>e6yMWAG$H$J$kNs$r;XDj$9$k(Bindex.
    \param r	$BItJ,9TNs$N9T?t(B.
    \param c	$BItJ,9TNs$NNs?t(B.
    \return	$B@8@.$5$l$?ItJ,9TNs(B.
*/
template <class T> Matrix<T>
Matrix<T>::operator ()(u_int i, u_int j, u_int r, u_int c)
{
    return Matrix<T>(*this, i, j, r, c);
}

//! $B$3$N@5J}9TNs$rA4$FF10l$NBP3Q@.J,CM$r;}$DBP3Q9TNs$K$9$k(B
/*!
  \param c	$BBP3Q@.J,$NCM(B.
  \return	$B$3$N9TNs!$$9$J$o$A(B\f$\TUvec{A}{} \leftarrow \diag(c,\ldots,c)\f$.
*/
template <class T> Matrix<T>&
Matrix<T>::diag(double c)
{
    check_dim(dim());
    *this = 0.0;
    for (int i = 0; i < dim(); i++)
	(*this)[i][i] = c;
    return *this;
}

template <class T> Matrix<T>&
Matrix<T>::rot(double angle, int axis)
{
    check_dim(dim());
    return *this;
}

//! $B$3$N(B?x3$B9TNs$N3F9T$H(B3$B<!85%Y%/%H%k$H$N%Y%/%H%k@Q$r$H$k(B
/*!
  \param v	3$B<!85%Y%/%H%k(B.
  \return	$B$3$N9TNs!$$9$J$o$A(B
		\f$\TUvec{A}{}\leftarrow(\TUtvec{A}{}\times\TUvec{v}{})^\top\f$.
*/
template <class T> Matrix<T>&
Matrix<T>::operator ^=(const Vector<T>& v)
{
    for (int i = 0; i < nrow(); i++)
	(*this)[i] ^= v;
    return *this;
}

//! $B$3$N9TNs$NE>CV9TNs$rJV$9(B
/*!
  \return	$BE>CV9TNs!$$9$J$o$A(B\f$\TUtvec{A}{}\f$.
*/
template <class T> Matrix<T>
Matrix<T>::trns() const				// transpose
{
    Matrix<T> val(ncol(), nrow());
    for (int i = 0; i < nrow(); i++)
	for (int j = 0; j < ncol(); j++)
	    val[j][i] = (*this)[i][j];
    return val;
}

//! $B$3$N9TNs$N5U9TNs$rJV$9(B
/*!
  \return	$B5U9TNs!$$9$J$o$A(B\f$\TUinv{A}{}\f$.
*/
template <class T> Matrix<T>
Matrix<T>::inv() const
{
    Matrix<T>	identity(nrow(), ncol());
    
    return identity.diag(1.0).solve(*this);
}

//! $BO"N)(B1$B<!J}Dx<0$r2r$/(B
/*!
  \param m	$B@5B'$J@5J}9TNs(B.
  \return	\f$\TUvec{A}{} = \TUvec{X}{}\TUvec{M}{}\f$
		$B$N2r$rG<$a$?$3$N9TNs!$$9$J$o$A(B
		\f$\TUvec{A}{} \leftarrow \TUvec{A}{}\TUinv{M}{}\f$.
*/
template <class T> Matrix<T>&
Matrix<T>::solve(const Matrix<T>& m)
{
    LUDecomposition<T>	lu(m);
    
    for (int i = 0; i < nrow(); i++)
	lu.substitute((*this)[i]);
    return *this;
}

//! $B$3$N9TNs$N9TNs<0$rJV$9(B
/*!
  \return	$B9TNs<0!$$9$J$o$A(B\f$\det\TUvec{A}{}\f$.
*/
template <class T> T
Matrix<T>::det() const
{
    return LUDecomposition<T>(*this).det();
}

//! $B$3$N9TNs$N>.9TNs<0$rJV$9(B
/*!
  \param p	$B85$N9TNs$+$i<h$j=|$/9T$r;XDj$9$k(Bindex.
  \param q	$B85$N9TNs$+$i<h$j=|$/Ns$r;XDj$9$k(Bindex.
  \return	$B>.9TNs<0!$$9$J$o$A(B\f$\det\TUvec{A}{pq}\f$.
*/
template <class T> T
Matrix<T>::det(u_int p, u_int q) const
{
    Matrix<T>		d(nrow()-1, ncol()-1);
    for (int i = 0; i < p; ++i)
    {
	for (int j = 0; j < q; ++j)
	    d[i][j] = (*this)[i][j];
	for (int j = q; j < d.ncol(); ++j)
	    d[i][j] = (*this)[i][j+1];
    }
    for (int i = p; i < d.nrow(); ++i)
    {
	for (int j = 0; j < q; ++j)
	    d[i][j] = (*this)[i+1][j];
	for (int j = q; j < d.ncol(); ++j)
	    d[i][j] = (*this)[i+1][j+1];
    }
    return d.det();
}

//! $B$3$N9TNs$N(Btrace$B$rJV$9(B
/*!
  \return	trace, $B$9$J$o$A(B\f$\trace\TUvec{A}{}\f$.
*/
template <class T> T
Matrix<T>::trace() const
{
    if (nrow() != ncol())
        throw
	  std::invalid_argument("TU::Matrix<T>::trace(): not square matrix!!");
    T	val = 0.0;
    for (int i = 0; i < nrow(); ++i)
	val += (*this)[i][i];
    return val;
}

//! $B$3$N9TNs$NM>0x;R9TNs$rJV$9(B
/*!
  \return	$BM>0x;R9TNs!$$9$J$o$A(B
		\f$\TUtilde{A}{} = (\det\TUvec{A}{})\TUinv{A}{}\f$.
*/
template <class T> Matrix<T>
Matrix<T>::adj() const
{
    Matrix<T>		val(nrow(), ncol());
    for (int i = 0; i < val.nrow(); ++i)
	for (int j = 0; j < val.ncol(); ++j)
	    val[i][j] = ((i + j) % 2 ? -det(j, i) : det(j, i));
    return val;
}

//! $B$3$N9TNs$N5?;w5U9TNs$rJV$9(B
/*!
  \param cndnum	$B:GBgFC0[CM$KBP$9$k@dBPCM$N3d9g$,$3$l$KC#$7$J$$4pDl$OL5;k(B.
  \return	$B5?;w5U9TNs!$$9$J$o$AM?$($i$l$?9TNs$NFC0[CMJ,2r$r(B
		\f$\TUvec{A}{} = \TUvec{V}{}\diag(\sigma_0,\ldots,\sigma_{n-1})
		\TUtvec{U}{}\f$$B$H$9$k$H(B
		\f[
		  \TUvec{u}{0}\sigma_0^{-1}\TUtvec{v}{0} + \cdots +
		  \TUvec{u}{r}\sigma_{r-1}^{-1}\TUtvec{v}{r-1},
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \TUabs{\sigma_1} > \epsilon\TUabs{\sigma_0},\ldots,
		  \TUabs{\sigma_{r-1}} > \epsilon\TUabs{\sigma_0}
		\f]
*/
template <class T> Matrix<T>
Matrix<T>::pinv(double cndnum) const
{
    SVDecomposition<T>	svd(*this);
    Matrix<T>		B(svd.ncol(), svd.nrow());
    
    for (int i = 0; i < svd.diagonal().dim(); ++i)
	if (fabs(svd[i]) * cndnum > fabs(svd[0]))
	    B += (svd.Ut()[i] / svd[i]) % svd.Vt()[i];

    return B;
}

//! $B$3$NBP>N9TNs$N8GM-CM$H8GM-%Y%/%H%k$rJV$9(B
/*!
    \param eval	$B@dBPCM$NBg$-$$=g$KJB$Y$i$l$?8GM-CM(B.
    \return	$B3F9T$,8GM-%Y%/%H%k$+$i@.$k2sE>9TNs!$$9$J$o$A(B
		\f[
		  \TUvec{A}{}\TUvec{U}{} = \TUvec{U}{}\diag(e_0,\ldots,e_{n-1}),
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \TUtvec{U}{}\TUvec{U}{} = \TUvec{I}{n},~\det\TUvec{U}{} = 1
		\f]
		$B$J$k(B\f$\TUtvec{U}{}\f$.
*/
template <class T> Matrix<T>
Matrix<T>::eigen(Vector<T>& eval) const
{
    TriDiagonal<T>	tri(*this);

    tri.diagonalize();
    eval = tri.diagonal();

    return tri.Ut();
}

//! $B$3$NBP>N9TNs$N0lHL8GM-CM$H0lHL8GM-%Y%/%H%k$rJV$9(B
/*!
    \param B	$B$b$H$N9TNs$HF10l%5%$%:$N@5CMBP>N9TNs(B.
    \param eval	$B@dBPCM$NBg$-$$=g$KJB$Y$i$l$?0lHL8GM-CM(B.
    \return	$B3F9T$,0lHL8GM-%Y%/%H%k$+$i@.$k@5B'9TNs(B
		$B!J$?$@$7D>8r9TNs$G$O$J$$!K!$$9$J$o$A(B
		\f[
		  \TUvec{A}{}\TUvec{U}{} =
		  \TUvec{B}{}\TUvec{U}{}\diag(e_0,\ldots,e_{n-1}),
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \TUtvec{U}{}\TUvec{B}{}\TUvec{U}{} = \TUvec{I}{n}
		\f]
		$B$J$k(B\f$\TUtvec{U}{}\f$.
*/
template <class T> Matrix<T>
Matrix<T>::geigen(const Matrix<T>& B, Vector<T>& eval) const
{
    Matrix<T>	Ltinv = B.cholesky().inv(), Linv = Ltinv.trns();
    Matrix<T>	Ut = (Linv * (*this) * Ltinv).eigen(eval);
    
    return Ut * Linv;
}

//! $B$3$N@5CMBP>N9TNs$N(BCholesky$BJ,2r!J>eH>;03Q9TNs!K$rJV$9(B
/*!
  $B7W;;$K$*$$$F$O!$$b$H$N9TNs$N>eH>ItJ,$7$+;H$o$J$$(B.
  \return	\f$\TUvec{A}{} = \TUvec{L}{}\TUtvec{L}{}\f$$B$J$k(B
		\f$\TUtvec{L}{}\f$$B!J>eH>;03Q9TNs!K(B.
*/
template <class T> Matrix<T>
Matrix<T>::cholesky() const
{
    if (nrow() != ncol())
        throw
	    std::invalid_argument("TU::Matrix<T>::cholesky(): not square matrix!!");

    Matrix<T>	Lt(*this);
    for (int i = 0; i < nrow(); ++i)
    {
	T d = Lt[i][i];
	if (d <= 0.0)
	    throw std::runtime_error("TU::Matrix<T>::cholesky(): not positive definite matrix!!");
	for (int j = 0; j < i; ++j)
	    Lt[i][j] = 0.0;
	Lt[i][i] = d = sqrt(d);
	for (int j = i + 1; j < ncol(); ++j)
	    Lt[i][j] /= d;
	for (int j = i + 1; j < nrow(); ++j)
	    for (int k = j; k < ncol(); ++k)
		Lt[j][k] -= (Lt[i][j] * Lt[i][k]);
    }
    
    return Lt;
}

//! $B$3$N9TNs$N%N%k%`$r(B1$B$K@55,2=$9$k(B
/*!
    \return	$B$3$N9TNs!$$9$J$o$A(B
		\f$
		  \TUvec{A}{}\leftarrow\frac{\TUvec{A}{}}{\TUnorm{\TUvec{A}{}}}
		\f$.
*/
template <class T> Matrix<T>&
Matrix<T>::normalize()
{
    double	sum = 0.0;
    
    for (int i = 0; i < nrow(); ++i)
	sum += (*this)[i] * (*this)[i];
    return *this /= sqrt(sum);
}

//! $B$3$N9TNs$N:8$+$i!JE>CV$5$l$?!K2sE>9TNs$r3]$1$k(B
/*!
    \return	$B$3$N9TNs!$$9$J$o$A(B
		\f$\TUvec{A}{}\leftarrow\TUtvec{R}{}\TUvec{A}{}\f$.
*/
template <class T> Matrix<T>&
Matrix<T>::rotate_from_left(const Rotation& r)
{
    for (int j = 0; j < ncol(); j++)
    {
	const T	tmp = (*this)[r.p()][j];
	
	(*this)[r.p()][j] =  r.cos()*tmp + r.sin()*(*this)[r.q()][j];
	(*this)[r.q()][j] = -r.sin()*tmp + r.cos()*(*this)[r.q()][j];
    }
    return *this;
}

//! $B$3$N9TNs$N1&$+$i2sE>9TNs$r3]$1$k(B
/*!
    \return	$B$3$N9TNs!$$9$J$o$A(B
		\f$\TUvec{A}{}\leftarrow\TUvec{A}{}\TUvec{R}{}\f$.
*/
template <class T> Matrix<T>&
Matrix<T>::rotate_from_right(const Rotation& r)
{
    for (int i = 0; i < nrow(); i++)
    {
	const T	tmp = (*this)[i][r.p()];
	
	(*this)[i][r.p()] =  tmp*r.cos() + (*this)[i][r.q()]*r.sin();
	(*this)[i][r.q()] = -tmp*r.sin() + (*this)[i][r.q()]*r.cos();
    }
    return *this;
}

//! $B$3$N9TNs$N(B2$B>h%N%k%`$N(B2$B>h$rJV$9(B
/*!
    \return	$B9TNs$N(B2$B>h%N%k%`$N(B2$B>h!$$9$J$o$A(B\f$\TUnorm{\TUvec{A}{}}^2\f$.
*/
template <class T> double
Matrix<T>::square() const
{
    double	val = 0.0;
    for (int i = 0; i < nrow(); ++i)
	val += (*this)[i] * (*this)[i];
    return val;
}

//! $B$3$N9TNs$N2<H>;03QItJ,$r>eH>;03QItJ,$K%3%T!<$7$FBP>N2=$9$k(B
/*!
    \return	$B$3$N9TNs(B.
*/
template <class T> Matrix<T>&
Matrix<T>::symmetrize()
{
    for (int i = 0; i < nrow(); ++i)
	for (int j = 0; j < i; ++j)
	    (*this)[j][i] = (*this)[i][j];
    return *this;
}

//! $B$3$N9TNs$N2<H>;03QItJ,$NId9f$rH?E>$7!$>eH>;03QItJ,$K%3%T!<$7$FH?BP>N2=$9$k(B
/*!
    \return	$B$3$N9TNs(B.
*/
template <class T> Matrix<T>&
Matrix<T>::antisymmetrize()
{
    for (int i = 0; i < nrow(); ++i)
    {
	(*this)[i][i] = 0.0;
	for (int j = 0; j < i; ++j)
	    (*this)[j][i] = -(*this)[i][j];
    }
    return *this;
}

//! $B$3$N(B3$B<!852sE>9TNs$+$i3F<4<~$j$N2sE>3Q$r<h$j=P$9(B
/*!
  $B$3$N9TNs$r(B\f$\TUtvec{R}{}\f$$B$H$9$k$H!$(B
  \f[
    \TUvec{R}{} =
    \TUbeginarray{ccc}
      \cos\theta_z & -\sin\theta_z & \\
      \sin\theta_z &  \cos\theta_z & \\
      & & 1
    \TUendarray
    \TUbeginarray{ccc}
       \cos\theta_y & & \sin\theta_y \\
       & 1 & \\
      -\sin\theta_y & & \cos\theta_y
    \TUendarray
    \TUbeginarray{ccc}
      1 & & \\
      & \cos\theta_x & -\sin\theta_x \\
      & \sin\theta_x &  \cos\theta_x
    \TUendarray
  \f]
  $B$J$k(B\f$\theta_x, \theta_y, \theta_z\f$$B$,2sE>3Q$H$J$k!%(B
 \param theta_x	x$B<4<~$j$N2sE>3Q(B(\f$ -\pi \le \theta_x \le \pi\f$)$B$rJV$9(B.
 \param theta_y	y$B<4<~$j$N2sE>3Q(B
	(\f$ -\frac{\pi}{2} \le \theta_y \le \frac{\pi}{2}\f$)$B$rJV$9(B.
 \param theta_z	z$B<4<~$j$N2sE>3Q(B(\f$ -\pi \le \theta_z \le \pi\f$)$B$rJV$9(B.
*/
template <class T> void
Matrix<T>::rot2angle(double& theta_x, double& theta_y, double& theta_z) const
{
    using namespace	std;
    
    if (nrow() != 3 || ncol() != 3)
	throw std::invalid_argument("TU::Matrix<T>::rot2angle: input matrix must be 3x3!!");

    if ((*this)[0][0] == 0.0 && (*this)[0][1] == 0.0)
    {
	theta_x = atan2(-(*this)[2][1], (*this)[1][1]);
	theta_y = ((*this)[0][2] < 0.0 ? M_PI / 2.0 : -M_PI / 2.0);
	theta_z = 0.0;
    }
    else
    {
	theta_x = atan2((*this)[1][2], (*this)[2][2]);
	theta_y = -asin((*this)[0][2]);
	theta_z = atan2((*this)[0][1], (*this)[0][0]);
    }
}

//! $B$3$N(B3$B<!852sE>9TNs$+$i2sE>3Q$H2sE><4$r<h$j=P$9(B
/*!
  $B$3$N9TNs$r(B\f$\TUtvec{R}{}\f$$B$H$9$k$H!$(B
  \f[
    \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
    + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
    - \TUskew{n}{}\sin\theta
  \f]
  $B$J$k(B\f$\theta\f$$B$H(B\f$\TUvec{n}{}\f$$B$,$=$l$>$l2sE>3Q$H2sE><4$H$J$k!%(B
 \param c	$B2sE>3Q$N(Bcos$BCM!$$9$J$o$A(B\f$\cos\theta\f$$B$rJV$9(B.
 \param s	$B2sE>3Q$N(Bsin$BCM!$$9$J$o$A(B\f$\sin\theta\f$$B$rJV$9(B.
 \return	$B2sE><4$rI=$9(B3$B<!85C10L%Y%/%H%k!$$9$J$o$A(B\f$\TUvec{n}{}\f$.
*/
template <class T> Vector<T>
Matrix<T>::rot2axis(double& c, double& s) const
{
    if (nrow() != 3 || ncol() != 3)
	throw std::invalid_argument("TU::Matrix<T>::rot2axis: input matrix must be 3x3!!");

  // Compute cosine and sine of rotation angle.
    const double	trace = (*this)[0][0] + (*this)[1][1] + (*this)[2][2];
    c = (trace - 1.0) / 2.0;
    s = sqrt((trace + 1.0)*(3.0 - trace)) / 2.0;

  // Compute rotation axis.
    Vector<T>	n(3);
    n[0] = (*this)[1][2] - (*this)[2][1];
    n[1] = (*this)[2][0] - (*this)[0][2];
    n[2] = (*this)[0][1] - (*this)[1][0];
    n.normalize();

    return n;
}

//! $B$3$N(B3$B<!852sE>9TNs$+$i2sE>3Q$H2sE><4$r<h$j=P$9(B
/*!
  $B$3$N9TNs$r(B\f$\TUtvec{R}{}\f$$B$H$9$k$H!$(B
  \f[
    \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
    + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
    - \TUskew{n}{}\sin\theta
  \f]
  $B$J$k(B\f$\theta\f$$B$H(B\f$\TUvec{n}{}\f$$B$,$=$l$>$l2sE>3Q$H2sE><4$H$J$k!%(B
 \return	$B2sE>3Q$H2sE><4$rI=$9(B3$B<!85%Y%/%H%k!$$9$J$o$A(B
		\f$\theta\TUvec{n}{}\f$.
*/
template <class T> Vector<T>
Matrix<T>::rot2axis() const
{
    if (nrow() != 3 || ncol() != 3)
	throw std::invalid_argument("TU::Matrix<T>::rot2axis: input matrix must be 3x3!!");

    Vector<T>	axis(3);
    axis[0] = ((*this)[1][2] - (*this)[2][1]) * 0.5;
    axis[1] = ((*this)[2][0] - (*this)[0][2]) * 0.5;
    axis[2] = ((*this)[0][1] - (*this)[1][0]) * 0.5;
    const double	s = sqrt(axis.square());
    if (s + 1.0 == 1.0)		// s << 1 ?
	return axis;
    const double	trace = (*this)[0][0] + (*this)[1][1] + (*this)[2][2];
    if (trace > 1.0)		// cos > 0 ?
	return  asin(s) / s * axis;
    else
	return -asin(s) / s * axis;
}

//! 3$B<!852sE>9TNs$r@8@.$9$k(B
/*!
  \param n	$B2sE><4$rI=$9(B3$B<!85C10L%Y%/%H%k(B.
  \param c	$B2sE>3Q$N(Bcos$BCM(B.
  \param s	$B2sE>3Q$N(Bsin$BCM(B.
  \return	$B@8@.$5$l$?2sE>9TNs!$$9$J$o$A(B
		\f[
		  \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
		  + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
		  - \TUskew{n}{}\sin\theta
		\f]
*/
template <class T> Matrix<T>
Matrix<T>::Rt(const Vector<T>& n, double c, double s)
{
    Matrix<T>	Qt = n % n;
    Qt *= (1.0 - c);
    Qt[0][0] += c;
    Qt[1][1] += c;
    Qt[2][2] += c;
    Qt[0][1] += n[2] * s;
    Qt[0][2] -= n[1] * s;
    Qt[1][0] -= n[2] * s;
    Qt[1][2] += n[0] * s;
    Qt[2][0] += n[1] * s;
    Qt[2][1] -= n[0] * s;

    return Qt;
}

//! 3$B<!852sE>9TNs$r@8@.$9$k(B
/*!
  \param axis	$B2sE>3Q$H2sE><4$rI=$9(B3$B<!85%Y%/%H%k(B.
  \return	$B@8@.$5$l$?2sE>9TNs!$$9$J$o$A(B
		\f[
		  \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
		  + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
		  - \TUskew{n}{}\sin\theta,{\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \theta = \TUnorm{\TUvec{a}{}},~
		  \TUvec{n}{} = \frac{\TUvec{a}{}}{\TUnorm{\TUvec{a}{}}}
		\f]
*/
template <class T> Matrix<T>
Matrix<T>::Rt(const Vector<T>& axis)
{
    double	theta = axis.length();
    if (theta + 1.0 == 1.0)		// theta << 1 ?
	return I(3);
    else
    {
	double	c = cos(theta), s = sin(theta);
	return Rt(axis / theta, c, s);
    }
}

/************************************************************************
*  numerical operators							*
************************************************************************/
//! 2$B$D$N%Y%/%H%k$NFb@Q(B
/*!
  \param v	$BBh(B1$B0z?t(B.
  \param w	$BBh(B2$B0z?t(B.
  \return	$BFb@Q!$$9$J$o$A(B\f$\TUtvec{v}{}\TUvec{w}{}\f$.
*/
template <class T> double
operator *(const Vector<T>& v, const Vector<T>& w)	// inner product
{
    v.check_dim(w.dim());
    double val = 0;
    for (int i = 0; i < v.dim(); i++)
	val += v[i] * w[i];
    return val;
}

//! $B%Y%/%H%k$H9TNs$N@Q(B
/*!
  \param v	$B%Y%/%H%k(B.
  \param m	$B9TNs(B.
  \return	$B7k2L$N%Y%/%H%k!$$9$J$o$A(B\f$\TUtvec{v}{}\TUvec{M}{}\f$.
*/
template <class T> Vector<T>
operator *(const Vector<T>& v, const Matrix<T>& m)	// multiply by matrix
{
    v.check_dim(m.nrow());
    Vector<T> val(m.ncol());
    for (int j = 0; j < m.ncol(); j++)
	for (int i = 0; i < m.nrow(); i++)
	    val[j] += v[i] * m[i][j];
    return val;
}

//! 2$B$D$N%Y%/%H%k$N30@Q(B
/*!
  \param v	$BBh(B1$B0z?t(B.
  \param w	$BBh(B2$B0z?t(B.
  \return	$B7k2L$N9TNs!$$9$J$o$A(B\f$\TUvec{v}{}\TUtvec{w}{}\f$.
*/
template <class T> Matrix<T>
operator %(const Vector<T>& v, const Vector<T>& w)	// multiply by vector
{
    Matrix<T> val(v.dim(), w.dim());
    for (int i = 0; i < v.dim(); i++)
	for (int j = 0; j < w.dim(); j++)
	    val[i][j] = v[i] * w[j];
    return val;
}

//! 3$B<!85%Y%/%H%k$H(B3x?$B9TNs$N3FNs$H$N%Y%/%H%k@Q(B
/*!
  \param v	3$B<!85%Y%/%H%k(B.
  \param m	3x?$B9TNs(B.
  \return	$B7k2L$N(B3x?$B9TNs!$$9$J$o$A(B\f$\TUvec{v}{}\times\TUvec{M}{}\f$.
*/
template <class T> Matrix<T>
operator ^(const Vector<T>& v, const Matrix<T>& m)
{
    v.check_dim(m.nrow());
    if (v.dim() != 3)
	throw std::invalid_argument("operator ^(const Vecotr<T>&, const Matrix<T>&): dimension of vector must be 3!!");
    Matrix<T>	val(m.nrow(), m.ncol());
    for (int j = 0; j < val.ncol(); j++)
    {
	val[0][j] = v[1] * m[2][j] - v[2] * m[1][j];
	val[1][j] = v[2] * m[0][j] - v[0] * m[2][j];
	val[2][j] = v[0] * m[1][j] - v[1] * m[0][j];
    }
    return val;
}

//! 2$B$D$N9TNs$N@Q(B
/*!
  \param m	$BBh(B1$B0z?t(B.
  \param n	$BBh(B2$B0z?t(B.
  \return	$B7k2L$N9TNs!$$9$J$o$A(B\f$\TUvec{M}{}\TUvec{N}{}\f$.
*/
template <class T> Matrix<T>
operator *(const Matrix<T>& m, const Matrix<T>& n)	// multiply by matrix
{
    n.check_dim(m.ncol());
    Matrix<T> val(m.nrow(), n.ncol());
    for (int i = 0; i < m.nrow(); i++)
	for (int j = 0; j < n.ncol(); j++)
	    for (int k = 0; k < m.ncol(); k++)
		val[i][j] += m[i][k] * n[k][j];
    return val;
}

//! $B9TNs$H%Y%/%H%k$N@Q(B
/*!
  \param m	$B9TNs(B.
  \param v	$B%Y%/%H%k(B.
  \return	$B7k2L$N%Y%/%H%k!$$9$J$o$A(B\f$\TUvec{M}{}\TUvec{v}{}\f$.
*/
template <class T> Vector<T>
operator *(const Matrix<T>& m, const Vector<T>& v)	// multiply by vector
{
    Vector<T> val(m.nrow());
    for (int i = 0; i < m.nrow(); i++)
	val[i] = m[i] * v;
    return val;
}

/************************************************************************
*  class LUDecomposition<T>						*
************************************************************************/
template <class T>
LUDecomposition<T>::LUDecomposition(const Matrix<T>& m)
    :Array2<Vector<T> >(m), _index(ncol()), _det(1.0)
{
    if (nrow() != ncol())
        throw std::invalid_argument("TU::LUDecomposition<T>::LUDecomposition: not square matrix!!");

    for (int j = 0; j < ncol(); j++)	// initialize column index
	_index[j] = j;			// for explicit pivotting

    Vector<T>	scale(ncol());
    for (int j = 0; j < ncol(); j++)	// find maximum abs. value in each col.
    {					// for implicit pivotting
	double max = 0.0;

	for (int i = 0; i < nrow(); i++)
	{
	    const double tmp = fabs((*this)[i][j]);
	    if (tmp > max)
		max = tmp;
	}
	scale[j] = (max != 0.0 ? 1.0 / max : 1.0);
    }

    for (int i = 0; i < nrow(); i++)
    {
	for (int j = 0; j < i; j++)		// left part (j < i)
	{
	    T& sum = (*this)[i][j];
	    for (int k = 0; k < j; k++)
		sum -= (*this)[i][k] * (*this)[k][j];
	}

	int	jmax;
	double	max = 0.0;
	for (int j = i; j < ncol(); j++)  // diagonal and right part (i <= j)
	{
	    T& sum = (*this)[i][j];
	    for (int k = 0; k < i; k++)
		sum -= (*this)[i][k] * (*this)[k][j];
	    const double tmp = fabs(sum) * scale[j];
	    if (tmp >= max)
	    {
		max  = tmp;
		jmax = j;
	    }
	}
	if (jmax != i)			// pivotting required ?
	{
	    for (int k = 0; k < nrow(); k++)	// swap i-th and jmax-th column
		swap((*this)[k][i], (*this)[k][jmax]);
	    swap(_index[i], _index[jmax]);	// swap column index
	    swap(scale[i], scale[jmax]);	// swap colum-wise scale factor
	    _det = -_det;
	}

	_det *= (*this)[i][i];

	if ((*this)[i][i] == 0.0)	// singular matrix ?
	    break;

	for (int j = i + 1; j < nrow(); j++)
	    (*this)[i][j] /= (*this)[i][i];
    }
}

template <class T> void
LUDecomposition<T>::substitute(Vector<T>& b) const
{
    if (b.dim() != ncol())
	throw std::invalid_argument("TU::LUDecomposition<T>::substitute: Dimension of given vector is not equal to mine!!");
    
    Vector<T>	tmp(b);
    for (int j = 0; j < b.dim(); j++)
	b[j] = tmp[_index[j]];

    for (int j = 0; j < b.dim(); j++)		// forward substitution
	for (int i = 0; i < j; i++)
	    b[j] -= b[i] * (*this)[i][j];
    for (int j = b.dim(); --j >= 0; )		// backward substitution
    {
	for (int i = b.dim(); --i > j; )
	    b[j] -= b[i] * (*this)[i][j];
	if ((*this)[j][j] == 0.0)		// singular matrix ?
	    throw std::runtime_error("TU::LUDecomposition<T>::substitute: singular matrix !!");
	b[j] /= (*this)[j][j];
    }
}

/************************************************************************
*  class Householder<T>							*
************************************************************************/
template <class T>
Householder<T>::Householder(const Matrix<T>& a, u_int d)
    :Matrix<T>(a), _d(d), _sigma(nrow())
{
    if (nrow() != ncol())
	throw std::invalid_argument("TU::Householder<T>::Householder: Given matrix must be square !!");
}

template <class T> void
Householder<T>::apply_from_left(Matrix<T>& a, int m)
{
    using namespace	std;
    
    if (a.nrow() < dim())
	throw std::invalid_argument("TU::Householder<T>::apply_from_left: # of rows of given matrix is smaller than my dimension !!");
    
    double	scale = 0.0;
    for (int i = m+_d; i < dim(); i++)
	scale += fabs(a[i][m]);
	
    if (scale != 0.0)
    {
	double	h = 0.0;
	for (int i = m+_d; i < dim(); i++)
	{
	    a[i][m] /= scale;
	    h += a[i][m] * a[i][m];
	}

	const double	s = (a[m+_d][m] > 0.0 ? sqrt(h) : -sqrt(h));
	h	     += s * a[m+_d][m];			// H = u^2 / 2
	a[m+_d][m]   += s;				// m-th col <== u
	    
	for (int j = m+1; j < a.ncol(); j++)
	{
	    T	p = 0.0;
	    for (int i = m+_d; i < dim(); i++)
		p += a[i][m] * a[i][j];
	    p /= h;					// p[j] (p' = u'A / H)
	    for (int i = m+_d; i < dim(); i++)
		a[i][j] -= a[i][m] * p;			// A = A - u*p'
	    a[m+_d][j] = -a[m+_d][j];
	}
	    
	for (int i = m+_d; i < dim(); i++)
	    (*this)[m][i] = scale * a[i][m];		// copy u
	_sigma[m+_d] = scale * s;
    }
}

template <class T> void
Householder<T>::apply_from_right(Matrix<T>& a, int m)
{
    using namespace	std;
    
    if (a.ncol() < dim())
	throw std::invalid_argument("Householder<T>::apply_from_right: # of column of given matrix is smaller than my dimension !!");
    
    double	scale = 0.0;
    for (int j = m+_d; j < dim(); j++)
	scale += fabs(a[m][j]);
	
    if (scale != 0.0)
    {
	double	h = 0.0;
	for (int j = m+_d; j < dim(); j++)
	{
	    a[m][j] /= scale;
	    h += a[m][j] * a[m][j];
	}

	const double	s = (a[m][m+_d] > 0.0 ? sqrt(h) : -sqrt(h));
	h	     += s * a[m][m+_d];			// H = u^2 / 2
	a[m][m+_d]   += s;				// m-th row <== u

	for (int i = m+1; i < a.nrow(); i++)
	{
	    T	p = 0.0;
	    for (int j = m+_d; j < dim(); j++)
		p += a[i][j] * a[m][j];
	    p /= h;					// p[i] (p = Au / H)
	    for (int j = m+_d; j < dim(); j++)
		a[i][j] -= p * a[m][j];			// A = A - p*u'
	    a[i][m+_d] = -a[i][m+_d];
	}
	    
	for (int j = m+_d; j < dim(); j++)
	    (*this)[m][j] = scale * a[m][j];		// copy u
	_sigma[m+_d] = scale * s;
    }
}

template <class T> void
Householder<T>::apply_from_both(Matrix<T>& a, int m)
{
    using namespace	std;
    
    Vector<T>		u = a[m](m+_d, a.ncol()-m-_d);
    double		scale = 0.0;
    for (int j = 0; j < u.dim(); j++)
	scale += fabs(u[j]);
	
    if (scale != 0.0)
    {
	u /= scale;

	double		h = u * u;
	const double	s = (u[0] > 0.0 ? sqrt(h) : -sqrt(h));
	h	     += s * u[0];			// H = u^2 / 2
	u[0]	     += s;				// m-th row <== u

	Matrix<T>	A = a(m+_d, m+_d, a.nrow()-m-_d, a.ncol()-m-_d);
	Vector<T>	p = _sigma(m+_d, nrow()-m-_d);
	for (int i = 0; i < A.nrow(); i++)
	    p[i] = (A[i] * u) / h;			// p = Au / H

	const double	k = (u * p) / (h + h);		// K = u*p / 2H
	for (int i = 0; i < A.nrow(); i++)
	{				// m-th col of 'a' is used as 'q'
	    a[m+_d+i][m] = p[i] - k * u[i];		// q = p - Ku
	    for (int j = 0; j <= i; j++)		// A = A - uq' - qu'
		A[j][i] = (A[i][j] -= (u[i]*a[m+_d+j][m] + a[m+_d+i][m]*u[j]));
	}
	for (int j = 1; j < A.nrow(); j++)
	    A[j][0] = A[0][j] = -A[0][j];

	for (int j = m+_d; j < a.ncol(); j++)
	    (*this)[m][j] = scale * a[m][j];		// copy u
	_sigma[m+_d] = scale * s;
    }
}

template <class T> void
Householder<T>::make_transformation()
{
    for (int m = nrow(); --m >= 0; )
    {
	for (int i = m+1; i < nrow(); i++)
	    (*this)[i][m] = 0.0;

	if (_sigma[m] != 0.0)
	{
	    for (int i = m+1; i < nrow(); i++)
	    {
		T	g = 0.0;
		for (int j = m+1; j < ncol(); j++)
		    g += (*this)[i][j] * (*this)[m-_d][j];
		g /= (_sigma[m] * (*this)[m-_d][m]);	// g[i] (g = Uu / H)
		for (int j = m; j < ncol(); j++)
		    (*this)[i][j] -= g * (*this)[m-_d][j];	// U = U - gu'
	    }
	    for (int j = m; j < ncol(); j++)
		(*this)[m][j] = (*this)[m-_d][j] / _sigma[m];
	    (*this)[m][m] -= 1.0;
	}
	else
	{
	    for (int j = m+1; j < ncol(); j++)
		(*this)[m][j] = 0.0;
	    (*this)[m][m] = 1.0;
	}
    }
}

template <class T> bool
Householder<T>::sigma_is_zero(int m, T comp) const
{
    return (T(fabs(_sigma[m])) + comp == comp);
}

/************************************************************************
*  class QRDeomposition<T>						*
************************************************************************/
template <class T>
QRDecomposition<T>::QRDecomposition(const Matrix<T>& m)
    :Matrix<T>(m), _Qt(m.ncol(), 0)
{
    for (int j = 0; j < ncol(); ++j)
	_Qt.apply_from_right(*this, j);
    _Qt.make_transformation();
    for (int i = 0; i < nrow(); ++i)
    {
	(*this)[i][i] = _Qt.sigma()[i];
	for (int j = i + 1; j < ncol(); ++j)
	    (*this)[i][j] = 0.0;
    }
}

/************************************************************************
*  class TriDiagonal<T>							*
************************************************************************/
template <class T>
TriDiagonal<T>::TriDiagonal(const Matrix<T>& a)
    :_Ut(a, 1), _diagonal(_Ut.nrow()), _off_diagonal(_Ut.sigma())
{
    if (_Ut.nrow() != _Ut.ncol())
        throw std::invalid_argument("TU::TriDiagonal<T>::TriDiagonal: not square matrix!!");

    for (int m = 0; m < dim(); m++)
    {
	_Ut.apply_from_both(_Ut, m);
	_diagonal[m] = _Ut[m][m];
    }

    _Ut.make_transformation();
}

template <class T> void
TriDiagonal<T>::diagonalize()
{
    for (int n = dim(); --n >= 0; )
    {
	int	niter = 0;
	
#ifdef TUVectorPP_DEBUG
	std::cerr << "******** n = " << n << " ********" << std::endl;
#endif
	while (!off_diagonal_is_zero(n))
	{					// n > 0 here
	    if (niter++ > NITER_MAX)
		throw std::runtime_error("TU::TriDiagonal::diagonalize(): Number of iteration exceeded maximum value!!");

	  /* Find first m (< n) whose off-diagonal element is 0 */
	    int	m = n;
	    while (!off_diagonal_is_zero(--m));	// 0 <= m < n < dim() here

	  /* Set x and y which determine initial(i = m+1) plane rotation */
	    double	x, y;
	    initialize_rotation(m, n, x, y);
	  /* Apply rotation P(i-1, i) for each i (i = m+1, n+2, ... , n) */
	    for (int i = m; ++i <= n; )
	    {
		Rotation	rot(i-1, i, x, y);
		
		_Ut.rotate_from_left(rot);

		if (i > m+1)
		    _off_diagonal[i-1] = rot.length();
		const double w = _diagonal[i] - _diagonal[i-1];
		const double d = rot.sin()*(rot.sin()*w
			       + 2.0*rot.cos()*_off_diagonal[i]);
		_diagonal[i-1]	 += d;
		_diagonal[i]	 -= d;
		_off_diagonal[i] += rot.sin()*(rot.cos()*w
				  - 2.0*rot.sin()*_off_diagonal[i]);
		if (i < n)
		{
		    x = _off_diagonal[i];
		    y = rot.sin()*_off_diagonal[i+1];
		    _off_diagonal[i+1] *= rot.cos();
		}
	    }
#ifdef TUVectorPP_DEBUG
	    std::cerr << "  niter = " << niter << ": " << off_diagonal();
#endif	    
	}
    }

    for (int m = 0; m < dim(); m++)	// sort eigen values and eigen vectors
	for (int n = m+1; n < dim(); n++)
	    if (fabs(_diagonal[n]) > fabs(_diagonal[m]))
	    {
		swap(_diagonal[m], _diagonal[n]);
		for (int j = 0; j < dim(); j++)
		{
		    const T	tmp = _Ut[m][j];
		    _Ut[m][j] = _Ut[n][j];
		    _Ut[n][j] = -tmp;
		}
	    }
}

template <class T> bool
TriDiagonal<T>::off_diagonal_is_zero(int n) const
{
    return (n == 0 || _Ut.sigma_is_zero(n, fabs(_diagonal[n-1]) +
					   fabs(_diagonal[n])));
}

template <class T> void
TriDiagonal<T>::initialize_rotation(int m, int n, double& x, double& y) const
{
    using namespace	std;
    
    const double	g = (_diagonal[n] - _diagonal[n-1]) /
			    (2.0*_off_diagonal[n]),
			absg = fabs(g),
			gg1 = (absg > 1.0 ?
			       absg * sqrt(1.0 + (1.0/absg)*(1.0/absg)) :
			       sqrt(1.0 + absg*absg)),
			t = (g > 0.0 ? g + gg1 : g - gg1);
    x = _diagonal[m] - _diagonal[n] - _off_diagonal[n]/t;
  //x = _diagonal[m];					// without shifting
    y = _off_diagonal[m+1];
}

/************************************************************************
*  class BiDiagonal<T>							*
************************************************************************/
template <class T>
BiDiagonal<T>::BiDiagonal(const Matrix<T>& a)
    :_Dt((a.nrow() < a.ncol() ? a.ncol() : a.nrow()), 0),
     _Et((a.nrow() < a.ncol() ? a.nrow() : a.ncol()), 1),
     _diagonal(_Dt.sigma()), _off_diagonal(_Et.sigma()), _anorm(0),
     _Ut(a.nrow() < a.ncol() ? _Dt : _Et),
     _Vt(a.nrow() < a.ncol() ? _Et : _Dt)
{
    if (nrow() < ncol())
	for (int i = 0; i < nrow(); ++i)
	    for (int j = 0; j < ncol(); ++j)
		_Dt[i][j] = a[i][j];
    else
	for (int i = 0; i < nrow(); ++i)
	    for (int j = 0; j < ncol(); ++j)
		_Dt[j][i] = a[i][j];

  /* Householder reduction to bi-diagonal (off-diagonal in lower part) form */
    for (int m = 0; m < _Et.dim(); ++m)
    {
	_Dt.apply_from_right(_Dt, m);
	_Et.apply_from_left(_Dt, m);
    }

    _Dt.make_transformation();	// Accumulate right-hand transformation: V
    _Et.make_transformation();	// Accumulate left-hand transformation: U

    for (int m = 0; m < _Et.dim(); ++m)
    {
	double	anorm = fabs(_diagonal[m]) + fabs(_off_diagonal[m]);
	if (anorm > _anorm)
	    _anorm = anorm;
    }
}

template <class T> void
BiDiagonal<T>::diagonalize()
{
    for (int n = _Et.dim(); --n >= 0; )
    {
	int	niter = 0;
	
#ifdef TUVectorPP_DEBUG
	std::cerr << "******** n = " << n << " ********" << std::endl;
#endif
	while (!off_diagonal_is_zero(n))	// n > 0 here
	{
	    if (niter++ > NITER_MAX)
		throw std::runtime_error("TU::BiDiagonal::diagonalize(): Number of iteration exceeded maximum value");
	    
	  /* Find first m (< n) whose off-diagonal element is 0 */
	    int m = n;
	    do
	    {
		if (diagonal_is_zero(m-1))
		{ // If _diagonal[m-1] is zero, make _off_diagonal[m] zero.
		    double	x = _diagonal[m], y = _off_diagonal[m];
		    _off_diagonal[m] = 0.0;
		    for (int i = m; i <= n; ++i)
		    {
			Rotation	rotD(m-1, i, x, -y);

			_Dt.rotate_from_left(rotD);
			
			_diagonal[i] = -y*rotD.sin()
				     + _diagonal[i]*rotD.cos();
			if (i < n)
			{
			    x = _diagonal[i+1];
			    y = _off_diagonal[i+1]*rotD.sin();
			    _off_diagonal[i+1] *= rotD.cos();
			}
		    }
		    break;	// if _diagonal[n-1] is zero, m == n here.
		}
	    } while (!off_diagonal_is_zero(--m)); // 0 <= m < n < nrow() here.
	    if (m == n)
		break;		// _off_diagonal[n] has been made 0. Retry!

	  /* Set x and y which determine initial(i = m+1) plane rotation */
	    double	x, y;
	    initialize_rotation(m, n, x, y);
#ifdef TUBiDiagonal_DEBUG
	    std::cerr << "--- m = " << m << ", n = " << n << "---"
		      << std::endl;
	    std::cerr << "  diagonal:     " << diagonal();
	    std::cerr << "  off-diagonal: " << off_diagonal();
#endif
	  /* Apply rotation P(i-1, i) for each i (i = m+1, n+2, ... , n) */
	    for (int i = m; ++i <= n; )
	    {
	      /* Apply rotation from left */
		Rotation	rotE(i-1, i, x, y);
		
		_Et.rotate_from_left(rotE);

		if (i > m+1)
		    _off_diagonal[i-1] = rotE.length();
		T	tmp = _diagonal[i-1];
		_diagonal[i-1]	 =  rotE.cos()*tmp
				 +  rotE.sin()*_off_diagonal[i];
		_off_diagonal[i] = -rotE.sin()*tmp
				 +  rotE.cos()*_off_diagonal[i];
		if (diagonal_is_zero(i))
		    break;		// No more Given's rotation needed.
		y		 =  rotE.sin()*_diagonal[i];
		_diagonal[i]	*=  rotE.cos();

		x = _diagonal[i-1];
		
	      /* Apply rotation from right to recover bi-diagonality */
		Rotation	rotD(i-1, i, x, y);

		_Dt.rotate_from_left(rotD);

		_diagonal[i-1] = rotD.length();
		tmp = _off_diagonal[i];
		_off_diagonal[i] =  tmp*rotD.cos() + _diagonal[i]*rotD.sin();
		_diagonal[i]	 = -tmp*rotD.sin() + _diagonal[i]*rotD.cos();
		if (i < n)
		{
		    if (off_diagonal_is_zero(i+1))
			break;		// No more Given's rotation needed.
		    y		        = _off_diagonal[i+1]*rotD.sin();
		    _off_diagonal[i+1] *= rotD.cos();

		    x		        = _off_diagonal[i];
		}
	    }
#ifdef TUVectorPP_DEBUG
	    std::cerr << "  niter = " << niter << ": " << off_diagonal();
#endif
	}
    }

    for (int m = 0; m < _Et.dim(); m++)	// sort singular values and vectors
	for (int n = m+1; n < _Et.dim(); n++)
	    if (fabs(_diagonal[n]) > fabs(_diagonal[m]))
	    {
		swap(_diagonal[m], _diagonal[n]);
		for (int j = 0; j < _Et.dim(); j++)
		{
		    const T	tmp = _Et[m][j];
		    _Et[m][j] = _Et[n][j];
		    _Et[n][j] = -tmp;
		}
		for (int j = 0; j < _Dt.dim(); j++)
		{
		    const T	tmp = _Dt[m][j];
		    _Dt[m][j] = _Dt[n][j];
		    _Dt[n][j] = -tmp;
		}
	    }

    int l = _Et.dim() - 1;		// last index
    for (int m = 0; m < l; m++)		// ensure positivity of all singular
	if (_diagonal[m] < 0.0)		// values except for the last one.
	{
	    _diagonal[m] = -_diagonal[m];
	    _diagonal[l] = -_diagonal[l];
	    for (int j = 0; j < _Et.dim(); j++)
	    {
		_Et[m][j] = -_Et[m][j];
		_Et[l][j] = -_Et[l][j];
	    }
	}
}

template <class T> bool
BiDiagonal<T>::diagonal_is_zero(int n) const
{
    return _Dt.sigma_is_zero(n, _anorm);
}

template <class T> bool
BiDiagonal<T>::off_diagonal_is_zero(int n) const
{
    return _Et.sigma_is_zero(n, _anorm);
}

template <class T> void
BiDiagonal<T>::initialize_rotation(int m, int n, double& x, double& y) const
{
    using namespace	std;
    
    const double	g = ((_diagonal[n]     + _diagonal[n-1])*
			     (_diagonal[n]     - _diagonal[n-1])+
			     (_off_diagonal[n] + _off_diagonal[n-1])*
			     (_off_diagonal[n] - _off_diagonal[n-1]))
			  / (2.0*_diagonal[n-1]*_off_diagonal[n]),
      // Caution!! You have to ensure that _diagonal[n-1] != 0
      // as well as _off_diagonal[n].
			absg = fabs(g),
			gg1 = (absg > 1.0 ?
			       absg * sqrt(1.0 + (1.0/absg)*(1.0/absg)) :
			       sqrt(1.0 + absg*absg)),
			t = (g > 0.0 ? g + gg1 : g - gg1);
    x = ((_diagonal[m] + _diagonal[n])*(_diagonal[m] - _diagonal[n]) -
	 _off_diagonal[n]*(_off_diagonal[n] + _diagonal[n-1]/t)) / _diagonal[m];
  //x = _diagonal[m];				// without shifting
    y = _off_diagonal[m+1];
}

/************************************************************************
*  class Minimization1<S>						*
************************************************************************/
/*
 *  Minimize 1-dimensional function using golden section search and minima
 *  is returned in x. Minimum value of the func is also returned as a return
 *  value.
 */
template <class S> S
Minimization1<S>::minimize(S& x, S w) const
{
#define W	0.38197
    using namespace	std;

    S	x1 = x, x2 = x + w, f1 = (*this)(x1), f2 = (*this)(x2);
    
    if (f1 < f2)			// guarantee that f1 >= f2
    {
	S	tmp = x1;		// swap x1 & x2
	x1  = x2;
	x2  = tmp;
	tmp = f1;			// swap f1 & f2
	f1  = f2;
	f2  = tmp;
    }
    S	x0;
    do
    {
	x0  = x1;
	x1  = x2;
	x2 += (1.0 / W - 1.0) * (x1 - x0);	// elongate to right
#ifdef MIN1_DEBUG
	S	f0 = f1;
#endif
	f1  = f2;
	f2  = (*this)(x2);
#ifdef MIN1_DEBUG
	std::cerr << "Bracketting: [" << x0 << ", " << x1 << ", " << x2
		  << "], (" << f0 << ", " << f1 << ", " << f2 << ")"
		  << std::endl;
#endif
    } while (f1 > f2);
    
  /* Golden section search */
    S	x3 = x2;
    if (fabs(x1 - x0) > fabs(x2 - x1))
    {
	x2  = x1;
	x1 -= W * (x2 - x0);		// insert new x1 between x0 & x2
	f2  = f1;
	f1  = (*this)(x1);
    }
    else
    {
	x2 -= (1.0 - W) * (x3 - x1);	// insert new x2 between x1 & x3
	f2  = (*this)(x2);
    }
#ifdef MIN1_DEBUG
    std::cerr << "Initial:     [" << x0 << ", " << x1 << ", " << x2
	      << ", " << x3 << "], (" << f1 << ", " << f2 << ")" << std::endl;
#endif
    int	i;
    for (i = 0;
	 i < _niter_max && fabs(x3 - x0) > _tol * (fabs(x1) + fabs(x2)); ++i)
    {
	if (f1 < f2)
	{
	    x3  = x2;			// shift x2 & x3 to left
	    x2  = x1;
	    x1 -= W * (x2 - x0);	// insert new x1 between x0 & x2
	    f2  = f1;
	    f1  = (*this)(x1);
	}
	else
	{
	    x0  = x1;			// shift x0 & x1 to right
	    x1  = x2;
	    x2 += W * (x3 - x1);	// insert new x2 between x1 & x3
	    f1  = f2;
	    f2  = (*this)(x2);
	}
#ifdef MIN1_DEBUG
	std::cerr << "Golden:      [" << x0 << ", " << x1 << ", " << x2
		  << ", " << x3 << "], (" << f1 << ", " << f2 << ")"
		  << std::endl;
#endif
    }
    if (i == _niter_max)
	throw std::runtime_error("TU::Minimization1<S>::minimize(): Too many iterations!!");

    if (f1 < f2)
    {
	x = x1;
	return f1;
    }
    else
    {
	x = x2;
	return f2;
    }
}

/************************************************************************
*  class Minimization<S, T>						*
************************************************************************/
/*
 *  Minimize multi-dimensional function using conjugate gradient method and
 *  minima is returned in x. Minimum value of the func is also returned as
 *  a return value.
 */
template <class S, class T> S
Minimization<S, T>::minimize(T& x)
{
    S		val = (*this)(x);
    Vector<S>	g   = ngrad(x), h = g;
    
    for (int i = 0; i < _niter_max; ++i)
    {
	if (_print)
	    print(i, val, x);

	const S		g_sqr = g * g;
	if (g_sqr == 0.0)
	    return val;

	const S		val_next = line_minimize(x, h);
	if (near_enough(val, val_next))
	    return val_next;
	val = val_next;

	const Vector<S>	g_next = ngrad(x);
	h = g_next + (((g_next - g) * g_next) / g_sqr) * h;
	g = g_next;
	update(x);
    }

    std::cerr << "TU::Minimization<S, T>::minimize(): Too many iterations!!"
	      << std::endl;
    return val;
}

/*
 *  Minimize multi-dimensional function using steepest descent method and
 *  minima is returned in x. Minimum value of the func is also returned as
 *  a return value.
 */
template <class S, class T> S
Minimization<S, T>::steepest_descent(T& x)
{
    S		val = (*this)(x);
    Vector<S>	g   = ngrad(x);
    
    for (int i = 0; i < _niter_max; ++i)
    {
	if (_print)
	    print(i, val, x);
	
	const S		g_sqr = g * g;
	if (g_sqr == 0.0)
	    return val;

	const S		val_next = line_minimize(x, g);
	if (near_enough(val, val_next))
	    return val_next;
	val = val_next;

	g = ngrad(x);
	update(x);
    }

    std::cerr << "TU::Minimization<S, T>::steepest_descent(): Too many iterations!!"
	      << std::endl;
    return val;
}

/*
 *  Minimize function along direction h and minima is returned in x.
 *  Minimum value of the function is also returned as a return value.
 */
template <class S, class T> S
Minimization<S, T>::line_minimize(T &x, const Vector<S>& h) const
{
    LineFunction	lfunc(*this, x, h, _tol, _niter_max);
    S			d = 0.0, val = lfunc.minimize(d, 1.0);
    x = proceed(x, d * h);
    return val;
}

/*
 *  Update the status of the function to be minimized.
 */
template <class S, class T> void
Minimization<S, T>::update(const T&)
{
}

/*
 *  Print intermediate values
 */
template <class S, class T> void
Minimization<S, T>::print(int i, S val, const T& x) const
{
    std::cerr << std::setw(3) << i << ": (" << val << ')' << x;
}
 
}
