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
 *  $Id: Warp.h,v 1.13 2011-07-21 23:39:35 ueshiba Exp $
 */
#ifndef	__TUWarp_h
#define	__TUWarp_h

#include "TU/Image++.h"
#include "TU/Camera++.h"
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

    template <class T>
    void	initialize(const Matrix<T, FixedSizedBuf<T, 9>,
					FixedSizedBuf<Vector<T>, 3> >& Htinv,
			   u_int inWidth,  u_int inHeight,
			   u_int outWidth, u_int outHeight)		;
    template <class I>
    void	initialize(const typename I::matrix33_type& Htinv,
			   const I& intrinsic,
			   u_int inWidth,  u_int inHeight,
			   u_int outWidth, u_int outHeight)		;
    template <class T>
    void	operator ()(const Image<T>& in, Image<T>& out,
			    u_int vs=0, u_int ve=0)		const	;
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

//! �摜���ˉe�ϊ����邽�߂̍s���ݒ肷��D
/*!
  ���͉摜�_u�͎ˉe�ϊ�
  \f[
    \TUbeginarray{c} \TUvec{v}{} \\ 1 \TUendarray \simeq
    \TUvec{H}{} \TUbeginarray{c} \TUvec{u}{} \\ 1 \TUendarray
  \f]
  �ɂ���ďo�͉摜�_v�Ɏʂ����D
  \param Htinv		�ό`���w�肷��3x3�ˉe�ϊ��s��̋t�s��̓]�u�C���Ȃ킿
			\f$\TUtinv{H}{}\f$
  \param inWidth	���͉摜�̕�
  \param inHeight	���͉摜�̍���
  \param outWidth	�o�͉摜�̕�
  \param outWidth	�o�͉摜�̍���
*/
template <class T> inline void
Warp::initialize(const Matrix<T, FixedSizedBuf<T, 9>,
			      FixedSizedBuf<Vector<T>, 3> >& Htinv,
		 u_int inWidth,  u_int inHeight,
		 u_int outWidth, u_int outHeight)
{
    initialize(Htinv, IntrinsicBase<T>(),
	       inWidth, inHeight, outWidth, outHeight);
}

//! �摜�̔���`�c�݂�����������Ɏˉe�ϊ����s�����߂̍s��ƃJ���������p�����[�^��ݒ肷��D
/*!

  canonical���Wx����摜���Wu�ւ̕ϊ���\f$\TUvec{u}{} = {\cal
  K}(\TUvec{x}{})\f$ �ƕ\�����J���������p�����[�^�ɂ��āC���̐��`��
  ��������\��3x3�㔼�O�p�s���K�Ƃ���ƁC
  \f[
    \TUbeginarray{c} \TUbar{u}{} \\ 1 \TUendarray =
    \TUvec{K}{}
    \TUbeginarray{c} {\cal K}^{-1}(\TUvec{u}{}) \\ 1 \TUendarray
  \f]
  �ɂ���ĉ摜�̔���`�c�݂����������ł���D�{�֐��́C���̘c�݂����������摜�_��
  �ˉe�ϊ�H�ɂ���ďo�͉摜�_v�Ɏʂ��悤�ɕό`�p�����[�^��ݒ肷��D���Ȃ킿�C
  �S�̂̕ό`��
  \f[
    \TUbeginarray{c} \TUvec{v}{} \\ 1 \TUendarray \simeq
    \TUvec{H}{}\TUvec{K}{}
    \TUbeginarray{c} {\cal K}^{-1}(\TUvec{u}{}) \\ 1 \TUendarray
  \f]
  �ƂȂ�D
  \param Htinv		�ό`���w�肷��3x3�ˉe�ϊ��s��̋t�s��̓]�u
  \param Intrinsic	���͉摜�ɉ�������Ă�����˘c�Ȃ�\���J���������p�����[�^
  \param inWidth	���͉摜�̕�
  \param inHeight	���͉摜�̍���
  \param outWidth	�o�͉摜�̕�
  \param outWidth	�o�͉摜�̍���
*/
template <class I> void
Warp::initialize(const typename I::matrix33_type& Htinv, const I& intrinsic,
		 u_int inWidth,  u_int inHeight,
		 u_int outWidth, u_int outHeight)
{
    typedef I						intrinsic_type;
    typedef typename intrinsic_type::point2_type	point2_type;
    typedef typename intrinsic_type::vector_type	vector_type;
    typedef typename intrinsic_type::matrix_type	matrix_type;
    
    _fracs.resize(outHeight);
    _width = outWidth;
    
  /* Compute frac for each pixel. */
    const matrix_type&	HKtinv = Htinv * intrinsic.Ktinv();
    vector_type		leftmost = HKtinv[2];
    for (u_int v = 0; v < height(); ++v)
    {
	vector_type	x = leftmost;
	FracArray	frac(width());
	u_int		n = 0;
	for (u_int u = 0; u < width(); ++u)
	{
	    const point2_type&	m = intrinsic.u(point2_type(x[0]/x[2],
							    x[1]/x[2]));
	    if (0.0 <= m[0] && m[0] <= inWidth - 2 &&
		0.0 <= m[1] && m[1] <= inHeight - 2)
	    {
		if (n == 0)
		    frac.lmost = u;
		frac.us[n] = (short)floor(m[0]);
		frac.vs[n] = (short)floor(m[1]);
		frac.du[n] = (u_char)floor((m[0] - floor(m[0])) * 128.0);
		frac.dv[n] = (u_char)floor((m[1] - floor(m[1])) * 128.0);
		++n;
	    }
	    x += HKtinv[0];
	}

	_fracs[v].resize(n);
	_fracs[v].lmost = frac.lmost;
	for (u_int u = 0; u < n; ++u)
	{
	    _fracs[v].us[u] = frac.us[u];
	    _fracs[v].vs[u] = frac.vs[u];
	    _fracs[v].du[u] = frac.du[u];
	    _fracs[v].dv[u] = frac.dv[u];
	}

	leftmost += HKtinv[1];
    }
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
