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
 *  $Id: ImageBase.cc,v 1.25 2009-05-10 23:33:22 ueshiba Exp $
 */
#include "TU/Image++.h"
#include "TU/Camera.h"
#include "TU/Manip.h"

namespace TU
{
/************************************************************************
*  static functions							*
************************************************************************/
inline static u_int	bit2byte(u_int i)	{return ((i - 1)/8 + 1);}

/************************************************************************
*  class ImageBase							*
************************************************************************/
ImageBase::~ImageBase()
{
}

//! $BF~NO%9%H%j!<%`$+$i2hA|$N%X%C%@$rFI$_9~$`!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \return	$BFI$_9~$^$l$?2hA|$N2hAG$N%?%$%W(B
*/
ImageBase::Type
ImageBase::restoreHeader(std::istream& in)
{
    using namespace	std;

  // Reset calibration parameters.
    P = 0.0;
    P[0][0] = P[1][1] = P[2][2] = 1.0;
    d1 = d2 = 0.0;
    
  // Read the magic number.
    int	c = in.get();
    if (c == EOF)
	return DEFAULT;
    if (c != 'P')
	throw runtime_error("TU::ImageBase::restoreHeader: not a pbm file!!");

  // Read pbm type.
    in >> c >> ws;	// Read pbm type and trailing white spaces.
    Type	type;
    switch (c)
    {
      case U_CHAR:
	type = U_CHAR;
	break;
      case RGB_24:
	type = RGB_24;
	break;
      default:
	throw runtime_error("TU::ImageBase::restoreHeader: unknown pbm type!!");
    }

  // Process comment lines.
    bool	legacy = false;	// legacy style of dist. param. representation
    for (; (c = in.get()) == '#'; in >> ign)
    {
	char	key[256], val[256];
	in >> key;
	if (!strcmp(key, "DataType:"))		// pixel data type
	{
	    in >> val;
	    if (!strcmp(val, "Short"))
		type = SHORT;
	    else if (!strcmp(val, "Int"))
		type = INT;
	    else if (!strcmp(val, "Float"))
		type = FLOAT;
	    else if (!strcmp(val, "Double"))
		type = DOUBLE;
	    else if (!strcmp(val, "YUV444"))
		type = YUV_444;
	    else if (!strcmp(val, "YUV422"))
		type = YUV_422;
	    else if (!strcmp(val, "YUV411"))
		type = YUV_411;
	}
	else if (!strcmp(key, "Endian:"))	// big- or little-endian
	{
	    in >> val;
	    switch (type)
	    {
	      case SHORT:
	      case INT:
	      case FLOAT:
	      case DOUBLE:
#ifdef TU_BIG_ENDIAN
		if (!strcmp(val, "Little"))
		    throw runtime_error("TU::ImageBase::restore_epbm: big endian is not supported!!");
#else
		if (!strcmp(val, "Big"))
		    throw runtime_error("TU::ImageBase::restore_epbm: little endian is not supported!!");
#endif
		break;
	    }
	}
	else if (!strcmp(key, "PinHoleParameterH11:"))
	    in >> P[0][0];
	else if (!strcmp(key, "PinHoleParameterH12:"))
	    in >> P[0][1];
	else if (!strcmp(key, "PinHoleParameterH13:"))
	    in >> P[0][2];
	else if (!strcmp(key, "PinHoleParameterH14:"))
	    in >> P[0][3];
	else if (!strcmp(key, "PinHoleParameterH21:"))
	    in >> P[1][0];
	else if (!strcmp(key, "PinHoleParameterH22:"))
	    in >> P[1][1];
	else if (!strcmp(key, "PinHoleParameterH23:"))
	    in >> P[1][2];
	else if (!strcmp(key, "PinHoleParameterH24:"))
	    in >> P[1][3];
	else if (!strcmp(key, "PinHoleParameterH31:"))
	    in >> P[2][0];
	else if (!strcmp(key, "PinHoleParameterH32:"))
	    in >> P[2][1];
	else if (!strcmp(key, "PinHoleParameterH33:"))
	    in >> P[2][2];
	else if (!strcmp(key, "PinHoleParameterH34:"))
	    in >> P[2][3];
	else if (!strcmp(key, "DistortionParameterD1:"))
	{
	    in >> d1;
	    legacy = false;
	}
	else if (!strcmp(key, "DistortionParameterD2:"))
	{
	    in >> d2;
	    legacy = false;
	}
	else if (!strcmp(key, "DistortionParameterA:"))	// legacy dist. param.
	{
	    in >> d1;
	    legacy = true;
	}
	else if (!strcmp(key, "DistortionParameterB:"))	// legacy dist. param.
	{
	    in >> d2;
	    legacy = true;
	}
    }
    in.putback(c);

    if (legacy)
    {
	Camera	camera(P);
	double	k = camera.k();
	d1 *= (k * k);
	d2 *= (k * k * k * k);
    }

    u_int	w, h;
    in >> w;
    in >> h;
    _resize(h, w, type);			// set width & height
    in >> w >> ign;				// skip MaxValue

    return type;
}

//! $B;XDj$7$?2hAG%?%$%W$G=PNO%9%H%j!<%`$K2hA|$N%X%C%@$r=q$-=P$9!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \param type	$B2hAG%?%$%W!%$?$@$7!$(B#DEFAULT$B$r;XDj$7$?>l9g$O!$(B
		$B$3$N2hA|%*%V%8%'%/%H$N2hAG%?%$%W$G=q$-=P$5$l$k!%(B
  \return	$B<B:]$K=q$-=P$9>l9g$N2hAG%?%$%W!%(B
*/
ImageBase::Type
ImageBase::saveHeader(std::ostream& out, Type type) const
{
    using namespace	std;

    if (type == DEFAULT)
	type = _defaultType();
    
    out << 'P';
    switch (type)
    {
      case RGB_24:
	out << int(RGB_24) << endl;
	break;
      default:
	out << int(U_CHAR) << endl;
	break;
    }

    const u_int	depth = type2depth(type);
    out << "# PixelLength: " << bit2byte(depth) << endl;
    out << "# DataType: ";
    switch (type)
    {
      default:
	out << "Char" << endl;
	break;
      case RGB_24:
	out << "RGB24" << endl;
	break;
      case SHORT:
	out << "Short" << endl;
	break;
      case INT:
	out << "Int" << endl;
	break;
      case FLOAT:
	out << "Float" << endl;
	break;
      case DOUBLE:
	out << "Double" << endl;
	break;
      case YUV_444:
	out << "YUV444" << endl;
	break;
      case YUV_422:
	out << "YUV422" << endl;
	break;
      case YUV_411:
	out << "YUV411" << endl;
	break;
    }
    out << "# Sign: ";
    switch (type)
    {
      case SHORT:
      case INT:
      case FLOAT:
      case DOUBLE:
	out << "Signed" << endl;
	break;
      default:
	out << "Unsigned" << endl;
	break;
    }
#ifdef TU_BIG_ENDIAN
    out << "# Endian: Big" << endl;
#else
    out << "# Endian: Little" << endl;
#endif
    out << "# PinHoleParameterH11: " << P[0][0] << endl
	<< "# PinHoleParameterH12: " << P[0][1] << endl
	<< "# PinHoleParameterH13: " << P[0][2] << endl
	<< "# PinHoleParameterH14: " << P[0][3] << endl
	<< "# PinHoleParameterH21: " << P[1][0] << endl
	<< "# PinHoleParameterH22: " << P[1][1] << endl
	<< "# PinHoleParameterH23: " << P[1][2] << endl
	<< "# PinHoleParameterH24: " << P[1][3] << endl
	<< "# PinHoleParameterH31: " << P[2][0] << endl
	<< "# PinHoleParameterH32: " << P[2][1] << endl
	<< "# PinHoleParameterH33: " << P[2][2] << endl
	<< "# PinHoleParameterH34: " << P[2][3] << endl;
    if (d1 != 0.0 || d2 != 0.0)
	out << "# DistortionParameterD1: " << d1 << endl
	    << "# DistortionParameterD2: " << d2 << endl;
    out << _width() << ' ' << _height() << '\n'
	<< 255 << endl;
    
    return type;
}

//! $B;XDj$5$l$?%?%$%W$N2hAG$N%S%C%H?t$rJV$9!%(B
/*!
  \param type	$B2hAG$N%?%$%W(B
  \return	$B2hAG$N%S%C%H?t(B
*/
u_int
ImageBase::type2depth(Type type)
{
    switch (type)
    {
      case SHORT:
	return 8*sizeof(short);
      case INT:
	return 8*sizeof(int);
      case FLOAT:
	return 8*sizeof(float);
      case DOUBLE:
	return 8*sizeof(double);
      case RGB_24:
      case YUV_444:
	return 24;
      case YUV_422:
	return 16;
      case YUV_411:
	return 12;
    }

    return 8;
}
 
}
