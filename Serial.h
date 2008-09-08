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
 *  $Id: Serial.h,v 1.1 2008-09-08 08:06:20 ueshiba Exp $
 */
#ifndef __TUSerial_h
#define __TUSerial_h

#if defined(__GNUC__) //&& !defined(__INTEL_COMPILER)
#  define HAVE_STDIO_FILEBUF
#endif

#include <termios.h>
#ifdef HAVE_STDIO_FILEBUF
#  include <ext/stdio_filebuf.h>
#else
#  include <fstream>
#endif
#include "TU/Manip.h"
#include "TU/Vector++.h"

namespace TU
{
/************************************************************************
*  class Serial								*
************************************************************************/
class Serial
#ifdef HAVE_STDIO_FILEBUF
    : public std::basic_iostream<char>
#else
    : public std::fstream
#endif
{
  public:
		Serial(const char*)			;
    virtual	~Serial()				;

    Serial&	i_nl2cr()				;
    Serial&	i_igncr()				;
    Serial&	i_cr2nl()				;
#ifndef __APPLE__
    Serial&	i_upper2lower()				;
#endif
    Serial&	i_through()				;
    Serial&	o_nl2crnl()				;
#ifndef __APPLE__
    Serial&	o_cr2nl()				;
    Serial&	o_lower2upper()				;
#endif
    Serial&	o_through()				;
    Serial&	c_baud(int)				;
    Serial&	c_csize(int)				;
    Serial&	c_even()				;
    Serial&	c_odd()					;
    Serial&	c_noparity()				;
    Serial&	c_stop1()				;
    Serial&	c_stop2()				;
    
  private:
    Serial&	set_flag(tcflag_t termios::* flag,
			 unsigned long clearbits,
			 unsigned long setbits)		;
#ifdef HAVE_STDIO_FILEBUF
    int		fd()					{return _fd;}
    const int	_fd;
    __gnu_cxx::stdio_filebuf<char>	_filebuf;
#else
    int		fd()					{return rdbuf()->fd();}
#endif
    termios	_termios_bak;
};

inline Serial&
operator >>(Serial& serial, Serial& (*f)(Serial&))
{
    return f(serial);
}

inline Serial&
operator <<(Serial& serial, Serial& (*f)(Serial&))
{
    return f(serial);
}

extern IOManip<Serial>	nl2cr;
#ifndef __APPLE__
extern IOManip<Serial>	cr2nl;
extern IOManip<Serial>	upperlower;
#endif
extern IOManip<Serial>	through;
Serial&			igncr	(Serial&)		;
Serial&			even	(Serial&)		;
Serial&			odd	(Serial&)		;
Serial&			noparity(Serial&)		;
Serial&			stop1	(Serial&)		;
Serial&			stop2	(Serial&)		;
OManip1<Serial, int>	baud	(int)			;
OManip1<Serial, int>	csize	(int)			;

/************************************************************************
*  class Puma								*
************************************************************************/
class Puma : public Serial
{
  public:
    typedef Vector<float, FixedSizedBuf<float, 6> >	Position;
	
    enum Axis		{Jt1=1, Jt2=2, Jt3=3, Jt4=4, Jt5=5, Jt6=6};
    
			Puma(const char*)			;
    
    Puma&		operator +=(int)			;
    Puma&		operator -=(int)			;
    
    friend Puma&	wait(Puma&)				;
    friend Puma&	echo(Puma&)				;
    friend Puma&	no_echo(Puma&)				;
    friend OManip1<Puma, Axis>	axis(Puma::Axis)		;
    
  private:
    enum Echo		{NoEcho = 0, DoEcho = 1};

    Puma&		set_axis(Puma::Axis)			;
    int			wait()					;

    Axis		_axis;
    Echo		_echo;
};

inline Puma&
operator <<(Puma& puma, Puma& (*f)(Puma&))
{
    return (*f)(puma);
}

Puma&	operator <<(Puma&, const Puma::Position)		;
Puma&	operator >>(Puma&, Puma::Position&)			;
Puma&	wait   (Puma&)						;
Puma&	calib  (Puma&)						;
Puma&	ready  (Puma&)						;
Puma&	nest   (Puma&)						;
Puma&	echo   (Puma&)						;
Puma&	no_echo(Puma&)						;

/************************************************************************
*  class Pata								*
************************************************************************/ 
class Pata : public Serial
{
  public:
		Pata(const char*)				;

  private:
    enum	{SX=2, EX=3};
};

/************************************************************************
*  class Microscope							*
************************************************************************/ 
class Microscope : public Serial
{
  public:
    enum Axis	{X = 'X', Y = 'Y', Z = 'Z'};

		Microscope(const char*)				;

    Microscope&	operator +=(int)				;
    Microscope&	operator -=(int)				;
    Microscope&	operator ++()					;
    Microscope&	operator --()					;
    
    friend OManip1<Microscope, Microscope::Axis>
		axis(Microscope::Axis)				;
    
  private:
    Microscope&	set_axis(Microscope::Axis)			;
    
    Axis	_axis;
};

inline Microscope&
operator <<(Microscope& ms, Microscope& (*f)(Microscope&))
{
    return (*f)(ms);
}

Microscope&	operator <<(Microscope&, const Array<int>&)	;
Microscope&	operator >>(Microscope&, Array<int>&)		;
Microscope&	calib(Microscope&)				;
Microscope&	ready(Microscope&)				;

/************************************************************************
*  class TriggerGenerator						*
************************************************************************/
class TriggerGenerator : public Serial
{
  public:
    TriggerGenerator(const char* ttyname)			;

    TriggerGenerator&	showId(std::ostream& out)		;
    TriggerGenerator&	selectChannel(u_int channel)		;
    TriggerGenerator&	setInterval(u_int interval)		;
    TriggerGenerator&	oneShot()				;
    TriggerGenerator&	continuousShot()			;
    TriggerGenerator&	stopContinuousShot()			;
    int			getConfiguration(u_int& channel,
					 u_int& interval)	;
};
 
}

#endif	/* !__TUSerial_h		*/
