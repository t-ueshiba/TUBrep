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
 *  $Id: Rotation.cc,v 1.12 2009-07-31 07:04:45 ueshiba Exp $
 */
#include "TU/Vector++.h"

namespace TU
{
static inline double	sqr(double x)	{return x * x;}
    
//! 2���������ʓ��ł̉�]�𐶐�����
/*!
  \param p	p�����w�肷��index
  \param q	q�����w�肷��index
  \param x	��]�p�𐶐�����ۂ�x�l
  \param y	��]�p�𐶐�����ۂ�y�l
		\f[
		  \cos\theta = \frac{x}{\sqrt{x^2+y^2}},{\hskip 1em}
		  \sin\theta = \frac{y}{\sqrt{x^2+y^2}}
		\f]
*/
Rotation::Rotation(u_int p, u_int q, double x, double y)
    :_p(p), _q(q), _l(1.0), _c(1.0), _s(0.0)
{
    const double	absx = fabs(x), absy = fabs(y);
    _l = (absx > absy ? absx * sqrt(1.0 + sqr(absy/absx))
		      : absy * sqrt(1.0 + sqr(absx/absy)));
    if (_l != 0.0)
    {
	_c = x / _l;
	_s = y / _l;
    }
}

//! 2���������ʓ��ł̉�]�𐶐�����
/*!
  \param p	p�����w�肷��index
  \param q	q�����w�肷��index
  \param theta	��]�p
*/
Rotation::Rotation(u_int p, u_int q, double theta)
    :_p(p), _q(q), _l(1.0), _c(::cos(theta)), _s(::sin(theta))
{
}
 
}
