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
 *  $Id: CanonicalCamera.cc,v 1.9 2008-10-03 04:23:37 ueshiba Exp $
 */
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class CanonicalCamera						*
************************************************************************/
CameraBase&
CanonicalCamera::setProjection(const Matrix34d& PP)
{
    SVDecomposition<double>	svd(PP(0, 0, 3, 3));
    if (svd[2] < 0.0)
	throw std::runtime_error("TU::CanonicalCamera::setProjection: cannot extract camera rotation due to nevative singular value!");
    setRotation(svd.Vt().trns() * svd.Ut());
    Vector<double>	tc(3);
    tc[0] = -PP[0][3];
    tc[1] = -PP[1][3];
    tc[2] = -PP[2][3];
    setTranslation(tc * Rt());

    return *this;
}

const CameraBase::Intrinsic&
CanonicalCamera::intrinsic() const
{
    return _intrinsic;
}

CameraBase::Intrinsic&
CanonicalCamera::intrinsic()
{
    return _intrinsic;
}
    
}
