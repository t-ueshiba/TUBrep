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
 *  $Id: Array++.h,v 1.2 2002-07-25 02:38:03 ueshiba Exp $
 */
#ifndef __TUArrayPP_h
#define __TUArrayPP_h

#include <iostream>
#include "TU/types.h"

namespace TU
{
/************************************************************************
*  class Array<T>							*
************************************************************************/
/*!
  T$B7?%*%V%8%'%/%H$N(B1$B<!85G[Ns%/%i%9(B
*/
template <class T>
class Array
{
  public:
    typedef T	ET;				//!< $BG[NsMWAG$N7?(B

  //! $BMWAG?t$r;XDj$7$FG[Ns$r@8@.$9$k(B
    explicit Array(u_int d=0):_d(d), _p(new T[_d]), _shr(0), _siz(_d)	{}
  //! $B30It$NNN0h$H$=$N%5%$%:$r;XDj$7$FG[Ns$r@8@.$9$k(B
    Array(T* p, u_int d) :_d(d), _p(p),	  _shr(1), _siz(_d)	{}
    Array(const Array&, u_int, u_int)	;
    Array(const Array&)			;
    ~Array()					{if (!_shr) delete [] _p;}
    Array&	operator =(const Array&)	;

  //! $BG[Ns$NMWAG?t$rJV$9(B
    u_int	dim()				const	{return _d;}
  //! $BG[Ns$NFbIt5-21NN0h$X$N%]%$%s%?$rJV$9(B
    		operator T*()			const	{return _p;}
  //! $BG[Ns$NMWAG$X%"%/%;%9$9$k(B(index$B$N%A%'%C%/$"$j(B)
    const ET&	at(int i)			const	;
  //! $BG[Ns$NMWAG$X%"%/%;%9$9$k(B(index$B$N%A%'%C%/$"$j(B)
    ET&		at(int i)				;
  //! $BG[Ns$NMWAG$X%"%/%;%9$9$k(B(index$B$N%A%'%C%/$J$7(B)
    const ET&	operator [](int i)		const	{return _p[i];}
  //! $BG[Ns$NMWAG$X%"%/%;%9$9$k(B(index$B$N%A%'%C%/$J$7(B)
    ET&		operator [](int i)			{return _p[i];}
    Array&	operator  =(double c)			;
    Array&	operator *=(double c)			;
    Array&	operator /=(double c)			;
    Array&	operator +=(const Array& a)		;
    Array&	operator -=(const Array& a)		;
    bool	operator ==(const Array& a)	const	;
  //! 2$B$D$NG[Ns$rMWAGKh$KHf3S$7!$0[$J$k$b$N$,B8:_$9$k$+$rD4$Y$k(B
    bool	operator !=(const Array& a)	const	{return !(*this == a);}
  //! $B$9$Y$F$NMWAG$NId9f$rH?E>$7$?G[Ns$rJV$9(B
    Array	operator  -()			const	{Array r(*this);
							 r *= -1; return r;}
    std::istream&	restore(std::istream&)		;
    std::ostream&	save(std::ostream&)	const	;
    bool		resize(u_int)			;
    void		resize(T*, u_int)		;
    void		check_dim(u_int)	const	;
    std::istream&	get(std::istream&, int)		;
    
  private:
    u_int	_d;		// dimension of array
    ET*		_p;		// pointer to buffer area
    u_int	_shr : 1;	// buffer area is shared with other object
    u_int	_siz : 31;	// buffer size (unit: element, >= dim())
	
  //    friend std::istream&	operator >>(std::istream& in, Array& a)	;
};

template <class T> inline std::istream&
operator >>(std::istream& in, Array<T>& a)
{
    return a.get(in >> std::ws, 0);
}

template <class T> std::ostream&
operator <<(std::ostream&, const Array<T>&);

/************************************************************************
*  class Array2<T>							*
************************************************************************/
/*!
  T$B7?G[Ns$N(B1$B<!85G[Ns$H$7$FDj5A$5$l$?(B2$B<!85G[Ns%/%i%9(B
*/
template <class T>
class Array2 : public Array<T>
{
  public:
    typedef typename T::ET	ET;		//! T$B7?G[Ns$NMWAG$N7?(B
    
  //! $B9T?t$HNs?t$r;XDj$7$F(B2$B<!85G[Ns$r@8@.$9$k(B
    explicit Array2(u_int r=0, u_int c=0)
	:Array<T>(r), _cols(c), _ary(nrow()*ncol())	{set_rows();}
  //! $B30It$NNN0h$H$=$N9T?t$*$h$SNs?t$r;XDj$7$F(B2$B<!85G[Ns$r@8@.$9$k(B
    Array2(ET* p, u_int r, u_int c)
	:Array<T>(r), _cols(c), _ary(p, nrow()*ncol())	{set_rows();}
    Array2(const Array2&, u_int, u_int, u_int, u_int)	;
    Array2(const Array2& a)					;
    Array2&	operator =(const Array2& a)			;
    virtual ~Array2()						;

  //! 2$B<!85G[Ns$N9T?t$rJV$9(B    
    u_int	nrow()			const	{return dim();}
  //! 2$B<!85G[Ns$NNs?t$rJV$9(B    
    u_int	ncol()			const	{return _cols;}
  //! 2$B<!85G[Ns$NFbIt5-21NN0h$X$N%]%$%s%?$rJV$9(B
		operator ET*()		const	{return (ET*)_ary;}

  //! $BG[Ns$NA4MWAG$K?tCM$rBeF~$9$k(B
    Array2&	operator  =(double c)		{Array<T>::operator  =(c);
						 return *this;}
  //! $BG[Ns$NA4MWAG$rDj?tG\$9$k(B
    Array2&	operator *=(double c)		{Array<T>::operator *=(c);
						 return *this;}
  //! $BG[Ns$NA4MWAG$rDj?t$G3d$k(B
    Array2&	operator /=(double c)		{Array<T>::operator /=(c);
						 return *this;}
  //! $BG[Ns$KJL$NG[Ns$rMWAGKh$KB-$9(B
    Array2&	operator +=(const Array2& a)	{Array<T>::operator +=(a);
						 return *this;}
  //! $BG[Ns$+$iJL$NG[Ns$rMWAGKh$K0z$/(B
    Array2&	operator -=(const Array2& a)	{Array<T>::operator -=(a);
						 return *this;}
  //! $B$9$Y$F$NMWAG$NId9f$rH?E>$7$?G[Ns$rJV$9(B
    Array2	operator  -()		const	{Array2 r(*this);
						 r *= -1; return r;}
    std::istream&	restore(std::istream&)		;
    std::ostream&	save(std::ostream&)	const	;
    void		resize(u_int, u_int)		;
    void		resize(ET*, u_int, u_int)	;
    std::istream&	get(std::istream&, int, int, int)	;
    
  private:
    virtual void	set_rows()		;
    
    u_int		_cols;			// # of columns (width)
    Array<ET>		_ary;

  /*    friend std::istream&	operator >>(std::istream& in, Array2<T>& a)
	{return a.get(in >> std::ws, 0, 0, 0);}*/
};

template <class T> inline std::istream&
operator >>(std::istream& in, Array2<T>& a)
{
    return a.get(in >> std::ws, 0, 0, 0);
}

/************************************************************************
*  numerical operators							*
************************************************************************/
template <class T> inline Array<T>
operator +(const Array<T>& a, const Array<T>& b)
    {Array<T> r(a); r += b; return r;}

template <class T> inline Array<T>
operator -(const Array<T>& a, const Array<T>& b)
    {Array<T> r(a); r -= b; return r;}

template <class T> inline Array<T>
operator *(double c, const Array<T>& a)
    {Array<T> r(a); r *= c; return r;}

template <class T> inline Array<T>
operator *(const Array<T>& a, double c)
    {Array<T> r(a); r *= c; return r;}

template <class T> inline Array<T>
operator /(const Array<T>& a, double c)
    {Array<T> r(a); r /= c; return r;}

template <class T> inline Array2<T>
operator +(const Array2<T>& a, const Array2<T>& b)
    {Array2<T> r(a); r += b; return r;}

template <class T> inline Array2<T>
operator -(const Array2<T>& a, const Array2<T>& b)
    {Array2<T> r(a); r -= b; return r;}

template <class T> inline Array2<T>
operator *(double c, const Array2<T>& a)
    {Array2<T> r(a); r *= c; return r;}

template <class T> inline Array2<T>
operator *(const Array2<T>& a, double c)
    {Array2<T> r(a); r *= c; return r;}

template <class T> inline Array2<T>
operator /(const Array2<T>& a, double c)
    {Array2<T> r(a); r /= c; return r;}
 
}

#endif	/* !__TUArrayPP_h */
