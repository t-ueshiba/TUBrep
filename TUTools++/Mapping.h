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
 *  $Id: Mapping.h,v 1.7 2009-09-04 04:01:06 ueshiba Exp $
 */
#ifndef __TUMapping_h
#define __TUMapping_h

#include "TU/utility.h"
#include "TU/Normalize.h"
#include "TU/Minimize.h"

namespace TU
{
/************************************************************************
*  class ProjectiveMapping						*
************************************************************************/
//! �ˉe�ϊ����s���N���X
/*!
  \f$\TUvec{T}{} \in \TUspace{R}{(n+1)\times(m+1)}\f$��p����m������Ԃ̓_
  \f$\TUud{x}{} \in \TUspace{R}{m+1}\f$��n������Ԃ̓_
  \f$\TUud{y}{} \simeq \TUvec{T}{}\TUud{x}{} \in \TUspace{R}{n+1}\f$
  �Ɏʂ��i\f$m \neq n\f$�ł��\��Ȃ��j�D
*/
class __PORT ProjectiveMapping
{
  public:
    typedef double	value_type;

  public:
    ProjectiveMapping(u_int inDim=2, u_int outDim=2)			;

  //! �ϊ��s����w�肵�Ďˉe�ϊ��I�u�W�F�N�g�𐶐�����D
  /*!
    \param T			(m+1)x(n+1)�s��im, n�͓��́^�o�͋�Ԃ̎����j
  */
    ProjectiveMapping(const Matrix<double>& T)	:_T(T)			{}

    template <class Iterator>
    ProjectiveMapping(Iterator first, Iterator last, bool refine=false)	;

  //! �ϊ��s����w�肷��D
  /*!
    \param T			(m+1)x(n+1)�s��im, n�͓��́^�o�͋�Ԃ̎����j
  */
    void		set(const Matrix<double>& T)	{_T = T;}
    
    template <class Iterator>
    void		fit(Iterator first, Iterator last,
			    bool refine=false)				;

  //! ���̎ˉe�ϊ��̓��͋�Ԃ̎�����Ԃ��D
  /*! 
    \return	���͋�Ԃ̎���(�������W�̃x�N�g���Ƃ��Ă̎�����#inDim()+1)
  */
    u_int		inDim()			const	{return _T.ncol()-1;}

  //! ���̎ˉe�ϊ��̏o�͋�Ԃ̎�����Ԃ��D
  /*! 
    \return	�o�͋�Ԃ̎���(�������W�̃x�N�g���Ƃ��Ă̎�����#outDim()+1)
  */
    u_int		outDim()		const	{return _T.nrow()-1;}

    u_int		ndataMin()		const	;
    
  //! ���̎ˉe�ϊ���\������s���Ԃ��D
  /*! 
    \return	(#outDim()+1)x(#inDim()+1)�s��
  */
    const Matrix<double>&	T()		const	{return _T;}

    template <class S, class B>
    Vector<double>	operator ()(const Vector<S, B>& x)	const	;
    template <class S, class B>
    Vector<double>	mapP(const Vector<S, B>& x)		const	;
    template <class S, class B>
    Matrix<double>	jacobian(const Vector<S, B>& x)		const	;

    template <class In, class Out>
    double		sqdist(const std::pair<In, Out>& pair)	const	;
    template <class In, class Out>
    double		dist(const std::pair<In, Out>& pair)	const	;
    double		square()				const	;
			operator const Vector<double>()		const	;
    u_int		dof()					const	;
    void		update(const Vector<double>& dt)		;
    
  protected:
    Matrix<double>	_T;			//!< �ˉe�ϊ���\������s��

  protected:
  //! �ˉe�ϊ��s��̍Ŗސ���̂��߂̃R�X�g�֐�
    template <class AT, class Iterator>
    class Cost
    {
      public:
	typedef double	value_type;

	Cost(Iterator first, Iterator last)				;

	Vector<double>	operator ()(const AT& map)		const	;
	Matrix<double>	jacobian(const AT& map)			const	;
	static void	update(AT& map, const Vector<double>& dm)	;
	u_int		npoints()				const	;

      private:
	const Iterator	_first, _last;
	const u_int	_npoints;
    };
};

//! �^����ꂽ�_�Η�̔񓯎����W����ˉe�ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  \param first			�_�Η�̐擪�����������q
  \param last			�_�Η�̖��������������q
  \param refine			����^�œK���̗L(true)�^��(false)���w��
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class Iterator> inline
ProjectiveMapping::ProjectiveMapping(Iterator first, Iterator last,
				     bool refine)
{
    fit(first, last, refine);
}

//! �^����ꂽ�_�Η�̔񓯎����W����ˉe�ϊ����v�Z����D
/*!
  \param first			�_�Η�̐擪�����������q
  \param last			�_�Η�̖��������������q
  \param refine			����^�œK���̗L(true)�^��(false)���w��
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class Iterator> void
ProjectiveMapping::fit(Iterator first, Iterator last, bool refine)
{
  // �_��̐��K��
    const Normalize	xNormalize(make_const_first_iterator(first),
				   make_const_first_iterator(last)),
			yNormalize(make_const_second_iterator(first),
				   make_const_second_iterator(last));

  // �[���Ȍ��̓_�΂����邩�H
    const u_int		ndata = std::distance(first, last);
    const u_int		xdim1 = xNormalize.spaceDim() + 1,
			ydim  = yNormalize.spaceDim();
    if (ndata*ydim < xdim1*(ydim + 1) - 1)	// _T�̃T�C�Y������Ȃ̂�
						// ndataMin()�͖���
	throw std::invalid_argument("ProjectiveMapping::fit(): not enough input data!!");

  // �f�[�^�s��̌v�Z
    Matrix<double>	A(xdim1*(ydim + 1), xdim1*(ydim + 1));
    for (Iterator iter = first; iter != last; ++iter)
    {
	const Vector<double>&	x  = xNormalize.normalizeP(iter->first);
	const Vector<double>&	y  = yNormalize(iter->second);
	const Matrix<double>&	xx = x % x;
	A(0, 0, xdim1, xdim1) += xx;
	for (int j = 0; j < ydim; ++j)
	    A(ydim*xdim1, j*xdim1, xdim1, xdim1) -= y[j] * xx;
	A(ydim*xdim1, ydim*xdim1, xdim1, xdim1) += (y*y) * xx;
    }
    for (int j = 1; j < ydim; ++j)
	A(j*xdim1, j*xdim1, xdim1, xdim1) = A(0, 0, xdim1, xdim1);
    A.symmetrize();

  // �f�[�^�s��̍ŏ��ŗL�l�ɑΉ�����ŗL�x�N�g������ϊ��s����v�Z���C
  // ���K�����L�����Z������D
    Vector<double>	eval;
    Matrix<double>	Ut = A.eigen(eval);
    _T = yNormalize.Tinv()
       * Matrix<double>((double*)Ut[Ut.nrow()-1], ydim + 1, xdim1)
       * xNormalize.T();

  // �ϊ��s�񂪐����Ȃ�΁C���̍s�񎮂��P�ɂȂ�悤�ɐ��K������D
    if (_T.nrow() == _T.ncol())
    {
	double	det = _T.det();
	if (det > 0)
	    _T /= pow(det, 1.0/_T.nrow());
	else
	    _T /= -pow(-det, 1.0/_T.nrow());
    }

  // ����^�œK�����s���D
    if (refine)
    {
	Cost<ProjectiveMapping, Iterator>	cost(first, last);
	ConstNormConstraint<ProjectiveMapping>	constraint(*this);
	minimizeSquare(cost, constraint, *this);
    }
}

//! �ˉe�ϊ������߂邽�߂ɕK�v�ȓ_�΂̍ŏ�����Ԃ��D
/*!
  ���ݐݒ肳��Ă�����o�͋�Ԃ̎��������ƂɌv�Z�����D
  \return	�K�v�ȓ_�΂̍ŏ������Ȃ킿���͋�Ԃ̎���m�C�o�͋�Ԃ̎���n
		�ɑ΂��� m + 1 + m/n
*/
inline u_int
ProjectiveMapping::ndataMin() const
{
    return inDim() + 1 + u_int(ceil(double(inDim())/double(outDim())));
}
    
//! �^����ꂽ�_�Ɏˉe�ϊ���K�p���Ă��̔񓯎����W��Ԃ��D
/*!
  \param x	�_�̔񓯎����W�i#inDim()�����j�܂��͓������W�i#inDim()+1�����j
  \return	�ˉe�ϊ����ꂽ�_�̔񓯎����W�i#outDim()�����j
*/
template <class S, class B> inline Vector<double>
ProjectiveMapping::operator ()(const Vector<S, B>& x) const
{
    return mapP(x).inhomogenize();
}

//! �^����ꂽ�_�Ɏˉe�ϊ���K�p���Ă��̓������W��Ԃ��D
/*!
  \param x	�_�̔񓯎����W�i#inDim()�����j�܂��͓������W�i#inDim()+1�����j
  \return	�ˉe�ϊ����ꂽ�_�̓������W�i#outDim()+1�����j
*/
template <class S, class B> inline Vector<double>
ProjectiveMapping::mapP(const Vector<S, B>& x) const
{
    if (x.dim() == inDim())
	return _T * x.homogenize();
    else
	return _T * x;
}

//! �^����ꂽ�_�ɂ����郄�R�r�s���Ԃ��D
/*!
  ���R�r�s��Ƃ͎ˉe�ϊ��s�񐬕��Ɋւ���1�K�����̂��Ƃł���D
  \param x	�_�̔񓯎����W�i#inDim()�����j�܂��͓������W�i#inDim()+1�����j
  \return	#outDim() x (#outDim()+1)x(#inDim()+1)���R�r�s��
*/
template <class S, class B> Matrix<double>
ProjectiveMapping::jacobian(const Vector<S, B>& x) const
{
    Vector<double>		xP;
    if (x.dim() == inDim())
	xP = x.homogenize();
    else
	xP = x;
    const Vector<double>&	y  = mapP(xP);
    Matrix<double>		J(outDim(), (outDim() + 1)*xP.dim());
    for (int i = 0; i < J.nrow(); ++i)
    {
	J[i](i*xP.dim(), xP.dim()) = xP;
	(J[i](outDim()*xP.dim(), xP.dim()) = xP) *= (-y[i]/y[outDim()]);
    }
    J /= y[outDim()];

    return J;
}
    
//! ���͓_�Ɏˉe�ϊ���K�p�����_�Əo�͓_�̋�����2���Ԃ��D
/*!
  \param pair	���͓_�̔񓯎����W�i#inDim()�����j�Əo�͓_�̔񓯎����W
		�i#outDim()�����j�̑�
  \return	�ϊ����ꂽ���͓_�Əo�͓_�̋�����2��
*/
template <class In, class Out> inline double
ProjectiveMapping::sqdist(const std::pair<In, Out>& pair) const
{
    return (*this)(pair.first).sqdist(pair.second);
}
    
//! ���͓_�Ɏˉe�ϊ���K�p�����_�Əo�͓_�̋�����Ԃ��D
/*!
  \param pair	���͓_�̔񓯎����W�i#inDim()�����j�Əo�͓_�̔񓯎����W
		�i#outDim()�����j�̑�
  \return	�ϊ����ꂽ���͓_�Əo�͓_�̋���
*/
template <class In, class Out> inline double
ProjectiveMapping::dist(const std::pair<In, Out>& pair) const
{
    return sqrt(sqdist(pair));
}

//! �ˉe�ϊ��s��̃m������2���Ԃ��D
/*!
  \return	�ˉe�ϊ��s��̃m������2��
*/
inline double
ProjectiveMapping::square() const
{
    return _T.square();
}

//! �ˉe�ϊ��s��̊e�s�����Ԃɕ��ׂ��x�N�g����Ԃ��D
/*!
  \return	#T()�̐�������ׂ��x�N�g���i(#outDim()+1)x(#inDim()+1)�����j
*/
inline
ProjectiveMapping::operator const Vector<double>() const
{
    return Vector<double>(const_cast<Matrix<double>&>(_T));
}

//! ���̎ˉe�ϊ��̎��R�x��Ԃ��D
/*!
  \return	�ˉe�ϊ��̎��R�x�i(#outDim()+1)x(#inDim()+1)-1�j
*/
inline u_int
ProjectiveMapping::dof() const
{
    return (outDim() + 1)*(inDim() + 1)-1;
}

//! �ˉe�ϊ��s���^����ꂽ�ʂ����C������D
/*!
  \param dt	�C���ʂ�\���x�N�g���i(#outDim()+1)x(#inDim()+1)�����j
*/
inline void
ProjectiveMapping::update(const Vector<double>& dt)
{
    Vector<double>	t(_T);
    double		l = t.length();
    (t -= dt).normalize() *= l;
}
    
template <class AT, class Iterator>
ProjectiveMapping::Cost<AT, Iterator>::Cost(Iterator first, Iterator last)
    :_first(first), _last(last), _npoints(std::distance(_first, _last))
{
}
    
template <class AT, class Iterator> Vector<double>
ProjectiveMapping::Cost<AT, Iterator>::operator ()(const AT& map) const
{
    const u_int		outDim = map.outDim();
    Vector<double>	val(_npoints*outDim);
    int			n = 0;
    for (Iterator iter = _first; iter != _last; ++iter)
    {
	val(n, outDim) = map(iter->first) - iter->second;
	n += outDim;
    }
    
    return val;
}
    
template <class AT, class Iterator> Matrix<double>
ProjectiveMapping::Cost<AT, Iterator>::jacobian(const AT& map) const
{
    const u_int		outDim = map.outDim();
    Matrix<double>	J(_npoints*outDim, map.dof()+1);
    int			n = 0;
    for (Iterator iter = _first; iter != _last; ++iter)
    {
	J(n, 0, outDim, J.ncol()) = map.jacobian(iter->first);
	n += outDim;
    }

    return J;
}

template <class AT, class Iterator> inline void
ProjectiveMapping::Cost<AT, Iterator>::update(AT& map, const Vector<double>& dm)
{
    map.update(dm);
}
    
/************************************************************************
*  class AffineMapping							*
************************************************************************/
//! �A�t�B���ϊ����s���N���X
/*!
  \f$\TUvec{A}{} \in \TUspace{R}{n\times m}\f$��
  \f$\TUvec{b}{} \in \TUspace{R}{n}\f$��p����m������Ԃ̓_
  \f$\TUvec{x}{} \in \TUspace{R}{m}\f$��n������Ԃ̓_
  \f$\TUvec{y}{} \simeq \TUvec{A}{}\TUvec{x}{} + \TUvec{b}{}
  \in \TUspace{R}{n}\f$�Ɏʂ��i\f$m \neq n\f$�ł��\��Ȃ��j�D
*/
class __PORT AffineMapping : public ProjectiveMapping
{
  public:
  //! ���͋�ԂƏo�͋�Ԃ̎������w�肵�ăA�t�B���ϊ��I�u�W�F�N�g�𐶐�����D
  /*!
    �P���ϊ��Ƃ��ď����������D
    \param inDim	���͋�Ԃ̎���
    \param outDim	�o�͋�Ԃ̎���
  */
    AffineMapping(u_int inDim=2, u_int outDim=2)
	:ProjectiveMapping(inDim, outDim)				{}

    AffineMapping(const Matrix<double>& T)				;
    template <class Iterator>
    AffineMapping(Iterator first, Iterator last)			;

    void	set(const Matrix<double>& T)				;
    template <class Iterator>
    void	fit(Iterator first, Iterator last)			;
    u_int	ndataMin()					const	;
    
  //! ���̃A�t�B���ϊ��̕ό`������\������s���Ԃ��D
  /*! 
    \return	#outDim() x #inDim()�s��
  */
    const Matrix<double>
			A()	const	{return _T(0, 0, outDim(), inDim());}
    
    Vector<double>	b()	const	;
};

//! �ϊ��s����w�肵�ăA�t�B���ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  �ϊ��s��̉��[�s�͋����I�� 0,0,...,0,1 �ɐݒ肳���D
  \param T			(m+1)x(n+1)�s��im, n�͓��́^�o�͋�Ԃ̎����j
*/
inline
AffineMapping::AffineMapping(const Matrix<double>& T)
    :ProjectiveMapping(T)
{
    _T[outDim()]	  = 0.0;
    _T[outDim()][inDim()] = 1.0;
}

//! �^����ꂽ�_�Η�̔񓯎����W����A�t�B���ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  \param first			�_�Η�̐擪�����������q
  \param last			�_�Η�̖��������������q
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class Iterator> inline
AffineMapping::AffineMapping(Iterator first, Iterator last)
{
    fit(first, last);
}

//! �ϊ��s����w�肷��D
/*!
  �ϊ��s��̉��[�s�͋����I�� 0,0,...,0,1 �ɐݒ肳���D
  \param T			(m+1)x(n+1)�s��im, n�͓��́^�o�͋�Ԃ̎����j
*/
inline void
AffineMapping::set(const Matrix<double>& T)
{
    ProjectiveMapping::set(T);
    _T[outDim()]	  = 0.0;
    _T[outDim()][inDim()] = 1.0;
}
    
//! �^����ꂽ�_�Η�̔񓯎����W����A�t�B���ϊ����v�Z����D
/*!
  \param first			�_�Η�̐擪�����������q
  \param last			�_�Η�̖��������������q
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class Iterator> void
AffineMapping::fit(Iterator first, Iterator last)
{
  // �[���Ȍ��̓_�΂����邩�H
    const u_int		ndata = std::distance(first, last);
    if (ndata == 0)		// first���L�����H
	throw std::invalid_argument("AffineMapping::fit(): 0-length input data!!");
    const u_int		xdim = first->first.dim();
    if (ndata < xdim + 1)	// _T�̃T�C�Y������Ȃ̂�ndataMin()�͖���
	throw std::invalid_argument("AffineMapping::fit(): not enough input data!!");

  // �f�[�^�s��̌v�Z
    const u_int		ydim = first->second.dim(), xydim2 = xdim*ydim;
    Matrix<double>	M(xdim, xdim);
    Vector<double>	c(xdim), v(xydim2 + ydim);
    for (; first != last; ++first)
    {
	const Vector<double>&	x = first->first;
	const Vector<double>&	y = first->second;

	M += x % x;
	c += x;
	for (int j = 0; j < ydim; ++j)
	    v(j*xdim, xdim) += y[j]*x;
	v(xydim2, ydim) += y;
    }
    Matrix<double>	W(xydim2 + ydim, xydim2 + ydim);
    for (int j = 0; j < ydim; ++j)
    {
	W(j*xdim, j*xdim, xdim, xdim) = M;
	W[xydim2 + j](j*xdim, xdim)   = c;
	W[xydim2 + j][xydim2 + j]     = ndata;
    }
    W.symmetrize();

  // W*u = v�������ĕϊ��p�����[�^�����߂�D
    v.solve(W);

  // �ϊ��s����Z�b�g����D
    _T.resize(ydim + 1, xdim + 1);
    _T(0, 0, ydim, xdim) = Matrix<double>((double*)v, ydim, xdim);
    for (int j = 0; j < ydim; ++j)
	 _T[j][xdim] = v[xydim2 + j];
    _T[ydim][xdim] = 1.0;
}

//! �A�t�B���ϊ������߂邽�߂ɕK�v�ȓ_�΂̍ŏ�����Ԃ��D
/*!
  ���ݐݒ肳��Ă�����o�͋�Ԃ̎��������ƂɌv�Z�����D
  \return	�K�v�ȓ_�΂̍ŏ������Ȃ킿���͋�Ԃ̎���m�ɑ΂��� m + 1
*/
inline u_int
AffineMapping::ndataMin() const
{
    return inDim() + 1;
}
    
}
#endif	/* !__TUMapping_h */
