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
 *  $Id: Geometry++.inst.cc,v 1.2 2002-07-25 02:38:04 ueshiba Exp $
 */
#ifdef __GNUG__

#include "TU/Geometry++.h"
#include "TU/Geometry++.cc"

namespace TU
{
/*
 *  Cannot instantiate Coordinate<T, D> because of infinite recursive
 *  instantiation of CoordinateP<T, D> (= Coordinate<T, D+1u>)
 */
template class CoordBase<short,   2u>;
template class CoordBase<u_short, 2u>;
template class CoordBase<int,     2u>;
template class CoordBase<u_int,   2u>;
template class CoordBase<float,   2u>;
template class CoordBase<double,  2u>;

template class CoordBase<short,   3u>;
template class CoordBase<u_short, 3u>;
template class CoordBase<int,     3u>;
template class CoordBase<u_int,   3u>;
template class CoordBase<float,   3u>;
template class CoordBase<double,  3u>;

template class CoordBase<float,   4u>;
template class CoordBase<double,  4u>;

template std::istream&	operator >>(std::istream&, CoordBase<short,   2u>&);
template std::istream&	operator >>(std::istream&, CoordBase<u_short, 2u>&);
template std::istream&	operator >>(std::istream&, CoordBase<int,     2u>&);
template std::istream&	operator >>(std::istream&, CoordBase<u_int,   2u>&);
template std::istream&	operator >>(std::istream&, CoordBase<float,   2u>&);
template std::istream&	operator >>(std::istream&, CoordBase<double,  2u>&);
template std::istream&	operator >>(std::istream&, CoordBase<short,   3u>&);
template std::istream&	operator >>(std::istream&, CoordBase<u_short, 3u>&);
template std::istream&	operator >>(std::istream&, CoordBase<int,     3u>&);
template std::istream&	operator >>(std::istream&, CoordBase<u_int,   3u>&);
template std::istream&	operator >>(std::istream&, CoordBase<float,   3u>&);
template std::istream&	operator >>(std::istream&, CoordBase<double,  3u>&);
template std::istream&	operator >>(std::istream&, CoordBase<short,   4u>&);
template std::istream&	operator >>(std::istream&, CoordBase<u_short, 4u>&);
template std::istream&	operator >>(std::istream&, CoordBase<int,     4u>&);
template std::istream&	operator >>(std::istream&, CoordBase<u_int,   4u>&);
template std::istream&	operator >>(std::istream&, CoordBase<float,   4u>&);
template std::istream&	operator >>(std::istream&, CoordBase<double,  4u>&);

template std::ostream&
operator <<(std::ostream&, const CoordBase<short, 2u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<u_short, 2u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<int, 2u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<u_int, 2u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<float, 2u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<double, 2u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<short, 3u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<u_short, 3u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<int, 3u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<u_int, 3u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<float, 3u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<double, 3u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<short, 4u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<u_short, 4u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<int, 4u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<u_int, 4u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<float, 4u>&);
template std::ostream&
operator <<(std::ostream&, const CoordBase<double, 4u>&);
    
template float
operator *(const CoordBase<float, 2u>&, const CoordBase<float, 2u>&);
template float
operator *(const CoordBase<float, 3u>&, const CoordBase<float, 3u>&);
template float
operator *(const CoordBase<float, 4u>&, const CoordBase<float, 4u>&);
template double
operator *(const CoordBase<double, 2u>&, const CoordBase<double, 2u>&);
template double
operator *(const CoordBase<double, 3u>&, const CoordBase<double, 3u>&);
template double
operator *(const CoordBase<double, 4u>&, const CoordBase<double, 4u>&);

template class Coordinate<short,   2u>;
template class Coordinate<u_short, 2u>;
template class Coordinate<int,     2u>;
template class Coordinate<u_int,   2u>;
template class Coordinate<float,   2u>;
template class Coordinate<double,  2u>;

template class Coordinate<float,   3u>;
template class Coordinate<double,  3u>;

template Coordinate<float, 3u>
operator ^(const Coordinate<float, 3u>&, const Coordinate<float, 3u>&);
template Coordinate<double, 3u>
operator ^(const Coordinate<double, 3u>&, const Coordinate<double, 3u>&);

template class CoordinateP<short,   2u>;
template class CoordinateP<u_short, 2u>;
template class CoordinateP<int,     2u>;
template class CoordinateP<u_int,   2u>;
template class CoordinateP<float,   2u>;
template class CoordinateP<double,  2u>;

template class CoordinateP<float,   3u>;
template class CoordinateP<double,  3u>;

template CoordinateP<float, 2u>
operator ^(const CoordinateP<float, 2u>&, const CoordinateP<float, 2u>&);
template CoordinateP<double, 2u>
operator ^(const CoordinateP<double, 2u>&, const CoordinateP<double, 2u>&);

template class Point2<short>;
template class Point2<int>;
    
}

#endif	/* __GNUG__	*/
