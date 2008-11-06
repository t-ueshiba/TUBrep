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
 *  $Id: CorrectIntensity.h,v 1.3 2008-11-06 23:21:12 ueshiba Exp $
 */
#ifndef	__TUCorrectIntensity_h
#define	__TUCorrectIntensity_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class CorrectIntensity						*
************************************************************************/
//! $B2hA|$N@~7A51EYJd@5$r9T$&%/%i%9(B
class CorrectIntensity
{
  public:
  //! $B%*%U%;%C%H$H%2%$%s$r;XDj$7$F51EYJd@5%*%V%8%'%/%H$r@8@.$9$k!%(B
  /*!
    \param offset	$B%*%U%;%C%H(B
    \param gain		$B%2%$%s(B
  */
    CorrectIntensity(float offset=0.0, float gain=1.0)
	:_offset(offset), _gain(gain) 					{}

    void	initialize(float offset, float gain)			;
    template <class T>
    void	operator()(Image<T>& image, int vs=0, int ve=0)	const	;
    
  private:
    template <class T>
    T		val(T pixel)					const	;
    
    float	_offset;
    float	_gain;
};

//! $B%*%U%;%C%H$H%2%$%s$r;XDj$7$F51EYJd@5%*%V%8%'%/%H$r=i4|2=$9$k!%(B
/*!
  \param offset		$B%*%U%;%C%H(B
  \param gain		$B%2%$%s(B
*/
inline void
CorrectIntensity::initialize(float offset, float gain)
{
    _offset = offset;
    _gain   = gain;
}

}

#endif	/* !__TUCorrectIntensity_h */
