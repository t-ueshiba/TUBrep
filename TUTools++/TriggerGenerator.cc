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
 *  $Id: TriggerGenerator.cc,v 1.14 2009-03-19 05:11:03 ueshiba Exp $
 */
#include <cstdlib>
#include "TU/Serial.h"

namespace TU
{
/************************************************************************
*  static functions							*
************************************************************************/
static void
skipl(FILE* fp)
{
    for (int c; (c = fgetc(fp)) != EOF; )
	if (c == '\n')
	    break;
}
    
/************************************************************************
*  class TriggerGenerator						*
************************************************************************/
TriggerGenerator::TriggerGenerator(const char* ttyname)
    :Serial(ttyname)
{
    i_through()
	.o_through().o_lower2upper()
	.c_baud(9600).c_csize(8).c_noparity().c_stop1();
}

TriggerGenerator&
TriggerGenerator::showId(std::ostream& out)
{
    using namespace	std;
    
    fputs("V\n", fp());
    for (int c; (c = fgetc(fp())) != EOF; )
    {
	out << char(c);
	if (c == '\n')
	    break;
    }
    return *this;
}

TriggerGenerator&
TriggerGenerator::selectChannel(u_int channel)
{
    fprintf(fp(), "A%0.8x\n", channel);
    skipl(fp());
    return *this;
}

TriggerGenerator&
TriggerGenerator::setInterval(u_int interval)
{
    if (10 <= interval && interval <= 255)
    {
	fprintf(fp(), "F%d\n", interval);
	skipl(fp());
    }
    return *this;
}

TriggerGenerator&
TriggerGenerator::oneShot()
{
    fputs("T\n", fp());
    skipl(fp());
    return *this;
}

TriggerGenerator&
TriggerGenerator::continuousShot()
{
    fputs("R\n", fp());
    skipl(fp());
    return *this;
}

TriggerGenerator&
TriggerGenerator::stopContinuousShot()
{
    fputs("S\n", fp());
    skipl(fp());
    return *this;
}

bool
TriggerGenerator::getStatus(u_int& channel, u_int& interval) const
{
    fputs("I\n", fp());

    char	token[64], *p = token;
    for (int c; (c = fgetc(fp())) != EOF; )
    {
	if (c == '\n')
	    break;
	else if (c == ',')
	{
	    *p = '\0';
	    if (token[0] == 'A')
		channel = strtoul(token + 1, NULL, 16);
	    else
		interval = strtoul(token + 1, NULL, 10);
	    p = token;
	}
	else
	    *p++ = c;
    }
    *p = '\0';
    
    return !strcmp(token, "RUN");
}

}

