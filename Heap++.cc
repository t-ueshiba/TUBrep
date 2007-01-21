/*
 *  $Id: Heap++.cc,v 1.3 2007-01-21 23:36:36 ueshiba Exp $
 */
#include "TU/Heap++.h"
#include <stdexcept>

namespace TU
{
/************************************************************************
*  class Heap<T, Compare>						*
************************************************************************/
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
