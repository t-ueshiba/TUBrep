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
 *  $Id: Serial.h,v 1.4 2009-03-19 05:11:03 ueshiba Exp $
 */
#if !defined(__TUSerial_h)
#define __TUSerial_h

#include <termios.h>
#include <cstdio>
#include "TU/Vector++.h"

namespace TU
{
/************************************************************************
*  class Serial								*
************************************************************************/
class Serial
{
  public:
			Serial(const char*)			;
    virtual		~Serial()				;

    const Serial&	put(const char* s)		const	;
    const Serial&	get(char* s, u_int size)	const	;
    
    Serial&		i_nl2cr()				;
    Serial&		i_igncr()				;
    Serial&		i_cr2nl()				;
#if !defined(__APPLE__)
    Serial&		i_upper2lower()				;
#endif
    Serial&		i_through()				;
    Serial&		o_nl2crnl()				;
#if !defined(__APPLE__)
    Serial&		o_cr2nl()				;
    Serial&		o_lower2upper()				;
#endif
    Serial&		o_through()				;
    Serial&		c_baud(int)				;
    Serial&		c_csize(int)				;
    Serial&		c_even()				;
    Serial&		c_odd()					;
    Serial&		c_noparity()				;
    Serial&		c_stop1()				;
    Serial&		c_stop2()				;

  protected:
    FILE*		fp()				const	{return _fp;}
    
  private:
    Serial&		set_flag(tcflag_t termios::* flag,
				 unsigned long clearbits,
				 unsigned long setbits)		;

    const int		_fd;
    FILE* const		_fp;
    termios		_termios_bak;
};

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
    bool		getStatus(u_int& channel,
				  u_int& interval)	const	;
};
 
}

#endif	/* !__TUSerial_h	*/
