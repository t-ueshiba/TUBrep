/*
 *  平成14-19年（独）産業技術総合研究所 著作権所有
 *  
 *  創作者：植芝俊夫
 *
 *  本プログラムは（独）産業技術総合研究所の職員である植芝俊夫が創作し，
 *  （独）産業技術総合研究所が著作権を所有する秘密情報です．著作権所有
 *  者による許可なしに本プログラムを使用，複製，改変，第三者へ開示する
 *  等の行為を禁止します．
 *  
 *  このプログラムによって生じるいかなる損害に対しても，著作権所有者お
 *  よび創作者は責任を負いません。
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
 *  $Id$
 */
/*!
  \file		Ransac.h
  \brief	RANSACを行う関数の定義と実装
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
//! RANSACによってoutlierを含む点集合にモデルを当てはめる．
/*!
  テンプレートパラメータPointSetは点集合を表すクラスであり，以下の条件を
  満たすこと：
  -# forward_iteratorをサポートするコンテナである．
  -# このコンテナの型を
	PointSet::Container
     という名前でtypedefしている．
  -# inlierの割合をメンバ関数
	double	Pointset::inlierRate() const;
     によって知ることができる．
  -# メンバ関数
	PoinstSet::Container	Pointset::sample(u_int npoints) const;
     によってランダムにnpoints個の部分集合を取り出せる．

  テンプレートパラメータModelは当てはめるべきモデルを表すクラスであり，
  以下の条件を満たすこと：
  -# メンバ関数
	template <class Iterator>
	void	Model::fit(Iterator first, Iterator last);
     によって点集合にモデルを当てはめることができる．
  -# 1.に必要な最少点数をメンバ関数
	u_int	Model::ndataMin() const;
     によって知ることができる．

  テンプレートパラメータConformは点のモデルへの適合性を判定する関数
  オブジェクトであり，
	bool	Conform::operator()(const PointSet::Container::valu_type& p,
				    const Model& model);
  なるインタフェースによってpがmodelに適合しているか判定できること．

  \param pointSet	inlierとoutlierを含む点集合
  \param model		pointSetに含まれるinlierを当てはめるモデル
  \param conform	点のモデルへの適合性を判定する関数オブジェクト
  \param hitRate	RANSACによって正しくinlierを引き当てる確率.
			0 <= hitRate < 1でなければならない
  \return		pointSetに含まれるinlier
*/
template <class PointSet, class Model, class Conform>
typename PointSet::Container
ransac(const PointSet& pointSet, Model& model, Conform conform,
       double hitRate=0.99)
{
    using namespace	std;
    
    typedef typename PointSet::Container	Container;

    if (distance(pointSet.begin(), pointSet.end()) < model.ndataMin())
	throw runtime_error("ransac<PointSet, Model>: not enough points in the given point set!!");
    
  // 与えられたhitRate，PointSetに含まれるinlierの割合およびModelの生成に
  // 要する最少点数から，サンプリングの必要回数を求める．
    if (hitRate < 0.0 || hitRate >= 1.0)
	throw invalid_argument("ransac<PointSet, Model>: given hit rate is not within [0, 1)!!");
    const double	inlierRate = pointSet.inlierRate();
    if (inlierRate < 0.0 || inlierRate >= 1.0)
	throw invalid_argument("ransac<PointSet, Model>: inlier rate is not within [0, 1)!!");
    double	tmp = 1.0;
    for (u_int n = model.ndataMin(); n-- > 0; )
	tmp *= inlierRate;
    const u_int	ntrials = u_int(ceil(log(1.0 - hitRate) / log(1.0 - tmp)));

  // 試行（最小個数の点をサンプル，モデル生成，inlier検出）をntrials回行う．
    Container	inlierSetA, inlierSetB;
    Container	*inliers = &inlierSetA, *inliersMax = &inlierSetB;
    for (u_int n = 0; n < ntrials; ++n)
    {
      // 点集合からモデルの計算に必要な最小個数の点をサンプルする．
	const Container&	minimalSet = pointSet.sample(model.ndataMin());

      // サンプルした点からモデルを生成する．
	model.fit(minimalSet.begin(), minimalSet.end());

      // 全点の中で生成したモデルに適合する(inlier)ものを集める．
	inliers->clear();
	for (typename PointSet::const_iterator iter = pointSet.begin();
	     iter != pointSet.end(); ++iter)
	    if (conform(*iter, model))
		inliers->push_back(*iter);

      // これまでのどのモデルよりもinlierの数が多ければその集合を記録する．
	if (inliers->size() > inliersMax->size())
	    swap(inliers, inliersMax);
    }
  // 最大集合に含まれる点を真のinlierとし，それら全てからモデルを生成する．
  // サンプルされた点（minimalSetに含まれる点）が持つ自由度がモデルの自由度
  // よりも大きい場合は，これらに誤差0でモデルを当てはめられるとは限らないので，
  // これらの点が必ずinlierに含まれる保証はない．よって，inlierが足りなくて
  // 次の当てはめが失敗することもあり得る．
    model.fit(inliersMax->begin(), inliersMax->end());

    return *inliersMax;
}
    
}

#endif // !__TURansac_h
