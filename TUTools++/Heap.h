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
 *  $Id: Heap.h,v 1.6 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		Heap.h
  \brief	�N���X TU::Heap �̒�`�Ǝ���
*/
#ifndef __TUHeap_h
#define __TUHeap_h

#include "TU/Array++.h"
#include <functional>

namespace TU
{
/************************************************************************
*  class Heap<T, Cmp>							*
************************************************************************/
//! �ő�q�[�v(�e�m�[�h�͎q�m�[�h���傫����������)��\���N���X
/*!
  �e���v���[�g�p�����[�^T�͗v�f�̌^���w�肷��D�e���v���[�g�p�����[�^Cmp
  ��2��T�^�v�f�������ɕ���ł��鎞��true��Ԃ��֐��I�u�W�F�N�g�^�ł���D
  ���Ƃ��΁C2��int�^���r����ꍇ��
  \verbatim
  struct ordered
  {
    bool	operator ()(const int& elm0, const int& elm1) const
		{
		    return elm0 < elm1;
		}
  };
  \endverbatim
  �ƒ�`����D
*/
template <class T, class Cmp=std::less<T> >
class Heap
{
  public:
    typedef T					value_type;
    typedef value_type&				reference;
    typedef const value_type&			const_reference;
    typedef value_type*				pointer;
    typedef const value_type*			const_pointer;
    typedef typename Array<T>::const_iterator	const_iterator;
    typedef Cmp					value_compare;
    typedef value_type				key_type;
    typedef value_compare			key_compare;
    
  public:
    explicit Heap(u_int d=0, const Cmp& cmp=Cmp())	;
    Heap(Array<T>& a, const Cmp& cmp=Cmp())		;

  //! �q�[�v�̐擪�v�f(�ő�v�f)���w�������q��Ԃ��D
  /*!
    begin() ���� end() �܂ł͍~���ɕ���ł���Ƃ͌���Ȃ����Ƃɒ��ӁD
    \return	�q�[�v�̐擪�v�f���w�������q
  */
    const_iterator	begin()		const	{return _array.begin();}

  //! �q�[�v�̖������w�������q��Ԃ��D
  /*!
    begin() ���� end() �܂ł͍~���ɕ���ł���Ƃ͌���Ȃ����Ƃɒ��ӁD
    \return	�q�[�v�̖������w�������q
  */
    const_iterator	end()		const	{return _array.begin() + _n;}

  //! ���݂̗v�f����Ԃ��D
  /*!
    \return	�v�f��
  */
    size_t	size()			const	{return _n;}

  //! �q�[�v�Ɋi�[�ł���ő�v�f����Ԃ��D
  /*!
    \return	�ő�v�f��
  */
    size_t	max_size()		const	{return _array.dim();}

  //! �q�[�v����ł��邩���ׂ�D
  /*!
    \return	��ł����true, �����łȂ����false
  */
    bool	empty()			const	{return _n == 0;}
    
  //! �q�[�v�����t�ł��邩���ׂ�D
  /*!
    \return	���t�ł����true, �����łȂ����false
  */
    bool	full()			const	{return _n == max_size();}
    
    const T&	top()			const	;
    void	push(const T& elm)		;
    bool	push_or_replace(const T& elm)	;
    T		pop()				;
    void	clear()				;
    void	resize(size_t d)		;
    
  private:
    void	upheap(u_int current)		;
    void	downheap(u_int current)		;
    
    Array<T>	_array;
    u_int	_n;		// # of elements in the heap.
    const Cmp	_cmp;
};

//! ��̃q�[�v�����D
/*!
  \param d	�q�[�v�̗e��
  \param cmp	��r�֐��I�u�W�F�N�g
*/
template <class T, class Cmp>
Heap<T, Cmp>::Heap(u_int d, const Cmp& cmp)
    :_array(d), _n(0), _cmp(cmp)
{
}

//! 1�����z�񂩂�q�[�v�����D
/*!
  \param a	1�����z��
  \param cmp	��r�֐��I�u�W�F�N�g
*/
template <class T, class Cmp>
Heap<T, Cmp>::Heap(Array<T>& a, const Cmp& cmp)
    :_array((T*)a, a.dim()), _n(a.dim()), _cmp(cmp)
{
    for (u_int i = _n / 2; i > 0; )
	downheap(--i);
}

//! �q�[�v�̐擪�v�f��Ԃ��D
/*!
  �q�[�v�̐擪�̓q�[�v���̍ő�v�f�ƂȂ�D
  \return	�擪�v�f
*/
template <class T, class Cmp> inline const T&
Heap<T, Cmp>::top() const
{
    if (_n == 0)
	throw std::out_of_range("TU::Heap<T, Cmp::top: Heap is empty!!");
    return _array[0];
}

//! �q�[�v�ɗv�f��ǉ�����D
/*!
  \param elm	�ǉ�����v�f
*/
template <class T, class Cmp> inline void
Heap<T, Cmp>::push(const T& elm)
{
    if (_n >= _array.dim())
	throw std::length_error("TU::Heap<T, Cmp>::push: Heap is full!!");

    _array[_n] = elm;
    upheap(_n++);
}

//! �w�肳�ꂽ�v�f��ǉ��܂��͐擪�v�f�Ɠ���ւ���D
/*!
  �q�[�v�����t�łȂ��Ƃ��͖������ɒǉ����C���t�̂Ƃ��͎w�肳�ꂽ�v�f��
  �擪�v�f�����������Ƃ��ɐ擪�v�f����菜���Ă���ǉ�����D
  \param elm	�ǉ�����v�f
  \return	elm�����ۂɒǉ����ꂽ��true, �����łȂ����false
*/
template <class T, class Cmp> inline bool
Heap<T, Cmp>::push_or_replace(const T& elm)
{
    if (!full())		// ���t�łȂ�������...
    {
	push(elm);		// elm��ǉ�����D
	return true;
     }
    else if (_cmp(elm, top()))	// ���t����elm���q�[�v�̍ő�v�f����
    {				// ���������...
	pop();			// ���̍ő�v�f����菜���C
	push(elm);		// elm��ǉ�����D
	return true;
    }
    return false;
}
    
//! �q�[�v�̐擪�v�f����菜���D
/*!
  �q�[�v�̐擪�̓q�[�v���̍ő�v�f�ł���C�������菜���D
  \return	��菜���ꂽ�v�f
*/
template <class T, class Cmp> inline T
Heap<T, Cmp>::pop()
{
    if (_n == 0)
	throw
	    std::out_of_range("TU::Heap<T, Cmp>::pop: Heap is empty!!");

    T	val = _array[0];			// Store the root value.
    _array[0] = _array[--_n];			// Move the last to the root.
    downheap(0);
    return val;
}

//! �q�[�v����ɂ���D
template <class T, class Cmp> inline void
Heap<T, Cmp>::clear()
{
    _n = 0;
}
    
//! �q�[�v�Ɋi�[�ł���ő�v�f����ύX����D
/*!
  ����܂ł̃q�[�v�̓��e�͂��ׂĔj�������D
  \param d	�ő�v�f��
*/
template <class T, class Cmp> inline void
Heap<T, Cmp>::resize(size_t d)
{
    _array.resize(d);
    _n = 0;
}
    
template <class T, class Cmp> void
Heap<T, Cmp>::upheap(u_int current)
{
    T	val = _array[current];
    while (current > 0)				// While having a parent...
    {
	u_int	parent = (current - 1) / 2;	// Index of the parent node.
	if (!_cmp(_array[parent], val))
	    break;
	
	_array[current] = _array[parent];	// Pull-down the parent.
	current = parent;			// Go upward.
    }
    _array[current] = val;
}

template <class T, class Cmp> void
Heap<T, Cmp>::downheap(u_int current)
{
    T	val = _array[current];
    for (u_int child; (child = 2 * current + 1) < _n; )
    {
	if (child + 1 < _n && _cmp(_array[child], _array[child + 1]))
	    ++child;				// Choose larger child.

	if (!_cmp(val, _array[child]))
	    break;

	_array[current] = _array[child];	// Pull-up the child.
	current = child;			// Go downward.
    }
    _array[current] = val;
}

/************************************************************************
*  Global functions							*
************************************************************************/
//! 1�����z����q�[�v�\�[�g�ɂ���ď����Ƀ\�[�g����D
/*!
  \param a	�\�[�g����z��
  \param cmp	��r�֐��I�u�W�F�N�g
*/
template <class T, class Cmp> void
sort(Array<T>& a, const Cmp& cmp)
{
    Heap<T, Cmp>	heap(a, cmp);

  // ��납�� a[] �ɑ�����Ȃ��� a[] �̗̈������ŋ��L���Ă���
  // �q�[�v���󂵂Ă��܂����Ƃɒ��ӁD
    for (u_int i = a.dim(); i > 0; )
	a[--i] = heap.pop();
}
 
}

#endif	// !__TUHeap_h
