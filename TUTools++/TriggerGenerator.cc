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
 *  $Id: TriggerGenerator.cc,v 1.9 2007-11-26 07:55:48 ueshiba Exp $
 */
#include <iomanip>
#include <cstdlib>
#include "TU/Serial++.h"

namespace TU
{
/************************************************************************
*  class TriggerGenerator						*
************************************************************************/
TriggerGenerator::TriggerGenerator(const char* ttyname)
    :Serial(ttyname)
{
    using namespace	std;

    *this >> through << through
	  << baud(9600) << csize(8) << noparity << stop1;
    setf(ios::uppercase);
}

TriggerGenerator&
TriggerGenerator::showId(std::ostream& o)
{
    using namespace	std;
    
    *this << 'V' << endl;
    for (char c; get(c); )
    {
	o << c;
	if (c == '\n')
	    break;
    }
    return *this;
}

TriggerGenerator&
TriggerGenerator::selectChannel(u_int channel)
{
    using namespace	std;
    
    setf(ios::hex, ios::basefield);
    *this << 'A';
    width(8);
    fill('0');
    *this << channel << endl;
    *this >> ign;
    return *this;
}

TriggerGenerator&
TriggerGenerator::setInterval(u_int interval)
{
    using namespace	std;
    
    if (10 <= interval && interval <= 255)
    {
	setf(ios::dec, ios::basefield);
	*this << 'F' << interval << endl;
	*this >> ign;
    }
    return *this;
}

TriggerGenerator&
TriggerGenerator::oneShot()
{
    *this << 'T' << std::endl;
    *this >> ign;
    return *this;
}

TriggerGenerator&
TriggerGenerator::continuousShot()
{
    *this << 'R' << std::endl;
    *this >> ign;
    return *this;
}

TriggerGenerator&
TriggerGenerator::stopContinuousShot()
{
    *this << 'S' << std::endl;
    *this >> ign;
    return *this;
}

int
TriggerGenerator::getConfiguration(u_int& channel, u_int& interval)
{
    *this << 'I' << std::endl;
    char	token[64];
    for (char c, *p = token; get(c); )
    {
	if (c == '\n')
	{
	    *p = '\0';
	    break;
	}
	else if (c == ',')
	{
	    *p = '\0';
	    p = token;
	    if (token[0] == 'A')
		channel = strtoul(token + 1, NULL, 16);
	    else
		interval = strtoul(token + 1, NULL, 10);
	}
	else
	    *p++ = c;
    }
    
    return !strcmp(token, "RUN");
}

}

