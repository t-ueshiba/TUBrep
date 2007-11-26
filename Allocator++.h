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
 *  $Id: Allocator++.h,v 1.5 2007-11-26 07:28:09 ueshiba Exp $
 */
#ifndef __TUAllocatorPP_h
#define __TUAllocatorPP_h

#include "TU/List++.h"
#include "TU/Array++.h"

namespace TU
{
/************************************************************************
*  class Allocator<T>							*
************************************************************************/
template <class T>
class Allocator
{
  public:
    class Page : public List<Page>::Node
    {
      public:
	Page(u_int nelements)
	    :_nelements(nelements),
	     _p(new char[sizeof(T)*_nelements])	{}
	~Page()					{delete [] _p;}

	u_int		nelements()	const	{return _nelements;}
	void*		nth(int i)	const	{return _p + sizeof(T)*i;}

      private:
	const u_int	_nelements;	// # of elements in this page.
	char* const	_p;		// memory pool.
    };

    class PageList : public List<Page>
    {
      public:
      	~PageList()					;

	using		List<Page>::begin;
	using		List<Page>::end;
	using		List<Page>::empty;
	using		List<Page>::front;
	using		List<Page>::pop_front;
    };

    class Element : public List<Element>::Node
    {
    };

    class FreeList : public List<Element>
    {
      public:
	void		addPage(Page& page)		;

	using		List<Element>::begin;
	using		List<Element>::end;
	using		List<Element>::empty;
	using		List<Element>::front;
	using		List<Element>::pop_front;
    };

    class Enumerator : public List<Enumerator>::Node
    {
      public:
	Enumerator(const Allocator<T>& allocator)	;
	~Enumerator()					;

	void		head()				;
			operator T*()			{return _p;}
	T*		operator ->()			{return _p;}
	Enumerator&	operator ++()			;
	
      private:
	friend class	Allocator;
	
	void		unmark(const void* p)		;
	
	const Allocator<T>&			_allocator;
	typename List<Page>::ConstIterator	_pageIter;
	u_int					_page, _index, _bit;
	T*					_p;
	Array<Array<u_int> >	_mark;	// marked if not in freeList.
    };

  public:
    Allocator(u_int nelmsPerPage=1024)		{setPageSize(nelmsPerPage);}

    void*		alloc()			;
    void		free(void* p)		;
    void		setPageSize(u_int n)	;

  private:
    friend class	Enumerator;

    PageList		_pageList;
    FreeList		_freeList;
    u_int		_nelmsPerPage;
    List<Enumerator>	_enumeratorList;
};
 
}

#endif	// !__TUAllocatorPP_h
