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
 *  $Id: Allocator++.cc,v 1.7 2007-11-29 07:06:35 ueshiba Exp $
 */
#include "TU/Allocator++.h"


#define UintNBits	(8*sizeof(u_int))

namespace TU
{
/************************************************************************
*  class Allocator<T>							*
************************************************************************/
template <class T> void*
Allocator<T>::alloc()
{
    if (_freeList.empty())
    {
	_pageList.push_front(*new Page(_nelmsPerPage));
	_freeList.addPage(_pageList.front());
    }
    return &_freeList.pop_front();
}

template <class T> void
Allocator<T>::free(void* p)
{
    if (p != 0)
    {
	_freeList.push_front(*(Element*)p);
	for (typename List<Enumerator>::Iterator iter = _enumeratorList.begin();
	     iter != _enumeratorList.end(); ++iter)
	    iter->unmark(p);
    }
}

template <class T> void
Allocator<T>::setPageSize(u_int n)
{
    if (n == 0)
	_nelmsPerPage = UintNBits;
    else
	_nelmsPerPage = UintNBits * ((n - 1) / UintNBits + 1);
}

/************************************************************************
*  class Allocator<T>::PageList						*
************************************************************************/
template <class T>
Allocator<T>::PageList::~PageList()
{
    while (!empty())
	delete &pop_front();
}

/************************************************************************
*  class Allocator<T>::FreeList						*
************************************************************************/
template <class T> void
Allocator<T>::FreeList::addPage(Page& page)
{
    for (int i = 0; i < page.nelements(); ++i)
	push_front(*(Element*)page.nth(i));
}

/************************************************************************
*  class Allocator<T>::Enumerator					*
************************************************************************/
template <class T>
Allocator<T>::Enumerator::Enumerator(const Allocator<T>& allocator)
    :_allocator(allocator), _pageIter(_allocator._pageList.begin()),
     _page(0), _index(0), _bit(0), _mark(0)
{
  // Register myself to the allocator.
    ((Allocator<T>&)_allocator)._enumeratorList.push_front(*this);
    
  // Allocate mark bits.
    for (; _pageIter != _allocator._pageList.end(); ++_pageIter)
	++_page;
    _mark.resize(_page);			// # of pages
    _page = 0;
    for (_pageIter  = _allocator._pageList.begin();
	 _pageIter != _allocator._pageList.end(); ++_pageIter)
	_mark[_page++].resize((_pageIter->nelements() - 1) / UintNBits + 1);

  // Set all bits.
    for (_page = 0; _page < _mark.dim(); ++_page)
	for (_index = 0; _index < _mark[_page].dim(); ++_index)
	    _mark[_page][_index] = ~0;

  // Unmark bits corresponding to the elements in _freeList.
    for (typename FreeList::ConstIterator iter = _allocator._freeList.begin();
	 iter != _allocator._freeList.end(); ++iter)
	unmark(iter.operator ->());

    head();
}

template <class T>
Allocator<T>::Enumerator::~Enumerator()
{
    ((Allocator<T>&)_allocator)._enumeratorList.remove(*this);
}

template <class T> void
Allocator<T>::Enumerator::head()
{
    _pageIter = _allocator._pageList.begin();
    _page = 0;
    _index = 0;
    _bit = 0x01;
    ++(*this);
}

template <class T> typename Allocator<T>::Enumerator&
Allocator<T>::Enumerator::operator ++()
{
    do	// for all pages...
    {
	do	// for all indices...
	{
	    if (_mark[_page][_index] != 0)
	    {
		do	// for all bits...
		{
		    if (_mark[_page][_index] & _bit)
		    {
			u_int	r = 0;
			for (u_int bit = _bit; (bit >>= 1) != 0; )
			    ++r;
			_p = (T*)_pageIter->nth(UintNBits * _index + r);
			_bit <<= 1;
			return *this;
		    }
		} while ((_bit <<= 1) != 0x0);
	    }

	    _bit = 0x1;
	} while (++_index < _mark[_page].dim());

	++_pageIter;
	_index = 0;
    } while (++_page < _mark.dim());

    _p = 0;
    return *this;
}

template <class T> void
Allocator<T>::Enumerator::unmark(const void* p)
{
    typename PageList::ConstIterator	iter = _allocator._pageList.begin();
    u_int				page = 0;
    for (; p < iter->nth(0) || p >= iter->nth(iter->nelements()); ++iter)
	++page;

    u_int	offset = ((char*)p - (char*)iter->nth(0)) / sizeof(T),
		index  = offset / UintNBits,
		bit    = 0x1 << (offset % UintNBits);
    _mark[page][index] &= ~bit;
}
 
}
