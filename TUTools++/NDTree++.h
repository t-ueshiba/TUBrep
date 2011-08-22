/*
 *  $Id: NDTree++.h,v 1.3 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		NDTree++.h
  \brief	�N���X TU::NDTree �̒�`�Ǝ���
*/
#ifndef __TUNDTreePP_h
#define __TUNDTreePP_h

#include <stack>
#include "TU/Array++.h"

namespace TU
{
/************************************************************************
*  class NDTree<T, D>							*
************************************************************************/
//! D������Ԃ�\������2^D���؂�\���N���X
/*!
  \param T	�v�f�̌^
  \param D	��Ԃ̎����CD=2�̂Ƃ�quad tree, D=3�̂Ƃ�octree�ƂȂ�
 */
template <class T, u_int D>
class NDTree
{
  private:
    enum	{Dim = D, NChildren = (1 << D)};

    class	Node;
    class	Branch;
    class	Leaf;
    
  public:
    typedef T			value_type;	//!< �v�f�̌^
    typedef value_type&		reference;	//!< �v�f�ւ̎Q��
    typedef const value_type&	const_reference;//!< �萔�v�f�ւ̎Q��
    typedef value_type*		pointer;	//!< �v�f�ւ̃|�C���^
    typedef const value_type*	const_pointer;	//!< �萔�v�f�ւ̃|�C���^
    typedef Array<int, FixedSizedBuf<int, D> >
				position_type;	//!< ��Ԓ��̈ʒu

  //! 2^D���؂̂��߂̑O�i�����q
    template <class S>
    class Iterator : public std::iterator<std::forward_iterator_tag, S>
    {
      public:
	typedef S		value_type;	//!< �v�f�̌^
	typedef value_type&	reference;	//!< �v�f�ւ̎Q��
	typedef value_type*	pointer;	//!< �v�f�ւ̃|�C���^

      private:
	struct NodeInfo
	{
	    NodeInfo(Node* n, const position_type& p, u_int l)
		:node(n), dp(p), len(l)					{}
	    
	    Node*		node;		//!< �m�[�h�ւ̃|�C���^
	    position_type	dp;		//!< �m�[�h�̑��Έʒu
	    u_int		len;		//!< �m�[�h�̃Z����
	};
	
      public:
			Iterator()					;
			Iterator(const NDTree& tree)			;
	
	position_type	position()				const	;
	u_int		length()				const	;
	reference	operator *()				const	;
	pointer		operator ->()				const	;
	Iterator&	operator ++()					;
	Iterator	operator ++(int)				;
	bool		operator ==(const Iterator& iter)	const	;
	bool		operator !=(const Iterator& iter)	const	;

      private:
	position_type	new_dp(u_int idx)			const	;
	
      private:
	position_type		_org;		//!< 2^D���؂̌��_�̈ʒu
	Leaf*			_leaf;		//!< ���̔����q���w���Ă���t
	position_type		_dp;		//!< �t�̑��Έʒu
	u_int			_len;		//!< �t�̃Z����
	std::stack<NodeInfo>	_fringe;	//!< ���K��̃m�[�h����
    };

    typedef Iterator<value_type>	iterator;	//!< �����q
    typedef Iterator<const value_type>	const_iterator;	//!< �萔�����q

  public:
			NDTree()					;
			NDTree(const NDTree& tree)			;
			~NDTree()					;
    NDTree&		operator =(const NDTree& tree)			;

    const position_type&
			origin()				const	;
    u_int		length0()				const	;
    u_int		size()					const	;
    bool		empty()					const	;
    void		clear()						;
    const_pointer	find(const position_type& pos)		const	;
    void		insert(const position_type& pos,
			       const_reference val)			;
    void		erase(const position_type& pos)			;
    
    iterator		begin()						;
    const_iterator	begin()					const	;
    iterator		end()						;
    const_iterator	end()					const	;
    
    std::ostream&	put(std::ostream& out)			const	;
    std::istream&	get(std::istream& in)				;
    std::ostream&	print(std::ostream& out)		const	;
    
  private:
    bool		out_of_range(const position_type& pos)	const	;

  private:
    class Node
    {
      public:
	virtual		~Node()						;

	virtual Node*	clone()					 const	= 0;
	virtual u_int	size()					 const	= 0;
	virtual const_pointer
			find(const position_type& dp, u_int len) const	= 0;
	virtual void	insert(const position_type& dp,
			       const_reference val, u_int len)		= 0;
	virtual Node*	erase(const position_type& dp, u_int len)	= 0;
	virtual Branch* branch()				  	= 0;
	virtual Leaf*	leaf()					  	= 0;
	virtual void	print(std::ostream& out, u_int nindents) const	= 0;
	static Node*	create(const position_type& dp,
			       const_reference val, u_int len)		;
    };

    class Branch : public Node
    {
      public:
		        Branch()					;
        virtual		~Branch()					;

	virtual Node*	clone()					 const	;
	virtual u_int	size()					 const	;
	virtual const_pointer
			find(const position_type& dp, u_int len) const	;
	virtual void	insert(const position_type& dp,
			       const_reference val, u_int len)		;
	virtual Node*	erase(const position_type& dp, u_int len)	;
	virtual Branch* branch()				  	;
	virtual Leaf*	leaf()					  	;
	virtual void	print(std::ostream& out, u_int nindents) const	;
	static Node*	ascend(Node* node, u_int idx)			;
	Node*		descend(u_int& idx)				;

      private:
		        Branch(const Branch&)				;
        Branch&		operator =(const Branch&)			;

	static u_int	child_idx(const position_type& dp, u_int len)	;

	friend class	Iterator<value_type>;
	friend class	Iterator<const value_type>;

      private:
        Node*		_children[NChildren];
    };

    class Leaf : public Node
    {
      public:
			Leaf(const_reference val)			;
	virtual		~Leaf()						;
	
	virtual Node*	clone()					 const	;
	virtual u_int	size()					 const	;
	virtual const_pointer
			find(const position_type& dp, u_int len) const	;
	virtual void	insert(const position_type& dp,
			       const_reference val, u_int len)		;
	virtual Node*	erase(const position_type& dp, u_int len)	;
	virtual Branch* branch()				  	;
	virtual Leaf*	leaf()					  	;
	virtual void	print(std::ostream& out, u_int nindents) const	;

	friend class	Iterator<value_type>;
	friend class	Iterator<const value_type>;

      private:
        value_type	_val;
    };

  private:
    position_type	_org;
    u_int		_len0;
    Node*		_root;
};

/************************************************************************
*  class NDTree<T, D>							*
************************************************************************/
//! D������Ԃ�\������2^D���؂𐶐�����D
template <class T, u_int D> inline
NDTree<T, D>::NDTree()
    :_org(), _len0(0), _root(0)
{
}

//! �R�s�[�R���X�g���N�^
/*!
  \param tree	�R�s�[����2^D����
*/
template <class T, u_int D> inline
NDTree<T, D>::NDTree(const NDTree& tree)
    :_org(tree._org), _len0(tree._len0), _root(0)
{
    if (tree._root)
	_root = tree._root->clone();
}

//! �f�X�g���N�^
template <class T, u_int D> inline
NDTree<T, D>::~NDTree()
{
    delete _root;
}

//! ������Z�q
/*!
  \param tree	�R�s�[����2^D����
  \return	����2^D����
*/
template <class T, u_int D> inline NDTree<T, D>&
NDTree<T, D>::operator =(const NDTree& tree)
{
    if (this != &tree)
    {
	_org  = tree._org;
	_len0 = tree._len0;
	_root = (tree._root ? tree._root->clone() : 0);
    }
    
    return *this;
}

//! ����2^D���؂�root�Z���̌��_�ʒu��Ԃ��D
/*!
  \return	root�Z���̌��_�ʒu
*/
template <class T, u_int D> inline const typename NDTree<T, D>::position_type&
NDTree<T, D>::origin() const
{
    return _org;
}

//! ����2^D���؂�root�Z���̈�ӂ̒�����Ԃ��D
/*!
  \return	root�Z���̈�ӂ̒���
*/
template <class T, u_int D> inline u_int
NDTree<T, D>::length0() const
{
    return _len0;
}

//! ����2^D���ؒ��̗t�̐���Ԃ��D
/*!
  \return	�t�̐�
*/
template <class T, u_int D> inline u_int
NDTree<T, D>::size() const
{
    return (_root ? _root->size() : 0);
}

//! ����2^D���؂���ł��邩���ׂ�D
/*!
  \return	��ł����true, �����łȂ����false
*/
template <class T, u_int D> inline bool
NDTree<T, D>::empty() const
{
    return !_root;
}

//! ����2^D���؂���ɂ���D
template <class T, u_int D> inline void
NDTree<T, D>::clear()
{
    delete _root;
    _root = 0;
    _len0 = 0;
}

//! D������Ԓ��̎w�肳�ꂽ�ʒu�ɂ�����l��T���D
/*!
  \param pos	D������Ԓ��̈ʒu
  \return	pos�Ŏw�肳�ꂽ�ʒu�ɗt�����݂���΂��̒l�ւ̃|�C���^
		��Ԃ��D���݂��Ȃ����0��Ԃ��D
*/
template <class T, u_int D> inline typename NDTree<T, D>::const_pointer
NDTree<T, D>::find(const position_type& pos) const
{
    if (!_root || out_of_range(pos))
	return 0;
    
    return _root->find(position_type(pos) -= _org, _len0);
}

//! D������Ԓ��̎w�肳�ꂽ�ʒu�ɒl���i�[����D
/*!
  �w�肳�ꂽ�ʒu�ɗt���Ȃ���ΐV���ɗt������C�����ɒl���i�[�����D
  \param pos	D������Ԓ��̈ʒu
  \param val	�i�[����l
*/
template <class T, u_int D> void
NDTree<T, D>::insert(const position_type& pos, const_reference val)
{
    if (_root)
    {
	for (;;)
	{
	    bool	ascend = false;
	    u_int	idx = 0;
	    for (u_int d = 0; d < Dim; ++d)
		if (pos[d] < _org[d])		// �������Ɉ�E�Ȃ�...
		{
		    ascend = true;		// _root�̏��K���K�v
		    _org[d] -= int(_len0);	// ���_�𕉕����Ɉړ�
		    idx |= (1 << d);
		}
		else if (pos[d] >= _org[d] + int(_len0))  // �������Ɉ�E�Ȃ�...
		    ascend = true;		// _root�̏��K���K�v
	    if (!ascend)			// _root�̏��K���s�v�Ȃ��
		break;				// �����ɒE�o

	    _len0 <<= 1;			// _root�̃Z������2�{�ɂ���D

	    _root = Branch::ascend(_root, idx);	// _root�����K
	}
	
	_root->insert(position_type(pos) -= _org, val, _len0);
    }
    else
    {
	_len0 = 1;
	_org  = pos;

	_root = Node::create(position_type(pos) -= _org, val, _len0);
    }
}

//! D������Ԓ��̎w�肳�ꂽ�ʒu�̗t����������D
/*!
  \param pos	D������Ԓ��̈ʒu
*/
template <class T, u_int D> void
NDTree<T, D>::erase(const position_type& pos)
{
    if (!_root || out_of_range(pos))
	return;
    
  // pos�̈ʒu�ɂ���t�������D	
    _root = _root->erase(position_type(pos) -= _org, _len0);

    if (!_root)					// ��Ȃ��...
    {
	_len0 = 0;				// _root�̃Z������0�ɂ���
	return;					// �����Ƀ��^�[��
    }

  // _root���q��1�������}�Ȃ�΁C�q��_root�ɕt���ւ��邱�Ƃɂ��~�K����D
    for (Branch* b; b = _root->branch(); )	// _root���}�Ȃ��...
    {
	u_int	idx;
	Node*	child = b->descend(idx);	// 1�i���̍~�K�����݂�D
	if (!child)				// �~�K�ł��Ȃ����...
	    break;				// �����ɒE�o

	_root = child;				// �q��_root�ɕt���ւ���
		
	_len0 >>= 1;				// _root�̃Z�����𔼕��ɂ���D
	position_type	dp;			// �Â�root���猩���q�̑��Έʒu
	for (u_int d = 0; d < Dim; ++d)			// �q��index��
	    dp[d] = (idx & (1 << d) ? _len0 : 0);	// ���Έʒu�ɕϊ�
	_org += dp;				// ���_���q�̈ʒu�Ɉڂ��D
    }
}

//! 2^D���؂̐擪�v�f���w�������q��Ԃ��D
/*!
  \return	�擪�v�f���w�������q
*/
template <class T, u_int D> inline typename NDTree<T, D>::iterator
NDTree<T, D>::begin()
{
    return iterator(*this);
}

//! 2^D���؂̐擪�v�f���w���萔�����q��Ԃ��D
/*!
  \return	�擪�v�f���w���萔�����q
*/
template <class T, u_int D> inline typename NDTree<T, D>::const_iterator
NDTree<T, D>::begin() const
{
    return const_iterator(*this);
}

//! 2^D���؂̖������w�������q��Ԃ��D
/*!
  \return	�������w�������q
*/
template <class T, u_int D> inline typename NDTree<T, D>::iterator
NDTree<T, D>::end()
{
    return iterator();
}

//! 2^D���؂̖������w���萔�����q��Ԃ��D
/*!
  \return	�������w���萔�����q
*/
template <class T, u_int D> inline typename NDTree<T, D>::const_iterator
NDTree<T, D>::end() const
{
    return const_iterator();
}

//! �o�̓X�g���[����2^D���؂������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, u_int D> std::ostream&
NDTree<T, D>::put(std::ostream& out) const
{
    using namespace	std;
    
    for (const_iterator iter = begin(); iter != end(); ++iter)
	iter.position().put(out) << '\t' << iter.length() << '\t'
				 << *iter << endl;

    return out;
}
    
//! ���̓X�g���[������2^D���؂�ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, u_int D> std::istream&
NDTree<T, D>::get(std::istream& in)
{
    clear();					// �����̑S�Z����p��

    for (position_type pos; in >> pos; )	// �t�̈ʒu��ǂݍ���
    {
	u_int		len;
	value_type	val;
	in >> len >> val;			// �t�̃Z�����ƒl��ǂݍ��ށD
	insert(pos, val);			// �w�肳�ꂽ�ʒu�ɒl��}��
    }

    return in;
}

//! �o�̓X�g���[����2^D���؂̍\���������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, u_int D> inline std::ostream&
NDTree<T, D>::print(std::ostream& out) const
{
    if (_root)
	_root->print(out, 0);

    return out;
}
    
template <class T, u_int D> bool
NDTree<T, D>::out_of_range(const position_type& pos) const
{
    for (u_int d = 0; d < Dim; ++d)
	if ((pos[d] < _org[d]) || (pos[d] >= _org[d] + int(_len0)))
	    return true;
    return false;
}

/************************************************************************
*  class NDTree<T, D>::Iterator<S>					*
************************************************************************/
//! �����w���Ȃ�2^D���؂̂��߂̔����q�����D
template <class T, u_int D> template <class S> inline
NDTree<T, D>::Iterator<S>::Iterator()
    :_org(), _leaf(0), _dp(), _len(0)
{
}

//! 2^D���؂̂��߂̔����q�����D
/*!
  �����q��2^D���؂̐擪�v�f���w���悤�ɏ����������D
  \param tree	2^D����
*/
template <class T, u_int D> template <class S> inline
NDTree<T, D>::Iterator<S>::Iterator(const NDTree& tree)
    :_org(), _leaf(0), _dp(), _len(0)
{
    if (tree._root)
    {
	_org = tree._org;
	_dp  = 0;
	_len = tree._len0;
	_fringe.push(NodeInfo(tree._root, _dp, _len));
	++*this;			// �ŏ��̗t���w���܂Ŕ����q��i�߂�
    }
}

//! ���̔����q���w���l�����߂��Ă���t�̈ʒu��Ԃ��D
/*!
  \return	�t�̈ʒu
*/
template <class T, u_int D> template <class S>
inline typename NDTree<T, D>::position_type
NDTree<T, D>::Iterator<S>::position() const
{
    return position_type(_org) += _dp;
}

//! ���̔����q���w���l�����߂��Ă���t�̃Z������Ԃ��D
/*!
  \return	�t�̃Z����
*/
template <class T, u_int D> template <class S> inline u_int
NDTree<T, D>::Iterator<S>::length() const
{
    return _len;
}

//! ���̔����q���w���l�ւ̎Q�Ƃ�Ԃ��D
/*!
  \return	�l�ւ̎Q��
*/
template <class T, u_int D> template <class S> inline S&
NDTree<T, D>::Iterator<S>::operator *() const
{
    return _leaf->_val;
}

//! ���̔����q���w���l�ւ̃|�C���^��Ԃ��D
/*!
  \return	�l�ւ̃|�C���^
*/
template <class T, u_int D> template <class S> inline S*
NDTree<T, D>::Iterator<S>::operator ->() const
{
    return &(operator *());
}

//! ���̔����q���C���N�������g����(�O�u)�D
/*!
  \return	�C���N�������g��̂��̔����q
*/
template <class T, u_int D> template <class S> NDTree<T, D>::Iterator<S>&
NDTree<T, D>::Iterator<S>::operator ++()
{
  // 1. _leaf != 0			�����q����łȂ��t���w���Ă�����
  // 2. _leaf == 0 && _fringe.emtpy()	�����q�������ɓ��B���Ă�����
  // 3. _leaf == 0 && !_fringe.empty()	���Lwhile���[�v�̓r��
    
    _leaf = 0;					// �ߋ��̗t���̂Ă�D
    
    while (!_fringe.empty())
    {
	Node*	node = _fringe.top().node;	// �擪�m�[�h�C
	_dp  = _fringe.top().dp;		// ���̃Z���ʒu
	_len = _fringe.top().len;		// ����уZ������
	_fringe.pop();				// pop����D

	if (_leaf = node->leaf())		// pop�����m�[�h���t�ł����...
	    break;				// �C���N�������g��̈ʒu�ɓ��B
	
	Branch* branch = node->branch();	// pop�����m�[�h�͎}
	_len >>= 1;				// �q�̃��x����1�i�~��
	    
	for (int i = NChildren; --i >= 0; )
	    if (branch->_children[i])		// ��łȂ��q���t����push
		_fringe.push(NodeInfo(branch->_children[i],
				      new_dp(i), _len));
    }

    return *this;
}

//! ���̔����q���C���N�������g����(��u)�D
/*!
  \return	�C���N�������g�O�̔����q
*/
template <class T, u_int D> template <class S> inline NDTree<T, D>::Iterator<S>
NDTree<T, D>::Iterator<S>::operator ++(int)
{
    Iterator	tmp = *this;
    ++*this;
    return tmp;
}

//! ���̔����q�Ɨ^����ꂽ�����q������̗v�f���w���Ă��邩���ׂ�D
/*!
  \param iter	��r�Ώۂ̔����q
  \return	����̗v�f���w���Ă����true, �����łȂ����false
*/
template <class T, u_int D> template <class S> inline bool
NDTree<T, D>::Iterator<S>::operator ==(const Iterator& iter) const
{
    return _leaf == iter._leaf;
}

//! ���̔����q�Ɨ^����ꂽ�����q���قȂ�v�f���w���Ă��邩���ׂ�D
/*!
  \param iter	��r�Ώۂ̔����q
  \return	�قȂ�v�f���w���Ă����true, �����łȂ����false
*/
template <class T, u_int D> template <class S> inline bool
NDTree<T, D>::Iterator<S>::operator !=(const Iterator& iter) const
{
    return !operator ==(iter);
}

template <class T, u_int D> template <class S>
typename NDTree<T, D>::position_type
NDTree<T, D>::Iterator<S>::new_dp(u_int idx) const
{
    position_type	dp = _dp;
    for (u_int d = 0; d < Dim; ++d)
	if (idx & (1 << d))
	    dp[d] |= _len;

    return dp;
}

/************************************************************************
*  class NDTree<T, D>::Node						*
************************************************************************/
template <class T, u_int D>
NDTree<T, D>::Node::~Node()
{
}

template <class T, u_int D> inline typename NDTree<T, D>::Node*
NDTree<T, D>::Node::create(const position_type& dp,
			   const_reference val, u_int len)
{
    if (len != 1)
    {
	Branch*	b = new Branch;
	b->insert(dp, val, len);	// �V���ɐ��������m�[�h�ɑ}������D
	return b;
    }
    else
	return new Leaf(val);
}

/************************************************************************
*  class NDTree<T, D>::Branch						*
************************************************************************/
template <class T, u_int D>
NDTree<T, D>::Branch::Branch()
{
    for (u_int i = 0; i < NChildren; ++i)
	_children[i] = 0;
}

template <class T, u_int D>
NDTree<T, D>::Branch::~Branch()
{
    for (u_int i = 0; i < NChildren; ++i)
	delete _children[i];
}

template <class T, u_int D> typename NDTree<T, D>::Node*
NDTree<T, D>::Branch::clone() const
{
    Branch*	b = new Branch;
    for (u_int i = 0; i < NChildren; ++i)
	if (_children[i])
	    b->_children[i] = _children[i]->clone();
    return b;
}
    
template <class T, u_int D> u_int
NDTree<T, D>::Branch::size() const
{
    u_int	n = 0;
    for (u_int i = 0; i < NChildren; ++i)
	if (_children[i])
	    n += _children[i]->size();
    return n;
}

template <class T, u_int D> typename NDTree<T, D>::const_pointer
NDTree<T, D>::Branch::find(const position_type& dp, u_int len) const
{
    len >>= 1;						// 1���̃��x����
    const Node*	child = _children[child_idx(dp, len)];	// �q
    
    return (child ? child->find(dp, len) : 0);
}

template <class T, u_int D> void
NDTree<T, D>::Branch::insert(const position_type& dp,
			     const_reference val, u_int len)
{
    len >>= 1;						// 1���̃��x����
    Node*&	child = _children[child_idx(dp, len)];	// �q

    if (child)						// �q�������...
	child->insert(dp, val, len);			// �����̎q�ɑ}��
    else						// �q���Ȃ����...
	child = create(dp, val, len);			// �V���Ɏq������đ}��
}

template <class T, u_int D> typename NDTree<T, D>::Node*
NDTree<T, D>::Branch::erase(const position_type& dp, u_int len)
{
    len >>= 1;						// 1���̃��x����
    Node*&	child = _children[child_idx(dp, len)];	// �q

    if (!child)						// �q���Ȃ����...
	return this;					// ���̎}���g��Ԃ�
    child = child->erase(dp, len);			// �q���������

    for (u_int i = 0; i < NChildren; ++i)
	if (_children[i])	// �q��1�ł��c���Ă����...
	    return this;	// ���̎}���g��Ԃ�

    delete this;		// �S�Ă̎q����Ȃ̂ŁC���̎}���g�����
    return 0;			// 0��Ԃ�
}
    
template <class T, u_int D> typename NDTree<T, D>::Branch*
NDTree<T, D>::Branch::branch()
{
    return this;
}
    
template <class T, u_int D> typename NDTree<T, D>::Leaf*
NDTree<T, D>::Branch::leaf()
{
    return 0;
}
    
template <class T, u_int D> void
NDTree<T, D>::Branch::print(std::ostream& out, u_int nindents) const
{
    using namespace	std;

    out << endl;
    for (u_int i = 0; i < NChildren; ++i)
    {
	for (u_int n = 0; n < nindents; ++n)
	    out << ' ';
	out << '[' << i << "]: ";
	if (_children[i])
	    _children[i]->print(out, nindents + 2);
	else
	    out << "NULL"<< endl;
    }
}
    
template <class T, u_int D> typename NDTree<T, D>::Node*
NDTree<T, D>::Branch::ascend(Node* node, u_int idx)
{
    Branch*	b = new Branch;
    b->_children[idx] = node;

    return b;
}
    
template <class T, u_int D> typename NDTree<T, D>::Node*
NDTree<T, D>::Branch::descend(u_int& idx)
{
    u_int	nchildren = 0;
    for (u_int i = 0; i < NChildren; ++i)
	if (_children[i])
	{
	    ++nchildren;		// �q�̐���
	    idx = i;			// ����index�𒲂ׂ�D
	}
    if (nchildren != 1)			// �q��1�łȂ����...
	return 0;			// �K�w�͌��点�Ȃ��̂ŒE�o

    Node*	root = _children[idx];	// ���̎q��V����root�ɂ���1�K�w�팸
    
    _children[idx] = 0;			// �q�ւ̃����N�������Ă���
    delete this;			// ���g���������D

    return root;			// �V����root��Ԃ��D
}
    
template <class T, u_int D> u_int
NDTree<T, D>::Branch::child_idx(const position_type& dp, u_int len)
{
    u_int	i = 0;
    for (int d = 0; d < Dim; ++d)
	if (dp[d] & len)
	    i |= (1 << d);

    return i;
}
    
/************************************************************************
*  class NDTree<T, D>::Leaf						*
************************************************************************/
template <class T, u_int D> inline
NDTree<T, D>::Leaf::Leaf(const_reference val)
    :_val(val)
{
}

template <class T, u_int D>
NDTree<T, D>::Leaf::~Leaf()
{
}

template <class T, u_int D> typename NDTree<T, D>::Node*
NDTree<T, D>::Leaf::clone() const
{
    return new Leaf(*this);
}
    
template <class T, u_int D> u_int
NDTree<T, D>::Leaf::size() const
{
    return 1;
}
    
template <class T, u_int D> typename NDTree<T, D>::const_pointer
NDTree<T, D>::Leaf::find(const position_type&, u_int len) const
{
    if (len != 1)
	throw std::logic_error("NDTree<T, D>::Leaf::find: non-zero \'len\'!");
    return &_val;
}

template <class T, u_int D> void
NDTree<T, D>::Leaf::insert(const position_type& pos,
			   const_reference val, u_int len)
{
    if (len != 1)
	throw std::logic_error("NDTree<T, D>::Leaf::insert: non-zero \'len\'!");
    _val = val;
}

template <class T, u_int D> typename NDTree<T, D>::Node*
NDTree<T, D>::Leaf::erase(const position_type&, u_int len)
{
    if (len != 1)
	throw std::logic_error("NDTree<T, D>::Leaf::erase: non-zero \'len\'!");
    delete this;				// ���̗t���g�����
    return 0;
}

template <class T, u_int D> typename NDTree<T, D>::Branch*
NDTree<T, D>::Leaf::branch()
{
    return 0;
}
    
template <class T, u_int D> typename NDTree<T, D>::Leaf*
NDTree<T, D>::Leaf::leaf()
{
    return this;
}
    
template <class T, u_int D> void
NDTree<T, D>::Leaf::print(std::ostream& out, u_int nindents) const
{
    out << _val << std::endl;
}

/************************************************************************
*  global fucntions							*
************************************************************************/
//! ���̓X�g���[������2^D���؂�ǂݍ���(ASCII)�D
/*!
  \param in	���̓X�g���[��
  \param tree	2^D���؂̓ǂݍ��ݐ�
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, u_int D> inline std::istream&
operator >>(std::istream& in, NDTree<T, D>& tree)
{
    return tree.get(in);
}

//! �o�̓X�g���[����2^D���؂������o��(ASCII)�D
/*!
  \param out	�o�̓X�g���[��
  \param tree	�����o��2^D����
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, u_int D> inline std::ostream&
operator <<(std::ostream& out, const NDTree<T, D>& tree)
{
    return tree.put(out);
}

}
#endif
