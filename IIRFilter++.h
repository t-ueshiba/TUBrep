/*
 *  $Id: IIRFilter++.h,v 1.1 2006-10-23 01:22:00 ueshiba Exp $
 */
#ifndef __TUIIRFilterPP_h
#define __TUIIRFilterPP_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class IIRFilter							*
************************************************************************/
/*!
  �Б�Infinite Inpulse Response Filter��\���N���X�D
*/
template <u_int D> class IIRFilter
{
  public:
    IIRFilter&	initialize(const float c[D+D])				;
    template <class S> const IIRFilter&
		forward(const Array<S>& in, Array<float>& out)	const	;
    template <class S> const IIRFilter&
		backward(const Array<S>& in, Array<float>& out)	const	;
    void	limitsF(float& limit0F,
			float& limit1F, float& limit2F)		const	;
    void	limitsB(float& limit0B,
			float& limit1B, float& limit2B)		const	;
    
  private:
    float	_c[D+D];	// coefficients
};

/************************************************************************
*  class BilateralIIRFilter						*
************************************************************************/
/*!
  ����Infinite Inpulse Response Filter��\���N���X�D
*/
template <u_int D> class BilateralIIRFilter
{
  public:
  //! �����̊K��
    enum Order
    {
	Zeroth,						//! 0�K����
	First,						//! 1�K����
	Second						//! 2�K����
    };
    
    BilateralIIRFilter&	initialize(const float cF[D+D], const float cB[D+D]);
    BilateralIIRFilter&	initialize(const float c[D+D], Order order)	;
    template <class S>
    BilateralIIRFilter&	convolve(const Array<S>& in)			;
    u_int		dim()					const	;
    float		operator [](int i)			const	;
    void		limits(float& limit0,
			       float& limit1,
			       float& limit2)			const	;
    
  private:
    IIRFilter<D>	_iirF;
    Array<float>	_bufF;
    IIRFilter<D>	_iirB;
    Array<float>	_bufB;
};

//! �t�B���^��z�ϊ��W�����Z�b�g����
/*!
  \param cF	�O�iz�ϊ��W��. z�ϊ��� 
		  Hf(w) =
		      (cF[D-1] + cF[D-2]*w + cF[D-3]*w^2 + ... + cF[0]*w^(D-1))
		    / (1 - cF[D+D-1]*w - cF[D+D-2]*w^2... - cF[D]*w^D)
		�ƂȂ�. ������ w = z^(-1).
  \param cB	���z�ϊ��W��. z�ϊ���
		  Hb(z) = (cB[0]*z + cB[1]*z^2 + ... + cB[D-1]*z^D)
		        / (1 - cB[D]*z - cB[D+1]*z^2... - cB[D+D-1]*z^D)
		�ƂȂ�.
*/
template <u_int D> inline BilateralIIRFilter<D>&
BilateralIIRFilter<D>::initialize(const float cF[D+D], const float cB[D+D])
{
    _iirF.initialize(cF);
    _iirB.initialize(cB);
#ifdef DEBUG
    float	limit0, limit1, limit2;
    limits(limit0, limit1, limit2);
    std::cerr << "limit0 = " << limit0 << ", limit1 = " << limit1
	      << ", limit2 = " << limit2 << std::endl;
#endif
    return *this;
}

//! �t�B���^�ɂ���ݍ��݂��s��. �o�͂� operator [](int) �Ŏ��o��
/*!
  \param in	���̓f�[�^��.
  return	���̃t�B���^���g.
*/
template <u_int D> template <class S> inline BilateralIIRFilter<D>&
BilateralIIRFilter<D>::convolve(const Array<S>& in)
{
    _iirF.forward(in, _bufF);
    _iirB.backward(in, _bufB);

    return *this;
}

//! ��ݍ��݂̏o�̓f�[�^��̎�����Ԃ�
/*!
  \return	�o�̓f�[�^��̎���.
*/
template <u_int D> inline u_int
BilateralIIRFilter<D>::dim() const
{
    return _bufF.dim();
}

//! ��ݍ��݂̏o�̓f�[�^�̓���̗v�f��Ԃ�
/*!
  \param i	�v�f��index.
  \return	�v�f�̒l.
*/
template <u_int D> inline float
BilateralIIRFilter<D>::operator [](int i) const
{
    return _bufF[i] + _bufB[i];
}

/************************************************************************
*  class BilateralIIRFilter2						*
************************************************************************/
/*!
  2��������Infinite Inpulse Response Filter��\���N���X�D
*/
template <u_int D> class BilateralIIRFilter2
{
  public:
    typedef typename BilateralIIRFilter<D>::Order	Order;
    
    BilateralIIRFilter2&
		initialize(float cHF[D+D], float cHB[D+D],
			   float cVF[D+D], float cVB[D+D])		;
    BilateralIIRFilter2&
		initialize(float cHF[D+D], Order orderH,
			   float cVF[D+D], Order orderV)		;
    template <class S, class T> BilateralIIRFilter2&
		convolve(const Image<S>& in, Image<T>& out)		;
    
  private:
    BilateralIIRFilter<D>	_iirH;
    BilateralIIRFilter<D>	_iirV;
    Array2<Array<float> >	_buf;
};
    
//! �t�B���^��z�ϊ��W�����Z�b�g����
/*!
  \param cHF	�������O�iz�ϊ��W��.
  \param cHB	���������z�ϊ��W��.
  \param cHV	�c�����O�iz�ϊ��W��.
  \param cHV	�c�������z�ϊ��W��.
  \return	���̃t�B���^���g.
*/
template <u_int D> inline BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::initialize(float cHF[D+D], float cHB[D+D],
				   float cVF[D+D], float cVB[D+D])
{
    _iirH.initialize(cHF, cHB);
    _iirV.initialize(cVF, cVB);

    return *this;
}

//! �t�B���^��z�ϊ��W�����Z�b�g����
/*!
  \param cHF	�������O�iz�ϊ��W��.
  \param orderH �����������K��.
  \param cHV	�c�����O�iz�ϊ��W��.
  \param orderV	�c���������K��.
  \return	���̃t�B���^���g.
*/
template <u_int D> inline BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::initialize(float cHF[D+D], Order orderH,
				   float cVF[D+D], Order orderV)
{
    _iirH.initialize(cHF, orderH);
    _iirV.initialize(cVF, orderV);

    return *this;
}

/************************************************************************
*  class DericheConvoler						*
************************************************************************/
/*!
  Canny-Deriche�j�ɂ��X���[�V���O�C1�����������2���������܂�
  �摜��ݍ��݂��s���N���X�D
*/
class DericheConvolver : private BilateralIIRFilter2<2u>
{
  public:
    using	BilateralIIRFilter2<2u>::Order;
    
    DericheConvolver(float alpha=1.0)		{initialize(alpha);}

    DericheConvolver&	initialize(float alpha)				;
    template <class S, class T>
    DericheConvolver&	smooth(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    DericheConvolver&	gradH(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    DericheConvolver&	gradV(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    DericheConvolver&	laplacian(const Image<S>& in, Image<T>& out)	;

  private:
    float		_c0[4];	// forward coefficients for smoothing
    float		_c1[4];	// forward coefficients for 1st derivatives
    float		_c2[4];	// forward coefficients for 2nd derivatives
    Image<float>	_tmp;	// buffer for storing intermediate values
};

//! Canny-Deriche�j�ɂ��X���[�V���O
/*!
  \param in	���͉摜.
  \param out	�o�͉摜.
  \return	����Canny-Deriche�j���g.
*/
template <class S, class T> inline DericheConvolver&
DericheConvolver::smooth(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ�鉡����1������
/*!
  \param in	���͉摜.
  \param out	�o�͉摜.
  \return	����Canny-Deriche�j���g.
*/
template <class S, class T> inline DericheConvolver&
DericheConvolver::gradH(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c1, BilateralIIRFilter<2u>::First,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ��c����1������
/*!
  \param in	���͉摜.
  \param out	�o�͉摜.
  \return	����Canny-Deriche�j���g.
*/
template <class S, class T> inline DericheConvolver&
DericheConvolver::gradV(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c1, BilateralIIRFilter<2u>::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ�郉�v���V�A��
/*!
  \param in	���͉摜.
  \param out	�o�͉摜.
  \return	����Canny-Deriche�j���g.
*/
template <class S, class T> inline DericheConvolver&
DericheConvolver::laplacian(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c2, BilateralIIRFilter<2u>::Second,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, _tmp).
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c2, BilateralIIRFilter<2u>::Second).convolve(in, out);
    out += _tmp;
    
    return *this;
}

/************************************************************************
*  class GaussianConvoler						*
************************************************************************/
/*!
  Gauss�j�ɂ��X���[�V���O�C1������(DOG)�����2������(LOG)���܂�
  �摜��ݍ��݂��s���N���X�D
*/
class GaussianConvolver : private BilateralIIRFilter2<4u>
{
  private:
    struct Params
    {
	void		set(double aa, double bb, double tt, double aaa);
	Params&		operator -=(const Vector<double>& p)		;
    
	double		a, b, theta, alpha;
    };

    class EvenConstraint
    {
      public:
	typedef double		T;
	typedef Array<Params>	AT;

	EvenConstraint(T sigma) :_sigma(sigma)				{}
	
	Vector<T>	operator ()(const AT& params)		const	;
	Matrix<T>	jacobian(const AT& params)		const	;

      private:
	T		_sigma;
    };

    class CostFunction
    {
      public:
	typedef double		T;
	typedef Array<Params>	AT;
    
	enum			{D = 2};

	CostFunction(int ndivisions, T range)
	    :_ndivisions(ndivisions), _range(range)			{}
    
	Vector<T>	operator ()(const AT& params)		const	;
	Matrix<T>	jacobian(const AT& params)		const	;
	void		update(AT& params, const Vector<T>& dp)	const	;

      private:
	const int	_ndivisions;
	const T		_range;
    };

  public:
    GaussianConvolver(float sigma=1.0)		{initialize(sigma);}

    GaussianConvolver&	initialize(float sigma)				;
    template <class S, class T>
    GaussianConvolver&	smooth(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    GaussianConvolver&	gradH(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    GaussianConvolver&	gradV(const Image<S>& in, Image<T>& out)	;
    template <class S, class T>
    GaussianConvolver&	laplacian(const Image<S>& in, Image<T>& out)	;

  private:
    float		_c0[8];	// forward coefficients for smoothing
    float		_c1[8];	// forward coefficients for 1st derivatives
    float		_c2[8];	// forward coefficients for 2nd derivatives
    Image<float>	_tmp;	// buffer for storing intermediate values
};

//! Gauss�j�ɂ��X���[�V���O
/*!
  \param in	���͉摜.
  \param out	�o�͉摜.
  \return	����Gauss�j���g.
*/
template <class S, class T> inline GaussianConvolver&
GaussianConvolver::smooth(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss�j�ɂ�鉡����1������(DOG)
/*!
  \param in	���͉摜.
  \param out	�o�͉摜.
  \return	����Gauss�j���g.
*/
template <class S, class T> inline GaussianConvolver&
GaussianConvolver::gradH(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c1, BilateralIIRFilter<4u>::First,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss�j�ɂ��c����1������(DOG)
/*!
  \param in	���͉摜.
  \param out	�o�͉摜.
  \return	����Gauss�j���g.
*/
template <class S, class T> inline GaussianConvolver&
GaussianConvolver::gradV(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c1, BilateralIIRFilter<4u>::First).convolve(in, out);

    return *this;
}

//! Gauss�j�ɂ�郉�v���V�A��(LOG)
/*!
  \param in	���͉摜.
  \param out	�o�͉摜.
  \return	����Gauss�j���g.
*/
template <class S, class T> inline GaussianConvolver&
GaussianConvolver::laplacian(const Image<S>& in, Image<T>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c2, BilateralIIRFilter<4u>::Second,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, _tmp).
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c2, BilateralIIRFilter<4u>::Second).convolve(in, out);
    out += _tmp;
    
    return *this;
}

}

#endif	// !__TUIIRFilterPP_h
