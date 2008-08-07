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
 *  $Id: DericheConvolver.h,v 1.1 2008-08-07 07:26:47 ueshiba Exp $
 */
#ifndef	__TUDericheConvolver_h
#define	__TUDericheConvolver_h

#include "TU/IIRFilter++.h"

namespace TU
{
/************************************************************************
*  class DericheConvoler						*
************************************************************************/
//! Canny-Deriche�ˤˤ��������߹��ߤ�Ԥ����饹
class DericheConvolver : private BilateralIIRFilter2<2u>
{
  public:
    using	BilateralIIRFilter2<2u>::Order;
    
    DericheConvolver(float alpha=1.0)		{initialize(alpha);}

    DericheConvolver&	initialize(float alpha)				;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	smooth(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffH(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffV(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffHH(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffHV(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffVV(const Array2<T1, B1>& in, Array2<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	laplacian(const Array2<T1, B1>& in, Array2<T2, B2>& out)	;

  private:
    float		_c0[4];	// forward coefficients for smoothing
    float		_c1[4];	// forward coefficients for 1st derivatives
    float		_c2[4];	// forward coefficients for 2nd derivatives
    Array2<Array<float> >
			_tmp;	// buffer for storing intermediate values
};

//! Canny-Deriche�ˤˤ�륹�ࡼ����
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Canny-Deriche�˼���
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::smooth(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche�ˤˤ�벣����1����ʬ
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Canny-Deriche�˼���
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffH(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c1, BilateralIIRFilter<2u>::First,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche�ˤˤ�������1����ʬ
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Canny-Deriche�˼���
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffV(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c1, BilateralIIRFilter<2u>::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche�ˤˤ�벣����2����ʬ
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Canny-Deriche�˼���
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffHH(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c2, BilateralIIRFilter<2u>::Second,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche�ˤˤ��Ĳ�ξ����2����ʬ
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Canny-Deriche�˼���
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffHV(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c1, BilateralIIRFilter<2u>::First,
		   _c1, BilateralIIRFilter<2u>::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche�ˤˤ�������2����ʬ
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Canny-Deriche�˼���
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffVV(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c2, BilateralIIRFilter<2u>::Second).convolve(in, out);

    return *this;
}

//! Canny-Deriche�ˤˤ���ץ饷����
/*!
  \param in	���ϲ���
  \param out	���ϲ���
  \return	����Canny-Deriche�˼���
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::laplacian(const Array2<T1, B1>& in, Array2<T2, B2>& out)
{
    diffHH(in, _tmp).diffVV(in, out);
    out += _tmp;
    
    return *this;
}

}

#endif	/* !__TUDericheConvolver_h */
