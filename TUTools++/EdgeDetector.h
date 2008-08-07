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
 *  $Id: EdgeDetector.h,v 1.1 2008-08-07 07:26:48 ueshiba Exp $
 */
#ifndef	__TUEdgeDetector_h
#define	__TUEdgeDetector_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class EdgeDetector							*
************************************************************************/
//! ���å����Ф�Ԥ����饹
class EdgeDetector
{
  public:
    enum
    {
	TRACED	= 0x04,
	EDGE	= 0x02,					//!< �������å���
	WEAK	= 0x01					//!< �夤���å���
    };
    
    EdgeDetector(float th_low=2.0, float th_high=5.0)			;
    
    EdgeDetector&	initialize(float th_low, float th_high)		;
    const EdgeDetector&
	strength(const Image<float>& edgeH,
		 const Image<float>& edgeV, Image<float>& out)	  const	;
    const EdgeDetector&
	direction4(const Image<float>& edgeH,
		   const Image<float>& edgeV, Image<u_char>& out) const	;
    const EdgeDetector&
	direction8(const Image<float>& edgeH,
		   const Image<float>& edgeV, Image<u_char>& out) const	;
    const EdgeDetector&
	suppressNonmaxima(const Image<float>& strength,
			  const Image<u_char>& direction,
			  Image<u_char>& out)			  const	;
    const EdgeDetector&
	zeroCrossing(const Image<float>& in, Image<u_char>& out)  const	;
    const EdgeDetector&
	zeroCrossing(const Image<float>& in,
		     const Image<float>& strength,
		     Image<u_char>& out)			  const	;
    const EdgeDetector&
	hysteresisThresholding(Image<u_char>& edge)		  const	;

  private:
    float		_th_low, _th_high;
};

//! ���å����д����������
/*!
  \param th_low		�夤���å�������
  \param th_low		�������å�������
*/
inline
EdgeDetector::EdgeDetector(float th_low, float th_high)
{
    initialize(th_low, th_high);
}

//! ���å����Ф����ͤ����ꤹ��
/*!
  \param th_low		�夤���å�������
  \param th_low		�������å�������
  \return		���Υ��å����дＫ��
*/
inline EdgeDetector&
EdgeDetector::initialize(float th_low, float th_high)
{
    _th_low  = th_low;
    _th_high = th_high;

    return *this;
}
 
}

#endif	/* !__TUEdgeDetector_h */
