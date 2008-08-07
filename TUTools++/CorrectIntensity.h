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
 *  $Id: CorrectIntensity.h,v 1.1 2008-08-07 11:45:05 ueshiba Exp $
 */
#ifndef	__TUCorrectIntensity_h
#define	__TUCorrectIntensity_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class CorrectIntensity						*
************************************************************************/
class CorrectIntensity
{
  public:
    CorrectIntensity(float gain=1.0, float offset=0.0)
	:_gain(gain), _offset(offset)					{}

    void	initialize(float gain, float offset)			;
    template <class T>
    void	operator()(Image<T>& image, int vs=0, int ve=0)	const	;
    
  private:
    template <class T>
    T		val(T pixel)					const	;
    
    float	_gain, _offset;
};

inline void
CorrectIntensity::initialize(float gain, float offset)
{
    _gain   = gain;
    _offset = offset;
}

}

#endif	/* !__TUCorrectIntensity_h */
