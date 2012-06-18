/*
 *  $Id: V4L2++.h,v 1.1 2012-06-18 08:21:22 ueshiba Exp $
 */
/*!
  \mainpage	libTUV4L2++ - Video for Linux v.2�ǥХ��������椹��C++�饤�֥��
  \anchor	libTUV4L2
*/
#ifndef __TUV4L2PP_h

#include <asm/types.h>		// for videodev2.h
#include <linux/videodev2.h>
#include <vector>
#include <string>
#include <boost/iterator_adaptors.hpp>
#ifdef HAVE_LIBTUTOOLS__
#  include "TU/Image++.h"
#endif

/*!
  \namespace	TU
  \brief	�ܥ饤�֥���������줿���饹����Ӵؿ�������̾������
*/
namespace TU
{
/************************************************************************
*  class V4L2Camera							*
************************************************************************/
//! Video for Linux v.2 ���б����륫����ɽ�����饹
class V4L2Camera
{
  public:
    enum PixelFormat		//! ���ϲ����β��Ǥη���
    {
	BGR24	= V4L2_PIX_FMT_BGR24,	//!< 24 bits/pix, BGR-8-8-8
	RGB24	= V4L2_PIX_FMT_RGB24,	//!< 24 bits/pix, RGB-8-8-8
	BGR32	= V4L2_PIX_FMT_BGR32,	//!< 32 bits/pix, BGR-8-8-8-8
	RGB32	= V4L2_PIX_FMT_RGB32,	//!< 32 bits/pix, RGB-8-8-8-8

	GREY	= V4L2_PIX_FMT_GREY,	//!<  8 bits/pix, Greyscale
	Y16	= V4L2_PIX_FMT_Y16,	//!< 16 bits/pix, Greyscale

	UYVY	= V4L2_PIX_FMT_UYVY,	//!< 16 bits/pix, YUV 4:2:2

	SBGGR8	= V4L2_PIX_FMT_SBGGR8,	//!<  8 bits/pix, BGGR bayer pattern
	SGBRG8	= V4L2_PIX_FMT_SGBRG8,	//!<  8 bits/pix, GBRG bayer pattern
	SGRBG8	= V4L2_PIX_FMT_SGRBG8,	//!<  8 bits/pix, GRBG bayer pattern

	UNKNOWN_PIXEL_FORMAT = v4l2_fourcc('U', 'K', 'N', 'W')
    };

    enum Feature		//! ������°��
    {
	BRIGHTNESS			= V4L2_CID_BRIGHTNESS,
	BRIGHTNESS_AUTO			= V4L2_CID_AUTOBRIGHTNESS,
	CONTRAST			= V4L2_CID_CONTRAST,
	GAIN				= V4L2_CID_GAIN,
	SATURATION			= V4L2_CID_SATURATION,
	HUE				= V4L2_CID_HUE,
	HUE_AUTO			= V4L2_CID_HUE_AUTO,
	GAMMA				= V4L2_CID_GAMMA,
	SHARPNESS			= V4L2_CID_SHARPNESS,
	WHITE_BALANCE_TEMPERATURE	= V4L2_CID_WHITE_BALANCE_TEMPERATURE,
	WHITE_BALANCE_AUTO		= V4L2_CID_AUTO_WHITE_BALANCE,
	BACKLIGHT_COMPENSATION		= V4L2_CID_BACKLIGHT_COMPENSATION,
	POWER_LINE_FREQUENCY		= V4L2_CID_POWER_LINE_FREQUENCY,
	EXPOSURE_AUTO			= V4L2_CID_EXPOSURE_AUTO,
	EXPOSURE_AUTO_PRIORITY		= V4L2_CID_EXPOSURE_AUTO_PRIORITY,
	EXPOSURE_ABSOLUTE		= V4L2_CID_EXPOSURE_ABSOLUTE,
	FOCUS_ABSOLUTE			= V4L2_CID_FOCUS_ABSOLUTE,
	FOCUS_RELATIVE			= V4L2_CID_FOCUS_RELATIVE,
	FOCUS_AUTO			= V4L2_CID_FOCUS_AUTO,
	ZOOM_ABSOLUTE			= V4L2_CID_ZOOM_ABSOLUTE,
	ZOOM_RELATIVE			= V4L2_CID_ZOOM_RELATIVE,
	ZOOM_CONTINUOUS			= V4L2_CID_ZOOM_CONTINUOUS,
	PAN_ABSOLUTE			= V4L2_CID_PAN_ABSOLUTE,
	PAN_RELATIVE			= V4L2_CID_PAN_RELATIVE,
	PAN_RESET			= V4L2_CID_PAN_RESET,
	TILT_ABSOLUTE			= V4L2_CID_TILT_ABSOLUTE,
	TILT_RELATIVE			= V4L2_CID_TILT_RELATIVE,
	TILT_RESET			= V4L2_CID_TILT_RESET,

	UNKNOWN_FEATURE
    };

    template <class T>
    struct Range		//! �ͤ��ϰ�
    {
	bool	involves(T val) const
		{
		    return (min <= val && val <= max &&
			    (val - min) % step == 0);
		}

	T	min;				//!< �Ǿ���
	T	max;				//!< ������
	T	step;				//!< ��ʬ���ƥå�
    };

    struct FrameRate		//! �ե졼��졼��
    {
	Range<u_int>		fps_n;		//!< ʬ��
	Range<u_int>		fps_d;		//!< ʬ��
    };
    typedef std::vector<FrameRate>::const_iterator	FrameRateIterator;

    struct FrameSize		//! �������礭��
    {
	std::pair<FrameRateIterator, FrameRateIterator>
				availableFrameRates()		const	;
	
	Range<u_int>		width;		//!< ��������
	Range<u_int>		height;		//!< �����ι⤵
	std::vector<FrameRate>	frameRates;	//!< �ե졼��ֳ�
    };
    typedef std::vector<FrameSize>::const_iterator	FrameSizeIterator;

    struct MenuItem		//! ��˥塼����
    {
	int			index;		//!< ��˥塼���ܤμ��̻�
	std::string		name;		//!< ��˥塼����̾
    };
    typedef std::vector<MenuItem>::const_iterator	MenuItemIterator;
    
  private:
    struct Format		//! �����ե����ޥå�
    {
	PixelFormat		pixelFormat;	//!< ���ǥե����ޥå�
	std::string		name;		//!< ���ǥե����ޥå�̾
	std::vector<FrameSize>	frameSizes;	//!< �������礭��
    };

    struct Control		//! ����ȥ���
    {
      public:
	Feature			feature;	//!< ����ȥ���μ��̻�
	std::string		name;		//!< ����ȥ���̾
	v4l2_ctrl_type		type;		//!< �ͤη�
	Range<int>		range;		//!< �ͤ��ϰ�
	int			def;		//!< �ǥե������
	u_int			flags;
	std::vector<MenuItem>	menuItems;
    };

    class Buffer	//! �����ѥХåե�
    {
      public:
	Buffer()	:_p(0), _size(0)		{}
	~Buffer()					;

	void		map(int fd, u_int index)	;
	const void*	p()			const	{return _p;}
	u_int		size()			const	{return _size;}
	
      private:
	void*		_p;
	u_int		_size;
    };

    template <class S, class T>
    struct MemberIterator
	: public boost::iterator_adaptor<MemberIterator<S, T>,
		typename std::vector<T>::const_iterator, const S>
    {
	MemberIterator(typename std::vector<T>::const_iterator iter)
	    :boost::iterator_adaptor<MemberIterator<S, T>,
		typename std::vector<T>::const_iterator, const S>(iter)	{}
	const S&	dereference()				 const	;
    };

  public:
    typedef MemberIterator<PixelFormat, Format>		PixelFormatIterator;
    typedef MemberIterator<Feature, Control>		FeatureIterator;
    
  public:
    V4L2Camera(const char* deviceName)					;
    ~V4L2Camera()							;

  // Format stuffs.
    std::pair<PixelFormatIterator, PixelFormatIterator>
			availablePixelFormats()			const	;
    std::pair<FrameSizeIterator, FrameSizeIterator>
			availableFrameSizes(PixelFormat pixelFormat)
								const	;
    bool		isAvailable(PixelFormat pixelFormat)	const	;
    V4L2Camera&		setFormat(PixelFormat pixelFormat,
				  u_int width, u_int height,
				  u_int fps_n, u_int fps_d)		;
    void		getFrameRate(u_int& fps_n, u_int& fps_d) const	;
    u_int		width()					const	;
    u_int		height()				const	;
    PixelFormat		pixelFormat()				const	;
    std::ostream&	put(std::ostream& out,
			    PixelFormat pixelFormat)		const	;
  
  // Feature stuffs.
    std::pair<FeatureIterator, FeatureIterator>
			availableFeatures()			const	;
    std::pair<MenuItemIterator, MenuItemIterator>
			availableMenuItems(Feature feature)	const	;
    bool		isAvailable(Feature feature)		const	;
    V4L2Camera&		setValue(Feature feature, int value)		;
    int			getValue(Feature feature)		const	;
    void		getMinMaxStep(Feature feature, int& min,
				      int& max, int& step)	const	;
    int			getDefaultValue(Feature feature)	const	;
    const std::string&	getName(Feature feature)		const	;
    std::ostream&	put(std::ostream& out, Feature feature)	const	;
    
  // Capture stuffs.
    V4L2Camera&		continuousShot()				;
    V4L2Camera&		stopContinuousShot()				;
    V4L2Camera&		snap()						;
#ifdef HAVE_LIBTUTOOLS__
    template <class T> const V4L2Camera&
			operator >>(Image<T>& image)		const	;
    template <class T> const V4L2Camera&
			captureRGBImage(Image<T>& image)	const	;
    template <class T> const V4L2Camera&
			captureDirectly(Image<T>& image)	const	;
#endif
    const V4L2Camera&	captureRaw(void* image)			const	;
    const V4L2Camera&	captureBayerRaw(void* image)		const	;

  // Utility functions.
    static PixelFormat	uintToPixelFormat(u_int pixelFormat)		;
    static Feature	uintToFeature(u_int feature)			;
    
  private:
    void		enumerateFormats()				;
    void		enumerateControls()				;
    int			enumerateMenuItems(
			    const v4l2_queryctrl& ctrl,
			    std::vector<MenuItem>& menuItems)		;
    const Format&	pixelFormatToFormat(PixelFormat pixelFormat)
								const	;
    const Control&	featureToControl(Feature feature)	const	;

    void		mapBuffers(u_int n)				;
    void		unmapBuffers()					;
    u_int		requestBuffers(u_int n)				;
    void		enqueueBuffer(u_int index)		const	;
    u_int		dequeueBuffer()				const	;
    
    int			ioctl(int request, void* arg)		const	;
    int			ioctl(int id, v4l2_queryctrl& ctrl)	const	;

    friend std::ostream&
	operator <<(std::ostream& out, const Format& format)		;
    friend std::ostream&
	operator <<(std::ostream& out, const Control& control)		;
    
  private:
    const int			_fd;
    std::vector<Format>		_formats;
    std::vector<Control>	_controls;
    u_int			_width;
    u_int			_height;
    PixelFormat			_pixelFormat;
    std::vector<Buffer>		_buffers;
    u_int			_current;	// ���塼������Ф���Ƥ���
};

//! ���Υ��������ѤǤ�����ǥե����ޥåȤ��ϰϤ��֤�
/*!
  \return	���ǥե����ޥå�(#PixelFormat)��ؤ����ȿ���ҤΥڥ�
*/
inline std::pair<V4L2Camera::PixelFormatIterator,
		 V4L2Camera::PixelFormatIterator>
V4L2Camera::availablePixelFormats() const
{
    return std::make_pair(_formats.begin(), _formats.end());
}
    
//! ���ꤷ�����ǥե����ޥåȤΤ�ȤǤ��Υ��������ѤǤ���������������ϰϤ��֤�
/*!
  \param pixelFormat	���ǥե����ޥå�
  \return		����������(#FrameSize)��ؤ����ȿ���ҤΥڥ�
*/
inline std::pair<V4L2Camera::FrameSizeIterator,
		 V4L2Camera::FrameSizeIterator>
V4L2Camera::availableFrameSizes(PixelFormat pixelFormat) const
{
    const Format&	format = pixelFormatToFormat(pixelFormat);
    return std::make_pair(format.frameSizes.begin(), format.frameSizes.end());
}
    
//! �������ꤵ��Ƥ�����������֤�
inline u_int
V4L2Camera::width() const
{
    return _width;
}

//! �������ꤵ��Ƥ�������⤵���֤�
inline u_int
V4L2Camera::height() const
{
    return _height;
}

//! �������ꤵ��Ƥ�����ǥե����ޥå�(#PixelFormat)���֤�
inline V4L2Camera::PixelFormat
V4L2Camera::pixelFormat() const
{
    return _pixelFormat;
}

//! ���ꤵ�줿���ǥե����ޥåȤ����Ƥ���Ϥ��롥
/*
  \param out		���ϥ��ȥ꡼��
  \param pixelFormat	���ǥե����ޥå�
  \return		out�ǻ��ꤷ�����ϥ��ȥ꡼��
*/ 
inline std::ostream&
V4L2Camera::put(std::ostream& out, PixelFormat pixelFormat) const
{
    return out << pixelFormatToFormat(pixelFormat);
}

//! ���Υ��������ѤǤ���°�����ϰϤ��֤�
/*!
  \return	°��(#Feature)��ؤ����ȿ���ҤΥڥ�
*/
inline std::pair<V4L2Camera::FeatureIterator, V4L2Camera::FeatureIterator>
V4L2Camera::availableFeatures() const
{
    return std::make_pair(_controls.begin(), _controls.end());
}
    
//! ����°�������ѤǤ����˥塼���ܤ��ϰϤ��֤�
/*!
  \return	��˥塼����(#MenuItem)��ؤ����ȿ���ҤΥڥ�
*/
inline std::pair<V4L2Camera::MenuItemIterator, V4L2Camera::MenuItemIterator>
V4L2Camera::availableMenuItems(Feature feature) const
{
    const Control&	control = featureToControl(feature);
    return std::make_pair(control.menuItems.begin(), control.menuItems.end());
}

//! ���ꤵ�줿°�������Ƥ���Ϥ���
/*
  \param out		���ϥ��ȥ꡼��
  \param feature	°��
  \return		out�ǻ��ꤷ�����ϥ��ȥ꡼��
*/ 
inline std::ostream&
V4L2Camera::put(std::ostream& out, Feature feature) const
{
    return out << featureToControl(feature);
}

//! ���ꤵ�줿°���Υǥե�����ͤ�Ĵ�٤�
/*!
  \param feature	�оݤȤʤ�°��
  \return		�ǥե������
*/
inline int
V4L2Camera::getDefaultValue(Feature feature) const
{
    return featureToControl(feature).def;
}

inline const std::string&
V4L2Camera::getName(Feature feature) const
{
    return featureToControl(feature).name;
}

//! ����餫����Ϥ����ǽ�β������ݻ�����
/*!
  ����餫��β������Ϥ�, continuousShot() �ˤ�äƹԤ���. �ºݤ˲����ǡ�����
  ���������ޤ�, �ܴؿ��ϸƤӽФ�¦��������֤��ʤ�. 
  \return	���Υ���饪�֥�������
 */
inline V4L2Camera&
V4L2Camera::snap()
{
  //if (_current != ~0)			// �����˼��������Хåե��������...
    _current = dequeueBuffer();		// �ǡ��������������Τ��Ԥ�
    enqueueBuffer(_current);	// ���塼���᤹
    return *this;
}

#ifdef HAVE_LIBTUTOOLS__
//! ����餫����Ϥ��줿������ľ��Ū�˼�����
/*!
  #operator >>() �Ȥΰ㤤��, �����������Ѵ���Ԥ�ʤ����Ȥ�, Image<T> ��¤��
  ����Υǡ����ΰ�ؤΥݥ��󥿤�V4L2���ϥХåե��ؤΥݥ��󥿤˽񤭴����뤳�Ȥ�
  ��ä�, �ºݤˤϥǡ����Υ��ԡ���Ԥ�ʤ����ȤǤ���. �ƥ�ץ졼�ȥѥ�᡼��T��,
  ��Ǽ��β����β��Ƿ�����ɽ��. �ʤ�, �ܴؿ���ƤӽФ����� snap() �ˤ�ä�
  ����餫��β������ݻ����Ƥ����ʤ���Фʤ�ʤ�. 
  \param image	�����ǡ������Ǽ����������֥�������. ���������ȹ⤵��, 
		���ߥ��������ꤵ��Ƥ�������������˹�碌�Ƽ�ưŪ��
		���ꤵ���. 
  \return	���Υ���饪�֥�������
*/
template <class T> const V4L2Camera&
V4L2Camera::captureDirectly(Image<T>& image) const
{
    if (_current == ~0)
	throw std::runtime_error("V4L2Camera::captureDirectly(): no images snapped!!");
    image.resize((T*)_buffers[_current].p(), height(), width());

    return *this;
}
#endif

template <> inline const V4L2Camera::PixelFormat&
V4L2Camera::MemberIterator<V4L2Camera::PixelFormat,
			   V4L2Camera::Format>::dereference() const
{
    return base_reference()->pixelFormat;
}

template <> inline const V4L2Camera::Feature&
V4L2Camera::MemberIterator<V4L2Camera::Feature,
			   V4L2Camera::Control>::dereference() const
{
    return base_reference()->feature;
}

//! ���ꤷ�������������Τ�ȤǤ��Υ��������ѤǤ���ե졼��졼�Ȥ��ϰϤ��֤�
/*!
  \return		�ե졼��졼��(#FrameRate)��ؤ����ȿ���ҤΥڥ�
*/
inline std::pair<V4L2Camera::FrameRateIterator,
		 V4L2Camera::FrameRateIterator>
V4L2Camera::FrameSize::availableFrameRates() const
{
    return std::make_pair(frameRates.begin(), frameRates.end());
}
    
/************************************************************************
*  global functions							*
************************************************************************/
template <class T>
std::ostream&	operator <<(std::ostream& out,
			    const typename V4L2Camera::Range<T>& range)	;
std::ostream&	operator <<(std::ostream& out,
			    const V4L2Camera::FrameSize& frameSize)	;
std::ostream&	operator <<(std::ostream& out,
			    const V4L2Camera::FrameRate& frameRate)	;
std::ostream&	operator <<(std::ostream& out,
			    const V4L2Camera::MenuItem& menuItem)	;
}
#endif //!__TUV4L2PP_h
