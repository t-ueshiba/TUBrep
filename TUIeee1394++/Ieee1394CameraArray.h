/*
 *  $Id: Ieee1394CameraArray.h,v 1.10 2012-08-10 02:54:48 ueshiba Exp $
 */
/*!
  \file		Ieee1394CameraArray.h
  \brief	$B%/%i%9(B TU::Ieee1394CameraArray $B$NDj5A$H<BAu(B
*/
#ifndef __TUIeee1394CameraArray_h
#define __TUIeee1394CameraArray_h

#ifdef HAVE_LIBTUTOOLS__
#  include "TU/Array++.h"
#  include "TU/Ieee1394++.h"
#  include "TU/io.h"

//! $B%G%U%)%k%H$N%+%a%iL>(B
#  define DEFAULT_CAMERA_NAME	"IEEE1394Camera"
//! $B%+%a%i@_Dj%U%!%$%k$r<}$a$k%G%U%)%k%H$N%G%#%l%/%H%jL>(B
#  define DEFAULT_CONFIG_DIRS	".:/usr/local/etc/cameras"

namespace TU
{
/************************************************************************
*  class Ieee1394CameraArray						*
************************************************************************/
//! IEEE1394$B%G%8%?%k%+%a%i$NG[Ns$rI=$9%/%i%9(B
/*!
  TU::Ieee1394Camera$B$X$N%]%$%s%?$NG[Ns$H$7$FDj5A$5$l$k(B.
*/
class Ieee1394CameraArray : public Array<Ieee1394Camera*>
{
  public:
    Ieee1394CameraArray()						;
    Ieee1394CameraArray(const char* name, const char* dirs=0,
			Ieee1394Node::Speed speed=Ieee1394Node::SPD_400M,
			int ncameras=-1)				;
    ~Ieee1394CameraArray()						;

    void		initialize(const char* name, const char* dirs=0,
				   Ieee1394Node::Speed
				       speed=Ieee1394Node::SPD_400M,
				   int ncameras=-1)			;
    const std::string&	fullName()				const	;
    std::string		configFile()				const	;
    std::string		calibFile()				const	;

  private:
    std::string		_fullName;	//!< $B%+%a%i$N(Bfull path$BL>(B
};

//! $B%+%a%i$N(Bfull path$BL>$rJV$9(B.
/*!
  \return	$B%+%a%i$N(Bfull path$BL>(B
*/
inline const std::string&
Ieee1394CameraArray::fullName() const
{
    return _fullName;
}
    
//! $B%+%a%i@_Dj%U%!%$%kL>$rJV$9(B.
/*!
  \return	$B%+%a%i@_Dj%U%!%$%kL>(B
*/
inline std::string
Ieee1394CameraArray::configFile() const
{
    return _fullName + ".conf";
}
    
//! $B%-%c%j%V%l!<%7%g%s%U%!%$%kL>$rJV$9(B.
/*!
  \return	$B%-%c%j%V%l!<%7%g%s%U%!%$%kL>(B
*/
inline std::string
Ieee1394CameraArray::calibFile() const
{
    return _fullName + ".calib";
}
    
}
#endif	/* HAVE_LIBTUTOOLS__		*/    
#endif	/* !__TUIeeeCameraArray_h	*/
