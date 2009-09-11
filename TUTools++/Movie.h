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
 *  $Id: Movie.h,v 1.7 2009-09-11 05:56:13 ueshiba Exp $
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
//! ���[�r�[��\���N���X
/*!
  ������view�������Ƃ��ł��C���̃T�C�Y�͊eview���ɕʌɎw��ł���D
  \param T	��f�̌^
*/
template <class T> class Movie
{
  private:
    class View : public Image<T>
    {
      public:
	View()	:Image<T>(), offset(0)			{}
	
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
				      ImageBase::Type type, u_int m)	;
    static u_int	nelements(u_int npixels)			;
    
  private:
    Array<View>		_views;
    u_int		_cView;			// current view #.
    u_int		_nelements;		// # of elements per frame.
    Array<Array<T> >	_frames;
    u_int		_cFrame;		// current frame #.
#ifdef _DEBUG
    std::ofstream	_log;
#endif
};

//! ���[�r�[�𐶐�����D
template <class T> inline
Movie<T>::Movie()
    :_views(0), _cView(0), _nelements(0), _frames(0), _cFrame(0)
{
#ifdef _DEBUG
    _log.open("Movie.log");
#endif
}

//! ���[�r�[��j�󂷂�D
template <class T> inline
Movie<T>::~Movie()
{
#ifdef _DEBUG
    _log.close();
#endif
}

//! ���[�r�[�̏�Ԃ�����ł��邩���ׂ�D
/*!
  \return	���݂̃t���[�����u�����̎��v�ɒB���Ă����false, �����łȂ����true
*/
template <class T> inline
Movie<T>::operator bool() const
{
    return (_cView < nviews() && _cFrame < nframes());
}

//! �t���[������Ԃ��D
/*!
  \return	�t���[����
*/
template <class T> inline u_int
Movie<T>::nframes() const
{
    return _frames.dim();
}

//! view����Ԃ��D
/*!
  \return	view��
*/
template <class T> inline u_int
Movie<T>::nviews() const
{
    return _views.dim();
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

//! ���݂�view�ԍ���Ԃ��D
/*!
  \return	view�ԍ�
*/
template <class T> inline u_int
Movie<T>::currentView() const
{
    return _cView;
}

//! ���݂�view�̉摜�̕���Ԃ��D
/*!
  \return	�摜�̕�
*/
template <class T> inline u_int
Movie<T>::width() const
{
    return (_cView < nviews() ? _views[_cView].width() : 0);
}
    
//! ���݂�view�̉摜�̍�����Ԃ��D
/*!
  \return	�摜�̍���
*/
template <class T> inline u_int
Movie<T>::height() const
{
    return (_cView < nviews() ? _views[_cView].height() : 0);
}

//! ���݂�view��frame�ɑΉ�����摜��Ԃ��D
/*!
  \return	�摜
*/
template <class T> inline const Image<T>&
Movie<T>::image() const
{
    return _views[_cView];
}

//! ���݂�view��frame�ɑΉ�����摜��Ԃ��D
/*!
  \return	�摜
*/
template <class T> inline Image<T>&
Movie<T>::image()
{
    return _views[_cView];
}

//! ���݂̃t���[�����w�肷��D
/*!
  frame < #nframes() �łȂ��ꍇ�͌��݃t���[���� #nframes() �ƂȂ�C
  #operator ()��false���Ԃ�����ԂɂȂ�D
  \param frame	�t���[���ԍ�
  \return	���̃��[�r�[�I�u�W�F�N�g
*/
template <class T> Movie<T>&
Movie<T>::setFrame(u_int frame)
{
#ifdef _DEBUG
    using namespace	std;
    _log << "  Begin: Movie<T>::setFrame(frame = " << frame << ")" << endl;
#endif
    if (frame != _cFrame)
    {
	if (frame < nframes())
	{
	    _cFrame = frame;

	    for (u_int i = 0; i < nviews(); ++i)
		_views[i].resize((T*)_frames[_cFrame] + _views[i].offset,
				 _views[i].height(), _views[i].width());
	}
	else
	    _cFrame = nframes();
    }
#ifdef _DEBUG
    _log << "  End:   Movie<T>::setFrame()" << endl;
#endif
    return *this;
}

//! ���݂�view���w�肷��D
/*!
  view < #nviews() �łȂ��ꍇ�͉����ύX���Ȃ��D
  \param view	view�ԍ�
  \return	���̃��[�r�[�I�u�W�F�N�g
*/
template <class T> inline Movie<T>&
Movie<T>::setView(u_int view)
{
#ifdef _DEBUG
    using namespace	std;
    _log << "  Begin: Movie<T>::setView(view = " << view << ")" << endl;
#endif
    if (view < nviews())
	_cView = view;
#ifdef _DEBUG
    _log << "  End:   Movie<T>::setView()" << endl;
#endif
    return *this;
}

//! ���݂̃t���[�����ŏ�(0)�ɖ߂��D
/*!
  \return	���̃��[�r�[�I�u�W�F�N�g
*/
template <class T> inline Movie<T>&
Movie<T>::rewind()
{
    return setFrame(0);
}

//! ���݂̃t���[����1��ɐi�߂�D
/*!
  ���ɍŌ�̃t���[���ɒB���Ă���ꍇ�̓t���[���ԍ���#nframes()�ɂ���D
  \return	���̃��[�r�[�I�u�W�F�N�g
*/
template <class T> inline Movie<T>&
Movie<T>::operator ++()
{
    if (_cFrame < nframes())
	setFrame(_cFrame + 1);
    return *this;
}

//! ���݂̃t���[����1�߂��D
/*!
  ���ɍŏ��̃t���[���ɒB���Ă���ꍇ�͉������Ȃ��D
  \return	���̃��[�r�[�I�u�W�F�N�g
*/
template <class T> inline Movie<T>&
Movie<T>::operator --()
{
    if (_cFrame > 0)
	setFrame(_cFrame - 1);
    return *this;
}

//! �eview�̃T�C�Y�ƃt���[�������w�肵�ă��[�r�[�̋L���̈���m�ۂ���D
/*!
  �m�ی�́C���݂�view�ƃt���[��������0�ɐݒ肷��D
  \param sizes	�eview�̕��ƍ����̃y�A�����߂��z��D�z��̃T�C�Y��view���ƂȂ�D
  \param nf	�t���[����
  \return	���ۂɊm�ۂ��ꂽ�t���[����
*/
template <class T> u_int
Movie<T>::alloc(const Array<std::pair<u_int, u_int> >& sizes, u_int nf)
{
#ifdef _DEBUG
    using namespace	std;
    _log << "  Begin: Movie<T>::alloc(nframes = " << nf
	 << ", nviews = " << sizes.dim()
	 << ")" << endl;
#endif
  // �eview�̃I�t�Z�b�g��1�t���[��������̉�f����ݒ�D
    _views.resize(sizes.dim());
    _nelements = 0;
    for (u_int i = 0; i < nviews(); ++i)
    {
	_views[i].offset = _nelements;
	_nelements += nelements(sizes[i].first * sizes[i].second);
    }
	     
  // �w�肳�ꂽ�����̃t���[����ݒ�D
    _frames.resize(nf);
    for (u_int j = 0; j < _frames.dim(); ++j)
    {
	try
	{
	    _frames[j].resize(_nelements);
#ifdef _DEBUG
	    _log << "    " << j << "-th frame allocated..." << endl;
#endif
	}
	catch (...)
	{
	    _frames.resize(j);
#ifdef _DEBUG
	    _log << "    " << j << "-th frame cannot be allocated!" << endl;
#endif
	    break;
	}
    }
    
  // �w�肳�ꂽ����view�Ƃ��̑傫����ݒ�D
    if (nframes() > 0)
	for (u_int i = 0; i < nviews(); ++i)
	    _views[i].resize((T*)_frames[0] + _views[i].offset,
			     sizes[i].second, sizes[i].first);
    
    _cFrame = 0;
    _cView  = 0;
#ifdef _DEBUG
    _log << "  End:   Movie<T>::alloc()" << endl;
#endif
    return nframes();			// Return #frames allocated correctly.
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

template <class T> ImageBase::Type
Movie<T>::restoreHeader(std::istream& in)
{
    using namespace	std;
    
    char	c;
    if (!in.get(c))
	return ImageBase::DEFAULT;
    if (c != 'M')
	throw runtime_error("TU::Movie<T>::restoreHeader: not a movie file!!");
    
    u_int	nv;
    in >> nv >> skipl;
    _views.resize(nv);

    ImageBase::Type	type = ImageBase::DEFAULT;
    _nelements = 0;
    for (u_int i = 0; i < nviews(); ++i)
    {
	type = _views[i].restoreHeader(in);
	_views[i].offset = _nelements;
	_nelements += nelements(_views[i].width() * _views[i].height());
    }

    return type;
}

template <class T> std::istream&
Movie<T>::restoreFrames(std::istream& in, ImageBase::Type type, u_int m)
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
	for (u_int i = 0; i < nviews(); ++i)
	{
	    _views[i].resize((T*)frame + _views[i].offset,
			      _views[i].height(), _views[i].width());
	    _views[i].restoreData(in, type);
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

//! ���[�r�[���w�肵����f�^�C�v�ŏo�̓X�g���[���ɏ����o���D
/*!
 \param out	�o�̓X�g���[��
 \param type	��f�^�C�v�D�������C#DEFAULT���w�肵���ꍇ�́C���̃��[�r�[��
		��f�^�C�v�ŏ����o�����D   
 \return	out�Ŏw�肵���o�̓X�g���[��
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

//! ���[�r�[�̃w�b�_���w�肵����f�^�C�v�ŏo�̓X�g���[���ɏ����o���D
/*!
 \param out	�o�̓X�g���[��
 \param type	��f�^�C�v�D�������C#DEFAULT���w�肵���ꍇ�́C���̃��[�r�[��
		��f�^�C�v�ŏ����o�����D   
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
 \param type	��f�^�C�v�D�������C#DEFAULT���w�肵���ꍇ�́C���̃��[�r�[��
		��f�^�C�v�ŏ����o�����D   
 \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class T> std::ostream&
Movie<T>::saveFrame(std::ostream& out, ImageBase::Type type) const
{
    for (u_int i = 0; i < nviews(); ++i)
	_views[i].saveData(out, type);
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
