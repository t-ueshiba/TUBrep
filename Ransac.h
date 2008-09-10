/*
 *  $BJ?@.(B14-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n8"=jM-(B
 *  $B<T$K$h$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$k(B
 *  $BEy$N9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
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
 *  $Id: Ransac.h,v 1.3 2008-09-10 05:10:46 ueshiba Exp $
 */
#ifndef __TURansac_h
#define __TURansac_h

#include <math.h>
#include <stdexcept>
#include "TU/types.h"

namespace TU
{
/************************************************************************
*  function ransac							*
************************************************************************/
//! RANSAC$B$K$h$C$F(Boutlier$B$r4^$`E@=89g$K%b%G%k$rEv$F$O$a$k!%(B
/*!
  $B%F%s%W%l!<%H%Q%i%a!<%?(BPointSet$B$OE@=89g$rI=$9%/%i%9$G$"$j!$0J2<$N>r7o$r(B
  $BK~$?$9$3$H!'(B
  \verbatim
  1. forward_iterator$B$r%5%]!<%H$9$k%3%s%F%J$G$"$k!%(B
  2. $B$3$N%3%s%F%J$N7?$r(BPointSet::Container$B$H$$$&L>A0$G(Btypedef$B$7$F$$$k!%(B
  3. inlier$B$N3d9g$r%a%s%P4X?t(B
	double	Pointset::inlierRate() const;
     $B$K$h$C$FCN$k$3$H$,$G$-$k!%(B
  4. $B%a%s%P4X?t(B
	PoinstSet::Container	Pointset::sample(u_int npoints) const;
     $B$K$h$C$F%i%s%@%`$K(Bnpoints$B8D$NItJ,=89g$r<h$j=P$;$k!%(B
  \endverbatim
  $B%F%s%W%l!<%H%Q%i%a!<%?(BModel$B$OEv$F$O$a$k$Y$-%b%G%k$rI=$9%/%i%9$G$"$j!$(B
  $B0J2<$N>r7o$rK~$?$9$3$H!'(B
  \verbatim
  1. $B%a%s%P4X?t(B
	template <class Iterator>
	void	Model::fit(Iterator first, Iterator last);
     $B$K$h$C$FE@=89g$K%b%G%k$rEv$F$O$a$k$3$H$,$G$-$k!%(B
  2. 1.$B$KI,MW$J:G>/E@?t$r%a%s%P4X?t(B
	u_int	Model::ndataMin() const;
     $B$K$h$C$FCN$k$3$H$,$G$-$k!%(B
  \endverbatim
  $B%F%s%W%l!<%H%Q%i%a!<%?(BConform$B$OE@$N%b%G%k$X$NE,9g@-$rH=Dj$9$k4X?t(B
  $B%*%V%8%'%/%H$G$"$j!$(B
  \verbatim
	bool	Conform::operator()(const PointSet::Container::valu_type& p,
				    const Model& model);
  \endverbatim
  $B$J$k%$%s%?%U%'!<%9$K$h$C$F(Bp$B$,(Bmodel$B$KE,9g$7$F$$$k$+H=Dj$G$-$k$3$H!%(B
  \param pointSet	inlier$B$H(Boutlier$B$r4^$`E@=89g(B
  \param model		pointSet$B$K4^$^$l$k(Binlier$B$rEv$F$O$a$k%b%G%k!%(B
  \param hitRate	RANSAC$B$K$h$C$F@5$7$/(Binlier$B$r0z$-Ev$F$k3NN(!%(B
			0 <= hitRate < 1$B$G$J$1$l$P$J$i$J$$!%(B
  \return		pointSet$B$K4^$^$l$k(Binlier
*/
template <class PointSet, class Model, class Conform>
typename PointSet::Container
ransac(const PointSet& pointSet, Model& model, Conform conform,
       double hitRate=0.99)
{
    typedef typename PointSet::Container	Container;
    
  // $BM?$($i$l$?(BhitRate$B!$(BPointSet$B$K4^$^$l$k(Binlier$B$N3d9g$*$h$S(BModel$B$N@8@.$K(B
  // $BMW$9$k:G>/E@?t$+$i!$%5%s%W%j%s%0$NI,MW2s?t$r5a$a$k!%(B
    if (hitRate < 0.0 || hitRate >= 1.0)
	throw std::invalid_argument("ransac<PointSet, Model>: given hit rate is not within [0, 1)!!");
    const double	inlierRate = pointSet.inlierRate();
    if (inlierRate < 0.0 || inlierRate >= 1.0)
	throw std::invalid_argument("ransac<PointSet, Model>: inlier rate is not within [0, 1)!!");
    double	tmp = 1.0;
    for (u_int n = model.ndataMin(); n-- > 0; )
	tmp *= inlierRate;
    const u_int	ntrials = u_int(ceil(log(1.0 - hitRate) / log(1.0 - tmp)));

  // $B;n9T!J:G>.8D?t$NE@$r%5%s%W%k!$%b%G%k@8@.!$(Binlier$B8!=P!K$r(Bntrials$B2s9T$&!%(B
    Container	inlierSetA, inlierSetB;
    Container	*inliers = &inlierSetA, *inliersMax = &inlierSetB;
    for (int n = 0; n < ntrials; ++n)
    {
      // $BE@=89g$+$i%b%G%k$N7W;;$KI,MW$J:G>.8D?t$NE@$r%5%s%W%k$9$k!%(B
	const Container&	minimalSet = pointSet.sample(model.ndataMin());

      // $B%5%s%W%k$7$?E@$+$i%b%G%k$r@8@.$9$k!%(B
	model.fit(minimalSet.begin(), minimalSet.end());

      // $BA4E@$NCf$G@8@.$7$?%b%G%k$KE,9g$9$k(B(inlier)$B$b$N$r=8$a$k!%(B
	inliers->clear();
	for (typename PointSet::const_iterator iter = pointSet.begin();
	     iter != pointSet.end(); ++iter)
	    if (conform(*iter, model))
		inliers->push_back(*iter);

      // $B$3$l$^$G$N$I$N%b%G%k$h$j$b(Binlier$B$N?t$,B?$1$l$P$=$N=89g$r5-O?$9$k!%(B
	if (inliers->size() > inliersMax->size())
	    std::swap(inliers, inliersMax);
    }
  // $B:GBg=89g$K4^$^$l$kE@$r??$N(Binlier$B$H$7!$$=$l$iA4$F$+$i%b%G%k$r@8@.$9$k!%(B
    model.fit(inliersMax->begin(), inliersMax->end());

    return *inliersMax;
}
    
}

#endif // !__TURansac_h
