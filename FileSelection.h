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
 *  $Id: FileSelection.h,v 1.4 2007-11-29 07:06:07 ueshiba Exp $
 */
#ifndef __TUvFileSelection_h
#define __TUvFileSelection_h

#include <fstream>
#include <string>
#include <dirent.h>
#include "TU/v/ModalDialog.h"

namespace TU
{
namespace v
{
/************************************************************************
*  class FileSelection							*
************************************************************************/
class FileSelection : public ModalDialog
{
  public:
    FileSelection(Window& parentWindow)					;
    virtual		~FileSelection()				;

    bool		open(std::ifstream& in)				;
    bool		open(std::ofstream& out)			;

    virtual void	callback(CmdId id, CmdVal val)			;

  private:
    struct cmp
    {
	bool	operator ()(const char* a, const char* b)
						{return ::strcmp(a, b) < 0;}
    };
    
    void		changeDirectory(const std::string& dirname)	;
    void		getFileNames(DIR* dirp, int n)			;
    std::string		fullPathName(const char* filename)	const	;

    std::string		_fullname;  // fullpath file name currently selected.
    std::string		_dirname;   // directory name currently browsing.
    Array<char*>	_filenames; // file names under _dirname.
};

}
}
#endif // !__TUvFileSelection_h
