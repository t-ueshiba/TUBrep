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
 *  $Id: Array++.inst.cc,v 1.3 2002-12-18 05:46:12 ueshiba Exp $
 */
#if defined __GNUG__ || defined __INTEL_COMPILER

#include "TU/Array++.cc"

namespace TU
{
template class Array<char>;
template class Array<u_char>;
template class Array<short>;
template class Array<u_short>;
template class Array<int>;
template class Array<u_int>;
template class Array<long>;
template class Array<u_long>;
template class Array<float>;
template class Array<double>;

template class Array2<Array<char> >;
template class Array2<Array<u_char> >;
template class Array2<Array<short> >;
template class Array2<Array<u_short> >;
template class Array2<Array<int> >;
template class Array2<Array<u_int> >;
template class Array2<Array<long> >;
template class Array2<Array<u_long> >;
template class Array2<Array<float> >;
template class Array2<Array<double> >;

template std::ostream&	operator <<(std::ostream&, const Array<char>&);
template std::ostream&	operator <<(std::ostream&, const Array<u_char>&);
template std::ostream&	operator <<(std::ostream&, const Array<short>&);
template std::ostream&	operator <<(std::ostream&, const Array<u_short>&);
template std::ostream&	operator <<(std::ostream&, const Array<int>&);
template std::ostream&	operator <<(std::ostream&, const Array<u_int>&);
template std::ostream&	operator <<(std::ostream&, const Array<long>&);
template std::ostream&	operator <<(std::ostream&, const Array<u_long>&);
template std::ostream&	operator <<(std::ostream&, const Array<float>&);
template std::ostream&	operator <<(std::ostream&, const Array<double>&);
}

#endif
