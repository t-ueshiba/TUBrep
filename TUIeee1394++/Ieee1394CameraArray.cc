/*
 *  $Id: Ieee1394CameraArray.cc,v 1.10 2012-08-10 02:54:48 ueshiba Exp $
 */
#include <cstdlib>
#include "TU/Ieee1394CameraArray.h"

#ifdef HAVE_LIBTUTOOLS__

namespace TU
{
/************************************************************************
*  class Ieee1394CameraArray						*
************************************************************************/
//! $B6u$N(BIEEE1394$B%G%8%?%k%+%a%i$NG[Ns$r@8@.$9$k(B.
Ieee1394CameraArray::Ieee1394CameraArray()
    :Array<Ieee1394Camera*>()
{
}
    
//! IEEE1394$B%G%8%?%k%+%a%i$NG[Ns$r@8@.$9$k(B.
/*!
  \param name		$B%+%a%iL>(B
  \param dirs		$B%+%a%i@_Dj%U%!%$%k$NC5:w%G%#%l%/%H%jL>$NJB$S(B
  \param i1394b		IEEE1394b$B%b!<%I(B (800Mbps)$B$GF0:n(B
  \param ncameras	$B@8@.$9$k%+%a%iBf?t(B. $B@_Dj%U%!%$%k$K5-$5$l$F$$$k:G=i$N(B
			ncameras$BBf$,@8@.$5$l$k(B. -1$B$r;XDj$9$k$H(B, $B@_Dj%U%!%$%k(B
			$BCf$NA4%+%a%i$,@8@.$5$l$k(B. 
*/
Ieee1394CameraArray::Ieee1394CameraArray(const char* name, const char* dirs,
					 Ieee1394Node::Speed speed,
					 int ncameras)
    :Array<Ieee1394Camera*>(), _fullName()
{
    initialize(name, dirs, speed, ncameras);
}
    
//! IEEE1394$B%G%8%?%k%+%a%i$NG[Ns$r=i4|2=$9$k(B.
/*!
  \param name		$B%+%a%iL>(B
  \param dirs		$B%+%a%i@_Dj%U%!%$%k$NC5:w%G%#%l%/%H%jL>$NJB$S(B
  \param i1394b		IEEE1394b$B%b!<%I(B (800Mbps)$B$GF0:n(B
  \param ncameras	$B@8@.$9$k%+%a%iBf?t(B. $B@_Dj%U%!%$%k$K5-$5$l$F$$$k:G=i$N(B
			ncameras$BBf$,@8@.$5$l$k(B. -1$B$r;XDj$9$k$H(B, $B@_Dj%U%!%$%k(B
			$BCf$NA4%+%a%i$,@8@.$5$l$k(B. 
*/
void
Ieee1394CameraArray::initialize(const char* name, const char* dirs,
				Ieee1394Node::Speed speed, int ncameras)
{
    using namespace	std;

  // $B8=:_@_Dj$5$l$F$$$kA4%+%a%i$rGQ4~$9$k(B.
    for (int i = 0; i < dim(); ++i)
	delete (*this)[i];

  // $B@_Dj%U%!%$%k$N(Bfull path$BL>$r@8@.$7(B, $B%U%!%$%k$r%*!<%W%s$9$k(B.
    ifstream	in;
    _fullName = openFile(in,
			 string(name != 0 ? name : DEFAULT_CAMERA_NAME),
			 string(dirs != 0 ? dirs : DEFAULT_CONFIG_DIRS),
			 ".conf");
    
  // $B@_Dj%U%!%$%k$+$iCY1d%Q%i%a!<%?$H%+%a%i?t$rFI$_9~$`(B.
    int	delay, n;
    in >> delay >> n;
    if ((ncameras < 0) || (ncameras > n))
	ncameras = n;
    resize(ncameras);
    
  // $B@_Dj%U%!%$%k$K5-$5$l$?A4%+%a%i$r@8@.$9$k(B.
    for (int i = 0; i < dim(); ++i)
    {
	string		s;
	in >> s;			// global unique ID$B$NFI$_9~$_(B
	u_int64_t	uniqId = strtoull(s.c_str(), 0, 0);
	(*this)[i] = new Ieee1394Camera(Ieee1394Camera::Monocular,
					uniqId, speed, delay);
	in >> *(*this)[i];		// $B%+%a%i%Q%i%a!<%?$NFI$_9~$_$H@_Dj(B
    }
}

//! IEEE1394$B%G%8%?%k%+%a%i$NG[Ns$rGK2u$9$k(B.
Ieee1394CameraArray::~Ieee1394CameraArray()
{
    for (int i = 0; i < dim(); ++i)
	delete (*this)[i];
}

}
#endif	/* HAVE_LIBTUTOOLS__	*/    
