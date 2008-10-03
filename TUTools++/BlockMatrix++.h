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
 *  $Id: BlockMatrix++.h,v 1.9 2008-10-03 04:23:37 ueshiba Exp $
 */
#ifndef __TUBlockMatrixPP_h
#define __TUBlockMatrixPP_h

#include "TU/Vector++.h"

namespace TU
{
/************************************************************************
*  class BlockMatrix<T>							*
************************************************************************/
//! T$B7?$NMWAG$r;}$D>.9TNs$+$i@.$k%V%m%C%/BP3Q9TNs$rI=$9%/%i%9(B
/*!
  $B6qBNE*$K$O(Bd$B8D$N>.9TNs(B\f$\TUvec{B}{1}, \TUvec{B}{2},\ldots, \TUvec{B}{d}\f$
  ($BF10l%5%$%:$H$O8B$i$J$$(B)$B$+$i@.$k(B
  \f$
  \TUvec{B}{} =
  \TUbeginarray{cccc}
  \TUvec{B}{1} & & & \\ & \TUvec{B}{2} & & \\ & & \ddots & \\
  & & & \TUvec{B}{d}
  \TUendarray
  \f$
  $B$J$k7A$N9TNs!%(B
  \param T	$BMWAG$N7?(B
*/
template <class T>
class BlockMatrix : public Array<Matrix<T> >
{
  public:
  //! $B;XDj$5$l$?8D?t$N>.9TNs$+$i@.$k%V%m%C%/BP3Q9TNs$r@8@.$9$k!%(B
  /*!
    \param d	$B>.9TNs$N8D?t(B
  */
    explicit BlockMatrix(u_int d=0)	:Array<Matrix<T> >(d)	{}
    BlockMatrix(const Array<u_int>& nrows,
		const Array<u_int>& ncols)			;

    using		Array<Matrix<T> >::dim;
    u_int		nrow()				const	;
    u_int		ncol()				const	;
    BlockMatrix		trns()				const	;
    BlockMatrix&	operator  =(const T& c)			;
    BlockMatrix&	operator *=(double c)
			{Array<Matrix<T> >::operator *=(c); return *this;}
    BlockMatrix&	operator /=(double c)
			{Array<Matrix<T> >::operator /=(c); return *this;}

  //! $B$3$N%V%m%C%/BP3Q9TNs$KB>$N%V%m%C%/BP3Q9TNs$rB-$9!%(B
  /*!
    \param b	$BB-$9%V%m%C%/BP3Q9TNs(B
    \return	$B$3$N%V%m%C%/BP3Q9TNs(B
  */
    BlockMatrix&	operator +=(const BlockMatrix& b)
			{Array<Matrix<T> >::operator +=(b); return *this;}

  //! $B$3$N%V%m%C%/BP3Q9TNs$+$iB>$N%V%m%C%/BP3Q9TNs$r0z$/!%(B
  /*!
    \param b	$B0z$/%V%m%C%/BP3Q9TNs(B
    \return	$B$3$N%V%m%C%/BP3Q9TNs(B
  */
    BlockMatrix&	operator -=(const BlockMatrix& b)
			{Array<Matrix<T> >::operator -=(b); return *this;}

			operator Matrix<T>()		const	;
};

/************************************************************************
*  numeric operators							*
************************************************************************/
template <class T> BlockMatrix<T>
operator *(const BlockMatrix<T>& a, const BlockMatrix<T>& b)	;

template <class T> Matrix<T>
operator *(const BlockMatrix<T>& b, const Matrix<T>& m)		;

template <class T> Matrix<T>
operator *(const Matrix<T>& m, const BlockMatrix<T>& b)		;

template <class T> Vector<T>
operator *(const BlockMatrix<T>& b, const Vector<T>& v)		;

template <class T> Vector<T>
operator *(const Vector<T>& v, const BlockMatrix<T>& b)		;
 
}

#endif	/* !__TUBlockMatrixPP_h	*/
