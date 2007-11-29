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
 *  $Id: FileSelection.cc,v 1.7 2007-11-29 07:06:07 ueshiba Exp $
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <algorithm>
#include "TU/v/FileSelection.h"
#include "TU/v/Notify.h"
#include "TU/v/Confirm.h"

namespace TU
{
namespace v
{
/************************************************************************
*  static data								*
************************************************************************/
enum	{c_Directory, c_FileList, c_Cancel, c_FileName};

static CmdDef Cmds[] =
{
    {C_Label,  c_Directory,0, "",	noProp, CA_NoBorder, 0, 0, 2, 1, 0},
    {C_List,   c_FileList, 0, "",	noProp, CA_None, 0, 1, 1, 1, 15},
    {C_Button, c_Cancel,   0, "Cancel", noProp, CA_None, 1, 1, 1, 1, 0},
    {C_TextIn, c_FileName, 0, "",	noProp, CA_None, 0, 2, 1, 1, 0},
    EndOfCmds
};

/************************************************************************
*  static functions							*
************************************************************************/
static mode_t
fileMode(const std::string& filename)
{
    struct stat	statbuf;
    if (::stat(filename.c_str(), &statbuf))	// error ?
	return 0;
    else
	return statbuf.st_mode;
}

/************************************************************************
*  class FileSelection							*
************************************************************************/
FileSelection::FileSelection(Window& parentWindow)
    :ModalDialog(parentWindow, "File selection", Cmds),
     _fullname(), _dirname(), _filenames(1)
{
  // Initialize _dirname to current working directory.
    char	s[1024];
    ::getcwd(s, sizeof(s)/sizeof(s[0]));
    _dirname = s;
    if (_dirname[_dirname.length()-1] != '/')
	_dirname += '/';

  // Set file names in current working directory to _filenames.
    _filenames[0] = 0;
    changeDirectory(_dirname);
}

FileSelection::~FileSelection()
{
    for (int i = 0; _filenames[i] != 0; ++i)
	delete [] _filenames[i];
}

bool
FileSelection::open(std::ifstream& in)
{
    for (;;)
    {
	show();
	if (_fullname.empty())		// $B%U%!%$%kL>$,A*Br$5$l$F$$$J$$!)(B
	    return false;
	in.open(_fullname.c_str());
	if (in)				// $B@5>o$K%*!<%W%s$5$l$?(B?
	    break;
	Notify	notify(*this);
	notify << "Cannot open " << _fullname << ": " << strerror(errno);
	notify.show();
    }
    return true;
}

bool
FileSelection::open(std::ofstream& out)
{
    for (;;)
    {
	show();
	if (_fullname.empty())		// $B%U%!%$%kL>$,A*Br$5$l$F$$$J$$!)(B
	    return false;
	if (fileMode(_fullname))	// $B4{B8%U%!%$%k!)(B
	{
	    Confirm	confirm(*this);
	    confirm << _fullname << " already exists. Override?";
	    if (!confirm.ok())
		continue;
	}
	out.open(_fullname.c_str());
	if (out)			// $B@5>o$K%*!<%W%s$5$l$?!)(B
	    break;
	Notify	notify(*this);
	notify << "Cannot open " << _fullname << ": " << strerror(errno);
	notify.show();
    }
    return true;
}

void
FileSelection::callback(CmdId id, CmdVal val)
{
    switch (id)
    {
      case c_Cancel:
	_fullname.erase();
	hide();
	break;
	
      case c_FileList:
      case c_FileName:
      {
	const std::string&
		fullname = fullPathName(id == c_FileList ? _filenames[val] :
					pane().getString(id));
	mode_t	filemode = fileMode(fullname);
	if (id == c_FileList && filemode & S_IFDIR)	   // directory ?
	    changeDirectory(fullname);
	else if ((filemode & S_IFREG) || (filemode == 0))  // normal/new file?
	{
	    _fullname = fullname;
	    hide();
	}
      }
	break;

	
    }
}

// $B;XDj$7$?(Bdirectory$B$K0\F0$7!$$=$NCf$N%U%!%$%k$r(B_filenames$B$K%;%C%H$9$k!%(B
/*!
  \param dirname	$B0\F0@h$N(Bdirectory$BL>!%KvHx$O(B'\'$B$G$J$1$l$P$J$i$J$$!%(B
*/
void
FileSelection::changeDirectory(const std::string& dirname)
{
  // Get file names in the new working directory.
    DIR*	dirp = ::opendir(dirname.c_str());
    if (dirp == NULL)
    {
	std::cerr << "Failed to open direcotry [" << dirname << "]: "
		  << strerror(errno) << std::endl;
	return;
    }
    getFileNames(dirp, 0);
    ::closedir(dirp);

  // Sort file names.
    std::sort(&_filenames[0], &_filenames[_filenames.dim() - 1], cmp());

  // Append '/' to directory names.
    for (int i = 0; _filenames[i] != 0; ++i)
	if (fileMode(dirname + _filenames[i]) & S_IFDIR)
	{
	    int	len = strlen(_filenames[i]);
	    _filenames[i][len] = '/';
	    _filenames[i][len+1] = '\0';
	}

  // Set file names in scrolling list.
    pane().setProp(c_FileList, (char**)_filenames);

  // Change directory.
    _dirname = dirname;
    pane().setString(c_Directory, _dirname.c_str());
}

void
FileSelection::getFileNames(DIR* dirp, int n)
{
    const dirent* dp = ::readdir(dirp);
    if (dp == NULL)
    {
	for (int i = 0; _filenames[i] != 0; ++i)
	    delete [] _filenames[i];
	_filenames.resize(n+1);
	_filenames[n] = 0;
    }
    else
    {
	char* const	name = new char[strlen(dp->d_name) + 2];
	strcpy(name, dp->d_name);
	getFileNames(dirp, n+1);
	_filenames[n] = name;
    }
}

std::string
FileSelection::fullPathName(const char* name) const
{
    if (!strcmp(name, "./"))
	return _dirname;
    else if (!strcmp(name, "../"))
    {  // _dirname$B$NKvHx$,(B'/'$B$G=*$o$C$F$$$k$H2>Dj$7$F$$$k!%(B
	std::string	fullname = _dirname;
	fullname.erase(fullname.rfind('/'));	// $BKvHx$N(B'/'$B$r=|5n!%(B
	std::string::size_type	slash = fullname.rfind('/');
	if (slash != std::string::npos)
	    fullname.erase(slash + 1);		// $B:G8e$N(B'/'$B0J9_$r=|5n!%(B
	else
	    fullname = '/';	// $BKvHx0J30$K(B'/'$B$,;D$i$J$+$C$?$i(Broot$B!%(B
	return fullname;
    }
    else
    {  // _dirname$B$NKvHx$,(B'/'$B$G=*$o$C$F$$$k$H2>Dj$7$F$$$k!%(B
	std::string	fullname = _dirname + name;;
	return fullname;
    }
}

}
}
