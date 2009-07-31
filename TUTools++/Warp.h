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
 *  $Id: Warp.h,v 1.9 2009-07-31 07:04:46 ueshiba Exp $
 */
#ifndef	__TUWarp_h
#define	__TUWarp_h

#include "TU/Image++.h"
#include "TU/Camera.h"
#include "TU/mmInstructions.h"

namespace TU
{
/************************************************************************
*  class Warp								*
************************************************************************/
//! �摜��ό`���邽�߂̃N���X
class Warp
{
  private:
    struct FracArray
    {
	FracArray(u_int d=0)
	    :us(d), vs(d), du(d), dv(d), lmost(0)	{}

	u_int		width()			const	{return us.dim();}
	void		resize(u_int d)			;

#if defined(__INTEL_COMPILER)
	Array<short,  AlignedBuf<short> >	us, vs;
	Array<u_char, AlignedBuf<u_char> >	du, dv;
#else
	Array<short>				us, vs;
	Array<u_char>				du, dv;
#endif
	int					lmost;
    };
    
  public:
  //! �摜�ό`�I�u�W�F�N�g�𐶐�����D
    Warp()	:_fracs(), _width(0)			{}

  //! �o�͉摜�̕���Ԃ��D
  /*!
    return	�o�͉摜�̕�
  */
    u_int	width()				const	{return _width;}

  //! �o�͉摜�̍�����Ԃ��D
  /*!
    return	�o�͉摜�̍���
  */
    u_int	height()			const	{return _fracs.dim();}
    
    int		lmost(int v)			const	;
    int		rmost(int v)			const	;

    void	initialize(const Matrix33d& Htinv,
			   u_int inWidth,  u_int inHeight,
			   u_int outWidth, u_int outHeight)		;
    void	initialize(const Matrix33d& Htinv,
			   const CameraBase::Intrinsic& intrinsic,
			   u_int inWidth,  u_int inHeight,
			   u_int outWidth, u_int outHeight)		;
    template <class T>
    void	operator ()(const Image<T>& in, Image<T>& out,
			    int vs=0, int ve=0)			const	;
    Vector2f	operator ()(int u, int v)			const	;
#if defined(SSE2)
    mmFlt	src(int u, int v)				const	;
#endif

  private:
    Array<FracArray>	_fracs;
    u_int		_width;
};

inline void
Warp::FracArray::resize(u_int d)
{
    us.resize(d);
    vs.resize(d);
    du.resize(d);
    dv.resize(d);
}

//! �o�͉摜�ɂ�����w�肳�ꂽ�s�̗L�����[�ʒu��Ԃ��D
/*!
  ���͉摜����`�ł��o�͉摜����`�Ƃ͌���Ȃ��̂ŁC�o�͉摜�̈ꕔ����
  ���͉摜�̒l��(�L���̈�)�ƂȂ�Ȃ��D�{�֐��́C�o�͉摜�̎w�肳�ꂽ�s
  �ɂ��āC���̗L���̈�̍��[�ƂȂ��f�ʒu��Ԃ��D
  \param v	�s���w�肷��intex
  \return	���[�ʒu
*/
inline int
Warp::lmost(int v) const
{
    return _fracs[v].lmost;
}

//! �o�͉摜�ɂ�����w�肳�ꂽ�s�̗L���E�[�ʒu�̎���Ԃ��D
/*!
  ���͉摜����`�ł��o�͉摜����`�Ƃ͌���Ȃ��̂ŁC�o�͉摜�̈ꕔ����
  ���͉摜�̒l��(�L���̈�)�ƂȂ�Ȃ��D�{�֐��́C�o�͉摜�̎w�肳�ꂽ�s
  �ɂ��āC���̗L���̈�̉E�[�̉E�ׂƂȂ��f�ʒu��Ԃ��D
  \param v	�s���w�肷��intex
  \return	�E�[�ʒu�̎�
*/
inline int
Warp::rmost(int v) const
{
    return _fracs[v].lmost + _fracs[v].width();
}

//! �o�͉摜�_���w�肵�Ă���Ƀ}�b�v�������͉摜�_��2�������W��Ԃ��D
/*!
  \param u	�o�͉摜�_�̉����W
  \param v	�o�͉摜�_�̏c���W
  \return	�o�͉摜�_(u, v)�Ƀ}�b�v�������͉摜�_��2�������W
*/
inline Vector2f
Warp::operator ()(int u, int v) const
{
    Vector2f		val;
    const FracArray&	fracs = _fracs[v];
    val[0] = float(fracs.us[u]) + float(fracs.du[u]) / 128.0f;
    val[1] = float(fracs.vs[u]) + float(fracs.dv[u]) / 128.0f;
    return val;
}

#if defined(SSE2)
//! 2�̏o�͉摜�_���w�肵�Ă��ꂼ��Ƀ}�b�v�����2�̓��͉摜�_��2�������W��Ԃ��D
/*!
  �w�肳�ꂽ2�������W(u, v)�ɑ΂��C2�_(u, v-1), (u, v)�Ƀ}�b�v�������͉摜�_��
  2�������W���Ԃ����D
  \param u	�o�͉摜�_�̉����W
  \param v	�o�͉摜�_�̏c���W
  \return	�o�͉摜�_(u, v-1), (u, v)�Ƀ}�b�v�������͉摜�_��2�������W
*/
inline mmFlt
Warp::src(int u, int v) const
{
    const FracArray	&fp = _fracs[v-1], &fc = _fracs[v];
    const mmInt16	tmp = mmSet<mmInt16>(fc.dv[u], fc.du[u],
					     fp.dv[u], fp.du[u],
					     fc.vs[u], fc.us[u],
					     fp.vs[u], fp.us[u]);
    return mmCvt<mmFlt>(tmp) +
	   mmCvt<mmFlt>(mmShiftElmR<4>(tmp)) / mmSetAll<mmFlt>(128.0);
}
#endif
}

#endif	/* !__TUWarp_h */
