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
 *  $Id: Geometry++.h,v 1.31 2009-09-04 04:01:05 ueshiba Exp $
 */
#ifndef __TUGeometryPP_h
#define __TUGeometryPP_h

#include "TU/Vector++.h"
#include "TU/Normalize.h"

namespace TU
{
/************************************************************************
*  class Point2<T>							*
************************************************************************/
//! T�^�̍��W����������2�����_��\���N���X
/*!
  \param T	���W�̌^
 */
template <class T>
class Point2 : public Vector<T, FixedSizedBuf<T, 2> >
{
  private:
    typedef Vector<T, FixedSizedBuf<T, 2> >	array_type;
    
  public:
    Point2(T u=0, T v=0)						;

  //! ����2�����x�N�g���Ɠ���v�f������2�����_�����D
  /*!
    \param v	�R�s�[��2�����x�N�g��
  */
    template <class T2, class B2>
    Point2(const Vector<T2, B2>& v) :array_type(v)			{}

  //! ����2�����x�N�g���������ɑ������D
  /*!
    \param v	�R�s�[��2�����x�N�g��
    \return	����2�����_
  */
    template <class T2, class B2>
    Point2&	operator =(const Vector<T2, B2>& v)
		{
		    array_type::operator =(v);
		    return *this;
		}
    Point2	neighbor(int)					const	;
    Point2&	move(int)						;
    int		adj(const Point2&)				const	;
    int		dir(const Point2&)				const	;
    int		angle(const Point2&, const Point2&)		const	;
};

//! �w�肳�ꂽ���W����������2�����_�����D
/*!
  \param u	u���W
  \param v	v���W
*/
template <class T> inline
Point2<T>::Point2(T u, T v)
    :array_type()
{
    (*this)[0] = u;
    (*this)[1] = v;
}

//! �w�肳�ꂽ������8�ߖT�_��Ԃ��D
/*!
  \param dir	8�ߖT�_�̕���(mod 8�ŉ��߁D�E�ׂ�0�Ƃ��C���v����1�Â���)
  \return	8�ߖT�_
*/
template <class T> inline Point2<T>
Point2<T>::neighbor(int dir) const
{
    return Point2(*this).move(dir);
}

//! �w�肳�ꂽ������8�ߖT�_�Ɏ��g���ړ�����D
/*!
  \param dir	8�ߖT�_�̕���(mod 8�ŉ��߁D�E�ׂ�0�Ƃ��C���v����1�Â���)
  \return	�ړ���̂��̓_
*/
template <class T> Point2<T>&
Point2<T>::move(int dir)
{
    switch (dir % 8)
    {
      case 0:
	++(*this)[0];
	break;
      case 1:
      case -7:
	++(*this)[0];
	++(*this)[1];
	break;
      case 2:
      case -6:
	++(*this)[1];
	break;
      case 3:
      case -5:
	--(*this)[0];
	++(*this)[1];
	break;
      case 4:
      case -4:
	--(*this)[0];
	break;
      case 5:
      case -3:
	--(*this)[0];
	--(*this)[1];
	break;
      case 6:
      case -2:
	--(*this)[1];
	break;
      case 7:
      case -1:
	++(*this)[0];
	--(*this)[1];
	break;
    }
    return *this;
}

//! ����3�����_�Ǝw�肳�ꂽ3�����_��8�אڂ��Ă��邩���ׂ�D
/*!
  \param p	3�����_
  \return	p�ƈ�v���Ă����-1�C8�אڂ��Ă����1�C������ł��Ȃ����0
*/
template <class T> int
Point2<T>::adj(const Point2<T>& p) const
{
    const int	du = int(p[0] - (*this)[0]), dv = int(p[1] - (*this)[1]);

    if (du == 0 && dv == 0)
        return -1;
    switch (du)
    {
      case -1:
      case 0:
      case 1:
        switch (dv)
        {
          case -1:
          case 0:
          case 1:
            return 1;
          default:
            return 0;
        }
        break;
    }
    return 0;
}

//! ����3�����_����w�肳�ꂽ3�����_�ւ̌�����Ԃ��D
/*!
  \param p	3�����_
  \return	-180deg����180deg�܂ł�8����������Ԃ�\��-4����3�܂ł̐����l�D
		���ɁCp�����̓_�Ɉ�v����Ȃ��4
*/
template <class T> int
Point2<T>::dir(const Point2<T>& p) const
{
    const int	du = int(p[0] - (*this)[0]), dv = int(p[1] - (*this)[1]);

    if (du == 0 && dv == 0)
        return 4;
    if (dv >= 0)
        if (du > dv)
            return 0;
        else if (du > 0)
            return 1;
        else if (du > -dv)
            return 2;
        else if (dv > 0)
            return 3;
        else
            return -4;
    else
        if (du >= -dv)
            return -1;
        else if (du >= 0)
            return -2;
        else if (du >= dv)
            return -3;
        else
            return -4;
}

//! ����3�����_�Ǝw�肳�ꂽ2��3�����_���Ȃ��p�x��Ԃ��D
/*!
  \param pp	3�����_
  \param pn	3�����_
  \return	pp->*this->pn���Ȃ��p�x��-180deg����180deg�܂ł�8����������Ԃ�
		�\����-4����3�܂ł̐����l�D���ɁCpp, pn�̏��Ȃ��Ƃ���������̓_��
		��v����Ȃ��4
*/
template <class T> int
Point2<T>::angle(const Point2<T>& pp, const Point2<T>& pn) const
{
    int dp = pp.dir(*this), ang = dir(pn);
    
    if (dp == 4 || ang == 4)
        return 4;
    else if ((ang -= dp) > 3)
        return ang - 8;
    else if (ang < -4)
        return ang + 8;
    else
        return ang;
}

typedef Point2<short>	Point2s;		//!< short�^���W������2�����_
typedef Point2<int>	Point2i;		//!< int�^���W������2�����_
typedef Point2<float>	Point2f;		//!< float�^���W������2�����_
typedef Point2<double>	Point2d;		//!< double�^���W������2�����_

/************************************************************************
*  class Point3<T>							*
************************************************************************/
//! T�^�̍��W����������3�����_��\���N���X
/*!
  \param T	���W�̌^
 */
template <class T>
class Point3 : public Vector<T, FixedSizedBuf<T, 3> >
{
  private:
    typedef Vector<T, FixedSizedBuf<T, 3> >	array_type;
    
  public:
    Point3(T x=0, T y=0, T z=0)						;

  //! ����3�����x�N�g���Ɠ���v�f������3�����_�����D
  /*!
    \param v	�R�s�[��3�����x�N�g��
  */
    template <class T2, class B2>
    Point3(const Vector<T2, B2>& v) :array_type(v)			{}

  //! ����3�����x�N�g���������ɑ������D
  /*!
    \param v	�R�s�[��3�����x�N�g��
    \return	����3�����_
  */
    template <class T2, class B2>
    Point3&	operator =(const Vector<T2, B2>& v)
		{
		    array_type::operator =(v);
		    return *this;
		}
};

//! �w�肳�ꂽ���W����������3�����_�����D
/*!
  \param x	x���W
  \param y	y���W
  \param z	z���W
*/
template <class T> inline
Point3<T>::Point3(T x, T y, T z)
    :array_type()
{
    (*this)[0] = x;
    (*this)[1] = y;
    (*this)[2] = z;
}

typedef Point3<short>	Point3s;		//!< short�^���W������3�����_
typedef Point3<int>	Point3i;		//!< int�^���W������3�����_
typedef Point3<float>	Point3f;		//!< float�^���W������3�����_
typedef Point3<double>	Point3d;		//!< double�^���W������3�����_

/************************************************************************
*  class HyperPlane<T, B>						*
************************************************************************/
//! d�����ˉe��Ԓ��̒����ʂ�\������N���X
/*!
  d�����ˉe��Ԃ̓_\f$\TUud{x}{} \in \TUspace{R}{d+1}\f$�ɑ΂���
  \f$\TUtud{p}{}\TUud{x}{} = 0,~\TUud{p}{} \in \TUspace{R}{d+1}\f$
  �ɂ���ĕ\�����D
*/
template <class T, class B=Buf<T> >
class HyperPlane : public Vector<T, B>
{
  public:
    HyperPlane(u_int d=2)						;

  //! �������W�x�N�g�����w�肵�Ē����ʃI�u�W�F�N�g�𐶐�����D
  /*!
    \param p	(d+1)�����x�N�g���id�͒����ʂ����݂���ˉe��Ԃ̎����j
  */
    template <class T2, class B2>
    HyperPlane(const Vector<T2, B2>& p)	:Vector<T, B>(p)		{}

    template <class Iterator>
    HyperPlane(Iterator first, Iterator last)				;

  //! �����ʃI�u�W�F�N�g�̓������W�x�N�g�����w�肷��D
  /*!
    \param v	(d+1)�����x�N�g���id�͒����ʂ����݂���ˉe��Ԃ̎����j
    \return	���̒����ʃI�u�W�F�N�g
  */
    template <class T2, class B2>
    HyperPlane&	operator =(const Vector<T2, B2>& v)
				{Vector<T, B>::operator =(v); return *this;}

    template <class Iterator>
    void	fit(Iterator first, Iterator last)			;

  //! ���̒����ʂ����݂���ˉe��Ԃ̎�����Ԃ��D
  /*! 
    \return	�ˉe��Ԃ̎���(�������W�̃x�N�g���Ƃ��Ă̎�����#spaceDim()+1)
  */
    u_int	spaceDim()		const	{return Vector<T, B>::dim()-1;}

  //! �����ʂ����߂邽�߂ɕK�v�ȓ_�̍ŏ�����Ԃ��D
  /*!
    ���ݐݒ肳��Ă���ˉe��Ԃ̎��������ƂɌv�Z�����D
    \return	�K�v�ȓ_�̍ŏ������Ȃ킿���͋�Ԃ̎���#spaceDim()
  */
    u_int	ndataMin()		const	{return spaceDim();}

    template <class T2, class B2> inline T
    sqdist(const Vector<T2, B2>& x)		const	;
    template <class T2, class B2> inline double
    dist(const Vector<T2, B2>& x)		const	;
};

//! ��Ԃ̎������w�肵�Ē����ʃI�u�W�F�N�g�𐶐�����D
/*!
  ������������([0, 0,..., 0, 1])�ɏ����������D
  \param d	���̒����ʂ����݂���ˉe��Ԃ̎���
*/
template <class T, class B> inline
HyperPlane<T, B>::HyperPlane(u_int d)
    :Vector<T, B>(d + 1)
{
    (*this)[d] = 1;
}
    
//! �^����ꂽ�_��̔񓯎����W�ɓ��Ă͂߂�ꂽ�����ʃI�u�W�F�N�g�𐶐�����D
/*!
  \param first			�_��̐擪�����������q
  \param last			�_��̖��������������q
  \throw std::invalid_argument	�_�̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class T, class B> template <class Iterator> inline
HyperPlane<T, B>::HyperPlane(Iterator first, Iterator last)
{
    fit(first, last);
}

//! �^����ꂽ�_��̔񓯎����W�ɒ����ʂ𓖂Ă͂߂�D
/*!
  \param first			�_��̐擪�����������q
  \param last			�_��̖��������������q
  \throw std::invalid_argument	�_�̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class T, class B> template <class Iterator> void
HyperPlane<T, B>::fit(Iterator first, Iterator last)
{
  // �_��̐��K��
    const Normalize	normalize(first, last);

  // �[���Ȍ��̓_�����邩�H
    const u_int		ndata = std::distance(first, last),
			d     = normalize.spaceDim();
    if (ndata < d)	// Vector<T, B>�̃T�C�Y������Ȃ̂�ndataMin()�͖���
	throw std::invalid_argument("Hyperplane::initialize(): not enough input data!!");

  // �f�[�^�s��̌v�Z
    Matrix<T>	A(d, d);
    while (first != last)
    {
	const Vector<T>&	x = normalize(*first++);
	A += x % x;
    }

  // �f�[�^�s��̍ŏ��ŗL�l�ɑΉ�����ŗL�x�N�g������@���x�N�g�����v�Z���C
  // ����ɓ_��̏d�S��茴�_����̋������v�Z����D
    Vector<T>		eval;
    const Matrix<T>&	Ut = A.eigen(eval);
    Vector<T, B>::resize(d+1);
    (*this)(0, d) = Ut[Ut.nrow()-1];
    (*this)[d] = -((*this)(0, d)*normalize.centroid());
    if ((*this)[d] > 0.0)
	Vector<T, B>::operator *=(-1.0);
}

//! �^����ꂽ�_�ƒ����ʂ̋�����2���Ԃ��D
/*!
  \param x	�_�̔񓯎����W�i#spaceDim()�����j�܂��͓������W
		�i#spaceDim()+1�����j
  \return	�_�ƒ����ʂ̋�����2��
*/
template <class T, class B> template <class T2, class B2> inline T
HyperPlane<T, B>::sqdist(const Vector<T2, B2>& x) const
{
    const double	d = dist(x);
    return d*d;
}

//! �^����ꂽ�_�ƒ����ʂ̋�����Ԃ��D
/*!
  \param x			�_�̔񓯎����W�i#spaceDim()�����j�܂���
				�������W�i#spaceDim()+1�����j
  \return			�_�ƒ����ʂ̋����i�񕉁j
  \throw std::invalid_argument	�_�̃x�N�g���Ƃ��Ă̎�����#spaceDim()�C
				#spaceDim()+1�̂�����ł��Ȃ��ꍇ�C��������
				���̓_���������_�ł���ꍇ�ɑ��o�D
*/
template <class T, class B> template <class T2, class B2> double
HyperPlane<T, B>::dist(const Vector<T2, B2>& x) const
{
    const Vector<T2>&	p = (*this)(0, spaceDim());
    if (x.dim() == spaceDim())
	return (p * x + (*this)[spaceDim()]) / p.length();
    else if (x.dim() == spaceDim() + 1)
    {
	if (x[spaceDim()] == 0.0)
	    throw std::invalid_argument("HyperPlane::dist(): point at infinitiy!!");
	return (*this * x) / (p.length() * x[spaceDim()]);
    }
    else
	throw std::invalid_argument("HyperPlane::dist(): dimension mismatch!!");

    return 0;
}

typedef HyperPlane<float,  FixedSizedBuf<float,  3> >
	LineP2f;			//!< float�^���W������2������Ԓ��̒���
typedef HyperPlane<double, FixedSizedBuf<double, 3> >
	LineP2d;			//!< double�^���W������2������Ԓ��̒���
typedef HyperPlane<float,  FixedSizedBuf<float,  4> >
	PlaneP3f;			//!< float�^���W������3������Ԓ��̕���
typedef HyperPlane<double, FixedSizedBuf<double, 4> >
	PlaneP3d;			//!< double�^���W������3������Ԓ��̕���
}
#endif	/* !__TUGeometryPP_h */
