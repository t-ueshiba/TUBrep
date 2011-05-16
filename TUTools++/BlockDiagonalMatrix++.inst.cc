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
 *  $Id: BlockDiagonalMatrix++.inst.cc,v 1.1 2011-05-16 05:33:38 ueshiba Exp $
 */
#include "TU/BlockDiagonalMatrix++.h"

namespace TU
{
template class Array<Matrix<float> >;
template class Array<Matrix<double> >;
template class BlockDiagonalMatrix<float>;
template class BlockDiagonalMatrix<double>;

template BlockDiagonalMatrix<float>
operator *(const BlockDiagonalMatrix<float>&,
	   const BlockDiagonalMatrix<float>&);

template BlockDiagonalMatrix<double>
operator *(const BlockDiagonalMatrix<double>&,
	   const BlockDiagonalMatrix<double>&);

template Matrix<float>
operator *(const BlockDiagonalMatrix<float>&, const Matrix<float>&);

template Matrix<double>
operator *(const BlockDiagonalMatrix<double>&, const Matrix<double>&);

template Matrix<float>
operator *(const Matrix<float>&, const BlockDiagonalMatrix<float>&);

template Matrix<double>
operator *(const Matrix<double>&, const BlockDiagonalMatrix<double>&);

template Vector<float>
operator *(const BlockDiagonalMatrix<float>&, const Vector<float>& v);

template Vector<double>
operator *(const BlockDiagonalMatrix<double>&, const Vector<double>& v);

template Vector<float>
operator *(const Vector<float>&, const BlockDiagonalMatrix<float>&);

template Vector<double>
operator *(const Vector<double>&, const BlockDiagonalMatrix<double>&);
}
