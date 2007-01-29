/*
 *  $Id: EdgeDetector.cc,v 1.6 2007-01-29 04:43:26 ueshiba Exp $
 */
#include "TU/Image++.h"
#ifdef __INTEL_COMPILER
#  include "TU/mmInstructions.h"
#endif

namespace TU
{
static const float	slant = 0.414214;	// tan(M_PI/8)
    
/************************************************************************
*  static functions							*
************************************************************************/
#ifdef SSE2
static inline mmInt	mmDir4(mmFlt eH, mmFlt eV)
			{
			    mmInt  l0 = mmCastToInt(mmCmpLEF(eH, eV)),
				   l1 = mmCastToInt(
					  mmCmpLEF(eH, mmSubF(mmZeroF(), eV)));
			    return mmOr(mmAnd(mmXor(l0, l1), mmSet32(0x2)),
					mmAnd(l1, mmSet32(0x4)));
			}
static inline mmInt	mmDir8(mmFlt eH, mmFlt eV)
			{
			    mmFlt  sH = mmMulF(mmSetF(slant), eH),
				   sV = mmMulF(mmSetF(slant), eV);
			    mmInt  l0 = mmCastToInt(mmCmpLEF(sH, eV)),
				   l1 = mmCastToInt(mmCmpLEF(eH, sV)),
				   l2 = mmCastToInt(
					  mmCmpLEF(eH, mmSubF(mmZeroF(), sV))),
				   l3 = mmCastToInt(
					  mmCmpLEF(sH, mmSubF(mmZeroF(), eV)));
			    return mmOr(mmOr(mmAnd(mmOr(mmXor(l0, l1),
							mmXor(l2, l3)),
						   mmSet32(0x1)),
					     mmAnd(mmXor(l1, l3),
						   mmSet32(0x2))),
					mmAnd(l3, mmSet32(0x4)));
			}
#endif

//! $B$"$k%(%C%8E@$H;XDj$5$l$?J}8~$N6aK5E@$,@\B3$7$F$$$k$+D4$Y$k(B
/*!
  \param edge	$B%(%C%82hA|(B
  \param p	$B%(%C%8E@(B
  \param dir	$B6aK5E@$NJ}8~(B
  \return	$B@\B3$7$F$$$l$P(Btrue$B!$$=$&$G$J$1$l$P(Bfalse
*/
static inline bool
isLink(const Image<u_char>& edge, const Point2<int>& p, int dir)
{
  // (1) $B6aK5E@$,>/$J$/$H$b6/(B/$B<e%(%C%8E@$G$"$j!$$+$D!$(B(2a) 4$B6aK5E@$G$"$k$+!$(B
  // (2b) $BN>NY$N6aK5E@$,6/(B/$B<e%(%C%8E@$G$J$$>l9g$K@\B3$7$F$$$k$HH=Dj$9$k!%(B
    return (edge(p.neighbor(dir)) &&
	    (!(dir & 0x1) ||
	     (!edge(p.neighbor(dir-1)) && !edge(p.neighbor(dir+1)))));
}
    
//! $B$"$k%(%C%8E@$r5/E@$K$7$F!$@\B3$9$k%(%C%8E@$rDI@W$9$k(B
/*!
  \param edge	$B%(%C%82hA|(B
  \param p	$B%(%C%8E@(B
*/
static void
trace(Image<u_char>& edge, const Point2<int>& p)
{
    u_char&	e = edge(p);		// $B$3$NE@(Bp$B$N2hAGCM(B
    
    if (e & EdgeDetector::TRACED)	// $B4{$K$3$NE@$,K,Ld:Q$_$J$i$P!$(B
	return;				// $BD>$A$KLa$k!%(B

    e |= (EdgeDetector::TRACED | EdgeDetector::EDGE);	// $BK,Ld:Q$_$+$D%(%C%8E@(B
    for (int dir = 0; dir < 8; ++dir)	// p$B$N(B8$B$D$N6aK5E@$=$l$>$l$K$D$$$F(B
	if (isLink(edge, p, dir))	// p$B$H@\B3$7$F$$$l$P(B
	    trace(edge, p.neighbor(dir));	// $B$5$i$KDI@W$rB3$1$k!%(B
}

//! $B$"$kE@$rBG$F$P(BEDGE$B%i%Y%k$,IU$$$F$$$kE@$H$=$&$G$J$$E@$r7k$Y$k$+D4$Y$k(B
/*!
  \param edge	$B%(%C%82hA|(B
  \param p	$BBG$H$&$H$9$kE@(B
  \return	$B7k$Y$k$N$G$"$l$P(Btrue$B!$$=$&$G$J$1$l$P(Bfalse
*/
static bool
canInterpolate(const Image<u_char>& edge, const Point2<int>& p)
{
    int	nedges = 0, nweaks = 0;
    
    for (int dir = 0; dir < 8; ++dir)	// p$B$N(B8$B$D$N6aK5E@$=$l$>$l$K$D$$$F(B
    {
	u_char	e = edge(p.neighbor(dir));
	
	if (e & EdgeDetector::EDGE)
	    ++nedges;			// EDGE$B%i%Y%k$,IU$$$F$$$kE@(B
	else if (e & EdgeDetector::WEAK)
	    ++nweaks;			// EDGE$B%i%Y%k$,IU$$$F$$$J$$<e$$%(%C%8E@(B
    }

  // p$B$N6aK5$K!$4{$K(BEDGE$B%i%Y%k$,IU$$$F$$$kE@$,>/$J$/$H$b(B1$B$D!$$*$h$S(B
  // $BIU$$$F$$$J$$<e$$%(%C%8E@$,(B1$B$D$@$1$"$l$P!$(Btrue$B$rJV$9!%(B
    return (nedges != 0 && nweaks == 1);
}
    
/************************************************************************
*  class EdgeDetector							*
************************************************************************/
//! $B%(%C%86/EY$r5a$a$k(B
/*!
  \param edgeH	$B2#J}8~(B1$B3,HyJ,F~NO2hA|(B
  \param edgeV	$B=DJ}8~(B1$B3,HyJ,F~NO2hA|(B
  \param out	$B%(%C%86/EY=PNO2hA|(B
  \return	$B$3$N%(%C%88!=P4o<+?H(B
*/
const EdgeDetector&
EdgeDetector::strength(const Image<float>& edgeH,
		       const Image<float>& edgeV, Image<float>& out) const
{
    out.resize(edgeH.height(), edgeH.width());
    for (int v = 0; v < out.height(); ++v)
    {
	const float		*eH = edgeH[v], *eV = edgeV[v];
	float*			dst = out[v];
	const float* const	end = dst + out.width();
#ifdef SSE
	for (const float* const end2 = dst + 4*(out.width()/4); dst < end2; )
	{
	    const mmFlt	fH = mmLoadFU(eH), fV = mmLoadFU(eV);
	    
	    mmStoreFU(dst, mmSqrtF(mmAddF(mmMulF(fH, fH), mmMulF(fV, fV))));
	    eH  += 4;
	    eV  += 4;
	    dst += 4;
	}
#endif
	while (dst < end)
	{
	    *dst++ = sqrtf(*eH * *eH + *eV * *eV);
	    ++eH;
	    ++eV;
	}
    }

    return *this;
}
    
//! 4$B6aK5$K$h$k%(%C%8J}8~$r5a$a$k(B
/*!
  \param edgeH	$B2#J}8~(B1$B3,HyJ,F~NO2hA|(B
  \param edgeV	$B=DJ}8~(B1$B3,HyJ,F~NO2hA|(B
  \param out	$B%(%C%8J}8~=PNO2hA|(B
  \return	$B$3$N%(%C%88!=P4o<+?H(B
*/
const EdgeDetector&
EdgeDetector::direction4(const Image<float>& edgeH,
			 const Image<float>& edgeV, Image<u_char>& out) const
{
    out.resize(edgeH.height(), edgeH.width());
    for (int v = 0; v < out.height(); ++v)
    {
	const float		*eH = edgeH[v], *eV = edgeV[v];
	u_char*			dst = out[v];
	const u_char* const	end = dst + out.width();
#ifdef SSE2
	for (const u_char* const end2 = dst + mmNBytes*(out.width()/mmNBytes);
	     dst < end2; dst += mmNBytes)
	{
	    const mmInt	d0 = mmDir4(mmLoadFU(eH), mmLoadFU(eV));
	    eH  += 4;
	    eV  += 4;
	    const mmInt	d1 = mmDir4(mmLoadFU(eH), mmLoadFU(eV));
	    eH  += 4;
	    eV  += 4;
	    const mmInt	d2 = mmDir4(mmLoadFU(eH), mmLoadFU(eV));
	    eH  += 4;
	    eV  += 4;
	    const mmInt	d3 = mmDir4(mmLoadFU(eH), mmLoadFU(eV));
	    eH  += 4;
	    eV  += 4;
	    mmStoreU((mmInt*)dst, mmPack(mmPack32(d0, d1), mmPack32(d2, d3)));
	}
#endif
	while (dst < end)
	{
	    *dst++ = (*eH <= *eV ? (*eH <= -*eV ? 4 : 2)
				 : (*eH <= -*eV ? 6 : 0));
	    ++eH;
	    ++eV;
	}
    }
    
    return *this;
}
    
//! 8$B6aK5$K$h$k%(%C%8J}8~$r5a$a$k(B
/*!
  \param edgeH	$B2#J}8~(B1$B3,HyJ,F~NO2hA|(B
  \param edgeV	$B=DJ}8~(B1$B3,HyJ,F~NO2hA|(B
  \param out	$B%(%C%8J}8~=PNO2hA|(B
  \return	$B$3$N%(%C%88!=P4o<+?H(B
*/
const EdgeDetector&
EdgeDetector::direction8(const Image<float>& edgeH,
			 const Image<float>& edgeV, Image<u_char>& out) const
{
    out.resize(edgeH.height(), edgeH.width());
    for (int v = 0; v < out.height(); ++v)
    {
	const float		*eH = edgeH[v], *eV = edgeV[v];
	u_char*			dst = out[v];
	const u_char* const	end = dst + out.width();
#ifdef SSE2
	for (const u_char* const end2 = dst + mmNBytes*(out.width()/mmNBytes);
	     dst < end2; dst += mmNBytes)
	{
	    const mmInt	d0 = mmDir8(mmLoadFU(eH), mmLoadFU(eV));
	    eH  += 4;
	    eV  += 4;
	    const mmInt	d1 = mmDir8(mmLoadFU(eH), mmLoadFU(eV));
	    eH  += 4;
	    eV  += 4;
	    const mmInt	d2 = mmDir8(mmLoadFU(eH), mmLoadFU(eV));
	    eH  += 4;
	    eV  += 4;
	    const mmInt	d3 = mmDir8(mmLoadFU(eH), mmLoadFU(eV));
	    eH  += 4;
	    eV  += 4;
	    mmStoreU((mmInt*)dst, mmPack(mmPack32(d0, d1), mmPack32(d2, d3)));
	}
#endif
	while (dst < end)
	{
	    const float	sH = slant * *eH, sV = slant * *eV;
	    
	    *dst++ = (sH <= *eV ?
		      (*eH <= sV ?
		       (*eH <= -sV ?
			(sH <= -*eV ? 4 : 3) : 2) : 1) :
		      (sH <= -*eV ?
		       (*eH <= -sV ?
			(*eH <=  sV ? 5 : 6) : 7) : 0));
	    ++eH;
	    ++eV;
	}
    }
    
    return *this;
}
    
//! $BHs6KBgCMM^@)=hM}$K$h$j:Y@~2=$r9T$&(B
/*!
  \param strength	$B%(%C%86/EYF~NO2hA|(B
  \param direction	$B%(%C%8J}8~F~NO2hA|(B
  \param out		$B6/$$%(%C%8E@$H<e$$%(%C%8E@$K$=$l$>$l(B#EDGE$B%i%Y%k$H(B
			#WEAK$B%i%Y%k$rIU$1$?=PNO2hA|(B
  \return		$B$3$N%(%C%88!=P4o<+?H(B
*/
const EdgeDetector&
EdgeDetector::suppressNonmaxima(const Image<float>& strength,
				const Image<u_char>& direction,
				Image<u_char>& out) const
{
    out.resize(strength.height(), strength.width());

  // $B=PNO2hA|$N30<~$r(B0$B$K$9$k!%(B
    if (out.height() > 0)
	for (int u = 0; u < out.width(); ++u)
	    out[0][u] = out[out.height()-1][u] = 0;
    if (out.width() > 0)
	for (int v = 0; v < out.height(); ++v)
	    out[v][0] = out[v][out.width()-1] = 0;

  // $B3FE@$N%(%C%86/EY$,(B (1) $B$=$NE@$N%(%C%8J}8~$K1h$C$?N>NY$HHf3S$7$F6KBg$K(B
  // $B$J$C$F$*$j!$$+$D!$(B(2a) $B6/$$ogCM0J>e$J$i$P(BEDGE$B%i%Y%k$r!$(B(2b) $B<e$$ogCM(B
  // $B0J>e$J$i$P(BWEAK$B%i%Y%k$r$=$l$>$l=q$-9~$`!%$=$&$G$J$1$l$P(B0$B$r=q$-9~$`!%(B
    for (int v = 0; ++v < out.height() - 1; )
    {
	const float		*prv = strength[v-1],
				*str = strength[v],
				*nxt = strength[v+1];
	const u_char		*dir = direction[v];
	const u_char* const	end  = &out[v][out.width() - 1];
	for (u_char* dst = out[v]; ++dst < end; )
	{
	    ++prv;
	    ++str;
	    ++nxt;
	    ++dir;
	    
	    if (*str >= _th_low)	// $B<e$$ogCM0J>e$J$i(B
		switch (*dir)		// $B%(%C%8J}8~$r8+$k!%(B
		{
		  case 0:
		  case 4:
		    *dst = (*str > *(str-1) && *str > *(str+1) ?
			    (*str >= _th_high ? EDGE : WEAK) : 0);
		    break;
		  case 1:
		  case 5:
		    *dst = (*str > *(prv-1) && *str > *(nxt+1) ?
			    (*str >= _th_high ? EDGE : WEAK) : 0);
		    break;
		  case 2:
		  case 6:
		    *dst = (*str > *prv && *str > *nxt ?
			    (*str >= _th_high ? EDGE : WEAK) : 0);
		    break;
		  default:
		    *dst = (*str > *(prv+1) && *str > *(nxt-1) ?
			    (*str >= _th_high ? EDGE : WEAK) : 0);
		    break;
		}
	    else
		*dst = 0;		// $B<e$$ogCML$K~$J$i(B 0
	}
    }

    return *this;
}

//! 2$B<!HyJ,2hA|$N%<%m8r:9E@$r8!=P$9$k(B
/*!
  \param in		$BF~NO(B2$B<!HyJ,2hA|(B
  \param out		$B%<%m8r:9E@$r(B255$B!$$=$&$G$J$$E@$r(B0$B$H$7$?=PNO2hA|(B
  \return		$B$3$N%(%C%88!=P4o<+?H(B
*/
const EdgeDetector&
EdgeDetector::zeroCrossing(const Image<float>& in, Image<u_char>& out) const
{
    out.resize(in.height(), in.width());

  // $B=PNO2hA|$N2<C<$H1&C<$r(B0$B$K$9$k!%(B
    if (out.height() > 0)
	for (int u = 0; u < out.width(); ++u)
	    out[out.height()-1][u] = 0;
    if (out.width() > 0)
	for (int v = 0; v < out.height(); ++v)
	    out[v][out.width()-1] = 0;

  // $B8=:_E@$r:8>e6y$H$9$k(B2x2$B%&%#%s%I%&Cf$N2hAG$,0[Id9f$J$i%(%C%8E@$H$9$k!%(B
    for (int v = 0; v < out.height() - 1; ++v)
    {
	const float		*cur = in[v],
				*nxt = in[v+1];
	const u_char* const	end  = &out[v][out.width() - 1];
	for (u_char* dst = out[v]; dst < end; )
	{
	    if ((*cur >= 0.0 && *(cur+1) >= 0.0 &&
		 *nxt >= 0.0 && *(nxt+1) >= 0.0) ||
		(*cur <= 0.0 && *(cur+1) <= 0.0 &&
		 *nxt <= 0.0 && *(nxt+1) <= 0.0))
		*dst++ = 0;
	    else
		*dst++ = 255;
	    ++cur;
	    ++nxt;
	}
    }

    return *this;
}
    
//! 2$B<!HyJ,2hA|$N%<%m8r:9E@$r8!=P$7!$%(%C%86/EY$K$h$C$FJ,N`$9$k(B
/*!
  \param in		$BF~NO(B2$B<!HyJ,2hA|(B
  \param strength	$BF~NO%(%C%86/EY2hA|(B
  \param out		$B6/$$%(%C%8E@$H<e$$%(%C%8E@$K$=$l$>$l(B#EDGE$B%i%Y%k$H(B
			#WEAK$B%i%Y%k$rIU$1$?=PNO2hA|(B
  \return		$B$3$N%(%C%88!=P4o<+?H(B
*/
const EdgeDetector&
EdgeDetector::zeroCrossing(const Image<float>& in, const Image<float>& strength,
			   Image<u_char>& out) const
{
    out.resize(in.height(), in.width());

  // $B=PNO2hA|$N30<~$r(B0$B$K$9$k!%(B
    if (out.height() > 0)
	for (int u = 0; u < out.width(); ++u)
	    out[0][u] = out[out.height()-1][u] = 0;
    if (out.width() > 0)
	for (int v = 0; v < out.height(); ++v)
	    out[v][0] = out[v][out.width()-1] = 0;

  // $B8=:_E@$r:8>e6y$H$9$k(B2x2$B%&%#%s%I%&Cf$N2hAG$,0[Id9f$J$i%(%C%8E@$H$9$k!%(B
    for (int v = 0; ++v < out.height() - 1; )
    {
	const float		*cur = in[v],
				*nxt = in[v+1],
				*str = strength[v];
	const u_char* const	end  = &out[v][out.width() - 1];
	for (u_char* dst = out[v]; ++dst < end; )
	{
	    ++cur;
	    ++nxt;
	    ++str;

	    if ((*str < _th_low) ||
		(*cur >= 0.0 && *(cur+1) >= 0.0 &&
		 *nxt >= 0.0 && *(nxt+1) >= 0.0) ||
		(*cur <= 0.0 && *(cur+1) <= 0.0 &&
		 *nxt <= 0.0 && *(nxt+1) <= 0.0))
		*dst = 0;
	    else
		*dst = (*str >= _th_high ? EDGE : WEAK);
	}
    }

    return *this;
}

//! $B6/$$%(%C%8E@$r5/E@$K<e$$%(%C%8$rDI@W$9$k$3$H$K$h$j%R%9%F%j%7%9ogCM=hM}$r9T$&(B
/*!
  \param edge		$B6/$$%(%C%8E@$H<e$$%(%C%8E@$K$=$l$>$l(B#EDGE$B%i%Y%k$H(B
			#WEAK$B%i%Y%k$rIU$1$?2hA|!%=hM}$,=*$o$k$H:G=*E*$J%(%C%8(B
			$BE@$K(B255$B$r!$$=$&$G$J$$E@$K$O(B0$B$r=q$-9~$s$GJV$5$l$k!%(B
  \return		$B$3$N%(%C%88!=P4o<+?H(B
*/
const EdgeDetector&
EdgeDetector::hysteresisThresholding(Image<u_char>& edge) const
{
    
  // $B6/$$%(%C%8E@$r5/E@$K$7$F!$@\B3$9$k<e$$%(%C%8E@$rDI@W$7(BEDGE$B%i%Y%k$rIU$1$k!%(B
    for (int v = 0; ++v < edge.height() - 1; )
	for (int u = 0; ++u < edge.width() - 1; )
	    if (edge[v][u] & EDGE)
		trace(edge, Point2<int>(u, v));

  // EDGE$B%i%Y%k$,IU$$$F$*$i$:!$$+$DIU$$$F$$$kE@$HIU$$$F$$$J$$<e$$%(%C%8E@$N(B
  // $B66EO$7$K$J$l$kE@$K?7$?$K(BEDGE$B%i%Y%k$rIU$1$FDI@W$r9T$&!%(B
    for (int v = 0; ++v < edge.height() - 1; )
	for (int u = 0; ++u < edge.width() - 1; )
	{
	    Point2<int>	p(u, v);
	    
	    if (!(edge(p) & EDGE) && canInterpolate(edge, p))
		trace(edge, p);
	}

  // EDGE$BE@$K$O(B255$B$r!$$=$&$G$J$$E@$K$O(B0$B$r=q$-9~$`!%(B
    for (int v = 0; v < edge.height(); )
    {
	u_char*	dst = edge[v++];
	for (const u_char* const end = dst + edge.width(); dst < end; ++dst)
	    *dst = (*dst & EDGE ? 255 : 0);
    }

    return *this;
}
    
}
