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
 *  $Id: Movie.h,v 1.14 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		Movie.h
  \brief	�N���X TU::Movie �̒�`�Ǝ���
*/
#ifndef __TUMovie_h
#define __TUMovie_h

#include <utility>
#include <list>
#include "TU/Image++.h"
#include "TU/Manip.h"

namespace TU
{
/************************************************************************
*  class Movie<T>							*
************************************************************************/
//! ���[�r�[��\���N���X
/*!
  �����̃r���[�������Ƃ��ł��C���̃T�C�Y�͊e�r���[���ɌʂɎw��ł���D
  \param T	��f�̌^
*/
template <class T> class Movie
{
  public:
  //! �e�r���[�̕��ƍ����̃y�A
    typedef std::pair<u_int, u_int>			Size;

  private:
  //! �r���[
    struct View : public Image<T>
    {
	View()	:Image<T>(), offset(0)					{}
	
	u_int	offset;		//!< �t���[���̐擪����̉摜�f�[�^�̈�̃I�t�Z�b�g
    };

  //! �t���[��
    class Frame : public Array<T>
    {
      private:
	typedef Array<T>				super;

      public:
	explicit Frame(u_int n=0)	:super(n)	{super::operator =(0);}
    };

    typedef std::list<Frame>				Frames;
    typedef typename Frames::iterator			iterator;

  public:
    Movie(u_int nviews=0)						;

  // Set sizes for each view.
    Movie<T>&		setSizes(const Array<Size>& sizes)		;

  // General information.
    bool		isCircularMode()			const	;
    Movie<T>&		setCircularMode(bool circular)			;
    u_int		nviews()				const	;
    u_int		width(u_int view)			const	;
    u_int		height(u_int view)			const	;
    const Image<T>&	image(u_int view)			const	;
    Image<T>&		image(u_int view)			;

  // Handling frames.
			operator bool()				const	;
    u_int		nframes()				const	;
    u_int		currentFrame()				const	;
    Movie<T>&		setFrame(u_int frame)				;
    Movie<T>&		rewind()					;
    Movie<T>&		operator ++()					;
    Movie<T>&		operator --()					;
    
  // Edit movie.
    Movie<T>&		insert(u_int n)					;
    const Movie<T>&	copy(u_int n)				const	;
    Movie<T>&		cut(u_int n)					;
    u_int		paste()						;
    Movie<T>&		swap()						;
    
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
    Movie<T>&		setFrameToViews()				;
    ImageBase::TypeInfo	restoreHeader(std::istream& in)			;
    std::istream&	restoreFrames(std::istream& in,
				      ImageBase::TypeInfo typeInfo,
				      u_int m)				;
    static u_int	npixels(u_int n)				;
    
  private:
    bool		_circular;	//!< �z���[�h/��z���[�h
    Array<View>		_views;		//!< �r���[�̕���
    Frames		_frames;	//!< �t���[���̕���
    iterator		_dummy;		//!< �t���[���̖�����\���_�~�[�t���[��
    iterator		_current;	//!< ���t���[��
    u_int		_cFrame;	//!< ���t���[���̔ԍ�
    mutable Frames	_buf;		//!< �ҏW�p�o�b�t�@
};

//! �r���[�����w�肵�ă��[�r�[�𐶐�����D
/*!
  \param nviews		�r���[��
*/
template <class T> inline
Movie<T>::Movie(u_int nviews)
    :_circular(false), _views(nviews), _frames(1),
     _dummy(_frames.begin()), _current(_dummy), _cFrame(0), _buf()
{
}

//! �e�r���[�̃T�C�Y���w�肷��D
/*!
  �w���́C���݂̃r���[�ƃt���[��������0�ɐݒ肷��D
  \param sizes	�e�r���[�̕��ƍ����̃y�A�����߂��z��D�z��̃T�C�Y���r���[���ƂȂ�D
  \return	���̃��[�r�[
*/
template <class T> Movie<T>&
Movie<T>::setSizes(const Array<Size>& sizes)
{
  // �r���[���Ɗe�r���[�̃I�t�Z�b�g��ݒ�D
    _views.resize(sizes.dim());
    u_int	n = 0;
    for (u_int i = 0; i < nviews(); ++i)
    {
	_views[i].offset = n;
	n += npixels(sizes[i].first * sizes[i].second);
    }
    
  // �傫����1�t���[��������̉�f���ɓ������_�~�[�t���[�����m�ہD
    _frames.clear();			// �S�t���[����p��
    _frames.push_front(Frame(n));	// �_�~�[�t���[�����m��
    _dummy  = _frames.begin();

  // �e�r���[�Ƀ_�~�[�t���[����ݒ�D
    for (u_int i = 0; i < nviews(); ++i)
	_views[i].resize((T*)*_dummy + _views[i].offset,
			 sizes[i].second, sizes[i].first);

  // ���t���[�����_�~�[�t���[���ɐݒ�D
    _current = _dummy;
    _cFrame  = 0;
    
    return *this;
}

//! �z���[�h�ł��邩���ׂ�D
/*!
  \return	�z���[�h�ł����true, �����łȂ����false
*/
template <class T> inline bool
Movie<T>::isCircularMode() const
{
    return _circular;
}
    
//! �z��/��z���[�h��ݒ肷��D
/*!
  �z���[�h�ɐݒ肷��ꍇ�́C���t���[�������[�r�[�̖����ł���ΐ擪�ɐݒ肷��D
  \param circular	�z���[�h�ł����true, �����łȂ����false
  \return		���̃��[�r�[
*/
template <class T> Movie<T>&
Movie<T>::setCircularMode(bool circular)
{
    _circular = circular;

    if (_circular && _current == _dummy)
	return rewind();
    else
	return *this;
}
    
//! �r���[����Ԃ��D
/*!
  \return	view��
*/
template <class T> inline u_int
Movie<T>::nviews() const
{
    return _views.dim();
}

//! �w�肳�ꂽ�r���[�ɑΉ�����摜�̕���Ԃ��D
/*!
  \param view	�r���[�ԍ�
  \return	�摜�̕�
*/
template <class T> inline u_int
Movie<T>::width(u_int view) const
{
    return _views[view].width();
}
    
//! �w�肳�ꂽ�r���[�ɑΉ�����摜�̍�����Ԃ��D
/*!
  \param view	�r���[�ԍ�
  \return	�摜�̍���
*/
template <class T> inline u_int
Movie<T>::height(u_int view) const
{
    return _views[view].height();
}

//! ���݂̃t���[���̎w�肳�ꂽ�r���[�ɑΉ�����摜��Ԃ��D
/*!
  \param view	�r���[�ԍ�
  \return	�摜
*/
template <class T> inline const Image<T>&
Movie<T>::image(u_int view) const
{
    return _views[view];
}

//! ���݂̃t���[���̎w�肳�ꂽ�r���[�ɑΉ�����摜��Ԃ��D
/*!
  \param view	�r���[�ԍ�
  \return	�摜
*/
template <class T> inline Image<T>&
Movie<T>::image(u_int view)
{
    return _views[view];
}

//! ���t���[���̏�Ԃ𒲂ׂ�D
/*!
  \return	���t���[�����Ō�̃t���[���̎��ɒB���Ă����false,
		�����łȂ����true
*/
template <class T> inline
Movie<T>::operator bool() const
{
    return (_current != _dummy);
}

//! �t���[������Ԃ��D
/*!
  \return	�t���[����
*/
template <class T> inline u_int
Movie<T>::nframes() const
{
    return _frames.size() - 1;		// �_�~�[�t���[���̓J�E���g���Ȃ��D
}

//! ���݂̃t���[���ԍ���Ԃ��D
/*!
  \return	�t���[���ԍ�
*/
template <class T> inline u_int
Movie<T>::currentFrame() const
{
    return _cFrame;
}

//! ���t���[�����w�肷��D
/*!
  frame >= nframes() �̏ꍇ�͌��t���[���� nframes() ���Ԃ��l�ɐݒ肳��C
  #operator bool() ��false���Ԃ�����ԂɂȂ�D
  \param frame	�t���[���ԍ�
  \return	���̃��[�r�[
*/
template <class T> inline Movie<T>&
Movie<T>::setFrame(u_int frame)
{
    using namespace	std;

    _cFrame  = min(frame, nframes());
    _current = _frames.begin();
    advance(_current, _cFrame);

    return setFrameToViews();
}
    
//! ���t���[�������[�r�[�̐擪�ɖ߂��D
/*!
  \return	���̃��[�r�[
*/
template <class T> inline Movie<T>&
Movie<T>::rewind()
{
    return setFrame(0);
}

//! ���t���[����1��ɐi�߂�D
/*!
  ���t���[�������ɍŌ�̃t���[���̎��ɒB���Ă�����( #operator bool() ��
  false���Ԃ�����ԂɂȂ��Ă�����)�C���������Ƀ��^�[������D
  ���t���[�����Ō�̃t���[���ł���ꍇ�C�z���[�h�łȂ��Ȃ�΂����
  �Ō�̃t���[���̎��ɐi�݁C #operator bool() ��false���Ԃ�����ԂɂȂ�D
  �z���[�h�Ȃ�ΐ擪�t���[���Ɉړ�����D
  \return	���̃��[�r�[
*/
template <class T> inline Movie<T>&
Movie<T>::operator ++()
{
    if (_current == _dummy)
	return *this;

    ++_current;
    ++_cFrame;
    if (_current == _dummy && _circular)	// �����ɒB���C
    {						// ���z���[�h�Ȃ��...
	_current = _frames.begin();		// �擪�Ɉړ�����D
	_cFrame  = 0;
    }

    return setFrameToViews();
}

//! ���݂̃t���[����1�O�ɖ߂��D
/*!
  ���t���[�������[�r�[�̐擪�̏ꍇ�C�z���[�h�łȂ��Ȃ�΃��[�r�[��
  �Ō�̃t���[���̎��Ɉړ����C #operator bool() ��false���Ԃ�����ԂɂȂ�D
  �z���[�h�Ȃ�΍Ō�̃t���[���Ɉړ�����D
  \return	���̃��[�r�[
*/
template <class T> inline Movie<T>&
Movie<T>::operator --()
{
    if (_current == _frames.begin())	// ���[�r�[�̐擪�Ȃ��...
    {
	_current = _dummy;		// �Ō�̃t���[���̎��Ɉړ�����D
	_cFrame  = nframes();
	
	if (_circular)			// ����ɏz���[�h�Ȃ��...
	{
	    --_current;			// �Ō�̃t���[���ɖ߂�D
	    --_cFrame;
	}
    }
    else
    {
	--_current;
	--_cFrame;
    }

    return setFrameToViews();
}

//! ���t���[���̒��O�Ɏw�肵�������̃t���[����}������D
/*!
  ���t���[���͑}�������ŏ��̃t���[���ƂȂ�D
  \param n	�}�����閇��
  \return	���̃��[�r�[
*/
template <class T> Movie<T>&
Movie<T>::insert(u_int n)
{
    iterator	cur = _current;		// ���t���[�����L������D

  // �}����� _current �̍Đݒ�ɔ�����D
    if (_current == _frames.begin())	// �擪�ɑ}������Ȃ�...
	_current = _frames.end();	// �ꎞ�I�ɖ������D
    else				// �����łȂ����...
	--_current;			// �}���ʒu��1�O�D

  // ���t���[���̒��O�ɑ}��
    _frames.insert(cur, n, Frame(_dummy->dim()));

  // _current �ɑ}�����ꂽ�̈�̐擪���Đݒ肷��D
  // �擪����� _current �̃I�t�Z�b�g _cFrame �͕ω����Ȃ��D
    if (_current == _frames.end())	// �擪�ɑ}�������ꍇ��...
	_current = _frames.begin();	// ���[�r�[�̐擪�D
    else				// �����łȂ����...
	++_current;			// �}���ʒu��1�O�̎��D

    return setFrameToViews();
}
    
//! ���t���[������w�肳�ꂽ�����̃t���[����ҏW�p�o�b�t�@�ɃR�s�[����D
/*!
  �ҏW�p�o�b�t�@�̓��e�͏㏑�������D���t���[���͕ω����Ȃ��D
  \param n	�R�s�[����閇��
  \return	���̃��[�r�[
 */
template <class T> const Movie<T>&
Movie<T>::copy(u_int n) const
{
  // �R�s�[�����̈�̖��������o����D
    iterator	tail = _current;
    std::advance(tail, std::min(n, nframes() - _cFrame));

  // [_current, tail) �� _buf �ɃR�s�[����D
    _buf.assign(_current, tail);

    return *this;
}

//! ���t���[������w�肳�ꂽ�t���[�������J�b�g���ĕҏW�p�o�b�t�@�Ɉڂ��D
/*!
  �ҏW�p�o�b�t�@�̓��e�͏㏑�������D���t���[���̓J�b�g���ꂽ�̈�̒���ƂȂ�D
  \param n	�J�b�g�����t���[����
  \return	���̃��[�r�[
 */
template <class T> Movie<T>&
Movie<T>::cut(u_int n)
{
  // �J�b�g�����̈�̖��������o����D
    n = std::min(n, nframes() - _cFrame);
    iterator	tail = _current;
    std::advance(tail, n);

  // [_current, tail) �� _buf �Ɉڂ��D
    _buf.clear();
    _buf.splice(_buf.begin(), _frames, _current, tail);
    
  // _current���J�b�g���ꂽ�̈�̒���ɍĐݒ肷��D
  // �擪����� _current �̃I�t�Z�b�g _cFrame �͕ω����Ȃ��D
    _current = tail;

    if (_current == _dummy && _circular)  // �����܂ŃJ�b�g���z���[�h�Ȃ��...
	return rewind();		  // �擪�ɖ߂�D
    else				  // �����łȂ����...
	return setFrameToViews();	  // _current ���e�r���[�ɐݒ肷��D
}

//! ���t���[���̒��O�ɕҏW�p�o�b�t�@�̓��e��}������D
/*!
  �ҏW�p�o�b�t�@�͋�ɂȂ�D���t���[���͑}�����ꂽ�̈�̐擪�ɂȂ�D
  \return	�}�����ꂽ�t���[����
 */
template <class T> u_int
Movie<T>::paste()
{
    iterator	cur = _current;		// ���t���[�����L������D
    u_int	n = _buf.size();	// �ҏW�p�o�b�t�@���̃t���[����
    
  // �}����� _current �̍Đݒ�ɔ�����D
    if (_current == _frames.begin())	// �擪�ɑ}������Ȃ�...
	_current = _frames.end();	// �ꎞ�I�ɖ������D
    else				// �����łȂ����...
	--_current;			// �}���ʒu��1�O�D

  // _buf�̓��e�����t���[���̒��O�ɑ}������D
    _frames.splice(cur, _buf);

  // _current �ɑ}�����ꂽ�̈�̐擪���Đݒ肷��D
  // �擪����� _current �̃I�t�Z�b�g _cFrame �͕ω����Ȃ��D
    if (_current == _frames.end())	// �擪�ɑ}�������ꍇ��...
	_current = _frames.begin();	// ���[�r�[�̐擪�D
    else				// �����łȂ����...
	++_current;			// �}���ʒu��1�O�̎��D

    setFrameToViews();

    return n;
}

//! ���t���[���̑O�����������D
/*!
  ���t���[���͌����O�̃��[�r�[�̐擪�ɂȂ�D
  \return	���̃��[�r�[
 */
template <class T> inline Movie<T>&
Movie<T>::swap()
{
    iterator	tmp = _frames.begin();	// �����O�̐擪���L��

    _frames.splice(_frames.begin(), _frames, _current, _dummy);

    _current = tmp;			// �����O�̐擪�����t���[���Ƃ���
    _cFrame  = nframes() - _cFrame;	// ������̑O���̒��� = �����O�̌㔼�̒���
    
    return setFrameToViews();
}
    
//! ���̓X�g���[�����烀�[�r�[��ǂݍ��ށD
/*!
  ���̓X�g���[���̖����ɒB����܂Ńt���[����ǂݑ�����D
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class T> inline std::istream&
Movie<T>::restore(std::istream& in)
{
    return restoreFrames(in, restoreHeader(in), 0);
}

//! ���[�r�[���w�肵����f�^�C�v�ŏo�̓X�g���[���ɏ����o���D
/*!
 \param out	�o�̓X�g���[��
 \param type	��f�^�C�v�D�������C #TU::ImageBase::DEFAULT ���w�肵���ꍇ�́C
		���̃��[�r�[�̉�f�^�C�v�ŏ����o�����D   
 \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T> std::ostream&
Movie<T>::save(std::ostream& out, ImageBase::Type type)
{
    saveHeader(out, type);

    bool	circular = isCircularMode();
    setCircularMode(false);

    for (rewind(); *this; ++(*this))
	saveFrame(out, type);
    rewind();

    setCircularMode(circular);
    
    return out;
}

//! ���[�r�[�̃w�b�_���w�肵����f�^�C�v�ŏo�̓X�g���[���ɏ����o���D
/*!
 \param out	�o�̓X�g���[��
 \param type	��f�^�C�v�D�������C #TU::ImageBase::DEFAULT ���w�肵���ꍇ�́C
		���̃��[�r�[�̉�f�^�C�v�ŏ����o�����D   
 \return	���ۂɏ����o���ꍇ�̉�f�^�C�v
*/
template <class T> ImageBase::Type
Movie<T>::saveHeader(std::ostream& out, ImageBase::Type type) const
{
    using namespace	std;
    
    out << 'M' << nviews() << endl;
    for (u_int i = 0; i < nviews(); ++i)
	type = _views[i].saveHeader(out, type);
    return type;
}

//! ���݂̃t���[�����w�肵����f�^�C�v�ŏo�̓X�g���[���ɏ����o���D
/*!
 \param out	�o�̓X�g���[��
 \param type	��f�^�C�v�D�������C #TU::ImageBase::DEFAULT ���w�肵���ꍇ�́C
		���̃��[�r�[�̉�f�^�C�v�ŏ����o�����D   
 \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T> std::ostream&
Movie<T>::saveFrame(std::ostream& out, ImageBase::Type type) const
{
    for (u_int i = 0; i < nviews(); ++i)
	_views[i].saveData(out, type);
    return out;
}

/*
 *  private member functions
 */
//! ���t���[�����X�̃r���[�ɃZ�b�g����D
/*!
  \return	���̃��[�r�[
*/ 
template <class T> Movie<T>&
Movie<T>::setFrameToViews()
{
    for (u_int i = 0; i < _views.dim(); ++i)
	_views[i].resize((T*)*_current + _views[i].offset,
			 _views[i].height(), _views[i].width());
    return *this;
}
    
template <class T> ImageBase::TypeInfo
Movie<T>::restoreHeader(std::istream& in)
{
    using namespace	std;

  // �t�@�C���̐擪������'M'�ł��邱�Ƃ��m�F����D
    char	c;
    if (!in.get(c))
	return ImageBase::TypeInfo(ImageBase::DEFAULT);
    if (c != 'M')
	throw runtime_error("TU::Movie<T>::restoreHeader: not a movie file!!");

  // �r���[����ǂݍ��݁C���̂��߂̗̈���m�ۂ���D
    u_int	nv;
    in >> nv >> skipl;
    _views.resize(nv);

  // �e�r���[�̃w�b�_��ǂݍ��݁C���̉摜�T�C�Y���Z�b�g����D
    ImageBase::TypeInfo	typeInfo(ImageBase::DEFAULT);
    Array<Size>		sizes(nviews());
    for (u_int i = 0; i < nviews(); ++i)
    {
	typeInfo = _views[i].restoreHeader(in);
	sizes[i] = make_pair(_views[i].width(), _views[i].height());
    }
    setSizes(sizes);
    
    return typeInfo;
}

template <class T> std::istream&
Movie<T>::restoreFrames(std::istream& in, ImageBase::TypeInfo typeInfo, u_int m)
{
    for (;;)
    {
      // �Ƃ肠�����_�~�[�t���[���ɓǂݍ��ށD
	for (u_int i = 0; i < nviews(); ++i)
	    if (!_views[i].restoreData(in, typeInfo))
		goto finish;

      // �R�s�[���ă_�~�[�t���[���̒��O�ɑ}��
	_frames.insert(_dummy, *_dummy);
    }

  finish:
    rewind();	// �擪�t���[�������t���[���Ƃ���D
    
    return in;
}

template <class T> inline u_int
Movie<T>::npixels(u_int n)
{
    return n;
}

template <> inline u_int
Movie<YUV411>::npixels(u_int n)
{
    return n / 2;
}

}
#endif	// !__TUMovie_h
