/*
 *  $Id: Heap++.h,v 1.3 2007-01-21 23:36:36 ueshiba Exp $
 */
#ifndef __TUHeapPP_h
#define __TUHeapPP_h

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

/************************************************************************
*  Global functions							*
************************************************************************/
template <class T, class Compare> void
sort(Array<T>& a, Compare compare)	;
 
}

#endif	// !__TUHeapPP_h
