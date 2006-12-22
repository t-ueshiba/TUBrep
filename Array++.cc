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
 *  $Id: Array++.cc,v 1.6 2006-12-22 00:05:55 ueshiba Exp $
 */
#include "TU/Array++.h"
#include <stdexcept>

namespace TU
{
/************************************************************************
*  class Array<T>							*
************************************************************************/
static inline u_int
_pdim(u_int i, u_int d, u_int a)
{
    return (i+d <= a ? d : i < a ? a-i : 0);
}

//! $B5-21NN0h$r85$NG[Ns$H6&M-$7$?ItJ,G[Ns$r:n$k(B
/*!
  \param a	$BG[Ns(B.
  \param i	$BItJ,G[Ns$NBh(B0$BMWAG$r;XDj$9$k(Bindex.
  \param d	$BItJ,G[Ns$N<!85(B($BMWAG?t(B).
*/
template <class T>
Array<T>::Array(const Array<T>& a, u_int i, u_int d)
    :_d(_pdim(i, d, a.dim())),
     _p(dim() > 0 ? (T*)&a[i] : 0), _siz(dim()), _shr(1)
{
#ifdef TUArrayPP_DEBUG
    std::cerr << "TU::Array<T>::Array(const Array<T>&, u_int, u_int) invoked!\n  this = "
	      << (void*)this << ", dim = " << dim()
	      << std::endl;
#endif
}

//! $B%3%T!<%3%s%9%H%i%/%?(B
/*!
  \param a	$B%3%T!<85$NG[Ns(B.
*/
template <class T>
Array<T>::Array(const Array<T>& a)
    :_d(a.dim()), _p(new T[dim()]), _siz(dim()), _shr(0)
{
#ifdef TUArrayPP_DEBUG
    std::cerr << "TU::Array<T>::Array(const Array<T>&) invoked!\n"
	      << "  this = " << (void*)this << ", dim = " << dim()
	      << std::endl;		
#endif
    for (int i = 0; i < dim(); ++i)
	(*this)[i] = a[i];
}

//! $BG[Ns$N%3%T!<(B
/*!
  \param a	$B%3%T!<85$NG[Ns(B.
*/
template <class T> Array<T>&
Array<T>::operator =(const Array<T>& a)
{
#ifdef TUArrayPP_DEBUG
    std::cerr << "TU::Array<T>::operator =(const Array<T>&) invoked!"
	      << std::endl;
#endif
    if (_p != a._p)
    {
	resize(a.dim());
	for (int i = 0; i < dim(); ++i)
	    (*this)[i] = a[i];
    }
    return *this;
}

//! $BG[Ns$NMWAG$X%"%/%;%9$9$k(B(index$B$N%A%'%C%/$"$j(B)
/*!
  \param i			$BMWAG$r;XDj$9$k(Bindex.
  \return			index$B$K$h$C$F;XDj$5$l$?MWAG(B.
  \throw std::out_of_range	0 <= i < dim()$B$G$J$$>l9g$KAw=P(B.
*/
template <class T> const T&
Array<T>::at(int i) const
{
    if (i < 0 || i >= dim())
	throw std::out_of_range("TU::Array<T>::at: invalid index!");
    return _p[i];
}

//! $BG[Ns$NMWAG$X%"%/%;%9$9$k(B(index$B$N%A%'%C%/$"$j(B)
/*!
  \param i			$BMWAG$r;XDj$9$k(Bindex.
  \return			index$B$K$h$C$F;XDj$5$l$?MWAG(B.
  \throw std::out_of_range	0 <= i < dim()$B$G$J$$>l9g$KAw=P(B.
*/
template <class T> T&
Array<T>::at(int i)
{
    if (i < 0 || i >= dim())
	throw std::out_of_range("TU::Array<T>::at: invalid index!");
    return _p[i];
}

//! $BA4$F$NMWAG$KF10l$N?tCM$rBeF~$9$k(B
/*!
  \param d	$BBeF~$9$kCM(B.
*/
template <class T> Array<T>&
Array<T>::operator =(double c)
{
    for (int i = 0; i < dim(); )
	(*this)[i++] = c;
    return *this;
}

//! $BA4$F$NMWAG$KF10l$N?tCM$r3]$1$k(B
/*!
  \param d	$B3]$1$kCM(B.
*/
template <class T> Array<T>&
Array<T>::operator *=(double c)
{
    for (int i = 0; i < dim(); )
	(*this)[i++] *= c;
    return *this;
}
    
//! $BA4$F$NMWAG$rF10l$N?tCM$G3d$k(B
/*!
  \param d	$B3d$kCM(B.
*/
template <class T> Array<T>&
Array<T>::operator /=(double c)
{
    for (int i = 0; i < dim(); )
	(*this)[i++] /= c;
    return *this;
}

//! $B3FMWAG$KB>$NG[Ns$NMWAG$rB-$9(B
/*!
  \param a	$BB-$9G[Ns(B.
*/
template <class T> Array<T>&
Array<T>::operator +=(const Array<T>& a)
{
    check_dim(a.dim());
    for (int i = 0; i < dim(); ++i)
	(*this)[i] += a[i];
    return *this;
}

//! $B3FMWAG$+$iB>$NG[Ns$NMWAG$r0z$/(B
/*!
  \param a	$B0z$/G[Ns(B.
*/
template <class T> Array<T>&
Array<T>::operator -=(const Array<T>& a)
{
    check_dim(a.dim());
    for (int i = 0; i < dim(); ++i)
	(*this)[i] -= a[i];
    return *this;
}

//! 2$B$D$NG[Ns$rMWAGKh$KHf3S$7!$F1$8$G$"$k$+D4$Y$k(B
/*!
  \param a	$BHf3SBP>]$H$J$kG[Ns(B.
  \return	$BA4$F$NMWAG$,F1$8$J$i$P(Btrue$B$r!$$=$&$G$J$1$l$P(Bfalse$B$rJV$9!%(B
*/
template <class T> bool
Array<T>::operator ==(const Array<T>& a) const
{
    if (dim() != a.dim())
	return false;
    for (int i = 0; i < dim(); ++i)
	if ((*this)[i] != a[i])
	    return false;
    return true;
}

//! $BF~NO%9%H%j!<%`$+$iG[Ns$rFI$_9~$`(B(binary)
/*!
  \param in	$BF~NO%9%H%j!<%`(B.
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B.
*/
template <class T> std::istream&
Array<T>::restore(std::istream& in)
{
    in.read((char*)_p, sizeof(T) * dim());
    return in;
}

//! $B=PNO%9%H%j!<%`$KG[Ns$r=q$-=P$9(B(binary)
/*!
  \param out	$B=PNO%9%H%j!<%`(B.
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B.
*/
template <class T> std::ostream&
Array<T>::save(std::ostream& out) const
{
    out.write((const char*)(T*)(*this), sizeof(T) * dim());
    return out;
}

//! $BG[Ns$N<!85$rJQ99$9$k(B
/*!
  $B$?$@$7!$B>$N%*%V%8%'%/%H$H5-21NN0h$r6&M-$7$F$$$kG[Ns$N<!85$rJQ99$9$k$3$H$O(B
  $B$G$-$J$$!%(B
  \param d			$B?7$7$$<!85(B.
  \return			d$B$,85$N<!85$h$j$bBg$-$1$l$P(Btrue$B$r!$$=$&$G$J(B
				$B$1$l$P(Bfalse$B$rJV$9(B.
  \throw std::logic_error	$B5-21NN0h$rB>$N%*%V%8%'%/%H$H6&M-$7$F$$$k>l9g(B
				$B$KAw=P(B.
*/
template <class T> bool
Array<T>::resize(u_int d)
{
    if (dim() == d)
	return false;
    
    if (_shr)
	throw std::logic_error("Array<T>::resize: cannot change dimension of shared array!");

    const u_int	old_dim = dim();
    if (_siz < (_d = d))
    {
	delete [] _p;
	_siz = dim();
	_p = new T[_siz];
    }
    return dim() > old_dim;
}

//! $BG[Ns$,FbIt$G;HMQ$9$k5-21NN0h$r;XDj$7$?$b$N$KJQ99$9$k(B
/*!
  \param p	$B?7$7$$5-21NN0h$X$N%]%$%s%?(B.
  \param d	$B?7$7$$<!85(B.
*/
template <class T> void
Array<T>::resize(T* p, u_int d)
{
    _d = d;
    if (!_shr)
	delete [] _p;
    _p = p;
    _shr = 1;
    _siz = d;
}

//! $B;XDj$7$?Id9f$J$7@0?tCM$,$3$NG[Ns$N<!85$K0lCW$9$k$+D4$Y$k(B
/*!
  \param d			$BD4$Y$?$$Id9f$J$7@0?tCM(B.
  \throw std::invalid_argument	d != dim()$B$N>l9g$KAw=P(B.
*/
template <class T> void
Array<T>::check_dim(u_int d) const
{
    if (dim() != d)
	throw
	    std::invalid_argument("Array<T>::check_dim: dimension mismatch!");
}

//! $BF~NO%9%H%j!<%`$+$i;XDj$7$?2U=j$KG[Ns$rFI$_9~$`(B(ASCII)
/*!
  \param in	$BF~NO%9%H%j!<%`(B.
  \param m	$BFI$_9~$_@h$N@hF,$r;XDj$9$k(Bindex.
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B.
*/
template <class T> std::istream&
Array<T>::get(std::istream& in, int m)
{
#define TUArrayPPBufSiz	2048
    Array<T>	tmp(TUArrayPPBufSiz);
    int		n = 0;
    
    while (n < tmp.dim())
    {
	char	c;
	while (in.get(c))		// skip white spaces other than '\n'
	    if (!isspace(c) || c == '\n')
		break;

	if (!in || c == '\n')
	{
	    resize(m + n);
	    break;
	}

	in.putback(c);
	in >> tmp[n++];
    }
    if (n == tmp.dim())
	get(in, m + n);

    for (int i = 0; i < n; ++i)
	(*this)[m + i] = tmp[i];

    return in;
}

//! $B=PNO%9%H%j!<%`$XG[Ns$r=q$-=P$9(B(ASCII)
/*!
  \param out	$B=PNO%9%H%j!<%`(B.
  \param a	$B=q$-=P$9G[Ns(B.
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B.
*/
template <class T> std::ostream&
operator <<(std::ostream& out, const Array<T>& a)
{
    for (int i = 0; i < a.dim(); )
	out << ' ' << a[i++];
    return out << std::endl;
}

/************************************************************************
*  class Array2<T>							*
************************************************************************/
//! $B5-21NN0h$r85$NG[Ns$H6&M-$7$?ItJ,G[Ns$r:n$k(B
/*!
  \param a	$BG[Ns(B.
  \param i	$BItJ,G[Ns$N:8>e6yMWAG$N9T$r;XDj$9$k(Bindex.
  \param j	$BItJ,G[Ns$N:8>e6yMWAG$NNs$r;XDj$9$k(Bindex.
  \param r	$BItJ,G[Ns$N9T?t(B.
  \param c	$BItJ,G[Ns$NNs?t(B.
*/
template <class T>
Array2<T>::Array2(const Array2<T>& a, u_int i, u_int j, u_int r, u_int c)
    :Array<T>(_pdim(i, r, a.nrow())), _cols(_pdim(j, c, a.ncol())),
     _ary((nrow() > 0 && ncol() > 0 ? (ET*)&a[i][j] : 0), nrow()*ncol())
{
#ifdef TUArrayPP_DEBUG
    std::cerr << "TU::Array2<T>::Array2(const Array2<T>&, u_int, u_int, u_int, u_int) invoked!\n"
	      << "  this = " << (void*)this << ", " << nrow() << 'x' << ncol()
	      << std::endl;
#endif
    for (int ii = 0; ii < nrow(); ++ii)
	(*this)[ii].resize((ET*)&a[i+ii][j], ncol());
}    

//! $B%3%T!<%3%s%9%H%i%/%?(B
/*!
  \param a	$B%3%T!<85$NG[Ns(B.
*/
template <class T>
Array2<T>::Array2(const Array2<T>& a)
    :Array<T>(a.nrow()), _cols(a.ncol()), _ary(nrow()*ncol())
{
#ifdef TUArrayPP_DEBUG
    std::cerr << "TU::Array2<T>::Array2(const Array2<T>&) invoked!\n"
	      << "  this = " << (void*)this << ", " << nrow() << 'x' << ncol()
	      << std::endl;
#endif
    set_rows();
    for (int i = 0; i < nrow(); ++i)
	(*this)[i] = a[i];
}    

//! $B%G%9%H%i%/%?(B
template <class T>
Array2<T>::~Array2()
{
}

//! $BG[Ns$N%3%T!<(B
/*!
  \param a	$B%3%T!<85$NG[Ns(B.
*/
template <class T> Array2<T>&
Array2<T>::operator =(const Array2<T>& a)
{
#ifdef TUArrayPP_DEBUG
    std::cerr << "TU::Array2<T>::operator =(const Array2<T>&) invoked!"
	      << std::endl;
#endif
    resize(a.nrow(), a.ncol());
    for (int i = 0; i < nrow(); ++i)
	(*this)[i] = a[i];
    return *this;
}

//! $BF~NO%9%H%j!<%`$+$iG[Ns$rFI$_9~$`(B(binary)
/*!
  \param in	$BF~NO%9%H%j!<%`(B.
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B.
*/
template <class T> std::istream&
Array2<T>::restore(std::istream& in)
{
    for (int i = 0; i < nrow(); )
	(*this)[i++].restore(in);
    return in;
}

//! $B=PNO%9%H%j!<%`$KG[Ns$r=q$-=P$9(B(binary)
/*!
  \param out	$B=PNO%9%H%j!<%`(B.
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B.
*/
template <class T> std::ostream&
Array2<T>::save(std::ostream& out) const
{
    for (int i = 0; i < nrow(); )
	(*this)[i++].save(out);
    return out;
}

//! $BG[Ns$N%5%$%:$rJQ99$9$k(B
/*!
  \param r	$B?7$7$$9T?t(B.
  \param c	$B?7$7$$Ns?t(B.
  \return	r$B$,85$N9T?t$h$jBg$-$$Kt$O(Bc$B$,85$NNs?t$H0[$J$l$P(Btrue$B$r!$(B
		$B$=$&$G$J$1$l$P(Bfalse$B$rJV$9(B.
*/
template <class T> bool
Array2<T>::resize(u_int r, u_int c)
{
    if (!Array<T>::resize(r) && ncol() == c)
	return false;

    _cols = c;
    _ary.resize(nrow()*ncol());
    set_rows();
    return true;
}

//! $BG[Ns$,FbIt$G;HMQ$9$k5-21NN0h$r;XDj$7$?$b$N$KJQ99$9$k(B
/*!
  \param p	$B?7$7$$5-21NN0h$X$N%]%$%s%?(B.
  \param r	$B?7$7$$9T?t(B.
  \param c	$B?7$7$$Ns?t(B.
*/
template <class T> void
Array2<T>::resize(ET* p, u_int r, u_int c)
{
    Array<T>::resize(r);
    _cols = c;
    _ary.resize(p, nrow()*ncol());
    set_rows();
}

template <class T> void
Array2<T>::set_rows()
{
    for (int i = 0; i < nrow(); ++i)
	(*this)[i].resize((ET*)*this + i*ncol(), ncol());
}

template <class T> std::istream&
Array2<T>::get(std::istream& in, int i, int j, int jmax)
{
    char		c;

    while (in.get(c))			// Skip white spaces other than '\n'.
	if (!isspace(c) || c == '\n')
	    break;

    if (!in || c == '\n')
    {
	++i;				// Proceed to the next row.
	if (j > jmax)
	    jmax = j;
	j = 0;				// Return to the first column.

	while (in.get(c))		// Skip white spaces other than '\n'.
	    if (!isspace(c) || c == '\n')
		break;

	if (!in || c == '\n')
	{
	    resize(i, jmax);
	    return in;
	}
    }
    in.putback(c);
    ET val;
    in >> val;
    get(in, i, j + 1, jmax);
    (*this)[i][j] = val;
    return in;
}    
 
}
