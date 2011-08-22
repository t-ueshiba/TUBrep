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
 *  $Id: IntegralImage.h,v 1.7 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		IntegralImage.h
  \brief	�N���X TU::IntegralImage, TU::DiagonalIntegralImage �̒�`�Ǝ���
*/
#ifndef	__TUIntegralImage_h
#define	__TUIntegralImage_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class IntegralImage<T>						*
************************************************************************/
//! �ϕ��摜(integral image)��\���N���X
template <class T>
class IntegralImage : public Image<T>
{
  public:
    IntegralImage()							;
    template <class S, class B>
    IntegralImage(const Image<S, B>& image)				;

    template <class S, class B> IntegralImage&
		initialize(const Image<S, B>& image)			;
    T		crop(int u, int v, int w, int h)		const	;
    T		crop2(int umin, int umax, int vmin, int vmax)	const	;
    T		crossVal(int u, int v, int cropSize)		const	;
    template <class S, class B> const IntegralImage&
		crossVal(Image<S, B>& out, int cropSize)	const	;

    u_int	originalWidth()					const	;
    u_int	originalHeight()				const	;
    
    using	Image<T>::width;
    using	Image<T>::height;
};

//! ��̐ϕ��摜�����
template <class T> inline
IntegralImage<T>::IntegralImage()
{
}
    
//! �^����ꂽ�摜����ϕ��摜�����
/*!
  \param image		���͉摜
*/
template <class T> template <class S, class B> inline
IntegralImage<T>::IntegralImage(const Image<S, B>& image)
{
    initialize(image);
}
    
//! �^����ꂽ�摜����ϕ��摜�����
/*!
  \param image		���͉摜
  \return		���̐ϕ��摜
*/
template <class T> template <class S, class B> IntegralImage<T>&
IntegralImage<T>::initialize(const Image<S, B>& image)
{
  // ���摜������ƍ��ɂP�s�N�Z�����傫���T�C�Y��p��
    resize(image.height() + 1, image.width() + 1);

  // ��ƍ��ɗ]��������
    (*this)[0] = 0;				// 0�s�ڂ͂��ׂ�0
    for (u_int v = 1; v < height(); ++v)
    {
	T*		dst = (*this)[v];
	*dst = 0;				// 0��ڂ�0

	const S*	src = image[v-1];
	T		val = 0;		// ���̍s�̂��̉�f�܂ł̘a
	const T*	prv = (*this)[v-1];	// 1�s��
	const T* const	end = dst + width();
	while (++dst < end)
	    *dst = (val += *src++) + *(++prv);
    }

    return *this;
}

//! ���摜�ɐݒ肵�������`�E�B���h�E���̉�f�l�̑��a��Ԃ�
/*!
  \param u		�E�B���h�E�̍�����̉����W
  \param v		�E�B���h�E�̍�����̏c���W
  \param w		�E�B���h�E�̕�
  \param h		�E�B���h�E�̍���
  \return		�E�B���h�E���̉�f�l�̑��a
*/
template <class T> inline T
IntegralImage<T>::crop(int u, int v, int w, int h) const
{
    const int	u1 = std::min(u + w, int(width())  - 1),
		v1 = std::min(v + h, int(height()) - 1);
    if (u >= int(width()) || v >= int(height()) || u1 < 0 || v1 < 0)
	return 0;
    if (u < 0)
	u = 0;
    if (v < 0)
	v = 0;
    
    return (*this)[v1][u1] + (*this)[v ][u]
	 - (*this)[v ][u1] - (*this)[v1][u];
}

//! ���摜�ɐݒ肵�������`�E�B���h�E���̉�f�l�̑��a��Ԃ�
/*!
  \param umin		�E�B���h�E�̍�����̉����W
  \param umax		�E�B���h�E�̉E�����̉����W
  \param vmin		�E�B���h�E�̍�����̏c���W
  \param vmax		�E�B���h�E�̉E�����̏c���W
  \return		�E�B���h�E���̉�f�l�̑��a
*/
template <class T> inline T
IntegralImage<T>::crop2(int umin, int umax, int vmin, int vmax) const
{
    ++umax;
    ++vmax;

    return (*this)[vmax][umax] + (*this)[vmin][umin]
	 - (*this)[vmin][umax] - (*this)[vmax][umin];
}

//! ���摜�ɐ����`�̓�l�\���e���v���[�g��K�p�����l��Ԃ�
/*!
  \param u		�e���v���[�g���S�̉����W
  \param v		�e���v���[�g���S�̏c���W
  \param cropSize	�e���v���[�g�͈�� 2*cropSize + 1 �̐����`
  \return		�e���v���[�g��K�p�����l
*/
template <class T> inline T
IntegralImage<T>::crossVal(int u, int v, int cropSize) const
{
    return crop(u+1,	    v+1,	cropSize, cropSize)
	 - crop(u-cropSize, v+1,	cropSize, cropSize)
	 + crop(u-cropSize, v-cropSize, cropSize, cropSize)
	 - crop(u+1,	    v-cropSize, cropSize, cropSize);
}
    
//! ���摜�̑S�Ă̓_�ɐ����`�̓�l�\���e���v���[�g��K�p�����摜�����߂�
/*!
  \param out		���摜�Ƀe���v���[�g��K�p�����o�͉摜
  \param cropSize	�e���v���[�g�T�C�Y���w�肷��p�����[�^
			�e���v���[�g�͈�� 2*cropSize+1 �̐����`
  \return		���̐ϕ��摜
*/
template <class T> template <class S, class B> const IntegralImage<T>&
IntegralImage<T>::crossVal(Image<S, B>& out, int cropSize) const
{
    out.resize(originalHeight(), originalWidth());
    for (u_int v = 0; v < out.height(); ++v)
	for (u_int u = 0; u < out.width(); ++u)
	    out[v][u] = crossVal(u, v, cropSize);

    return *this;
}

//! ���摜�̕���Ԃ�
/*!
  �ϕ��摜���̂̕��ƍ����͂��ꂼ�ꌴ�摜����1�����傫���̂ł��̕����������l��Ԃ��D
  \return	���摜�̕�
*/
template <class T> u_int
IntegralImage<T>::originalWidth() const
{
    return width() - 1;
}

//! ���摜�̍�����Ԃ�
/*!
  �ϕ��摜���̂̕��ƍ����͂��ꂼ�ꌴ�摜����1�����傫���̂ł��̕����������l��Ԃ��D
  \return	���摜�̍���
*/
template <class T> u_int
IntegralImage<T>::originalHeight() const
{
    return height() - 1;
}

/************************************************************************
*  class DiagonalIntegralImage<T>					*
************************************************************************/
//! �Ίp�ϕ��摜(diagonal integral image)��\���N���X
template <class T>
class DiagonalIntegralImage : public Image<T>
{
  public:
    DiagonalIntegralImage()						;
    template <class S, class B>
    DiagonalIntegralImage(const Image<S, B>& image)			;

    template <class S, class B> DiagonalIntegralImage&
		initialize(const Image<S, B>& image)			;
    T		crop(int u, int v, int w, int h)		const	;
    T		crossVal(int u, int v, int cropSize)		const	;
    template <class S, class B> const DiagonalIntegralImage&
		crossVal(Image<S, B>& out, int cropSize)	const	;

    u_int	originalWidth()					const	;
    u_int	originalHeight()				const	;
    
    using	Image<T>::width;
    using	Image<T>::height;

  private:
    void	correct(int& u, int& v)				const	;
};

//! ��̑Ίp�ϕ��摜�����
template <class T> inline
DiagonalIntegralImage<T>::DiagonalIntegralImage()
{
}
    
//! �^����ꂽ�摜����Ίp�ϕ��摜�����
/*!
  \param image		���͉摜
*/
template <class T> template <class S, class B> inline
DiagonalIntegralImage<T>::DiagonalIntegralImage(const Image<S, B>& image)
{
    initialize(image);
}
    
//! �^����ꂽ�摜����Ίp�ϕ��摜�����
/*!
  \param image		���͉摜
  \return		���̑Ίp�ϕ��摜
*/
template <class T> template <class S, class B> DiagonalIntegralImage<T>&
DiagonalIntegralImage<T>::initialize(const Image<S, B>& image)
{
    resize(image.height(), image.width());
    
    Array<T>	K(width() + height() - 1), L(width() + height() - 1);
    for (u_int i = 0; i < K.dim(); ++i)
	K[i] = L[i] = 0;
    
    for (u_int v = 0; v < height(); ++v)
    {
	const S*	src = image[v];
	T		*dst = (*this)[v],
			*kp = &K[height() - 1 - v], *lp = &L[v];
	if (v == 0)
	    for (const T* const end = dst + width(); dst < end; )
		*dst++ = *kp++ = *lp++ = *src++;
	else
	{
	    const T*	prv = (*this)[v-1];
	    for (const T* const end = dst + width(); dst < end; )
	    {
		*dst++ = *src + *kp + *lp + *prv++;
		*kp++ += *src;
		*lp++ += *src++;
	    }
	}
    }

    return *this;
}

//! ���摜��45�x�X���Đݒ肵�������`�E�B���h�E���̉�f�l�̑��a��Ԃ�
/*!
  \param u		�E�B���h�E�̏���̉����W
  \param v		�E�B���h�E�̏���̏c���W
  \param w		�E�B���h�E�̕�
  \param h		�E�B���h�E�̍���
  \return		�E�B���h�E���̉�f�l�̑��a
*/
template <class T> T
DiagonalIntegralImage<T>::crop(int u, int v, int w, int h) const
{
    --v;
    int		ul = u - h, vl = v + h, ur = u + w, vr = v + w,
		ut = u + w - h, vt = v + w + h;
    correct(u,  v);
    correct(ul, vl);
    correct(ur, vr);
    correct(ut, vt);
    if (vt >= int(height()))
	return 0;
    return (v  >= 0 ? (*this)[v][u]   : 0)
	 + (vt >= 0 ? (*this)[vt][ut] : 0)
	 - (vl >= 0 ? (*this)[vl][ul] : 0)
	 - (vr >= 0 ? (*this)[vr][ur] : 0);
}

//! ���摜�ɐ����`�̓�l�N���X�e���v���[�g��K�p�����l��Ԃ�
/*!
  \param u		�e���v���[�g���S�̉����W
  \param v		�e���v���[�g���S�̏c���W
  \param cropSize	�e���v���[�g�͈�� 2*cropSize + 1 �̐����`
  \return		�e���v���[�g��K�p�����l
*/
template <class T> inline T
DiagonalIntegralImage<T>::crossVal(int u, int v, int cropSize) const
{
    return crop(u+cropSize+1, v-cropSize+1, cropSize, cropSize)
	 - crop(u,	      v+2,	    cropSize, cropSize)
	 + crop(u-cropSize-1, v-cropSize+1, cropSize, cropSize)
	 - crop(u,	      v-2*cropSize, cropSize, cropSize);
}

//! ���摜�̑S�Ă̓_�ɐ����`�̓�l�N���X�e���v���[�g��K�p�����摜�����߂�
/*!
  \param out		���摜�Ƀe���v���[�g��K�p�����o�͉摜
  \param cropSize	�e���v���[�g�T�C�Y���w�肷��p�����[�^
			�e���v���[�g�͈�� 2*cropSize+1 �̐����`
  \return		���̑Ίp�ϕ��摜
*/
template <class T> template <class S, class B> const DiagonalIntegralImage<T>&
DiagonalIntegralImage<T>::crossVal(Image<S, B>& out, int cropSize) const
{
    out.resize(height(), width());
    for (u_int v = 0; v < out.height() - 2*cropSize - 1; ++v)
	for (u_int u = 0; u < out.width(); ++u)
	    out[v][u] = crossVal(u, v, cropSize);

    return *this;
}

//! ���摜�̕���Ԃ�
/*!
  �Ίp�ϕ��摜���̂̕��ƍ����͌��摜�Ɠ����ł���D
  \return	���摜�̕�
*/
template <class T> u_int
DiagonalIntegralImage<T>::originalWidth() const
{
    return width();
}

//! ���摜�̍�����Ԃ�
/*!
  �Ίp�ϕ��摜���̂̕��ƍ����͌��摜�Ɠ����ł���D
  \return	���摜�̍���
*/
template <class T> u_int
DiagonalIntegralImage<T>::originalHeight() const
{
    return height();
}

template <class T> inline void
DiagonalIntegralImage<T>::correct(int& u, int& v) const
{
    if (u < 0)
    {
	v += u;
	u  = 0;
    }
    else if (u >= int(width()))
    {
	v += (int(width()) - 1 - u);
	u  = width() - 1;
    }
}

}

#endif	/* !__TUIntegralImage_h */
