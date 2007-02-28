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
 *  $Id: Vector++.inst.cc,v 1.5 2007-02-28 00:16:06 ueshiba Exp $
 */
#if defined(__GNUG__) || defined(__INTEL_COMPILER)

#include "TU/Vector++.cc"

namespace TU
{
template class Vector<short,  FixedSizedBuf<short,  2> >;
template class Vector<int,    FixedSizedBuf<int,    2> >;
template class Vector<float,  FixedSizedBuf<float,  2> >;
template class Vector<double, FixedSizedBuf<double, 2> >;

template class Vector<short,  FixedSizedBuf<short,  3> >;
template class Vector<int,    FixedSizedBuf<int,    3> >;
template class Vector<float,  FixedSizedBuf<float,  3> >;
template class Vector<double, FixedSizedBuf<double, 3> >;

template class Vector<short,  FixedSizedBuf<short,  4> >;
template class Vector<int,    FixedSizedBuf<int,    4> >;
template class Vector<float,  FixedSizedBuf<float,  4> >;
template class Vector<double, FixedSizedBuf<double, 4> >;

template class Vector<float>;
template class Vector<double>;

template class Matrix<float>;
template class Matrix<double>;

template class LUDecomposition<float>;
template class LUDecomposition<double>;

template class QRDecomposition<float>;
template class QRDecomposition<double>;

template class TriDiagonal<float>;
template class TriDiagonal<double>;

template class BiDiagonal<float>;
template class BiDiagonal<double>;
}

#endif
