/*
 *  $Id: Ieee1394CameraArray.cc,v 1.1 2009-05-10 23:34:27 ueshiba Exp $
 */
#include "TU/Ieee1394CameraArray.h"

#ifdef HAVE_LIBTUTOOLS__
namespace TU
{
/************************************************************************
*  class Ieee1394CameraArray						*
************************************************************************/
//! IEEE1394�f�W�^���J�����̔z��𐶐�����D
/*!
  \param in		�J�����̐ݒ�t�@�C����ǂݍ��ޓ��̓X�g���[��
  \param i1394b		IEEE1394b���[�h (800Mbps)�œ���
  \param ncameras	��������J�����䐔�D�ݒ�t�@�C���ɋL����Ă���ŏ���
			ncameras�䂪���������D-1���w�肷��ƁC�ݒ�t�@�C��
			���̑S�J���������������D
*/
Ieee1394CameraArray::Ieee1394CameraArray(std::istream& in, bool i1394b,
					 int ncameras)
    :Array<Ieee1394Camera*>()
{
    using namespace	std;

  // �ݒ�t�@�C������x���p�����[�^�ƃJ��������ǂݍ��ށD
    int	delay, n;
    in >> delay >> n;
    if ((ncameras < 0) || (ncameras > n))
	ncameras = n;
    resize(ncameras);
    
  // �ݒ�t�@�C���ɋL���ꂽ�S�J�����𐶐�����D
    for (int i = 0; i < dim(); ++i)
    {
	string	s;
	in >> s;			// global unique ID�̓ǂݍ���
	u_int64	uniqId = strtoull(s.c_str(), 0, 0);
	(*this)[i] = new Ieee1394Camera(Ieee1394Camera::Monocular,
					i1394b, uniqId, delay);
	in >> *(*this)[i];		// �J�����p�����[�^�̓ǂݍ��݂Ɛݒ�
    }
}

//! IEEE1394�f�W�^���J�����̔z���j�󂷂�D
Ieee1394CameraArray::~Ieee1394CameraArray()
{
    for (int i = 0; i < dim(); ++i)
	delete (*this)[i];
}
 
}
#endif	/* HAVE_LIBTUTOOLS__	*/    
