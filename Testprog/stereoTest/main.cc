/*
 *  $Id: main.cc 1246 2012-11-30 06:23:09Z ueshiba $
 */
#include <unistd.h>
#include <algorithm>
#include <limits>
#include "TU/io.h"
#include "TU/Rectify.h"
#include "TU/SADStereo.h"
#include "TU/GFStereo.h"
#include "TU/Profiler.h"

#define DEFAULT_PARAM_FILE	"stereo"
#define DEFAULT_CONFIG_DIRS	".:/usr/local/etc/cameras"
#define DEFAULT_SCALE		1.0
#define DEFAULT_GRAINSIZE	50

namespace TU
{
/************************************************************************
*  static functions							*
************************************************************************/
template <class T> static void
scaleDisparity(Image<T>& disparityMap, u_int disparitySearchWidth)
{
    const float	k = 255.0 / float(disparitySearchWidth);
    for (int v = 0; v < disparityMap.height(); ++v)
    {
	T*	p = disparityMap[v].data();
	for (int u = 0; u < disparityMap.width(); ++u)
	    *p++ *= k;
    }
}

template <class STEREO, class T> static void
doJob(std::istream& in, const typename STEREO::Parameters& params,
      double scale, bool binocular)
{
    using namespace	std;
    
  // $B%9%F%l%*%^%C%A%s%0%Q%i%a!<%?$r@_Dj!%(B
    cerr << "--- Stereo matching parameters ---\n";
    params.put(cerr);

  // $B2hA|$rFI$_9~$`!%(B
    Image<T>		images[3];
    for (int i = 0; i < 3; ++i)
	if (!images[i].restore(cin))
	{
	    if (i < 2)
		throw runtime_error("Need two or more images!!");
	    else if (i == 2)
		binocular = true;
	    break;
	}

  // $B2hA|$rJ?9T2=$9$k!%(B
    Rectify		rectify;
    STEREO		stereo(params);
    Image<T>		rectifiedImages[3];
    if (binocular)
	rectify.initialize(images[0], images[1],
			   scale,
			   stereo.getParameters().disparitySearchWidth,
			   stereo.getParameters().disparityMax);
	
    else
	rectify.initialize(images[0], images[1], images[2],
			   scale,
			   stereo.getParameters().disparitySearchWidth,
			   stereo.getParameters().disparityMax);

  // $B%9%F%l%*%^%C%A%s%0$r9T$&!%(B
    Profiler		profiler(2);
    Image<float>	disparityMap(rectify.width(0), rectify.height(0));
    cerr << "Disparity map: "
	 << disparityMap.width() << 'x' << disparityMap.height() << endl;
    
    if (binocular)    
    {
	rectify(images[0], images[1],
		rectifiedImages[0], rectifiedImages[1]);
	stereo(rectifiedImages[0].cbegin(), rectifiedImages[0].cend(),
	       rectifiedImages[1].cbegin(), disparityMap.begin());
    }
    else
    {
	rectify(images[0], images[1], images[2],
		rectifiedImages[0], rectifiedImages[1], rectifiedImages[2]);
	stereo(rectifiedImages[0].cbegin(), rectifiedImages[0].cend(),
	       rectifiedImages[0].cend(), rectifiedImages[1].cbegin(),
	       rectifiedImages[2].cbegin(), disparityMap.begin());
    }

#if defined(NO_INTERPOLATION)
    scaleDisparity(disparityMap, params.disparitySearchWidth);
    disparityMap.save(cout, ImageBase::U_CHAR);
#else
    disparityMap.save(cout, ImageBase::FLOAT);
#endif
}

}
/************************************************************************
*  global functions							*
************************************************************************/
int
main(int argc, char* argv[])
{
    using namespace	std;
    using namespace	TU;

#if defined(HUGE_IMAGE)
    typedef SADStereo<int,  u_short>	SADStereoType;
    typedef GFStereo<float, u_short>	GFStereoType;
#else    
    typedef SADStereo<short, u_char>	SADStereoType;
    typedef GFStereo<float,  u_char>	GFStereoType;
#endif

    bool	gfstereo		= false;
    bool	doHorizontalBackMatch	= true;
    bool	doVerticalBackMatch	= true;
    string	paramFile		= DEFAULT_PARAM_FILE;
    string	configDirs		= DEFAULT_CONFIG_DIRS;
    double	scale			= DEFAULT_SCALE;
    bool	binocular		= false;
    size_t	windowSize		= 0;
    size_t	disparitySearchWidth	= 0;
    size_t	disparityMax		= 0;
    size_t	grainSize		= DEFAULT_GRAINSIZE;
    
  // $B%3%^%s%I9T$N2r@O!%(B
    extern char*	optarg;
    for (int c; (c = getopt(argc, argv, "GHVp:d:s:BW:D:M:g:")) != EOF; )
	switch (c)
	{
	  case 'G':
	    gfstereo = true;
	    break;
	  case 'H':
	    doHorizontalBackMatch = false;
	    break;
	  case 'V':
	    doVerticalBackMatch = false;
	    break;
	  case 'p':
	    paramFile = optarg;
	    break;
	  case 'd':
	    configDirs = optarg;
	    break;
	  case 's':
	    scale = atof(optarg);
	    break;
	  case 'B':
	    binocular = true;
	    break;
	  case 'W':
	    windowSize = atoi(optarg);
	    break;
	  case 'D':
	    disparitySearchWidth = atoi(optarg);
	    break;
	  case 'M':
	    disparityMax = atoi(optarg);
	    break;
	  case 'g':
	    grainSize = atoi(optarg);
	    break;
	}
    
  // $BK\Ev$N$*;E;v!%(B
    try
    {
	ifstream	in;
	openFile(in, paramFile, configDirs, ".params");

	if (gfstereo)
	{
	    GFStereoType::Parameters	params;
	    params.get(in);
	    
	    if (windowSize != 0)
		params.windowSize = windowSize;
	    if (disparityMax != 0)
		params.disparityMax = disparityMax;
	    if (disparitySearchWidth != 0)
		params.disparitySearchWidth = disparitySearchWidth;
	    params.doHorizontalBackMatch = doHorizontalBackMatch;
	    params.doVerticalBackMatch	 = doVerticalBackMatch;
	    params.grainSize		 = grainSize;

	    doJob<GFStereoType, u_char>(in, params, scale, binocular);
	}
	else
	{
	    SADStereoType::Parameters	params;
	    params.get(in);
	    
	    if (windowSize != 0)
		params.windowSize = windowSize;
	    if (disparityMax != 0)
		params.disparityMax = disparityMax;
	    if (disparitySearchWidth != 0)
		params.disparitySearchWidth = disparitySearchWidth;
	    params.doHorizontalBackMatch = doHorizontalBackMatch;
	    params.doVerticalBackMatch	 = doVerticalBackMatch;
	    params.grainSize		 = grainSize;

	    doJob<SADStereoType, u_char>(in, params, scale, binocular);
	}
    }
    catch (exception& err)
    {
	cerr << err.what() << endl;
	return 1;
    }

    return 0;
}
