/*
 *  $BJ?(B9$BG/(B $BEE;R5;=QAm9g8&5f=j(B $B?"<G=SIW(B $BCx:n8"=jM-(B
 *
 *  $BCx:n<T$K$h$k5v2D$J$7$K$3$N%W%m%0%i%`$NBh;0<T$X$N3+<(!"J#@=!"2~JQ!"(B
 *  $B;HMQEy$=$NB>$NCx:n?M3J8"$r?/32$9$k9T0Y$r6X;_$7$^$9!#(B
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *
 *  Copyright 1996
 *  Toshio UESHIBA, Electrotechnical Laboratory
 *
 *  All rights reserved.
 *  Any changing, copying or giving information about source programs of
 *  any part of this software and/or documentation without permission of the
 *  authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damage in use of this program.
 */

/*
 *  $Id: Image++.h,v 1.27 2007-07-25 23:43:13 ueshiba Exp $
 */
#ifndef	__TUImagePP_h
#define	__TUImagePP_h

#include <string.h>
#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  struct RGB, BGR, RGBA & ABGR						*
************************************************************************/
struct BGR;
struct RGBA;
struct ABGR;
struct YUV444;

//! Red, Green, Blue$B!J3F(B8bit$B!K$N=g$GJB$s$@%+%i!<2hAG(B
struct RGB
{
    RGB()					:r(0),  g(0),  b(0)	{}
    RGB(u_char rr, u_char gg, u_char bb)	:r(rr), g(gg), b(bb)	{}
    RGB(const BGR& p)							;
    RGB(const RGBA& p)							;
    RGB(const ABGR& p)							;
    RGB(const YUV444& p)						;
    template <class T>
    RGB(const T& p)	:r(u_char(p)), g(u_char(p)), b(u_char(p))	{}

		operator u_char()	const	{return u_char(double(*this));}
		operator short()	const	{return short(double(*this));}
		operator int()		const	{return int(double(*this));}
		operator float()	const	{return float(double(*this));}
		operator double()	const	{return 0.3*r+0.59*g+0.11*b+0.5;}
    
    RGB&	operator +=(const RGB& p)	{r += p.r; g += p.g; b += p.b;
						 return *this;}
    RGB&	operator -=(const RGB& p)	{r -= p.r; g -= p.g; b -= p.b;
						 return *this;}
    bool	operator ==(const RGB& p) const	{return (r == p.r &&
							 g == p.g &&
							 b == p.b);}
    bool	operator !=(const RGB& p) const	{return !(*this == p);}
    
    u_char	r, g, b;
};

inline std::istream&
operator >>(std::istream& in, RGB& p)
{
    return in >> (u_int&)p.r >> (u_int&)p.g >> (u_int&)p.b;
}

inline std::ostream&
operator <<(std::ostream& out, const RGB& p)
{
    return out << (u_int)p.r << ' ' << (u_int)p.g << ' ' << (u_int)p.b;
}

//! Blue, Green, Red$B!J3F(B8bit$B!K$N=g$GJB$s$@%+%i!<2hAG(B
struct BGR
{
    BGR()					:b(0),   g(0),   r(0)	{}
    BGR(u_char rr, u_char gg, u_char bb)	:b(bb),  g(gg),  r(rr)	{}
    BGR(const RGB& p)				:b(p.b), g(p.g), r(p.r)	{}
    BGR(const RGBA& p)							;
    BGR(const ABGR& p)							;
    BGR(const YUV444& p)						;
    template <class T>
    BGR(const T& c)	:b(u_char(c)), g(u_char(c)), r(u_char(c))	{}

		operator u_char()	const	{return u_char(double(*this));}
		operator short()	const	{return short(double(*this));}
		operator int()		const	{return int(double(*this));}
		operator float()	const	{return float(double(*this));}
		operator double()	const	{return 0.3*r+0.59*g+0.11*b+0.5;}

    BGR&	operator +=(const BGR& p)	{r += p.r; g += p.g; b += p.b;
						 return *this;}
    BGR&	operator -=(const BGR& p)	{r -= p.r; g -= p.g; b -= p.b;
						 return *this;}
    bool	operator ==(const BGR& p) const	{return (r == p.r &&
							 g == p.g &&
							 b == p.b);}
    bool	operator !=(const BGR& p) const	{return !(*this != p);}
    
    u_char	b, g, r;
};

inline
RGB::RGB(const BGR& p)	:r(p.r), g(p.g), b(p.b)	{}

inline std::istream&
operator >>(std::istream& in, BGR& p)
{
    return in >> (u_int&)p.r >> (u_int&)p.g >> (u_int&)p.b;
}

inline std::ostream&
operator <<(std::ostream& out, const BGR& p)
{
    return out << (u_int)p.r << ' ' << (u_int)p.g << ' ' << (u_int)p.b;
}

struct Alpha
{
    Alpha(u_char aa=255)	:a(aa)			{}

    int		operator ==(const Alpha& p) const	{return a == p.a;}
    int		operator !=(const Alpha& p) const	{return !(*this == p);}
    
    u_char	a;
};

//! Red, Green, Blue, Alpha$B!J3F(B8bit$B!K$N=g$GJB$s$@%+%i!<2hAG(B
struct RGBA : public RGB, public Alpha
{
    RGBA()		:RGB(),        Alpha()		{}
    RGBA(u_char r, u_char g, u_char b, u_char a=255)
			:RGB(r, g, b), Alpha(a)		{}
    template <class T>
    RGBA(const T& p)	:RGB(p),       Alpha()		{}

    bool	operator ==(const RGBA& p) const
		{return (Alpha::operator ==(p) && RGB::operator ==(p));}
    bool	operator !=(const RGBA& p)	const	{return !(*this != p);}
};

//! Alpha, Blue, Green, Red$B!J3F(B8bit$B!K$N=g$GJB$s$@%+%i!<2hAG(B
struct ABGR : public Alpha, public BGR
{
    ABGR()		:Alpha(),  BGR()		{}
    ABGR(u_char r, u_char g, u_char b, u_char a=255)
			:Alpha(a), BGR(r, g, b)  	{}
    template <class T>
    ABGR(const T& p)	:Alpha(),  BGR(p)		{}

    bool	operator ==(const ABGR& p) const
		{return (Alpha::operator ==(p) && BGR::operator ==(p));}
    bool	operator !=(const ABGR& p) const	{return !(*this != p);}
};

inline
RGB::RGB(const RGBA& p)	:r(p.r), g(p.g), b(p.b)	{}

inline
RGB::RGB(const ABGR& p)	:r(p.r), g(p.g), b(p.b)	{}

inline
BGR::BGR(const RGBA& p)	:r(p.r), g(p.g), b(p.b)	{}

inline
BGR::BGR(const ABGR& p)	:r(p.r), g(p.g), b(p.b)	{}

/************************************************************************
*  struct YUV444, YUV422, YUV411					*
************************************************************************/
//! Y, U, V$B!J3F(B8bit$B!K$N=g$GJB$s$@%+%i!<2hAG(B
struct YUV444
{
    YUV444(u_char yy=0, u_char uu=128, u_char vv=128)
			:u(uu), y(yy), v(vv)		{}
    template <class T> 
    YUV444(const T& p)	:u(128), y(u_char(p)), v(128)	{}

		operator u_char()		const	{return u_char(y);}
		operator short()		const	{return short(y);}
		operator int()			const	{return int(y);}
		operator float()		const	{return float(y);}
		operator double()		const	{return double(y);}
    bool	operator ==(const YUV444& yuv)	const	{return (u == yuv.u &&
								 y == yuv.y &&
								 v == yuv.v);}
    bool	operator !=(const YUV444& yuv)	const	{return !(*this==yuv);}
    

    u_char	u, y, v;
};
    
inline std::istream&
operator >>(std::istream& in, YUV444& yuv)
{
    return in >> (u_int&)yuv.y >> (u_int&)yuv.u >> (u_int&)yuv.v;
}

inline std::ostream&
operator <<(std::ostream& out, const YUV444& yuv)
{
    return out << (u_int)yuv.y << ' ' << (u_int)yuv.u << ' ' << (u_int)yuv.v;
}

//! [U, Y0], [V, Y1]$B!J3F(B8bit$B!K$N=g$GJB$s$@%+%i!<2hAG(B(16bits/pixel)
struct YUV422
{
    YUV422(u_char yy=0, u_char xx=128)	:x(xx), y(yy)	{}
    template <class T>
    YUV422(const T& p)		:x(128), y(u_char(p))	{}

		operator u_char()		const	{return u_char(y);}
		operator short()		const	{return short(y);}
		operator int()			const	{return int(y);}
		operator float()		const	{return float(y);}
		operator double()		const	{return double(y);}
    bool	operator ==(const YUV422& p)	const	{return (x == p.x &&
								 y == p.y);}
    bool	operator !=(const YUV422& p)	const	{return !(*this == p);}
    
    u_char	x, y;
};

inline std::istream&
operator >>(std::istream& in, YUV422& yuv)
{
    return in >> (u_int&)yuv.y >> (u_int&)yuv.x;
}

inline std::ostream&
operator <<(std::ostream& out, const YUV422& yuv)
{
    return out << (u_int)yuv.y << ' ' << (u_int)yuv.x;
}

//! [U, Y0, Y1], [V, Y2, Y3]$B!J3F(B8bit$B!K$N=g$GJB$s$@%+%i!<2hAG(B(12bits/pixel)
struct YUV411
{
    YUV411(u_char yy0=0, u_char yy1=0, u_char xx=128)
			:x(xx), y0(yy0), y1(yy1)		{}
    template <class T>
    YUV411(const T& p)	:x(128), y0(u_char(p)), y1(u_char(p))	{}

    bool	operator ==(const YUV411& p)	const	{return (x  == p.x  &&
								 y0 == p.y0 &&
								 y1 == p.y1);}
    bool	operator !=(const YUV411& p)	const	{return !(*this == p);}
    
    u_char	x, y0, y1;
};

inline std::istream&
operator >>(std::istream& in, YUV411& yuv)
{
    return in >> (u_int&)yuv.y0 >> (u_int&)yuv.y1 >> (u_int&)yuv.x;
}

inline std::ostream&
operator <<(std::ostream& out, const YUV411& yuv)
{
    return out << (u_int)yuv.y0 << ' ' << (u_int)yuv.y1 << ' ' << (u_int)yuv.x;
}

/************************************************************************
*  function fromYUV<T>()						*
************************************************************************/
//! $B%+%i!<$N(BY, U, V$BCM$rM?$($FB>$N%+%i!<I=8=$KJQ49$9$k%/%i%9(B
class ConversionFromYUV
{
  public:
    ConversionFromYUV()					;

  private:
    template <class T>
    friend T	fromYUV(u_char y, u_char u, u_char v)	;
    
    int		_r[256], _g0[256], _g1[256], _b[256];
};

extern const ConversionFromYUV	conversionFromYUV;

template <class T> inline T
fromYUV(u_char y, u_char u, u_char v)
{
    T	val;
    int	tmp = y + conversionFromYUV._r[v];
    val.r = (tmp > 255 ? 255 : tmp < 0 ? 0 : tmp);
    tmp   =
	y - (int(conversionFromYUV._g0[v] + conversionFromYUV._g1[u]) >> 10);
    val.g = (tmp > 255 ? 255 : tmp < 0 ? 0 : tmp);
    tmp   = y + conversionFromYUV._b[u];
    val.b = (tmp > 255 ? 255 : tmp < 0 ? 0 : tmp);
    return val;
}

template <> inline u_char
fromYUV<u_char>(u_char y, u_char, u_char)
{
    return y;
}

template <> inline short
fromYUV<short>(u_char y, u_char, u_char)
{
    return y;
}

template <> inline int
fromYUV<int>(u_char y, u_char, u_char)
{
    return y;
}

template <> inline float
fromYUV<float>(u_char y, u_char, u_char)
{
    return y;
}

template <> inline double
fromYUV<double>(u_char y, u_char, u_char)
{
    return y;
}

template <> inline YUV444
fromYUV<YUV444>(u_char y, u_char u, u_char v)
{
    return YUV444(y, u, v);
}

inline
RGB::RGB(const YUV444& p)
{
    *this = fromYUV<RGB>(p.y, p.u, p.v);
}

inline
BGR::BGR(const YUV444& p)
{
    *this = fromYUV<BGR>(p.y, p.u, p.v);
}

/************************************************************************
*  class ImageBase:	basic image class				*
************************************************************************/
//! $B2hAG$N(B2$B<!85G[Ns$H$7$FDj5A$5$l$?$"$i$f$k2hA|$N4pDl$H$J$k%/%i%9(B
class ImageBase
{
  public:
    enum Type		{END = 0, U_CHAR = 5, RGB_24 = 6,
			 SHORT, INT, FLOAT, DOUBLE,
			 YUV_444, YUV_422, YUV_411};
    
  protected:
    ImageBase()
	:P(3, 4), d1(0), d2(0)		{P[0][0] = P[1][1] = P[2][2] = 1.0;}
    virtual ~ImageBase()		;
    
    static u_int	type2depth(Type type)		;
    
  public:
    Type		restoreHeader(std::istream& in)			;
    std::ostream&	saveHeader(std::ostream& out, Type type) const	;

    u_int		width()			const	{return _width();}
    u_int		height()		const	{return _height();}
    void		resize(u_int h, u_int w)	{_resize(h, w, END);}
	
  private:
    virtual u_int	_width()			const	= 0;
    virtual u_int	_height()			const	= 0;
    virtual void	_resize(u_int h, u_int w, Type type)	= 0;

  public:
    Matrix34d		P;			//!< 3x4$B%+%a%i9TNs(B
    double		d1, d2;			//!< $B%l%s%:OD$_78?t(B
};

/************************************************************************
*  class ImageLine<T>:	Generic image scanline class			*
************************************************************************/
//! T$B7?$N2hAG$r;}$D2hA|$N%9%-%c%s%i%$%s$rI=$9%/%i%9(B
/*!
  \param T	$B2hAG$N7?(B
*/
template <class T>
class ImageLine : public Array<T>
{
  public:
    explicit ImageLine(u_int d=0)
        :Array<T>(d), _lmost(0), _rmost(d)		{*this = 0;}
    ImageLine(T* p, u_int d)
        :Array<T>(p, d), _lmost(0), _rmost(d)		{}
    ImageLine&		operator =(T c)
			{
			    Array<T>::operator =(c);
			    return *this;
			}

    using		Array<T>::dim;
    const YUV422*	fill(const YUV422* src)		;
    const YUV411*	fill(const YUV411* src)		;
    const T*		fill(const T* src)		;
    template <class S>
    const S*		fill(const S* src)		;
    int			lmost()			const	{return _lmost;}
    int			rmost()			const	{return _rmost;}
    void		setLimits(int l, int r)		{_lmost = l;
							 _rmost = r;}
    bool		valid(int u)		const	{return (u >= _lmost &&
								 u <  _rmost);}
	
    bool		resize(u_int d)			;
    void		resize(T* p, u_int d)		;

  private:
    int			_lmost;
    int			_rmost;
};

template <class T> inline const T*
ImageLine<T>::fill(const T* src)
{
    memcpy((T*)*this, src, dim() * sizeof(T));
    return src + dim();
}

template <class T> inline bool
ImageLine<T>::resize(u_int d)
{
    _lmost = 0;
    _rmost = d;
    return Array<T>::resize(d);
}

template <class T> inline void
ImageLine<T>::resize(T* p, u_int d)
{
    _lmost = 0;
    _rmost = d;
    Array<T>::resize(p, d);
}

template <>
class ImageLine<YUV422> : public Array<YUV422>
{
  public:
    explicit ImageLine(u_int d=0)
	:Array<YUV422>(d), _lmost(0), _rmost(d)		{*this = 0;}
    ImageLine(YUV422* p, u_int d)
	:Array<YUV422>(p, d), _lmost(0), _rmost(d)	{}
    ImageLine&		operator =(YUV422 c)
			{
			    Array<YUV422>::operator =(c);
			    return *this;
			}
    const YUV444*	fill(const YUV444* src)		;
    const YUV422*	fill(const YUV422* src)		;
    const YUV411*	fill(const YUV411* src)		;
    template <class S>
    const S*		fill(const S* src)		;
    int			lmost()			const	{return _lmost;}
    int			rmost()			const	{return _rmost;}
    void		setLimits(int l, int r)		{_lmost = l;
							 _rmost = r;}
    bool		valid(int u)		const	{return (u >= _lmost &&
								 u <  _rmost);}
	
    bool		resize(u_int d)			;
    void		resize(YUV422* p, u_int d)	;

  private:
    int			_lmost;
    int			_rmost;
};

inline const YUV422*
ImageLine<YUV422>::fill(const YUV422* src)
{
    memcpy((YUV422*)*this, src, dim() * sizeof(YUV422));
    return src + dim();
}

inline bool
ImageLine<YUV422>::resize(u_int d)
{
    _lmost = 0;
    _rmost = d;
    return Array<YUV422>::resize(d);
}

inline void
ImageLine<YUV422>::resize(YUV422* p, u_int d)
{
    _lmost = 0;
    _rmost = d;
    Array<YUV422>::resize(p, d);
}

template <>
class ImageLine<YUV411> : public Array<YUV411>
{
  public:
    explicit ImageLine(u_int d=0)
	:Array<YUV411>(d), _lmost(0), _rmost(d)		{*this = 0;}
    ImageLine(YUV411* p, u_int d)
	:Array<YUV411>(p, d), _lmost(0), _rmost(d)	{}
    ImageLine&		operator =(YUV411 c)
			{
			    Array<YUV411>::operator =(c);
			    return *this;
			}
    const YUV444*	fill(const YUV444* src)		;
    const YUV422*	fill(const YUV422* src)		;
    const YUV411*	fill(const YUV411* src)		;
    template <class S>
    const S*		fill(const S* src)		;
    int			lmost()			const	{return _lmost;}
    int			rmost()			const	{return _rmost;}
    void		setLimits(int l, int r)		{_lmost = l;
							 _rmost = r;}
    bool		valid(int u)		const	{return (u >= _lmost &&
								 u <  _rmost);}
	
    bool		resize(u_int d)			;
    void		resize(YUV411* p, u_int d)	;

  private:
    int			_lmost;
    int			_rmost;
};

inline const YUV411*
ImageLine<YUV411>::fill(const YUV411* src)
{
    memcpy((YUV411*)*this, src, dim() * sizeof(YUV411));
    return src + dim();
}

inline bool
ImageLine<YUV411>::resize(u_int d)
{
    _lmost = 0;
    _rmost = d;
    return Array<YUV411>::resize(d);
}

inline void
ImageLine<YUV411>::resize(YUV411* p, u_int d)
{
    _lmost = 0;
    _rmost = d;
    Array<YUV411>::resize(p, d);
}

/************************************************************************
*  class Image<T>:	Generic image class				*
************************************************************************/
//! T$B7?$N2hAG$r;}$D2hA|$rI=$9%/%i%9(B
/*!
  \param T	$B2hAG$N7?(B
  \param B	$B%P%C%U%!$N7?(B
*/
template <class T, class B=Buf<T> >
class Image : public Array2<ImageLine<T>, B>, public ImageBase
{
  public:
    explicit Image(u_int w=0, u_int h=0)
	:Array2<ImageLine<T>, B>(h, w), ImageBase()		{*this = 0;}
    Image(T* p, u_int w, u_int h)			
	:Array2<ImageLine<T>, B>(p, h, w), ImageBase()		{}
    template <class B2>
    Image(const Image<T, B2>& i, int u, int v, u_int w, u_int h)
	:Array2<ImageLine<T>, B>(i, v, u, h, w), ImageBase(i)	{}

    template <class S>
    S		at(const Point2<S>& p)				const	;
    template <class S>
    const T&	operator ()(const Point2<S>& p)
					const	{return (*this)[p[1]][p[0]];}
    template <class S>
    T&		operator ()(const Point2<S>& p)	{return (*this)[p[1]][p[0]];}
    u_int	width()			const	{return
						 Array2<ImageLine<T> >::ncol();}
    u_int	height()		const	{return
						 Array2<ImageLine<T> >::nrow();}
    
    Image&	operator = (T c)		{Array2<ImageLine<T> >::
						 operator =(c); return *this;}
    std::istream&	restore(std::istream& in)			;
    std::ostream&	save(std::ostream& out, Type type)	const	;
    std::istream&	restoreData(std::istream& in, Type type)	;
    std::ostream&	saveData(std::ostream& out, Type type)	const	;
    void		resize(u_int h, u_int w)			;
    void		resize(T* p, u_int h, u_int w)			;

  private:
    template <class S>
    std::istream&	restoreRows(std::istream& in)			;
    template <class D>
    std::ostream&	saveRows(std::ostream& out)		const	;

    virtual u_int	_width()				const	;
    virtual u_int	_height()				const	;
    virtual void	_resize(u_int h, u_int w, Type)			;
};

template <class T, class B> template <class S> inline S
Image<T, B>::at(const Point2<S>& p) const
{
    const int	u  = floor(p[0]), v  = floor(p[1]);
    const S	du = p[0] - u,	  dv = p[1] - v;
    const T	*in0 = &(*this)[v][u], *in1 = &(*this)[v+1][u];
    const S	out0 = *in0 + du*(*(in0 + 1) - *in0),
		out1 = *in1 + du*(*(in1 + 1) - *in1);
    return out0 + dv * (out1 - out0);
}

template <class T, class B> inline std::istream&
Image<T, B>::restore(std::istream& in)
{
    return restoreData(in, restoreHeader(in));
}

template <class T, class B> inline std::ostream&
Image<T, B>::save(std::ostream& out, Type type) const
{
    saveHeader(out, type);
    return saveData(out, type);
}

template <class T, class B> inline void
Image<T, B>::resize(u_int h, u_int w)
{
    Array2<ImageLine<T>, B>::resize(h, w);
}

template <class T, class B> inline void
Image<T, B>::resize(T* p, u_int h, u_int w)
{
    Array2<ImageLine<T>, B>::resize(p, h, w);
}
 
template <> inline
Image<YUV411, Buf<YUV411> >::Image(u_int w, u_int h)
    :Array2<ImageLine<YUV411>, Buf<YUV411> >(h, w/2), ImageBase()
{
    *this = 0;
}

template <> inline
Image<YUV411, Buf<YUV411> >::Image(YUV411* p, u_int w, u_int h)
    :Array2<ImageLine<YUV411>, Buf<YUV411> >(p, h, w/2), ImageBase()
{
}

template <> template <class B2> inline
Image<YUV411, Buf<YUV411> >::Image(const Image<YUV411, B2>& i,
				   int u, int v, u_int w, u_int h)
    :Array2<ImageLine<YUV411>, Buf<YUV411> >(i, v, u/2, h, w/2), ImageBase(i)
{
}

template <> inline u_int
Image<YUV411, Buf<YUV411> >::width() const
{
    return 2 * ncol();
}

template <> inline void
Image<YUV411, Buf<YUV411> >::resize(YUV411* p, u_int h, u_int w)
{
    Array2<ImageLine<YUV411>, Buf<YUV411> >::resize(p, h, w/2);
}

/************************************************************************
*  class IIRFilter							*
************************************************************************/
//! $BJRB&(BInfinite Inpulse Response Filter$B$rI=$9%/%i%9(B
template <u_int D> class IIRFilter
{
  public:
    IIRFilter&	initialize(const float c[D+D])				;
    template <class S, class B, class B2> const IIRFilter&
		forward(const Array<S, B>& in,
			Array<float, B2>& out)			const	;
    template <class S, class B, class B2> const IIRFilter&
		backward(const Array<S, B>& in,
			 Array<float, B2>& out)			const	;
    void	limitsF(float& limit0F,
			float& limit1F, float& limit2F)		const	;
    void	limitsB(float& limit0B,
			float& limit1B, float& limit2B)		const	;
    
  private:
    float	_c[D+D];	// coefficients
};

/************************************************************************
*  class BilateralIIRFilter						*
************************************************************************/
//! $BN>B&(BInfinite Inpulse Response Filter$B$rI=$9%/%i%9(B
template <u_int D> class BilateralIIRFilter
{
  public:
  //! $BHyJ,$N3,?t(B
    enum Order
    {
	Zeroth,						//!< 0$B3,HyJ,(B
	First,						//!< 1$B3,HyJ,(B
	Second						//!< 2$B3,HyJ,(B
    };
    
    BilateralIIRFilter&	initialize(const float cF[D+D], const float cB[D+D]);
    BilateralIIRFilter&	initialize(const float c[D+D], Order order)	;
    template <class S, class B>
    BilateralIIRFilter&	convolve(const Array<S, B>& in)			;
    u_int		dim()					const	;
    float		operator [](int i)			const	;
    void		limits(float& limit0,
			       float& limit1,
			       float& limit2)			const	;
    
  private:
    IIRFilter<D>	_iirF;
    Array<float>	_bufF;
    IIRFilter<D>	_iirB;
    Array<float>	_bufB;
};

//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param cF	$BA0?J(Bz$BJQ4978?t(B. z$BJQ49$O(B 
		\f[
		  H^F(z^{-1}) = \frac{c^F_{D-1} + c^F_{D-2}z^{-1}
		  + c^F_{D-3}z^{-2} + \cdots
		  + c^F_{0}z^{-(D-1)}}{1 - c^F_{2D-1}z^{-1}
		  - c^F_{2D-2}z^{-2} - \cdots - c^F_{D}z^{-D}}
		\f]
		$B$H$J$k(B. 
  \param cB	$B8eB`(Bz$BJQ4978?t(B. z$BJQ49$O(B
		\f[
		  H^B(z) = \frac{c^B_{0}z + c^B_{1}z^2 + \cdots + c^B_{D-1}z^D}
		       {1 - c^B_{D}z - c^B_{D+1}z^2 - \cdots - c^B_{2D-1}z^D}
		\f]
		$B$H$J$k(B.
*/
template <u_int D> inline BilateralIIRFilter<D>&
BilateralIIRFilter<D>::initialize(const float cF[D+D], const float cB[D+D])
{
    _iirF.initialize(cF);
    _iirB.initialize(cB);
#ifdef DEBUG
    float	limit0, limit1, limit2;
    limits(limit0, limit1, limit2);
    std::cerr << "limit0 = " << limit0 << ", limit1 = " << limit1
	      << ", limit2 = " << limit2 << std::endl;
#endif
    return *this;
}

//! $B%U%#%k%?$K$h$k>v$_9~$_$r9T$&(B. $B=PNO$O(B operator [](int) $B$G<h$j=P$9(B
/*!
  \param in	$BF~NO%G!<%?Ns(B
  return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> template <class S, class B> inline BilateralIIRFilter<D>&
BilateralIIRFilter<D>::convolve(const Array<S, B>& in)
{
    _iirF.forward(in, _bufF);
    _iirB.backward(in, _bufB);

    return *this;
}

//! $B>v$_9~$_$N=PNO%G!<%?Ns$N<!85$rJV$9(B
/*!
  \return	$B=PNO%G!<%?Ns$N<!85(B
*/
template <u_int D> inline u_int
BilateralIIRFilter<D>::dim() const
{
    return _bufF.dim();
}

//! $B>v$_9~$_$N=PNO%G!<%?$NFCDj$NMWAG$rJV$9(B
/*!
  \param i	$BMWAG$N(Bindex
  \return	$BMWAG$NCM(B
*/
template <u_int D> inline float
BilateralIIRFilter<D>::operator [](int i) const
{
    return _bufF[i] + _bufB[i];
}

/************************************************************************
*  class BilateralIIRFilter2						*
************************************************************************/
//! 2$B<!85N>B&(BInfinite Inpulse Response Filter$B$rI=$9%/%i%9(B
template <u_int D> class BilateralIIRFilter2
{
  public:
    typedef typename BilateralIIRFilter<D>::Order	Order;
    
    BilateralIIRFilter2&
		initialize(float cHF[D+D], float cHB[D+D],
			   float cVF[D+D], float cVB[D+D])		;
    BilateralIIRFilter2&
		initialize(float cHF[D+D], Order orderH,
			   float cVF[D+D], Order orderV)		;
    template <class T1, class B1, class T2, class B2> BilateralIIRFilter2&
		convolve(const Image<T1, B1>& in, Image<T2, B2>& out)	;
    
  private:
    BilateralIIRFilter<D>	_iirH;
    BilateralIIRFilter<D>	_iirV;
    Array2<Array<float> >	_buf;
};
    
//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param cHF	$B2#J}8~A0?J(Bz$BJQ4978?t(B
  \param cHB	$B2#J}8~8eB`(Bz$BJQ4978?t(B
  \param cHV	$B=DJ}8~A0?J(Bz$BJQ4978?t(B
  \param cHV	$B=DJ}8~8eB`(Bz$BJQ4978?t(B
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> inline BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::initialize(float cHF[D+D], float cHB[D+D],
				   float cVF[D+D], float cVB[D+D])
{
    _iirH.initialize(cHF, cHB);
    _iirV.initialize(cVF, cVB);

    return *this;
}

//! $B%U%#%k%?$N(Bz$BJQ4978?t$r%;%C%H$9$k(B
/*!
  \param cHF	$B2#J}8~A0?J(Bz$BJQ4978?t(B
  \param orderH $B2#J}8~HyJ,3,?t(B
  \param cHV	$B=DJ}8~A0?J(Bz$BJQ4978?t(B
  \param orderV	$B=DJ}8~HyJ,3,?t(B
  \return	$B$3$N%U%#%k%?<+?H(B
*/
template <u_int D> inline BilateralIIRFilter2<D>&
BilateralIIRFilter2<D>::initialize(float cHF[D+D], Order orderH,
				   float cVF[D+D], Order orderV)
{
    _iirH.initialize(cHF, orderH);
    _iirV.initialize(cVF, orderV);

    return *this;
}

/************************************************************************
*  class DericheConvoler						*
************************************************************************/
//! Canny-Deriche$B3K$K$h$k2hA|>v$_9~$_$r9T$&%/%i%9(B
class DericheConvolver : private BilateralIIRFilter2<2u>
{
  public:
    using	BilateralIIRFilter2<2u>::Order;
    
    DericheConvolver(float alpha=1.0)		{initialize(alpha);}

    DericheConvolver&	initialize(float alpha)				;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	smooth(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffH(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffV(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffHH(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffHV(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	diffVV(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> DericheConvolver&
	laplacian(const Image<T1, B1>& in, Image<T2, B2>& out)		;

  private:
    float		_c0[4];	// forward coefficients for smoothing
    float		_c1[4];	// forward coefficients for 1st derivatives
    float		_c2[4];	// forward coefficients for 2nd derivatives
    Image<float>	_tmp;	// buffer for storing intermediate values
};

//! Canny-Deriche$B3K$K$h$k%9%`!<%7%s%0(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::smooth(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k2#J}8~(B1$B3,HyJ,(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffH(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c1, BilateralIIRFilter<2u>::First,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k=DJ}8~(B1$B3,HyJ,(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffV(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c1, BilateralIIRFilter<2u>::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k2#J}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffHH(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c2, BilateralIIRFilter<2u>::Second,
		   _c0, BilateralIIRFilter<2u>::Zeroth).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k=D2#N>J}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffHV(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c1, BilateralIIRFilter<2u>::First,
		   _c1, BilateralIIRFilter<2u>::First).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k=DJ}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::diffVV(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<2u>::
	initialize(_c0, BilateralIIRFilter<2u>::Zeroth,
		   _c2, BilateralIIRFilter<2u>::Second).convolve(in, out);

    return *this;
}

//! Canny-Deriche$B3K$K$h$k%i%W%i%7%"%s(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BCanny-Deriche$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline DericheConvolver&
DericheConvolver::laplacian(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    diffHH(in, _tmp).diffVV(in, out);
    out += _tmp;
    
    return *this;
}

/************************************************************************
*  class GaussianConvoler						*
************************************************************************/
//! Gauss$B3K$K$h$k2hA|>v$_9~$_$r9T$&%/%i%9(B
class GaussianConvolver : private BilateralIIRFilter2<4u>
{
  private:
    struct Params
    {
	void		set(double aa, double bb, double tt, double aaa);
	Params&		operator -=(const Vector<double>& p)		;
    
	double		a, b, theta, alpha;
    };

    class EvenConstraint
    {
      public:
	typedef double		ET;
	typedef Array<Params>	AT;

	EvenConstraint(ET sigma) :_sigma(sigma)				{}
	
	Vector<ET>	operator ()(const AT& params)		const	;
	Matrix<ET>	jacobian(const AT& params)		const	;

      private:
	ET		_sigma;
    };

    class CostFunction
    {
      public:
	typedef double		ET;
	typedef Array<Params>	AT;
    
	enum			{D = 2};

	CostFunction(int ndivisions, ET range)
	    :_ndivisions(ndivisions), _range(range)			{}
    
	Vector<ET>	operator ()(const AT& params)		 const	;
	Matrix<ET>	jacobian(const AT& params)		 const	;
	void		update(AT& params, const Vector<ET>& dp) const	;

      private:
	const int	_ndivisions;
	const ET	_range;
    };

  public:
    GaussianConvolver(float sigma=1.0)		{initialize(sigma);}

    GaussianConvolver&	initialize(float sigma)				;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	smooth(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffH(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffV(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffHH(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffHV(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	diffVV(const Image<T1, B1>& in, Image<T2, B2>& out)		;
    template <class T1, class B1, class T2, class B2> GaussianConvolver&
	laplacian(const Image<T1, B1>& in, Image<T2, B2>& out)		;

  private:
    float		_c0[8];	// forward coefficients for smoothing
    float		_c1[8];	// forward coefficients for 1st derivatives
    float		_c2[8];	// forward coefficients for 2nd derivatives
    Image<float>	_tmp;	// buffer for storing intermediate values
};

//! Gauss$B3K$K$h$k%9%`!<%7%s%0(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::smooth(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k2#J}8~(B1$B3,HyJ,(B(DOG)
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffH(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c1, BilateralIIRFilter<4u>::First,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k=DJ}8~(B1$B3,HyJ,(B(DOG)
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffV(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c1, BilateralIIRFilter<4u>::First).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k2#J}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffHH(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c2, BilateralIIRFilter<4u>::Second,
		   _c0, BilateralIIRFilter<4u>::Zeroth).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k=D2#N>J}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffHV(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c1, BilateralIIRFilter<4u>::First,
		   _c1, BilateralIIRFilter<4u>::First).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k=DJ}8~(B2$B3,HyJ,(B
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::diffVV(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    BilateralIIRFilter2<4u>::
	initialize(_c0, BilateralIIRFilter<4u>::Zeroth,
		   _c2, BilateralIIRFilter<4u>::Second).convolve(in, out);

    return *this;
}

//! Gauss$B3K$K$h$k%i%W%i%7%"%s(B(LOG)
/*!
  \param in	$BF~NO2hA|(B
  \param out	$B=PNO2hA|(B
  \return	$B$3$N(BGauss$B3K<+?H(B
*/
template <class T1, class B1, class T2, class B2> inline GaussianConvolver&
GaussianConvolver::laplacian(const Image<T1, B1>& in, Image<T2, B2>& out)
{
    diffHH(in, _tmp).diffVV(in, out);
    out += _tmp;
    
    return *this;
}

/************************************************************************
*  class EdgeDetector							*
************************************************************************/
//! $B%(%C%88!=P$r9T$&%/%i%9(B
class EdgeDetector
{
  public:
    enum
    {
	TRACED	= 0x04,
	EDGE	= 0x02,					//!< $B6/$$%(%C%8E@(B
	WEAK	= 0x01					//!< $B<e$$%(%C%8E@(B
    };
    
    EdgeDetector(float th_low=2.0, float th_high=5.0)			;
    
    EdgeDetector&	initialize(float th_low, float th_high)		;
    const EdgeDetector&
	strength(const Image<float>& edgeH,
		 const Image<float>& edgeV, Image<float>& out)	  const	;
    const EdgeDetector&
	direction4(const Image<float>& edgeH,
		   const Image<float>& edgeV, Image<u_char>& out) const	;
    const EdgeDetector&
	direction8(const Image<float>& edgeH,
		   const Image<float>& edgeV, Image<u_char>& out) const	;
    const EdgeDetector&
	suppressNonmaxima(const Image<float>& strength,
			  const Image<u_char>& direction,
			  Image<u_char>& out)			  const	;
    const EdgeDetector&
	zeroCrossing(const Image<float>& in, Image<u_char>& out)  const	;
    const EdgeDetector&
	zeroCrossing(const Image<float>& in,
		     const Image<float>& strength,
		     Image<u_char>& out)			  const	;
    const EdgeDetector&
	hysteresisThresholding(Image<u_char>& edge)		  const	;

  private:
    float		_th_low, _th_high;
};

//! $B%(%C%88!=P4o$r@8@.$9$k(B
/*!
  \param th_low		$B<e$$%(%C%8$NogCM(B
  \param th_low		$B6/$$%(%C%8$NogCM(B
*/
inline
EdgeDetector::EdgeDetector(float th_low, float th_high)
{
    initialize(th_low, th_high);
}

//! $B%(%C%88!=P$NogCM$r@_Dj$9$k(B
/*!
  \param th_low		$B<e$$%(%C%8$NogCM(B
  \param th_low		$B6/$$%(%C%8$NogCM(B
  \return		$B$3$N%(%C%88!=P4o<+?H(B
*/
inline EdgeDetector&
EdgeDetector::initialize(float th_low, float th_high)
{
    _th_low  = th_low;
    _th_high = th_high;

    return *this;
}

/************************************************************************
*  class IntegralImage<T>						*
************************************************************************/
//! $B@QJ,2hA|(B(integral image)$B$rI=$9%/%i%9(B
template <class T>
class IntegralImage : public Image<T>
{
  public:
    IntegralImage()							;
    template <class S, class B>
    IntegralImage(const Image<S, B>& image)				;

    template <class S, class B> IntegralImage&
		initialize(const Image<S, B>& image)			;
    T		crop(int u, int v, int w, int h)		const	;
    T		crossVal(int u, int v, int cropSize)		const	;
    template <class S, class B> const IntegralImage&
		crossVal(Image<S, B>& out, int cropSize)	const	;

    using	Image<T>::width;
    using	Image<T>::height;
};

//! $B6u$N@QJ,2hA|$r:n$k(B
template <class T> inline
IntegralImage<T>::IntegralImage()
{
}
    
//! $BM?$($i$l$?2hA|$+$i@QJ,2hA|$r:n$k(B
/*!
  \param image		$BF~NO2hA|(B
*/
template <class T> template <class S, class B> inline
IntegralImage<T>::IntegralImage(const Image<S, B>& image)
{
    initialize(image);
}
    
//! $B862hA|$K@5J}7A$NFsCM==;z%F%s%W%l!<%H$rE,MQ$7$?CM$rJV$9(B
/*!
  \param u		$B%F%s%W%l!<%HCf?4$N2#:BI8(B
  \param v		$B%F%s%W%l!<%HCf?4$N=D:BI8(B
  \param cropSize	$B%F%s%W%l!<%H$O0lJU(B 2*cropSize + 1 $B$N@5J}7A(B
  \return		$B%F%s%W%l!<%H$rE,MQ$7$?CM(B
*/
template <class T> inline T
IntegralImage<T>::crossVal(int u, int v, int cropSize) const
{
    return crop(u+1,	    v+1,	cropSize, cropSize)
	 - crop(u-cropSize, v+1,	cropSize, cropSize)
	 + crop(u-cropSize, v-cropSize, cropSize, cropSize)
	 - crop(u+1,	    v-cropSize, cropSize, cropSize);
}
    
/************************************************************************
*  class DiagonalIntegralImage						*
************************************************************************/
//! $BBP3Q@QJ,2hA|(B(diagonal integral image)$B$rI=$9%/%i%9(B
template <class T>
class DiagonalIntegralImage : public Image<T>
{
  public:
    DiagonalIntegralImage()						;
    template <class S, class B>
    DiagonalIntegralImage(const Image<S, B>& image)			;

    template <class S, class B> DiagonalIntegralImage&
		initialize(const Image<S, B>& image)			;
    T		crop(int u, int v, int w, int h)		const	;
    T		crossVal(int u, int v, int cropSize)		const	;
    template <class S, class B> const DiagonalIntegralImage&
		crossVal(Image<S, B>& out, int cropSize)	const	;

    using	Image<T>::width;
    using	Image<T>::height;

  private:
    void	correct(int& u, int& v)				const	;
};

//! $B6u$NBP3Q@QJ,2hA|$r:n$k(B
template <class T> inline
DiagonalIntegralImage<T>::DiagonalIntegralImage()
{
}
    
//! $BM?$($i$l$?2hA|$+$iBP3Q@QJ,2hA|$r:n$k(B
/*!
  \param image		$BF~NO2hA|(B
*/
template <class T> template <class S, class B> inline
DiagonalIntegralImage<T>::DiagonalIntegralImage(const Image<S, B>& image)
{
    initialize(image);
}
    
//! $B862hA|$K@5J}7A$NFsCM%/%m%9%F%s%W%l!<%H$rE,MQ$7$?CM$rJV$9(B
/*!
  \param u		$B%F%s%W%l!<%HCf?4$N2#:BI8(B
  \param v		$B%F%s%W%l!<%HCf?4$N=D:BI8(B
  \param cropSize	$B%F%s%W%l!<%H$O0lJU(B 2*cropSize + 1 $B$N@5J}7A(B
  \return		$B%F%s%W%l!<%H$rE,MQ$7$?CM(B
*/
template <class T> inline T
DiagonalIntegralImage<T>::crossVal(int u, int v, int cropSize) const
{
    return crop(u+cropSize+1, v-cropSize+1, cropSize, cropSize)
	 - crop(u,	      v+2,	    cropSize, cropSize)
	 + crop(u-cropSize-1, v-cropSize+1, cropSize, cropSize)
	 - crop(u,	      v-2*cropSize, cropSize, cropSize);
}

template <class T> inline void
DiagonalIntegralImage<T>::correct(int& u, int& v) const
{
    if (u < 0)
    {
	v += u;
	u  = 0;
    }
    else if (u >= width())
    {
	v += (int(width()) - 1 - u);
	u  = width() - 1;
    }
}
    
}

#endif	/* !__TUImagePP_h */
