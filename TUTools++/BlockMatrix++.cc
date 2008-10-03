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
 *  $Id: BlockMatrix++.cc,v 1.9 2008-10-03 04:23:37 ueshiba Exp $
 */
#include "TU/BlockMatrix++.h"
#include <stdexcept>

namespace TU
{
/************************************************************************
*  class BlockMatrix<T>							*
************************************************************************/
//! $B3F>.9TNs$N%5%$%:$r;XDj$7$F%V%m%C%/BP3Q9TNs$r@8@.$7!$A4MWAG$r(B0$B$G=i4|2=$9$k!%(B
/*!
  \param nrows	$B3F>.9TNs$N9T?t$r=g$K<}$a$?G[Ns(B
  \param ncols	$B3F>.9TNs$NNs?t$r=g$K<}$a$?G[Ns(B
*/
template <class T>
BlockMatrix<T>::BlockMatrix(const Array<u_int>& nrows,
			    const Array<u_int>& ncols)
    :Array<Matrix<T> >(nrows.dim())
{
    if (nrows.dim() != ncols.dim())
	throw std::invalid_argument("TU::BlockMatrix<T>::BlockMatrix: dimension mismatch between nrows and ncols!!");
    for (int i = 0; i < dim(); ++i)
    {
	(*this)[i].resize(nrows[i], ncols[i]);
	(*this)[i] = 0.0;
    }
}

//! $B%V%m%C%/BP3Q9TNs$NAm9T?t$rJV$9!%(B
/*!
  \return	$BAm9T?t(B
*/
template <class T> u_int
BlockMatrix<T>::nrow() const
{
    size_t	r = 0;
    for (int i = 0; i < dim(); ++i)
	r += (*this)[i].nrow();
    return r;
}

//! $B%V%m%C%/BP3Q9TNs$NAmNs?t$rJV$9!%(B
/*!
  \return	$BAmNs?t(B
*/
template <class T> u_int
BlockMatrix<T>::ncol() const
{
    size_t	c = 0;
    for (int i = 0; i < dim(); ++i)
	c += (*this)[i].ncol();
    return c;
}

//! $B$3$N%V%m%C%/BP3Q9TNs$NE>CV9TNs$rJV$9!%(B
/*!
  \return	$BE>CV9TNs!$$9$J$o$A(B
  \f$
  \TUtvec{B}{} =
  \TUbeginarray{cccc}
  \TUtvec{B}{1} & & & \\ & \TUtvec{B}{2} & & \\ & & \ddots & \\
  & & & \TUtvec{B}{d}
  \TUendarray
  \f$
*/
template <class T> BlockMatrix<T>
BlockMatrix<T>::trns() const
{
    BlockMatrix	val(dim());
    for (int i = 0; i < val.dim(); ++i)
	val[i] = (*this)[i].trns();
    return val;
}

//! $B$3$N%V%m%C%/BP3Q9TNs$NA4$F$N>.9TNs$NA4MWAG$KF10l$N?tCM$rBeF~$9$k!%(B
/*!
  \param c	$BBeF~$9$k?tCM(B
  \return	$B$3$N%V%m%C%/BP3Q9TNs(B
*/
template <class T> BlockMatrix<T>&
BlockMatrix<T>::operator =(const T& c)
{
    for (int i = 0; i < dim(); ++i)
	(*this)[i] = c;
    return *this;
}

//! $B$3$N%V%m%C%/BP3Q9TNs$rDL>o$N9TNs$KJQ49$9$k!%(B
/*!
  \return	$BJQ49$5$l$?9TNs(B
*/
template <class T>
BlockMatrix<T>::operator Matrix<T>() const
{
    Matrix<T>	val(nrow(), ncol());
    for (int r = 0, c = 0, i = 0; i < dim(); ++i)
    {
	val(r, c, (*this)[i].nrow(), (*this)[i].ncol()) = (*this)[i];
	r += (*this)[i].nrow();
	c += (*this)[i].ncol();
    }
    return val;
}

/************************************************************************
*  numeric operators							*
************************************************************************/
//! 2$B$D$N%V%m%C%/BP3Q9TNs$N@Q(B
/*!
  \param a	$BBh(B1$B0z?t(B
  \param b	$BBh(B2$B0z?t(B
  \return	$B7k2L$N%V%m%C%/BP3Q9TNs(B
*/
template <class T> BlockMatrix<T>
operator *(const BlockMatrix<T>& a, const BlockMatrix<T>& b)
{
    a.check_dim(b.dim());
    BlockMatrix<T>	val(a.dim());
    for (int i = 0; i < val.dim(); ++i)
	val[i] = a[i] * b[i];
    return val;
}

//! $B%V%m%C%/BP3Q9TNs$HDL>o$N9TNs$N@Q(B
/*!
  \param b	$BBh(B1$B0z?t(B($B%V%m%C%/BP3Q9TNs(B)
  \param m	$BBh(B2$B0z?t(B($BDL>o9TNs(B)
  \return	$B7k2L$NDL>o9TNs(B
*/
template <class T> Matrix<T>
operator *(const BlockMatrix<T>& b, const Matrix<T>& m)
{
    Matrix<T>	val(b.nrow(), m.ncol());
    int		r = 0, c = 0;
    for (int i = 0; i < b.dim(); ++i)
    {
	val(r, 0, b[i].nrow(), m.ncol())
	    = b[i] * m(c, 0, b[i].ncol(), m.ncol());
	r += b[i].nrow();
	c += b[i].ncol();
    }
    if (c != m.nrow())
	throw std::invalid_argument("TU::operaotr *(const BlockMatrix<T>&, const Matrix<T>&): dimension mismatch!!");
    return val;
}

//! $BDL>o$N9TNs$H%V%m%C%/BP3Q9TNs$N@Q(B
/*!
  \param m	$BBh(B1$B0z?t(B($BDL>o9TNs(B)
  \param b	$BBh(B2$B0z?t(B($B%V%m%C%/BP3Q9TNs(B)
  \return	$B7k2L$NDL>o9TNs(B
*/
template <class T> Matrix<T>
operator *(const Matrix<T>& m, const BlockMatrix<T>& b)
{
    Matrix<T>	val(m.nrow(), b.ncol());
    int		r = 0, c = 0;
    for (int i = 0; i < b.dim(); ++i)
    {
	val(0, c, m.nrow(), b[i].ncol())
	    = m(0, r, m.nrow(), b[i].nrow()) * b[i];
	r += b[i].nrow();
	c += b[i].ncol();
    }
    if (r != m.ncol())
	throw std::invalid_argument("TU::operaotr *(const Matrix<T>&, const BlockMatrix<T>&): dimension mismatch!!");
    return val;
}

//! $B%V%m%C%/BP3Q9TNs$H%Y%/%H%k$N@Q(B
/*!
  \param b	$B%V%m%C%/BP3Q9TNs(B
  \param v	$B%Y%/%H%k(B
  \return	$B7k2L$N%Y%/%H%k(B
*/
template <class T> Vector<T>
operator *(const BlockMatrix<T>& b, const Vector<T>& v)
{
    Vector<T>	val(b.nrow());
    int		r = 0, c = 0;
    for (int i = 0; i < b.dim(); ++i)
    {
	val(r, b[i].nrow()) = b[i] * v(c, b[i].ncol());
	r += b[i].nrow();
	c += b[i].ncol();
    }
    if (c != v.dim())
	throw std::invalid_argument("TU::operaotr *(const BlockMatrix<T>&, const Vector<T>&): dimension mismatch!!");
    return val;
}

//! $B%Y%/%H%k$H%V%m%C%/BP3Q9TNs$N@Q(B
/*!
  \param v	$B%Y%/%H%k(B
  \param b	$B%V%m%C%/BP3Q9TNs(B
  \return	$B7k2L$N%Y%/%H%k(B
*/
template <class T> Vector<T>
operator *(const Vector<T>& v, const BlockMatrix<T>& b)
{
    Vector<T>	val(b.ncol());
    int		r = 0, c = 0;
    for (int i = 0; i < b.dim(); ++i)
    {
	val(c, b[i].ncol()) = v(r, b[i].nrow()) * b[i];
	r += b[i].nrow();
	c += b[i].ncol();
    }
    if (r != v.dim())
	throw std::invalid_argument("TU::operaotr *(const Vector<T>&, const BlockMatrix<T>&): dimension mismatch!!");
    return val;
}
 
}
