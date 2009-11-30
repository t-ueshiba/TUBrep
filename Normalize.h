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
 *  $Id: Normalize.h,v 1.5 2009-11-30 00:05:31 ueshiba Exp $  
 */
#ifndef __TUNormalize_h
#define __TUNormalize_h

#include "TU/Vector++.h"

namespace TU
{
/************************************************************************
*  class Normalize							*
************************************************************************/
//! �_�̔񓯎����W�̐��K���ϊ����s���N���X
/*!
  \f$\TUud{x}{}=[\TUtvec{x}{}, 1]^\top~
  (\TUvec{x}{} \in \TUspace{R}{d})\f$�ɑ΂��āC�ȉ��̂悤�ȕ��s�ړ���
  �X�P�[�����O���s��:
  \f[
	\TUud{y}{} =
	\TUbeginarray{c} s^{-1}(\TUvec{x}{} - \TUvec{c}{}) \\ 1	\TUendarray =
	\TUbeginarray{ccc}
	  s^{-1} \TUvec{I}{d} & -s^{-1}\TUvec{c}{} \\ \TUtvec{0}{d} & 1
	\TUendarray
	\TUbeginarray{c} \TUvec{x}{} \\ 1 \TUendarray =
	\TUvec{T}{}\TUud{x}{}
  \f]
  \f$s\f$��\f$\TUvec{c}{}\f$�́C�U����2�敽�ϒl����Ԃ̎���\f$d\f$��,
  �d�S�����_�ɂȂ�悤���肳���D
*/
class __PORT Normalize
{
  public:
  //! ��Ԃ̎������w�肵�Đ��K���ϊ��I�u�W�F�N�g�𐶐�����D
  /*!
    �P���ϊ��Ƃ��ď����������D
    \param d	��Ԃ̎���
  */
    Normalize(u_int d=2) :_npoints(0), _scale(1.0), _centroid(d)	{}

    template <class Iterator>
    Normalize(Iterator first, Iterator last)				;
    
    template <class Iterator>
    void		update(Iterator first, Iterator last)		;

    u_int		spaceDim()				const	;
    template <class T2, class B2>
    Vector<double>	operator ()(const Vector<T2, B2>& x)	const	;
    template <class T2, class B2>
    Vector<double>	normalizeP(const Vector<T2, B2>& x)	const	;
    
    Matrix<double>		T()				const	;
    Matrix<double>		Tt()				const	;
    Matrix<double>		Tinv()				const	;
    Matrix<double>		Ttinv()				const	;
    double			scale()				const	;
    const Vector<double>&	centroid()			const	;
    
  private:
    u_int		_npoints;	//!< ����܂łɗ^�����_�̑���
    double		_scale;		//!< ����܂łɗ^�����_�̐U����RMS�l
    Vector<double>	_centroid;	//!< ����܂łɗ^�����_�Q�̏d�S
};

//! �^����ꂽ�_�Q�̔񓯎����W���琳�K���ϊ��I�u�W�F�N�g�𐶐�����D
/*!
  �U����2�敽�ϒl��#spaceDim(), �d�S�����_�ɂȂ�悤�Ȑ��K���ϊ����v�Z�����D
  \param first	�_�Q�̐擪�����������q
  \param last	�_�Q�̖��������������q
*/
template <class Iterator> inline
Normalize::Normalize(Iterator first, Iterator last)
    :_npoints(0), _scale(1.0), _centroid()
{
    update(first, last);
}
    
//! �V���ɓ_�Q��ǉ����Ă��̔񓯎����W���猻�݂̐��K���ϊ����X�V����D
/*!
  �U����2�敽�ϒl��#spaceDim(), �d�S�����_�ɂȂ�悤�Ȑ��K���ϊ����v�Z�����D
  \param first			�_�Q�̐擪�����������q
  \param last			�_�Q�̖��������������q
  \throw std::invalid_argument	����܂łɗ^����ꂽ�_�̑�����0�̏ꍇ�ɑ��o
*/
template <class Iterator> void
Normalize::update(Iterator first, Iterator last)
{
    if (_npoints == 0)
    {
	if (first == last)
	    throw std::invalid_argument("Normalize::update(): 0-length input data!!");
	_centroid.resize(first->dim());
    }
    _scale = _npoints * (spaceDim() * _scale * _scale + _centroid * _centroid);
    _centroid *= _npoints;
    while (first != last)
    {
	_scale += first->square();
	_centroid += *first++;
	++_npoints;
    }
    if (_npoints == 0)
	throw std::invalid_argument("Normalize::update(): no input data accumulated!!");
    _centroid /= _npoints;
    _scale = sqrt((_scale / _npoints - _centroid * _centroid) / spaceDim());
}

//! ���̐��K���ϊ����K�p������Ԃ̎�����Ԃ��D
/*! 
  \return	��Ԃ̎���(�������W�̃x�N�g���Ƃ��Ă̎�����#spaceDim()+1)
*/
inline u_int
Normalize::spaceDim() const
{
    return _centroid.dim();
}
    
//! �^����ꂽ�_�ɐ��K���ϊ���K�p���Ă��̔񓯎����W��Ԃ��D
/*!
  \param x	�_�̔񓯎����W�i#spaceDim()�����j
  \return	���K�����ꂽ�_�̔񓯎����W�i#spaceDim()�����j
*/
template <class T2, class B2> inline Vector<double>
Normalize::operator ()(const Vector<T2, B2>& x) const
{
    return (Vector<double>(x) -= _centroid) /= _scale;
}

//! �^����ꂽ�_�ɐ��K���ϊ���K�p���Ă��̓������W��Ԃ��D
/*!
  \param x	�_�̔񓯎����W�i#spaceDim()�����j
  \return	���K�����ꂽ�_�̓������W�i#spaceDim()+1�����j
*/
template <class T2, class B2> inline Vector<double>
Normalize::normalizeP(const Vector<T2, B2>& x) const
{
    return (*this)(x).homogeneous();
}

//! ���K���ϊ��̃X�P�[�����O�萔��Ԃ��D
/*!
  \return	�X�P�[�����O�萔�i�^����ꂽ�_��̐U����2�敽�ϒl�j
*/
inline double
Normalize::scale() const
{
    return _scale;
}

//! ���K���ϊ��̕��s�ړ�������Ԃ��D
/*!
  \return	���s�ړ������i�^����ꂽ�_��̏d�S�j
*/
inline const Vector<double>&
Normalize::centroid() const
{
    return _centroid;
}

}
#endif	/* !__TUNormalize_h */
