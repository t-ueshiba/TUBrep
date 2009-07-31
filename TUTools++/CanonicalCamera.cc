/*
 *  ����14-19�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
 *  
 *  �n��ҁF�A�ŏr�v
 *
 *  �{�v���O�����́i�Ɓj�Y�ƋZ�p�����������̐E���ł���A�ŏr�v���n�삵�C
 *  �i�Ɓj�Y�ƋZ�p���������������쌠�����L����閧���ł��D���쌠���L
 *  �҂ɂ�鋖�Ȃ��ɖ{�v���O�������g�p�C�����C���ρC��O�҂֊J������
 *  ���̍s�ׂ��֎~���܂��D
 *  
 *  ���̃v���O�����ɂ���Đ����邢���Ȃ鑹�Q�ɑ΂��Ă��C���쌠���L�҂�
 *  ��ёn��҂͐ӔC�𕉂��܂���B
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
 *  $Id: CanonicalCamera.cc,v 1.10 2009-07-31 07:04:44 ueshiba Exp $
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
