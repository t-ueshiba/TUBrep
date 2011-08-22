/*
 *  ����14-19�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
 *  
 *  �n��ҁF�A�ŏr�v
 *
 *  �{�v���O�����́i�Ɓj�Y�ƋZ�p�����������̐E���ł���A�ŏr�v���n�삵�C
 *  �i�Ɓj�Y�ƋZ�p���������������쌠�����L����閧���ł��D���쌠���L
 *  �҂ɂ�鋖�Ȃ��ɖ{�v���O�������g�p�C�����C���ρC��O�҂֊J������
 *  ���̍s�ׂ��֎~���܂��D
 *  
 *  ���̃v���O�����ɂ���Đ����邢���Ȃ鑹�Q�ɑ΂��Ă��C���쌠���L�҂�
 *  ��ёn��҂͐ӔC�𕉂��܂���B
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
 *  $Id: Manip.h,v 1.10 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		Manip.h
  \brief	�e��}�j�s�����[�^�̒�`�Ǝ���
*/
#ifndef __TUManip_h
#define __TUManip_h

#include <iostream>
#include "TU/types.h"

namespace TU
{
/************************************************************************
*  Manipulators								*
************************************************************************/
__PORT std::istream&	skipl(std::istream& in)			;

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
