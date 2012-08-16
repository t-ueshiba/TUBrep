/*
 *  ����14-24�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
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
 *  Copyright 2002-2012.
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
 *  $Id: algorithm.h,v 1.3 2012-08-16 04:00:42 ueshiba Exp $
 */
/*!
  \file		algorithm.h
  \brief	�e��A���S���Y���̒�`�Ǝ���
*/
#ifndef __TUalgorithm_h
#define __TUalgorithm_h

#include <algorithm>
#include "TU/types.h"

/*!
  \namespace	std
  \brief	�������̊�{�I�Ȋ֐��𖼑O���std�ɒǉ�
*/
namespace std
{
/************************************************************************
*  generic algorithms							*
************************************************************************/
//! 3�̈����̂����ŏ��̂��̂�Ԃ��D
template <class T> inline const T&
min(const T& a, const T& b, const T& c)
{
    return min(min(a, b), c);
}

//! 3�̈����̂����ő�̂��̂�Ԃ��D
template <class T> inline const T&
max(const T& a, const T& b, const T& c)
{
    return max(max(a, b), c);
}
    
//! 4�̈����̂����ŏ��̂��̂�Ԃ��D
template <class T> inline const T&
min(const T& a, const T& b, const T& c, const T& d)
{
    return min(min(a, b, c), d);
}

//! 4�̈����̂����ő�̂��̂�Ԃ��D
template <class T> inline const T&
max(const T& a, const T& b, const T& c, const T& d)
{
    return max(max(a, b, c), d);
}

}

/*!
  \namespace	TU
  \brief	�{���C�u�����Œ�`���ꂽ�N���X����ъ֐���[�߂閼�O���
*/
namespace TU
{
/************************************************************************
*  generic algorithms							*
************************************************************************/
//! �����𖞂����v�f���O���ɁC�����łȂ����̂��㔼�ɂȂ�悤�ɕ��בւ���D
/*!
  \param begin	�f�[�^��̐擪�����������q
  \param end	�f�[�^��̖��������������q
  \param pred	�������w�肷��P�����Z�q
  \return	�����𖞂����Ȃ��v�f�̐擪�����������q
*/
template <class Iter, class Pred> Iter
pull_if(Iter begin, Iter end, Pred pred)
{
    for (Iter iter = begin; iter != end; ++iter)
	if (pred(*iter))
	    std::iter_swap(begin++, iter);
    return begin;
}

//! 2�̈����̍��̐�Βl��Ԃ��D
template <class T> inline T
diff(const T& a, const T& b)
{
    return (a > b ? a - b : b - a);
}

//! 2�����f�[�^�ɑ΂���3x3�E�B���h�E�𑖍�����in-place�ߖT���Z���s���D
/*!
  \param begin	�ŏ��̍s�����������q
  \param end	�Ō�̍s�̎������������q
  \param op	3x3�E�B���h�E���`��Ƃ��鉉�Z�q
*/
template <class Iterator, class OP> void
op3x3(Iterator begin, Iterator end, OP op)
{
    typedef typename std::iterator_traits<Iterator>::value_type	row_type;
    typedef typename row_type::iterator				col_iterator;
    typedef typename std::iterator_traits<col_iterator>::value_type
								value_type;
    
    row_type	buf = *begin;		// ��O�̍s
    --end;
    for (Iterator iter = ++begin; iter != end; )
    {
	col_iterator	p    = buf.begin();	// �����f
	col_iterator	q    = iter->begin();	// ����f	
	value_type	val  = *q;		// ����f�ɂ����錋��
	col_iterator	cend = (++iter)->end();
	--cend;
	--cend;				// ������f�̉E�[
	for (col_iterator c = iter->begin(); c != cend; )   // ������f�ɂ���
	{						    // ���[�v
	    value_type	tmp = op(p, q, c);	// ���ډ�f�ɂ����錋��
	    *p  = *q;			// ���s�̍����f = ����f
	    *q  = val;			// ����f�ɂ����錋�ʂ���������
	    val	= tmp;			// ���E�B���h�E�̍���f�ɂ����錋�ʂ�ۑ�
	    ++c;
	    ++p;
	    ++q;
	}
	*p = *q;			// ���s�̍����f = ����f
	*q = val;			// ����f�ɂ����錋�ʂ���������
	++p;
	++q;
	*p = *q;			// ���s�̏��f = ���ډ�f
    }
}
    
/************************************************************************
*  morphological operations						*
************************************************************************/
//! 3x3�E�B���h�E���̍ő�l��Ԃ��D
/*!
  \param p	���ړ_�̍���_���w�������q
  \param q	���ړ_�̍��̓_���w�������q
  \param r	���ړ_�̍����_���w�������q
  \return	3x3�E�B���h�E���̍ő�l
*/
template <class P> inline typename std::iterator_traits<P>::value_type
max3x3(P p, P q, P r)
{
    using namespace	std;
	    
    return max(max(*p, *(p + 1), *(p + 2)),
	       max(*q, *(q + 1), *(q + 2)),
	       max(*r, *(r + 1), *(r + 2)));
}
    
//! 3x3�E�B���h�E���̍ŏ��l��Ԃ��D
/*!
  \param p	���ړ_�̍���_���w�������q
  \param q	���ړ_�̍��̓_���w�������q
  \param r	���ړ_�̍����_���w�������q
  \return	3x3�E�B���h�E���̍ŏ��l
*/
template <class P> inline typename std::iterator_traits<P>::value_type
min3x3(P p, P q, P r)
{
    using namespace	std;
	    
    return min(min(*p, *(p + 1), *(p + 2)),
	       min(*q, *(q + 1), *(q + 2)),
	       min(*r, *(r + 1), *(r + 2)));
}

//! morphological open���Z��in-place�ōs���D
/*
  �w�肳�ꂽ�񐔂������k(erosion)���s������C�����񐔂����c��(dilation)���s���D
  \param begin	�ŏ��̍s�����������q
  \param end	�Ō�̍s�̎������������q
  \param niter	���k�Ɩc���̉�
*/
template <class Iterator> void
mopOpen(Iterator begin, Iterator end, u_int niter=1)
{
    typedef typename std::iterator_traits<Iterator>::value_type::iterator
								col_iterator;

    for (u_int n = 0; n < niter; ++n)
	op3x3(begin, end, min3x3<col_iterator>);	// ���k(erosion)
    for (u_int n = 0; n < niter; ++n)
	op3x3(begin, end, max3x3<col_iterator>);	// �c��(dilation)
}

//! morphological close���Z��in-place�ōs���D
/*
  �w�肳�ꂽ�񐔂����c��(dilation)���s������C�����񐔂������k(erosion)���s���D
  \param begin	�ŏ��̍s�����������q
  \param end	�Ō�̍s�̎������������q
  \param niter	���k�Ɩc���̉�
*/
template <class Iterator> void
mopClose(Iterator begin, Iterator end, u_int niter=1)
{
    typedef typename std::iterator_traits<Iterator>::value_type::iterator
								col_iterator;
    
    for (u_int n = 0; n < niter; ++n)
	op3x3(begin, end, max3x3<col_iterator>);	// �c��(dilation)
    for (u_int n = 0; n < niter; ++n)
	op3x3(begin, end, min3x3<col_iterator>);	// ���k(erosion)
}
    
}
#endif
