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
 *  $Id: Heap.h,v 1.3 2009-07-31 07:04:44 ueshiba Exp $
 */
#ifndef __TUHeap_h
#define __TUHeap_h

#include "TU/Array++.h"

namespace TU
{
/************************************************************************
*  class Heap<T, Compare>						*
************************************************************************/
//! �����̗v�f���\�[�g����q�[�v��\���N���X
/*!
  �e���v���[�g�p�����[�^T�͗v�f�̌^���w�肷��D�e���v���[�g�p�����[�^Compare
  ��2��T�^�v�f�������ɕ���ł��鎞��true��Ԃ��֐��I�u�W�F�N�g�^�ł���D
  ���Ƃ��΁C2��int�^���r����ꍇ��
  \verbatim
  struct ordered
  {
    bool	operator ()(const int& item0, const int& item1) const
		{
		    return item0 < item1;
		}
  };
  \endverbatim
  �ƒ�`����D
*/
template <class T, class Compare>
class Heap
{
  public:
    Heap(u_int d, Compare compare)	;
    Heap(Array<T>& a, Compare compare)	;

  //! ���݂̗v�f����Ԃ�
  /*!
    \return	�v�f��
  */
    u_int	nelements()	const	{return _n;}

  //! �q�[�v�擪�̗v�f��Ԃ�
  /*!
    \return	�擪�̗v�f
  */
    T		head()		const	{return (_n != 0 ? _array[0] : 0);}

    void	add(T item)		;
    T		detach()		;
    
  private:
    void	upheap(int current)	;
    void	downheap(int current)	;
    
    Array<T>		_array;
    u_int		_n;		// # of items in the heap.
    const Compare	_compare;	// function for comparing two items.
};

//! ��̃q�[�v�����
/*!
  \param d		�q�[�v�̍ő�v�f��
  \param compare	��r�֐��I�u�W�F�N�g
*/
template <class T, class Compare>
Heap<T, Compare>::Heap(u_int d, Compare compare)
    :_array(d), _n(0), _compare(compare)
{
}

//! 1�����z�񂩂�q�[�v�����
/*!
  \param a		1�����z��
  \param compare	��r�֐��I�u�W�F�N�g
*/
template <class T, class Compare>
Heap<T, Compare>::Heap(Array<T>& a, Compare compare)
    :_array((T*)a, a.dim()), _n(a.dim()), _compare(compare)
{
    for (int i = _n / 2; --i >= 0; )
	downheap(i);
}

//! �q�[�v�ɗv�f��ǉ�����
/*!
  \param item	�ǉ�����v�f
*/
template <class T, class Compare> void
Heap<T, Compare>::add(T item)
{
    if (_n >= _array.dim())
	throw std::length_error("TU::Heap<T, Compare>::add: Heap is full!!");

    _array[_n] = item;
    upheap(_n++);
}

//! �q�[�v�擪�̗v�f����菜��
/*!
  \return	��菜���ꂽ�v�f
*/
template <class T, class Compare> T
Heap<T, Compare>::detach()
{
    if (_n == 0)
	throw
	    std::out_of_range("TU::Heap<T, Compare>::detach: Heap is empty!!");
    
    T	val = _array[0];			// Store the root value.
    _array[0] = _array[--_n];			// Move the last to the root.
    downheap(0);
    return val;
}

template <class T, class Compare> void
Heap<T, Compare>::upheap(int current)
{
    T	val = _array[current];
    while (current > 0)				// While having a parent...
    {
	int	parent = (current - 1) / 2;	// Index of the parent node.
	if (_compare(val, _array[parent]))
	    break;
	
	_array[current] = _array[parent];	// Pull-down the parent.
	current = parent;			// Go upward.
    }
    _array[current] = val;
}

template <class T, class Compare> void
Heap<T, Compare>::downheap(int current)
{
    T	val = _array[current];
    for (int child; (child = 2 * current + 1) < _n; )
    {
	if (child + 1 < _n && _compare(_array[child], _array[child + 1]))
	    ++child;				// Choose larger child.

	if (_compare(_array[child], val))
	    break;

	_array[current] = _array[child];	// Pull-up the child.
	current = child;			// Go downward.
    }
    _array[current] = val;
}

/************************************************************************
*  Global functions							*
************************************************************************/
//! 1�����z��̃q�[�v�\�[�g���s��
/*!
  \param a		�\�[�g����z��
  \param compare	��r�֐��I�u�W�F�N�g
*/
template <class T, class Compare> void
sort(Array<T>& a, Compare compare)
{
    Heap<T, Compare>	heap(a, compare);
    for (int i = a.dim(); i > 0; )
	a[--i] = heap.detach();
}
 
}

#endif	// !__TUHeap_h
