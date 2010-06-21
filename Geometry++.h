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
 *  $Id: Geometry++.h,v 1.40 2010-06-21 02:38:52 ueshiba Exp $
 */
#ifndef __TUGeometryPP_h
#define __TUGeometryPP_h

#include "TU/utility.h"
#include "TU/Vector++.h"
#include "TU/Normalize.h"
#include "TU/Minimize.h"
#include <limits>

namespace TU
{
/************************************************************************
*  class Point1<T>							*
************************************************************************/
//! T�^�̍��W����������1�����_��\���N���X
/*!
  \param T	���W�̌^
 */
template <class T>
class Point1 : public Vector<T, FixedSizedBuf<T, 1> >
{
  private:
    typedef Vector<T, FixedSizedBuf<T, 1> >	vector_type;
    
  public:
    Point1(T u=0)							;

  //! ����1�����x�N�g���Ɠ���v�f������1�����_�����D
  /*!
    \param v	�R�s�[��1�����x�N�g��
  */
    template <class T2, class B2>
    Point1(const Vector<T2, B2>& v) :vector_type(v)			{}

  //! ����1�����x�N�g���������ɑ������D
  /*!
    \param v	�R�s�[��1�����x�N�g��
    \return	����1�����_
  */
    template <class T2, class B2>
    Point1&	operator =(const Vector<T2, B2>& v)
		{
		    vector_type::operator =(v);
		    return *this;
		}
};

//! �w�肳�ꂽ���W����������1�����_�����D
/*!
  \param u	u���W
*/
template <class T> inline
Point1<T>::Point1(T u)
    :vector_type()
{
    (*this)[0] = u;
}

typedef Point1<short>	Point1s;		//!< short�^���W������1�����_
typedef Point1<int>	Point1i;		//!< int�^���W������1�����_
typedef Point1<float>	Point1f;		//!< float�^���W������1�����_
typedef Point1<double>	Point1d;		//!< double�^���W������1�����_

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
    typedef Vector<T, FixedSizedBuf<T, 2> >	vector_type;
    
  public:
    Point2(T u=0, T v=0)						;

  //! ����2�����x�N�g���Ɠ���v�f������2�����_�����D
  /*!
    \param v	�R�s�[��2�����x�N�g��
  */
    template <class T2, class B2>
    Point2(const Vector<T2, B2>& v) :vector_type(v)			{}

  //! ����2�����x�N�g���������ɑ������D
  /*!
    \param v	�R�s�[��2�����x�N�g��
    \return	����2�����_
  */
    template <class T2, class B2>
    Point2&	operator =(const Vector<T2, B2>& v)
		{
		    vector_type::operator =(v);
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
    :vector_type()
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

//! ����2�����_�Ǝw�肳�ꂽ2�����_��8�אڂ��Ă��邩���ׂ�D
/*!
  \param p	2�����_
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

//! ����2�����_����w�肳�ꂽ2�����_�ւ̌�����Ԃ��D
/*!
  \param p	2�����_
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

//! ����2�����_�Ǝw�肳�ꂽ2��2�����_���Ȃ��p�x��Ԃ��D
/*!
  \param pp	2�����_
  \param pn	2�����_
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
    typedef Vector<T, FixedSizedBuf<T, 3> >	vector_type;
    
  public:
    Point3(T x=0, T y=0, T z=0)						;

  //! ����3�����x�N�g���Ɠ���v�f������3�����_�����D
  /*!
    \param v	�R�s�[��3�����x�N�g��
  */
    template <class T2, class B2>
    Point3(const Vector<T2, B2>& v) :vector_type(v)			{}

  //! ����3�����x�N�g���������ɑ������D
  /*!
    \param v	�R�s�[��3�����x�N�g��
    \return	����3�����_
  */
    template <class T2, class B2>
    Point3&	operator =(const Vector<T2, B2>& v)
		{
		    vector_type::operator =(v);
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
    :vector_type()
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
*  class HyperPlane<V>							*
************************************************************************/
//! d�����ˉe��Ԓ��̒����ʂ�\������N���X
/*!
  d�����ˉe��Ԃ̓_\f$\TUud{x}{} \in \TUspace{R}{d+1}\f$�ɑ΂���
  \f$\TUtud{p}{}\TUud{x}{} = 0,~\TUud{p}{} \in \TUspace{R}{d+1}\f$
  �ɂ���ĕ\�����D
*/
template <class V>
class HyperPlane : public V
{
  public:
    typedef V						vector_type;
    typedef typename V::value_type			value_type;
    
  public:
    HyperPlane(u_int d=2)				;

  //! �������W�x�N�g�����w�肵�Ē����ʃI�u�W�F�N�g�𐶐�����D
  /*!
    \param p	(d+1)�����x�N�g���id�͒����ʂ����݂���ˉe��Ԃ̎����j
  */
    template <class T, class B>
    HyperPlane(const Vector<T, B>& p)	:V(p)		{}

    template <class Iterator>
    HyperPlane(Iterator begin, Iterator end)		;

    using	V::dim;

  //! �����ʃI�u�W�F�N�g�̓������W�x�N�g�����w�肷��D
  /*!
    \param v	(d+1)�����x�N�g���id�͒����ʂ����݂���ˉe��Ԃ̎����j
    \return	���̒����ʃI�u�W�F�N�g
  */
    template <class T, class B>
    HyperPlane&	operator =(const Vector<T, B>& v)	{V::operator =(v);
							 return *this;}

    template <class Iterator>
    void	fit(Iterator begin, Iterator end)	;

  //! ���̒����ʂ����݂���ˉe��Ԃ̎�����Ԃ��D
  /*! 
    \return	�ˉe��Ԃ̎���(�������W�̃x�N�g���Ƃ��Ă̎�����#spaceDim()+1)
  */
    u_int	spaceDim()			const	{return dim()-1;}

  //! �����ʂ����߂邽�߂ɕK�v�ȓ_�̍ŏ�����Ԃ��D
  /*!
    ���ݐݒ肳��Ă���ˉe��Ԃ̎��������ƂɌv�Z�����D
    \return	�K�v�ȓ_�̍ŏ������Ȃ킿���͋�Ԃ̎���#spaceDim()
  */
    u_int	ndataMin()			const	{return spaceDim();}

    template <class T, class B> inline value_type
    sqdist(const Vector<T, B>& x)		const	;
    template <class T, class B> inline value_type
    dist(const Vector<T, B>& x)			const	;
};

//! ��Ԃ̎������w�肵�Ē����ʃI�u�W�F�N�g�𐶐�����D
/*!
  ������������([0, 0,..., 0, 1])�ɏ����������D
  \param d	���̒����ʂ����݂���ˉe��Ԃ̎���
*/
template <class V> inline
HyperPlane<V>::HyperPlane(u_int d)
    :V(d + 1)
{
    (*this)[d] = 1;
}
    
//! �^����ꂽ�_��̔񓯎����W�ɓ��Ă͂߂�ꂽ�����ʃI�u�W�F�N�g�𐶐�����D
/*!
  \param begin			�_��̐擪�����������q
  \param end			�_��̖��������������q
  \throw std::invalid_argument	�_�̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class V> template <class Iterator> inline
HyperPlane<V>::HyperPlane(Iterator begin, Iterator end)
{
    fit(begin, end);
}

//! �^����ꂽ�_��̔񓯎����W�ɒ����ʂ𓖂Ă͂߂�D
/*!
  \param begin			�_��̐擪�����������q
  \param end			�_��̖��������������q
  \throw std::invalid_argument	�_�̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class V> template <class Iterator> void
HyperPlane<V>::fit(Iterator begin, Iterator end)
{
  // �_��̐��K��
    const Normalize	normalize(begin, end);

  // �[���Ȍ��̓_�����邩�H
    const u_int		ndata = std::distance(begin, end),
			d     = normalize.spaceDim();
    if (ndata < d)	// V�̃T�C�Y������Ȃ̂�ndataMin()�͖���
	throw std::invalid_argument("Hyperplane::initialize(): not enough input data!!");

  // �f�[�^�s��̌v�Z
    Matrix<value_type>	A(d, d);
    while (begin != end)
    {
	const Vector<double>&	x = normalize(*begin++);
	A += x % x;
    }

  // �f�[�^�s��̍ŏ��ŗL�l�ɑΉ�����ŗL�x�N�g������@���x�N�g�����v�Z���C
  // ����ɓ_��̏d�S��茴�_����̋������v�Z����D
    Vector<value_type>		eval;
    const Matrix<value_type>&	Ut = A.eigen(eval);
    V::resize(d+1);
    (*this)(0, d) = Ut[Ut.nrow()-1];
    (*this)[d] = -((*this)(0, d)*normalize.centroid());
    if ((*this)[d] > 0.0)
	*this *= -1.0;
}

//! �^����ꂽ�_�ƒ����ʂ̋�����2���Ԃ��D
/*!
  \param x	�_�̔񓯎����W�i#spaceDim()�����j�܂��͓������W
		�i#spaceDim()+1�����j
  \return	�_�ƒ����ʂ̋�����2��
*/
template <class V> template <class T, class B>
inline typename HyperPlane<V>::value_type
HyperPlane<V>::sqdist(const Vector<T, B>& x) const
{
    const value_type	d = dist(x);
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
template <class V> template <class T, class B>
typename HyperPlane<V>::value_type
HyperPlane<V>::dist(const Vector<T, B>& x) const
{
    const Vector<value_type>&	p = (*this)(0, spaceDim());
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

typedef HyperPlane<Vector3f>	LineP2f;
typedef HyperPlane<Vector3d>	LineP2d;
typedef HyperPlane<Vector4f>	PlaneP3f;
typedef HyperPlane<Vector4d>	PlaneP3d;

/************************************************************************
*  class Projectivity<M>						*
************************************************************************/
//! �ˉe�ϊ����s���N���X
/*!
  \f$\TUvec{T}{} \in \TUspace{R}{(n+1)\times(m+1)}\f$��p����m������Ԃ̓_
  \f$\TUud{x}{} \in \TUspace{R}{m+1}\f$��n������Ԃ̓_
  \f$\TUud{y}{} \simeq \TUvec{T}{}\TUud{x}{} \in \TUspace{R}{n+1}\f$
  �Ɏʂ��i\f$m \neq n\f$�ł��\��Ȃ��j�D
*/
template <class M>
class Projectivity : public M
{
  public:
    typedef M				matrix_type;
    typedef typename M::value_type	value_type;

    Projectivity()							;
    Projectivity(u_int inDim, u_int outDim)				;

  //! �ϊ��s����w�肵�Ďˉe�ϊ��I�u�W�F�N�g�𐶐�����D
  /*!
    \param T			(m+1)x(n+1)�s��im, n�͓��́^�o�͋�Ԃ̎����j
  */
    template <class S, class B, class R>
    Projectivity(const Matrix<S, B, R>& T) :M(T)			{}

    template <class Iterator>
    Projectivity(Iterator begin, Iterator end, bool refine=false)	;

    using	M::nrow;
    using	M::ncol;

  //! �ϊ��s����w�肷��D
  /*!
    \param T			(m+1)x(n+1)�s��im, n�͓��́^�o�͋�Ԃ̎����j
  */
    template <class S, class B, class R>
    void	set(const Matrix<S, B, R>& T)		{M::operator =(T);}
    
    template <class Iterator>
    void	fit(Iterator begin, Iterator end, bool refine=false)	;

  //! ���̎ˉe�ϊ��̓��͋�Ԃ̎�����Ԃ��D
  /*! 
    \return	���͋�Ԃ̎���(�������W�̃x�N�g���Ƃ��Ă̎�����#inDim()+1)
  */
    u_int	inDim()				const	{return ncol()-1;}

  //! ���̎ˉe�ϊ��̏o�͋�Ԃ̎�����Ԃ��D
  /*! 
    \return	�o�͋�Ԃ̎���(�������W�̃x�N�g���Ƃ��Ă̎�����#outDim()+1)
  */
    u_int	outDim()			const	{return nrow()-1;}

    u_int	ndataMin()			const	;

    Projectivity	inv()			const	;
    template <class S, class B>
    Vector<value_type>	operator ()(const Vector<S, B>& x)	const	;
    template <class S, class B>
    Vector<value_type>	mapP(const Vector<S, B>& x)		const	;
    template <class S, class B>
    Matrix<value_type>	jacobian(const Vector<S, B>& x)		const	;

    template <class In, class Out>
    value_type	sqdist(const std::pair<In, Out>& pair)		const	;
    template <class In, class Out>
    value_type	dist(const std::pair<In, Out>& pair)		const	;
    u_int	nparams()					const	;
    void	update(const Vector<value_type>& dt)			;

    template <class Iterator>
    value_type	reprojectionError(Iterator begin, Iterator end)	const	;
    
  protected:
  //! �ˉe�ϊ��s��̍Ŗސ���̂��߂̃R�X�g�֐�
    template <class Map, class Iterator>
    class Cost
    {
      public:
	typedef typename Map::value_type	value_type;
	
	Cost(Iterator begin, Iterator end)					;

	Vector<value_type>	operator ()(const Map& map)		const	;
	Matrix<value_type>	jacobian(const Map& map)		const	;
	static void		update(Map& map, const Vector<value_type>& dm)	;

      private:
	const Iterator	_begin, _end;
	const u_int	_npoints;
    };
};

//! �ˉe�ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  �P���ϊ��Ƃ��ď����������D
*/
template <class M>
Projectivity<M>::Projectivity()
    :M()
{
    if (nrow() > 0 && ncol() > 0)
    {
	u_int	n = std::min(ncol() - 1, nrow() - 1);
	for (u_int i = 0; i < n; ++i)
	    (*this)[i][i] = 1.0;
	(*this)[nrow() - 1][ncol() - 1] = 1.0;
    }
}
    
//! ���͋�ԂƏo�͋�Ԃ̎������w�肵�Ďˉe�ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  �P���ϊ��Ƃ��ď����������D
  \param inDim	���͋�Ԃ̎���
  \param outDim	�o�͋�Ԃ̎���
*/
template <class M>
Projectivity<M>::Projectivity(u_int inDim, u_int outDim)
    :M(outDim + 1, inDim + 1)
{
    u_int	n = std::min(inDim, outDim);
    for (u_int i = 0; i < n; ++i)
	(*this)[i][i] = 1.0;
    (*this)[outDim][inDim] = 1.0;
}
    
//! �^����ꂽ�_�Η�̔񓯎����W����ˉe�ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  \param begin			�_�Η�̐擪�����������q
  \param end			�_�Η�̖��������������q
  \param refine			����^�œK���̗L(true)�^��(false)���w��
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class M> template <class Iterator> inline
Projectivity<M>::Projectivity(Iterator begin, Iterator end, bool refine)
{
    fit(begin, end, refine);
}

//! �^����ꂽ�_�Η�̔񓯎����W����ˉe�ϊ����v�Z����D
/*!
  \param begin			�_�Η�̐擪�����������q
  \param end			�_�Η�̖��������������q
  \param refine			����^�œK���̗L(true)�^��(false)���w��
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template <class M> template <class Iterator> void
Projectivity<M>::fit(Iterator begin, Iterator end, bool refine)
{
  // �_��̐��K��
    const Normalize	xNormalize(make_const_first_iterator(begin),
				   make_const_first_iterator(end)),
			yNormalize(make_const_second_iterator(begin),
				   make_const_second_iterator(end));

  // �[���Ȍ��̓_�΂����邩�H
    const u_int		ndata = std::distance(begin, end);
    const u_int		xdim1 = xNormalize.spaceDim() + 1,
			ydim  = yNormalize.spaceDim();
    if (ndata*ydim < xdim1*(ydim + 1) - 1)	// �s��̃T�C�Y������Ȃ̂�
						// ndataMin()�͖���
	throw std::invalid_argument("Projectivity::fit(): not enough input data!!");

  // �f�[�^�s��̌v�Z
    Matrix<value_type>	A(xdim1*(ydim + 1), xdim1*(ydim + 1));
    for (Iterator iter = begin; iter != end; ++iter)
    {
	const Vector<double>&	x  = xNormalize.normalizeP(iter->first);
	const Vector<double>&	y  = yNormalize(iter->second);
	const Matrix<double>&	xx = x % x;
	A(0, 0, xdim1, xdim1) += xx;
	for (u_int j = 0; j < ydim; ++j)
	    A(ydim*xdim1, j*xdim1, xdim1, xdim1) -= y[j] * xx;
	A(ydim*xdim1, ydim*xdim1, xdim1, xdim1) += (y*y) * xx;
    }
    for (u_int j = 1; j < ydim; ++j)
	A(j*xdim1, j*xdim1, xdim1, xdim1) = A(0, 0, xdim1, xdim1);
    A.symmetrize();

  // �f�[�^�s��̍ŏ��ŗL�l�ɑΉ�����ŗL�x�N�g������ϊ��s����v�Z���C
  // ���K�����L�����Z������D
    Vector<value_type>	eval;
    Matrix<value_type>	Ut = A.eigen(eval);
    *this = yNormalize.Tinv()
	  * Matrix<value_type>((value_type*)Ut[Ut.nrow()-1], ydim + 1, xdim1)
	  * xNormalize.T();

  // �ϊ��s�񂪐����Ȃ�΁C���̍s�񎮂��P�ɂȂ�悤�ɐ��K������D
    if (nrow() == ncol())
    {
	value_type	d = M::det();
	if (d > 0)
	    *this /=  pow( d, value_type(1.0/nrow()));
	else
	    *this /= -pow(-d, value_type(1.0/nrow()));
    }

  // ����^�œK�����s���D
    if (refine)
    {
	Cost<Projectivity<M>, Iterator>		cost(begin, end);
	ConstNormConstraint<Projectivity<M> >	constraint(*this);
	minimizeSquare(cost, constraint, *this);
    }
}

//! ���̎ˉe�ϊ��̋t�ϊ���Ԃ��D
/*!
  \return	�t�ϊ�
*/
template <class M> inline Projectivity<M>
Projectivity<M>::inv() const
{
    return Projectivity(M::inv());
}
    
//! �ˉe�ϊ������߂邽�߂ɕK�v�ȓ_�΂̍ŏ�����Ԃ��D
/*!
  ���ݐݒ肳��Ă�����o�͋�Ԃ̎��������ƂɌv�Z�����D
  \return	�K�v�ȓ_�΂̍ŏ������Ȃ킿���͋�Ԃ̎���m�C�o�͋�Ԃ̎���n
		�ɑ΂��� m + 1 + m/n
*/
template <class M> inline u_int
Projectivity<M>::ndataMin() const
{
    return inDim() + 1 + u_int(ceil(double(inDim())/double(outDim())));
}
    
//! �^����ꂽ�_�Ɏˉe�ϊ���K�p���Ă��̔񓯎����W��Ԃ��D
/*!
  \param x	�_�̔񓯎����W�i#inDim()�����j�܂��͓������W�i#inDim()+1�����j
  \return	�ˉe�ϊ����ꂽ�_�̔񓯎����W�i#outDim()�����j
*/
template <class M> template <class S, class B>
inline Vector<typename Projectivity<M>::value_type>
Projectivity<M>::operator ()(const Vector<S, B>& x) const
{
    if (x.dim() == inDim())
    {
	Vector<value_type>	y(outDim());
	u_int			j;
	for (j = 0; j < y.dim(); ++j)
	{
	    y[j] = (*this)[j][x.dim()];
	    for (u_int i = 0; i < x.dim(); ++i)
		y[j] += (*this)[j][i] * x[i];
	}
	value_type	w = (*this)[j][x.dim()];
	for (u_int i = 0; i < x.dim(); ++i)
	    w += (*this)[j][i] * x[i];
	return y /= w;
    }
    else
	return (*this * x).inhomogeneous();
}

//! �^����ꂽ�_�Ɏˉe�ϊ���K�p���Ă��̓������W��Ԃ��D
/*!
  \param x	�_�̔񓯎����W�i#inDim()�����j�܂��͓������W�i#inDim()+1�����j
  \return	�ˉe�ϊ����ꂽ�_�̓������W�i#outDim()+1�����j
*/
template <class M> template <class S, class B>
inline Vector<typename Projectivity<M>::value_type>
Projectivity<M>::mapP(const Vector<S, B>& x) const
{
    if (x.dim() == inDim())
    {
	Vector<value_type>	y(nrow());
	for (u_int j = 0; j < y.dim(); ++j)
	{
	    y[j] = (*this)[j][x.dim()];
	    for (u_int i = 0; i < x.dim(); ++i)
		y[j] += (*this)[j][i] * x[i];
	}
	return y;
    }
    else
	return *this * x;
}

//! �^����ꂽ�_�ɂ����郄�R�r�s���Ԃ��D
/*!
  ���R�r�s��Ƃ͎ˉe�ϊ��s�񐬕��Ɋւ���1�K�����̂��Ƃł���D
  \param x	�_�̔񓯎����W�i#inDim()�����j�܂��͓������W�i#inDim()+1�����j
  \return	#outDim() x (#outDim()+1)x(#inDim()+1)���R�r�s��
*/
template <class M> template <class S, class B>
Matrix<typename Projectivity<M>::value_type>
Projectivity<M>::jacobian(const Vector<S, B>& x) const
{
    Vector<value_type>		xP;
    if (x.dim() == inDim())
	xP = x.homogeneous();
    else
	xP = x;
    const Vector<value_type>&	y  = mapP(xP);
    Matrix<value_type>		J(outDim(), (outDim() + 1)*xP.dim());
    for (u_int i = 0; i < J.nrow(); ++i)
    {
	J[i](i*xP.dim(), xP.dim()) = xP;
	(J[i](outDim()*xP.dim(), xP.dim()) = xP) *= (-y[i]/y[outDim()]);
    }
    J /= y[outDim()];

    return J;
}
    
//! ���͓_�Ɏˉe�ϊ���K�p�����_�Əo�͓_�̋�����2���Ԃ��D
/*!
  \param pair	���͓_�̔񓯎����W�i#inDim()�����j�Əo�͓_�̔񓯎����W
		�i#outDim()�����j�̑�
  \return	�ϊ����ꂽ���͓_�Əo�͓_�̋�����2��
*/
template <class M> template <class In, class Out>
inline typename Projectivity<M>::value_type
Projectivity<M>::sqdist(const std::pair<In, Out>& pair) const
{
    return (*this)(pair.first).sqdist(pair.second);
}
    
//! ���͓_�Ɏˉe�ϊ���K�p�����_�Əo�͓_�̋�����Ԃ��D
/*!
  \param pair	���͓_�̔񓯎����W�i#inDim()�����j�Əo�͓_�̔񓯎����W
		�i#outDim()�����j�̑�
  \return	�ϊ����ꂽ���͓_�Əo�͓_�̋���
*/
template <class M> template <class In, class Out>
inline typename Projectivity<M>::value_type
Projectivity<M>::dist(const std::pair<In, Out>& pair) const
{
    return sqrt(sqdist(pair));
}

//! ���̎ˉe�ϊ��̃p�����[�^����Ԃ��D
/*!
  �ˉe�ϊ��s��̗v�f���ł���C�ϊ��̎��R�x���Ƃ͈قȂ�D
  \return	�ˉe�ϊ��̃p�����[�^���i(#outDim()+1)x(#inDim()+1)�j
*/
template <class M> inline u_int
Projectivity<M>::nparams() const
{
    return (outDim() + 1)*(inDim() + 1);
}

//! �ˉe�ϊ��s���^����ꂽ�ʂ����C������D
/*!
  \param dt	�C���ʂ�\���x�N�g���i(#outDim()+1)x(#inDim()+1)�����j
*/
template <class M> inline void
Projectivity<M>::update(const Vector<value_type>& dt)
{
    Vector<value_type>	t(*this);
    value_type		l = t.length();
    t -= dt;
    t *= (l / t.length());
}

//! �^����ꂽ�_�Η�̕��ύē��e�덷��Ԃ��D
/*!
  \param begin	�_�Η�̐擪�����������q
  \param end	�_�Η�̖��������������q
  \return	���ύē��e�덷
*/
template <class M> template <class Iterator>
typename Projectivity<M>::value_type
Projectivity<M>::reprojectionError(Iterator begin, Iterator end) const
{
    value_type	sqrerr_sum = 0.0;
    u_int	npoints = 0;
    for (Iterator iter = begin; iter != end; ++iter)
    {
	const Vector<value_type>&	err = (*this)(iter->first) - iter->second;
	sqrerr_sum += err.square();
	++npoints;
    }

    return (npoints > 0 ? sqrt(sqrerr_sum / npoints) : 0.0);
}

template <class M> template <class Map, class Iterator>
Projectivity<M>::Cost<Map, Iterator>::Cost(Iterator begin, Iterator end)
    :_begin(begin), _end(end), _npoints(std::distance(_begin, _end))
{
}
    
template <class M> template <class Map, class Iterator>
Vector<typename Map::value_type>
Projectivity<M>::Cost<Map, Iterator>::operator ()(const Map& map) const
{
    const u_int		outDim = map.outDim();
    Vector<value_type>	val(_npoints*outDim);
    u_int		n = 0;
    for (Iterator iter = _begin; iter != _end; ++iter)
    {
	val(n, outDim) = map(iter->first) - iter->second;
	n += outDim;
    }
    
    return val;
}
    
template <class M> template <class Map, class Iterator>
Matrix<typename Map::value_type>
Projectivity<M>::Cost<Map, Iterator>::jacobian(const Map& map) const
{
    const u_int		outDim = map.outDim();
    Matrix<value_type>	J(_npoints*outDim, map.nparams());
    u_int		n = 0;
    for (Iterator iter = _begin; iter != _end; ++iter)
    {
	J(n, 0, outDim, J.ncol()) = map.jacobian(iter->first);
	n += outDim;
    }

    return J;
}

template <class M> template <class Map, class Iterator> inline void
Projectivity<M>::Cost<Map, Iterator>::update(Map& map,
					     const Vector<value_type>& dm)
{
    map.update(dm);
}

typedef Projectivity<Matrix22f>	Projectivity11f;
typedef Projectivity<Matrix22d>	Projectivity11d;
typedef Projectivity<Matrix33f>	Projectivity22f;
typedef Projectivity<Matrix33d>	Projectivity22d;
typedef Projectivity<Matrix44f>	Projectivity33f;
typedef Projectivity<Matrix44d>	Projectivity33d;
typedef Projectivity<Matrix34f>	Projectivity23f;
typedef Projectivity<Matrix34d>	Projectivity23d;

/************************************************************************
*  class Affinity<M>							*
************************************************************************/
//! �A�t�B���ϊ����s���N���X
/*!
  \f$\TUvec{A}{} \in \TUspace{R}{n\times m}\f$��
  \f$\TUvec{b}{} \in \TUspace{R}{n}\f$��p����m������Ԃ̓_
  \f$\TUvec{x}{} \in \TUspace{R}{m}\f$��n������Ԃ̓_
  \f$\TUvec{y}{} \simeq \TUvec{A}{}\TUvec{x}{} + \TUvec{b}{}
  \in \TUspace{R}{n}\f$�Ɏʂ��i\f$m \neq n\f$�ł��\��Ȃ��j�D
*/
template <class M>
class Affinity : public Projectivity<M>
{
  public:
    typedef typename Projectivity<M>::value_type	value_type;
    
  //! ���͋�ԂƏo�͋�Ԃ̎������w�肵�ăA�t�B���ϊ��I�u�W�F�N�g�𐶐�����D
  /*!
    �P���ϊ��Ƃ��ď����������D
    \param inDim	���͋�Ԃ̎���
    \param outDim	�o�͋�Ԃ̎���
  */
    Affinity(u_int inDim=2, u_int outDim=2)
	:Projectivity<M>(inDim, outDim)					{}

    template <class S, class B, class R>
    Affinity(const Matrix<S, B, R>& T)					;
    template <class Iterator>
    Affinity(Iterator begin, Iterator end)				;

    using	Projectivity<M>::inDim;
    using	Projectivity<M>::outDim;
    
    template <class S, class B, class R>
    void	set(const Matrix<S, B, R>& T)				;
    template <class Iterator>
    void	fit(Iterator begin, Iterator end)			;
    Affinity	inv()						const	;
    u_int	ndataMin()					const	;
    
  //! ���̃A�t�B���ϊ��̕ό`������\������s���Ԃ��D
  /*! 
    \return	#outDim() x #inDim()�s��
  */
    const Matrix<value_type>
		A()	const	{return M::operator ()(0, 0, outDim(), inDim());}
    
    Vector<value_type>
		b()	const	;
};

//! �ϊ��s����w�肵�ăA�t�B���ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  �ϊ��s��̉��[�s�͋����I�� 0,0,...,0,1 �ɐݒ肳���D
  \param T			(m+1)x(n+1)�s��im, n�͓��́^�o�͋�Ԃ̎����j
*/
template<class M> template <class S, class B, class R> inline
Affinity<M>::Affinity(const Matrix<S, B, R>& T)
    :Projectivity<M>(T)
{
    (*this)[outDim()]	  = 0.0;
    (*this)[outDim()][inDim()] = 1.0;
}

//! �^����ꂽ�_�Η�̔񓯎����W����A�t�B���ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  \param begin			�_�Η�̐擪�����������q
  \param end			�_�Η�̖��������������q
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template<class M> template <class Iterator> inline
Affinity<M>::Affinity(Iterator begin, Iterator end)
{
    fit(begin, end);
}

//! �ϊ��s����w�肷��D
/*!
  �ϊ��s��̉��[�s�͋����I�� 0,0,...,0,1 �ɐݒ肳���D
  \param T			(m+1)x(n+1)�s��im, n�͓��́^�o�͋�Ԃ̎����j
*/
template<class M> template <class S, class B, class R> inline void
Affinity<M>::set(const Matrix<S, B, R>& T)
{
    Projectivity<M>::set(T);
    (*this)[outDim()]	       = 0.0;
    (*this)[outDim()][inDim()] = 1.0;
}
    
//! �^����ꂽ�_�Η�̔񓯎����W����A�t�B���ϊ����v�Z����D
/*!
  \param begin			�_�Η�̐擪�����������q
  \param end			�_�Η�̖��������������q
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template<class M> template <class Iterator> void
Affinity<M>::fit(Iterator begin, Iterator end)
{
  // �[���Ȍ��̓_�΂����邩�H
    const u_int	ndata = std::distance(begin, end);
    if (ndata == 0)		// begin���L�����H
	throw std::invalid_argument("Affinity::fit(): 0-length input data!!");
    const u_int	xdim = begin->first.dim();
    if (ndata < xdim + 1)	// �s��̃T�C�Y������Ȃ̂�ndataMin()�͖���
	throw std::invalid_argument("Affinity::fit(): not enough input data!!");

  // �f�[�^�s��̌v�Z
    const u_int	ydim = begin->second.dim(), xydim2 = xdim*ydim;
    Matrix<value_type>	N(xdim, xdim);
    Vector<value_type>	c(xdim), v(xydim2 + ydim);
    for (Iterator iter = begin; iter != end; ++iter)
    {
	const Vector<value_type>&	x = iter->first;
	const Vector<value_type>&	y = iter->second;

	N += x % x;
	c += x;
	for (u_int j = 0; j < ydim; ++j)
	    v(j*xdim, xdim) += y[j]*x;
	v(xydim2, ydim) += y;
    }
    Matrix<value_type>	W(xydim2 + ydim, xydim2 + ydim);
    for (u_int j = 0; j < ydim; ++j)
    {
	W(j*xdim, j*xdim, xdim, xdim) = N;
	W[xydim2 + j](j*xdim, xdim)   = c;
	W[xydim2 + j][xydim2 + j]     = ndata;
    }
    W.symmetrize();

  // W*u = v�������ĕϊ��p�����[�^�����߂�D
    v.solve(W);

  // �ϊ��s����Z�b�g����D
    M::resize(ydim + 1, xdim + 1);
    M::operator ()(0, 0, ydim, xdim)
	= Matrix<value_type>((value_type*)v, ydim, xdim);
    for (u_int j = 0; j < ydim; ++j)
	(*this)[j][xdim] = v[xydim2 + j];
    (*this)[ydim][xdim] = 1.0;
}

//! ���̃A�t�B���ϊ��̕��s�ړ�������\������x�N�g����Ԃ��D
/*! 
  \return	#outDim()�����x�N�g��
*/
template <class M> Vector<typename Affinity<M>::value_type>
Affinity<M>::b() const
{
    Vector<value_type>	bb(outDim());
    for (u_int j = 0; j < bb.dim(); ++j)
	bb[j] = (*this)[j][inDim()];

    return bb;
}

//! ���̃A�t�B���ϊ��̋t�ϊ���Ԃ��D
/*!
  \return	�t�ϊ�
*/
template <class M> inline Affinity<M>
Affinity<M>::inv() const
{
    return Affinity(M::inv());
}
    
//! �A�t�B���ϊ������߂邽�߂ɕK�v�ȓ_�΂̍ŏ�����Ԃ��D
/*!
  ���ݐݒ肳��Ă�����o�͋�Ԃ̎��������ƂɌv�Z�����D
  \return	�K�v�ȓ_�΂̍ŏ������Ȃ킿���͋�Ԃ̎���m�ɑ΂��� m + 1
*/
template<class M> inline u_int
Affinity<M>::ndataMin() const
{
    return inDim() + 1;
}

typedef Affinity<Matrix22f>	Affinity11f;
typedef Affinity<Matrix22d>	Affinity11d;
typedef Affinity<Matrix33f>	Affinity22f;
typedef Affinity<Matrix33d>	Affinity22d;
typedef Affinity<Matrix44f>	Affinity33f;
typedef Affinity<Matrix44d>	Affinity33d;
typedef Affinity<Matrix34f>	Affinity23f;
typedef Affinity<Matrix34d>	Affinity23d;

/************************************************************************
*  class Homography<T>							*
************************************************************************/
//! 2�����ˉe�ϊ����s���N���X
/*!
  \f$\TUvec{H}{} = \in \TUspace{R}{3\times 3}\f$��p����2������Ԃ̓_
  \f$\TUud{x}{} \in \TUspace{R}{3}\f$��2������Ԃ̓_
  \f$\TUud{y}{} \simeq \TUvec{H}{}\TUud{x}{} \in \TUspace{R}{3}\f$
  �Ɏʂ��D
*/
template <class T>
class Homography : public Projectivity<Matrix<T, FixedSizedBuf<T, 9>,
					      FixedSizedBuf<Vector<T>, 3> > >
{
  private:
    typedef Projectivity<Matrix<T, FixedSizedBuf<T, 9>,
				FixedSizedBuf<Vector<T>, 3> > >	super;
    
  public:
    enum	{DOF=8};
    
    typedef typename super::matrix_type				matrix_type;
    typedef typename super::value_type				value_type;
    typedef Point2<value_type>					point_type;
    typedef Vector<value_type, FixedSizedBuf<value_type, DOF> >	param_type;
    typedef Matrix<value_type, FixedSizedBuf<value_type, 2*DOF>,
		   FixedSizedBuf<Vector<value_type>, 2> >	jacobian_type;

  public:
    Homography()			 :super()		{}
    template <class S, class B, class R>
    Homography(const Matrix<S, B, R>& H) :super(H)		{}
    template <class Iterator>
    Homography(Iterator begin, Iterator end, bool refine=false)	;

    using	super::operator ();
    using	super::inDim;
    using	super::outDim;
    using	super::ndataMin;
    using	super::nparams;

    Homography	inv()					const	;
    point_type	operator ()(int u, int v)		const	;
    static jacobian_type
		jacobian0(int u, int v)				;
    
    void	compose(const param_type& dt)			;
};

//! �^����ꂽ�_�Η�̔񓯎����W����2�����ˉe�ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  \param begin			�_�Η�̐擪�����������q
  \param end			�_�Η�̖��������������q
  \param refine			����^�œK���̗L(true)�^��(false)���w��
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template<class T> template <class Iterator> inline
Homography<T>::Homography(Iterator begin, Iterator end, bool refine)
    :super()
{
    fit(begin, end, refine);
}

//! ����2�����ˉe�ϊ��̋t�ϊ���Ԃ��D
/*!
  \return	�t�ϊ�
*/
template <class T> Homography<T>
Homography<T>::inv() const
{
    return Homography(matrix_type::inv());
}

template <class T> inline typename Homography<T>::point_type
Homography<T>::operator ()(int u, int v) const
{
    value_type	w = 1.0 / ((*this)[2][0]*u + (*this)[2][1]*v + (*this)[2][2]);
    return point_type(w * ((*this)[0][0]*u + (*this)[0][1]*v + (*this)[0][2]),
		      w * ((*this)[1][0]*u + (*this)[1][1]*v + (*this)[1][2]));
}

template <class T> inline typename Homography<T>::jacobian_type
Homography<T>::jacobian0(int u, int v)
{
    jacobian_type	J(2, 8);
    J[0][0] = J[1][3] = u;
    J[0][1] = J[1][4] = v;
    J[0][2] = J[1][5] = 1.0;
    J[0][3] = J[0][4] = J[0][5] = J[1][0] = J[1][1] = J[1][2] = 0.0;
    J[0][6]	      = -u * u;
    J[0][7] = J[1][6] = -u * v;
    J[1][7]	      = -v * v;

    return J;
}

template <class T> inline void
Homography<T>::compose(const param_type& dt)
{
    value_type	t0 = (*this)[0][0], t1 = (*this)[0][1], t2 = (*this)[0][2];
    (*this)[0][0] -= (t0*dt[0] + t1*dt[3] + t2*dt[6]);
    (*this)[0][1] -= (t0*dt[1] + t1*dt[4] + t2*dt[7]);
    (*this)[0][2] -= (t0*dt[2] + t1*dt[5]);
    
    t0 = (*this)[1][0];
    t1 = (*this)[1][1];
    t2 = (*this)[1][2];
    (*this)[1][0] -= (t0*dt[0] + t1*dt[3] + t2*dt[6]);
    (*this)[1][1] -= (t0*dt[1] + t1*dt[4] + t2*dt[7]);
    (*this)[1][2] -= (t0*dt[2] + t1*dt[5]);

    t0 = (*this)[2][0];
    t1 = (*this)[2][1];
    t2 = (*this)[2][2];
    (*this)[2][0] -= (t0*dt[0] + t1*dt[3] + t2*dt[6]);
    (*this)[2][1] -= (t0*dt[1] + t1*dt[4] + t2*dt[7]);
    (*this)[2][2] -= (t0*dt[2] + t1*dt[5]);
}

/************************************************************************
*  class Affinity2<T>							*
************************************************************************/
//! 2�����A�t�B���ϊ����s���N���X
/*!
  \f$\TUvec{A}{} = \in \TUspace{R}{3\times 3}\f$��p����2������Ԃ̓_
  \f$\TUud{x}{} \in \TUspace{R}{3}\f$��2������Ԃ̓_
  \f$\TUud{y}{} \simeq \TUvec{A}{}\TUud{x}{} \in \TUspace{R}{3}\f$
  �Ɏʂ��D
*/
template <class T>
class Affinity2 : public Affinity<Matrix<T, FixedSizedBuf<T, 9>,
					 FixedSizedBuf<Vector<T>, 3> > >
{
  private:
    typedef Affinity<Matrix<T, FixedSizedBuf<T, 9>,
			    FixedSizedBuf<Vector<T>, 3> > >	super;
    
  public:
    enum	{DOF=6};
    
    typedef typename super::matrix_type				matrix_type;
    typedef typename super::value_type				value_type;
    typedef Vector<value_type, FixedSizedBuf<value_type, DOF> >	param_type;
    typedef Point2<value_type>					point_type;
    typedef Matrix<value_type, FixedSizedBuf<value_type, 2*DOF>,
		   FixedSizedBuf<Vector<value_type>, 2> >	jacobian_type;

  public:
    Affinity2()				:super()		{}
    template <class S, class B, class R>
    Affinity2(const Matrix<S, B, R>& A)				;
    template <class Iterator>
    Affinity2(Iterator begin, Iterator end)			;

    using	super::operator ();
    using	super::inDim;
    using	super::outDim;
    using	super::ndataMin;
    using	super::nparams;

    Affinity2	inv()					const	;
    point_type	operator ()(int u, int v)		const	;
    static jacobian_type
		jacobian0(int u, int v)				;
    
    void	compose(const param_type& dt)			;
};

template <class T> template <class S, class B, class R> inline
Affinity2<T>::Affinity2(const Matrix<S, B, R>& A)
    :super(A)
{
    (*this)[2][0] = (*this)[2][1] = 0.0;
    (*this)[2][2] = 1.0;
}
    
//! �^����ꂽ�_�Η�̔񓯎����W����2�����A�t�B���ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  \param begin			�_�Η�̐擪�����������q
  \param end			�_�Η�̖��������������q
  \throw std::invalid_argument	�_�΂̐���#ndataMin()�ɖ����Ȃ��ꍇ�ɑ��o
*/
template<class T> template <class Iterator> inline
Affinity2<T>::Affinity2(Iterator begin, Iterator end)
{
    fit(begin, end);
}

//! ����2�����A�t�B���ϊ��̋t�ϊ���Ԃ��D
/*!
  \return	�t�ϊ�
*/
template <class T> inline Affinity2<T>
Affinity2<T>::inv() const
{
    return Affinity2(matrix_type::inv());
}
    
template <class T> inline typename Affinity2<T>::point_type
Affinity2<T>::operator ()(int u, int v) const
{
    return point_type((*this)[0][0]*u + (*this)[0][1]*v + (*this)[0][2],
		      (*this)[1][0]*u + (*this)[1][1]*v + (*this)[1][2]);
}

template <class T> inline typename Affinity2<T>::jacobian_type
Affinity2<T>::jacobian0(int u, int v)
{
    jacobian_type	J;
    J[0][0] = J[1][3] = u;
    J[0][1] = J[1][4] = v;
    J[0][2] = J[1][5] = 1.0;
    J[0][3] = J[0][4] = J[0][5] = J[1][0] = J[1][1] = J[1][2] = 0.0;

    return J;
}
    
template <class T> inline void
Affinity2<T>::compose(const param_type& dt)
{
    value_type	t0 = (*this)[0][0], t1 = (*this)[0][1];
    (*this)[0][0] -= (t0*dt[0] + t1*dt[3]);
    (*this)[0][1] -= (t0*dt[1] + t1*dt[4]);
    (*this)[0][2] -= (t0*dt[2] + t1*dt[5]);
    
    t0 = (*this)[1][0];
    t1 = (*this)[1][1];
    (*this)[1][0] -= (t0*dt[0] + t1*dt[3]);
    (*this)[1][1] -= (t0*dt[1] + t1*dt[4]);
    (*this)[1][2] -= (t0*dt[2] + t1*dt[5]);
}
    
/************************************************************************
*   class BoundingBox<P>						*
************************************************************************/
//! P�^�̓_�ɑ΂���bounding box��\���N���X
/*!
  \param P	�_�̌^�i�����͎��R�j
*/
template <class P>
class BoundingBox
{
  public:
  //! �_�̗v�f�̌^
    typedef typename P::value_type		value_type;

  public:
    BoundingBox()				;
    explicit BoundingBox(u_int d)		;

    bool		operator !()	const	;
  //! ����bounding box���������Ԃ̎�����Ԃ��D
  /*!
    \return	��Ԃ̎���
  */
    u_int		dim()		const	{return _min.dim();}

  //! ����bounding box�̍ŏ��_��Ԃ��D
  /*!
    \return	�ŏ��_
  */
    const P&		min()		const	{return _min;}

  //! ����bounding box�̍ő�_��Ԃ��D
  /*!
    \return	�ő�_
  */
    const P&		max()		const	{return _max;}

  //! ����bounding box�̍ŏ��_�̎w�肳�ꂽ���̍��W�l��Ԃ��D
  /*!
    \param i	�����w�肷��index
    \return	���̍��W�l
  */
    value_type		min(int i)	const	{return _min[i];}

  //! ����bounding box�̍ő�_�̎w�肳�ꂽ���̍��W�l��Ԃ��D
  /*!
    \param i	�����w�肷��index
    \return	���̍��W�l
  */
    value_type		max(int i)	const	{return _max[i];}

  //! ����bounding box�̎w�肳�ꂽ���ɉ�����������Ԃ��D
  /*!
    \param i	�����w�肷��index
    \return	���ɉ���������
  */
    value_type		length(int i)	const	{return _max[i] - _min[i];}

  //! ����bounding box�̕���Ԃ��D
  /*!
    \return	�� (#TU::BoundingBox::length (0)�ɓ�����)
  */
    value_type		width()		const	{return length(0);}

  //! ����bounding box�̍�����Ԃ��D
  /*!
    \return	���� (#TU::BoundingBox::length (1)�ɓ�����)
  */
    value_type		height()	const	{return length(1);}

  //! ����bounding box�̉��s����Ԃ��D
  /*!
    \return	���s�� (#TU::BoundingBox::length (2)�ɓ�����)
  */
    value_type		depth()		const	{return length(2);}

    template <class S, class B>
    bool		include(const Vector<S, B>& p)			;
    BoundingBox&	clear()						;
    template <class S, class B>
    BoundingBox&	expand(const Vector<S, B>& p)			;
    template <class S, class B>
    BoundingBox&	operator +=(const Vector<S, B>& dt)		;
    template <class S, class B>
    BoundingBox&	operator -=(const Vector<S, B>& dt)		;
    BoundingBox&	operator *=(double c)				;
    BoundingBox&	operator |=(const BoundingBox& bbox)		;
    BoundingBox&	operator &=(const BoundingBox& bbox)		;
    
  private:
  //! ���̓X�g���[������bounding box�𐬂�2�̓_�̍��W����͂���(ASCII)�D
  /*!
    \param in	���̓X�g���[��
    \param bbox	bounding box
    \return	in�Ŏw�肵�����̓X�g���[��
  */
    friend std::istream&
    operator >>(std::istream& in, BoundingBox<P>& bbox)
    {
	return in >> bbox._min >> bbox._max;
    }
    
    P	_min;
    P	_max;
};

//! ���bounding box�����D
template <class P> inline
BoundingBox<P>::BoundingBox()
    :_min(), _max()
{
    clear();
}

//! �w�肵�������̋�Ԃɂ����ċ��bounding box�����D
/*!
  \param d	��Ԃ̎���
*/
template <class P> inline
BoundingBox<P>::BoundingBox(u_int d)
    :_min(d), _max(d)
{
    clear();
}

//! bounding box����ł��邩���ׂ�D
/*!
  \return	��ł����true, �����łȂ����false
*/
template <class P> bool
BoundingBox<P>::operator !() const
{
    for (u_int i = 0; i < dim(); ++i)
	if (_min[i] > _max[i])
	    return true;
    return false;
}

//! bounding box���^����ꂽ�_���܂ނ����ׂ�D
/*!
  \param p	�_�̍��W
  \return	�܂߂�true, �����łȂ����false
*/
template <class P> template <class S, class B> bool
BoundingBox<P>::include(const Vector<S, B>& p)
{
    for (u_int i = 0; i < dim(); ++i)
	if (p[i] < _min[i] || p[i] > _max[i])
	    return false;
    return true;
}

//! bounding box����ɂ���D
/*!
  \return	��ɂ��ꂽ����bounding box
*/
template <class P> BoundingBox<P>&
BoundingBox<P>::clear()
{
    typedef std::numeric_limits<value_type>	Limits;
    
    for (u_int i = 0; i < dim(); ++i)
    {
	_min[i] = Limits::max();
	_max[i] = (Limits::is_integer ? Limits::min() : -Limits::max());
    }
    return *this;
}

//! bounding box��^����ꂽ�_���܂ނ悤�Ɋg������D
/*!
  \param p	�_�̍��W
  \return	�g�����ꂽ����bounding box
*/
template <class P> template <class S, class B> BoundingBox<P>&
BoundingBox<P>::expand(const Vector<S, B>& p)
{
    for (int i = 0; i < dim(); ++i)
    {
	_min[i] = std::min(_min[i], p[i]);
	_max[i] = std::max(_max[i], p[i]);
    }
    return *this;
}

//! bounding box��^����ꂽ�ψʂ����������ɕ��s�ړ�����D
/*!
  \param dt	�ψ�
  \return	���s�ړ����ꂽ����bounding box
*/
template <class P> template <class S, class B> inline BoundingBox<P>&
BoundingBox<P>::operator +=(const Vector<S, B>& dt)
{
    _min += dt;
    _max += dt;
    return *this;
}
    
//! bounding box��^����ꂽ�ψʂ����������ɕ��s�ړ�����D
/*!
  \param dt	�ψ�
  \return	���s�ړ����ꂽ����bounding box
*/
template <class P> template <class S, class B> inline BoundingBox<P>&
BoundingBox<P>::operator -=(const Vector<S, B>& dt)
{
    _min -= dt;
    _max -= dt;
    return *this;
}
    
//! bounding box��^����ꂽ�X�P�[�������g��^�k������D
/*!
  ���̃X�P�[����^�����bounding box�����]����D
  \param c	�X�P�[��
  \return	���s�ړ����ꂽ����bounding box
*/
template <class P> inline BoundingBox<P>&
BoundingBox<P>::operator *=(double c)
{
    if (c < 0.0)
	std::swap(_min, _max);
    _min *= c;
    _max *= c;
    return *this;
}

//! ����bounding box�Ǝw�肳�ꂽbounding box�Ƃ̌��т��Ƃ�D
/*!
  \param bbox	bounding box
  \return	���т��Ƃ�����̂���bounding box
*/
template <class P> inline BoundingBox<P>&
BoundingBox<P>::operator |=(const BoundingBox<P>& bbox)
{
    return expand(bbox.min()).expand(bbox.max());
}
    
//! ����bounding box�Ǝw�肳�ꂽbounding box�Ƃ̌������Ƃ�D
/*!
  �^����ꂽbounding box�Ƃ̊Ԃɋ��ʕ������Ȃ���΋��bounding box�ƂȂ�D
  \param bbox	bounding box
  \return	�������Ƃ�����̂���bounding box
*/
template <class P> BoundingBox<P>&
BoundingBox<P>::operator &=(const BoundingBox<P>& bbox)
{
    for (int i = 0; i < dim(); ++i)
    {
	_min[i] = std::max(_min[i], bbox.min(i));
	_max[i] = std::min(_max[i], bbox.max(i));
    }
    return *this;
}
    
//! 2��bounding box�̌��т��Ƃ�D
/*!
  \param a	bounding box
  \param b	bounding box
  \return	a��b�̌��тƂȂ�bounding box
*/
template <class P> inline BoundingBox<P>
operator |(const BoundingBox<P>& a, const BoundingBox<P>& b)
{
    BoundingBox<P>	c(a);
    return c |= b;
}
    
//! 2��bounding box�̌������Ƃ�D
/*!
  �^����ꂽbounding box�ɋ��ʕ������Ȃ���΋��bounding box��Ԃ��D
  \param a	bounding box
  \param b	bounding box
  \return	a��b�̌����ƂȂ�bounding box
*/
template <class P> inline BoundingBox<P>
operator &(const BoundingBox<P>& a, const BoundingBox<P>& b)
{
    BoundingBox<P>	c(a);
    return c &= b;
}

//! �o�̓X�g���[����bounding box�𐬂�2�̓_�̍��W���o�͂���(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \param bbox	bounding box
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class P> std::ostream&
operator <<(std::ostream& out, const BoundingBox<P>& bbox)
{
#ifdef _DEBUG
    for (u_int i = 0; i < bbox.dim(); ++i)
    {
	if (i != 0)
	    out << 'x';
	out << '[' << bbox.min(i) << ", " << bbox.max(i) << ']';
    }
    return out << std::endl;
#else
    return out << bbox.min() << bbox.max() << std::endl;
#endif
}
    
}
#endif	/* !__TUGeometryPP_h */
