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
 *  $Id: ShmDC.cc,v 1.6 2009-08-13 23:04:17 ueshiba Exp $  
 */
#include "TU/v/ShmDC.h"
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

namespace TU
{
namespace v
{
/************************************************************************
*  class ShmDC								*
************************************************************************/
ShmDC::ShmDC(CanvasPane& parentCanvasPane, u_int width, u_int height,
	     u_int mul, u_int div)
    :CanvasPaneDC(parentCanvasPane, width, height, mul, div),
     _xShmInfo(), _xShmSize(0), _xShmAvailable(true)
{
    int	ignore;
    if (!XQueryExtension(colormap().display(), "MIT-SHM",
			 &ignore, &ignore, &ignore))
    {
	std::cerr << "TU::v::ShmDC::ShmDC: MIT-SHM extension is unavailable!!"
		  << std::endl;
	_xShmAvailable = false;
    }
}

ShmDC::~ShmDC()
{
    if (_xShmSize != 0)
    {
	XShmDetach(colormap().display(), &_xShmInfo);
	shmdt(_xShmInfo.shmaddr);
	shmctl(_xShmInfo.shmid, IPC_RMID, 0);
    }
}

/*
 *  protected member functions
 */
void
ShmDC::allocateXImage(int buffWidth, int buffHeight)
{
    if (_xShmAvailable)
    {
	destroyShmImage();
	_ximage = XShmCreateImage(colormap().display(),
				  colormap().vinfo().visual,
				  colormap().vinfo().depth, ZPixmap, 0,
				  xShmInfo(), buffWidth, buffHeight);
	if (_ximage != 0)  // Succesfully allocated XImage ?
	{
	    _ximage->data = attachShm(_ximage->bytes_per_line*_ximage->height);
	    if (_ximage->data != 0)
		return;
	    XDestroyImage(_ximage);
	    _ximage = 0;
	}
    }
    XDC::allocateXImage(buffWidth, buffHeight);
}

void
ShmDC::putXImage() const
{
    if (_xShmSize != 0)
	XShmPutImage(colormap().display(), drawable(), gc(), _ximage,
		     0, 0, log2devR(offset()[0]), log2devR(offset()[1]),
		     _ximage->width, _ximage->height, False);
    else
	XDC::putXImage();
}

char*
ShmDC::attachShm(u_int size)
{
    if (_xShmSize != 0)
    {
      // Detach and remove shm previously allocated.
	XShmDetach(colormap().display(), &_xShmInfo);
	shmdt(_xShmInfo.shmaddr);
	shmctl(_xShmInfo.shmid, IPC_RMID, 0);
    }
    
    if (size != 0)
    {
      // Get new shm and attach it to the X server.
	_xShmInfo.shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0777);
	if (_xShmInfo.shmid != -1)  // Succesfully got shmid ? 
	{
	    _xShmSize = size;
	    _xShmInfo.shmseg = 0;
	    _xShmInfo.readOnly = False;
	    _xShmInfo.shmaddr = (char*)shmat(_xShmInfo.shmid, 0, 0);
	    if (_xShmInfo.shmaddr != (char*)-1)  // Succesfully got addr ?
	    {
		try
		{
		    XShmAttach(colormap().display(), &_xShmInfo);
		    XSync(colormap().display(), False);
		    return _xShmInfo.shmaddr;
		}
		catch (std::runtime_error& err)
		{
		    std::cerr << err.what() << std::endl;
		    shmdt(_xShmInfo.shmaddr);
		}
	    }
	    shmctl(_xShmInfo.shmid, IPC_RMID, 0);
	}
    }
    _xShmSize = 0;

    return 0;
}

void
ShmDC::destroyShmImage()
{
    if (_ximage != 0)
    {
	_ximage->data = 0;
	XDestroyImage(_ximage);
	_ximage = 0;
    }
}

}
}
