/*
 *  $Id: Ieee1394CameraArray.cc,v 1.4 2010-12-19 20:04:04 ueshiba Exp $
 */
#include "TU/Ieee1394CameraArray.h"

#ifdef HAVE_LIBTUTOOLS__
namespace TU
{
/************************************************************************
*  class Ieee1394CameraArray						*
************************************************************************/
//! $B6u$N(BIEEE1394$B%G%8%?%k%+%a%i$NG[Ns$r@8@.$9$k!%(B
Ieee1394CameraArray::Ieee1394CameraArray()
    :Array<Ieee1394Camera*>()
{
}
    
//! IEEE1394$B%G%8%?%k%+%a%i$NG[Ns$r@8@.$9$k!%(B
/*!
  \param in		$B%+%a%i$N@_Dj%U%!%$%k$rFI$_9~$`F~NO%9%H%j!<%`(B
  \param i1394b		IEEE1394b$B%b!<%I(B (800Mbps)$B$GF0:n(B
  \param ncameras	$B@8@.$9$k%+%a%iBf?t!%@_Dj%U%!%$%k$K5-$5$l$F$$$k:G=i$N(B
			ncameras$BBf$,@8@.$5$l$k!%(B-1$B$r;XDj$9$k$H!$@_Dj%U%!%$%k(B
			$BCf$NA4%+%a%i$,@8@.$5$l$k!%(B
*/
Ieee1394CameraArray::Ieee1394CameraArray(std::istream& in, bool i1394b,
					 int ncameras)
    :Array<Ieee1394Camera*>()
{
    initialize(in, i1394b, ncameras);
}
    
//! IEEE1394$B%G%8%?%k%+%a%i$NG[Ns$r=i4|2=$9$k!%(B
/*!
  \param in		$B%+%a%i$N@_Dj%U%!%$%k$rFI$_9~$`F~NO%9%H%j!<%`(B
  \param i1394b		IEEE1394b$B%b!<%I(B (800Mbps)$B$GF0:n(B
  \param ncameras	$B@8@.$9$k%+%a%iBf?t!%@_Dj%U%!%$%k$K5-$5$l$F$$$k:G=i$N(B
			ncameras$BBf$,@8@.$5$l$k!%(B-1$B$r;XDj$9$k$H!$@_Dj%U%!%$%k(B
			$BCf$NA4%+%a%i$,@8@.$5$l$k!%(B
*/
void
Ieee1394CameraArray::initialize(std::istream& in, bool i1394b, int ncameras)
{
    using namespace	std;

  // $B8=:_@_Dj$5$l$F$$$kA4%+%a%i$rGQ4~$9$k!%(B
    for (int i = 0; i < dim(); ++i)
	delete (*this)[i];

  // $B@_Dj%U%!%$%k$+$iCY1d%Q%i%a!<%?$H%+%a%i?t$rFI$_9~$`!%(B
    int	delay, n;
    in >> delay >> n;
    if ((ncameras < 0) || (ncameras > n))
	ncameras = n;
    resize(ncameras);
    
  // $B@_Dj%U%!%$%k$K5-$5$l$?A4%+%a%i$r@8@.$9$k!%(B
    for (int i = 0; i < dim(); ++i)
    {
	string		s;
	in >> s;			// global unique ID$B$NFI$_9~$_(B
	u_int64_t	uniqId = strtoull(s.c_str(), 0, 0);
	(*this)[i] = new Ieee1394Camera(Ieee1394Camera::Monocular,
					i1394b, uniqId, delay);
	in >> *(*this)[i];		// $B%+%a%i%Q%i%a!<%?$NFI$_9~$_$H@_Dj(B
    }
}

//! IEEE1394$B%G%8%?%k%+%a%i$NG[Ns$rGK2u$9$k!%(B
Ieee1394CameraArray::~Ieee1394CameraArray()
{
    for (int i = 0; i < dim(); ++i)
	delete (*this)[i];
}
 
}
#endif	/* HAVE_LIBTUTOOLS__	*/    
