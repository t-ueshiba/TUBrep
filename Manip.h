/*
 *  $BJ?@.(B19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $BF1=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n<T$K$h$k5v2D$J$7$K$3$N%W%m(B
 *  $B%0%i%`$rBh;0<T$X3+<(!$J#@=!$2~JQ!$;HMQ$9$kEy$NCx:n8"$r?/32$9$k9T0Y(B
 *  $B$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *  Copyright 2007
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Author: Toshio UESHIBA
 *
 *  Confidentail and all rights reserved.
 *  This program is confidential. Any changing, copying or giving
 *  information about the source code of any part of this software
 *  and/or documents without permission by the authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damages in the use of this program.
 *  
 *  $Id: Manip.h,v 1.3 2007-11-26 07:28:09 ueshiba Exp $
 */
#ifndef __TUManip_h
#define __TUManip_h

#include <iostream>

namespace TU
{
/************************************************************************
*  Manipulators								*
************************************************************************/
std::istream&	ign(std::istream& in)		;
std::istream&	skipl(std::istream&)		;

/************************************************************************
*  class IOManip							*
************************************************************************/
template <class S>
class IOManip
{
  public:
    IOManip(S& (S::*fi)(), S& (S::*fo)()) :_fi(fi), _fo(fo)	{}

    S&		iapply(S& s)		const	{return (s.*_fi)();}
    S&		oapply(S& s)		const	{return (s.*_fo)();}
    
  private:
    S&		(S::*_fi)();
    S&		(S::*_fo)();
};

template <class SS, class S> inline SS&
operator >>(SS& s, const IOManip<S>& m)
{
    m.iapply(s);
    return s;
}

template <class SS, class S> inline SS&
operator <<(SS& s, const IOManip<S>& m)
{
    m.oapply(s);
    return s;
}

/************************************************************************
*  class IManip1							*
************************************************************************/
template <class S, class T>
class IManip1
{
  public:
    IManip1(S& (S::*f)(T), T arg)	:_f(f), _arg(arg)	{}

    S&		apply(S& s)		const	{return (s.*_f)(_arg);}
    
  private:
    S&		(S::*_f)(T);
    const T	_arg;
};

template <class SS, class S, class T> inline SS&
operator >>(SS& s, const IManip1<S, T>& m)
{
    m.apply(s);
    return s;
}

/************************************************************************
*  class OManip1							*
************************************************************************/
template <class S, class T>
class OManip1
{
  public:
    OManip1(S& (S::*f)(T), T arg)	:_f(f), _arg(arg)	{}

    S&		apply(S& s)		const	{return (s.*_f)(_arg);}
    
  private:
    S&		(S::*_f)(T);
    const T	_arg;
};

template <class SS, class S, class T> inline SS&
operator <<(SS& s, const OManip1<S, T>& m)
{
    m.apply(s);
    return s;
}
    
/************************************************************************
*  class IOManip1							*
************************************************************************/
template <class S, class T>
class IOManip1
{
  public:
    IOManip1(S& (S::*fi)(T), S& (S::*fo)(T), T arg)
      :_fi(fi), _fo(fo), _arg(arg)				{}

    S&		iapply(S& s)		const	{return (s.*_fi)(_arg);}
    S&		oapply(S& s)		const	{return (s.*_fo)(_arg);}
    
  private:
    S&		(S::*_fi)(T);
    S&		(S::*_fo)(T);
    const T	_arg;
};

template <class SS, class S, class T> inline SS&
operator >>(SS& s, const IOManip1<S, T>& m)
{
    m.iapply(s);
    return s;
}

template <class SS, class S, class T> inline SS&
operator <<(SS& s, const IOManip1<S, T>& m)
{
    m.oapply(s);
    return s;
}

/************************************************************************
*  class IManip2							*
************************************************************************/
template <class S, class T, class U>
class IManip2
{
  public:
    IManip2(S& (S::*f)(T, U), T arg0, U arg1)
      :_f(f), _arg0(arg0), _arg1(arg1)				{}

    S&		apply(S& s)		const	{return (s.*_f)(_arg0, _arg1);}
    
  private:
    S&		(S::*_f)(T, U);
    const T	_arg0;
    const U	_arg1;
};

template <class SS, class S, class T, class U> inline SS&
operator >>(SS& s, const IManip2<S, T, U>& m)
{
    m.apply(s);
    return s;
}

/************************************************************************
*  class OManip2							*
************************************************************************/
template <class S, class T, class U>
class OManip2
{
  public:
    OManip2(S& (S::*f)(T, U), T arg0, U arg1)
      :_f(f), _arg0(arg0), _arg1(arg1)				{}

    S&		apply(S& s)		const	{return (s.*_f)(_arg0, _arg1);}
    
  private:
    S&		(S::*_f)(T, U);
    const T	_arg0;
    const U	_arg1;
};

template <class SS, class S, class T, class U> inline SS&
operator <<(SS& s, const OManip2<S, T, U>& m)
{
    m.apply(s);
    return s;
}
 
}

#endif	/* !__TUManip_h		*/
