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
 *  $Id: Vector++.inst.cc,v 1.13 2011-08-05 07:15:13 ueshiba Exp $
 */
#include "TU/Vector++.h"

namespace TU
{
template class Vector<float,  FixedSizedBuf<float,  2> >;
template class Vector<double, FixedSizedBuf<double, 2> >;

template class Vector<float,  FixedSizedBuf<float,  3> >;
template class Vector<double, FixedSizedBuf<double, 3> >;

template class Vector<float,  FixedSizedBuf<float,  4> >;
template class Vector<double, FixedSizedBuf<double, 4> >;

template class Matrix<float,  FixedSizedBuf<float,   4>,
		      FixedSizedBuf<Vector<float>,   2> >;
template class Matrix<double, FixedSizedBuf<double,  4>,
		      FixedSizedBuf<Vector<double>,  2> >;
template class Matrix<float,  FixedSizedBuf<float,   6>,
		      FixedSizedBuf<Vector<float>,   2> >;
template class Matrix<double, FixedSizedBuf<double,  6>,
		      FixedSizedBuf<Vector<double>,  2> >;
template class Matrix<float,  FixedSizedBuf<float,   9>,
		      FixedSizedBuf<Vector<float>,   3> >;
template class Matrix<double, FixedSizedBuf<double,  9>,
		      FixedSizedBuf<Vector<double>,  3> >;
template class Matrix<float,  FixedSizedBuf<float,  12>,
		      FixedSizedBuf<Vector<float>,   3> >;
template class Matrix<double, FixedSizedBuf<double, 12>,
		      FixedSizedBuf<Vector<double>,  3> >;
template class Matrix<float,  FixedSizedBuf<float,  16>,
		      FixedSizedBuf<Vector<float>,   4> >;
template class Matrix<double, FixedSizedBuf<double, 16>,
		      FixedSizedBuf<Vector<double>,  4> >;

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
