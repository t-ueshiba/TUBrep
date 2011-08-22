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
 *  $Id: Minimize.h,v 1.7 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		Minimize.h
  \brief	�ėp�ŏ�����@�Ɋ֘A����֐��̒�`�Ǝ���
*/
#ifndef __TUMinimize_h
#define __TUMinimize_h

#include "TU/Vector++.h"
#include <algorithm>
#include <stdexcept>

namespace TU
{
/************************************************************************
*  class NullConstraint							*
************************************************************************/
//! ���̍S�����^���Ȃ��Ƃ����_�~�[�̍S��������\���N���X
/*!
  ���͔̂C�ӂ̈����ɑ΂���0�����̃x�N�g�����o�͂���x�N�g���l�֐��ł���C
  #minimizeSquare() �� #minimizeSquareSparse() �̃e���v���[�g�p�����[�^G
  �Ƃ��ė��p���邱�Ƃ�z�肵�Ă���D
  \param ET �o�̓x�N�g���̗v�f�̌^
*/
template <class ET>
class NullConstraint
{
  public:
  //! �C�ӂ̈����ɑ΂���0�����x�N�g�����o�͂���D
    template <class AT>
    Vector<ET>	operator ()(const AT&)	const	{return Vector<ET>(0);}
  //! �C�ӂ̈����ɑ΂���0x0�s����o�͂���D
    template <class AT>
    Matrix<ET>	jacobian(const AT&)	const	{return Matrix<ET>(0, 0);}
};

/************************************************************************
*  class ConstNormConstraint						*
************************************************************************/
//! ������2��m�����l�����Ƃ����S��������\���N���X
/*!
  ���̂͗^����ꂽ������2��m�����l�ƖڕW�l�Ƃ̍���1�����x�N�g���Ƃ��ĕԂ�
  �x�N�g���l�֐��ł���C #minimizeSquare() �� #minimizeSquareSparse() ��
  �e���v���[�g�p�����[�^G�Ƃ��ė��p���邱�Ƃ�z�肵�Ă���D
  \param AT	�����̌^�D�ȉ��̏����𖞂������ƁF
  -# �x�N�g����s��ł���ꍇ�C���̗v�f�̌^��
	AT::value_type
     �Ƃ������O��typedef���Ă���D
  -# �����o�֐�
	AT::value_type	AT::square() const
     �ɂ���āC����2��m�����l��m�邱�Ƃ��ł���D
  -# Vector<AT::value_type>
     �^�ɕϊ��ł���(��F
     Matrix<AT::value_type>
     �^�͂��̗v�f���s�D�揇��1��ɕ��ׂ��x�N�g���ɕϊ��\)�D
*/
template <class AT>
class ConstNormConstraint
{
  private:
    typedef typename AT::value_type	ET;
    
  public:
  //! �V���ȍS�������𐶐����C����2��m�����̖ڕW�l��ݒ肷��D
  /*!
    \param x	����(����2��m�����l���ڕW�l�ƂȂ�)
  */
    ConstNormConstraint(const AT& x) :_sqr(x.square())			{}

  //! �^����ꂽ������2��m�����l�ƖڕW�l�̍����o�͂���D
  /*!
    \param x	����
    \return	x��2��m�����l�ƖڕW�l�̍������߂�1�����x�N�g��
  */
    Vector<ET>	operator ()(const AT& x) const
		{
		    Vector<ET>	val(1);
		    val[0] = x.square() - _sqr;
		    return val;
		}

  //! �^����ꂽ������2��m�����l�ɂ��āC���̈������g�ɂ��1�K�����l���o�͂���D
  /*!
    \param x	����
    \return	1�K�����l�����߂�1xd�s��(d�̓x�N�g�������ꂽ�����̎���)
  */
    Matrix<ET>	jacobian(const AT& x) const
		{
		    const Vector<ET>	y(x);
		    Matrix<ET>		L(1, y.dim());
		    (L[0] = y) *= 2.0;
		    return L;
		}
	    
  private:
    const ET	_sqr;
};

/************************************************************************
*  function minimizeSquare						*
*    -- Compute x st. ||f(x)||^2 -> min under g(x) = 0.			*
************************************************************************/
//! �^����ꂽ�x�N�g���l�֐���2��m������^����ꂽ�S�������̉��ōŏ���������������߂�D
/*!
  �{�֐��́C2�̃x�N�g���֐�\f$\TUvec{f}{}(\TUvec{x}{})\f$,
  \f$\TUvec{g}{}(\TUvec{x}{})\f$����я����l\f$\TUvec{x}{0}\f$���^����ꂽ�Ƃ��C
  \f$\TUvec{g}{}(\TUvec{x}{}) = \TUvec{0}{}\f$�Ȃ�S���̂��Ƃ�
  \f$\TUnorm{\TUvec{f}{}(\TUvec{x}{})}^2 \rightarrow \min\f$�Ƃ���
  \f$\TUvec{x}{}\f$�����߂�D
  
  �e���v���[�g�p�����[�^AT�́C�x�N�g���l�֐�����эS�������֐��̈�����\���^�ł���C
  �ȉ��̏����𖞂������ƁF
  -# �������x�N�g����s��ł���ꍇ�C���̗v�f�̌^��
	AT::value_type
     �Ƃ������O��typedef���Ă���D

  �e���v���[�g�p�����[�^F�́CAT�^�̈�������͂��ăx�N�g���l���o�͂���֐���\���^�ł���C
  �ȉ��̏����𖞂������ƁF
  -# �o�̓x�N�g���̗v�f�̌^��
	F::value_type
     �Ƃ������O��typedef���Ă���D
  -# ����x��^�����Ƃ��̊֐��l�́C�����o�֐�
	Vector<F:value_type>	F::operator ()(const AT& x) const
     �ɂ���ė^������D
  -# ����x��^�����Ƃ��̃��R�r�A���́C�����o�֐�
	Matrix<F:value_type>	F::jacobian(const AT& x) const
     �ɂ���ė^������D
  -# �����o�֐�
	void	F::update(const AT& x, const Vector<F::value_type>& dx) const
     �ɂ���Ĉ���x�������dx�����X�V���邱�Ƃ��ł���D

  �e���v���[�g�p�����[�^G�́CAT�^�̈�������͂��ăx�N�g���l���o�͂���֐���\���^�ł���C
  �ȉ��̏����𖞂������ƁF
  -# �o�̓x�N�g���̗v�f�̌^��
	G::value_type
     �Ƃ������O��typedef���Ă���D
  -# ����x��^�����Ƃ��̊֐��l�́C�����o�֐�
	Vector<G:value_type>	G::operator ()(const AT& x) const
     �ɂ���ė^������D
  -# ����x��^�����Ƃ��̃��R�r�A���́C�����o�֐�
	Matrix<G::value_type>	G::jacobian(const AT& x) const
     �ɂ���ė^������D

  \param f		����2��m�������ŏ������ׂ��x�N�g���l�֐�
  \param g		�S��������\���x�N�g���l�֐�
  \param x		�����l��^����ƁCg����x�N�g���ƂȂ�Ƃ����S�������̉���
			f��2��m�������ŏ�����������̒l���Ԃ����D
  \param niter_max	�ő�J��Ԃ���
  \param tol		�������������\��臒l(�X�V�ʂ����̒l�ȉ��ɂȂ�Ύ����ƌ��Ȃ�)
  \return		x�̐���l�̋����U�s��
*/
template <class F, class G, class AT> Matrix<typename F::value_type>
minimizeSquare(const F& f, const G& g, AT& x,
	       u_int niter_max=100, double tol=1.5e-8)
{
    using namespace			std;
    typedef typename F::value_type	ET;	// element type.

    Vector<ET>	fval   = f(x);			// function value.
    ET		sqr    = fval * fval;		// square value.
    ET		lambda = 1.0e-4;		// L-M parameter.

    for (u_int n = 0; n++ < niter_max; )
    {
	const Matrix<ET>&	J    = f.jacobian(x);	// Jacobian.
	const Vector<ET>&	Jtf  = fval * J;
	const Vector<ET>&	gval = g(x);		// constraint residual.
	const u_int		xdim = J.ncol(), gdim = gval.dim();
	Matrix<ET>		A(xdim + gdim, xdim + gdim);

	A(0, 0, xdim, xdim) = J.trns() * J;
	A(xdim, 0, gdim, xdim) = g.jacobian(x);
	A(0, xdim, xdim, gdim) = A(xdim, 0, gdim, xdim).trns();

	Vector<ET>		diagA(xdim);
	for (u_int i = 0; i < xdim; ++i)
	    diagA[i] = A[i][i];			// Keep diagonal elements.

	for (;;)
	{
	  // Compute dx: update for parameters x to be estimated.
	    for (u_int i = 0; i < xdim; ++i)
		A[i][i] = (1.0 + lambda) * diagA[i];	// Augument diagonals.
	    Vector<ET>	dx(xdim + gdim);
	    dx(0, xdim) = Jtf;
	    dx(xdim, gdim) = gval;
	    dx.solve(A);

	  // Compute updated parameters and function value to it.
	    AT			x_new(x);
	    f.update(x_new, dx(0, xdim));
	    const Vector<ET>&	fval_new = f(x_new);
	    const ET		sqr_new  = fval_new * fval_new;
#ifdef TUMinimizePP_DEBUG
	    cerr << "val^2 = " << sqr << ", gval = " << gval
		 << "  (update: val^2 = " << sqr_new
		 << ", lambda = " << lambda << ")" << endl;
#endif
	    if (fabs(sqr_new - sqr) <=
		tol * (fabs(sqr_new) + fabs(sqr) + 1.0e-10))
	    {
		for (u_int i = 0; i < xdim; ++i)
		    A[i][i] = diagA[i];
		return A(0, 0, xdim, xdim).pinv(1.0e8);
	    }

	    if (sqr_new < sqr)
	    {
		x	= x_new;		// Update parameters.
		fval	= fval_new;		// Update function value.
		sqr	= sqr_new;		// Update square value.
		lambda *= 0.1;			// Decrease L-M parameter.
		break;
	    }
	    else
		lambda *= 10.0;			// Increase L-M parameter.
	}
    }
    throw std::runtime_error("minimizeSquare: maximum iteration limit exceeded!");
    return Matrix<ET>(0, 0);
}

/************************************************************************
*  function minimizeSquareSparse					*
*    -- Compute a and b st. sum||f(a, b[j])||^2 -> min under g(a) = 0.	*
************************************************************************/
//! �^����ꂽ�x�N�g���l�֐���2��m������^����ꂽ�S�������̉��ōŏ���������������߂�D
/*!
  �{�֐��́C\f$\TUvec{x}{} = [\TUtvec{a}{}, \TUtvec{b}{1},
  \TUtvec{b}{2}, \ldots, \TUtvec{b}{J}]^\top\f$����͂Ƃ���2�̃x�N�g
  ���֐�\f$\TUvec{f}{}(\TUvec{x}{}) = [\TUtvec{f}{1}(\TUvec{a}{},
  \TUvec{b}{1}), \TUtvec{f}{2}(\TUvec{a}{}, \TUvec{b}{2}),\ldots,
  \TUtvec{f}{J}(\TUvec{a}{}, \TUvec{b}{J})]^\top\f$,
  \f$\TUvec{g}{}(\TUvec{x}{})\f$����я����l\f$\TUvec{x}{0}\f$���^����
  �ꂽ�Ƃ��C\f$\TUvec{g}{}(\TUvec{x}{}) = \TUvec{0}{}\f$�Ȃ�S���̂���
  ��\f$\TUnorm{\TUvec{f}{}(\TUvec{x}{})}^2 \rightarrow \min\f$�Ƃ���
  \f$\TUvec{x}{}\f$�����߂�D�X��\f$\TUvec{f}{j}(\cdot)\f$��
  \f$\TUvec{a}{}\f$��\f$\TUvec{b}{j}\f$�݂̂Ɉˑ����C
  \f$\TUvec{g}{}(\cdot)\f$��\f$\TUvec{a}{}\f$�݂̂Ɉˑ�����(���Ȃ킿
  \f$\TUvec{g}{}(\TUvec{x}{}) = \TUvec{g}{}(\TUvec{a}{})\f$)���̂Ƃ���D
  
  �e���v���[�g�p�����[�^ATA�́C�x�N�g���l�֐�f�̑�1��������эS�������֐�g��
  ����a��\���^�ł���C�ȉ��̏����𖞂������ƁF
  -# �������x�N�g����s��ł���ꍇ�C���̗v�f�̌^��
	ATA::value_type
     �Ƃ������O��typedef���Ă���D

  �e���v���[�g�p�����[�^IB�́C�X�̃x�N�g���l�֐�f_j�̑�2����b_j���w��
  �����q��\���^�ł���C�ȉ��̏����𖞂������ƁF
  -# iterator_traits<IB>::value_type
     �ł��̔����q���w�������̌^(�ȉ��CATB�Ƃ���)��m�邱�Ƃ��ł���D

  �e���v���[�g�p�����[�^F�́CATA�^�̈���a��ATB�^�̈���b_j����͂���
  �x�N�g���l���o�͂���֐���\���^�ł���C�ȉ��̏����𖞂������ƁF
  -# �o�̓x�N�g���̗v�f�̌^��
	F::value_type
     �Ƃ������O��typedef���Ă���D
  -# ���R�r�A���̌^��
	F::jacobian_type
     �Ƃ������O��typedef���Ă���D
  -# ATA�^�̈���a�������R�x��
	u_int	F::adim() const
     �ɂ���Ēm�邱�Ƃ��ł���D
  -# ����a��a_1, a_2,..., a_I�ɕ��������ꍇ�̊ea_i�������R�x��
	const Array<u_int>&	F::adims() const;
     �ɂ���Ēm�邱�Ƃ��ł���D���̔z��̗v�f�̑��a��
	F::adim()
     �ɓ������Da�������ł��Ȃ��ꍇ����1�̔z�񂪕Ԃ���C���̗B��̗v�f�̒l��
	F::adim()
     �ɓ������D
  -# ����a, b_j��^�����Ƃ���f_j�̊֐��l�́C�����o�֐�
	Vector<F:value_type>	F::operator ()(const ATA& a, const ATB& b, int j) const
     �ɂ���ė^������D
  -# ����a, b_j��^�����Ƃ���a�Ŕ����������R�r�A���́C�����o�֐�
	F::jacobian_type	F::jacobianA(const ATA& a, const ATB& b, int j) const
     �ɂ���ė^������D
  -# �����o�֐�
	void	F::updateA(const ATA& a, const Vector<F::value_type>& da) const
     �ɂ���Ĉ���a�������da�����X�V���邱�Ƃ��ł���D
  -# �����o�֐�
	void	F::updateB(const ATB& b_j, const Vector<F::value_type>& db_j) const
     �ɂ���Ĉ���b�������db_j�����X�V���邱�Ƃ��ł���D

  �e���v���[�g�p�����[�^G�́CATA�^�̈�������͂��ăx�N�g���l���o�͂���֐���
  �\���^�ł���C�ȉ��̏����𖞂������ƁF
  -# �o�̓x�N�g���̗v�f�̌^��
	G::value_type
     �Ƃ������O��typedef���Ă���D
  -# ����a��^�����Ƃ��̊֐��l�́C�����o�֐�
	Vector<G:value_type>	G::operator ()(const ATA& a) const
     �ɂ���ė^������D
  -# ����a��^�����Ƃ��̃��R�r�A���́C�����o�֐�
	Matrix<G::value_type>	G::jacobian(const ATA& a) const
     �ɂ���ė^������D

  \param f		����2��m�������ŏ������ׂ��x�N�g���l�֐�
  \param g		�S��������\���x�N�g���l�֐�
  \param a		�ef_j�̑�1�����ł���C����g�̈����D�����l��^�����
			�œK�����Ԃ����D
  \param bbegin		�ef_j�ɗ^�����2�����̕��т̐擪���w�������q
  \param bend		�ef_j�ɗ^�����2�����̕��т̖����̎����w�������q
  \param niter_max	�ő�J��Ԃ���
  \param tol		�������������\��臒l(�X�V�ʂ����̒l�ȉ��ɂȂ��
			�����ƌ��Ȃ�)
  \return		a, b_1, b_2,..., b_J�̐���l�̋����U�s��
*/
template <class F, class G, class ATA, class IB> Matrix<typename F::value_type>
minimizeSquareSparse(const F& f, const G& g, ATA& a, IB bbegin, IB bend,
		     u_int niter_max=100, double tol=1.5e-8)
{
    using namespace					std;
    typedef typename F::value_type			ET;  // element type.
    typedef typename F::jacobian_type			JT;  // Jacobian type.
    typedef typename iterator_traits<IB>::value_type	ATB; // arg. b type.
    
    const u_int			nb = distance(bbegin, bend);
    Array<Vector<ET> >		fval(nb);	// function values.
    ET				sqr = 0;	// sum of squares.
    int				j = 0;
    for (IB b = bbegin; b != bend; ++b, ++j)
    {
	fval[j] = f(a, *b, j);
	sqr    += fval[j] * fval[j];
    }
    ET	lambda = 1.0e-7;			// L-M parameter.

    for (u_int n = 0; n++ < niter_max; )
    {
	const u_int		adim = f.adim();
	JT			U(f.adims(), f.adims());
	Vector<ET>		Jtf(adim);
	Array<Matrix<ET> >	V(nb);
	Array<Matrix<ET> >	W(nb);
	Array<Vector<ET> >	Ktf(nb);
	j = 0;
	for (IB b = bbegin; b != bend; ++b, ++j)
	{
	    const JT&		J  = f.jacobianA(a, *b, j);
	    const JT&		Jt = J.trns();
	    const Matrix<ET>&	K  = f.jacobianB(a, *b, j);

	    U     += Jt * J;
	    Jtf   += fval[j] * J;
	    V[j]   = K.trns() * K;
	    W[j]   = Jt * K;
	    Ktf[j] = fval[j] * K;
	}

      	const Vector<ET>&	gval = g(a);
	const u_int		gdim = gval.dim();
	Matrix<ET>		A(adim + gdim, adim + gdim);
	
	A(adim, 0, gdim, adim) = g.jacobian(a);
	A(0, adim, adim, gdim) = A(adim, 0, gdim, adim).trns();

	for (;;)
	{
	  // Compute da: update for parameters a to be estimated.
	    A(0, 0, adim, adim) = U;
	    for (u_int i = 0; i < adim; ++i)
		A[i][i] *= (1.0 + lambda);		// Augument diagonals.

	    Vector<ET>		da(adim + gdim);
	    da(0, adim) = Jtf;
	    da(adim, gdim) = gval;
	    Array<Matrix<ET> >	VinvWt(nb);
	    Array<Vector<ET> >	VinvKtf(nb);
	    for (u_int j = 0; j < nb; ++j)
	    {
		Matrix<ET>	Vinv = V[j];
		for (u_int k = 0; k < Vinv.dim(); ++k)
		    Vinv[k][k] *= (1.0 + lambda);	// Augument diagonals.
		Vinv = Vinv.inv();
		VinvWt[j]  = Vinv * W[j].trns();
		VinvKtf[j] = Vinv * Ktf[j];
		A(0, 0, adim, adim) -= W[j] * VinvWt[j];
		da(0, adim) -= W[j] * VinvKtf[j];
	    }
	    da.solve(A);

	  // Compute updated parameters and function value to it.
	    ATA			a_new(a);
	    f.updateA(a_new, da(0, adim));
	    Array<ATB>		b_new(nb);
	    copy(bbegin, bend, b_new.begin());
	    Array<Vector<ET> >	fval_new(nb);
	    ET			sqr_new = 0;
	    for (u_int j = 0; j < nb; ++j)
	    {
		const Vector<ET>& db = VinvKtf[j] - VinvWt[j] * da(0, adim);
		f.updateB(b_new[j], db);
		fval_new[j] = f(a_new, b_new[j], j);
		sqr_new	   += fval_new[j] * fval_new[j];
	    }
#ifdef TUMinimizePP_DEBUG
	    cerr << "val^2 = " << sqr << ", gval = " << gval
		 << "  (update: val^2 = " << sqr_new
		 << ", lambda = " << lambda << ")" << endl;
#endif
	    if (fabs(sqr_new - sqr) <=
		tol * (fabs(sqr_new) + fabs(sqr) + 1.0e-10))
	    {
		u_int		bdim = 0;
		for (u_int j = 0; j < nb; ++j)
		    bdim += V[j].dim();
		Matrix<ET>	S(adim + bdim, adim + bdim);
		Matrix<ET>	Sa(S, 0, 0, adim, adim);
		Sa = U;
		for (u_int j = 0; j < nb; ++j)
		{
		    VinvWt[j] = V[j].inv() * W[j].trns();
		    Sa -= W[j] * VinvWt[j];
		}
		for (u_int jj = adim, j = 0; j < nb; ++j)
		{
		    const Matrix<ET>&	VinvWtSa = VinvWt[j] * Sa;
		    for (u_int kk = adim, k = 0; k <= j; ++k)
		    {
			S(jj, kk, VinvWtSa.nrow(), VinvWt[k].nrow())
			     = VinvWtSa * VinvWt[k].trns();
			kk += VinvWt[k].nrow();
		    }
		    S(jj, jj, V[j].nrow(), V[j].nrow()) += V[j].inv();
		    jj += VinvWt[j].nrow();
		}
		Sa = Sa.pinv(1.0e8);
		for (u_int jj = adim, j = 0; j < nb; ++j)
		{
		    S(jj, 0, VinvWt[j].nrow(), adim) = -VinvWt[j] * Sa;
		    jj += VinvWt[j].nrow();
		}
		    
		return S.symmetrize() *= sqr;
	    }
	    
	    if (sqr_new < sqr)
	    {
		a = a_new;			// Update parameters.
		copy(b_new.begin(), b_new.end(), bbegin);
		fval = fval_new;		// Update function values.
		sqr = sqr_new;			// Update residual.
		lambda *= 0.1;			// Decrease L-M parameter.
		break;
	    }
	    else
		lambda *= 10.0;			// Increase L-M parameter.
	}
    }
    throw std::runtime_error("minimizeSquareSparse: maximum iteration limit exceeded!");

    return Matrix<ET>(0, 0);
}

/************************************************************************
*  function minimizeSquareSparseDebug					*
*    -- Compute a and b st. sum||f(a, b[j])||^2 -> min under g(a) = 0.	*
************************************************************************/
template <class F, class G, class ATA, class IB>  Matrix<typename F::ET>
minimizeSquareSparseDebug(const F& f, const G& g, ATA& a, IB bbegin, IB bend,
			  u_int niter_max=100, double tol=1.5e-8)
{
    using namespace					std;
    typedef typename F::value_type			ET;  // element type.
    typedef typename iterator_traits<IB>::value_type	ATB; // arg. b type.

    const u_int			nb = distance(bbegin, bend);
    Array<Vector<ET> >		fval(nb);	// function values.
    ET				sqr = 0;	// sum of squares.
    int				j = 0;
    for (IB b = bbegin; b != bend; ++b, ++j)
    {
	fval[j] = f(a, *b, j);
	sqr    += fval[j] * fval[j];
    }
    ET	lambda = 1.0e-7;			// L-M parameter.

    for (u_int n = 0; n++ < niter_max; )
    {
	const u_int		adim = f.adim();
	const u_int		bdim = f.bdim() * nb;
      	const Vector<ET>&	gval = g(a);
	const u_int		gdim = gval.dim();
	Matrix<ET>		U(adim, adim);
	Vector<ET>		Jtf(adim);
	Array<Matrix<ET> >	V(nb);
	Array<Matrix<ET> >	W(nb);
	Array<Vector<ET> >	Ktf(nb);
	Matrix<ET>		A(adim + bdim + gdim, adim + bdim + gdim);
	j = 0;
	for (IB b = bbegin; b != bend; ++b, ++j)
	{
	    const Matrix<ET>&	J  = f.jacobianA(a, *b, j);
	    const Matrix<ET>&	Jt = J.trns();
	    const Matrix<ET>&	K  = f.jacobianB(a, *b, j);

	    U     += Jt * J;
	    Jtf   += fval[j] * J;
	    V[j]   = K.trns() * K;
	    W[j]   = Jt * K;
	    Ktf[j] = fval[j] * K;

	    A(0, adim + j*f.bdim(), adim, f.bdim()) = W[j];
	    A(adim + j*f.bdim(), 0, f.bdim(), adim) = W[j].trns();
	}
	A(adim + bdim, 0, gdim, adim) = g.jacobian(a);
	A(0, adim + bdim, adim, gdim) = A(adim + bdim, 0, gdim, adim).trns();

	for (;;)
	{
	  // Compute da: update for parameters a to be estimated.
	    A(0, 0, adim, adim) = U;
	    for (u_int i = 0; i < adim; ++i)
		A[i][i] *= (1.0 + lambda);
	    for (u_int j = 0; j < nb; ++j)
	    {
		A(adim + j*f.bdim(), adim + j*f.bdim(), f.bdim(), f.bdim())
		    = V[j];
		for (u_int k = 0; k < f.bdim(); ++k)
		    A[adim + j*f.bdim() + k][adim + j*f.bdim() + k]
			*= (1.0 + lambda);
	    }

	    Vector<ET>	dx(adim + bdim + gdim);
	    dx(0, adim) = Jtf;
	    for (u_int j = 0; j < nb; ++j)
		dx(adim + j*f.bdim(), f.bdim()) = Ktf[j];
	    dx(adim + bdim, gdim) = gval;
	    dx.solve(A);
	    
	  // Compute updated parameters and function value to it.
	    ATA			a_new(a);
	    f.updateA(a_new, dx(0, adim));
	    Array<ATB>		b_new(nb);
	    copy(bbegin, bend, b_new.begin());
	    Array<Vector<ET> >	fval_new(nb);
	    ET			sqr_new = 0;
	    for (u_int j = 0; j < nb; ++j)
	    {
		const Vector<ET>& db = dx(adim + j*f.bdim(), f.bdim());
	      //		cerr << "*** check:  "
	      // << (dx(0, adim) * W[j] + V[j] * db - Ktf[j]);
		f.updateB(b_new[j], db);
		fval_new[j] = f(a_new, b_new[j], j);
		sqr_new	   += fval_new[j] * fval_new[j];
	    }
#ifdef TUMinimizePP_DEBUG
	    cerr << "val^2 = " << sqr << ", gval = " << gval
		 << "  (update: val^2 = " << sqr_new
		 << ", lambda = " << lambda << ")" << endl;
#endif
	    if (fabs(sqr_new - sqr) <=
		tol * (fabs(sqr_new) + fabs(sqr) + 1.0e-10))
	    {
		A(0, 0, adim, adim) = U;
		for (u_int j = 0; j < nb; ++j)
		    A(adim + j*f.bdim(), adim + j*f.bdim(), f.bdim(), f.bdim())
			= V[j];
		Vector<ET>	evalue;
		A(0, 0, adim + bdim, adim + bdim).eigen(evalue);
		cerr << evalue;
		return A(0, 0, adim + bdim, adim + bdim).pinv(1.0e8) *= sqr;
	    }

	    if (sqr_new < sqr)
	    {
		a = a_new;			// Update parameters.
		copy(b_new.begin(), b_new.end(), bbegin);
		fval = fval_new;		// Update function values.
		sqr = sqr_new;			// Update residual.
		lambda *= 0.1;			// Decrease L-M parameter.
		break;
	    }
	    else
		lambda *= 10.0;			// Increase L-M parameter.
	}
    }
    throw std::runtime_error("minimizeSquareSparseDebug: maximum iteration limit exceeded!");

    return Matrix<ET>(0, 0);
}

}
#endif	/* !__TUMinimize_h	*/
