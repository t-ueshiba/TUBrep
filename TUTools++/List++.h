/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$;HMQ!$Bh;0<T$X3+<($9$k(B
 *  $BEy$NCx:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  Confidential and all rights reserved.
 *  This program is confidential. Any using, copying, changing, giving
 *  information about the source program of any part of this software
 *  to others without permission by the creators are prohibited.
 *
 *  No Warranty.
 *  Copyright holders or creators are not responsible for any damages
 *  in the use of this program.
 *  
 *  $Id: List++.h,v 1.6 2007-11-26 07:55:48 ueshiba Exp $
 */
#ifndef __TUListPP_h
#define __TUListPP_h
#include <iterator>

namespace TU
{
/************************************************************************
*  class List<T>							*
************************************************************************/
template <class T>
class List
{
  public:
    typedef T		value_type;
    typedef ptrdiff_t	difference_type;
    typedef T*		pointer;
    typedef T&		reference;
    typedef const T*	const_pointer;
    typedef const T&	const_reference;

    class		Iterator;
    class		ConstIterator;
    
    class Node
    {
      public:
     	Node(const Node&)	:_next(0)	{}
     	Node&		operator =(const Node&)	{return *this;}

      protected:
	Node()		:_next(0)		{}
	~Node()					{_next = 0;}
	
	T*		_next;

      private:
	void		insertNext(T* p)	{p->_next = _next; _next = p;}
	void		eraseNext()		{_next = _next->_next;}

	friend class	List<T>;	// Allow access to private members.
	friend class	Iterator;	// Allow access to _next.
	friend class	ConstIterator;	// ibid.
    };
    
    class Iterator
	: public std::iterator<std::forward_iterator_tag, 
			       value_type, difference_type, pointer, reference>
    {
      public:
	typedef std::forward_iterator_tag		iterator_category;
	typedef typename List<T>::value_type		value_type;
	typedef typename List<T>::difference_type	difference_type;
	typedef typename List<T>::pointer		pointer;
        typedef typename List<T>::reference		reference;

      public:
	Iterator()	:_list(0), _prev(0)	{}
	
	bool		operator ==(const Iterator& i) const
						{return _prev == i._prev;}
	bool		operator !=(const Iterator& i) const
						{return !(*this == i);}
	pointer		operator ->()	const	{return (_prev == 0 ?
							 _list->_front :
							 _prev->_next);}
	reference	operator * ()	const	{return *(operator ->());}
	Iterator&	operator ++()		{_prev = operator ->();
						 return *this;}
	Iterator	operator ++(int)	{Iterator tmp = *this;
						 ++*this;
						 return tmp;}
	
      protected:
	Iterator(List<T>* list, T* prev)
	  :_list(list), _prev(prev)		{}

      private:
	List<T>*	_list;
	T*		_prev;

	friend class	List<T>;	// Allow access to _prev.
    };

    class ConstIterator
	: public std::iterator<std::forward_iterator_tag,
			       value_type, difference_type,
			       const_pointer, const_reference>
    {
      public:
	typedef std::forward_iterator_tag		iterator_category;
	typedef typename List<T>::value_type		value_type;
	typedef typename List<T>::difference_type	difference_type;
	typedef typename List<T>::const_pointer		pointer;
        typedef typename List<T>::const_reference	reference;

      public:
	ConstIterator()	:_current(0)					{}
	ConstIterator(const Iterator& i) :_current(i.operator ->())	{}
	
	bool		operator ==(const ConstIterator& i) const
						{return _current==i._current;}
	bool		operator !=(const ConstIterator& i) const
						{return !(*this == i);}
	pointer		operator ->()	const	{return _current;}
	reference	operator * ()	const	{return *(operator ->());}
	ConstIterator&	operator ++()		{_current = _current->_next; 
						 return *this;}
	ConstIterator	operator ++(int)	{ConstIterator tmp = *this;
						 ++*this;
						 return tmp;}
	
      protected:
	ConstIterator(const T* current)
	  :_current(current)			{}
	
      private:
	const T*	_current;

	friend class	List<T>;	// Allow access to the protected ctor.
    };

  public:
    List()	:_front(0), _back(0)		{}
    ~List()					{}

    Iterator		begin()			{return Iterator(this, 0);}
    Iterator		end()			{return Iterator(this, _back);}
    ConstIterator	begin()		const	{return _front;}
    ConstIterator	end()		const	{return 0;}
    bool		empty()		const	{return _front == 0;}
    reference		front()			{return *begin();}
    const_reference	front()		const	{return *begin();}
    void		push_front(T& x)	{insert(begin(), x);}
    reference		pop_front()		{return erase(begin());}
    Iterator		insert(Iterator i, T& x);
    reference		erase(Iterator i)	;
    void		remove(const T& x)	;
    
  private:
    List(const List&)				;
    List&		operator =(const List&)	;
    
    T			*_front, *_back;

    friend class	Iterator;	// Allow access to _front.
};
 
}

#endif	// !__TUListPP_h
