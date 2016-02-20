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
#include "TU/Vector++.h"

namespace TU
{
template class Vector<float,  2>;
template class Vector<double, 2>;

template class Vector<float,  3>;
template class Vector<double, 3>;

template class Vector<float,  4>;
template class Vector<double, 4>;

template class Matrix<float,  2, 2>;
template class Matrix<double, 2, 2>;
template class Matrix<float,  2, 3>;
template class Matrix<double, 2, 3>;
template class Matrix<float,  3, 3>;
template class Matrix<double, 3, 3>;
template class Matrix<float,  3, 4>;
template class Matrix<double, 3, 4>;
template class Matrix<float,  4, 4>;
template class Matrix<double, 4, 4>;

template class Vector<float>;
template class Vector<double>;

template class Matrix<float>;
template class Matrix<double>;

template class LUDecomposition<float>;
template class LUDecomposition<double>;

template class Householder<float>;
template class Householder<double>;

template class QRDecomposition<float>;
template class QRDecomposition<double>;

template class TriDiagonal<float>;
template class TriDiagonal<double>;

template class BiDiagonal<float>;
template class BiDiagonal<double>;
}
