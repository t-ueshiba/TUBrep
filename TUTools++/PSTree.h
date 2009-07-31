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
 *  $Id: PSTree.h,v 1.3 2009-07-31 07:04:45 ueshiba Exp $
 */
#ifndef __TUPSTree_h
#define __TUPSTree_h

#include "TU/Heap.h"
#include "TU/List.h"

namespace TU
{
/************************************************************************
*  class PSTree<S, T, CompareX, CompareY>				*
************************************************************************/
/*
 *  PSTree �́A�Q�̋��S���� compareX, compareY�i�C�ӂ� p, q �ɑ΂���
 *  compareX(p, q), compareX(q, p), p == q �̂ǂꂩ�P���������藧���A
 *  �����ڗ��𖞂����B����āAcompareX(p, p) �͏�ɋU�B�j���^�����
 *  ���Ƃ�,
 *	1. �e�m�[�h _p �Ǝq�m�[�h _q �ɑ΂��� !compareX(_q, _p)
 *	2. �������� _l �ƉE������ _r �ɑ΂��� !compareY(_r, _l)
 *  �����������悤�ɍ\�������B�e�m�[�h�Ǝq�m�[�h�̊Ԃɂ� compareY
 *  �ɂ��ĉ��猈�܂����֌W�͖����B
 */
template <class S, class T, class CompareX, class CompareY>
class PSTree
{
  public:
    class ListNode : public TU::List<ListNode>::Node
    {
      public:
	ListNode(const T& pp)	:p(pp)	{}

	const T&	p;
    };

    typedef TU::List<ListNode>	List;

    class Node
    {
      public:
	Node(Array<T>& array, int il, int ir, CompareX compareX);
	~Node()						{delete _r; delete _l;}

	const T&	p()				const	{return _p;}
	const Node*	closestY(const S& q,
				 CompareX compareX,
				 CompareY compareY)	const	;
	void		inRegion(const S& q,
				 List& list,
				 CompareX compareX,
				 CompareY compareY)	const	;
	void		inRegion(const S& p,
				 const S& q,
				 List& list,
				 CompareX compareX,
				 CompareY compareY)	const	;
	void		print(std::ostream& out,
			      char kind)		const	;
	
      private:
	static int	shift(Array<T>& array, int il, int ir,
			      CompareX compareX)		;
	static int	middle(int il, int ir)			;
	
	const T		_p;
	const T		_b;
	Node*		_l;
	Node*		_r;
    };

  public:
    PSTree(Array<T>& array, CompareX compareX, CompareY compareY)	;
    ~PSTree()						{delete _root;}

  /*
   *  compareX(_p, q) && compareY(_p, q) �𖞂����m�[�h _p �̂����A
   *  ���̂ǂ� _p' �ɑ΂��Ă� !compareY(_p, _p') �ƂȂ���̂�Ԃ��B
   */
    const T*	closestY(const S& q)		 const	;
  /*
   *  compareX(_p, q) && compareY(_p, q) �𖞂����m�[�h _p �S�Ă�
   *  �񋓂��� list �ɕԂ��B
   */
    void	inRegion(const S& q, List& list) const	;
  /*
   *  !compareX(_p, p) && !compareY(_p, p) &&
   *   compareX(_p, q) &&  compareY(_p, q) �𖞂����m�[�h _p �S�Ă�
   *  �񋓂��� list �ɕԂ��B
   */
    void	inRegion(const S& p,
			 const S& q,
			 List& list)	 const	;
    void	print(std::ostream& out) const	{_root->print(out, 'C');}
    
  private:
    Array<T>&		_array;
    Node*		_root;
    const CompareX	_compareX;
    const CompareY	_compareY;
};

template <class S, class T, class CompareX, class CompareY>
PSTree<S, T, CompareX, CompareY>::PSTree(Array<T>& array,
					 CompareX compareX,
					 CompareY compareY)
    :_array(array), _root(0), _compareX(compareX), _compareY(compareY)
{
    if (array.dim() > 0)
    {
	sort(array, _compareY);
	_root = new Node(array, 0, array.dim(), _compareX);
    }
}

template <class S, class T, class CompareX, class CompareY> const T*
PSTree<S, T, CompareX, CompareY>::closestY(const S& q) const
{
    const Node*	node = _root->closestY(q, _compareX, _compareY);
    return (node != 0 ? &node->p() : 0);
}

template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::inRegion(const S& q, List& list) const
{
    _root->inRegion(q, list, _compareX, _compareY);
}

template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::inRegion(const S& p, const S& q,
					   List& list) const
{
    _root->inRegion(p, q, list, _compareX, _compareY);
}

/*
 *  class PSTree<S, T, CompareX, CompareY>::Node
 */
template <class S, class T, class CompareX, class CompareY>
PSTree<S, T, CompareX, CompareY>::Node::Node(Array<T>& array,
					     int il, int ir,
					     CompareX compareX)
    :_p(array[shift(array, il, ir, compareX)]), _b(array[middle(il, ir)]),
     _l(0), _r(0)
{
    int	ib = (il + 1 + ir) / 2;
    if (il + 1 < ib)
	_l = new Node(array, il+1, ib, compareX);
    if (ib < ir)
	_r = new Node(array, ib, ir, compareX);
}

template <class S, class T, class CompareX, class CompareY> int
PSTree<S, T, CompareX, CompareY>::Node::shift(Array<T>& array,
					      int il, int ir,
					      CompareX compareX)
{
    int im = il;
    for (int i = il+1; i < ir; ++i)	// Find the left-most element.
	if (compareX(array[i], array[im]))
	    im = i;

    T	tmp = array[im];
    while (im-- > il)	// Shift all the elements left of im to right.
	array[im+1] = array[im];
    array[il] = tmp;
    
    return il;
}

template <class S, class T, class CompareX, class CompareY> inline int
PSTree<S, T, CompareX, CompareY>::Node::middle(int il, int ir)
{
    int	ib = (il + 1 + ir) / 2;
    return (ib < ir ? ib : ir - 1);
}
 
/*
 *  compareX(_p, q) && compareY(_p, q) �𖞂����m�[�h _p �̂����A
 *  ���̂ǂ� _p' �ɑ΂��Ă� !compareY(_p, _p') �ƂȂ���̂�Ԃ��B
 */
template <class S, class T, class CompareX, class CompareY>
const typename PSTree<S, T, CompareX, CompareY>::Node*
PSTree<S, T, CompareX, CompareY>::Node::closestY(const S& q,
						 CompareX compareX,
						 CompareY compareY) const
{
    if (!compareX(_p, q))
	return 0;

    const Node*	node = (_r != 0 ? _r->closestY(q, compareX, compareY) : 0);
    if (node == 0 && _l != 0)
	node = _l->closestY(q, compareX, compareY);
    if (compareY(_p, q) && (node == 0 || !compareY(_p, node->p())))
	return this;
    else
	return node;
}

/*
 *  compareX(_p, q) && compareY(_p, q) �𖞂����m�[�h _p �S�Ă�
 *  �񋓂��� list �ɕԂ��B
 */
template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::Node::inRegion(const S& q, List& list,
						 CompareX compareX,
						 CompareY compareY) const
{
    if (!compareX(_p, q))
	return;

    if (_l != 0)
	_l->inRegion(q, list, compareX, compareY);
    if (_r != 0 && compareY(_b, q))
	_r->inRegion(q, list, compareX, compareY);
    if (compareY(_p, q))
	list.push_front(*new ListNode(_p));
}

/*
 *  !compareX(_p, p) && !compareY(_p, p) &&
 *   compareX(_p, q) &&  compareY(_p, q) �𖞂����m�[�h _p �S�Ă�
 *  �񋓂��� list �ɕԂ��B
 */
template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::Node::inRegion(const S& p, const S& q,
						 List& list,
						 CompareX compareX,
						 CompareY compareY) const
{
    if (!compareX(_p, q))
	return;

    if (_l != 0 && !compareY(_b, p))
	_l->inRegion(p, q, list, compareX, compareY);
    if (_r != 0 && compareY(_b, q))
	_r->inRegion(p, q, list, compareX, compareY);
    if (!compareX(_p, p) && !compareY(_p, p) && compareY(_p, q))
	list.push_front(*new ListNode(_p));
}

template <class S, class T, class CompareX, class CompareY> void
PSTree<S, T, CompareX, CompareY>::Node::print(std::ostream& out,
					      char kind) const
{
    static int	indent = 0;
    
    if (this == 0)
	return;
    
    for (int i = 0; i < indent; ++i)
	out << ' ';
    out << kind << ": " << _p;

    ++indent;
    _l->print(out, 'L');
    _r->print(out, 'R');
    --indent;
}
 
}
#endif	// !__TUPSTree_h
