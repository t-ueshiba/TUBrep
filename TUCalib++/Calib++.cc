/*
 *  $Id: Calib++.cc,v 1.3 2007-01-31 05:42:44 ueshiba Exp $
 */
#include "TU/Calib++.h"

namespace TU
{
/************************************************************************
*  class MeasurementMatrix						*
************************************************************************/
//! $BJ#?t$N;kE@$+$iJ?LL%Q%?!<%s$r4QB,$7$F%+%a%i%-%c%j%V%l!<%7%g%s$r9T$&!%(B
/*!
  $B%o!<%k%I:BI87O$r;2>HJ?LL$K8GDj$7!$J?LL>e$NFCD'E@$NF1<!:BI8$r(B
  \f$[X_j, Y_j, 0, 1]^\top~(j=0,\ldots,P-1)\f$$B$H$9$k!%$3$l$rFbIt%Q%i%a!<%?(B
  $B$r8GDj$7$F;kE@(Bi$B$+$i4QB,$7$?A|$r(B\f$\TUud{u}{ij}=[u_{ij},v_{ij},1]^\top~
  (i=0,\ldots,F-1)\f$$B$H$9$k!%$3$l$i$N%G!<%?$h$j4QB,9TNs$r(B
  \f[
    \TUtvec{W}{} =
    \TUbeginarray{cccccc}
      X_0     & Y_0     & 1 & \TUtud{u}{00}    & \cdots & \TUtud{u}{0~F-1} \\
      \vdots  & \vdots  & \vdots & \vdots      &	& \vdots	   \\
      X_{P-1} & Y_{P-1} & 1 & \TUtud{u}{P-1~0} & \cdots & \TUtud{u}{P-1~F-1}
    \TUendarray
  \f]
  $B$H9=@.$9$k!%$3$N4QB,9TNs$KBP$7$FK\%"%k%4%j%:%`E,MQ$9$k$H!$%+%a%i$NFbIt(B
  $B%Q%i%a!<%?$*$h$S3F;kE@$G$N%+%a%i$N30It%Q%i%a!<%?$,7W;;$5$l$k!%(B
  \param cameras	$B3F;kE@$K$D$$$F!$;2>HJ?LL$K8GDj$5$l$?%o!<%k%I:BI87O(B
			$B$+$i8+$?%+%a%i$N30It%Q%i%a!<%?$,JV$5$l$k!%(B
  \param doRefinement	true$B$N>l9g!$Hs@~7?:GE,2=$K$h$C$FFbIt(B/$B30It%Q%i%a!<%?(B
			$B$r(Brefine$B$9$k!%(B
  \return		$B%+%a%i$NFbIt%Q%i%a!<%?!%(B
 */
template <class INTRINSIC> INTRINSIC
MeasurementMatrix::calibrateWithPlanes(Array<CanonicalCamera>& cameras,
				       bool doRefinement)
    const
{
    INTRINSIC	K(initializeCalibrationWithPlanes(cameras));
    if (doRefinement)
	refineCalibrationWithPlanes(K, cameras);

    return K;
}

//! $BJ?LL>e$N%Q%?!<%s$rJ#?t$N;kE@$+$i4QB,$7$?2hA|$NA|$rMQ$$$F!$%+%a%i$NFbIt%Q%i%a!<%?$H30It%Q%i%a!<%?$r(Brefine$B$9$k!%(B
/*!
  $B4QB,9TNs$N7A<0$K$D$$$F$O(BcalibrateWithPlanes()$B$r;2>H!%(B
  \param K	 $B%+%a%i$NFbIt%Q%i%a!<%?$N=i4|CM$rM?$($k!%$=$N:GE,?dDjCM$,JV(B
		 $B$5$l$k!%(B
  \param cameras $B3F;kE@$K$D$$$F!$;2>HJ?LL$K8GDj$5$l$?%o!<%k%I:BI87O$+$i8+$?(B
		 $B%+%a%i$N30It%Q%i%a!<%?$N=i4|CM$rM?$($k!%$=$N:GE,?dDjCM$,(B
		 $BJV$5$l$k!%(B
 */
template <class INTRINSIC> void
MeasurementMatrix::refineCalibrationWithPlanes
    (INTRINSIC& K, Array<CanonicalCamera>& cameras) const
{
    if (nframes() < 3)
	throw std::invalid_argument("TU::MeasurementMatrix::refineCalibrationWithPlanes: Two or more frames required!!");

    CostCP		err(*this, K.dof());
    NullConstraint<ET>	g;

    minimizeSquareSparse(err, g, K, cameras, 200);
}

//! $B%+%a%i$NA4%Q%i%a!<%?$HFCD'E@0LCV$N=i4|CM$rHs@~7?:GE,2=$K$h$j(Brefine$B$9$k!%(B
/*!
  \param cameras	$B8D!9$N%+%a%i$NA4%Q%i%a!<%?$N=i4|CM$rM?$($k!%$=$N:G(B
			$BE,?dDjCM$,JV$5$l$k!%(B
  \param Xt		$BFCD'E@$N%f!<%/%j%C%I:BI87O$K$*$1$kF1<!:BI8$N=i4|CM$r(B
			$BM?$($k!%$=$N:GE,?dDjCM$,JV$5$l$k!%(B
*/
template <class CAMERA> void
MeasurementMatrix::bundleAdjustment(Array<CAMERA>& cameras,
				    Matrix<ET>& Xt) const
{
    if (nframes() < 2)
	throw std::invalid_argument("TU::MeasurementMatrix::bundleAdjustment: Two or more frames required!!");

    CostBA<CAMERA>			err(*this, cameras[0].dofIntrinsic());
    typename CostBA<CAMERA>::CostCD	g(cameras);
    Matrix<ET>				shape(Xt, 0, 0, Xt.nrow(), 3);

    minimizeSquareSparse(err, g, cameras, shape, 200);
}

//! $B%+%a%i$N30It%Q%i%a!<%?!$A4$F$N%+%a%i$K6&DL$J>GE@5wN%$*$h$SFCD'E@0LCV$N=i4|CM$rHs@~7?:GE,2=$K$h$j(Brefine$B$9$k!%(B
/*!
  \param K		$BA4$F$N%+%a%i$K6&DL$J>GE@5wN%$N=i4|CM$rM?$($k!%$=$N:G(B
			$BE,?dDjCM$,JV$5$l$k!%(B
  \param cameras	$B8D!9$N%+%a%i$N30It%Q%i%a!<%?$N=i4|CM$rM?$($k!%$=$N:G(B
			$BE,?dDjCM$,JV$5$l$k!%(B
  \param Xt		$BFCD'E@$N%f!<%/%j%C%I:BI87O$K$*$1$kF1<!:BI8$N=i4|CM$r(B
			$BM?$($k!%$=$N:GE,?dDjCM$,JV$5$l$k!%(B
*/
template <class INTRINSIC> void
MeasurementMatrix::bundleAdjustment(INTRINSIC& K,
				    Array<CanonicalCamera>& cameras,
				    Matrix<ET>& Xt) const
{
    if (nframes() < 2)
	throw std::invalid_argument("TU::MeasurementMatrix::bundleAdjustment (with common intrinsic parameters estimation): Two or more frames required!!");

    CostBACI<INTRINSIC>				err(*this);
    typename CostBACI<INTRINSIC>::ATA		params(K, cameras);
    typename CostBACI<INTRINSIC>::CostCD	g(params);
    Matrix<ET>					shape(Xt, 0, 0, Xt.nrow(), 3);

    minimizeSquareSparse(err, g, params, shape, 200);

    K = params.K;
    for (int i = 0; i < cameras.dim(); ++i)
	cameras[i] = params[i];
}

//! $B%+%a%i$N0LCV$,ITJQ$H$N2>Dj$N$b$H$G!$8D!9$N%+%a%i$NA4%Q%i%a!<%?$*$h$SFCD'E@0LCV$N=i4|CM$rHs@~7?:GE,2=$K$h$j(Brefine$B$9$k!%(B
/*!
  \param cameras	$B8D!9$N%+%a%i$NA4%Q%i%a!<%?$N=i4|CM$rM?$($k!%$=$N:G(B
			$BE,?dDjCM$,JV$5$l$k!%(B
  \param Xt		$BFCD'E@$N%f!<%/%j%C%I:BI87O$K$*$1$kF1<!:BI8$N=i4|CM$r(B
			$BM?$($k!%$=$N:GE,?dDjCM$,JV$5$l$k!%(B
*/
template <class CAMERA> void
MeasurementMatrix::bundleAdjustmentWithFixedCameraCenters
	(Array<CAMERA>& cameras, Matrix<ET>& Xt) const
{
    if (nframes() < 3)
	throw std::invalid_argument("TU::MeasurementMatrix::bundleAdjustmentWithFixedCameraCenters: Three or more frames required!!");

    CostBA<CAMERA>	err(*this, cameras[0].dofIntrinsic(), true);
    NullConstraint<ET>	g;
    Matrix<ET>		shape(Xt, 0, 0, Xt.nrow(), 3);

    minimizeSquareSparse(err, g, cameras, shape, 200);
}

/************************************************************************
*  class MeasurementMatrix::CostBA<CAMERA>				*
************************************************************************/
template <class CAMERA>
MeasurementMatrix::CostBA<CAMERA>::CostBA(const MeasurementMatrix& Wt,
					  u_int dofIntrinsic,
					  bool fixCameraCenter)
    :_Wt(Wt), _fcc(fixCameraCenter), _adim(0), _adims(nframes())
{
    _adims[0] = dofIntrinsic + (_fcc ? 3 : 0);
    _adim += _adims[0];
    for (int i = 1; i < _adims.dim(); ++i)
    {
	_adims[i] = dofIntrinsic + (_fcc ? 3 : 6);
	_adim += _adims[i];
    }
}

template <class CAMERA> Vector<double>
MeasurementMatrix::CostBA<CAMERA>::operator ()(const ATA& p,
					       const ATB& x, int j) const
{
    Vector<ET>	val(2 * nframes());
    for (int i = 0; i < nframes(); ++i)
    {
	const Point2<ET>&	u = p[i](x);
	val[2*i]   = u[0] - _Wt[j][3*i];
	val[2*i+1] = u[1] - _Wt[j][3*i+1];
    }

    return val;
}

template <class CAMERA> BlockMatrix<double>
MeasurementMatrix::CostBA<CAMERA>::jacobianA(const ATA& p,
					     const ATB& x, int j) const
{
    BlockMatrix<ET>	J(nframes());
    J[0] = (_fcc ? p[0].jacobianFCC(x) : p[0].jacobianK(x));
    for (int i = 1; i < nframes(); ++i)
	J[i] = (_fcc ? p[i].jacobianFCC(x) : p[i].jacobianP(x));
    
    return J;
}

template <class CAMERA> Matrix<double>
MeasurementMatrix::CostBA<CAMERA>::jacobianB(const ATA& p,
					     const ATB& x, int j) const
{
    Matrix<ET>	K(2 * nframes(), 3);
    for (int i = 0; i < nframes(); ++i)
	K(2*i, 0, 2, K.ncol()) = p[i].jacobianX(x);

    return K;
}

template <class CAMERA> void
MeasurementMatrix::CostBA<CAMERA>::updateA(ATA& p, const Vector<ET>& dp) const
{
    int	d = 0;
    if (_fcc)
	p[0].updateFCC(dp(d, _adims[0]));
    else
	p[0].updateIntrinsic(dp(d, _adims[0]));
    d += _adims[0];
    for (int i = 1; i < _adims.dim(); ++i)
    {
      // Update camera parameters.
	if (_fcc)
	    p[i].updateFCC(dp(d, _adims[i]));
	else
	    p[i].update(dp(d, _adims[i]));
	d += _adims[i];
    }

    if (!_fcc)
    {
      // Force the distance between 0th and 1st cameras keep constant.
	Vector<ET>	t = p[1].t() - p[0].t();
	t.normalize() *= p[0].t().dist(p[1].t());
	p[1].setTranslation(p[0].t() + t);
    }
}

template <class CAMERA> void
MeasurementMatrix::CostBA<CAMERA>::updateB(ATB& x, const Vector<ET>& dx) const
{
    x -= dx;
}

/************************************************************************
*  class MeasurementMatrix::CostBACI<INTRINSIC>				*
************************************************************************/
template <class INTRINSIC> Vector<double>
MeasurementMatrix::CostBACI<INTRINSIC>::operator ()(const ATA& p,
						    const ATB& x, int j) const
{
    Vector<ET>	val(2 * nframes());
    for (int i = 0; i < nframes(); ++i)
    {
	const Point2<ET>&	u = p.K(p[i].xc(x));
	val[2*i]   = u[0] - _Wt[j][3*i];
	val[2*i+1] = u[1] - _Wt[j][3*i+1];
    }

    return val;
}

template <class INTRINSIC> Matrix<double>
MeasurementMatrix::CostBACI<INTRINSIC>::jacobianA(const ATA& p,
						  const ATB& x, int j) const
{
    Matrix<ET>	J(2 * nframes(), adim());
    J(0, 0, 2, p.K.dof()) = p.K.jacobianK(p[0].xc(x));
    for (int i = 1; i < nframes(); ++i)
    {
	const Point2<ET>& xc = p[i].xc(x);
	J(2*i, 0, 2, p.K.dof()) = p.K.jacobianK(xc);
	J(2*i, p.K.dof() + 6*(i-1), 2, 6)
	    = p.K.jacobianXC(xc) * p[i].jacobianPc(x);
    }
    
    return J;
}

template <class INTRINSIC> Matrix<double>
MeasurementMatrix::CostBACI<INTRINSIC>::jacobianB(const ATA& p,
						  const ATB& x, int j) const
{
    Matrix<ET>	K(2 * nframes(), 3);
    for (int i = 0; i < nframes(); ++i)
	K(2*i, 0, 2, K.ncol())
	    = p.K.jacobianXC(p[i].xc(x)) * p[i].jacobianXc(x);

    return K;
}

template <class INTRINSIC> void
MeasurementMatrix::CostBACI<INTRINSIC>::updateA(ATA& p,
						const Vector<ET>& dp) const
{
    p.K.update(dp(0, p.K.dof()));
    for (int i = 1; i < nframes(); ++i)
	p[i].update(dp(p.K.dof() + 6*(i-1), 6));

  // Force the distance between 0th and 1st cameras keep constant.
    Vector<ET>	t = p[1].t() - p[0].t();
    t.normalize() *= (p[0].t().dist(p[1].t()));
    p[1].setTranslation(p[0].t() + t);
}

template <class INTRINSIC> void
MeasurementMatrix::CostBACI<INTRINSIC>::updateB(ATB& x,
						const Vector<ET>& dx) const
{
    x -= dx;
}
 
}
