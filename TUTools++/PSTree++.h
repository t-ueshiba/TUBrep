/*
 *  $Id: PSTree++.h,v 1.2 2002-07-25 02:38:06 ueshiba Exp $
 */
#ifndef __TUPSTreePP_h
#define __TUPSTreePP_h

#include "TU/Heap++.h"
#include "TU/List++.h"

namespace TU
{
/************************************************************************
*  class PSTree<S, T, CompareX, CompareY>				*
************************************************************************/
/*
 *  PSTree $B$O!"#2$D$N6/A4=g=x(B compareX, compareY$B!JG$0U$N(B p, q $B$KBP$7$F(B
 *  compareX(p, q), compareX(q, p), p == q $B$N$I$l$+#1$D$@$1$,@.$jN)$A!"(B
 *  $B$+$D?d0\N'$rK~$?$9!#$h$C$F!"(BcompareX(p, p) $B$O>o$K56!#!K$,M?$($i$l(B
 *  $B$?$H$-(B,
 *	1. $B?F%N!<%I(B _p $B$H;R%N!<%I(B _q $B$KBP$7$F(B !compareX(_q, _p)
 *	2. $B:8ItJ,LZ(B _l $B$H1&ItJ,LZ(B _r $B$KBP$7$F(B !compareY(_r, _l)
 *  $B$,K~$?$5$l$k$h$&$K9=@.$5$l$k!#?F%N!<%I$H;R%N!<%I$N4V$K$O(B compareY
 *  $B$K$D$$$F2?$i7h$^$C$?4X78$OL5$$!#(B
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
   *  compareX(_p, q) && compareY(_p, q) $B$rK~$?$9%N!<%I(B _p $B$N$&$A!"(B
   *  $BB>$N$I$N(B _p' $B$KBP$7$F$b(B !compareY(_p, _p') $B$H$J$k$b$N$rJV$9!#(B
   */
    const T*	closestY(const S& q)		 const	;
  /*
   *  compareX(_p, q) && compareY(_p, q) $B$rK~$?$9%N!<%I(B _p $BA4$F$r(B
   *  $BNs5s$7$F(B list $B$KJV$9!#(B
   */
    void	inRegion(const S& q, List& list) const	;
  /*
   *  !compareX(_p, p) && !compareY(_p, p) &&
   *   compareX(_p, q) &&  compareY(_p, q) $B$rK~$?$9%N!<%I(B _p $BA4$F$r(B
   *  $BNs5s$7$F(B list $B$KJV$9!#(B
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

template <class S, class T, class CompareX, class CompareY> inline int
PSTree<S, T, CompareX, CompareY>::Node::middle(int il, int ir)
{
    int	ib = (il + 1 + ir) / 2;
    return (ib < ir ? ib : ir - 1);
}
 
}
#endif	// !__TUPSTreePP_h
