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
 *  $Id: Image++.h,v 1.39 2008-09-10 05:10:38 ueshiba Exp $
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
	:P(), d1(0), d2(0)		{P[0][0] = P[1][1] = P[2][2] = 1.0;}
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
    template <class T2, class S>
    T2			at(S uf)		const	;
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

template <class T> template <class T2, class S> inline T2
ImageLine<T>::at(S uf) const
{
    const int	u   = floor(uf);
    const S	du  = uf - u;
    const T*	in = &(*this)[u];
    return T2(*in) + du*(T2(*(in + 1)) - T2(*in));
}

template <class T> const YUV422*
ImageLine<T>::fill(const YUV422* src)
{
    register T* dst = *this;
    for (register u_int u = 0; u < dim(); u += 2)
    {
	*dst++ = fromYUV<T>(src[0].y, src[0].x, src[1].x);
	*dst++ = fromYUV<T>(src[1].y, src[0].x, src[1].x);
	src += 2;
    }
    return src;
}

template <class T> const YUV411*
ImageLine<T>::fill(const YUV411* src)
{
    register T*  dst = *this;
    for (register u_int u = 0; u < dim(); u += 4)
    {
	*dst++ = fromYUV<T>(src[0].y0, src[0].x, src[1].x);
	*dst++ = fromYUV<T>(src[0].y1, src[0].x, src[1].x);
	*dst++ = fromYUV<T>(src[1].y0, src[0].x, src[1].x);
	*dst++ = fromYUV<T>(src[1].y1, src[0].x, src[1].x);
	src += 2;
    }
    return src;
}

template <class T> template <class S> const S*
ImageLine<T>::fill(const S* src)
{
    T* dst = *this;
    for (int n = dim() + 1; --n; )
	*dst++ = T(*src++);
    return src;
}

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

template <class S> const S*
ImageLine<YUV422>::fill(const S* src)
{
    YUV422* dst = *this;
    for (int n = dim() + 1; --n; )
	*dst++ = YUV422(*src++);
    return src;
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

template <class S> const S*
ImageLine<YUV411>::fill(const S* src)
{
    YUV411* dst = *this;
    for (int n = dim() + 1; --n; )
	*dst++ = YUV411(*src++);
    return src;
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

    const Image<T>	operator ()(int u, int v,
				    u_int w, u_int h)	const	;
    Image<T>		operator ()(int u, int v,
				    u_int w, u_int h)		;
    
    template <class T2, class S>
    T2		at(const Point2<S>& p)			const	;
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

template <class T, class B> inline const Image<T>
Image<T, B>::operator ()(int u, int v, u_int w, u_int h) const
{
    return Image<T>(*this, u, v, w, h);
}
    
template <class T, class B> inline Image<T>
Image<T, B>::operator ()(int u, int v, u_int w, u_int h)
{
    return Image<T>(*this, u, v, w, h);
}
    
template <class T, class B> template <class T2, class S> inline T2
Image<T, B>::at(const Point2<S>& p) const
{
    const int	v  = floor(p[1]);
    const S	dv = p[1] - v;
    const T2	out0 = (*this)[v  ].at<T2, S>(p[0]),
		out1 = (*this)[v+1].at<T2, S>(p[0]);
    return out0 + dv*(out1 - out0);
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

template <class T, class B> std::istream&
Image<T, B>::restoreData(std::istream& in, Type type)
{
    switch (type)
    {
      case U_CHAR:
	return restoreRows<u_char>(in);
      case SHORT:
	return restoreRows<short>(in);
      case INT:
	return restoreRows<int>(in);
      case FLOAT:
	return restoreRows<float>(in);
      case DOUBLE:
	return restoreRows<double>(in);
      case RGB_24:
	return restoreRows<RGB>(in);
      case YUV_444:
	return restoreRows<YUV444>(in);
      case YUV_422:
	return restoreRows<YUV422>(in);
      case YUV_411:
	return restoreRows<YUV411>(in);
    }
    return in;
}

template <class T, class B> std::ostream&
Image<T, B>::saveData(std::ostream& out, Type type) const
{
    switch (type)
    {
      case U_CHAR:
	return saveRows<u_char>(out);
      case SHORT:
	return saveRows<short>(out);
      case INT:
	return saveRows<int>(out);
      case FLOAT:
	return saveRows<float>(out);
      case DOUBLE:
	return saveRows<double>(out);
      case RGB_24:
	return saveRows<RGB>(out);
      case YUV_444:
	return saveRows<YUV444>(out);
      case YUV_422:
	return saveRows<YUV422>(out);
      case YUV_411:
	return saveRows<YUV411>(out);
    }
    return out;
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
 
template <class T, class B> template <class S> std::istream&
Image<T, B>::restoreRows(std::istream& in)
{
    ImageLine<S>	buf(width());
    for (int v = 0; v < height(); )
    {
	if (!buf.restore(in))
	    break;
	(*this)[v++].fill((S*)buf);
    }
    return in;
}

template <class T, class B> template <class D> std::ostream&
Image<T, B>::saveRows(std::ostream& out) const
{
    ImageLine<D>	buf(width());
    for (int v = 0; v < height(); )
    {
	buf.fill((const T*)(*this)[v++]);
	if (!buf.save(out))
	    break;
    }
    return out;
}

template <class T, class B> u_int
Image<T, B>::_width() const
{
    return Image<T, B>::width();	// Don't call ImageBase::width!
}

template <class T, class B> u_int
Image<T, B>::_height() const
{
    return Image<T, B>::height();	// Don't call ImageBase::height!
}

template <class T, class B> void
Image<T, B>::_resize(u_int h, u_int w, Type)
{
    Image<T, B>::resize(h, w);		// Don't call ImageBase::resize!
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
*  class GenericImage							*
************************************************************************/
//! $B2hAG$N7?$rLd$o$J$$Am>N2hA|%/%i%9(B
/*!
  $B8D!9$N9T$d2hAG$K%"%/%;%9$9$k$3$H$O$G$-$J$$!%(B
*/
class GenericImage : public Array2<Array<u_char> >, public ImageBase
{
  public:
    GenericImage() :_type(U_CHAR)					{}
    
    std::istream&	restore(std::istream& in)			;
    std::ostream&	save(std::ostream& out)			const	;
    std::istream&	restoreData(std::istream& in)			;
    std::ostream&	saveData(std::ostream& out)		const	;
    
  private:
    virtual u_int	_width()				const	;
    virtual u_int	_height()				const	;
    virtual void	_resize(u_int h, u_int w,
				ImageBase::Type type)			;

    ImageBase::Type	_type;
};

inline std::istream&
GenericImage::restore(std::istream& in)
{
    _type = restoreHeader(in);
    return restoreData(in);
}

inline std::ostream&
GenericImage::save(std::ostream& out) const
{
    saveHeader(out, _type);
    return saveData(out);
}

}

#endif	/* !__TUImagePP_h */
