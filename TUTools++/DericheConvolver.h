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
 *  $Id: DericheConvolver.h,v 1.13 2012-07-29 21:19:58 ueshiba Exp $
 */
/*!
  \file		DericheConvolver.h
  \brief	Canny-Deriche�j�ɂ���ݍ��݂Ɋւ���N���X�̒�`�Ǝ���
*/
#ifndef	__TUDericheConvolver_h
#define	__TUDericheConvolver_h

#include "TU/IIRFilter.h"

namespace TU
{
/************************************************************************
*  class DericheCoefficients<T>						*
************************************************************************/
//! Canny-Deriche�j�̌W����\���N���X
template <class T> class DericheCoefficients
{
  public:
    void	initialize(T alpha)			;
    
  protected:
    DericheCoefficients(T alpha)			{initialize(alpha);}
    
  protected:
    T		_c0[4];		//!< forward coefficients for smoothing
    T		_c1[4];		//!< forward coefficients for 1st derivatives
    T		_c2[4];		//!< forward coefficients for 2nd derivatives
};

//! Canny-Deriche�j�̏��������s��
/*!
  \param alpha	�t�B���^�T�C�Y��\�������i�������قǍL���肪�傫���j
*/
template <class T> inline void
DericheCoefficients<T>::initialize(T alpha)
{
    const T	e  = expf(-alpha), beta = sinhf(alpha);
    _c0[0] =  (alpha - 1.0) * e;		// i(n-1)
    _c0[1] =  1.0;				// i(n)
    _c0[2] = -e * e;				// oF(n-2)
    _c0[3] =  2.0 * e;				// oF(n-1)

    _c1[0] = -1.0;				// i(n-1)
    _c1[1] =  0.0;				// i(n)
    _c1[2] = -e * e;				// oF(n-2)
    _c1[3] =  2.0 * e;				// oF(n-1)

    _c2[0] =  (1.0 + beta) * e;			// i(n-1)
    _c2[1] = -1.0;				// i(n)
    _c2[2] = -e * e;				// oF(n-2)
    _c2[3] =  2.0 * e;				// oF(n-1)
}

/************************************************************************
*  class DericheConvoler<T>						*
************************************************************************/
//! Canny-Deriche�j�ɂ��1�����z���ݍ��݂��s���N���X
template <class T> class DericheConvolver
    : public DericheCoefficients<T>, private BidirectionalIIRFilter<2u, T>
{
  private:
    typedef DericheCoefficients<T>			coeffs;
    typedef BidirectionalIIRFilter<2u, T>		super;
    
  public:
    DericheConvolver(T alpha=1)	:DericheCoefficients<T>(alpha)		{}

    DericheConvolver&	initialize(T alpha)				;
    
    template <class IN, class OUT> OUT	smooth(IN ib, IN ie, OUT out)	;
    template <class IN, class OUT> OUT	diff  (IN ib, IN ie, OUT out)	;
    template <class IN, class OUT> OUT	diff2 (IN ib, IN ie, OUT out)	;

  protected:
    using	coeffs::_c0;
    using	coeffs::_c1;
    using	coeffs::_c2;
};

//! Canny-Deriche�j��alpha�l��ݒ肷��
/*!
  \param sigma	alpha�l
  \return	���̃K�E�X�j
*/
template <class T> DericheConvolver<T>&
DericheConvolver<T>::initialize(T alpha)
{
    coeffs::initialize(alpha);
    return *this;
}
    
//! Canny-Deriche�j�ɂ��X���[�V���O
/*!
  \param ib	���̓f�[�^��̐擪���w�������q
  \param ie	���̓f�[�^��̖����̎����w�������q
  \param out	�o�̓f�[�^��̐擪���w�������q
  \return	�o�̓f�[�^��̖����̎����w�������q
*/
template <class T> template <class IN, class OUT> inline OUT
DericheConvolver<T>::smooth(IN ib, IN ie, OUT out)
{
    return super::initialize(_c0, super::Zeroth)(ib, ie, out);
}

//! Canny-Deriche�j�ɂ��1�K����
/*!
  \param ib	���̓f�[�^��̐擪���w�������q
  \param ie	���̓f�[�^��̖����̎����w�������q
  \param out	�o�̓f�[�^��̐擪���w�������q
  \return	�o�̓f�[�^��̖����̎����w�������q
*/
template <class T> template <class IN, class OUT> inline OUT
DericheConvolver<T>::diff(IN ib, IN ie, OUT out)
{
    return super::initialize(_c1, super::First)(ib, ie, out);
}

//! Canny-Deriche�j�ɂ��2�K����
/*!
  \param ib	���̓f�[�^��̐擪���w�������q
  \param ie	���̓f�[�^��̖����̎����w�������q
  \param out	�o�̓f�[�^��̐擪���w�������q
  \return	�o�̓f�[�^��̖����̎����w�������q
*/
template <class T> template <class IN, class OUT> inline OUT
DericheConvolver<T>::diff2(IN ib, IN ie, OUT out)
{
    return super::initialize(_c2, super::Second)(ib, ie, out);
}

/************************************************************************
*  class DericheConvoler2<T>						*
************************************************************************/
//! Canny-Deriche�j�ɂ��2�����z���ݍ��݂��s���N���X
template <class T> class DericheConvolver2
    : public DericheCoefficients<T>, private BidirectionalIIRFilter2<2u, T>
{
  private:
    typedef DericheCoefficients<T>			coeffs;
    typedef BidirectionalIIRFilter2<2u, T>		super;
    typedef BidirectionalIIRFilter<2u, T>		IIRF;
    
  public:
    DericheConvolver2(T alpha=1)	:DericheCoefficients<T>(alpha)	{}

    DericheConvolver2	initialize(T alpha)				;
    
    template <class IN, class OUT> OUT	smooth(IN ib, IN ie, OUT out)	;
    template <class IN, class OUT> OUT	diffH (IN ib, IN ie, OUT out)	;
    template <class IN, class OUT> OUT	diffV (IN ib, IN ie, OUT out)	;
    template <class IN, class OUT> OUT	diffHH(IN ib, IN ie, OUT out)	;
    template <class IN, class OUT> OUT	diffHV(IN ib, IN ie, OUT out)	;
    template <class IN, class OUT> OUT	diffVV(IN ib, IN ie, OUT out)	;

  protected:
    using	coeffs::_c0;
    using	coeffs::_c1;
    using	coeffs::_c2;
};

//! Canny-Deriche�j��alpha�l��ݒ肷��
/*!
  \param sigma	alpha�l
  \return	���̃K�E�X�j
*/
template <class T> DericheConvolver2<T>&
DericheConvolver2<T>::initialize(T alpha)
{
    coeffs::initialize(alpha);
    return *this;
}
    
//! Canny-Deriche�j�ɂ��X���[�V���O
/*!
  \param ib	����2�����f�[�^�z��̐擪�s���w�������q
  \param ie	����2�����f�[�^�z��̖����̎��̍s���w�������q
  \param out	�o��2�����f�[�^�z��̐擪�s���w�������q
  \return	�o��2�����f�[�^�z��̖����̎��̍s���w�������q
*/
template <class T> template <class IN, class OUT> inline OUT
DericheConvolver2<T>::smooth(IN ib, IN ie, OUT out)
{
    return super::initialize(_c0, IIRF::Zeroth,
			     _c0, IIRF::Zeroth)(ib, ie, out);
}

//! Canny-Deriche�j�ɂ�鉡����1�K����
/*!
  \param ib	����2�����f�[�^�z��̐擪�s���w�������q
  \param ie	����2�����f�[�^�z��̖����̎��̍s���w�������q
  \param out	�o��2�����f�[�^�z��̐擪�s���w�������q
  \return	�o��2�����f�[�^�z��̖����̎��̍s���w�������q
*/
template <class T> template <class IN, class OUT> inline OUT
DericheConvolver2<T>::diffH(IN ib, IN ie, OUT out)
{
    return super::initialize(_c1, IIRF::First,
			     _c0, IIRF::Zeroth)(ib, ie, out);
}

//! Canny-Deriche�j�ɂ��c����1�K����
/*!
  \param ib	����2�����f�[�^�z��̐擪�s���w�������q
  \param ie	����2�����f�[�^�z��̖����̎��̍s���w�������q
  \param out	�o��2�����f�[�^�z��̐擪�s���w�������q
  \return	�o��2�����f�[�^�z��̖����̎��̍s���w�������q
*/
template <class T> template <class IN, class OUT> inline OUT
DericheConvolver2<T>::diffV(IN ib, IN ie, OUT out)
{
    return super::initialize(_c0, IIRF::Zeroth,
			     _c1, IIRF::First)(ib, ie, out);
}

//! Canny-Deriche�j�ɂ�鉡����2�K����
/*!
  \param ib	����2�����f�[�^�z��̐擪�s���w�������q
  \param ie	����2�����f�[�^�z��̖����̎��̍s���w�������q
  \param out	�o��2�����f�[�^�z��̐擪�s���w�������q
  \return	�o��2�����f�[�^�z��̖����̎��̍s���w�������q
*/
template <class T> template <class IN, class OUT> inline OUT
DericheConvolver2<T>::diffHH(IN ib, IN ie, OUT out)
{
    return super::initialize(_c2, IIRF::Second,
			     _c0, IIRF::Zeroth)(ib, ie, out);
}

//! Canny-Deriche�j�ɂ��c��������2�K����
/*!
  \param ib	����2�����f�[�^�z��̐擪�s���w�������q
  \param ie	����2�����f�[�^�z��̖����̎��̍s���w�������q
  \param out	�o��2�����f�[�^�z��̐擪�s���w�������q
  \return	�o��2�����f�[�^�z��̖����̎��̍s���w�������q
*/
template <class T> template <class IN, class OUT> inline OUT
DericheConvolver2<T>::diffHV(IN ib, IN ie, OUT out)
{
    return super::initialize(_c1, IIRF::First,
			     _c1, IIRF::First)(ib, ie, out);
}

//! Canny-Deriche�j�ɂ��c����2�K����
/*!
  \param ib	����2�����f�[�^�z��̐擪�s���w�������q
  \param ie	����2�����f�[�^�z��̖����̎��̍s���w�������q
  \param out	�o��2�����f�[�^�z��̐擪�s���w�������q
  \return	�o��2�����f�[�^�z��̖����̎��̍s���w�������q
*/
template <class T> template <class IN, class OUT> inline OUT
DericheConvolver2<T>::diffVV(IN ib, IN ie, OUT out)
{
    return super::initialize(_c0, IIRF::Zeroth,
			     _c2, IIRF::Second)(ib, ie, out);
}

}
#endif	/* !__TUDericheConvolver_h */
