/*
 *  $Id: MemoryDC.cc,v 1.1 2004-09-27 08:32:00 ueshiba Exp $
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
