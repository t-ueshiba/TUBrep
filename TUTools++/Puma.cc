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
 *  $Id: Puma.cc,v 1.11 2008-08-13 06:05:34 ueshiba Exp $
 */
#include "TU/Serial++.h"

namespace TU
{
/************************************************************************
*  Static functions							*
************************************************************************/ 
static inline Puma&
operator <<(Puma& puma, char c)
{
    puma.Serial::operator <<(c);
    return puma;
}

static inline Puma&
operator <<(Puma& puma, const char* s)
{
    puma.Serial::operator <<(s);
    return puma;
}

static inline Puma&
operator <<(Puma& puma, int i)
{
    puma.Serial::operator <<(i);
    return puma;
}

static inline Puma&
operator <<(Puma& puma, float f)
{
    puma.Serial::operator <<(f);
    return puma;
}

static Puma&
endc(Puma& puma)
{
    return puma << '\r' << wait;
}

static int
match_msg(const char* p[], int nmsg)
{
    for (int i = 0; i < nmsg; i++)
	if (*p[i] == '\0')
	    return i;
    return -1;
}

/************************************************************************
*  class Puma								*
************************************************************************/
Puma::Puma(const char* ttyname)
    :Serial(ttyname), _axis(Jt1), _echo(NoEcho)
{
    o_through().i_igncr();
    *this << '\r';
    if (wait() == 0)		/* wait for "... floppy (Y/N)? "	*/
	return;
    *this << "n\r";
    if (wait() == 0)		/* wait for "Initialize (Y/N)? "	*/
	return;
    *this << 'n' << endc;	/* wait for prompt			*/
}

Puma&
Puma::operator +=(int dangle)
{
    return *this << "do drive " << (int)_axis << ',' << dangle << ",100"
		 << endc;
}

Puma&
Puma::operator -=(int dangle)
{
    return *this << "do drive " << (int)_axis << ',' << -dangle << ",100"
		 << endc;
}

int
Puma::wait()
{
    static const char* const	msg[] = {"\n.", "? ", "?\n"};
    static const int		nmsg = sizeof(msg) / sizeof(msg[0]);
    static const char*		p[nmsg];
    int				i;

    for (i = 0; i < nmsg; i++)
	p[i] = msg[i];
    
    while ((i = match_msg(p, nmsg)) == -1)
    {
	char	c;
	
	if (!get(c))
	    break;
	else if (c == '\0')
	    continue;
	
	if (_echo)
	    std::cerr << c;

	for (i = 0; i < nmsg; i++)
	    if (c == *p[i] || c == *(p[i] = msg[i]))
		++p[i];
    }
    
    return i;
}

Puma&
Puma::set_axis(Puma::Axis axis)
{
    _axis = axis;
    return *this;
}

/************************************************************************
*  friends of class Puma						*
************************************************************************/
Puma&
operator <<(Puma& puma, const Puma::Position& position)
{
    puma << "poi tmp" << endc;		// wait for "Change ? "
    for (u_int i = 0; i < 5; i++)
	puma << position[i] << ',';
    puma << position[5] << endc;		// wait for "Change ? "
    puma << endc;				// wait for prompt
    puma << "do move tmp" << endc;		// wait for prompt

    return puma;
}

Puma&
operator >>(Puma& puma, Puma::Position& position)
{
    puma << "where\r" >> skipl >> skipl;	// ignore "X Y Z O A T"
    operator >>((Serial&)puma, position);
    return puma << wait;			// wait for prompt
}

/*
 *  Manipulators
 */
OManip1<Puma, Puma::Axis>
axis(Puma::Axis axis)
{
    return OManip1<Puma, Puma::Axis>(&Puma::set_axis, axis);
}


Puma&	wait   (Puma& puma)	{puma.wait(); return puma;}
Puma&	calib  (Puma& puma)	{return puma << "cal" << endc << 'y' << endc;}
Puma&	ready  (Puma& puma)	{return puma << "do ready" << endc;}
Puma&	nest   (Puma& puma)	{return puma << "do nest" << endc;}
Puma&	echo   (Puma& puma)	{puma._echo = Puma::DoEcho; return puma;}
Puma&	no_echo(Puma& puma)	{puma._echo = Puma::NoEcho; return puma;}

}

