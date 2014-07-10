/*
 *  $Id$
 */
/*!
  \file		Ieee1394CameraArray.h
  \brief	クラス TU::Ieee1394CameraArray の定義と実装
*/
#ifndef __TU_IEEE1394CAMERAARRAY_H
#define __TU_IEEE1394CAMERAARRAY_H

#ifdef HAVE_LIBTUTOOLS__
#  include "TU/Array++.h"
#  include "TU/Ieee1394++.h"
#  include "TU/io.h"

//! デフォルトのカメラ名
#  define DEFAULT_CAMERA_NAME	"IEEE1394Camera"
//! カメラ設定ファイルを収めるデフォルトのディレクトリ名
#  define DEFAULT_CONFIG_DIRS	".:/usr/local/etc/cameras"

namespace TU
{
/************************************************************************
*  class Ieee1394CameraArray						*
************************************************************************/
//! IEEE1394デジタルカメラの配列を表すクラス
/*!
  TU::Ieee1394Cameraへのポインタの配列として定義される.
*/
class Ieee1394CameraArray : public Array<Ieee1394Camera*>
{
  public:
    Ieee1394CameraArray()						;
    Ieee1394CameraArray(const char* name, const char* dirs=0,
			Ieee1394Node::Speed speed=Ieee1394Node::SPD_400M,
			int ncameras=-1)				;
    ~Ieee1394CameraArray()						;

    void		initialize(const char* name, const char* dirs=0,
				   Ieee1394Node::Speed
				       speed=Ieee1394Node::SPD_400M,
				   int ncameras=-1)			;
    const std::string&	fullName()				const	;
    std::string		configFile()				const	;
    std::string		calibFile()				const	;

  private:
    std::string		_fullName;	//!< カメラのfull path名
};

//! カメラのfull path名を返す.
/*!
  \return	カメラのfull path名
*/
inline const std::string&
Ieee1394CameraArray::fullName() const
{
    return _fullName;
}
    
//! カメラ設定ファイル名を返す.
/*!
  \return	カメラ設定ファイル名
*/
inline std::string
Ieee1394CameraArray::configFile() const
{
    return _fullName + ".conf";
}
    
//! キャリブレーションファイル名を返す.
/*!
  \return	キャリブレーションファイル名
*/
inline std::string
Ieee1394CameraArray::calibFile() const
{
    return _fullName + ".calib";
}
    
}
#endif	// HAVE_LIBTUTOOLS__
#endif	// ! __TU_IEEE1394CAMERAARRAY_H
