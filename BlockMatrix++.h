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
 *  $Id: BlockMatrix++.h,v 1.7 2007-11-29 07:06:35 ueshiba Exp $
 */
#ifndef __TUBlockMatrixPP_h
#define __TUBlockMatrixPP_h

#include "TU/Vector++.h"

namespace TU
{
/************************************************************************
*  class BlockMatrix<T>							*
************************************************************************/
template <class T>
class BlockMatrix : public Array<Matrix<T> >
{
  public:
    explicit BlockMatrix(u_int d=0)	:Array<Matrix<T> >(d)	{}
    BlockMatrix(const Array<u_int>& nrows,
		const Array<u_int>& ncols)			;

    using		Array<Matrix<T> >::dim;
    u_int		nrow()				const	;
    u_int		ncol()				const	;
    BlockMatrix		trns()				const	;
    BlockMatrix&	operator  =(T c)			;
    BlockMatrix&	operator *=(double c)
			{Array<Matrix<T> >::operator *=(c); return *this;}
    BlockMatrix&	operator /=(double c)
			{Array<Matrix<T> >::operator /=(c); return *this;}
    BlockMatrix&	operator +=(const BlockMatrix& b)
			{Array<Matrix<T> >::operator +=(b); return *this;}
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
