/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$kEy$NCx(B
 *  $B:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: MemoryDC.cc,v 1.3 2007-11-29 07:06:07 ueshiba Exp $
 */
#include "TU/v/MemoryDC.h"

namespace TU
{
namespace v
{
/************************************************************************
*  class MemoryDC						*
************************************************************************/
/*
 *  Public member functions
 */
MemoryDC::MemoryDC(Colormap& cmap, u_int w, u_int h)
    :XDC(w, h, cmap, XDefaultGC(cmap.display(), cmap.vinfo().screen)),
     _pixmap(XCreatePixmap(colormap().display(),
			   DefaultRootWindow(colormap().display()), w, h,
			   colormap().vinfo().depth))
{
}

MemoryDC::~MemoryDC()
{
    XFreePixmap(colormap().display(), _pixmap);
}

DC&
MemoryDC::setSize(u_int width, u_int height, u_int mul, u_int div)
{
    XDC::setSize(width, height, mul, div);
  // Viewport $B$NCf$G$3$N(B widget $B$r>.$5$/$9$k$H$-(B, $B0JA0IA2h$7$?$b$N$N;D(B
  // $B3<$,M>Gr$K;D$k$N$O8+6l$7$$$N$G!"(Bwidget $BA4BN$r%/%j%"$7$F$*$/!#$^$?!"(B
  // $BD>@\(B graphic hardware $B$K%"%/%;%9$9$k(B API $B!J(BXIL $B$J$I!K$H<B9T=g=x$,(B
  // $BF~$lBX$o$k$3$H$rKI$0$?$a!"(BXSync() $B$r8F$V!J(BXDC.cc $B;2>H!K!#(B
    XClearWindow(colormap().display(), drawable());
    XSync(colormap().display(), False);
    return *this;
}

/*
 *  Protected member functions
 */
Drawable
MemoryDC::drawable() const
{
    return _pixmap;
}

void
MemoryDC::initializeGraphics()
{
}

DC&
MemoryDC::repaintUnderlay()
{
    return *this;
}

DC&
MemoryDC::repaintOverlay()
{
    return *this;
}

/*
 *  Private member functions
 */

}
}
