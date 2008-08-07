/*
 *  ʿ��9-19ǯ���ȡ˻��ȵ�����縦��� �����ͭ
 *  
 *  �Ϻ�ԡ����ǽ���
 *
 *  �ܥץ����ϡ��ȡ˻��ȵ�����縦���ο����Ǥ��뿢�ǽ��פ��Ϻ��
 *  ���ȡ˻��ȵ�����縦��꤬������ͭ������̩����Ǥ����Ϻ�Ԥˤ�
 *  ����Ĥʤ����ܥץ�������ѡ�ʣ�������ѡ��軰�Ԥس�������������
 *  ��򿯳�����԰٤�ػߤ��ޤ���
 *  
 *  ���Υץ����ˤ�ä������뤤���ʤ�»�����Ф��Ƥ⡤�����ͭ�Ԥ�
 *  ����Ϻ�Ԥ���Ǥ���餤�ޤ���
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: GaussianConvolver.h,v 1.1 2008-08-07 07:26:48 ueshiba Exp $
 */
#ifndef	__TUGaussianConvolver_h
#define	__TUGaussianConvolver_h

#include "TU/Vector++.h"
#include "TU/IIRFilter++.h"

namespace TU
{
/************************************************************************
*  class GaussianConvoler						*
************************************************************************/
//! Gauss�ˤˤ��������߹��ߤ�Ԥ����饹
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
	typedef double		ET;
	typedef Array<Params>	AT;

	EvenConstraint(ET sigma) :_sigma(sigma)				{}
	
	Vector<ET>	operator ()(const AT& params)		const	;
	Matrix<ET>	jacobian(const AT& params)		const	;

      private:
	ET		_sigma;
    };

    class CostFunction
    {
      public:
	typedef double		ET;
	typedef Array<Params>	AT;
    
	enum			{D = 2};

	CostFunction(int ndivisions, ET range)
	    :_ndivisions(ndivisions), _range(range)			{}
    
	Vector<ET>	operator ()(const AT& params)		 const	;
	Matrix<ET>	jacobian(const AT& params)		 const	;
	void		update(AT& params, const Vector<ET>& dp) const	;

      private:
	const int	_ndivisions;
	const ET	_range;
    };

  public:
    GaussianConvolver(float sigma=1.0)		{initialize(sigma);}

    GaussianConvolver&	initialize(float sigma)				;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	smooth(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffH(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffV(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffHH(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffHV(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffVV(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	laplacian(const Array2<T1, B1>& in, Array2<T2, B2>& out)	;

  private:
    float		_c0[8];	// forward coefficients for smoothing
    float		_c1[8];	// forward coefficients for 1st derivatives
    float		_c2[8];	// forward coefficients for 2nd derivatives
    Array2<Array<float> >
			_tmp;	// buffer for storing intermediate values
};

//! Gauss�ˤˤ�륹�ࡼ����
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Gauss�˼���
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::smooth(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss�ˤˤ�벣����1����ʬ(DOG)
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Gauss�˼���
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffH(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c1, BilateralIIRFilter<4u>::First,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss�ˤˤ�������1����ʬ(DOG)
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Gauss�˼���
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffV(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c1, BilateralIIRFilter<4u>::First).convolve(in, out);

    return *this;
}

//! Gauss�ˤˤ�벣����2����ʬ
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Gauss�˼���
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffHH(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c2, BilateralIIRFilter<4u>::Second,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss�ˤˤ��Ĳ�ξ����2����ʬ
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Gauss�˼���
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffHV(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c1, BilateralIIRFilter<4u>::First,
		   _c1, BilateralIIRFilter<4u>::First).convolve(in, out);

    return *this;
}

//! Gauss�ˤˤ�������2����ʬ
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Gauss�˼���
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffVV(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c2, BilateralIIRFilter<4u>::Second).convolve(in, out);

    return *this;
}

//! Gauss�ˤˤ���ץ饷����(LOG)
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Gauss�˼���
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::laplacian(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    diffHH(in, _tmp).diffVV(in, out);
    out += _tmp;
    
    return *this;
}

}

#endif	/* !__TUGaussianConvolver_h */
