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
 *  $Id: Movie.h,v 1.2 2009-07-27 07:32:05 ueshiba Exp $
 */
#ifndef __TUMovie_h
#define __TUMovie_h

#include <utility>
#include "TU/Image++.h"
#include "TU/Manip.h"

namespace TU
{
/************************************************************************
*  class Movie<T>							*
************************************************************************/
//! $B%`!<%S!<$rI=$9%/%i%9(B
/*!
  $BJ#?t$N(Bview$B$r;}$D$3$H$,$G$-!$$=$N%5%$%:$O3F(Bview$BKh$KJL8D$K;XDj$G$-$k!%(B
  \param T	$B2hAG$N7?(B
*/
template <class T> class Movie
{
  private:
    class ImageWithOffset : public Image<T>
    {
      public:
	ImageWithOffset()	:Image<T>(), offset(0)			{}
	
	u_int	offset;
    };
    
  public:
    Movie()								;
    ~Movie()								;

  // Inquire movie status.
			operator bool()				const	;
    u_int		nframes()				const	;
    u_int		nviews()				const	;
    u_int		currentFrame()				const	;
    u_int		currentView()				const	;
    u_int		width()					const	;
    u_int		height()				const	;
    const Image<T>&	image()					const	;
    Image<T>&		image()						;

  // Change movie status.
    Movie&		setFrame(u_int frame)				;
    Movie&		setView(u_int view)				;
    Movie&		rewind()					;
    Movie&		operator ++()					;
    Movie&		operator --()					;

  // Allocate frames.
    u_int		alloc(const Array<std::pair<u_int, u_int> >& sizes,
			      u_int nf)					;

  // Restore/Save movie.
    std::istream&	restore(std::istream& in)			;
    std::ostream&	save(std::ostream& out,
			     ImageBase::Type type=ImageBase::DEFAULT)	;
    ImageBase::Type	saveHeader(std::ostream& out,
				   ImageBase::Type
				       type=ImageBase::DEFAULT)	const	;
    std::ostream&	saveFrame(std::ostream& out,
				  ImageBase::Type
				      type=ImageBase::DEFAULT)	const	;

  private:
    ImageBase::Type	restoreHeader(std::istream& in)			;
    std::istream&	restoreFrames(std::istream& in,
				      ImageBase::Type type, int m)	;
    static u_int	nelements(u_int npixels)			;
    
  private:
    Array<ImageWithOffset>	_images;
    u_int			_nelements;	// # of elements per frame.
    u_int			_cView;		// current view #.
    Array<Array<T> >		_frames;
    u_int			_cFrame;	// current frame #.
#ifdef DEBUG
    std::ofstream		_log;
#endif
};

//! $B%`!<%S!<$r@8@.$9$k!%(B
template <class T> inline
Movie<T>::Movie()
    :_images(0), _nelements(0), _cView(0), _frames(0), _cFrame(0)
{
#ifdef DEBUG
    _log.open("Movie.log");
#endif
}

//! $B%`!<%S!<$rGK2u$9$k!%(B
template <class T> inline
Movie<T>::~Movie()
{
#ifdef DEBUG
    _log.close();
#endif
}

//! 
template <class T> inline
Movie<T>::operator bool() const
{
    return (_cFrame < nframes() && _cView < nviews());
}

//! $B%U%l!<%`?t$rJV$9!%(B
/*!
  \return	$B%U%l!<%`?t(B
*/
template <class T> inline u_int
Movie<T>::nframes() const
{
    return _frames.dim();
}

//! view$B?t$rJV$9!%(B
/*!
  \return	view$B?t(B
*/
template <class T> inline u_int
Movie<T>::nviews() const
{
    return _images.dim();
}

//! $B8=:_$N%U%l!<%`HV9f$rJV$9!%(B
/*!
  \return	$B%U%l!<%`HV9f(B
*/
template <class T> inline u_int
Movie<T>::currentFrame() const
{
    return _cFrame;
}

//! $B8=:_$N(Bview$BHV9f$rJV$9!%(B
/*!
  \return	view$BHV9f(B
*/
template <class T> inline u_int
Movie<T>::currentView() const
{
    return _cView;
}

//! $B8=:_$N(Bview$B$N2hA|$NI}$rJV$9!%(B
/*!
  \return	$B2hA|$NI}(B
*/
template <class T> inline u_int
Movie<T>::width() const
{
    return (_cView < nviews() ? _images[_cView].width() : 0);
}
    
//! $B8=:_$N(Bview$B$N2hA|$N9b$5$rJV$9!%(B
/*!
  \return	$B2hA|$N9b$5(B
*/
template <class T> inline u_int
Movie<T>::height() const
{
    return (_cView < nviews() ? _images[_cView].height() : 0);
}

//! $B8=:_$N(Bview$B$H(Bframe$B$KBP1~$9$k2hA|$rJV$9!%(B
/*!
  \return	$B2hA|(B
*/
template <class T> inline const Image<T>&
Movie<T>::image() const
{
    return _images[_cView];
}

//! $B8=:_$N(Bview$B$H(Bframe$B$KBP1~$9$k2hA|$rJV$9!%(B
/*!
  \return	$B2hA|(B
*/
template <class T> inline Image<T>&
Movie<T>::image()
{
    return _images[_cView];
}

//! $B8=:_$N%U%l!<%`$r;XDj$9$k!%(B
/*!
  frame < #nframes() $B$G$J$$>l9g$O8=:_%U%l!<%`$O(B #nframes() $B$H$J$j!$(B
  #operator ()$B$G(Bfalse$B$,JV$5$l$k>uBV$K$J$k!%(B
  \param frame	$B%U%l!<%`HV9f(B
  \return	$B$3$N%`!<%S!<%*%V%8%'%/%H(B
*/
template <class T> Movie<T>&
Movie<T>::setFrame(u_int frame)
{
#ifdef DEBUG
    using namespace	std;
    _log << "  Begin: Movie<T>::setFrame(frame = " << frame << ")" << endl;
#endif
    if (frame != _cFrame)
    {
	if (frame < nframes())
	{
	    _cFrame = frame;

	    for (int i = 0; i < nviews(); ++i)
		_images[i].resize((T*)_frames[_cFrame] + _images[i].offset,
				  _images[i].height(), _images[i].width());
	}
	else
	    _cFrame = nframes();
    }
#ifdef DEBUG
    _log << "  End:   Movie<T>::setFrame()" << endl;
#endif
    return *this;
}

//! $B8=:_$N(Bview$B$r;XDj$9$k!%(B
/*!
  view < #nviews() $B$G$J$$>l9g$O2?$bJQ99$7$J$$!%(B
  \param view	view$BHV9f(B
  \return	$B$3$N%`!<%S!<%*%V%8%'%/%H(B
*/
template <class T> inline Movie<T>&
Movie<T>::setView(u_int view)
{
#ifdef DEBUG
    using namespace	std;
    _log << "  Begin: Movie<T>::setView(view = " << view << ")" << endl;
#endif
    if (view < nviews())
	_cView = view;
#ifdef DEBUG
    _log << "  End:   Movie<T>::setView()" << endl;
#endif
    return *this;
}

//! $B8=:_$N%U%l!<%`$r:G=i(B(0)$B$KLa$9!%(B
/*!
  \return	$B$3$N%`!<%S!<%*%V%8%'%/%H(B
*/
template <class T> inline Movie<T>&
Movie<T>::rewind()
{
    return setFrame(0);
}

//! $B8=:_$N%U%l!<%`$r(B1$B$D@h$K?J$a$k!%(B
/*!
  $B4{$K:G8e$N%U%l!<%`$KC#$7$F$$$k>l9g$O%U%l!<%`HV9f$r(B#nframes()$B$K$9$k!%(B
  \return	$B$3$N%`!<%S!<%*%V%8%'%/%H(B
*/
template <class T> inline Movie<T>&
Movie<T>::operator ++()
{
    if (_cFrame < nframes())
	setFrame(_cFrame + 1);
    return *this;
}

//! $B8=:_$N%U%l!<%`$r(B1$B$DLa$9!%(B
/*!
  $B4{$K:G=i$N%U%l!<%`$KC#$7$F$$$k>l9g$O2?$b$7$J$$!%(B
  \return	$B$3$N%`!<%S!<%*%V%8%'%/%H(B
*/
template <class T> inline Movie<T>&
Movie<T>::operator --()
{
    if (_cFrame > 0)
	setFrame(_cFrame - 1);
    return *this;
}

template <class T> u_int
Movie<T>::alloc(const Array<std::pair<u_int, u_int> >& sizes, u_int nf)
{
#ifdef DEBUG
    using namespace	std;
    _log << "  Begin: Movie<T>::alloc(nframes = " << nf
	 << ", nviews = " << sizes.dim()
	 << ")" << endl;
#endif
  // $B3F(Bview$B$N%*%U%;%C%H$H(B1$B%U%l!<%`$"$?$j$N2hAG?t$r@_Dj!%(B
    _images.resize(sizes.dim());
    _nelements = 0;
    for (int i = 0; i < nviews(); ++i)
    {
	_images[i].offset = _nelements;
	_nelements += nelements(sizes[i].first * sizes[i].second);
    }
	     
  // $B;XDj$5$l$?Kg?t$N%U%l!<%`$r@_Dj!%(B
    _frames.resize(nf);
    for (int j = 0; j < _frames.dim(); ++j)
    {
	try
	{
	    _frames[j].resize(_nelements);
#ifdef DEBUG
	    _log << "    " << j << "-th frame allocated..." << endl;
#endif
	}
	catch (...)
	{
	    _frames.resize(j);
#ifdef DEBUG
	    _log << "    " << j << "-th frame cannot be allocated!" << endl;
#endif
	    break;
	}
    }
    
  // $B;XDj$5$l$?8D?t$N(Bview$B$H$=$NBg$-$5$r@_Dj!%(B
    if (nframes() > 0)
	for (int i = 0; i < nviews(); ++i)
	    _images[i].resize((T*)_frames[0] + _images[i].offset,
			      sizes[i].second, sizes[i].first);
    
    _cFrame = 0;
    _cView  = 0;
#ifdef DEBUG
    _log << "  End:   Movie<T>::alloc()" << endl;
#endif
    return nframes();			// Return #frames allocated correctly.
}

//! $BF~NO%9%H%j!<%`$+$i%`!<%S!<$rFI$_9~$`!%(B
/*!
  $BF~NO%9%H%j!<%`$NKvHx$KC#$9$k$^$G%U%l!<%`$rFI$_B3$1$k!%(B
  \param in	$BF~NO%9%H%j!<%`(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
template <class T> inline std::istream&
Movie<T>::restore(std::istream& in)
{
    return restoreFrames(in, restoreHeader(in), 0);
}

template <class T> ImageBase::Type
Movie<T>::restoreHeader(std::istream& in)
{
    u_int	nv;
    in >> nv >> skipl;
    _images.resize(nv);

    ImageBase::Type	type = ImageBase::DEFAULT;
    _nelements = 0;
    for (int i = 0; i < nviews(); ++i)
    {
	type = _images[i].restoreHeader(in);
	_images[i].offset = _nelements;
	_nelements += nelements(_images[i].width() * _images[i].height());
    }

    return type;
}

template <class T> std::istream&
Movie<T>::restoreFrames(std::istream& in, ImageBase::Type type, int m)
{
    char	c;
    if (!in.get(c))
    {
	_frames.resize(m);
	return in;
    }
    in.putback(c);

    try
    {
	Array<T>	frame(_nelements);
	for (int i = 0; i < nviews(); ++i)
	{
	    _images[i].resize((T*)frame + _images[i].offset,
			      _images[i].height(), _images[i].width());
	    _images[i].restoreData(in, type);
	}
	restoreFrames(in, type, m + 1);
	_frames[m] = frame;
    }
    catch (...)
    {
	_frames.resize(m);
    }
        
    return in;
}

//! $B%`!<%S!<$r;XDj$7$?2hAG%?%$%W$G=PNO%9%H%j!<%`$K=q$-=P$9!%(B
/*!
 \param out	$B=PNO%9%H%j!<%`(B
 \param type	$B2hAG%?%$%W!%$?$@$7!$(B#DEFAULT$B$r;XDj$7$?>l9g$O!$$3$N%`!<%S!<$N(B
		$B2hAG%?%$%W$G=q$-=P$5$l$k!%(B   
 \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
template <class T> std::ostream&
Movie<T>::save(std::ostream& out, ImageBase::Type type)
{
    u_int	cFrame = _cFrame;

    saveHeader(out, type);
    for (rewind(); *this; ++(*this))
	saveFrame(out, type);
    setFrame(_cFrame);
    
    return out;
}

//! $B%`!<%S!<$N%X%C%@$r;XDj$7$?2hAG%?%$%W$G=PNO%9%H%j!<%`$K=q$-=P$9!%(B
/*!
 \param out	$B=PNO%9%H%j!<%`(B
 \param type	$B2hAG%?%$%W!%$?$@$7!$(B#DEFAULT$B$r;XDj$7$?>l9g$O!$$3$N%`!<%S!<$N(B
		$B2hAG%?%$%W$G=q$-=P$5$l$k!%(B   
 \return	$B<B:]$K=q$-=P$9>l9g$N2hAG%?%$%W(B
*/
template <class T> ImageBase::Type
Movie<T>::saveHeader(std::ostream& out, ImageBase::Type type) const
{
    using namespace	std;
    
    out << nviews() << endl;
    for (int i = 0; i < nviews(); ++i)
	type = _images[i].saveHeader(out, type);
    return type;
}

//! $B8=:_$N%U%l!<%`$r;XDj$7$?2hAG%?%$%W$G=PNO%9%H%j!<%`$K=q$-=P$9!%(B
/*!
 \param out	$B=PNO%9%H%j!<%`(B
 \param type	$B2hAG%?%$%W!%$?$@$7!$(B#DEFAULT$B$r;XDj$7$?>l9g$O!$$3$N%`!<%S!<$N(B
		$B2hAG%?%$%W$G=q$-=P$5$l$k!%(B   
 \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
template <class T> std::ostream&
Movie<T>::saveFrame(std::ostream& out, ImageBase::Type type) const
{
    for (int i = 0; i < nviews(); ++i)
	_images[i].saveData(out, type);
    return out;
}

template <class T> inline u_int
Movie<T>::nelements(u_int npixels)
{
    return npixels;
}

template <> inline u_int
Movie<YUV411>::nelements(u_int npixels)
{
    return npixels / 2;
}

}
#endif	// !__TUMovie_h
