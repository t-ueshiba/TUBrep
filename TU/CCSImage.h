/*
 *  ����21-22�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
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
 *  Copyright 2009-2010.
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
 *  $Id: CCSImage.h,v 1.2 2010-11-22 06:16:19 ueshiba Exp $
 */
#ifndef __TU_CCSIMAGE_H
#define __TU_CCSIMAGE_H

#include "TU/Image++.h"
#include <complex>
#include <limits>

namespace TU
{
/************************************************************************
*  static functions							*
************************************************************************/
template <class T> static inline void
setSign(T& val)
{
    val = (val > T(0) ? T(1) : val < T(0) ? T(-1) : T(0));
}
    
/************************************************************************
*  class CCSImageLine<T>						*
************************************************************************/
//! ���̗��U�t�[���G�ϊ��� complex conjugate symmetrical �`���ŕ\���������l1�����z��
template <class T>
class CCSImageLine : public ImageLine<T>
{
  public:
    typedef ImageLine<T>				line_type;
    typedef typename line_type::iterator		iterator;
    typedef typename line_type::const_iterator		const_iterator;
    typedef typename line_type::reverse_iterator	reverse_iterator;
    typedef typename line_type::const_reverse_iterator	const_reverse_iterator;
    
  public:
  //! CCS�`����1�����z��𐶐�����D
  /*!
    \param d	�z��̗v�f��
  */
    explicit CCSImageLine(u_int d=0) :line_type(d)			{}

    using		line_type::begin;
    using		line_type::end;
    using		line_type::rbegin;
    using		line_type::rend;
    using		line_type::size;
    using		line_type::check_size;
    
    std::complex<T>	operator ()(u_int u)			const	;
    std::complex<T>	conj(u_int u)				const	;
    CCSImageLine<T>	mag()					const	;
    CCSImageLine<T>	specmag()				const	;
    CCSImageLine<T>&	pdiff(const CCSImageLine<T>& spectrum)		;
    CCSImageLine<T>&	operator *=(const CCSImageLine<T>& spectrum)	;
    CCSImageLine<T>&	operator /=(const CCSImageLine<T>& magnitude)	;
    T			maximum(T& uMax)			const	;
    T			maximum()				const	;
};

//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C�w�肳�ꂽindex�ɑ΂���l�i���f���j��Ԃ��D
/*!
  \param u	index
  \return	u �ɂ���Ďw�肳�ꂽ�v�f�̒l
*/
template <class T> inline std::complex<T>
CCSImageLine<T>::operator ()(u_int u) const
{
    using namespace	std;

    const u_int	u2 = size() / 2;
    if (u == 0)
	return (*this)[0];
    else if (u < u2)
    {
	const u_int	uu = 2*u;
	return complex<T>((*this)[uu-1], (*this)[uu]);
    }
    else if (u == u2)
	return (*this)[size() - 1];
    else
    {
	const u_int	uu = 2*(size() - u);
	return complex<T>((*this)[uu-1], (*this)[uu]);
    }
}

//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C�w�肳�ꂽindex�ɑ΂���l�̋��𕡑f���l��Ԃ��D
/*!
  \param u	index
  \return	u �ɂ���Ďw�肳�ꂽ�v�f�̒l�̋��𕡑f���l
*/
template <class T> inline std::complex<T>
CCSImageLine<T>::conj(u_int u) const
{
    return std::complex<T>((*this)[u]);
}

//! ����CCS�z�񂪋�ԗ̈�ɂ���Ƃ��C�e�v�f�̐U����v�f�Ƃ���1�����z���Ԃ��D
/*!
  �Ԃ����1�����z��͋�ԗ̈�ɑ�����D�܂��C���_�͔z��̍��[�ƂȂ�D
  \return	�U����v�f�Ƃ���1�����z��
*/
template <class T> CCSImageLine<T>
CCSImageLine<T>::mag() const
{
    using namespace	std;
    
    CCSImageLine<T>	ccs(size());
    const_iterator	p = begin();
    for (iterator q = ccs.begin(); q != ccs.end(); )
	*q++ = abs(*p++);
    
    return ccs;
}

//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C�e�v�f�̐U����v�f�Ƃ���1�����z���Ԃ��D
/*!
  �Ԃ����1�����z��͋�ԗ̈�ɑ�����D�܂��C���_���z��̍��[���璆���Ɉڂ����D
  \return	�U����v�f�Ƃ���1�����z��
*/
template <class T> CCSImageLine<T>
CCSImageLine<T>::specmag() const
{
    using namespace	std;
    
    CCSImageLine<T>	ccs(size());
    const u_int		u2 = ccs.size() / 2;
    iterator		pF = ccs.begin() + u2;
    iterator		pB = pF;
    for (const_iterator p = begin() + 1, pe = end() - 1; p != pe; p += 2)
    {
	*++pF = *--pB = abs(complex<T>(*p, *(p+1)));
    }
    ccs[u2] = abs((*this)[0]);
    ccs[0]  = abs((*this)[size() - 1]);
    
    return ccs;
}

//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C�^����ꂽ�����ЂƂ�CCS�z��Ƃ̈ʑ����ɕϊ�����D
/*!
  \param spectrum	���g���̈�ɂ���CCS�z��	
  \return		�ʑ�����\��1�����z��
*/
template <class T> CCSImageLine<T>&
CCSImageLine<T>::pdiff(const CCSImageLine<T>& spectrum)
{
    using namespace	std;
    
    check_size(spectrum.size());

    iterator	p = begin() + 1;
    for (const_iterator q  = spectrum.begin() + 1,
			qe = spectrum.end()   - 1; q != qe; q += 2)
    {
	complex<T>	val = complex<T>(*p, *(p+1)) * complex<T>(*q, -*(q+1));
	if (val != T(0))
	    val /= abs(val);
	*p++ = val.real();
	*p++ = val.imag();
    }
    const u_int	u1 = size() - 1;
    setSign((*this)[0]  *= spectrum[0]);
    setSign((*this)[u1] *= spectrum[u1]);

    return *this;
}

//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C����ɕʂ�CCS�z��̕��f�������|����D
/*!
  \param spectrum	���g���̈�ɂ���CCS�z��	
  \return		specturm�̕��f�������|������̂��̔z��
*/
template <class T> CCSImageLine<T>&
CCSImageLine<T>::operator *=(const CCSImageLine<T>& spectrum)
{
    using namespace	std;
    
    check_size(spectrum.size());

    iterator	p = begin() + 1;
    for (const_iterator q  = spectrum.begin() + 1,
			qe = spectrum.end()   - 1; q != qe; q += 2)
    {
	complex<T>	val = complex<T>(*p, *(p+1)) * complex<T>(*q, -*(q+1));
	*p++ = val.real();
	*p++ = val.imag();
    }
    const u_int	u1 = size() - 1;
    (*this)[0]  *= spectrum[0];
    (*this)[u1] *= spectrum[u1];

    return *this;
}

//! ����CCS�z�񂪋�ԗ̈�ɂ���Ƃ��C�����ЂƂ�CCS�z��Ŋ���D
/*!
  \param spectrum	��ԗ̈�ɂ���CCS�z��	
  \return		�����Ƃ�����̂���CCS�z��
*/
template <class T> CCSImageLine<T>&
CCSImageLine<T>::operator /=(const CCSImageLine<T>& magnitude)
{
    using namespace	std;
    
    check_size(magnitude.size());

    T			thresh = magnitude.maximum() * 0.1;
    const_iterator	p = magnitude.begin();
    for (iterator q = begin(); q != end(); ++q)
    {
	const T	val = *p++;
	if (val > thresh)
	    *q /= val;
	else
	    *q = 0;
    }

    return *this;
}

//! ����CCS�z�񂪋�ԗ̈�ɂ���Ƃ��C�z��v�f���̍ő�l�Ƃ��̈ʒu��Ԃ��D
/*!
  \param uMax	�ő�l��^����v�f�̈ʒu
  \return	�ő�l
*/
template <class T> T
CCSImageLine<T>::maximum(T& uMax) const
{
    using namespace	std;
    
    T	valMax = numeric_limits<T>::min();
    for (u_int u = 0; u < size(); ++u)
    {
	const T	val = (*this)[u];
	if (val > valMax)
	{
	    valMax = val;
	    uMax   = u;
	}
    }
#if _DEBUG >= 2
    Image<T>	tmp(50, size());
    for (u_int t = 0; t < tmp.height(); ++t)
	tmp[t] = (*this)[t] * T(255) / valMax;
    tmp.save(cout, ImageBase::FLOAT);
    cerr << "CCSImageLine<T>::maximum()..." << endl;
#endif
#ifdef _DEBUG
    cerr << "CCSImageLine<T>::maximum(): " << valMax << "@(" << uMax
	 << ") in [0, " << size() << ')' << endl;
#endif
    return valMax;
}
    
template <class T> T
CCSImageLine<T>::maximum() const
{
    using namespace	std;
    
    T	valMax = numeric_limits<T>::min();
    for (u_int u = 0; u < size(); ++u)
    {
	const T	val = (*this)[u];
	if (val > valMax)
	    valMax = val;
    }

    return valMax;
}

/************************************************************************
*  class CCSImage<T>							*
************************************************************************/
//! ���̗��U�t�[���G�ϊ��� complex conjugate symmetrical �`���ŕ\���������l2�����z��
template <class T>
class CCSImage : public Image<T>
{
  public:
    typedef Image<T>					image_type;
    typedef typename image_type::iterator		iterator;
    typedef typename image_type::const_iterator		const_iterator;
    typedef typename image_type::reverse_iterator	reverse_iterator;
    typedef typename image_type::const_reverse_iterator	const_reverse_iterator;
    typedef ImageLine<T>				line_type;
    typedef typename line_type::iterator		pixel_iterator;
    typedef typename line_type::const_iterator		const_pixel_iterator;
    typedef typename line_type::reverse_iterator	reverse_pixel_iterator;
    typedef typename line_type::const_reverse_iterator	const_reverse_pixel_iterator;
    
  public:
  //! CCS�`����2�����z��𐶐�����D
  /*!
    \param w	�z��̕�
    \param h	�z��̍���
  */
    explicit CCSImage(u_int w=0, u_int h=0) :image_type(w, h)		{}

    using		image_type::begin;
    using		image_type::end;
    using		image_type::rbegin;
    using		image_type::rend;
    using		image_type::size;
    using		image_type::width;
    using		image_type::height;
    using		image_type::check_size;

    std::complex<T>	operator ()(u_int u, u_int v)		const	;
    std::complex<T>	conj(u_int u, u_int v)			const	;
    CCSImage<T>		specmag()				const	;
    CCSImage<T>		logpolar()				const	;
    CCSImageLine<T>	intpolar()				const	;
    CCSImage<T>&	pdiff(const CCSImage<T>& spectrum)		;
    CCSImage<T>&	operator *=(const CCSImage<T>& spectrum)	;
    T			maximum(Point2<T>& pMax)		const	;
    T			maximum()				const	;
};

//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C�w�肳�ꂽindex�ɑ΂���l�i���f���j��Ԃ��D
/*!
  \param u	��index
  \param v	�cindex
  \return	u, v �ɂ���Ďw�肳�ꂽ�v�f�̒l
*/
template <class T> std::complex<T>
CCSImage<T>::operator ()(u_int u, u_int v) const
{
    using namespace	std;
    
    const u_int	u2 = width() / 2;
    u_int	uu;
    if (u == 0)
	uu = 0;
    else if (u < u2)
    {
	uu = 2*u;
	return complex<T>((*this)[v][uu-1], (*this)[v][uu]);
    }
    else if (u == u2)
	uu = width() - 1;
    else
    {
	uu = 2*(width() - u);
	return complex<T>((*this)[v][uu-1], -(*this)[v][uu]);
    }

    const u_int	v2 = height() / 2;
    u_int	vv;
    if (v == 0)
	return (*this)[0][uu];
    else if (v < v2)
    {
	vv = 2*v;
	return complex<T>((*this)[vv-1][uu], (*this)[vv][uu]);
    }
    else if (v == v2)
	return (*this)[height() - 1][uu];
    else
    {
	vv = 2*(height() - v);
	return complex<T>((*this)[vv-1][uu], -(*this)[vv][uu]);
    }
}

//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C�w�肳�ꂽindex�ɑ΂���l�̋��𕡑f���l��Ԃ��D
/*!
  \param u	��index
  \param v	�cindex
  \return	u, v �ɂ���Ďw�肳�ꂽ�v�f�̒l
*/
template <class T> inline std::complex<T>
CCSImage<T>::conj(u_int u, u_int v) const
{
    return std::conj((*this)(u, v));
}

//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C�e�v�f�̐U����v�f�Ƃ���2�����z���Ԃ��D
/*!
  �Ԃ����2�����z��͋�ԗ̈�ɑ����C���_�ɑ΂��đΏ̂ł���D���̌��_�͔z���
  ������ł͂Ȃ������Ɉʒu����D
  \return	�U����v�f�Ƃ���2�����z��
*/
template <class T> CCSImage<T>
CCSImage<T>::specmag() const
{
    using namespace	std;
    
  // CCS�s�񂩂�U���������o���D
  // 1. CCS�s��̍ŏ��ƍŌ�̗�ȊO�̐U�����v�Z����D
    CCSImage<T>	ccs(width(), height());
    const u_int	u2 = ccs.width()  / 2;
    const u_int	v2 = ccs.height() / 2;
    iterator	lF = ccs.begin() + v2;
    iterator	lB = lF;
    for (const_iterator l = begin(); l != end(); ++l)
    {
	if (lF == ccs.end())
	{
	    lF = ccs.begin();
	    lB = ccs.end();
	}
	pixel_iterator	pF = (lF++)->begin() + u2;
	pixel_iterator	pB = (--lB)->begin() + u2;
	for (const_pixel_iterator p  = l->begin() + 1,
				  pe = l->end()	  - 1; p != pe; p += 2)
	{
	    *++pF = *--pB = abs(complex<T>(*p, *(p+1)));
	}
    }

  // 2. CCS�s��̍ŏ��ƍŌ�̗�i�ŏ��ƍŌ�̍s�ȊO�j�̐U�����v�Z����D
    const u_int	u1 = width() - 1;
    lF = ccs.begin() + v2;
    lB = lF;
    for (const_iterator l = begin() + 1; l != end() - 1; l += 2)
    {
	(*++lF)[u2] = (*--lB)[u2] = abs(complex<T>((*l)[0],  (*(l+1))[0]));
	(*lF)[0]    = (*lB)[0]    = abs(complex<T>((*l)[u1], (*(l+1))[u1]));
    }

  // 3. CCS�s��̎l���̐U�����v�Z����D
    const u_int	v1 = height() - 1;
    ccs[v2][u2] = abs((*this)[0][0]);
    ccs[v2][0]  = abs((*this)[0][u1]);
    ccs[0][u2]  = abs((*this)[v1][0]);
    ccs[0][0]   = abs((*this)[v1][u1]);

#if _DEBUG >= 3
    Image<T>	tmp(ccs);
    tmp *= T(255) / ccs.maximum();
    tmp.save(cout, ImageBase::FLOAT);
    cerr << "CCSImage<T>::mag()..." << endl;
#endif
    return ccs;
}

//! ����CCS�z�񂪋�ԗ̈�ɂ���Ƃ��C����log-polar�\����Ԃ��D
/*!
  \return		log-polar���W�n�ŕ\�����ꂽ2�����z��
*/
template <class T> CCSImage<T>
CCSImage<T>::logpolar() const
{
    using namespace	std;

    const u_int		size = max(width(), height()) / 2;

    static Array<T>	windowR;
    initializeHanningWindow(windowR, size);
    
    CCSImage<T>	lp(size, size);
    const T	u2 = width() / 2, v2 = height() / 2,
		u1 = width() - 1, v1 = height() - 1;
    const T	base = pow(T(lp.width() - 1), T(1)/T(lp.width() - 1));
    const T	step = T(M_PI) / T(lp.height());
    for (u_int t = 0; t < lp.height(); ++t)
    {
	line_type&	line = lp[t];
	const T		ang = t * step;
	Point2<T>	p(cos(ang), sin(ang));

	for (u_int r = 0; r < lp.width(); ++r)
	{
	    Point2<T>	q(p[0] + u2, p[1] + v2);

	    if (0 <= q[0] && q[0] <= u1 && 0 <= q[1] && q[1] <= v1)
	      //line[r] = at(q);
	      //line[r] = r * at(q);
		line[r] = windowR[r] * at(q);
	      //line[r] = log(T(1) + at(q));
	    p *= base;
	}
    }
#if _DEBUG >= 1
    Image<T>	tmp(lp);
    tmp *= T(255) / lp.maximum();
    tmp.save(cout, ImageBase::FLOAT);
    cerr << "CCSImage<T>::logpolar()..." << endl;
#endif
    return lp;
}

//! ����CCS�z�񂪋�ԗ̈�ɂ����Č��_���z�񒆉��ɂ���Ƃ��C���a�����ɐϕ�����polar�\����Ԃ��D
/*!
  \return		���a�����ɐϕ�����polar���W�n�ŕ\�����ꂽ1�����z��
*/
template <class T> CCSImageLine<T>
CCSImage<T>::intpolar() const
{
    using namespace	std;

    const u_int		size = max(width(), height());

    static Array<T>	windowR;
    initializeHanningWindow(windowR, size);

    CCSImageLine<T>	ip(size);
    const T		u2 = width() / 2, v2 = height() / 2,
			u1 = width() - 1, v1 = height() - 1;
    const T		base = pow(T(ip.size() - 1), T(1)/T(ip.size() - 1));
    const T		step = T(M_PI) / T(ip.size());
    for (u_int t = 0; t < ip.size(); ++t)
    {
	T&		pix = ip[t];
	const T		ang = t * step;
	const Point2<T>	p(cos(ang), sin(ang));
	for (u_int r = 1; r < ip.size(); ++r)
	{
	    Point2<T>	q(r*p[0] + u2, r*p[1] + v2);
	    if (0 <= q[0] && q[0] <= u1 && 0 <= q[1] && q[1] <= v1)
	      //pix += at(q);
	      //pix += r * at(q);
		pix += windowR[r] * at(q);
	      //pix += log(T(1) + at(q));
	}
    }
#if _DEBUG >= 1
    T		maxVal = ip.maximum();
    Image<T>	tmp(50, ip.size());
    for (u_int t = 0; t < tmp.height(); ++t)
	tmp[t] = ip[t] * T(255) / maxVal;
    tmp.save(cout, ImageBase::FLOAT);
    cerr << "CCSImage<T>::intpolar()..." << endl;
#endif
    return ip;
}
    
//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C�^����ꂽ�����ЂƂ�CCS�z��Ƃ̈ʑ����ɕϊ�����D
/*!
  \param spectrum	���g���̈�ɂ���CCS�z��	
  \return		specturm�Ƃ̈ʑ����ɕϊ�������̂��̔z��
*/
template <class T> CCSImage<T>&
CCSImage<T>::pdiff(const CCSImage<T>& spectrum)
{
    using namespace	std;

    check_size(spectrum.size());
    
  // 1. CCS�s��̍ŏ��ƍŌ�̗�ȊO�̐ς̈ʑ������߂�D
    iterator	l = begin();
    for (const_iterator m = spectrum.begin(); m != spectrum.end(); ++m)
    {
	pixel_iterator	p = (l++)->begin() + 1;
	for (const_pixel_iterator q  = m->begin() + 1,
				  qe = m->end()	  - 1; q != qe; q += 2)
	{
	    complex<T>	val = complex<T>(*p, *(p+1)) * complex<T>(*q, -*(q+1));
	    if (val != T(0))
		val /= abs(val);
	    *p++ = val.real();
	    *p++ = val.imag();
	}
    }

  // 2. CCS�s��̍ŏ��ƍŌ�̗�i�ŏ��ƍŌ�̍s�ȊO�j�̐ς̈ʑ������߂�D
    const u_int	u1 = width() - 1;
    l = begin() + 1;
    for (const_iterator m  = spectrum.begin() + 1;
			m != spectrum.end()   - 1; m += 2)
    {
      // �ŏ��̗�̗v�f�̎����Ƌ������܂� 2x1 �����s����Ƃ��Đς����߂�D
	complex<T>	val = complex<T>((*l)[0],  (*(l+1))[0])
			    * complex<T>((*m)[0], -(*(m+1))[0]);
	if (val != T(0))
	    val /= abs(val);
	(*l)[0]     = val.real();
	(*(l+1))[0] = val.imag();
	
      // �Ō�̗�̗v�f�̎����Ƌ������܂� 2x1 �����s����Ƃ��Đς̈ʑ������߂�D
	val = complex<T>((*l)[u1],  (*(l+1))[u1])
	    * complex<T>((*m)[u1], -(*(m+1))[u1]);
	if (val != T(0))
	    val /= abs(val);
	(*l++)[u1] = val.real();
	(*l++)[u1] = val.imag();
    }

  // 3. CCS�s��̎l���̐ς̈ʑ������߂�D
    const u_int	v1 = height() - 1;
    setSign((*this)[0][0]   *= spectrum[0][0]);
    setSign((*this)[0][u1]  *= spectrum[0][u1]);
    setSign((*this)[v1][0]  *= spectrum[v1][0]);
    setSign((*this)[v1][u1] *= spectrum[v1][u1]);

    return *this;
}
    
//! ����CCS�z�񂪎��g���̈�ɂ���Ƃ��C����ɕʂ�CCS�z��̕��f�������|����D
/*!
  \param spectrum	���g���̈�ɂ���CCS�z��	
  \return		specturm�̕��f�������|������̂��̔z��
*/
template <class T> CCSImage<T>&
CCSImage<T>::operator *=(const CCSImage<T>& spectrum)
{
    using namespace	std;

    check_size(spectrum.size());
    
  // 1. CCS�s��̍ŏ��ƍŌ�̗�ȊO�̐ς����߂�D
    iterator	l = begin();
    for (const_iterator m = spectrum.begin(); m != spectrum.end(); ++m)
    {
	pixel_iterator	p = (l++)->begin() + 1;
	for (const_pixel_iterator q  = m->begin() + 1,
				  qe = m->end()	  - 1; q != qe; q += 2)
	{
	    complex<T>	val = complex<T>(*p, *(p+1)) * complex<T>(*q, -*(q+1));
	    *p++ = val.real();
	    *p++ = val.imag();
	}
    }

  // 2. CCS�s��̍ŏ��ƍŌ�̗�i�ŏ��ƍŌ�̍s�ȊO�j�̐ς����߂�D
    const u_int	u1 = width() - 1;
    l = begin() + 1;
    for (const_iterator m  = spectrum.begin() + 1;
			m != spectrum.end()   - 1; m += 2)
    {
      // �ŏ��̗�̗v�f�̎����Ƌ������܂� 2x1 �����s����Ƃ��Đς����߂�D
	complex<T>	val = complex<T>((*l)[0],  (*(l+1))[0])
			    * complex<T>((*m)[0], -(*(m+1))[0]);
	(*l)[0]     = val.real();
	(*(l+1))[0] = val.imag();
	
      // �Ō�̗�̗v�f�̎����Ƌ������܂� 2x1 �����s����Ƃ��Đς����߂�D
	val = complex<T>((*l)[u1],  (*(l+1))[u1])
	    * complex<T>((*m)[u1], -(*(m+1))[u1]);
	(*l++)[u1] = val.real();
	(*l++)[u1] = val.imag();
    }

  // 3. CCS�s��̎l���̐ς����߂�D
    const u_int	v1 = height() - 1;
    (*this)[0][0]   *= spectrum[0][0];
    (*this)[0][u1]  *= spectrum[0][u1];
    (*this)[v1][0]  *= spectrum[v1][0];
    (*this)[v1][u1] *= spectrum[v1][u1];

    return *this;
}

//! ����CCS�z�񂪋�ԗ̈�ɂ���Ƃ��C�z��v�f���̍ő�l�Ƃ��̈ʒu��Ԃ��D
/*!
  \param pMax	�ő�l��^����v�f�̈ʒu
  \return	�ő�l
*/
template <class T> T
CCSImage<T>::maximum(Point2<T>& pMax) const
{
    using namespace	std;
    
    T	valMax = numeric_limits<T>::min();

    for (u_int v = 0; v < height(); ++v)
    {
	const ImageLine<T>&	line = (*this)[v];
	for (u_int u = 0; u < width(); ++u)
	{
	    const T	val = line[u];
	    if (val > valMax)
	    {
		valMax	= val;
		pMax[0] = u;
		pMax[1] = v;
	    }
	}
    }
#if _DEBUG >= 3
    Image<T>	tmp(*this);
    tmp *= T(255) / maximum();
    tmp.save(cout, ImageBase::FLOAT);
    cerr << "CCSImage<T>::maximum()..." << endl;
#endif
#ifdef _DEBUG
    cerr << "CCSImage<T>::maximum(): " << valMax
	 << "@(" << pMax[0] << ", " << pMax[1] << ") in ["
	 << width() << 'x' << height() << ']' << endl;
#endif
    return valMax;
}
    
template <class T> T
CCSImage<T>::maximum() const
{
    using namespace	std;
    
    T	valMax = numeric_limits<T>::min();

    for (u_int v = 0; v < height(); ++v)
    {
	const ImageLine<T>&	line = (*this)[v];
	for (u_int u = 0; u < width(); ++u)
	{
	    const T	val = line[u];
	    if (val > valMax)
		valMax	= val;
	}
    }

    return valMax;
}
    
}
#endif	// !__TU_CCSIMAGE_H
