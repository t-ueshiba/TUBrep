/*
 *  ����14-19�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
 *  
 *  �n��ҁF�A�ŏr�v
 *
 *  �{�v���O�����́i�Ɓj�Y�ƋZ�p�����������̐E���ł���A�ŏr�v���n�삵�C
 *  �i�Ɓj�Y�ƋZ�p���������������쌠�����L����閧���ł��D���쌠���L
 *  �҂ɂ�鋖�Ȃ��ɖ{�v���O�������g�p�C�����C���ρC��O�҂֊J������
 *  ���̍s�ׂ��֎~���܂��D
 *  
 *  ���̃v���O�����ɂ���Đ����邢���Ȃ鑹�Q�ɑ΂��Ă��C���쌠���L�҂�
 *  ��ёn��҂͐ӔC�𕉂��܂���B
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
 *  $Id: Image++.h,v 1.56 2010-07-26 08:18:37 ueshiba Exp $
 */
#ifndef	__TUImagePP_h
#define	__TUImagePP_h

#include <string.h>
#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  struct RGB, BGR, RGBA, ABGR, ARGB & BGRA				*
************************************************************************/
struct BGR;
struct RGBA;
struct ABGR;
struct ARGB;
struct BGRA;
struct YUV444;

//! Red, Green, Blue�i�e8bit�j�̏��ŕ��񂾃J���[��f
struct RGB
{
    RGB()					:r(0),  g(0),  b(0)	{}
    RGB(u_char rr, u_char gg, u_char bb)	:r(rr), g(gg), b(bb)	{}
    RGB(const BGR& p)							;
    RGB(const RGBA& p)							;
    RGB(const ABGR& p)							;
    RGB(const ARGB& p)							;
    RGB(const BGRA& p)							;
    RGB(const YUV444& p)						;
    template <class T>
    RGB(const T& p)	:r(u_char(p)), g(u_char(p)), b(u_char(p))	{}

		operator u_char()	const	{return u_char(double(*this));}
		operator s_char()	const	{return s_char(double(*this));}
		operator short()	const	{return short(double(*this));}
		operator int()		const	{return int(double(*this));}
		operator float()	const	{return float(double(*this));}
		operator double()	const	{return 0.3*r+0.59*g+0.11*b;}
    
    RGB&	operator +=(const RGB& p)	{r += p.r; g += p.g; b += p.b;
						 return *this;}
    RGB&	operator -=(const RGB& p)	{r -= p.r; g -= p.g; b -= p.b;
						 return *this;}
    template <class T>
    RGB&	operator *=(T c)		{r *= c; g *= c; b *= c;
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

//! Blue, Green, Red�i�e8bit�j�̏��ŕ��񂾃J���[��f
struct BGR
{
    BGR()					:b(0),   g(0),   r(0)	{}
    BGR(u_char rr, u_char gg, u_char bb)	:b(bb),  g(gg),  r(rr)	{}
    BGR(const RGB& p)				:b(p.b), g(p.g), r(p.r)	{}
    BGR(const RGBA& p)							;
    BGR(const ABGR& p)							;
    BGR(const ARGB& p)							;
    BGR(const BGRA& p)							;
    BGR(const YUV444& p)						;
    template <class T>
    BGR(const T& c)	:b(u_char(c)), g(u_char(c)), r(u_char(c))	{}

		operator u_char()	const	{return u_char(double(*this));}
		operator s_char()	const	{return s_char(double(*this));}
		operator short()	const	{return short(double(*this));}
		operator int()		const	{return int(double(*this));}
		operator float()	const	{return float(double(*this));}
		operator double()	const	{return 0.3*r+0.59*g+0.11*b;}

    BGR&	operator +=(const BGR& p)	{r += p.r; g += p.g; b += p.b;
						 return *this;}
    BGR&	operator -=(const BGR& p)	{r -= p.r; g -= p.g; b -= p.b;
						 return *this;}
    template <class T>
    BGR&	operator *=(T c)		{r *= c; g *= c; b *= c;
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

//! �J���[��f��alpha�`�����l��
struct Alpha
{
    Alpha(u_char aa=255)	:a(aa)			{}

    int		operator ==(const Alpha& p) const	{return a == p.a;}
    int		operator !=(const Alpha& p) const	{return !(*this == p);}
    
    u_char	a;
};

//! Red, Green, Blue, Alpha�i�e8bit�j�̏��ŕ��񂾃J���[��f
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

//! Alpha, Blue, Green, Red�i�e8bit�j�̏��ŕ��񂾃J���[��f
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

//! Alpha, Red, Green, Blue�i�e8bit�j�̏��ŕ��񂾃J���[��f
struct ARGB : public Alpha, public RGB
{
    ARGB()		:Alpha(),  RGB()		{}
    ARGB(u_char r, u_char g, u_char b, u_char a=255)
			:Alpha(a), RGB(r, g, b)  	{}
    template <class T>
    ARGB(const T& p)	:Alpha(),  RGB(p)		{}

    bool	operator ==(const ARGB& p) const
		{return (Alpha::operator ==(p) && RGB::operator ==(p));}
    bool	operator !=(const ARGB& p) const	{return !(*this != p);}
};

//! Blue, Green, Red, Alpha�i�e8bit�j�̏��ŕ��񂾃J���[��f
struct BGRA : public BGR, public Alpha
{
    BGRA()		:BGR(), Alpha()  		{}
    BGRA(u_char r, u_char g, u_char b, u_char a=255)
			:BGR(r, g, b), Alpha(a)   	{}
    template <class T>
    BGRA(const T& p)	:BGR(p), Alpha()		{}

    bool	operator ==(const BGRA& p) const
		{return (Alpha::operator ==(p) && BGR::operator ==(p));}
    bool	operator !=(const BGRA& p) const	{return !(*this != p);}
};

inline
RGB::RGB(const RGBA& p)	:r(p.r), g(p.g), b(p.b)	{}

inline
RGB::RGB(const ABGR& p)	:r(p.r), g(p.g), b(p.b)	{}

inline
RGB::RGB(const BGRA& p)	:r(p.r), g(p.g), b(p.b)	{}

inline
BGR::BGR(const RGBA& p)	:r(p.r), g(p.g), b(p.b)	{}

inline
BGR::BGR(const ABGR& p)	:r(p.r), g(p.g), b(p.b)	{}

inline
BGR::BGR(const BGRA& p)	:r(p.r), g(p.g), b(p.b)	{}

/************************************************************************
*  struct YUV444, YUV422, YUV411					*
************************************************************************/
//! Y, U, V�i�e8bit�j�̏��ŕ��񂾃J���[��f
struct YUV444
{
    YUV444(u_char yy=0, u_char uu=128, u_char vv=128)
			:u(uu), y(yy), v(vv)		{}
    template <class T> 
    YUV444(const T& p)	:u(128), y(u_char(p)), v(128)	{}

		operator u_char()		const	{return u_char(y);}
		operator s_char()		const	{return s_char(y);}
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

//! [U, Y0], [V, Y1]�i�e8bit�j�̏��ŕ��񂾃J���[��f(16bits/pixel)
struct YUV422
{
    YUV422(u_char yy=0, u_char xx=128)	:x(xx), y(yy)	{}
    template <class T>
    YUV422(const T& p)		:x(128), y(u_char(p))	{}

		operator u_char()		const	{return u_char(y);}
		operator s_char()		const	{return s_char(y);}
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

//! [U, Y0, Y1], [V, Y2, Y3]�i�e8bit�j�̏��ŕ��񂾃J���[��f(12bits/pixel)
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
//! �J���[��Y, U, V�l��^���đ��̃J���[�\���ɕϊ�����N���X
class __PORT ConversionFromYUV
{
  public:
    ConversionFromYUV()					;

  private:
    template <class T>
    friend T	fromYUV(u_char y, u_char u, u_char v)	;
    
    int		_r[256], _g0[256], _g1[256], _b[256];
};

__PORT extern const ConversionFromYUV	conversionFromYUV;

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

template <> inline s_char
fromYUV<s_char>(u_char y, u_char, u_char)
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
//! ��f��2�����z��Ƃ��Ē�`���ꂽ������摜�̊��ƂȂ�N���X
class __PORT ImageBase
{
  public:
  //! �O���L���ɓǂݏ�������ۂ̉�f�̃^�C�v
    enum Type
    {
	DEFAULT = 0,	//!< same as internal type
	U_CHAR	= 5,	//!< unsigned mono	 8bit/pixel
	RGB_24	= 6,	//!< RGB		24bit/pixel	
	SHORT,		//!< signed mono	16bit/pixel
	INT,		//!< signed mono	32bit/pixel	
	FLOAT,		//!< float mono		32bit/pixel 
	DOUBLE,		//!< double mono	64bit/pixel
	YUV_444,	//!< YUV444		24bit/pixel
	YUV_422,	//!< YUV422		16bit/pixel
	YUV_411,	//!< YUV411		12bit/pixel
	BMP_8,		//!< BMP indexed color   8bit/pixel
	BMP_24,		//!< BMP BGR		24bit/pixel
	BMP_32		//!< BMP BGRA		32bit/pixel
    };

  //! �O���L���ɓǂݏ�������ۂ̕t�����
    struct TypeInfo
    {
	__PORT	TypeInfo(Type ty=DEFAULT)	;

	Type	type;		//!< ��f�̌^ #Type 
	bool	bottomToTop;	//!< �s���������֎��߂��Ă���Ȃ�true
	u_int	ncolors;	//!< �J���[�p���b�g�̐F��
    };
    
  protected:
  //! �摜�𐶐������e�s��ƕ��˘c�ȌW��������������D
  /*!
    ���e�s���
    \f$\TUbeginarray{cc} \TUvec{I}{3\times 3} & \TUvec{0}{} \TUendarray\f$�ɁC
    2�̕��˘c�ȌW���͂������0�ɏ����������D
  */
    ImageBase()
	:P(), d1(0), d2(0)		{P[0][0] = P[1][1] = P[2][2] = 1.0;}
    virtual ~ImageBase()		;

    u_int		type2nbytes(Type type, bool padding)	const	;
    static u_int	type2depth(Type type)				;
    
  public:
    TypeInfo		restoreHeader(std::istream& in)			;
    Type		saveHeader(std::ostream& out,
				   Type type=DEFAULT)		const	;

  //! �摜�̕���Ԃ��D
  /*!
    \return	�摜�̕�
  */
    u_int		width()			const	{return _width();}

  //! �摜�̍�����Ԃ��D
  /*!
    \return	�摜�̍���
  */
    u_int		height()		const	{return _height();}

  //! �摜�̃T�C�Y��ύX����D
  /*!
    \param h	�V������
    \param w	�V��������
  */
    void		resize(u_int h, u_int w)	{_resize(h, w,
								 DEFAULT);}
	
  private:
    TypeInfo		restorePBMHeader(std::istream& in)	;
    TypeInfo		restoreBMPHeader(std::istream& in)	;
    Type		savePBMHeader(std::ostream& out,
				      Type type)	const	;
    Type		saveBMPHeader(std::ostream& out,
				      Type type)	const	;
    virtual u_int	_width()			const	= 0;
    virtual u_int	_height()			const	= 0;
    virtual Type	_defaultType()			const	= 0;
    virtual void	_resize(u_int h, u_int w,
				const TypeInfo& typeInfo)	= 0;

  public:
    Matrix34d		P;			//!< �J������3x4���e�s��
    double		d1;			//!< ���˘c�Ȃ̑�1�W��
    double		d2;			//!< ���˘c�Ȃ̑�2�W��
};

/************************************************************************
*  class ImageLine<T>:	Generic image scanline class			*
************************************************************************/
//! T�^�̉�f�����摜�̃X�L�������C����\���N���X
/*!
  \param T	��f�̌^
*/
template <class T>
class ImageLine : public Array<T>
{
  public:
  //! �w�肵����f���̃X�L�������C���𐶐�����D
  /*!
    \param d	��f��
  */
    explicit ImageLine(u_int d=0)
        :Array<T>(d), _lmost(0), _rmost(d)			{*this = 0;}

  //! �O���̗̈�Ɖ�f�����w�肵�ăX�L�������C���𐶐�����D
  /*!
    \param p	�O���̈�ւ̃|�C���^
    \param d	��f��
  */
    ImageLine(T* p, u_int d)
        :Array<T>(p, d), _lmost(0), _rmost(d)			{}

  //! �w�肳�ꂽ�X�L�������C���̕����X�L�������C���𐶐�����D
  /*!
    \param i	���̃X�L�������C��
    \param u	�����X�L�������C���̍��[�̍��W
    \param d	�����X�L�������C���̉�f��
  */
    ImageLine(const ImageLine<T>& l, u_int u, u_int d)
	:Array<T>(l, u, d), _lmost(0), _rmost(d)		{}

    const ImageLine	operator ()(u_int u, u_int d)	const	;
    ImageLine		operator ()(u_int u, u_int d)		;
    
  //! �S�Ẳ�f�ɓ���̒l��������D
  /*!
    \param c	��������f�l
    \return	���̃X�L�������C��
  */
    ImageLine&		operator =(const T& c)
			{
			    Array<T>::operator =(c);
			    return *this;
			}

    using		Array<T>::dim;
    template <class S>
    T			at(S uf)		const	;
    const YUV422*	fill(const YUV422* src)		;
    const YUV411*	fill(const YUV411* src)		;
    const T*		fill(const T* src)		;
    template <class S>
    const S*		fill(const S* src)		;
    template <class S, class L>
    const S*		lookup(const S* src,
			       const L* tbl)		;

  //! ���[�̗L����f�̈ʒu��Ԃ��D
  /*!
    \return	���[�̗L����f�̈ʒu
  */
    int			lmost()			const	{return _lmost;}

  //! �E�[�̗L����f�̎��̈ʒu��Ԃ��D
  /*!
    \return	�E�[�̗L����f�̎��̈ʒu
  */
    int			rmost()			const	{return _rmost;}

  //! �L����f�͈̔͂�ݒ肷��D
  /*!
    \param l	�L����f�̍��[
    \param r	�L����f�̉E�[�̎�
  */
    void		setLimits(int l, int r)		{_lmost = l;
							 _rmost = r;}
  //! �w�肳�ꂽ�ʒu�̉�f���L�������肷��D
  /*!
    \param u	��f�̈ʒu
    \return	�L���Ȃ��true�C�����Ȃ��false
  */
    bool		valid(int u)		const	{return (u >= _lmost &&
								 u <  _rmost);}
	
    bool		resize(u_int d)			;
    void		resize(T* p, u_int d)		;

  private:
    int			_lmost;
    int			_rmost;
};

//! ���̃X�L�������C���̕����X�L�������C���𐶐�����D
/*!
  \param u	�����X�L�������C���̍��[�̍��W
  \param d	�����X�L�������C���̉�f��
  \return	�������ꂽ�����X�L�������C��
*/
template <class T> inline const ImageLine<T>
ImageLine<T>::operator ()(u_int u, u_int d) const
{
    return ImageLine<T>(*this, u, d);
}

//! ���̃X�L�������C���̕����X�L�������C���𐶐�����D
/*!
  \param u	�����X�L�������C���̍��[�̍��W
  \param d	�����X�L�������C���̉�f��
  \return	�������ꂽ�����X�L�������C��
*/
template <class T> inline ImageLine<T>
ImageLine<T>::operator ()(u_int u, u_int d)
{
    return ImageLine<T>(*this, u, d);
}
    
//! �T�u�s�N�Z���ʒu�̉�f�l����`��Ԃŋ��߂�D
/*!
  �w�肳�ꂽ�ʒu�̗����̉�f�l����`��Ԃ��ďo�͂���D
  \param uf	�T�u�s�N�Z���Ŏw�肳�ꂽ�ʒu
  \return	���`��Ԃ��ꂽ��f�l
*/
template <class T> template <class S> inline T
ImageLine<T>::at(S uf) const
{
    const int	u  = floor(uf);
    const T*	in = &(*this)[u];
    const S	du = uf - u;
    return (du ? (1 - du) * *in + du * *(in + 1) : *in);
}

//! �|�C���^�Ŏw�肳�ꂽ�ʒu����X�L�������C���̉�f�����̉�f��ǂݍ��ށD
/*!
  \param src	�ǂݍ��݌��̐擪���w���|�C���^
  \return	�Ō�ɓǂݍ��܂ꂽ��f�̎��̉�f�ւ̃|�C���^
*/
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

//! �|�C���^�Ŏw�肳�ꂽ�ʒu����X�L�������C���̉�f�����̉�f��ǂݍ��ށD
/*!
  \param src	�ǂݍ��݌��̐擪���w���|�C���^
  \return	�Ō�ɓǂݍ��܂ꂽ��f�̎��̉�f�ւ̃|�C���^
*/
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

//! �|�C���^�Ŏw�肳�ꂽ�ʒu����X�L�������C���̉�f�����̉�f��ǂݍ��ށD
/*!
  \param src	�ǂݍ��݌��̐擪���w���|�C���^
  \return	�Ō�ɓǂݍ��܂ꂽ��f�̎��̉�f�ւ̃|�C���^
*/
template <class T> template <class S> const S*
ImageLine<T>::fill(const S* src)
{
    T* dst = *this;
    for (u_int n = dim() + 1; --n; )
	*dst++ = T(*src++);
    return src;
}

//! �C���f�b�N�X��ǂݍ��݁C���b�N�A�b�v�e�[�u���ŕϊ�����D
/*!
  \param src	�ǂݍ��݌��̐擪���w���|�C���^
  \param tbl	���b�N�A�b�v�e�[�u���̐擪���w���|�C���^
  \return	�Ō�ɓǂݍ��܂ꂽ��f�̎��̉�f�ւ̃|�C���^
*/
template <class T> template <class S, class L> const S*
ImageLine<T>::lookup(const S* src, const L* tbl)
{
    T* dst = *this;
    for (u_int n = dim() + 1; --n; )
	*dst++ = T(*(tbl + *src++));
    return src;
}

//! �|�C���^�Ŏw�肳�ꂽ�ʒu����X�L�������C���̉�f�����̉�f��ǂݍ��ށD
/*!
  \param src	�ǂݍ��݌��̐擪���w���|�C���^
  \return	�Ō�ɓǂݍ��܂ꂽ��f�̎��̉�f�ւ̃|�C���^
*/
template <class T> inline const T*
ImageLine<T>::fill(const T* src)
{
    memcpy((T*)*this, src, dim() * sizeof(T));
    return src + dim();
}

//! �X�L�������C���̉�f����ύX����D
/*!
  �������C���̃I�u�W�F�N�g�ƋL���̈�����L���Ă���X�L�������C���̉�f����
  �ύX���邱�Ƃ͂ł��Ȃ��D
  \param d			�V������f��
  \return			d�����̉�f�������傫�����true�C����
				�łȂ����false
  \throw std::logic_error	�L���̈�𑼂̃I�u�W�F�N�g�Ƌ��L���Ă���ꍇ
				�ɑ��o
*/
template <class T> inline bool
ImageLine<T>::resize(u_int d)
{
    _lmost = 0;
    _rmost = d;
    return Array<T>::resize(d);
}

//! �X�L�������C���������Ŏg�p����L���̈���w�肵�����̂ɕύX����D
/*!
  \param p	�V�����L���̈�ւ̃|�C���^
  \param d	�V������f��
*/
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
	:Array<YUV422>(d), _lmost(0), _rmost(d)			{*this = 0;}
    ImageLine(YUV422* p, u_int d)
	:Array<YUV422>(p, d), _lmost(0), _rmost(d)		{}
    ImageLine(const ImageLine<YUV422>& l, u_int u, u_int d)
	:Array<YUV422>(l, u, d), _lmost(0), _rmost(d)		{}
    const ImageLine	operator ()(u_int u, u_int d)	const	;
    ImageLine		operator ()(u_int u, u_int d)		;
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
    template <class S, class L>
    const S*		lookup(const S* src,
			       const L* tbl)		;
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

inline const ImageLine<YUV422>
ImageLine<YUV422>::operator ()(u_int u, u_int d) const
{
    return ImageLine<YUV422>(*this, u, d);
}
    
inline ImageLine<YUV422>
ImageLine<YUV422>::operator ()(u_int u, u_int d)
{
    return ImageLine<YUV422>(*this, u, d);
}
    
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
    for (u_int n = dim() + 1; --n; )
	*dst++ = YUV422(*src++);
    return src;
}

template <class S, class L> const S*
ImageLine<YUV422>::lookup(const S* src, const L* tbl)
{
    YUV422* dst = *this;
    for (u_int n = dim() + 1; --n; )
	*dst++ = YUV422(*(tbl + *src++));
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
	:Array<YUV411>(d), _lmost(0), _rmost(d)			{*this = 0;}
    ImageLine(YUV411* p, u_int d)
	:Array<YUV411>(p, d), _lmost(0), _rmost(d)		{}
    ImageLine(const ImageLine<YUV411>& l, u_int u, u_int d)
	:Array<YUV411>(l, u, d), _lmost(0), _rmost(d)		{}
    const ImageLine	operator ()(u_int u, u_int d)	const	;
    ImageLine		operator ()(u_int u, u_int d)		;
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
    template <class S, class L>
    const S*		lookup(const S* src,
			       const L* tbl)		;
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

inline const ImageLine<YUV411>
ImageLine<YUV411>::operator ()(u_int u, u_int d) const
{
    return ImageLine<YUV411>(*this, u, d);
}
    
inline ImageLine<YUV411>
ImageLine<YUV411>::operator ()(u_int u, u_int d)
{
    return ImageLine<YUV411>(*this, u, d);
}
    
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
    for (u_int n = dim() + 1; --n; )
	*dst++ = YUV411(*src++);
    return src;
}

template <class S, class L> const S*
ImageLine<YUV411>::lookup(const S* src, const L* tbl)
{
    YUV411* dst = *this;
    for (u_int n = dim() + 1; --n; )
	*dst++ = YUV422(*(tbl + *src++));
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

/********st****************************************************************
*  class Image<T>:	Generic image class				*
************************************************************************/
//! T�^�̉�f�����摜��\���N���X
/*!
  \param T	��f�̌^
  \param B	�o�b�t�@�̌^
*/
template <class T, class B=Buf<T> >
class Image : public Array2<ImageLine<T>, B>, public ImageBase
{
  public:
  //! ���ƍ������w�肵�ĉ摜�𐶐�����D
  /*!
    \param w	�摜�̕�
    \param h	�摜�̍���
  */
    explicit Image(u_int w=0, u_int h=0)
	:Array2<ImageLine<T>, B>(h, w), ImageBase()		{*this = 0;}

  //! �O���̗̈�ƕ�����э������w�肵�ĉ摜�𐶐�����D
  /*!
    \param p	�O���̈�ւ̃|�C���^
    \param w	�摜�̕�
    \param h	�摜�̍���
  */
    Image(T* p, u_int w, u_int h)			
	:Array2<ImageLine<T>, B>(p, h, w), ImageBase()		{}

  //! �w�肳�ꂽ�摜�̕����摜�𐶐�����D
  /*!
    \param i	���̉摜
    \param u	�����摜�̍���[�̉����W
    \param v	�����摜�̍���[�̏c���W
    \param w	�����摜�̕�
    \param h	�����摜�̍���
  */
    template <class B2>
    Image(const Image<T, B2>& i, u_int u, u_int v, u_int w, u_int h)
	:Array2<ImageLine<T>, B>(i, v, u, h, w), ImageBase(i)	{}

    const Image<T>	operator ()(u_int u, u_int v,
				    u_int w, u_int h)	const	;
    Image<T>		operator ()(u_int u, u_int v,
				    u_int w, u_int h)		;
    
    template <class S>
    T		at(const Point2<S>& p)			const	;

  //! �w�肳�ꂽ�ʒu�̉�f�ɃA�N�Z�X����D
  /*!
    \param p	��f�̈ʒu
    \return	�w�肳�ꂽ��f
  */
    template <class S>
    const T&	operator ()(const Point2<S>& p)
					const	{return (*this)[p[1]][p[0]];}

  //! �w�肳�ꂽ�ʒu�̉�f�ɃA�N�Z�X����D
  /*!
    \param p	��f�̈ʒu
    \return	�w�肳�ꂽ��f
  */
    template <class S>
    T&		operator ()(const Point2<S>& p)	{return (*this)[p[1]][p[0]];}
    
    u_int	width()			const	{return
						 Array2<ImageLine<T> >::ncol();}
    u_int	height()		const	{return
						 Array2<ImageLine<T> >::nrow();}
    
  //! �S�Ẳ�f�ɓ���̒l��������D
  /*!
    \param c	��������f�l
    \return	���̉摜
  */
    Image&	operator = (const T& c)		{Array2<ImageLine<T> >::
						 operator =(c); return *this;}
    std::istream&	restore(std::istream& in)			;
    std::ostream&	save(std::ostream& out,
			     Type type=DEFAULT)			const	;
    std::istream&	restoreData(std::istream& in,
				    const TypeInfo& typeInfo)		;
    std::ostream&	saveData(std::ostream& out,
				 Type type=DEFAULT)		const	;
    void		resize(u_int h, u_int w)			;
    void		resize(T* p, u_int h, u_int w)			;

  private:
    template <class S>
    std::istream&	restoreRows(std::istream& in,
				    const TypeInfo& typeInfo)		;
    template <class S, class L>
    std::istream&	restoreAndLookupRows(std::istream& in,
					     const TypeInfo& typeInfo)	;
    template <class D, class L>
    std::ostream&	saveRows(std::ostream& out, Type type)	const	;
    Type		defaultType()				const	;
    
    virtual u_int	_width()				const	;
    virtual u_int	_height()				const	;
    virtual Type	_defaultType()				const	;
    virtual void	_resize(u_int h, u_int w, const TypeInfo&)	;
};

//! ���̉摜�̕����摜�𐶐�����D
/*!
  \param u	�����摜�̍���[�̉����W
  \param v	�����摜�̍���[�̏c���W
  \param w	�����摜�̕�
  \param h	�����摜�̍���
  \return	�������ꂽ�����摜
*/
template <class T, class B> inline const Image<T>
Image<T, B>::operator ()(u_int u, u_int v, u_int w, u_int h) const
{
    return Image<T>(*this, u, v, w, h);
}
    
//! ���̉摜�̕����摜�𐶐�����D
/*!
  \param u	�����摜�̍���[�̉����W
  \param v	�����摜�̍���[�̏c���W
  \param w	�����摜�̕�
  \param h	�����摜�̍���
  \return	�������ꂽ�����摜
*/
template <class T, class B> inline Image<T>
Image<T, B>::operator ()(u_int u, u_int v, u_int w, u_int h)
{
    return Image<T>(*this, u, v, w, h);
}
    
//! �T�u�s�N�Z���ʒu�̉�f�l��o���`��Ԃŋ��߂�D
/*!
  �w�肳�ꂽ�ʒu���͂�4�̉�f�l��o���`��Ԃ��ďo�͂���D
  \param p	�T�u�s�N�Z���Ŏw�肳�ꂽ�ʒu
  \return	�o���`��Ԃ��ꂽ��f�l
*/
template <class T, class B> template <class S> inline T
Image<T, B>::at(const Point2<S>& p) const
{
    const int	v  = floor(p[1]);
    const T	out0 = (*this)[v].at(p[0]);
    const S	dv = p[1] - v;
    return (dv ? (1 - dv)*out0 + dv*(*this)[v+1].at(p[0]) : out0);
}

//! ���̓X�g���[������摜��ǂݍ��ށD
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, class B> inline std::istream&
Image<T, B>::restore(std::istream& in)
{
    return restoreData(in, restoreHeader(in));
}

//! �w�肵����f�^�C�v�ŏo�̓X�g���[���ɉ摜�������o���D
/*!
  \param out	�o�̓X�g���[��
  \param type	��f�^�C�v�D�������C#DEFAULT���w�肵���ꍇ�́C
		���̉摜�I�u�W�F�N�g�̉�f�^�C�v�ŏ����o�����D
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, class B> inline std::ostream&
Image<T, B>::save(std::ostream& out, Type type) const
{
    return saveData(out, saveHeader(out, type));
}

//! ���̓X�g���[������摜�̉�f�f�[�^��ǂݍ��ށD
/*!
  \param in	���̓X�g���[��
  \param type	�X�g���[�����̃f�[�^�̉�f�^�C�v(�ǂݍ��ݐ�̉摜�̉�f�^�C�v�ł͂Ȃ�)
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T, class B> std::istream&
Image<T, B>::restoreData(std::istream& in, const TypeInfo& typeInfo)
{
    switch (typeInfo.type)
    {
      case DEFAULT:
	break;
      case U_CHAR:
	return restoreRows<u_char>(in, typeInfo);
      case SHORT:
	return restoreRows<short>(in, typeInfo);
      case INT:
	return restoreRows<int>(in, typeInfo);
      case FLOAT:
	return restoreRows<float>(in, typeInfo);
      case DOUBLE:
	return restoreRows<double>(in, typeInfo);
      case RGB_24:
	return restoreRows<RGB>(in, typeInfo);
      case YUV_444:
	return restoreRows<YUV444>(in, typeInfo);
      case YUV_422:
	return restoreRows<YUV422>(in, typeInfo);
      case YUV_411:
	return restoreRows<YUV411>(in, typeInfo);
      case BMP_8:
	return restoreAndLookupRows<u_char, BGRA>(in, typeInfo);
      case BMP_24:
	return restoreRows<BGR>(in, typeInfo);
      case BMP_32:
	return restoreRows<BGRA>(in, typeInfo);
      default:
	throw std::runtime_error("Image<T, B>::restoreData(): unknown pixel type!!");
    }
    return in;
}

//! �w�肵����f�^�C�v�ŏo�̓X�g���[���ɉ摜�̉�f�f�[�^�������o���D
/*!
  \param out	�o�̓X�g���[��
  \param type	��f�^�C�v�D�������C#DEFAULT���w�肵���ꍇ�́C
		���̉摜�I�u�W�F�N�g�̉�f�^�C�v�ŏ����o�����D
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T, class B> std::ostream&
Image<T, B>::saveData(std::ostream& out, Type type) const
{
    if (type == DEFAULT)
	type = defaultType();

    switch (type)
    {
      case U_CHAR:
	return saveRows<u_char, RGB>(out, type);
      case SHORT:
	return saveRows<short, RGB>(out, type);
      case INT:
	return saveRows<int, RGB>(out, type);
      case FLOAT:
	return saveRows<float, RGB>(out, type);
      case DOUBLE:
	return saveRows<double, RGB>(out, type);
      case RGB_24:
	return saveRows<RGB, RGB>(out, type);
      case YUV_444:
	return saveRows<YUV444, RGB>(out, type);
      case YUV_422:
	return saveRows<YUV422, RGB>(out, type);
      case YUV_411:
	return saveRows<YUV411, RGB>(out, type);
      case BMP_8:
	return saveRows<u_char, BGRA>(out, type);
      case BMP_24:
	return saveRows<BGR, BGRA>(out, type);
      case BMP_32:
	return saveRows<BGRA, BGRA>(out, type);
      default:
	throw std::runtime_error("Image<T, B>::saveData(): unknown pixel type!!");
    }
    return out;
}

//! �摜�̃T�C�Y��ύX����D
/*!
  \param h	�V��������
  \param w	�V������
*/
template <class T, class B> inline void
Image<T, B>::resize(u_int h, u_int w)
{
    Array2<ImageLine<T>, B>::resize(h, w);
}

//! �O���̗̈���w�肵�ĉ摜�̃T�C�Y��ύX����D
/*!
  \param p	�O���̈�ւ̃|�C���^
  \param h	�摜�̍���
  \param w	�摜�̕�
  */
template <class T, class B> inline void
Image<T, B>::resize(T* p, u_int h, u_int w)
{
    Array2<ImageLine<T>, B>::resize(p, h, w);
}
 
template <class T, class B> template <class S> std::istream&
Image<T, B>::restoreRows(std::istream& in, const TypeInfo& typeInfo)
{
    const u_int		npads = type2nbytes(typeInfo.type, true);
    ImageLine<S>	buf(width());
    if (typeInfo.bottomToTop)
    {
	for (u_int v = height(); v > 0; )
	{
	    if (!buf.restore(in) || !in.ignore(npads))
		break;
	    (*this)[--v].fill((S*)buf);
	}
    }
    else
    {
	for (u_int v = 0; v < height(); )
	{
	    if (!buf.restore(in) || !in.ignore(npads))
		break;
	    (*this)[v++].fill((S*)buf);
	}
    }

    return in;
}

template <class T, class B> template <class S, class L> std::istream&
Image<T, B>::restoreAndLookupRows(std::istream& in, const TypeInfo& typeInfo)
{
    Array<L>	colormap(typeInfo.ncolors);
    colormap.restore(in);
	
    const u_int		npads = type2nbytes(typeInfo.type, true);
    ImageLine<S>	buf(width());
    if (typeInfo.bottomToTop)
    {
	for (u_int v = height(); v > 0; )
	{
	    if (!buf.restore(in) || !in.ignore(npads))
		break;
	    (*this)[--v].lookup((S*)buf, (L*)colormap);
	}
    }
    else
    {
	for (u_int v = 0; v < height(); )
	{
	    if (!buf.restore(in) || !in.ignore(npads))
		break;
	    (*this)[v++].lookup((S*)buf, (L*)colormap);
	}
    }

    return in;
}

template <class T, class B> template <class D, class L> std::ostream&
Image<T, B>::saveRows(std::ostream& out, Type type) const
{
    TypeInfo	typeInfo(type);

    Array<L>	colormap(typeInfo.ncolors);
    for (u_int i = 0; i < colormap.dim(); ++i)
	colormap[i] = i;
    colormap.save(out);
    
    Array<u_char>	pad(type2nbytes(type, true));
    pad = 0;
    
    ImageLine<D>	buf(width());
    if (typeInfo.bottomToTop)
    {
	for (u_int v = height(); v > 0; )
	{
	    buf.fill((const T*)(*this)[--v]);
	    if (!buf.save(out) || !pad.save(out))
		break;
	}
    }
    else
    {
	for (u_int v = 0; v < height(); )
	{
	    buf.fill((const T*)(*this)[v++]);
	    if (!buf.save(out) || !pad.save(out))
		break;
	}
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

template <class T, class B> ImageBase::Type
Image<T, B>::_defaultType() const
{
    return Image<T, B>::defaultType();
}

template <class T, class B> inline ImageBase::Type
Image<T, B>::defaultType() const
{
    return RGB_24;
}

template <> inline ImageBase::Type
Image<u_char, Buf<u_char> >::defaultType() const
{
    return U_CHAR;
}

template <> inline ImageBase::Type
Image<short, Buf<short> >::defaultType() const
{
    return SHORT;
}

template <> inline ImageBase::Type
Image<int, Buf<int> >::defaultType() const
{
    return INT;
}

template <> inline ImageBase::Type
Image<float, Buf<float> >::defaultType() const
{
    return FLOAT;
}

template <> inline ImageBase::Type
Image<double, Buf<double> >::defaultType() const
{
    return DOUBLE;
}

template <> inline ImageBase::Type
Image<YUV444, Buf<YUV444> >::defaultType() const
{
    return YUV_444;
}

template <> inline ImageBase::Type
Image<YUV422, Buf<YUV422> >::defaultType() const
{
    return YUV_422;
}

template <> inline ImageBase::Type
Image<YUV411, Buf<YUV411> >::defaultType() const
{
    return YUV_411;
}

template <class T, class B> void
Image<T, B>::_resize(u_int h, u_int w, const TypeInfo&)
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
				   u_int u, u_int v, u_int w, u_int h)
    :Array2<ImageLine<YUV411>, Buf<YUV411> >(i, v, u/2, h, w/2), ImageBase(i)
{
}

template <> inline u_int
Image<YUV411, Buf<YUV411> >::width() const
{
    return 2 * ncol();
}

template <> inline void
Image<YUV411, Buf<YUV411> >::resize(u_int h, u_int w)
{
    Array2<ImageLine<YUV411>, Buf<YUV411> >::resize(h, w/2);
}

template <> inline void
Image<YUV411, Buf<YUV411> >::resize(YUV411* p, u_int h, u_int w)
{
    Array2<ImageLine<YUV411>, Buf<YUV411> >::resize(p, h, w/2);
}

/************************************************************************
*  class GenericImage							*
************************************************************************/
//! ��f�̌^����Ȃ����̉摜�N���X
/*!
  �X�̍s���f�ɃA�N�Z�X���邱�Ƃ͂ł��Ȃ��D
*/
class GenericImage : public Array2<Array<u_char> >, public ImageBase
{
  public:
  //! ���̉摜�𐶐�����D
    GenericImage() :_typeInfo(U_CHAR), _colormap(0)			{}

    const TypeInfo&		typeInfo()			const	;
    std::istream&		restore(std::istream& in)		;
    std::ostream&		save(std::ostream& out)		const	;
    __PORT std::istream&	restoreData(std::istream& in)		;
    __PORT std::ostream&	saveData(std::ostream& out)	const	;
    
  private:
    __PORT virtual u_int	_width()			const	;
    __PORT virtual u_int	_height()			const	;
    __PORT virtual Type		_defaultType()			const	;
    __PORT virtual void		_resize(u_int h, u_int w,
					const TypeInfo& typeInfo)	;

    TypeInfo			_typeInfo;
    Array<BGRA>			_colormap;
};

//! ���ݕێ����Ă���摜�̃^�C�v����Ԃ��D
/*!
  \return	�^�C�v���
*/
inline const ImageBase::TypeInfo&
GenericImage::typeInfo() const
{
    return _typeInfo;
}

//! ���̓X�g���[������摜��ǂݍ��ށD
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
inline std::istream&
GenericImage::restore(std::istream& in)
{
    restoreHeader(in);
    return restoreData(in);
}

//! �o�̓X�g���[���ɉ摜�������o���D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
inline std::ostream&
GenericImage::save(std::ostream& out) const
{
    saveHeader(out, _typeInfo.type);
    return saveData(out);
}

}
#endif	/* !__TUImagePP_h */
