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
 *  $Id: DericheConvolver.h,v 1.7 2009-07-31 07:04:44 ueshiba Exp $
 */
#ifndef	__TUDericheConvolver_h
#define	__TUDericheConvolver_h

#include "TU/IIRFilter.h"

namespace TU
{
/************************************************************************
*  class DericheCoefficients						*
************************************************************************/
class DericheCoefficients
{
  public:
    void	initialize(float alpha)			;
    
  protected:
    DericheCoefficients(float alpha)			{initialize(alpha);}
    
  protected:
    float	_c0[4];		// forward coefficients for smoothing
    float	_c1[4];		// forward coefficients for 1st derivatives
    float	_c2[4];		// forward coefficients for 2nd derivatives
};

//! Canny-Deriche�j�̏��������s��
/*!
  \param alpha	�t�B���^�T�C�Y��\�������i�������قǍL���肪�傫���j
*/
inline void
DericheCoefficients::initialize(float alpha)
{
    const float	e  = expf(-alpha), beta = sinhf(alpha);
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
*  class DericheConvoler						*
************************************************************************/
//! Canny-Deriche�j�ɂ��1�����z���ݍ��݂��s���N���X
class DericheConvolver
    : public DericheCoefficients, private BilateralIIRFilter<2u>
{
  public:
    typedef BilateralIIRFilter<2u>		BIIRF;
    
    DericheConvolver(float alpha=1.0)	:DericheCoefficients(alpha)	{}

    template <class T1, class B1, class T2, class B2> DericheConvolver&
	smooth(const Array<T1, B1>& in, Array<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diff(const Array<T1, B1>& in, Array<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diff2(const Array<T1, B1>& in, Array<T2, B2>& out)		;
};

//! Canny-Deriche�j�ɂ��X���[�V���O
/*!
  \param in	����1�����z��
  \param out	�o��1�����z��
  \return	����Canny-Deriche�j���g
*/
template <class T1, class B1, class T2, class B2>
inline DericheConvolver&
DericheConvolver::smooth(const Array<T1, B1>& in, Array<T2, B2>& out)
{
    BIIRF::initialize(_c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ��1�K����
/*!
  \param in	����1�����z��
  \param out	�o��1�����z��
  \return	����Canny-Deriche�j���g
*/
template <class T1, class B1, class T2, class B2>
inline DericheConvolver&
DericheConvolver::diff(const Array<T1, B1>& in, Array<T2, B2>& out)
{
    BIIRF::initialize(_c1, BIIRF::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ��2�K����
/*!
  \param in	����1�����z��
  \param out	�o��1�����z��
  \return	����Canny-Deriche�j���g
*/
template <class T1, class B1, class T2, class B2>
inline DericheConvolver&
DericheConvolver::diff2(const Array<T1, B1>& in, Array<T2, B2>& out)
{
    BIIRF::initialize(_c2, BIIRF::Second).convolve(in, out);

    return *this;
}

/************************************************************************
*  class DericheConvoler2<BIIRH, BIIRV>					*
************************************************************************/
//! Canny-Deriche�j�ɂ��2�����z���ݍ��݂��s���N���X
template <class BIIRH=BilateralIIRFilter<2u>, class BIIRV=BIIRH>
class DericheConvolver2
    : public DericheCoefficients, private BilateralIIRFilter2<BIIRH, BIIRV>
{
  public:
    typedef BilateralIIRFilter<2u>		BIIRF;
    typedef BilateralIIRFilter2<BIIRH, BIIRV>	BIIRF2;
    
    DericheConvolver2(float alpha=1.0)	:DericheCoefficients(alpha)	{}
    DericheConvolver2(float alpha, u_int nthreads)
    	:DericheCoefficients(alpha), BIIRF2(nthreads)			{}

    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	smooth(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffH(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffV(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffHH(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffHV(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	diffVV(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out)	;
    template <class T1, class B1, class R1, class T2, class B2, class R2>
    DericheConvolver2&
	laplacian(const Array2<T1, B1, R1>& in, Array2<T2, B2, R2>& out);

  private:
    Array2<Array<float> >	_tmp;	// buffer for computing Laplacian
};

//! Canny-Deriche�j�ɂ��X���[�V���O
/*!
  \param in	����2�����z��
  \param out	�o��2�����z��
  \return	����Canny-Deriche�j���g
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::smooth(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c0, BIIRF::Zeroth,
		       _c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ�鉡����1�K����
/*!
  \param in	����2�����z��
  \param out	�o��2�����z��
  \return	����Canny-Deriche�j���g
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffH(const Array2<T1, B1, R1>& in,
				       Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c1, BIIRF::First,
		       _c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ��c����1�K����
/*!
  \param in	����2�����z��
  \param out	�o��2�����z��
  \return	����Canny-Deriche�j���g
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffV(const Array2<T1, B1, R1>& in,
				       Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c0, BIIRF::Zeroth,
		       _c1, BIIRF::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ�鉡����2�K����
/*!
  \param in	����2�����z��
  \param out	�o��2�����z��
  \return	����Canny-Deriche�j���g
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffHH(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c2, BIIRF::Second,
		       _c0, BIIRF::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ��c��������2�K����
/*!
  \param in	����2�����z��
  \param out	�o��2�����z��
  \return	����Canny-Deriche�j���g
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffHV(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c1, BIIRF::First,
		       _c1, BIIRF::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ��c����2�K����
/*!
  \param in	����2�����z��
  \param out	�o��2�����z��
  \return	����Canny-Deriche�j���g
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::diffVV(const Array2<T1, B1, R1>& in,
					Array2<T2, B2, R2>& out)
{
    BIIRF2::initialize(_c0, BIIRF::Zeroth,
		       _c2, BIIRF::Second).convolve(in, out);

    return *this;
}

//! Canny-Deriche�j�ɂ�郉�v���V�A��
/*!
  \param in	����2�����z��
  \param out	�o��2�����z��
  \return	����Canny-Deriche�j���g
*/
template <class BIIRH, class BIIRV>
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline DericheConvolver2<BIIRH, BIIRV>&
DericheConvolver2<BIIRH, BIIRV>::laplacian(const Array2<T1, B1, R1>& in,
					   Array2<T2, B2, R2>& out)
{
    diffHH(in, _tmp).diffVV(in, out);
    out += _tmp;
    
    return *this;
}

}

#endif	/* !__TUDericheConvolver_h */
