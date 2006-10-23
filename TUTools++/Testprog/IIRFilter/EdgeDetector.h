/*
 *  $Id: EdgeDetector.h,v 1.1 2006-10-23 01:22:31 ueshiba Exp $
 */
#ifndef __TUEdgeDetector_h
#define __TUEdgeDetector_h

#include "TU/Image++.h"

namespace TU
{
/************************************************************************
*  class EdgeDetector							*
************************************************************************/
/*!
  �G�b�W���o���s���N���X.
*/ 
class EdgeDetector
{
  public:
    enum	{TRACED = 0x4, EDGE = 0x02, WEAK = 0x01};
    
    EdgeDetector(float th_low=10.0, float th_high=20.0)			;
    
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

  private:
    float		_th_low, _th_high;
};

//! �G�b�W���o��𐶐�����
/*!
  \param th_low		�ア�G�b�W��臒l.
  \param th_low		�����G�b�W��臒l.
*/
inline
EdgeDetector::EdgeDetector(float th_low, float th_high)
{
    initialize(th_low, th_high);
}

//! �G�b�W���o��臒l��ݒ肷��
/*!
  \param th_low		�ア�G�b�W��臒l.
  \param th_low		�����G�b�W��臒l.
  \return		���̃G�b�W���o�펩�g.
*/
inline EdgeDetector&
EdgeDetector::initialize(float th_low, float th_high)
{
    _th_low  = th_low;
    _th_high = th_high;

    return *this;
}
    
}

#endif	// !__TUEdgeDetector_h
