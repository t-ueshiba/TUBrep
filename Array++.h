/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$kEy$NCx(B
 *  $B:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: Array++.h,v 1.20 2008-08-06 07:51:44 ueshiba Exp $
 */
#ifndef __TUArrayPP_h
#define __TUArrayPP_h

#include <iostream>
#include <stdexcept>
#include "TU/types.h"
#ifdef __INTEL_COMPILER
#  include <mmintrin.h>
#endif

namespace TU
{
/************************************************************************
*  class Buf<T>								*
************************************************************************/
//! $B2DJQD9%P%C%U%!%/%i%9(B
/*!
  $BC1FH$G;HMQ$9$k$3$H$O$J$/!$(B#TU::Array<T, B>$B$^$?$O(B#TU::Array2<T, B>$B$N(B
  $BBh(B2$B%F%s%W%l!<%H0z?t$K;XDj$9$k$3$H$K$h$C$F!$$=$l$i$N4pDl%/%i%9$H$7$F;H$&!%(B
  \param T	$BMWAG$N7?(B
*/
template <class T>
class Buf
{
  public:
    explicit Buf(u_int siz=0)					;
    Buf(T* p, u_int siz)					;
    Buf(const Buf& b)						;
    Buf&		operator =(const Buf& b)		;
    ~Buf()							;

			operator T*()				;
			operator const T*()		const	;
    size_t		size()				const	;
    u_int		dim()				const	;
    bool		resize(u_int siz)			;
    void		resize(T* p, u_int siz)			;
    static u_int	align(u_int siz)			;
    std::istream&	get(std::istream& in, int m=0)		;
    std::ostream&	put(std::ostream& out)		const	;
    
  private:
    u_int	_size;		// the number of elements in the buffer
    T*		_p;		// pointer to the buffer area
    u_int	_shared	  :  1;	// buffer area is shared with other object
    u_int	_capacity : 31;	// buffer capacity (unit: element, >= _size)
};

//! $B;XDj$7$?MWAG?t$N%P%C%U%!$r@8@.$9$k!%(B
/*!
  \param siz	$BMWAG?t(B
*/
template <class T> inline
Buf<T>::Buf(u_int siz)
    :_size(siz), _p(new T[_size]), _shared(0), _capacity(_size)
{
}

//! $B30It$NNN0h$HMWAG?t$r;XDj$7$F%P%C%U%!$r@8@.$9$k!%(B
/*!
  \param p	$B30ItNN0h$X$N%]%$%s%?(B
  \param siz	$BMWAG?t(B
*/
template <class T> inline
Buf<T>::Buf(T* p, u_int siz)
    :_size(siz), _p(p), _shared(1), _capacity(_size)
{
}
    
//! $B%3%T!<%3%s%9%H%i%/%?(B
template <class T>
Buf<T>::Buf(const Buf<T>& b)
    :_size(b._size), _p(new T[_size]), _shared(0), _capacity(_size)
{
    for (int i = 0; i < _size; ++i)
	_p[i] = b._p[i];
}

//! $BI8=`BeF~1i;;;R(B
template <class T> Buf<T>&
Buf<T>::operator =(const Buf<T>& b)
{
    if (this != &b)
    {
	resize(b._size);
	for (int i = 0; i < _size; ++i)
	    _p[i] = b._p[i];
    }
    return *this;
}

//! $B%G%9%H%i%/%?(B
template <class T> inline
Buf<T>::~Buf()
{
    if (!_shared)
	delete [] _p;
}
    
//! $B%P%C%U%!$,;HMQ$9$kFbIt5-21NN0h$X$N%]%$%s%?$rJV$9!%(B
template <class T> inline
Buf<T>::operator T*()
{
    return _p;
}

//! $B%P%C%U%!$,;HMQ$9$kFbIt5-21NN0h$X$N%]%$%s%?$rJV$9!%(B
template <class T> inline
Buf<T>::operator const T*() const
{
    return _p;
}
    
//! $B%P%C%U%!$NMWAG?t!J<!85(B dim() $B$KEy$7$$!K$rJV$9!%(B
template <class T> inline size_t
Buf<T>::size() const
{
    return _size;
}
    
//! $B%P%C%U%!$N<!85!JMWAG?t(B size() $B$KEy$7$$!K$rJV$9!%(B
template <class T> inline u_int
Buf<T>::dim() const
{
    return _size;
}
    
//! $B%P%C%U%!$NMWAG?t$rJQ99$9$k!%(B
/*!
  $B$?$@$7!$B>$N%*%V%8%'%/%H$H5-21NN0h$r6&M-$7$F$$$k%P%C%U%!$NMWAG?t$r(B
  $BJQ99$9$k$3$H$O$G$-$J$$!%(B
  \param siz			$B?7$7$$MWAG?t(B
  \return			siz$B$,85$NMWAG?t$h$j$bBg$-$1$l$P(Btrue$B!$$=$&(B
				$B$G$J$1$l$P(Bfalse
  \throw std::logic_error	$B5-21NN0h$rB>$N%*%V%8%'%/%H$H6&M-$7$F$$$k>l9g(B
				$B$KAw=P(B
*/
template <class T> bool
Buf<T>::resize(u_int siz)
{
    if (_size == siz)
	return false;
    
    if (_shared)
	throw std::logic_error("Buf<T>::resize: cannot change dimension of shared array!");

    const u_int	old_size = _size;
    if (_capacity < (_size = siz))
    {
	delete [] _p;
	_p = new T[_size];
	_capacity = _size;
    }
    return _size > old_size;
}

//! $B%P%C%U%!$,FbIt$G;HMQ$9$k5-21NN0h$r;XDj$7$?$b$N$KJQ99$9$k!%(B
/*!
  \param p	$B?7$7$$5-21NN0h$X$N%]%$%s%?(B
  \param siz	$B?7$7$$MWAG?t(B
*/
template <class T> inline void
Buf<T>::resize(T* p, u_int siz)
{
    _size = siz;
    if (!_shared)
	delete [] _p;
    _p = p;
    _shared = 1;
    _capacity = _size;
}

//! $B5-21NN0h$r(Balign$B$9$k$?$a$KI,MW$JMWAG?t$rJV$9!%(B
/*!
  $BI,MW$J5-21MFNL$,%P%C%U%!$K$h$C$F7h$^$kFCDj$NCM$NG\?t$K$J$k$h$&!$M?$($i$l$?(B
  $BMWAG?t$r7+$j>e$2$k!%(B
  \param siz	$BMWAG?t(B
  \return	align$B$5$l$?MWAG?t(B
*/
template <class T> inline u_int
Buf<T>::align(u_int siz)
{
    return siz;
}
    
//! $BF~NO%9%H%j!<%`$+$i;XDj$7$?2U=j$KG[Ns$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \param m	$BFI$_9~$_@h$N@hF,$r;XDj$9$k(Bindex
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
template <class T> std::istream&
Buf<T>::get(std::istream& in, int m)
{
    const u_int	BufSiz = 2048;
    T		tmp[BufSiz];
    int		n = 0;
    
    while (n < BufSiz)
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
    if (n == BufSiz)
	get(in, m + n);

    for (int i = 0; i < n; ++i)
	_p[m + i] = tmp[i];

    return in;
}

//! $B=PNO%9%H%j!<%`$KG[Ns$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
template <class T> std::ostream&
Buf<T>::put(std::ostream& out) const
{
    for (int i = 0; i < _size; )
	out << ' ' << _p[i++];
    return out << std::endl;
}

#ifdef __INTEL_COMPILER
/************************************************************************
*  class AlignedBuf<T>							*
************************************************************************/
//! $B5-21NN0h$N%"%I%l%9$,(B16byte$B$NG\?t$K$J$k$h$&(Balign$B$5$l$?2DJQD9%P%C%U%!%/%i%9(B
/*!
  $BC1FH$G;HMQ$9$k$3$H$O$J$/!$(B#TU::Array<T, B>$B$^$?$O(B#TU::Array2<T, B>$B$N(B
  $BBh(B2$B%F%s%W%l!<%H0z?t$K;XDj$9$k$3$H$K$h$C$F!$$=$l$i$N4pDl%/%i%9$H$7$F;H$&!%(B
  \param T	$BMWAG$N7?(B
*/
template <class T>
class AlignedBuf : public Buf<T>
{
  public:
    explicit AlignedBuf(u_int siz=0)				;
    ~AlignedBuf()						;

    using		Buf<T>::operator T*;
    using		Buf<T>::operator const T*;
    using		Buf<T>::size;
    using		Buf<T>::dim;
    
    bool		resize(u_int siz)			;
    static u_int	align(u_int siz)			;
    
  private:
    static T*		memalign(u_int siz)			;

    enum		{ALIGN = 16};
    class LCM		//! sizeof(T)$B$H(BALIGN$B$N:G>.8xG\?t(B
    {
      public:
	LCM()							;
			operator u_int()		const	{return _val;}
      private:
	u_int		_val;
    };
    static const LCM	_lcm;
};

//! $B;XDj$7$?MWAG?t$N%P%C%U%!$r@8@.$9$k!%(B
/*!
  \param siz	$BMWAG?t(B
*/
template <class T> inline
AlignedBuf<T>::AlignedBuf(u_int siz)
    :Buf<T>(memalign(siz), siz)
{
}

//! $B%G%9%H%i%/%?(B
template <class T> inline
AlignedBuf<T>::~AlignedBuf()
{
    _mm_free((T*)*this);
}
    
//! $B%P%C%U%!$NMWAG?t$rJQ99$9$k!%(B
/*!
  \param siz	$B?7$7$$MWAG?t(B
  \return	siz$B$,85$NMWAG?t$HEy$7$1$l$P(Btrue$B!$$=$&$G$J$1$l$P(Bfalse
*/
template <class T> inline bool
AlignedBuf<T>::resize(u_int siz)
{
    if (siz == size())
	return false;

    _mm_free((T*)*this);
    Buf<T>::resize(memalign(siz), siz);
    return true;
}

//! $B5-21NN0h$r(Balign$B$9$k$?$a$KI,MW$JMWAG?t$rJV$9!%(B
/*!
  $BI,MW$J5-21MFNL$,(B16byte$B$NG\?t$K$J$k$h$&!$M?$($i$l$?MWAG?t$r7+$j>e$2$k!%(B
  \param siz	$BMWAG?t(B
  \return	align$B$5$l$?MWAG?t(B
*/
template <class T> inline u_int
AlignedBuf<T>::align(u_int siz)
{
  // _lcm * m >= sizeof(T) * siz $B$J$k:G>.$N(B m $B$r5a$a$k!%(B
    const int	m = (sizeof(T)*siz + _lcm - 1) / _lcm;
    return (_lcm * m) / sizeof(T);
}

template <class T> inline T*
AlignedBuf<T>::memalign(u_int siz)
{
    void*	p = _mm_malloc(sizeof(T)*siz, ALIGN);
    if (p == 0)
	throw std::runtime_error("AlignedBuf<T>::memalign(): failed to allocate memory!!");
    return (T*)p;
}

template <class T>
AlignedBuf<T>::LCM::LCM()
    :_val(ALIGN * sizeof(T))
{
  // sizeof(T)$B$H(BALIGN$B$N:GBg8xLs?t(B(GCD)$B$r5a$a$k!%(B
    u_int	gcd = ALIGN;
    for (u_int m = sizeof(T); m > 0; m -= gcd)
	if (m < gcd)
	    std::swap(m, gcd);

  // sizeof(T)$B$H(BALIGN$B$N:G>.8xG\?t(B(LCM)
    _val /= gcd;
}

template <class T> const AlignedBuf<T>::LCM	AlignedBuf<T>::_lcm;

#endif	// __INTEL_COMPILER
/************************************************************************
*  class FixedSizedBuf<T, D>						*
************************************************************************/
//! $BDj?t%5%$%:$N%P%C%U%!%/%i%9(B
/*!
  $BC1FH$G;HMQ$9$k$3$H$O$J$/!$(B#TU::Array<T, B>$B$NBh(B2$B%F%s%W%l!<%H0z?t$K;XDj$9$k(B
  $B$3$H$K$h$C$F(B#TU::Array<T, B>$B$N4pDl%/%i%9$H$7$F;H$&!%(B
  \param T	$BMWAG$N7?(B
  \param D	$B%P%C%U%!Cf$NMWAG?t(B
*/
template <class T, size_t D>
class FixedSizedBuf
{
  public:
    explicit FixedSizedBuf(u_int siz=D)			;
    FixedSizedBuf(T* p, u_int siz)				;
    FixedSizedBuf(const FixedSizedBuf& b)			;
    FixedSizedBuf&	operator =(const FixedSizedBuf& b)	;
    
			operator T*()				;
			operator const T*()		const	;
    static size_t	size()					;
    static u_int	dim()					;
    static bool		resize(u_int siz)			;
    void		resize(T* p, u_int siz)			;
    static u_int	align(u_int siz)			;
    std::istream&	get(std::istream& in)			;
    std::ostream&	put(std::ostream& out)		const	;

  private:
    T			_p[D];				// D-sized buffer
};

//! $B%P%C%U%!$r@8@.$9$k!%(B
/*!
  \param siz			$BMWAG?t(B
  \throw std::logic_error	siz$B$,%F%s%W%l!<%H%Q%i%a!<%?(BD$B$K0lCW$7$J$$>l9g$K(B
				$BAw=P(B
*/
template <class T, size_t D> inline
FixedSizedBuf<T, D>::FixedSizedBuf(u_int siz)
{
    resize(siz);
}

//! $B30It$NNN0h$HMWAG?t$r;XDj$7$F%P%C%U%!$r@8@.$9$k!J%@%_!<4X?t!K!%(B
/*!
  $B<B:]$O%P%C%U%!$,;HMQ$9$k5-21NN0h$O8GDj$5$l$F$$$FJQ99$G$-$J$$$N$G!$(B
  $B$3$N4X?t$O>o$KNc30$rAw=P$9$k!%(B
  \throw std::logic_error	$B$3$N4X?t$,8F$P$l$?$iI,$:Aw=P(B
*/
template <class T, size_t D> inline
FixedSizedBuf<T, D>::FixedSizedBuf(T* p, u_int siz)
{
    throw std::logic_error("FixedSizedBuf<T, D>::FixedSizedBuf(T*, u_int): cannot specify a pointer to external storage!!");
}

//! $B%3%T!<%3%s%9%H%i%/%?(B
template <class T, size_t D>
FixedSizedBuf<T, D>::FixedSizedBuf(const FixedSizedBuf<T, D>& b)
{
    for (int i = 0; i < D; ++i)
	_p[i] = b._p[i];
}

//! $BI8=`BeF~1i;;;R(B
template <class T, size_t D> FixedSizedBuf<T, D>&
FixedSizedBuf<T, D>::operator =(const FixedSizedBuf<T, D>& b)
{
    if (this != &b)
	for (int i = 0; i < D; ++i)
	    _p[i] = b._p[i];
    return *this;
}

//! $B%P%C%U%!$,;HMQ$9$kFbIt5-21NN0h$X$N%]%$%s%?$rJV$9!%(B
template <class T, size_t D> inline
FixedSizedBuf<T, D>::operator T*()
{
    return _p;
}

//! $B%P%C%U%!$,;HMQ$9$kFbIt5-21NN0h$X$N%]%$%s%?$rJV$9!%(B
template <class T, size_t D> inline
FixedSizedBuf<T, D>::operator const T*() const
{
    return _p;
}
    
//! $B%P%C%U%!$NMWAG?t!J<!85(B dim() $B$KEy$7$$!K$rJV$9!%(B
template <class T, size_t D> inline size_t
FixedSizedBuf<T, D>::size()
{
    return D;
}
    
//! $B%P%C%U%!$N<!85!JMWAG?t(B size() $B$KEy$7$$!K$rJV$9!%(B
template <class T, size_t D> inline u_int
FixedSizedBuf<T, D>::dim()
{
    return D;
}
    
//! $B%P%C%U%!$NMWAG?t$rJQ99$9$k!%(B
/*!
  $B<B:]$K$O%P%C%U%!$NMWAG?t$rJQ99$9$k$3$H$O$G$-$J$$$N$G!$M?$($i$l$?MWAG?t$,(B
  $B$3$N%P%C%U%!$NMWAG?t$KEy$7$$>l9g$N$_!$DL>o$I$*$j$K$3$N4X?t$+$i@)8f$,JV$k!%(B
  \param siz			$B?7$7$$MWAG?t(B
  \return			$B>o$K(Bfalse
  \throw std::logic_error	siz$B$,%F%s%W%l!<%H%Q%i%a!<%?(BD$B$K0lCW$7$J$$>l9g$K(B
				$BAw=P(B
*/
template <class T, size_t D> inline bool
FixedSizedBuf<T, D>::resize(u_int siz)
{
    if (siz != D)
	throw std::logic_error("FixedSizedBuf<T, D>::resize(u_int): cannot change buffer size!!");
    return false;
}
    
//! $B%P%C%U%!$,FbIt$G;HMQ$9$k5-21NN0h$r;XDj$7$?$b$N$KJQ99$9$k!%(B
/*!
  $B<B:]$K$O%P%C%U%!$N5-21NN0h$rJQ99$9$k$3$H$O$G$-$J$$$N$G!$M?$($i$l$?%]%$%s%?(B
  $B$HMWAG?t$,$3$N%P%C%U%!$N$=$l$i$KEy$7$$>l9g$N$_!$DL>o$I$*$j$K$3$N4X?t$+$i@)8f(B
  $B$,JV$k!%(B
  \param p			$B?7$7$$5-21NN0h$X$N%]%$%s%?(B
  \param siz			$B?7$7$$MWAG?t(B
  \throw std::logic_error	p$B$,$3$N%P%C%U%!$NFbIt5-21NN0h$K0lCW$7$J$$$+!$(B
				siz$B$,%F%s%W%l!<%H%Q%i%a!<%?(BD$B$K0lCW$7$J$$>l9g$K(B
				$BAw=P(B
*/
template <class T, size_t D> inline void
FixedSizedBuf<T, D>::resize(T* p, u_int siz)
{
    if (p != _p || siz != D)
	throw std::logic_error("FixedSizedBuf<T, D>::resize(T*, u_int): cannot specify a potiner to external storage!!");
}
    
//! $B5-21NN0h$r(Balign$B$9$k$?$a$KI,MW$JMWAG?t$rJV$9!%(B
/*!
  $BI,MW$J5-21MFNL$,%P%C%U%!$K$h$C$F7h$^$kFCDj$NCM$NG\?t$K$J$k$h$&!$M?$($i$l$?(B
  $BMWAG?t$r7+$j>e$2$k!%(B
  \param siz	$BMWAG?t(B
  \return	align$B$5$l$?MWAG?t(B
*/
template <class T, size_t D> inline u_int
FixedSizedBuf<T, D>::align(u_int siz)
{
    return siz;
}
    
//! $BF~NO%9%H%j!<%`$+$iG[Ns$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
template <class T, size_t D> std::istream&
FixedSizedBuf<T, D>::get(std::istream& in)
{
    for (int i = 0; i < D; ++i)
	in >> _p[i];
    return in;
}
    
//! $B=PNO%9%H%j!<%`$KG[Ns$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
template <class T, size_t D> std::ostream&
FixedSizedBuf<T, D>::put(std::ostream& out) const
{
    for (int i = 0; i < D; )
	out << ' ' << _p[i++];
    return out;
}

/************************************************************************
*  class Array<T, B>							*
************************************************************************/
//! B$B7?%P%C%U%!$K$h$C$F<BAu$5$l$k(BT$B7?%*%V%8%'%/%H$N(B1$B<!85G[Ns%/%i%9(B
/*!
  \param T	$BMWAG$N7?(B
  \param B	$B%P%C%U%!(B
*/
template <class T, class B=Buf<T> >
class Array : public B
{
  public:
    typedef T			ET;		  //!< $BMWAG$N7?(B
    typedef ET			value_type;	  //!< $BMWAG$N7?(B
    typedef ptrdiff_t		difference_type;  //!< $B%]%$%s%?4V$N:9(B
    typedef value_type&		reference;	  //!< $BMWAG$X$N;2>H(B
    typedef const value_type&	const_reference;  //!< $BDj?tMWAG$X$N;2>H(B
    typedef value_type*		pointer;	  //!< $BMWAG$X$N%]%$%s%?(B
    typedef const value_type*	const_pointer;	  //!< $BDj?tMWAG$X$N%]%$%s%?(B
    typedef pointer		iterator;	  //!< $BH?I|;R(B
    typedef const_pointer	const_iterator;	  //!< $BDj?tH?I|;R(B
    
  public:
    Array()								;
    explicit Array(u_int d)						;
    Array(T* p, u_int d)						;
    template <class B2>
    Array(const Array<T, B2>& a, int i, u_int d)			;
    template <class T2, class B2>
    Array(const Array<T2, B2>& a)					;
    template <class T2, class B2>
    Array&		operator =(const Array<T2, B2>& a)		;
    Array&		operator =(const T& c)				;

    iterator		begin()						;
    const_iterator	begin()					const	;
    iterator		end()						;
    const_iterator	end()					const	;

    using		B::size;
    using		B::dim;

			operator T*()					;
			operator const T*()			const	;
    T&			at(int i)					;
    const T&		at(int i)				const	;
    T&			operator [](int i)				;
    const T&		operator [](int i)			const	;
    Array&		operator *=(double c)				;
    Array&		operator /=(double c)				;
    template <class T2, class B2>
    Array&		operator +=(const Array<T2, B2>& a)		;
    template <class T2, class B2>
    Array&		operator -=(const Array<T2, B2>& a)		;
    template <class T2, class B2>
    bool		operator ==(const Array<T2, B2>& a)	const	;
    template <class T2, class B2>
    bool		operator !=(const Array<T2, B2>& a)	const	;
    std::istream&	restore(std::istream& in)			;
    std::ostream&	save(std::ostream& out)			const	;
    void		check_dim(u_int d)			const	;

  protected:
    static u_int	partial_dim(int i, u_int d, u_int a)		;
};

//! $BG[Ns$r@8@.$9$k!%(B
template <class T, class B> inline
Array<T, B>::Array()
    :B()
{
}

//! $B;XDj$7$?MWAG?t$NG[Ns$r@8@.$9$k!%(B
/*!
  \param d	$BG[Ns$NMWAG?t(B
*/
template <class T, class B> inline
Array<T, B>::Array(u_int d)
    :B(d)
{
}

//! $B30It$NNN0h$HMWAG?t$r;XDj$7$FG[Ns$r@8@.$9$k!%(B
/*!
  \param p	$B30ItNN0h$X$N%]%$%s%?(B
  \param d	$BG[Ns$NMWAG?t(B
*/
template <class T, class B> inline
Array<T, B>::Array(T* p, u_int d)
    :B(p, d)
{
}

//! $B5-21NN0h$r85$NG[Ns$H6&M-$7$?ItJ,G[Ns$r:n$k!%(B
/*!
  \param a	$BG[Ns(B
  \param i	$BItJ,G[Ns$NBh(B0$BMWAG$r;XDj$9$k(Bindex
  \param d	$BItJ,G[Ns$N<!85(B($BMWAG?t(B)
*/
template <class T, class B> template <class B2> inline
Array<T, B>::Array(const Array<T, B2>& a, int i, u_int d)
    :B((T*)&a[i], partial_dim(i, d, a.dim()))
{
}

//! $BB>$NG[Ns$HF10lMWAG$r;}$DG[Ns$r:n$k!J%3%T!<%3%s%9%H%i%/%?$N3HD%!K!%(B
/*!
  $B%3%T!<%3%s%9%H%i%/%?$OJL8D<+F0E*$K@8@.$5$l$k!%(B
  \param a	$B%3%T!<85$NG[Ns(B
*/
template <class T, class B> template <class T2, class B2>
Array<T, B>::Array(const Array<T2, B2>& a)
    :B(a.dim())
{
    for (int i = 0; i < dim(); ++i)
	(*this)[i] = a[i];
}
	
//! $BB>$NG[Ns$r<+J,$KBeF~$9$k!JI8=`BeF~1i;;;R$N3HD%!K!%(B
/*!
  $BI8=`BeF~1i;;;R$OJL8D<+F0E*$K@8@.$5$l$k!%(B
  \param a	$B%3%T!<85$NG[Ns(B
  \return	$B$3$NG[Ns(B
*/
template <class T, class B> template <class T2, class B2> Array<T, B>&
Array<T, B>::operator =(const Array<T2, B2>& a)
{
    resize(a.dim());
    for (int i = 0; i < dim(); ++i)
	(*this)[i] = a[i];
    return *this;
}
    
//! $BA4$F$NMWAG$KF10l$NCM$rBeF~$9$k!%(B
/*!
  \param c	$BBeF~$9$kCM(B
  \return	$B$3$NG[Ns(B
*/
template <class T, class B> Array<T, B>&
Array<T, B>::operator =(const T& c)
{
    for (int i = 0; i < dim(); )
	(*this)[i++] = c;
    return *this;
}

//! $BA4$F$NMWAG$KF10l$N?tCM$r3]$1$k!%(B
/*!
  \param c	$B3]$1$k?tCM(B
  \return	$B$3$NG[Ns(B
*/
template <class T, class B> Array<T, B>&
Array<T, B>::operator *=(double c)
{
    for (int i = 0; i < dim(); )
	(*this)[i++] *= c;
    return *this;
}

//! $BA4$F$NMWAG$rF10l$N?tCM$G3d$k!%(B
/*!
  \param c	$B3d$k?tCM(B
  \return	$B$3$NG[Ns(B
*/
template <class T, class B> Array<T, B>&
Array<T, B>::operator /=(double c)
{
    for (int i = 0; i < dim(); )
	(*this)[i++] /= c;
    return *this;
}

//! $BG[Ns$N@hF,MWAG$r;X$9H?I|;R$rJV$9!%(B
/*!
  \return	$B@hF,MWAG$r;X$9H?I|;R(B
*/
template <class T, class B> inline typename Array<T, B>::iterator
Array<T, B>::begin()
{
    return operator pointer();
}

//! $BG[Ns$N@hF,MWAG$r;X$9Dj?tH?I|;R$rJV$9!%(B
/*!
  \return	$B@hF,MWAG$r;X$9Dj?tH?I|;R(B
*/
template <class T, class B> inline typename Array<T, B>::const_iterator
Array<T, B>::begin() const
{
    return operator const_pointer();
}

//! $BG[Ns$NKvHx$r;X$9H?I|;R$rJV$9!%(B
/*!
  \return	$BKvHx$r;X$9H?I|;R(B
*/
template <class T, class B> inline typename Array<T, B>::iterator
Array<T, B>::end()
{
    return begin() + size();
}

//! $BG[Ns$NKvHx$r;X$9Dj?tH?I|;R$rJV$9!%(B
/*!
  \return	$BKvHx$r;X$9Dj?tH?I|;R(B
*/
template <class T, class B> inline typename Array<T, B>::const_iterator
Array<T, B>::end() const
{
    return begin() + size();
}

//! $BG[Ns$NMWAG$X%"%/%;%9$9$k!J(Bindex$B$N%A%'%C%/$"$j!K!%(B
/*!
  \param i			$BMWAG$r;XDj$9$k(Bindex
  \return			index$B$K$h$C$F;XDj$5$l$?MWAG(B
  \throw std::out_of_range	0 <= i < dim()$B$G$J$$>l9g$KAw=P(B
*/
template <class T, class B> inline T&
Array<T, B>::at(int i)
{
    if (i < 0 || i >= dim())
	throw std::out_of_range("TU::Array<T, B>::at: invalid index!");
    return (*this)[i];
}

//! $BG[Ns$NFbIt5-21NN0h$X$N%]%$%s%?$rJV$9!%(B
/*!
  \return	$BFbIt5-21NN0h$X$N%]%$%s%?(B
*/
template <class T, class B> inline
Array<T, B>::operator T*()
{
    return B::operator T*();
}

//! $BG[Ns$NFbIt5-21NN0h$X$N%]%$%s%?$rJV$9!%(B
/*!
  \return	$BFbIt5-21NN0h$X$N%]%$%s%?(B
*/
template <class T, class B> inline
Array<T, B>::operator const T*() const
{
    return B::operator const T*();
}

//! $BG[Ns$NMWAG$X%"%/%;%9$9$k!J(Bindex$B$N%A%'%C%/$"$j!K!%(B
/*!
  \param i			$BMWAG$r;XDj$9$k(Bindex
  \return			index$B$K$h$C$F;XDj$5$l$?MWAG(B
  \throw std::out_of_range	0 <= i < dim()$B$G$J$$>l9g$KAw=P(B
*/
template <class T, class B> inline const T&
Array<T, B>::at(int i) const
{
    if (i < 0 || i >= dim())
	throw std::out_of_range("TU::Array<T, B>::at: invalid index!");
    return (*this)[i];
}

//! $BG[Ns$NMWAG$X%"%/%;%9$9$k!J(Bindex$B$N%A%'%C%/$J$7!K!%(B
/*!
  \param i	$BMWAG$r;XDj$9$k(Bindex
  \return	index$B$K$h$C$F;XDj$5$l$?MWAG(B
*/
template <class T, class B> inline T&
Array<T, B>::operator [](int i)
{
    return Array::operator pointer()[i];
}

//! $BG[Ns$NMWAG$X%"%/%;%9$9$k!J(Bindex$B$N%A%'%C%/$J$7!K!%(B
/*!
  \param i	$BMWAG$r;XDj$9$k(Bindex
  \return	index$B$K$h$C$F;XDj$5$l$?MWAG(B
*/
template <class T, class B> inline const T&
Array<T, B>::operator [](int i) const
{
    return Array::operator const_pointer()[i];
}

//! $B$3$NG[Ns$KB>$NG[Ns$rB-$9!%(B
/*!
  \param a	$BB-$9G[Ns(B
  \return	$B$3$NG[Ns(B
*/
template <class T, class B> template <class T2, class B2> Array<T, B>&
Array<T, B>::operator +=(const Array<T2, B2>& a)
{
    check_dim(a.dim());
    for (int i = 0; i < dim(); ++i)
	(*this)[i] += a[i];
    return *this;
}

//! $B$3$NG[Ns$+$iB>$NG[Ns$r0z$/!%(B
/*!
  \param a	$B0z$/G[Ns(B
  \return	$B$3$NG[Ns(B
*/
template <class T, class B> template <class T2, class B2> Array<T, B>&
Array<T, B>::operator -=(const Array<T2, B2>& a)
{
    check_dim(a.dim());
    for (int i = 0; i < dim(); ++i)
	(*this)[i] -= a[i];
    return *this;
}

//! 2$B$D$NG[Ns$rMWAGKh$KHf3S$7!$F1$8$G$"$k$+D4$Y$k!%(B
/*!
  \param a	$BHf3SBP>]$H$J$kG[Ns(B
  \return	$BA4$F$NMWAG$,F1$8$J$i$P(Btrue$B$r!$$=$&$G$J$1$l$P(Bfalse
*/
template <class T, class B> template <class T2, class B2> bool
Array<T, B>::operator ==(const Array<T2, B2>& a) const
{
    if (dim() != a.dim())
	return false;
    for (int i = 0; i < dim(); ++i)
	if ((*this)[i] != a[i])
	    return false;
    return true;
}

//! 2$B$D$NG[Ns$rMWAGKh$KHf3S$7!$0[$J$k$b$N$,B8:_$9$k$+D4$Y$k!%(B
/*!
  \param a	$BHf3SBP>]$H$J$kG[Ns(B
  \return	$B0[$J$kMWAG$,B8:_$9$l$P(Btrue$B$r!$$=$&$G$J$1$l$P(Bfalse
*/
template <class T, class B> template <class T2, class B2> inline bool
Array<T, B>::operator !=(const Array<T2, B2>& a) const
{
    return !(*this == a);
}

//! $BF~NO%9%H%j!<%`$+$iG[Ns$rFI$_9~$`(B(binary)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
template <class T, class B> inline std::istream&
Array<T, B>::restore(std::istream& in)
{
    in.read((char*)(T*)*this, sizeof(T) * dim());
    return in;
}

//! $B=PNO%9%H%j!<%`$KG[Ns$r=q$-=P$9(B(binary)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
template <class T, class B> inline std::ostream&
Array<T, B>::save(std::ostream& out) const
{
    out.write((const char*)(const T*)*this, sizeof(T) * dim());
    return out;
}

//! $B;XDj$7$?CM$,$3$NG[Ns$NMWAG?t$K0lCW$9$k$+D4$Y$k!%(B
/*!
  \param d			$BD4$Y$?$$CM(B
  \throw std::invalid_argument	d != dim()$B$N>l9g$KAw=P(B
*/
template <class T, class B> inline void
Array<T, B>::check_dim(u_int d) const
{
    if (d != dim())
	throw std::invalid_argument("Array<T, B>::check_dim: dimension mismatch!");
}

template <class T, class B> inline u_int
Array<T, B>::partial_dim(int i, u_int d, u_int a)
{
    return (i+d <= a ? d : i < a ? a-i : 0);
}

//! $BF~NO%9%H%j!<%`$+$iG[Ns$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \param a	$BG[Ns$NFI$_9~$_@h(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
template <class T, class B> inline std::istream&
operator >>(std::istream& in, Array<T, B>& a)
{
    return a.get(in >> std::ws);
}

//! $B=PNO%9%H%j!<%`$XG[Ns$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \param a	$B=q$-=P$9G[Ns(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
template <class T, class B> inline std::ostream&
operator <<(std::ostream& out, const Array<T, B>& a)
{
    return a.put(out);
}

/************************************************************************
*  class Array2<T, B>							*
************************************************************************/
//! 1$B<!85G[Ns(BT$B$N(B1$B<!85G[Ns$H$7$FDj5A$5$l$?(B2$B<!85G[Ns%/%i%9(B
/*!
  \param T	1$B<!85G[Ns$N7?(B
  \param B	$B%P%C%U%!(B
*/
template <class T, class B=Buf<typename T::ET> >
class Array2 : public Array<T>
{
  public:
    typedef T			RT;		  //!< $B9T$N7?(B
    typedef RT			row_type;	  //!< $B9T$N7?(B
    typedef typename T::ET	ET;		  //!< $BMWAG$N7?(B
    typedef ET			value_type;	  //!< $BMWAG$N7?(B
    typedef ptrdiff_t		difference_type;  //!< $B%]%$%s%?4V$N:9(B
    typedef value_type&		reference;	  //!< $BMWAG$X$N;2>H(B
    typedef const value_type&	const_reference;  //!< $BDj?tMWAG$X$N;2>H(B
    typedef value_type*		pointer;	  //!< $BMWAG$X$N%]%$%s%?(B
    typedef const value_type*	const_pointer;	  //!< $BDj?tMWAG$X$N%]%$%s%?(B

  public:
    explicit Array2(u_int r=0, u_int c=0)				;
    Array2(ET* p, u_int r, u_int c)					;
    template <class B2>
    Array2(const Array2<T, B2>& a, int i, int j, u_int r, u_int c)	;
    Array2(const Array2& a)						;
    template <class T2, class B2>
    Array2(const Array2<T2, B2>& a)					;
    Array2&		operator =(const Array2& a)			;
    template <class T2, class B2>
    Array2&		operator =(const Array2<T2, B2>& a)		;
    Array2&		operator =(const ET& c)				;

    using		Array<T>::begin;
    using		Array<T>::end;
    using		Array<T>::size;
    using		Array<T>::dim;
    
			operator ET*()					;
			operator const ET*()			const	;
    u_int		nrow()					const	;
    u_int		ncol()					const	;
    bool		resize(u_int r, u_int c)			;
    void		resize(ET* p, u_int r, u_int c)			;
    std::istream&	restore(std::istream& in)			;
    std::ostream&	save(std::ostream& out)			const	;
    std::istream&	get(std::istream& in,
			    int i=0, int j=0, int jmax=0)		;

  private:
    void		set_rows()					;
    
    u_int		_ncol;
    B			_buf;
};

//! $B9T?t$HNs?t$r;XDj$7$F(B2$B<!85G[Ns$r@8@.$9$k!%(B
/*!
  \param r	$B9T?t(B
  \param c	$BNs?t(B
*/
template <class T, class B> inline
Array2<T, B>::Array2(u_int r, u_int c)
    :Array<T>(r), _ncol(c), _buf(nrow()*B::align(ncol()))
{
    set_rows();
}

//! $B30It$NNN0h$H9T?t$*$h$SNs?t$r;XDj$7$F(B2$B<!85G[Ns$r@8@.$9$k!%(B
/*!
  \param p	$B30ItNN0h$X$N%]%$%s%?(B
  \param r	$B9T?t(B
  \param c	$BNs?t(B
*/
template <class T, class B> inline
Array2<T, B>::Array2(ET* p, u_int r, u_int c)
    :Array<T>(r), _ncol(c), _buf(p, nrow()*B::align(ncol()))
{
    set_rows();
}

//! $B5-21NN0h$r85$NG[Ns$H6&M-$7$?ItJ,G[Ns$r:n$k(B
/*!
  \param a	$BG[Ns(B
  \param i	$BItJ,G[Ns$N:8>e6yMWAG$N9T$r;XDj$9$k(Bindex
  \param j	$BItJ,G[Ns$N:8>e6yMWAG$NNs$r;XDj$9$k(Bindex
  \param r	$BItJ,G[Ns$N9T?t(B
  \param c	$BItJ,G[Ns$NNs?t(B
*/
template <class T, class B> template <class B2>
Array2<T, B>::Array2(const Array2<T, B2>& a, int i, int j, u_int r, u_int c)
    :Array<T>(Array<T>::partial_dim(i, r, a.nrow())),
     _ncol(Array<T>::partial_dim(j, c, a.ncol())),
     _buf((nrow() > 0 && ncol() > 0 ? (ET*)&a[i][j] : 0),
	  nrow()*B::align(ncol()))
{
    for (int ii = 0; ii < nrow(); ++ii)
	(*this)[ii].resize((ET*)&a[i+ii][j], ncol());
}    

//! $B%3%T!<%3%s%9%H%i%/%?(B
/*!
  \param a	$B%3%T!<85$NG[Ns(B
*/
template <class T, class B> inline
Array2<T, B>::Array2(const Array2& a)
    :Array<T>(a.nrow()), _ncol(a.ncol()), _buf(nrow()*B::align(ncol()))
{
    set_rows();
    Array<T>::operator =(a);
}    

//! $BB>$NG[Ns$HF10lMWAG$r;}$DG[Ns$r:n$k!J%3%T!<%3%s%9%H%i%/%?$N3HD%!K!%(B
/*!
  $B%3%T!<%3%s%9%H%i%/%?$rDj5A$7$J$$$H<+F0E*$K:n$i$l$F$7$^$&$N$G!$(B
  $B$3$N%3%s%9%H%i%/%?$,$"$C$F$b%3%T!<%3%s%9%H%i%/%?$rJL8D$KDj5A(B
  $B$7$J$1$l$P$J$i$J$$!%(B
  \param a	$B%3%T!<85$NG[Ns(B
*/
template <class T, class B> template <class T2, class B2> inline
Array2<T, B>::Array2(const Array2<T2, B2>& a)
    :Array<T>(a.nrow()), _ncol(a.ncol()), _buf(nrow()*B::align(ncol()))
{
    set_rows();
    Array<T>::operator =(a);
}    

//! $BI8=`BeF~1i;;;R(B
/*!
  \param a	$B%3%T!<85$NG[Ns(B
  \return	$B$3$NG[Ns(B
*/
template <class T, class B> inline Array2<T, B>&
Array2<T, B>::operator =(const Array2& a)
{
    resize(a.nrow(), a.ncol());
    Array<T>::operator =(a);
    return *this;
}

//! $BB>$NG[Ns$r<+J,$KBeF~$9$k!JI8=`BeF~1i;;;R$N3HD%!K!%(B
/*!
  $BI8=`BeF~1i;;;R$rDj5A$7$J$$$H<+F0E*$K:n$i$l$F$7$^$&$N$G!$$3$NBeF~1i;;;R$,(B
  $B$"$C$F$bI8=`BeF~1i;;;R$rJL8D$KDj5A$7$J$1$l$P$J$i$J$$!%(B
  \param a	$B%3%T!<85$NG[Ns(B
  \return	$B$3$NG[Ns(B
*/
template <class T, class B> template <class T2, class B2> inline Array2<T, B>&
Array2<T, B>::operator =(const Array2<T2, B2>& a)
{
    resize(a.nrow(), a.ncol());
    Array<T>::operator =(a);
    return *this;
}

//! $BA4$F$NMWAG$KF10l$NCM$rBeF~$9$k!%(B
/*!
  \param c	$BBeF~$9$kCM(B
  \return	$B$3$NG[Ns(B
*/
template <class T, class B> Array2<T, B>&
Array2<T, B>::operator =(const ET& c)
{
    for (int i = 0; i < nrow(); )
	(*this)[i++] = c;
    return *this;
}

//! 2$B<!85G[Ns$NFbIt5-21NN0h$X$N%]%$%s%?$rJV$9!%(B
/*!
  \return	$BFbIt5-21NN0h$X$N%]%$%s%?(B
*/
template <class T, class B> inline
Array2<T, B>::operator typename T::ET*()
{
    return _buf.operator ET*();
}

//! 2$B<!85G[Ns$NFbIt5-21NN0h$X$N%]%$%s%?$rJV$9!%(B
/*!
  \return	$BFbIt5-21NN0h$X$N%]%$%s%?(B
*/
template <class T, class B> inline
Array2<T, B>::operator const typename T::ET*() const
{
    return _buf.operator const ET*();
}

//! 2$B<!85G[Ns$N9T?t$rJV$9!%(B
/*!
  \return	$B9T?t(B
*/
template <class T, class B> inline u_int
Array2<T, B>::nrow() const
{
    return size();
}

//! 2$B<!85G[Ns$NNs?t$rJV$9!%(B
/*!
  \return	$BNs?t(B
*/
template <class T, class B> inline u_int
Array2<T, B>::ncol() const
{
    return _ncol;
}

//! $BG[Ns$N%5%$%:$rJQ99$9$k!%(B
/*!
  \param r	$B?7$7$$9T?t(B
  \param c	$B?7$7$$Ns?t(B
  \return	r$B$,85$N9T?t$h$jBg$-$$Kt$O(Bc$B$,85$NNs?t$H0[$J$l$P(Btrue$B!$(B
		$B$=$&$G$J$1$l$P(Bfalse
*/
template <class T, class B> bool
Array2<T, B>::resize(u_int r, u_int c)
{
    if (!Array<T>::resize(r) && ncol() == c)
	return false;

    _ncol = c;
    _buf.resize(nrow()*B::align(ncol()));
    set_rows();
    return true;
}

//! $BG[Ns$,FbIt$G;HMQ$9$k5-21NN0h$r;XDj$7$?$b$N$KJQ99$9$k!%(B
/*!
  \param p	$B?7$7$$5-21NN0h$X$N%]%$%s%?(B
  \param r	$B?7$7$$9T?t(B
  \param c	$B?7$7$$Ns?t(B
*/
template <class T, class B> void
Array2<T, B>::resize(ET* p, u_int r, u_int c)
{
    Array<T>::resize(r);
    _ncol = c;
    _buf.resize(p, nrow()*B::align(ncol()));
    set_rows();
}

//! $BF~NO%9%H%j!<%`$+$iG[Ns$rFI$_9~$`(B(binary)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
template <class T, class B> std::istream&
Array2<T, B>::restore(std::istream& in)
{
    for (int i = 0; i < nrow(); )
	(*this)[i++].restore(in);
    return in;
}

//! $B=PNO%9%H%j!<%`$KG[Ns$r=q$-=P$9(B(binary)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
template <class T, class B> std::ostream&
Array2<T, B>::save(std::ostream& out) const
{
    for (int i = 0; i < nrow(); )
	(*this)[i++].save(out);
    return out;
}

//! $BF~NO%9%H%j!<%`$+$i;XDj$7$?2U=j$K(B2$B<!85G[Ns$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \param i	$BFI$_9~$_@h$N@hF,9T$r;XDj$9$k(Bindex
  \param j	$BFI$_9~$_@h$N@hF,Ns$r;XDj$9$k(Bindex
  \param jmax	$B$3$l$^$G$KFI$s$G$-$?MWAG$NNsHV9f$N:GBgCM(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
template <class T, class B> std::istream&
Array2<T, B>::get(std::istream& in, int i, int j, int jmax)
{
    char	c;

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
    ET	val;
    in >> val;
    get(in, i, j + 1, jmax);
    (*this)[i][j] = val;
    return in;
}    

template <class T, class B> void
Array2<T, B>::set_rows()
{
    const u_int	stride = B::align(ncol());
    for (int i = 0; i < nrow(); ++i)
	(*this)[i].resize((ET*)*this + i*stride, ncol());
}
    
//! $BF~NO%9%H%j!<%`$+$iG[Ns$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \param a	$BG[Ns$NFI$_9~$_@h(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
template <class T, class B> inline std::istream&
operator >>(std::istream& in, Array2<T, B>& a)
{
    return a.get(in >> std::ws);
}

}

#endif	/* !__TUArrayPP_h */
