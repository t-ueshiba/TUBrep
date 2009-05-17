/*
 *  $Id: Ieee1394CameraArray.h,v 1.2 2009-05-17 23:36:38 ueshiba Exp $
 */
#ifndef __TUIeee1394CameraArray_h
#define __TUIeee1394CameraArray_h

#ifdef HAVE_LIBTUTOOLS__
#  include "TU/Array++.h"
#  include "TU/Ieee1394++.h"

namespace TU
{
/************************************************************************
*  class Ieee1394CameraArray						*
************************************************************************/
//! IEEE1394$B%G%8%?%k%+%a%i$NG[Ns$rI=$9%/%i%9(B
/*!
  TU::Ieee1394Camera$B$X$N%]%$%s%?$NG[Ns$H$7$FDj5A$5$l$k!%(B
*/
class Ieee1394CameraArray : public Array<Ieee1394Camera*>
{
  public:
    Ieee1394CameraArray(std::istream& in, bool i1394b, int ncameras=-1)	;
    ~Ieee1394CameraArray()						;
};

}
#endif	/* HAVE_LIBTUTOOLS__		*/    
#endif	/* !__TUIeeeCameraArray_h	*/
