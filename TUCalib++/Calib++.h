/*
 *  $BJ?@.(B9$BG/(B $BEE;R5;=QAm9g8&5f=j(B $B?"<G=SIW(B $BCx:n8"=jM-(B
 *
 *  $BCx:n<T$K$h$k5v2D$J$7$K$3$N%W%m%0%i%`$NBh;0<T$X$N3+<(!"J#@=!"2~JQ!"(B
 *  $B;HMQEy$=$NB>$NCx:n?M3J8"$r?/32$9$k9T0Y$r6X;_$7$^$9!#(B
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *
 *  Copyright 1996
 *  Toshio UESHIBA, Electrotechnical Laboratory
 *
 *  All rights reserved.
 *  Any changing, copying or giving information about source programs of
 *  any part of this software and/or documentation without permission of the
 *  authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damage in use of this program.
 */

/*
 *  $Id: Calib++.h,v 1.7 2003-03-17 00:49:53 ueshiba Exp $
 */
#ifndef __TUCalibPP_h
#define __TUCalibPP_h

#include "TU/Geometry++.h"
#include "TU/BlockMatrix++.h"
#include "TU/Minimize++.h"

/*!
  \mainpage	libTUCalib++ - $B%+%a%i%-%c%j%V%l!<%7%g%sMQ%i%$%V%i%j(B

  libTUCalib++$B$O!$%+%a%i$NFbIt%Q%i%a!<%?$*$h$S30It%Q%i%a!<%?$r5a$a$k(B
  $B$?$a$KI,MQ$JMM!9$J%"%k%4%j%:%`$r<BAu$7$?%-%c%j%V%l!<%7%g%s%i%$%V%i%j(B
  $B$G$"$k!%(B
*/

namespace TU
{
/************************************************************************
*  class Normalization						*
************************************************************************/
/*!
  $BF1<!:BI8$N@55,2=JQ49$rI=$9%/%i%9!%(B\f$\TUud{x}{}=[\TUtvec{x}{}, 1]^\top~
  (\TUvec{x}{} \in \TUspace{R}{d})\f$$B$KBP$7$F!$0J2<$N$h$&$JJ?9T0\F0$H(B
  $B%9%1!<%j%s%0$r9T$&JQ49$rI=8=$9$k(B:
  \f[
	\TUud{y}{} =
	\TUbeginarray{c} s^{-1}(\TUvec{x}{} - \TUvec{c}{}) \\ 1	\TUendarray =
	\TUbeginarray{ccc}
	  s^{-1} \TUvec{I}{d} & -s^{-1}\TUvec{c}{} \\ \TUtvec{0}{d} & 1
	\TUendarray
	\TUbeginarray{c} \TUvec{x}{} \\ 1 \TUendarray =
	\TUvec{T}{}\TUud{x}{}
  \f]
*/
class Normalization
{
  public:
  //! $B@55,2=JQ49%*%V%8%'%/%H$r=i4|2=$9$k!%(B
  /*!
    initialize()$B$r9T$o$J$$$H;HMQ$G$-$J$$!%(B
  */
    Normalization()	:_scale(1.0), _centroid()	{}

  //! $BM?$($i$l$?E@72$NF1<!:BI8$+$i7W;;$7$?@55,2=JQ49$G=i4|2=$9$k!%(B
  /*!
    $B6qBNE*$K$I$N$h$&$JJQ49$,7W;;$5$l$k$+$O(Binitialize()$B$r;2>H!%(B
    \param points	$BE@72$NF1<!:BI8!%E@$N8D?t$r(BN, $BE@$,B0$9$k6u4V$N<!85$r(Bd
			$B$H$9$k$H!$(BN\f$\times\f$(d+1)$B9TNs$H$7$FM?$($i$l$k!%(B
  */
    Normalization(const Matrix<double>& points)
	:_scale(0.0), _centroid(points.ncol()-1)	{initialize(points);}

    void		initialize(const Matrix<double>& points)	;

    u_int		dim()					  const	;
    Matrix<double>	T()					  const	;
    Matrix<double>	Tt()					  const	;
    Matrix<double>	Tinv()					  const	;
    Matrix<double>	Ttinv()					  const	;

  private:
    double		_scale;
    Vector<double>	_centroid;
};

//! $B$3$N@55,2=JQ49$,E,MQ$5$l$k6u4V$N<!85$rJV$9!%(B
/*! 
  \return	$B6u4V$N<!85(B($BJV$5$l$kCM$r(Bd$B$H$9$k$H!$F1;~:BI8$N%Y%/%H%k(B
		$B$H$7$F$N<!85$O(Bd+1)$B!%(B
*/
inline u_int
Normalization::dim() const
{
    return _centroid.dim();
}

/************************************************************************
*  class MeasurementMatrix						*
************************************************************************/
/*!
  $BJ#?t$N;kE@$+$i4QB,$5$l$?J#?t$NFCD'E@$NA|$+$i9=@.$5$l$k4QB,9TNs$rI=$9%/%i%9!%(B
  $B;kE@(Bi$B$+$i4QB,$7$?(Bj$BHVL\$NE@$NA|$r(B\f$\TUud{u}{ij} = [u_{ij}, v_{ij}, 1]^\top
  ~(i=0,\ldots,F-1; j=0,\ldots,P-1)\f$$B$H$9$k(B($B%U%l!<%`?t(BF, $BFCD'E@?t(BP)$B$H!$(B
  \f[
	\TUtvec{W}{} =
	\TUbeginarray{ccc}
	\TUtud{u}{00} & \cdots & \TUtud{u}{F-1~1} \\ \vdots & & \vdots \\
	\TUtud{u}{0~P-1} & \cdots & \TUtud{u}{F-1~P-1}
	\TUendarray
  \f]
  $B$HDj5A$5$l$k!%(B
*/
class MeasurementMatrix : public Matrix<double>
{
  public:
    typedef double	T;			//!< $B%Y%/%H%k(B, $B9TNs$NMWAG$N7?(B

  //! $B4QB,9TNs$r(B\f$0\times 0\f$$B9TNs(B($B%U%l!<%`?t$HFCD'E@?t$,6&$K(B0)$B$H$7$F=i4|2=!%(B
    MeasurementMatrix()	:Matrix<T>()			{}
    MeasurementMatrix(const MeasurementMatrix& Wt,
		      const Array<u_int>&      index)	;

  //! $B4QB,9TNs$K4^$^$l$k%U%l!<%`?t$rJV$9!%(B
  /*!
    \return	$B%U%l!<%`?t(B($B4QB,9TNs$NNs?t$N(B1/3)$B!%(B
  */
    u_int	nframes()			const	{return ncol()/3;}

  //! $B4QB,9TNs$K4^$^$l$kFCD'E@?t$rJV$9!%(B
  /*!
    \return	$BFCD'E@?t(B($B4QB,9TNs$N9T?t(B)$B!%(B
  */
    u_int		npoints()		const	{return nrow();}

    Vector<T>		centroid()		const	;
    const Matrix<T>	frame(u_int i)		const	;
    Matrix<T>		frame(u_int i)			;

    
    Matrix<T>	affineFundamental(u_int frame0=0, u_int frame1=1) const	;
    Matrix<T>	fundamental(u_int frame0=0, u_int frame1=1)	const	;
    Matrix<T>	homography(u_int frame0=0, u_int frame1=1)	const	;
    Matrix<T>	rotation(u_int frame0=0, u_int frame1=1)	const	;

    template <class INTRINSIC> INTRINSIC
		calibrateWithPlanes
			(Array<CanonicalCamera>& cameras)	const	;
    void	affineFactorization(Matrix<T>& P,
				    Matrix<T>& Xt)		const	;
    static void	affineToMetric(Matrix<T>& P,
			       Matrix<T>& Xt)				;
    void	projectiveFactorization(Matrix<T>& P,
					Matrix<T>& Xt)		const	;
    void	projectiveToMetric(Matrix<T>& P,
				   Matrix<T>& Xt)		const	;
    void	projectiveToMetricWithFocalLengthsEstimation
		    (Matrix<T>& P, Matrix<T>& Xt)		const	;
    void	projectiveToMetricWithCommonFocalLengthEstimation
		    (Matrix<T>& P, Matrix<T>& Xt)		const	;

    template <class INTRINSIC>
    void	refineCalibrationWithPlanes
				(INTRINSIC& K,
				 Array<CanonicalCamera>& cameras)
								const	;
    template <class CAMERA>
    void	bundleAdjustment(Array<CAMERA>& cameras,
				 Matrix<T>& Xt)			const	;
    template <class INTRINSIC>
    void	bundleAdjustment(INTRINSIC& K,
				 Array<CanonicalCamera>& cameras,
				 Matrix<T>& Xt)			const	;
    template <class CAMERA>
    void	bundleAdjustmentWithFixedCameraCenters
				(Array<CAMERA>& cameras,
				 Matrix<T>& Xt)			const	;
    Matrix<T>	reconstruction(const Matrix<T>& P,
			       bool inhomogeneous=false)	const	;
    T		assessFundamental(const Matrix<T>& F,
				  u_int frame0=0, u_int frame1=1) const	;
    T		assessHomography(const Matrix<T>& H,
				 u_int frame0=0, u_int frame1=1)  const	;
    T		assessError(const Matrix<T>& P)			  const	;
    
    friend std::istream&
		operator >>(std::istream& in, MeasurementMatrix& Wt)	;

  private:
    std::istream&	get(std::istream& in, int j, u_int nfrms)	;
    Matrix<T>	initializeCalibrationWithPlanes
			    (Array<CanonicalCamera>& cameras)	const	;
    void	initializeFocalLengthsEstimation(Matrix<T>& P,
						 Matrix<T>& Xt) const	;

    class CostH		// cost function for homography estimation.
    {
      public:
	typedef double		T;
	typedef Vector<T>	AT;

	class CostCN	// cost function for keeping norm of H constant.
	{
	  public:
	    CostCN(const AT& h)	:_sqr(h.square())			{}

	    Vector<T>	operator ()(const AT& h) const
			{
			    Vector<T>	val(1);
			    val[0] = h.square() - _sqr;
			    return val;
			}
	    Matrix<T>	jacobian(const AT& h) const
			{
			    Matrix<T>	L(1, h.dim());
			    (L[0] = h) *= 2.0;
			    return L;
			}
	
	  private:
	    const T	_sqr;
	};
	
      public:
	CostH(const MeasurementMatrix& Wt, u_int frame0, u_int frame1)
	    :_Wt(Wt), _frame0(frame0), _frame1(frame1)			{}

	Vector<T>	operator ()(const AT& h)		const	;
	Matrix<T>	jacobian(const AT& h)			const	;
	void		update(AT& h, const Vector<T>& dh)	const	;
	u_int		npoints()		const	{return _Wt.npoints();}
	
      private:
	const MeasurementMatrix&	_Wt;
	const u_int			_frame0, _frame1;
    };
    
    class CostPF	// cost function for projective factorization.
    {
      private:
	enum		{DEFAULT_NITER_MAX = 50};
    
      public:
	CostPF(const Matrix<double>& Wt0, u_int niter_max=DEFAULT_NITER_MAX);
    
	double			minimize(Vector<double>& mu)		;
    
	u_int			nframes()	const	{return _Wt0.ncol()/3;}
	u_int			npoints()	const	{return _Wt0.nrow();}
	u_int			N()		const	{return _Wt0.ncol();}
	const Vector<double>&	s()		const	{return _s;}
	const Matrix<double>&	Ut()		const	{return _Ut;}
	const Matrix<double>&	Vt()		const	{return _Vt;}
    
      private:
	int			frame_index(u_int p)		const	;
	int			point_index(u_int p)		const	;
	double			operator ()(const Vector<double>& mu)	;
	Matrix<double>		S(int m)			const	;
	Matrix<double>		T(int m)			const	;
	void			update(const Vector<double>& mu)	;
	void			print(int i, double val,
				      const Vector<double>& mu) const	;
    
	const u_int		_niter_max;
	const Matrix<double>&	_Wt0;	// initial measurement matrix: W
	Vector<double>		_s;	// singular values of W
	Matrix<double>		_Ut;	// right basis of SVD
	Matrix<double>		_Vt;	// left basis of SVD
    };

    class CostPM	// cost function for projective to metric conversion.
    {
      public:
      	typedef double		T;
	typedef Vector<T>	AT;

	CostPM(const Matrix<T>& A, const Vector<T>& b) :_AA(A), _b(b) {}
	
	Vector<T>	operator ()(const AT& p)	const	;
	Matrix<T>	jacobian(const AT& p)		const	;
	void		update(AT& p,
			       const Vector<T>& dp)	const	{p -= dp;}

      private:
	const Matrix<T>&	_AA;
	const Vector<T>&	_b;
    };

    class CostCP	// cost function for refining calibration
    {			//   using multiple planar patterns.
      public:
	typedef double			T;
	typedef CameraBase::Intrinsic	ATA;
	typedef CanonicalCamera		ATB;
	typedef Matrix<T>		JT;

      public:
	CostCP(const MeasurementMatrix& Wt, u_int adim)
	    :_Wt(Wt), _adim(adim)					{}

	Vector<T>	operator ()(const ATA& K,
				    const ATB& camera, int i)	const	;
	JT		jacobianA(const ATA& K,
				  const ATB& camera, int i)	const	;
	Matrix<T>	jacobianB(const ATA& K,
				  const ATB& camera, int i)	const	;
	void		updateA(ATA& K,
				const Vector<T>& dK)		const	;
	void		updateB(ATB& camera,
				const Vector<T>& dcamera)	const	;

	u_int		npoints()	const	{return _Wt.npoints();}
	u_int		adim()		const	{return _adim;}
	u_int		adims()		const	{return adim();}
	
      private:
	const MeasurementMatrix&	_Wt;	// measurement matrix.
	const u_int			_adim;
    };

    template <class CAMERA>
    class CostBA	// cost function for bundle adjustment.
    {
      public:
	typedef double			T;
	typedef Array<CAMERA>		ATA;
	typedef Vector<T>		ATB;
	typedef BlockMatrix<T>		JT;

	class CostCD	// cost function for keeping distance between 0th
	{		//   and 1st cameras constant.
	  public:
	    CostCD(const ATA& p)  :_sqdist01(p[0].t().sqdist(p[1].t()))	{}
	
	    Vector<T>	operator ()(const ATA& p) const
			{
			    Vector<T>	val(1);
			    val[0] = p[0].t().sqdist(p[1].t()) - _sqdist01;
			    return val;
			}
	    Matrix<T>	jacobian(const ATA& p) const
			{
			    Matrix<T>	L(1, p[0].dofIntrinsic() +
					  (6+p[0].dofIntrinsic())*(p.dim()-1));
			    (L[0](p[0].dofIntrinsic(), 3) = p[1].t()-p[0].t())
				*= 2.0;
			    return L;
			}

	  private:
	    const T	_sqdist01;
	};

      public:
	CostBA(const MeasurementMatrix& Wt,
	       u_int dofIntrinsic, bool fixCameraCenter=false)		;

	Vector<T>	operator ()(const ATA& p,
				    const ATB& x, int j)	const	;
	JT		jacobianA(const ATA& p,
				  const ATB& x, int j)		const	;
	Matrix<T>	jacobianB(const ATA& p,
				  const ATB& x, int j)		const	;
	void		updateA(ATA& p,	const Vector<T>& dp)	const	;
	void		updateB(ATB& x, const Vector<T>& dx)	const	;

	u_int			nframes()	const	{return _Wt.nframes();}
	u_int			adim()		const	{return _adim;}
	const Array<u_int>&	adims()		const	{return _adims;}
    
      private:
	const MeasurementMatrix&	_Wt;	// measurement matrix.
	const bool			_fcc;	// fix camera center or not.
	u_int				_adim;	// dimension of parameters A.
	Array<u_int>			_adims;	// dimensions of block jacobian.
    };

    template <class INTRINSIC>
    class CostBACI	// cost function for bundle adjustment
    {			//   with common intrinsic parameters estimation.
      public:
	typedef double			T;
	struct ATA : public Array<CanonicalCamera>
	{
	    ATA(const INTRINSIC& KK,
		const Array<CanonicalCamera>& cc)
		:Array<CanonicalCamera>(cc), K(KK)			{}
	    
	    INTRINSIC			K;	// common intrinsic parameters.
	};
	typedef Vector<T>		ATB;
	typedef Matrix<T>		JT;
    
	class CostCD	// cost function for keeping distance between 0th
	{		//   and 1st cameras constant.
	  public:
	    CostCD(const ATA& p) :_sqdist01(p[0].t().sqdist(p[1].t()))	{}
	
	    Vector<T>	operator ()(const ATA& p) const
			{
			    Vector<T>	val(1);
			    val[0] = p[0].t().sqdist(p[1].t()) - _sqdist01;
			    return val;
			}
	    Matrix<T>	jacobian(const ATA& p) const
			{
			    Matrix<T>	L(1, p.K.dof() + 6*(p.dim()-1));
			    (L[0](p.K.dof(), 3) = (p[1].t()-p[0].t()))
				*= 2.0;
			    return L;
			}

	  private:
	    const T	_sqdist01;
	};

      public:
	CostBACI(const MeasurementMatrix& Wt)	:_Wt(Wt)		{}

	Vector<T>	operator ()(const ATA& p,
				    const ATB& x, int j)	const	;
	Matrix<T>	jacobianA(const ATA& p,
				  const ATB& x, int j)		const	;
	Matrix<T>	jacobianB(const ATA& p,
				  const ATB& x, int j)		const	;
	void		updateA(ATA& p, const Vector<T>& dp)	const	;
	void		updateB(ATB& x, const Vector<T>& dx)	const	;

	u_int		nframes()	const	{return _Wt.nframes();}
	u_int		adim()		const	{return 1 + 6*(nframes() - 1);}
	u_int		adims()		const	{return adim();}
    
      private:
	const MeasurementMatrix&	_Wt;	// measurement matrix.
    };
};
 
//! $B4QB,9TNs$r%9%H%j!<%`$+$iFI$_9~$`(B
/*!
  \param in	$BF~NO%9%H%j!<%`!%(B
  \param Wt	$BFI$_9~$^$l$?4QB,9TNs!%(B
  \return	in$B$GM?$($?F~NO%9%H%j!<%`!%(B
*/
inline std::istream&
operator >>(std::istream& in, MeasurementMatrix& Wt)
{
    return Wt.get(in, 0, 0);
}

std::ostream&	operator <<(std::ostream& out, const MeasurementMatrix& Wt);

/************************************************************************
*  Calibration functions						*
************************************************************************/
void		get_flengths(const Matrix<double>& F,
			     double& kl, double& kr,
			     u_int commonFocalLengths=0);
Matrix<double>	get_principal(const Matrix<double>& F,
			      const Matrix<double>& ldata,
			      const Matrix<double>& rdata);
void		decompose_essential(const Matrix<double>& E,
				    const Matrix<double>& ldata,
				    const Matrix<double>& rdata,
				    Matrix<double>& Rt, Vector<double>& t);
void		decompose_homography(const Matrix<double>& H,
				     const Matrix<double>& ldata,
				     const Matrix<double>& rdata,
				     Matrix<double>& Rt,
				     Vector<double>& t,
				     Vector<double>& normal);
Matrix<double>	projection(const Matrix<double>& data);

/************************************************************************
*  Registration functions						*
************************************************************************/
double	euclidean_registration(const Matrix<double>& data0,
			       const Matrix<double>& data1,
			       Matrix<double>& Rt, Vector<double>& t);
double	similarity_registration(const Matrix<double>& data0,
				const Matrix<double>& data1,
				Matrix<double>& Rt, Vector<double>& t,
				double& s);
double	affine_registration(const Matrix<double>& data0,
			    const Matrix<double>& data1,
			    Matrix<double>& At, Vector<double>& b);

/************************************************************************
*  Assessment functions							*
************************************************************************/
void	assess_projection(const Matrix<double>& data, const Matrix<double>& P);

/************************************************************************
*  File I/O functions							*
************************************************************************/
Matrix<double>	get_Cdata(const char* filename, const char* suffix,
			  u_int ncol, u_int nrow_min);
Matrix<double>	get_full_Cdata(const char* filename,
			       const char* suffix, u_int nrow_min);
Matrix<double>	get_matrix(const char* filename, const char* suffix);
void		put_matrix(const char* filename, const char* suffix,
			   const Matrix<double>& m);
Matrix<double>	get_HomogeneousMatrix(const char* filename,
				      const char* suffix);
void		put_InHomogeneousMatrix(const char* filename,
					const char* suffix,
					const Matrix<double>& m);
}
#endif	// !__TUCalibPP_h
