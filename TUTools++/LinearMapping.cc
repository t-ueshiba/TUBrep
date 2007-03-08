/*
 *  $Id: LinearMapping.cc,v 1.2 2007-03-08 01:32:55 ueshiba Exp $
 */
#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  class ProjectiveMapping						*
************************************************************************/
//! ���͋�ԂƏo�͋�Ԃ̎������w�肵�Ďˉe�ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  �P���ϊ��Ƃ��ď����������D
  \param inDim	���͋�Ԃ̎���
  \param outDim	�o�͋�Ԃ̎���
*/
ProjectiveMapping::ProjectiveMapping(u_int inDim, u_int outDim)
    :_T(outDim + 1, inDim + 1)
{
    u_int	n = std::min(inDim, outDim);
    for (int i = 0; i < n; ++i)
	_T[i][i] = 1.0;
    _T[outDim][inDim] = 1.0;
}
    
/************************************************************************
*  class AffineMapping							*
************************************************************************/
//! ���̃A�t�B���ϊ��̕��s�ړ�������\������x�N�g����Ԃ��D
/*! 
  \return	#outDim()�����x�N�g��
*/
Vector<double>
AffineMapping::b() const
{
    Vector<double>	bb(outDim());
    for (int j = 0; j < bb.dim(); ++j)
	bb[j] = _T[j][inDim()];

    return bb;
}
    
}

