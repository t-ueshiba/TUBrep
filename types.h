/*
 *  $Id: types.h,v 1.33 2011-12-08 01:40:46 ueshiba Exp $
 */
/*!
  \mainpage	libTUTools++ - �z��C�x�N�g���C�s��C�摜���̊�{�I�ȃf�[�^�^�Ƃ���ɕt�������A���S���Y�������߂����C�u����
  \anchor	libTUTools

  \section copyright ���쌠
  ����14-19�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L

  �n��ҁF�A�ŏr�v

  �{�v���O�����́i�Ɓj�Y�ƋZ�p�����������̐E���ł���A�ŏr�v���n�삵�C
  �i�Ɓj�Y�ƋZ�p���������������쌠�����L����閧���ł��D���쌠���L
  �҂ɂ�鋖�Ȃ��ɖ{�v���O�������g�p�C�����C���ρC��O�҂֊J������
  ���̍s�ׂ��֎~���܂��D
   
  ���̃v���O�����ɂ���Đ����邢���Ȃ鑹�Q�ɑ΂��Ă��C���쌠���L�҂�
  ��ёn��҂͐ӔC�𕉂��܂���B

  Copyright 2002-2007.
  National Institute of Advanced Industrial Science and Technology (AIST)

  Creator: Toshio UESHIBA

  [AIST Confidential and all rights reserved.]
  This program is confidential. Any using, copying, changing or
  giving any information concerning with this program to others
  without permission by the copyright holder are strictly prohibited.

  [No Warranty.]
  The copyright holder or the creator are not responsible for any
  damages caused by using this program.

  \section abstract �T�v
  libTUTools++�́C�z��C�x�N�g���C�s��C�摜���̊�{�I�ȃf�[�^�^�Ƃ���
  �ɕt�������A���S���Y�������߂����C�u�����ł���D���ݎ�������Ă����
  �v�ȃN���X����ъ֐��͂����܂��Ɉȉ��̕���ɕ��ނ����D

  <b>1���������2�����z��</b>
  - #TU::Array
  - #TU::Array2

  <b>�x�N�g���ƍs�񂨂�ѐ��`�v�Z</b>
  - #TU::Vector
  - #TU::Matrix
  - #TU::LUDecomposition
  - #TU::Householder
  - #TU::QRDecomposition
  - #TU::TriDiagonal
  - #TU::BiDiagonal
  - #TU::SVDecomposition
  - #TU::BlockDiagonalMatrix
  - #TU::SparseMatrix

  <b>����`�œK��</b>
  - #TU::NullConstraint
  - #TU::ConstNormConstraint
  - #TU::minimizeSquare(const F&, const G&, AT&, u_int, double)
  - #TU::minimizeSquareSparse(const F&, const G&, ATA&, IB, IB, u_int, double)

  <b>RANSAC</b>
  - #TU::ransac(const PointSet&, Model&, Conform, double)

  <b>�_�C�����C���ʓ��̊􉽗v�f�Ƃ��̕ϊ�</b>
  - #TU::Point1
  - #TU::Point2
  - #TU::Point3
  - #TU::HyperPlane
  - #TU::Normalize
  - #TU::Projectivity
  - #TU::Affinity
  - #TU::Homography
  - #TU::Affinity2
  - #TU::BoundingBox
  
  <b>���e�̊�</b>
  - #TU::IntrinsicBase
  - #TU::IntrinsicWithFocalLength
  - #TU::IntrinsicWithEuclideanImagePlane
  - #TU::Intrinsic
  - #TU::IntrinsicWithDistortion
  - #TU::CanonicalCamera
  - #TU::Camera
  
  <b>��f�Ɖ摜</b>
  - #TU::RGB
  - #TU::BGR
  - #TU::RGBA
  - #TU::ABGR
  - #TU::ARGB
  - #TU::BGRA
  - #TU::YUV444
  - #TU::YUV422
  - #TU::YUV411
  - #TU::ImageLine
  - #TU::Image
  - #TU::GenericImage
  - #TU::Movie

  <b>�摜����</b>
  - #TU::EdgeDetector
  - #TU::CorrectIntensity
  - #TU::Warp
  - #TU::IntegralImage
  - #TU::DiagonalIntegralImage
  
  <b>�摜�Ɍ���Ȃ��M������</b>
  - #TU::IIRFilter
  - #TU::BilateralIIRFilter
  - #TU::BilateralIIRFilter2
  - #TU::DericheConvolver
  - #TU::DericheConvolver2
  - #TU::GaussianConvolver
  - #TU::GaussianConvolver2
  
  <b>����f�[�^�\��</b>
  - #TU::List
  - #TU::Heap
  - #TU::PSTree
  - #TU::NDTree
  
  <b>Bezier�Ȑ���Bezier�Ȗ�</b>
  - #TU::BezierCurve
  - #TU::BezierSurface
  
  <b>B-spline�Ȑ���B-spline�Ȗ�</b>
  - #TU::BSplineKnots
  - #TU::BSplineCurve
  - #TU::BSplineSurface
  
  <b>���b�V��</b>
  - #TU::Mesh

  <b>�W�����C�u�����̕⋭</b>
  - #std::min(const T&, const T&, const T&)
  - #std::min(const T&, const T&, const T&, const T&)
  - #std::max(const T&, const T&, const T&)
  - #std::max(const T&, const T&, const T&, const T&)
  - #TU::pull_if(Iter, Iter, Pred)
  - #TU::diff(const T&, const T&)
  - #TU::op3x3(Iterator begin, Iterator end, OP op)
  - #TU::mbr_iterator
  - #TU::skipl(std::istream&)
  - #TU::IOManip
  - #TU::IManip1
  - #TU::OManip1
  - #TU::IOManip1
  - #TU::IManip2
  - #TU::OManip2

  <b>�X�g���[��
  - #TU::fdistream
  - #TU::fdostream
  - #TU::fdstream
  
  <b>�V���A���C���^�t�F�[�X</b>
  - #TU::Serial
  - #TU::TriggerGenerator
  - #TU::PM16C_04
  - #TU::SHOT602

  <b>����������</b>
  - #TU::Random
  
  \file		types.h
  \brief	��{�I�ȃf�[�^�^���O���[�o���Ȗ��O��Ԃɒǉ�
*/
#ifndef __TUtypes_h
#define __TUtypes_h

#ifdef WIN32
#  ifdef _USRDLL
#    define __PORT	__declspec(dllexport)
#  else
#    define __PORT	__declspec(dllimport)
#  endif
#  define _USE_MATH_DEFINES	    // <math.h>��M_PI���̒�`��L����
#  define _CRT_SECURE_NO_WARNINGS   // C�W���֐��g�p���̌x����}��
#  define _CRT_NONSTDC_NO_DEPRECATE // POSIX�֐��g�p���̌x����}��
#  define _SCL_SECURE_NO_WARNINGS   // C++�W���֐��g�p���̌x����}��

typedef unsigned int		size_t;		//!< �z�񓙂̃T�C�Y��\���^
typedef unsigned char		u_char;		//!< �����Ȃ�8bit����
typedef unsigned short		u_short;	//!< �����Ȃ�16bit����
typedef unsigned int		u_int;		//!< �����Ȃ�32bit����
typedef unsigned long		u_long;		//!< �����Ȃ�32/64bit����
typedef signed char		int8_t;		//!< �����t��8bit����
typedef short			int16_t;	//!< �����t��16bit����
typedef int			int32_t;	//!< �����t��32bit����
typedef long long		int64_t;	//!< �����t��64bit����
typedef unsigned char		u_int8_t;	//!< �����Ȃ�8bit����
typedef unsigned short		u_int16_t;	//!< �����Ȃ�16bit����
typedef unsigned int		u_int32_t;	//!< �����Ȃ�32bit����
typedef unsigned long long	u_int64_t;	//!< �����Ȃ�64bit����
#else
#  define __PORT
#  include <sys/types.h>
#endif

typedef signed char		s_char;		//!< �����t��8bit����

#endif	/*  !__TUtypes_h	*/
