/*
 *  $Id: main.cc 1246 2012-11-30 06:23:09Z ueshiba $
 */
#include <cstdlib>
#include "TU/algorithm.h"
#include "TU/io.h"
#include "TU/Rectify.h"
#include "TU/GuidedFilter.h"
#include "TU/TreeFilter.h"
#include "TU/StereoUtility.h"

#define DEFAULT_PARAM_FILE	"stereo"
#define DEFAULT_CONFIG_DIRS	".:/usr/local/etc/cameras"
#define DEFAULT_SCALE		1.0
#define DEFAULT_GRAINSIZE	50

//#define SCORE_ARRAY2

namespace TU
{
enum Algorithm	{SAD, GF, WMF, TF};
    
/************************************************************************
*  class MyDiff<S, T>							*
************************************************************************/
template <class S, class T>
struct MyDiff
{
    typedef S	argument_type;
    typedef T	result_type;

    result_type	operator ()(argument_type x, argument_type y) const
		{
		    return std::abs(x - y);
		}
};

/************************************************************************
*  static functions							*
************************************************************************/
template <class S, class T> static void
doJob(const Image<T>& imageL, const Image<T>& imageR,
      const StereoParameters& params, double scale, Algorithm algo)
{
    using	disparity_type	= float;
    
  // $B2hA|$rJ?9T2=$9$k!%(B
    Rectify		rectify;
    Image<T>		rectifiedImageL, rectifiedImageR;
    rectify.initialize(imageL, imageR, scale,
		       params.disparitySearchWidth, params.disparityMax);
    rectify(imageL, imageR, rectifiedImageL, rectifiedImageR);

    std::cerr << "Left image:  "
	      << rectifiedImageL.height() << "(H)x"
	      << rectifiedImageL.width()  << "(W)"
	      << std::endl;
    std::cerr << "Right image: "
	      << rectifiedImageR.height() << "(H)x"
	      << rectifiedImageR.width()  << "(W)"
	      << std::endl;
    
  // $B%9%F%l%*%^%C%A%s%0$r9T$&!%(B
    Image<disparity_type>	disparityMap(rectify.width(0),
					     rectify.height(0));
    const auto			rowL  = TU::cbegin(rectifiedImageL);
    const auto			rowR  = TU::cbegin(rectifiedImageR);
    const auto			rowLe = TU::cend(rectifiedImageL);
    const auto			rowRe = TU::cend(rectifiedImageR);
    const auto			rowD  = TU::begin(disparityMap);
#ifdef SCORE_ARRAY2
    Array2<Array<S> >		scores(disparityMap.height(),
				       disparityMap.width());
#endif

    switch (algo)
    {
      case SAD:
      {
	BoxFilter2	filter(params.windowSize, params.windowSize);

	filter.convolve(make_range_iterator(
			    make_diff_iterator<S>(TU::cbegin(*rowL),
						  TU::cbegin(*rowR),
						  params.disparitySearchWidth,
						  params.intensityDiffMax),
			    std::make_tuple(stride(rowL), stride(rowR)),
			    TU::size(*rowL)),
			make_range_iterator(
			    make_diff_iterator<S>(TU::cbegin(*rowLe),
						  TU::cbegin(*rowRe),
						  params.disparitySearchWidth,
						  params.intensityDiffMax),
			    std::make_tuple(stride(rowLe), stride(rowRe)),
			    TU::size(*rowLe)),
#ifdef SCORE_ARRAY2
			scores.begin());
#else
			make_range_iterator(
			    make_matching_iterator<S>(
				TU::begin(*(rowD + params.windowSize/2))
				+ params.windowSize/2,
				params.disparitySearchWidth,
				params.disparityMax,
				params.disparityInconsistency,
				params.doHorizontalBackMatch),
				stride(rowD), TU::size(*rowD)));
#endif
      }
	break;

      case GF:
      {
	GuidedFilter2<S>
		filter(params.windowSize, params.windowSize, params.sigma);
	
	filter.convolve(make_range_iterator(
			    make_diff_iterator<S>(TU::cbegin(*rowL),
						  TU::cbegin(*rowR),
						  params.disparitySearchWidth,
						  params.intensityDiffMax),
			    std::make_tuple(stride(rowL), stride(rowR)),
			    TU::size(*rowL)),
			make_range_iterator(
			    make_diff_iterator<S>(TU::cbegin(*rowLe),
						  TU::cbegin(*rowRe),
						  params.disparitySearchWidth,
						  params.intensityDiffMax),
			    std::make_tuple(stride(rowLe), stride(rowRe)),
			    TU::size(*rowLe)),
			rowL, rowLe,
#ifdef SCORE_ARRAY2
			scores.begin());
#else
			make_range_iterator(
			    make_matching_iterator<S>(
				TU::begin(*rowD),
				params.disparitySearchWidth,
				params.disparityMax,
				params.disparityInconsistency,
				params.doHorizontalBackMatch),
			    stride(rowD), TU::size(*rowD)));
#endif
      }
	break;

      case TF:
      {
	using wfunc_type	= MyDiff<T, float>;

	boost::TreeFilter<Array<S>, wfunc_type>	filter(wfunc_type(),
						       params.sigma);
	
	filter.convolve(make_range_iterator(
			    make_diff_iterator<S>(TU::cbegin(*rowL),
						  TU::cbegin(*rowR),
						  params.disparitySearchWidth,
						  params.intensityDiffMax),
			    std::make_tuple(stride(rowL), stride(rowR)),
			    TU::size(*rowL)),
			make_range_iterator(
			    make_diff_iterator<S>(TU::cbegin(*rowLe),
						  TU::cbegin(*rowRe),
						  params.disparitySearchWidth,
						  params.intensityDiffMax),
			    std::make_tuple(stride(rowLe), stride(rowRe)),
			    TU::size(*rowLe)),
			rowL, rowLe,
#ifdef SCORE_ARRAY2
			scores.begin(),
#else
			make_range_iterator(
			    make_matching_iterator<S>(
				TU::begin(*rowD),
				params.disparitySearchWidth,
				params.disparityMax,
				params.disparityInconsistency,
				params.doHorizontalBackMatch),
			    stride(rowD), TU::size(*rowD)),
#endif
			true);
      }
	break;

      default:
	break;
    }

#ifdef SCORE_ARRAY2
    std::copy(scores.cbegin(), scores.cend(),
	      make_range_iterator(make_matching_iterator<S>(
				      TU::begin(*rowD),
				      params.disparitySearchWidth,
				      params.disparityMax,
				      params.disparityInconsistency,
				      params.doHorizontalBackMatch),
				  stride(rowD), TU::size(*rowD)));
#endif
    disparityMap.save(std::cout);
}

}
/************************************************************************
*  global functions							*
************************************************************************/
int
main(int argc, char* argv[])
{
    using namespace	TU;

    typedef u_char	pixel_type;
    typedef float	score_type;
    
    Algorithm	algo			= SAD;
    std::string	paramFile		= DEFAULT_PARAM_FILE;
    std::string	configDirs		= DEFAULT_CONFIG_DIRS;
    double	scale			= DEFAULT_SCALE;
    size_t	windowSize		= 0;
    size_t	disparitySearchWidth	= 0;
    size_t	disparityMax		= 0;
    bool	doHorizontalBackMatch	= true;
    size_t	grainSize		= DEFAULT_GRAINSIZE;
    
  // $B%3%^%s%I9T$N2r@O!%(B
    extern char*	optarg;
    for (int c; (c = getopt(argc, argv, "GMTp:c:s:w:d:m:Hg:")) != EOF; )
	switch (c)
	{
	  case 'G':
	    algo = GF;
	    break;
	  case 'M':
	    algo = WMF;
	    break;
	  case 'T':
	    algo = TF;
	    break;
	  case 'p':
	    paramFile = optarg;
	    break;
	  case 'c':
	    configDirs = optarg;
	    break;
	  case 's':
	    scale = atof(optarg);
	    break;
	  case 'w':
	    windowSize = atoi(optarg);
	    break;
	  case 'd':
	    disparitySearchWidth = atoi(optarg);
	    break;
	  case 'm':
	    disparityMax = atoi(optarg);
	    break;
	  case 'H':
	    doHorizontalBackMatch = false;
	    break;
	  case 'g':
	    grainSize = atoi(optarg);
	    break;
	}
    
  // $BK\Ev$N$*;E;v!%(B
    try
    {
	std::ifstream		in;
	openFile(in, paramFile, configDirs, ".params");
	StereoParameters	params;
	params.get(in);
	    
	if (windowSize != 0)
	    params.windowSize		= windowSize;
	if (disparityMax != 0)
	    params.disparityMax		= disparityMax;
	if (disparitySearchWidth != 0)
	    params.disparitySearchWidth = disparitySearchWidth;
	params.doHorizontalBackMatch	= doHorizontalBackMatch;
	params.grainSize		= grainSize;

      // $B%9%F%l%*%^%C%A%s%0%Q%i%a!<%?$rI=<(!%(B
	std::cerr << "--- Stereo matching parameters ---\n";
	params.put(std::cerr);

      // $B2hA|$rFI$_9~$`!%(B
	Image<pixel_type>	imageL, imageR;
	imageL.restore(std::cin);
	imageR.restore(std::cin);

	doJob<score_type>(imageL, imageR, params, scale, algo);
    }
    catch (std::exception& err)
    {
	std::cerr << err.what() << std::endl;
	return 1;
    }

    return 0;
}
