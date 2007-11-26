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
 *  $Id: Microscope.cc,v 1.7 2007-11-26 07:28:09 ueshiba Exp $
 */
#include "TU/Serial++.h"
#include <stdexcept>
#include <iomanip>

namespace TU
{
/************************************************************************
*  Static functions							*
************************************************************************/ 
static inline Microscope&
operator <<(Microscope& ms, char c)
{
    ms.Serial::operator <<(c);
    return ms;
}

static inline Microscope&
operator <<(Microscope& ms, int i)
{
    ms.Serial::operator <<(i);
    return ms;
}

static inline Microscope&
operator <<(Microscope& ms, const char* s)
{
    ms.Serial::operator <<(s);
    return ms;
}
    
static Microscope&
endc(Microscope& ms)
{
    ms << std::endl;
    if (ms.get() == 'N')
	throw std::runtime_error("TU::endc: error happened!");
    ms >> skipl;
    return ms;
}

static Microscope&
operator <<(Microscope& ms, Microscope::Axis axis)
{
    int		busy;
    do
    {
	ms << (char)axis << "SG" << endc;
	ms >> busy >> skipl;
    } while (busy);
    return ms << (char)axis;
}

/************************************************************************
*  class Microscope							*
************************************************************************/ 
/*
 *  Public member functions
 */
Microscope::Microscope(const char* ttyname)
    :Serial(ttyname), _axis(X)
{
    o_nl2crnl().i_cr2nl();
}

Microscope&
Microscope::operator +=(int d)
{
    char	dir = '+';
    if (d < 0)
    {
	d = -d;
	dir = '-';
    }
    return *this << _axis << "AC,A114," << d << endc
		 << _axis << "II," << dir << endc;
}

Microscope&
Microscope::operator -=(int d)
{
    char	dir = '-';
    if (d < 0)
    {
	d = -d;
	dir = '+';
    }
    return *this  << _axis << "AC,A114," << d << endc
		  << _axis << "II," << dir << endc;
}

Microscope&
Microscope::operator ++()
{
    return *this << _axis << "JG,+" << endc;
}

Microscope&
Microscope::operator --()
{
    return *this << _axis << "JG,-" << endc;
}

/*
 *  Private member functions
 */
Microscope&
Microscope::set_axis(Microscope::Axis axis)
{
    _axis = axis;
    return *this;
}

/************************************************************************
*  friends of class Microscope						*
************************************************************************/
Microscope&
operator <<(Microscope& ms, const Array<int>& position)
{
    switch (position.dim())
    {
      default:
	ms << Microscope::Z << "AC,A115," << position[2] << endc
	   << Microscope::Z << "AI" << endc;
      case 2:
	ms << Microscope::Y << "AC,A115," << position[1] << endc
	   << Microscope::Y << "AI" << endc;
      case 1:
	ms << Microscope::X << "AC,A115," << position[0] << endc
	   << Microscope::X << "AI" << endc;
      case 0:
	break;
    }
    return ms;
}

Microscope&
operator >>(Microscope& ms, Array<int>& position)
{
    position.resize(3);
    ms << Microscope::X << "AG" << endc >> position[0] >> skipl;
    ms << Microscope::Y << "AG" << endc >> position[1] >> skipl;
    ms << Microscope::Z << "AG" << endc >> position[2] >> skipl;
    return ms;
}
    
OManip1<Microscope, Microscope::Axis>
axis(Microscope::Axis axis)
{
    return OManip1<Microscope, Microscope::Axis>(&Microscope::set_axis, axis);
}

Microscope&
calib(Microscope& ms)
{
    const int	X0 = 30000, Y0 = 15000, Z0 = 0;

    ms << Microscope::X << "RO" << endc
       << Microscope::Y << "RO" << endc
       << Microscope::Z << "RO" << endc;
    
    ms << axis(Microscope::X) += X0;
    ms << axis(Microscope::Y) += Y0;
    ms << axis(Microscope::Z) += Z0;
    ms << axis(Microscope::X);
    
    ms << Microscope::X << "RP" << endc
       << Microscope::X << "AC,A100,0" << endc
       << Microscope::X << "SR" << endc
       << Microscope::X << "AC,A100,1" << endc;
    ms << Microscope::Y << "RP" << endc
       << Microscope::Y << "AC,A100,0" << endc
       << Microscope::Y << "SR" << endc
       << Microscope::Y << "AC,A100,1" << endc;
    ms << Microscope::Z << "RP" << endc
       << Microscope::Z << "AC,A100,0" << endc
       << Microscope::Z << "SR" << endc
       << Microscope::Z << "AC,A100,1" << endc;
    return ms;
}

Microscope&
ready(Microscope& ms)
{
    return ms << Microscope::X << "RT" << endc
	      << Microscope::Y << "RT" << endc
	      << Microscope::Z << "RT" << endc;
}
 
}
