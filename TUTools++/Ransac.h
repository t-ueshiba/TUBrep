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
 *  $Id: Ransac.h,v 1.5 2009-07-31 07:04:45 ueshiba Exp $
 */
#ifndef __TURansac_h
#define __TURansac_h

#include <math.h>
#include <stdexcept>
#include "TU/types.h"

namespace TU
{
/************************************************************************
*  function ransac							*
************************************************************************/
//! RANSAC�ɂ����outlier���܂ޓ_�W���Ƀ��f���𓖂Ă͂߂�D
/*!
  �e���v���[�g�p�����[�^PointSet�͓_�W����\���N���X�ł���C�ȉ��̏�����
  ���������ƁF
  -# forward_iterator���T�|�[�g����R���e�i�ł���D
  -# ���̃R���e�i�̌^��
	PointSet::Container
     �Ƃ������O��typedef���Ă���D
  -# inlier�̊����������o�֐�
	double	Pointset::inlierRate() const;
     �ɂ���Ēm�邱�Ƃ��ł���D
  -# �����o�֐�
	PoinstSet::Container	Pointset::sample(u_int npoints) const;
     �ɂ���ă����_����npoints�̕����W�������o����D

  �e���v���[�g�p�����[�^Model�͓��Ă͂߂�ׂ����f����\���N���X�ł���C
  �ȉ��̏����𖞂������ƁF
  -# �����o�֐�
	template <class Iterator>
	void	Model::fit(Iterator first, Iterator last);
     �ɂ���ē_�W���Ƀ��f���𓖂Ă͂߂邱�Ƃ��ł���D
  -# 1.�ɕK�v�ȍŏ��_���������o�֐�
	u_int	Model::ndataMin() const;
     �ɂ���Ēm�邱�Ƃ��ł���D

  �e���v���[�g�p�����[�^Conform�͓_�̃��f���ւ̓K�����𔻒肷��֐�
  �I�u�W�F�N�g�ł���C
	bool	Conform::operator()(const PointSet::Container::valu_type& p,
				    const Model& model);
  �Ȃ�C���^�t�F�[�X�ɂ����p��model�ɓK�����Ă��邩����ł��邱�ƁD

  \param pointSet	inlier��outlier���܂ޓ_�W��
  \param model		pointSet�Ɋ܂܂��inlier�𓖂Ă͂߂郂�f���D
  \param hitRate	RANSAC�ɂ���Đ�����inlier���������Ă�m���D
			0 <= hitRate < 1�łȂ���΂Ȃ�Ȃ��D
  \return		pointSet�Ɋ܂܂��inlier
*/
template <class PointSet, class Model, class Conform>
typename PointSet::Container
ransac(const PointSet& pointSet, Model& model, Conform conform,
       double hitRate=0.99)
{
    typedef typename PointSet::Container	Container;
    
  // �^����ꂽhitRate�CPointSet�Ɋ܂܂��inlier�̊��������Model�̐�����
  // �v����ŏ��_������C�T���v�����O�̕K�v�񐔂����߂�D
    if (hitRate < 0.0 || hitRate >= 1.0)
	throw std::invalid_argument("ransac<PointSet, Model>: given hit rate is not within [0, 1)!!");
    const double	inlierRate = pointSet.inlierRate();
    if (inlierRate < 0.0 || inlierRate >= 1.0)
	throw std::invalid_argument("ransac<PointSet, Model>: inlier rate is not within [0, 1)!!");
    double	tmp = 1.0;
    for (u_int n = model.ndataMin(); n-- > 0; )
	tmp *= inlierRate;
    const u_int	ntrials = u_int(ceil(log(1.0 - hitRate) / log(1.0 - tmp)));

  // ���s�i�ŏ����̓_���T���v���C���f�������Cinlier���o�j��ntrials��s���D
    Container	inlierSetA, inlierSetB;
    Container	*inliers = &inlierSetA, *inliersMax = &inlierSetB;
    for (int n = 0; n < ntrials; ++n)
    {
      // �_�W�����烂�f���̌v�Z�ɕK�v�ȍŏ����̓_���T���v������D
	const Container&	minimalSet = pointSet.sample(model.ndataMin());

      // �T���v�������_���烂�f���𐶐�����D
	model.fit(minimalSet.begin(), minimalSet.end());

      // �S�_�̒��Ő����������f���ɓK������(inlier)���̂��W�߂�D
	inliers->clear();
	for (typename PointSet::const_iterator iter = pointSet.begin();
	     iter != pointSet.end(); ++iter)
	    if (conform(*iter, model))
		inliers->push_back(*iter);

      // ����܂ł̂ǂ̃��f������inlier�̐���������΂��̏W�����L�^����D
	if (inliers->size() > inliersMax->size())
	    std::swap(inliers, inliersMax);
    }
  // �ő�W���Ɋ܂܂��_��^��inlier�Ƃ��C�����S�Ă��烂�f���𐶐�����D
    model.fit(inliersMax->begin(), inliersMax->end());

    return *inliersMax;
}
    
}

#endif // !__TURansac_h
