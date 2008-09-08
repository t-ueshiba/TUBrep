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
 *  $Id: Warp.h,v 1.2 2008-09-08 08:06:23 ueshiba Exp $
 */
#ifndef	__TUWarp_h
#define	__TUWarp_h

#include "TU/Image++.h"
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class Warp								*
************************************************************************/
//! �������ѷ����뤿��Υ��饹��
class Warp
{
  private:
    struct FracArray
    {
	FracArray(u_int d=0)
	    :us(d), vs(d), du(d), dv(d), lmost(0)	{}

	u_int		width()			const	{return us.dim();}
	void		resize(u_int d)			;

	Array<short>				us, vs;
#ifdef __INTEL_COMPILER	
	Array<u_char, AlignedBuf<u_char> >	du, dv;
#else
	Array<u_char>				du, dv;
#endif
	int					lmost;
    };
    
  public:
    Warp()	:_fracs(), _width(0)			{}

    u_int	width()				const	{return _width;}
    u_int	height()			const	{return _fracs.dim();}
    int		lmost(int v)			const	;
    int		rmost(int v)			const	;

    void	initialize(const Matrix<double>& Htinv,
			   u_int inWidth,  u_int inHeight,
			   u_int outWidth, u_int outHeight)		;
    void	initialize(const Matrix<double>& Htinv,
			   const CameraBase::Intrinsic& intrinsic,
			   u_int inWidth,  u_int inHeight,
			   u_int outWidth, u_int outHeight)		;
    template <class T>
    void	operator ()(const Image<T>& in, Image<T>& out,
			    int vs=0, int ve=0)			const	;
    Vector2f	operator ()(int u, int v)			const	;

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

inline int
Warp::lmost(int v) const
{
    return _fracs[v].lmost;
}

inline int
Warp::rmost(int v) const
{
    return _fracs[v].lmost + _fracs[v].width();
}

inline Vector2f
Warp::operator ()(int u, int v) const
{
    Vector2f		val;
    const FracArray&	fracs = _fracs[v];
    val[0] = float(fracs.us[u]) + float(fracs.du[u]) / 128.0;
    val[1] = float(fracs.vs[u]) + float(fracs.dv[u]) / 128.0;
    return val;
}

}

#endif	/* !__TUWarp_h */
