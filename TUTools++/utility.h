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
 *  $Id: utility.h,v 1.14 2007-11-26 07:55:48 ueshiba Exp $
 */
#ifndef __TUutility_h
#define __TUutility_h

#include <algorithm>
#include <iterator>

namespace std
{
/************************************************************************
*  generic algorithms							*
************************************************************************/
//! 3$B$D$N0z?t$N$&$A:G>.$N$b$N$rJV$9!%(B
template <class T> inline const T&
min(const T& a, const T& b, const T& c)
{
    return min(min(a, b), c);
}

//! 3$B$D$N0z?t$N$&$A:GBg$N$b$N$rJV$9!%(B
template <class T> inline const T&
max(const T& a, const T& b, const T& c)
{
    return max(max(a, b), c);
}
    
//! 4$B$D$N0z?t$N$&$A:G>.$N$b$N$rJV$9!%(B
template <class T> inline const T&
min(const T& a, const T& b, const T& c, const T& d)
{
    return min(min(a, b, c), d);
}

//! 4$B$D$N0z?t$N$&$A:GBg$N$b$N$rJV$9!%(B
template <class T> inline const T&
max(const T& a, const T& b, const T& c, const T& d)
{
    return max(max(a, b, c), d);
}

}

/*!
  \namespace	TU
  \brief	$BK\%i%$%V%i%j$GDj5A$5$l$?%/%i%9$*$h$S4X?t$rG<$a$kL>A06u4V(B
*/
namespace TU
{
/************************************************************************
*  generic algorithms							*
************************************************************************/
//! $B>r7o$rK~$?$9MWAG$,A0H>$K!$$=$&$G$J$$$b$N$,8eH>$K$J$k$h$&$KJB$YBX$($k!%(B
/*!
  \param first	$B%G!<%?Ns$N@hF,$r<($9H?I|;R(B
  \param last	$B%G!<%?Ns$NKvHx$r<($9H?I|;R(B
  \param pred	$B>r7o$r;XDj$9$kC19`1i;;;R(B
  \return	$B>r7o$rK~$?$5$J$$MWAG$N@hF,$r<($9H?I|;R(B
*/
template <class Iter, class Pred> Iter
pull_if(Iter first, Iter last, Pred pred)
{
    for (Iter iter = first; iter != last; ++iter)
	if (pred(*iter))
	    std::iter_swap(first++, iter);
    return first;
}

//! 2$B$D$N0z?t$N:9$N@dBPCM$rJV$9!%(B
template <class T> inline T
diff(const T& a, const T& b)
{
    return (a > b ? a - b : b - a);
}

/************************************************************************
*  class mem_iterator							*
************************************************************************/
//! $B%3%s%F%J$NMWAG$NFCDj$N%a%s%P$K%"%/%;%9$9$kH?I|;R(B
/*!
  \param Iterator	$BK\H?I|;R$N%Y!<%9$H$J$kH?I|;R(B
*/
template <class Iterator, class T>
class mbr_iterator
  : public
      std::iterator<typename std::iterator_traits<Iterator>::iterator_category,
		    T,
		    typename std::iterator_traits<Iterator>::difference_type,
		    T*, T&>
{
  public:
    typedef typename std::iterator_traits<Iterator>::iterator_category
							iterator_categoty;
    typedef T						value_type;
    typedef typename std::iterator_traits<Iterator>::difference_type
							difference_type;
    typedef value_type*					pointer;
    typedef value_type&					reference;
    typedef value_type std::iterator_traits<Iterator>::value_type::*
							mbr_pointer;
    
    mbr_iterator(Iterator i, mbr_pointer m)	:_i(i), _m(m)	{}

    Iterator		base() const
			{
			    return _i;
			}
    reference		operator * () const
			{
			    return (*_i).*_m;
			}
    pointer		operator ->() const
			{
			    return &(operator *());
			}
    mbr_iterator&	operator ++()
			{
			    ++_i;
			    return *this;
			}
    mbr_iterator	operator ++(int)
			{
			    mbr_iterator	tmp = *this;
			    ++_i;
			    return tmp;
			}
    mbr_iterator&	operator --()
			{
			    --_i;
			    return *this;
			}
    mbr_iterator	operator --(int)
			{
			    mbr_iterator	tmp = *this;
			    --_i;
			    return tmp;
			}
    mbr_iterator&	operator +=(difference_type n)
			{
			    _i += n;
			    return *this;
			}
    mbr_iterator&	operator -=(difference_type n)
			{
			    _i -= n;
			    return *this;
			}
    mbr_iterator	operator +(difference_type n) const
			{
			    mbr_iterator	tmp = *this;
			    return tmp += n;
			}
    mbr_iterator	operator -(difference_type n) const
			{
			    mbr_iterator	tmp = *this;
			    return tmp -= n;
			}
    reference		operator [](difference_type n) const
			{
			    return *(*this + n);
			}
	
  private:
    Iterator		_i;
    const mbr_pointer	_m;
};

template <class Iterator, class T> inline bool 
operator ==(const mbr_iterator<Iterator, T>& x,
	    const mbr_iterator<Iterator, T>& y) 
{
    return x.base() == y.base();
}

template<class Iterator, class T> inline bool 
operator !=(const mbr_iterator<Iterator, T>& x, 
	    const mbr_iterator<Iterator, T>& y) 
{
    return !(x == y);
}

template<class Iterator, class T> inline bool 
operator <(const mbr_iterator<Iterator, T>& x, 
	   const mbr_iterator<Iterator, T>& y) 
{
    return x.base() < y.base();
}

template<class Iterator, class T> inline bool 
operator >(const mbr_iterator<Iterator, T>& x, 
	   const mbr_iterator<Iterator, T>& y) 
{
    return y < x;
}

template<class Iterator, class T> inline bool 
operator <=(const mbr_iterator<Iterator, T>& x, 
	    const mbr_iterator<Iterator, T>& y) 
{
    return !(y < x);
}

template<class Iterator, class T> inline bool 
operator >=(const mbr_iterator<Iterator, T>& x, 
	    const mbr_iterator<Iterator, T>& y) 
{
    return !(x < y);
}

template<class Iterator, class T>
inline typename mbr_iterator<Iterator, T>::difference_type
operator -(const mbr_iterator<Iterator, T>& x, 
	   const mbr_iterator<Iterator, T>& y) 
{
    return x.base() - y.base();
}

template<class Iterator, class T> inline mbr_iterator<Iterator, T> 
operator +(typename mbr_iterator<Iterator, T>::difference_type n,
	   const mbr_iterator<Iterator, T>& x) 
{
    return x + n;
}

//! T$B7?$N%a%s%P$r;}$D%*%V%8%'%/%H$rMWAG$H$9$k%3%s%F%J$K$D$$$F$=$N%a%s%P$K%"%/%;%9(B(R/W)$B$9$kH?I|;R$r:n$k!%(B
template <class Iterator, class T> inline mbr_iterator<Iterator, T>
make_mbr_iterator(Iterator i, T std::iterator_traits<Iterator>::value_type::* m)
{
    return mbr_iterator<Iterator, T>(i, m);
}
    
//! T$B7?$N%a%s%P$r;}$D%*%V%8%'%/%H$rMWAG$H$9$k%3%s%F%J$K$D$$$F$=$N%a%s%P$K%"%/%;%9(B(R)$B$9$kH?I|;R$r:n$k!%(B
template <class Iterator, class T> inline mbr_iterator<Iterator, const T>
make_const_mbr_iterator(Iterator i,
			const T std::iterator_traits<Iterator>::value_type::* m)
{
    return mbr_iterator<Iterator, const T>(i, m);
}

//! std::pair$B$rMWAG$H$9$k%3%s%F%J$K$D$$$F(Bpair$B$NBh(B1$BMWAG$K%"%/%;%9(B(R/W)$B$9$kH?I|;R$r:n$k!%(B
/*!
  \param i	$B%Y!<%9$H$J$kH?I|;R(B
*/
template <class Iterator>
inline mbr_iterator<Iterator, typename std::iterator_traits<Iterator>
					  ::value_type::first_type>
make_first_iterator(Iterator i)
{
    return make_mbr_iterator(i, &std::iterator_traits<Iterator>
				    ::value_type::first);
}
    
//! std::pair$B$rMWAG$H$9$k%3%s%F%J$K$D$$$F(Bpair$B$NBh(B1$BMWAG$K%"%/%;%9(B(R)$B$9$kH?I|;R$r:n$k!%(B
/*!
  \param i	$B%Y!<%9$H$J$kH?I|;R(B
*/
template <class Iterator>
inline mbr_iterator<Iterator, const typename std::iterator_traits<Iterator>
						::value_type::first_type>
make_const_first_iterator(Iterator i)
{
    return make_const_mbr_iterator(i, &std::iterator_traits<Iterator>
					  ::value_type::first);
}
    
//! std::pair$B$rMWAG$H$9$k%3%s%F%J$K$D$$$F(Bpair$B$NBh(B2$BMWAG$K%"%/%;%9(B(R/W)$B$9$kH?I|;R$r:n$k!%(B
/*!
  \param i	$B%Y!<%9$H$J$kH?I|;R(B
*/
template <class Iterator>
inline mbr_iterator<Iterator, typename std::iterator_traits<Iterator>
					  ::value_type::second_type>
make_second_iterator(Iterator i)
{
    return make_mbr_iterator(i, &std::iterator_traits<Iterator>
				    ::value_type::second);
}

//! std::pair$B$rMWAG$H$9$k%3%s%F%J$K$D$$$F(Bpair$B$NBh(B2$BMWAG$K%"%/%;%9(B(R)$B$9$kH?I|;R$r:n$k!%(B
/*!
  \param i	$B%Y!<%9$H$J$kH?I|;R(B
*/
template <class Iterator>
inline mbr_iterator<Iterator, const typename std::iterator_traits<Iterator>
						::value_type::second_type>
make_const_second_iterator(Iterator i)
{
    return make_const_mbr_iterator(i, &std::iterator_traits<Iterator>
					  ::value_type::second);
}
    
}

#endif	/* __TUutility_h */
