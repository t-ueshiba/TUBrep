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
 *  $Id: Camera.h,v 1.4 2008-10-05 23:25:16 ueshiba Exp $
 */
#ifndef __TUCamera_h
#define __TUCamera_h

#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  class CameraBase							*
************************************************************************/
//! $B$9$Y$F$NF);kEj1F%+%a%i$N4pDl$H$J$k%/%i%9(B
class CameraBase
{
  public:
  //! $B%+%a%i$NFbIt%Q%i%a!<%?$rI=$9%/%i%9(B
    class Intrinsic
    {
      public:
	virtual ~Intrinsic()						;
	
      // various operations.
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Point2d		xd(const Point2d& x)		const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& x)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;

      // calibration matrices.    
	virtual Matrix33d	K()				const	;
	virtual Matrix33d	Kt()				const	;
	virtual Matrix33d	Kinv()				const	;
	virtual Matrix33d	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		k()				const	;
	virtual Point2d		principal()			const	;
	virtual double		aspect()			const	;
	virtual double		skew()				const	;
	virtual double		d1()				const	;
	virtual double		d2()				const	;
	virtual Intrinsic&	setFocalLength(double k)		;
	virtual Intrinsic&	setPrincipal(double u0, double v0)	;
	virtual Intrinsic&	setAspect(double aspect)		;
	virtual Intrinsic&	setSkew(double skew)			;
	virtual Intrinsic&	setIntrinsic(const Matrix33d& K)	;
	virtual Intrinsic&	setDistortion(double d1, double d2)	;

      // parameter updating functions.
	virtual Intrinsic&	update(const Vector<double>& dp)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;
    };
    
  public:
  //! $B0LCV$r86E@$K!$;Q@*$rC10L9TNs$K%;%C%H$7$F=i4|2=$9$k!%(B
    CameraBase() :_t(), _Rt()	{_Rt[0][0] = _Rt[1][1] = _Rt[2][2] = 1.0;}

  //! $B0LCV$H;Q@*$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param t	$B%+%a%i0LCV$rI=$9(B3$B<!85%Y%/%H%k(B
    \param Rt	$B%+%a%i;Q@*$rI=$9(B3x3$B2sE>9TNs(B
  */
    CameraBase(const Point3d& t, const Matrix33d& Rt)	:_t(t), _Rt(Rt)	{}
    virtual ~CameraBase()						;
    
  // various operations in canonical coordinates.
    Point2d		xc(const Point3d& X)			const	;
    Point2d		xcFromU(const Point2d& u)		const	;
    Matrix34d		Pc()					const	;
    Matrix<double>	jacobianPc(const Point3d& X)		const	;
    Matrix23d		jacobianXc(const Point3d& X)		const	;

  // various oeprations in image coordinates.
    Point2d		operator ()(const Point3d& X)		const	;
    Matrix34d		P()					const	;
    Matrix<double>	jacobianP(const Point3d& X)		const	;
    Matrix<double>	jacobianFCC(const Point3d& X)		const	;
    Matrix23d		jacobianX(const Point3d& X)		const	;
    Matrix<double>	jacobianK(const Point3d& X)		const	;
    Matrix22d		jacobianXC(const Point3d& X)		const	;

  //! $BEj1F9TNs$r%;%C%H$9$k!%(B
  /*!
    \param P	3x4$BEj1F9TNs(B
    \return	$B$3$N%+%a%i(B
  */
    virtual CameraBase& setProjection(const Matrix34d& P)		=0;

  // parameter updating functions.
    void		update(const Vector<double>& dp)		;
    void		updateFCC(const Vector<double>& dp)		;
    void		updateIntrinsic(const Vector<double>& dp)	;
    
  // calibration matrices.
  //! $BFbIt%Q%i%a!<%?9TNs$rJV$9!%(B
  /*!
    \return	3x3$BFbIt%Q%i%a!<%?9TNs(B
  */
    Matrix33d		K()		const	{return intrinsic().K();}

  //! $BFbIt%Q%i%a!<%?9TNs$NE>CV$rJV$9!%(B
  /*!
    \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV(B
  */
    Matrix33d		Kt()		const	{return intrinsic().Kt();}

  //! $BFbIt%Q%i%a!<%?9TNs$N5U9TNs$rJV$9!%(B
  /*!
    \return	3x3$BFbIt%Q%i%a!<%?9TNs$N5U9TNs(B
  */
    Matrix33d		Kinv()		const	{return intrinsic().Kinv();}

  //! $BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs$rJV$9!%(B
  /*!
    \return	3x3$BFbIt%Q%i%a!<%?9TNs$NE>CV$N5U9TNs(B
  */
    Matrix33d		Ktinv()		const	{return intrinsic().Ktinv();}

  // extrinsic parameters.
  //! $B%+%a%i$N0LCV$rJV$9!%(B
  /*!
    \return	$B%+%a%i$N(B3$B<!850LCV(B
  */
    const Point3d&	t()		const	{return _t;}

  //! $B%+%a%i$N;Q@*$rJV$9!%(B
  /*!
    \return	$B%+%a%i$N;Q@*$rI=$9(B3x3$B2sE>9TNs(B
  */
    const Matrix33d&	Rt()		const	{return _Rt;}
    CameraBase&		setTranslation(const Point3d& t)	;
    CameraBase&		setRotation(const Matrix33d& Rt)	;

  // intrinsic parameters.
  //! $BFbIt%Q%i%a!<%?$rJV$9!%(B
  /*!
    \return	$BFbIt%Q%i%a!<%?(B
  */
    virtual const Intrinsic&
			intrinsic()	const	= 0;

  //! $BFbIt%Q%i%a!<%?$rJV$9!%(B
  /*!
    \return	$BFbIt%Q%i%a!<%?(B
  */
    virtual Intrinsic&	intrinsic()		= 0;

  //! $BFbIt%Q%i%a!<%?$N<+M3EY$rJV$9!%(B
  /*!
    \return	$BFbIt%Q%i%a!<%?$N<+M3EY(B
  */
    u_int		dofIntrinsic()	const	{return intrinsic().dof();}

  //! $B>GE@5wN%$rJV$9!%(B
  /*!
    \return	$B>GE@5wN%(B
  */
    double		k()		const	{return intrinsic().k();}

  //! $B2hA|<gE@$rJV$9!%(B
  /*!
    \return	$B2hA|<gE@(B
  */
    Point2d		principal()	const	{return intrinsic().principal();}

  //! $B%"%9%Z%/%HHf$rJV$9!%(B
  /*!
    \return	$B%"%9%Z%/%HHf(B
  */
    double		aspect()	const	{return intrinsic().aspect();}

  //! $BHsD>8rOD$_$rJV$9!%(B
  /*!
    \return	$BHsD>8rOD$_(B
  */
    double		skew()		const	{return intrinsic().skew();}

  //! $BJ|<MOD6J$NBh(B1$B78?t$rJV$9!%(B
  /*!
    \return	$BJ|<MOD6J$NBh(B1$B78?t(B
  */
    double		d1()		const	{return intrinsic().d1();}

  //! $BJ|<MOD6J$NBh(B2$B78?t$rJV$9!%(B
  /*!
    \return	$BJ|<MOD6J$NBh(B2$B78?t(B
  */
    double		d2()		const	{return intrinsic().d2();}
    CameraBase&		setFocalLength(double k)		;
    CameraBase&		setPrincipal(double u0, double v0)	;
    CameraBase&		setAspect(double aspect)		;
    CameraBase&		setSkew(double skew)			;
    CameraBase&		setIntrinsic(const Matrix33d& K)	;
    CameraBase&		setDistortion(double d1, double d2)	;
    
  // I/O functions.
    std::istream&	get(std::istream& in)			;
    std::ostream&	put(std::ostream& out)		const	;

  private:
    Point3d		_t;			// camera center.
    Matrix33d		_Rt;			// camera orientation.
};

//! 3$B<!856u4VCf$NE@$NEj1FE@$N(Bcanonical$B2hA|:BI87O$K$*$1$k0LCV$r5a$a$k!%(B
/*!
  \param X	$BBP>]E@$N(B3$B<!850LCV(B
  \return	canonical$B2hA|:BI87O$K$*$1$k(Bx$B$NEj1FE@$N0LCV!$$9$J$o$A(B
		\f$
		\TUvec{x}{} = 
		\frac{1}{\TUtvec{r}{z}(\TUvec{X}{} - \TUvec{t}{})}
		\TUbeginarray{c}
		\TUtvec{r}{x}(\TUvec{X}{} - \TUvec{t}{}) \\
		\TUtvec{r}{y}(\TUvec{X}{} - \TUvec{t}{})
		\TUendarray
		\f$
*/
inline Point2d
CameraBase::xc(const Point3d& X) const
{
    const Vector<double>&	x = _Rt * (X - _t);
    return Point2d(x[0] / x[2], x[1] / x[2]);
}

//! $B2hA|:BI8$K$*$1$kEj1FE@$N(B2$B<!850LCV$r(Bcanonical$B2hA|:BI87O$KD>$9!%(B
/*!
  \param u	$B2hA|:BI87O$K$*$1$kEj1FE@$N(B2$B<!850LCV(B
  \return	canonical$B2hA|%+%a%i:BI87O$K$*$1$k(Bu$B$N(B2$B<!850LCV!$$9$J$o$A(B
		\f$\TUvec{x}{} = {\cal K}^{-1}(\TUvec{u}{})\f$
*/
inline Point2d
CameraBase::xcFromU(const Point2d& u) const
{
    return intrinsic().xcFromU(u);
}

//! 3$B<!856u4VCf$NE@$NEj1FE@$N2hA|:BI87O$K$*$1$k0LCV$r5a$a$k!%(B
/*!
  \param X	$BBP>]E@$N(B3$B<!850LCV(B
  \return	X$B$NEj1FE@$N2hA|:BI87O$K$*$1$k0LCV!$$9$J$o$A(B
		\f$\TUvec{u}{} = {\cal K}(\TUvec{x}{}(\TUvec{X}{}))\f$
*/
inline Point2d
CameraBase::operator ()(const Point3d& X) const
{
    return intrinsic()(xc(X));
}

//! 3$B<!85%f!<%/%j%C%I6u4V$+$i2hA|J?LL$X$NEj1F9TNs$r5a$a$k!%(B
/*!
  \return	$B2hA|J?LL$X$NEj1F9TNs!$$9$J$o$A(B
  \f$
    \TUvec{P}{} = \TUvec{K}{}\TUtvec{R}{}
    \TUbeginarray{cc} \TUvec{I}{3\times 3} & -\TUvec{t}{} \TUendarray
  \f$
*/
inline Matrix34d
CameraBase::P() const
{
    return K() * Pc();
}

//! $B%+%a%i0LCV0J30$NA4%+%a%i%Q%i%a!<%?$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param X	$BBP>]E@$N(B3$B<!850LCV(B
  \return	$BEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x(3+#dofIntrinsic())$B%d%3%S9TNs!$(B
		$B$9$J$o$A(B
		\f$
		\TUbeginarray{ccc}
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\theta}{}} &
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}
		\TUendarray =
		\TUbeginarray{ccc}
		\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}}
		\TUdisppartial{\TUvec{x}{}}{\TUvec{\theta}{}} &
		\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}
		\TUendarray
		\f$
*/
inline Matrix<double>
CameraBase::jacobianFCC(const Point3d& X) const
{
    const Matrix<double>&	J = jacobianP(X);
    return Matrix<double>(J, 0, 3, J.nrow(), J.ncol() - 3);
}

//! $BE@$N(B3$B<!850LCV$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k!%(B
/*!
  \param X	$BBP>]E@$N(B3$B<!850LCV(B
  \return	$BEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x3$B%d%3%S9TNs!$$9$J$o$A(B
		\f$
		\TUdisppartial{\TUvec{u}{}}{\TUvec{X}{}} =
		\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}}
		\TUdisppartial{\TUvec{x}{}}{\TUvec{X}{}}
		\f$
*/
inline Matrix23d
CameraBase::jacobianX(const Point3d& X) const
{
    return intrinsic().jacobianXC(xc(X)) * jacobianXc(X);
}

//! $BFbIt%Q%i%a!<%?$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k(B
/*!
  \param X	$BBP>]E@$N(B3$B<!850LCV(B
  \return	$BEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x#dofIntrinsic()$B%d%3%S9TNs!$(B
		$B$9$J$o$A(B
		\f$\TUdisppartial{\TUvec{u}{}}{\TUvec{\kappa}{}}\f$
*/
inline Matrix<double>
CameraBase::jacobianK(const Point3d& X) const
{
    return intrinsic().jacobianK(xc(X));
}

//! canonical$B2hA|:BI8$K4X$9$kEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$r5a$a$k(B
/*!
  \param X	$BBP>]E@$N(B3$B<!850LCV(B
  \return	$BEj1FE@$N2hA|:BI8$N(B1$B3,HyJ,$rI=$9(B2x2$B%d%3%S9TNs!$$9$J$o$A(B
		\f$\TUdisppartial{\TUvec{u}{}}{\TUvec{x}{}}\f$
*/
inline Matrix22d
CameraBase::jacobianXC(const Point3d& X) const
{
    return intrinsic().jacobianXC(xc(X));
}

//! $BFbIt%Q%i%a!<%?$r;XDj$5$l$?NL$@$199?7$9$k!%(B
/*!
  \f$\Delta\TUvec{p}{} = \Delta\TUvec{\kappa}{}\f$$B$KBP$7$F(B
  \f$\TUvec{\kappa}{} \leftarrow \TUvec{\kappa}{} - \Delta\TUvec{\kappa}{}\f$
  $B$H99?7$9$k!%(B
  \param dp	$B99?7NL$rI=$9(B#dofIntrinsic()$B<!85%Y%/%H%k(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
inline void
CameraBase::updateIntrinsic(const Vector<double>& dp)
{
    intrinsic().update(dp);			// update intrinsic parameters.
}

//! $B%+%a%i$N;Q@*$HFbIt%Q%i%a!<%?$r;XDj$5$l$?NL$@$199?7$9$k!%(B
/*!
  \f$\Delta\TUvec{p}{} = [\Delta\TUtvec{\theta}{},
  ~\Delta\TUtvec{\kappa}{}]^\top\f$$B$KBP$7$F(B
  \f{eqnarray*}
  \TUtvec{R}{} & \leftarrow &
  \TUtvec{R}{}\TUtvec{R}{}(\Delta\TUvec{\theta}{}) \\
  \TUvec{\kappa}{} & \leftarrow & \TUvec{\kappa}{} - \Delta\TUvec{\kappa}{}
  \f}
  $B$H99?7$9$k!%%+%a%i$N0LCV$O99?7$5$l$J$$!%(B
  \param dp	$B99?7NL$rI=$9(B3+#dofIntrinsic()$B<!85%Y%/%H%k(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
inline void
CameraBase::updateFCC(const Vector<double>& dp)
{
    _Rt *= Matrix<double>::Rt(dp(0, 3));	// update rotation.
    updateIntrinsic(dp(3, dp.dim() - 3));	// update intrinsic parameters.
}

//! $B%+%a%i$N30It!?FbIt%Q%i%a!<%?$r;XDj$5$l$?NL$@$199?7$9$k!%(B
/*!
  \f$\Delta\TUvec{p}{} = [\Delta\TUtvec{t}{},~\Delta\TUtvec{\theta}{},
  ~\Delta\TUtvec{\kappa}{}]^\top\f$$B$KBP$7$F(B
  \f{eqnarray*}
  \TUvec{t}{} & \leftarrow & \TUvec{t}{} - \Delta\TUvec{t}{} \\
  \TUtvec{R}{} & \leftarrow &
  \TUtvec{R}{}\TUtvec{R}{}(\Delta\TUvec{\theta}{}) \\
  \TUvec{\kappa}{} & \leftarrow & \TUvec{\kappa}{} - \Delta\TUvec{\kappa}{}
  \f}
  $B$H99?7$9$k!%(B
  \param dp	$B99?7NL$rI=$9(B6+#dofIntrinsic()$B<!85%Y%/%H%k(B
  \return	$B$3$NFbIt%Q%i%a!<%?(B
*/
inline void
CameraBase::update(const Vector<double>& dp)
{
    _t -= dp(0, 3);				// update translation.
    updateFCC(dp(3, dp.dim() - 3));		// update other prameters.
}

//! $B%+%a%i$N0LCV$r@_Dj$9$k!%(B
/*!
  \param t	$B%+%a%i$N(B3$B<!850LCV(B
  \return	$B$3$N%+%a%i(B
*/
inline CameraBase&
CameraBase::setTranslation(const Point3d& t)
{
    _t = t;
    return *this;
}

//! $B%+%a%i$N;Q@*$r@_Dj$9$k!%(B
/*!
  \param Rt	$B%+%a%i$N;Q@*$rI=$9(B3x3$B2sE>9TNs(B
  \return	$B$3$N%+%a%i(B
*/
inline CameraBase&
CameraBase::setRotation(const Matrix33d& Rt)
{
    _Rt = Rt;
    return *this;
}

//! $B>GE@5wN%$r@_Dj$9$k!%(B
/*!
  \param k	$B>GE@5wN%(B
  \return	$B$3$N%+%a%i(B
*/
inline CameraBase&
CameraBase::setFocalLength(double k)
{
    intrinsic().setFocalLength(k);
    return *this;
}

//! $B2hA|<gE@$r@_Dj$9$k!%(B
/*!
  \param u0	$B2hA|<gE@$N2#:BI8(B
  \param v0	$B2hA|<gE@$N=D:BI8(B
  \return	$B$3$N%+%a%i(B
*/
inline CameraBase&
CameraBase::setPrincipal(double u0, double v0)
{
    intrinsic().setPrincipal(u0, v0);
    return *this;
}

//! $B%"%9%Z%/%HHf$r@_Dj$9$k!%(B
/*!
  \param aspect	$B%"%9%Z%/%HHf(B
  \return	$B$3$N%+%a%i(B
*/
inline CameraBase&
CameraBase::setAspect(double aspect)
{
    intrinsic().setAspect(aspect);
    return *this;
}

//! $BHsD>8r@-OD$_$r@_Dj$9$k!%(B
/*!
  \param skew	$BHsD>8r@-OD$_(B
  \return	$B$3$N%+%a%i(B
*/
inline CameraBase&
CameraBase::setSkew(double skew)
{
    intrinsic().setSkew(skew);
    return *this;
}

//! $BJ|<MOD6J78?t0J30$NFbIt%Q%i%a!<%?$r@_Dj$9$k!%(B
/*!
  \param K	3x3$BFbIt%Q%i%a!<%?9TNs(B
  \return	$B$3$N%+%a%i(B
*/
inline CameraBase&
CameraBase::setIntrinsic(const Matrix33d& K)
{
    intrinsic().setIntrinsic(K);
    return *this;
}

//! $BJ|<MOD6J78?t$r@_Dj$9$k!%(B
/*!
  \param d1	$BJ|<MOD6J$NBh(B1$B78?t(B
  \param d2	$BJ|<MOD6J$NBh(B2$B78?t(B
  \return	$B$3$N%+%a%i(B
*/
inline CameraBase&
CameraBase::setDistortion(double d1, double d2)
{
    intrinsic().setDistortion(d1, d2);
    return *this;
}

//! $BF~NO%9%H%j!<%`$+$i%+%a%i$N30It!?FbIt%Q%i%a!<%?$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \param camera	$B30It!?FbIt%Q%i%a!<%?$NFI$_9~$_@h(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
inline std::istream&
operator >>(std::istream& in, CameraBase& camera)
{
    return camera.get(in);
}

//! $B=PNO%9%H%j!<%`$K%+%a%i$N30It!?FbIt%Q%i%a!<%?$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \param camera	$B30It!?FbIt%Q%i%a!<%?$N=q$-=P$785(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
inline std::ostream&
operator <<(std::ostream& out, const CameraBase& camera)
{
    return camera.put(out);
}

//! $BF~NO%9%H%j!<%`$+$i%+%a%i$NFbIt%Q%i%a!<%?$rFI$_9~$`(B(ASCII)$B!%(B
/*!
  \param in	$BF~NO%9%H%j!<%`(B
  \param camera	$BFbIt%Q%i%a!<%?$NFI$_9~$_@h(B
  \return	in$B$G;XDj$7$?F~NO%9%H%j!<%`(B
*/
inline std::istream&
operator >>(std::istream& in, CameraBase::Intrinsic& intrinsic)
{
    return intrinsic.get(in);
}

//! $B=PNO%9%H%j!<%`$K%+%a%i$NFbIt%Q%i%a!<%?$r=q$-=P$9(B(ASCII)$B!%(B
/*!
  \param out	$B=PNO%9%H%j!<%`(B
  \param camera	$BFbIt%Q%i%a!<%?$N=q$-=P$785(B
  \return	out$B$G;XDj$7$?=PNO%9%H%j!<%`(B
*/
inline std::ostream&
operator <<(std::ostream& out, const CameraBase::Intrinsic& intrinsic)
{
    return intrinsic.put(out);
}

/************************************************************************
*  class CanonicalCamera						*
************************************************************************/
//! $B$9$Y$F$NFbIt%Q%i%a!<%?$,I8=`4{DjCM(B($B>GE@5wN%$H%"%9%Z%/%HHf$,(B1, $BHsD>8rOD$_$HJ|<MOD6J78?t$,(B0, $B2hA|<gE@$,86E@$K0lCW(B)$B$H$J$kF);kEj1F%+%a%i$rI=$9%/%i%9(B
class CanonicalCamera : public CameraBase
{
  public:
  //! $B0LCV$r86E@$K!$;Q@*$rC10L9TNs$K%;%C%H$7$F=i4|2=$9$k!%(B
    CanonicalCamera()	:CameraBase(), _intrinsic()	{}

  //! $B0LCV$H;Q@*$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param t	$B%+%a%i$N(B3$B<!850LCV(B
    \param Rt	$B%+%a%i$N;Q@*$rI=$9(B3x3$B2sE>9TNs(B
  */
    CanonicalCamera(const Point3d& t, const Matrix33d& Rt)
	:CameraBase(t, Rt), _intrinsic()		{}

  //! $BEj1F9TNs$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param P	3x4$BEj1F9TNs(B
  */
    CanonicalCamera(const Matrix34d& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}
    
    virtual CameraBase&	setProjection(const Matrix34d& P)		;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;

  private:
    Intrinsic	_intrinsic;
};

/************************************************************************
*  class CameraWithFocalLength						*
************************************************************************/
//! $B>GE@5wN%0J30$NFbIt%Q%i%a!<%?$,I8=`4{DjCM$H$J$kF);kEj1F%+%a%i$rI=$9%/%i%9(B
class CameraWithFocalLength : public CameraBase
{
  public:
  //! $B>GE@5wN%$N$_$+$i@.$kFbIt%Q%i%a!<%?$rI=$9%/%i%9(B
    class Intrinsic : public CanonicalCamera::Intrinsic
    {
      public:
      //! $BFbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
      /*!
	\param k	$B>GE@5wN%(B
      */
	Intrinsic(double k=1.0)	:_k(k)					{}

      // various operations.
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& x)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;

      // calibration matrices.
	virtual Matrix33d	K()				const	;
	virtual Matrix33d	Kt()				const	;
	virtual Matrix33d	Kinv()				const	;
	virtual Matrix33d	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		k()				const	;
	virtual	CameraBase::Intrinsic&
				setFocalLength(double k)		;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      private:
	double	_k;
    };
    
  public:
  //! $B0LCV$r86E@$K!$;Q@*$rC10L9TNs$K!$FbIt%Q%i%a!<%?$r%G%U%)%k%HCM$K%;%C%H$7$F=i4|2=$9$k!%(B
    CameraWithFocalLength()	:CameraBase(), _intrinsic()	{}

  //! $B30It!?FbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param t	$B%+%a%i$N(B3$B<!850LCV(B
    \param Rt	$B%+%a%i$N;Q@*$rI=$9(B3x3$B2sE>9TNs(B
    \param k	$B>GE@5wN%(B
  */
    CameraWithFocalLength(const Point3d& t,
			  const Matrix33d& Rt, double k=1.0)
	:CameraBase(t, Rt), _intrinsic(k)			{}

  //! $BEj1F9TNs$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param P	3x4$BEj1F9TNs(B
  */
    CameraWithFocalLength(const Matrix34d& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&		setProjection(const Matrix34d& P)	;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};

/************************************************************************
*  class CameraWithEuclideanImagePlane					*
************************************************************************/
//! $B>GE@5wN%$H2hA|<gE@0J30$NFbIt%Q%i%a!<%?$,I8=`4{DjCM$H$J$kF);kEj1F%+%a%i$rI=$9%/%i%9(B
class CameraWithEuclideanImagePlane : public CameraBase
{
  public:
  //! $B>GE@5wN%$H2hA|<gE@$+$i@.$kFbIt%Q%i%a!<%?$rI=$9%/%i%9(B
    class Intrinsic : public CameraWithFocalLength::Intrinsic
    {
      public:
      //! $BFbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
      /*!
	\param k	$B>GE@5wN%(B
	\param u0	$B2hA|<gE@$N2#:BI8(B
	\param v0	$B2hA|<gE@$N=D:BI8(B
      */
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0)
	    :CameraWithFocalLength::Intrinsic(k), _principal(u0, v0)	{}

      //! #CameraWithFocalLength$B7?%+%a%i$NFbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
      /*!
	$B2hA|<gE@$O(B(0, 0)$B$K=i4|2=$5$l$k!%(B
	\param intrinsic	#CameraWithFocalLength$B$NFbIt%Q%i%a!<%?(B
      */
	Intrinsic(const CameraWithFocalLength::Intrinsic& intrinsic)
	    :CameraWithFocalLength::Intrinsic(intrinsic),
	     _principal(0.0, 0.0)					{}
	
      // various operations.	
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;
    
      // calibration matrices.	
	virtual Matrix33d	K()				const	;
	virtual Matrix33d	Kt()				const	;
	virtual Matrix33d	Kinv()				const	;
	virtual Matrix33d	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual Point2d		principal()			const	;
	virtual CameraBase::Intrinsic&
				setPrincipal(double u0, double v0)	;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      private:
	Point2d	_principal;
    };
    
  public:
  //! $B0LCV$r86E@$K!$;Q@*$rC10L9TNs$K!$FbIt%Q%i%a!<%?$r%G%U%)%k%HCM$K%;%C%H$7$F=i4|2=$9$k!%(B
    CameraWithEuclideanImagePlane()	:CameraBase(), _intrinsic()	{}

  //! $B30It!?FbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param t	$B%+%a%i$N(B3$B<!850LCV(B
    \param Rt	$B%+%a%i$N;Q@*$rI=$9(B3x3$B2sE>9TNs(B
    \param k	$B>GE@5wN%(B
    \param u0	$B2hA|<gE@$N2#:BI8(B
    \param v0	$B2hA|<gE@$N=D:BI8(B
  */
    CameraWithEuclideanImagePlane(const Point3d&	t,
				  const Matrix33d&	Rt,
				  double		k=1.0,
				  double		u0=0,
				  double		v0=0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0)			{}

  //! $BEj1F9TNs$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param P	3x4$BEj1F9TNs(B
  */
    CameraWithEuclideanImagePlane(const Matrix34d& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&	setProjection(const Matrix34d& P)		;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};
    
/************************************************************************
*  class Camera								*
************************************************************************/
//! $BJ|<MOD6J78?t$N$_$,I8=`4{DjCM(B(0)$B$H$J$kF);kEj1F%+%a%i$rI=$9%/%i%9(B
class Camera : public CameraBase
{
  public:
  //! $BJ|<MOD6J78?t0U30$NA4FbIt%Q%i%a!<%?$rI=$9%/%i%9(B
    class Intrinsic : public CameraWithEuclideanImagePlane::Intrinsic
    {
      public:
      //! $BFbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
      /*!
	\param k	$B>GE@5wN%(B
	\param u0	$B2hA|<gE@$N2#:BI8(B
	\param v0	$B2hA|<gE@$N=D:BI8(B
	\param aspect	$B%"%9%Z%/%HHf(B
	\param skew	$BHsD>8r@-OD$_(B
      */
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0,
		  double aspect=1.0, double skew=0.0)
	    :CameraWithEuclideanImagePlane::Intrinsic(k, u0, v0),
	     _k00(aspect * k), _k01(skew * k)				{}

      //! #CameraWithEuclideanImagePlane$B7?%+%a%i$NFbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
      /*!
	$B%"%9%Z%/%HHf$HHsD>8rOD$_$O(B0$B$K=i4|2=$5$l$k!%(B
	\param intrinsic	#CameraWithEuclideanImagePlane$B$NFbIt%Q%i%a!<%?(B
      */
	Intrinsic(const CameraWithEuclideanImagePlane::Intrinsic& intrinsic)
	    :CameraWithEuclideanImagePlane::Intrinsic(intrinsic),
	     _k00(k()), _k01(0.0)					{}

      //! $BFbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
      /*!
	\param K	3x3$BFbIt%Q%i%a!<%?9TNs(B
      */
	Intrinsic(const Matrix33d& K)
	    :CameraWithEuclideanImagePlane::Intrinsic(),
	     _k00(k()), _k01(0.0)			{setIntrinsic(K);}
	
      // various operations.
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& x)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;

      // calibration matrices.
	virtual Matrix33d	K()				const	;
	virtual Matrix33d	Kt()				const	;
	virtual Matrix33d	Kinv()				const	;
	virtual Matrix33d	Ktinv()				const	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		aspect()			const	;
	virtual double		skew()				const	;
	virtual	CameraBase::Intrinsic&
				setFocalLength(double k)		;
	virtual CameraBase::Intrinsic&
				setAspect(double aspect)		;
	virtual CameraBase::Intrinsic&
				setSkew(double skew)			;
	virtual CameraBase::Intrinsic&
				setIntrinsic(const Matrix33d& K)	;

      // parameter updating functions.
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;
    
      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      protected:
      //! $B>GE@5wN%$H%"%9%Z%/%HHf$N@Q$rJV$9!%(B
      /*!
	\return		$B>GE@5wN%(Bk$B$H%"%9%Z%/%HHf(Ba$B$N@Q(Bak
      */
		double		k00()			const	{return _k00;}

      //! $B>GE@5wN%$HHsD>8rOD$_$N@Q$rJV$9!%(B
      /*!
	\return		$B>GE@5wN%(Bk$B$HHsD>8rOD$_(Bs$B$N@Q(Bsk
      */
		double		k01()			const	{return _k01;}
	
      private:
	double	_k00, _k01;
    };
    
  public:
  //! $B0LCV$r86E@$K!$;Q@*$rC10L9TNs$K!$FbIt%Q%i%a!<%?$r%G%U%)%k%HCM$K%;%C%H$7$F=i4|2=$9$k!%(B
    Camera()	:CameraBase(), _intrinsic()			{}

  //! $B30It!?FbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param t	$B%+%a%i$N(B3$B<!850LCV(B
    \param Rt	$B%+%a%i$N;Q@*$rI=$9(B3x3$B2sE>9TNs(B
    \param k	$B>GE@5wN%(B
    \param u0	$B2hA|<gE@$N2#:BI8(B
    \param v0	$B2hA|<gE@$N=D:BI8(B
    \param a	$B%"%9%Z%/%HHf(B
    \param s	$BHsD>8rOD$_(B
  */
    Camera(const Point3d&	t,
	   const Matrix33d&	Rt,
	   double		k=1.0,
	   double		u0=0,
	   double		v0=0,
	   double		aspect=1.0,
	   double		skew=0.0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0, aspect, skew)	{}

  //! $BEj1F9TNs$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param P	3x4$BEj1F9TNs(B
  */
    Camera(const Matrix34d& P)
	:CameraBase(), _intrinsic()			{setProjection(P);}

    virtual CameraBase&	setProjection(const Matrix34d& P)		;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};

/************************************************************************
*  class CameraWithDistortion						*
************************************************************************/
//! $BJ|<MOD6J78?t$r4^$`A4FbIt%Q%i%a!<%?$,2DJQ$H$J$kF);kEj1F%+%a%i$rI=$9%/%i%9(B
class CameraWithDistortion : public CameraBase
{
  public:
  //! $BJ|<MOD6J78?t$r4^$`A4FbIt%Q%i%a!<%?$rI=$9%/%i%9(B
    class Intrinsic : public Camera::Intrinsic
    {
      public:
      //! $BFbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
      /*!
	\param k	$B>GE@5wN%(B
	\param u0	$B2hA|<gE@$N2#:BI8(B
	\param v0	$B2hA|<gE@$N=D:BI8(B
	\param aspect	$B%"%9%Z%/%HHf(B
	\param skew	$BHsD>8r@-OD$_(B
	\param d1	$BJ|<MOD6J$NBh(B1$B78?t(B
	\param d2	$BJ|<MOD6J$NBh(B2$B78?t(B
      */
	Intrinsic(double k=1.0, double u0=0.0, double v0=0.0,
		  double aspect=1.0, double skew=0.0,
		  double d1=0.0, double d2=0.0)
	    :Camera::Intrinsic(k, u0, v0, aspect, skew),
	     _d1(d1), _d2(d2)						{}

      //! #Camera$B7?%+%a%i$NFbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
      /*!
	2$B$D$NJ|<MOD6J78?t$O(B0$B$K=i4|2=$5$l$k!%(B
	\param intrinsic	#Camera$B$NFbIt%Q%i%a!<%?(B
      */
	Intrinsic(const Camera::Intrinsic& intrinsic)
	    :Camera::Intrinsic(intrinsic), _d1(0.0), _d2(0.0)		{}

      //! $BFbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
      /*!
	2$B$D$NJ|<MOD6J78?t$O(B0$B$K=i4|2=$5$l$k!%(B
	\param K	3x3$BFbIt%Q%i%a!<%?9TNs(B
      */
	Intrinsic(const Matrix33d& K)
	    :Camera::Intrinsic(), _d1(0.0), _d2(0.0)	{setIntrinsic(K);}
	
      // various operations.
	virtual Point2d		operator ()(const Point2d& x)	const	;
	virtual Point2d		xd(const Point2d& x)		const	;
	virtual Matrix<double>	jacobianK(const Point2d& x)	const	;
	virtual Matrix22d	jacobianXC(const Point2d& x)	const	;
	virtual Point2d		xcFromU(const Point2d& u)	const	;
	virtual CameraBase::Intrinsic&
				update(const Vector<double>& dp)	;

      // intrinsic parameters.
	virtual u_int		dof()				const	;
	virtual double		d1()				const	;
	virtual double		d2()				const	;
	virtual CameraBase::Intrinsic&	
				setDistortion(double d1, double d2)	;

      // I/O functions.
	virtual std::istream&	get(std::istream& in)			;
	virtual std::ostream&	put(std::ostream& out)		const	;

      private:
	double	_d1, _d2;
    };
    
  public:
  //! $B0LCV$r86E@$K!$;Q@*$rC10L9TNs$K!$FbIt%Q%i%a!<%?$r%G%U%)%k%HCM$K%;%C%H$7$F=i4|2=$9$k!%(B
    CameraWithDistortion()	:CameraBase(), _intrinsic()		{}

  //! $B30It!?FbIt%Q%i%a!<%?$r%;%C%H$7$F=i4|2=$9$k!%(B
  /*!
    \param t	$B%+%a%i$N(B3$B<!850LCV(B
    \param Rt	$B%+%a%i$N;Q@*$rI=$9(B3x3$B2sE>9TNs(B
    \param k	$B>GE@5wN%(B
    \param u0	$B2hA|<gE@$N2#:BI8(B
    \param v0	$B2hA|<gE@$N=D:BI8(B
    \param a	$B%"%9%Z%/%HHf(B
    \param s	$BHsD>8rOD$_(B
    \param d1	$BJ|<MOD6J$NBh(B1$B78?t(B
    \param d2	$BJ|<MOD6J$NBh(B2$B78?t(B
  */
    CameraWithDistortion(const Point3d&		t,
			 const Matrix33d&	Rt,
			 double			k=1.0,
			 double			u0=0,
			 double			v0=0,
			 double			aspect=1.0,
			 double			skew=0.0,
			 double			d1=0.0,
			 double			d2=0.0)
	:CameraBase(t, Rt), _intrinsic(k, u0, v0, aspect, skew, d1, d2)	{}

    CameraWithDistortion(const Matrix34d& P,
			 double d1=0.0, double d2=0.0)			;

    virtual CameraBase&		setProjection(const Matrix34d& P)	;
    virtual const CameraBase::Intrinsic&	intrinsic()	const	;
    virtual CameraBase::Intrinsic&		intrinsic()		;
    
  private:
    Intrinsic	_intrinsic;
};
 
//! $BEj1F9TNs$HJ|<MOD6J78?t$r%;%C%H$7$F=i4|2=$9$k!%(B
/*!
  \param P	3x4$BEj1F9TNs(B
  \param d1	$BJ|<MOD6J$NBh(B1$B78?t(B
  \param d2	$BJ|<MOD6J$NBh(B2$B78?t(B
*/
inline
CameraWithDistortion::CameraWithDistortion(const Matrix34d& P,
					   double d1, double d2)
    :CameraBase(), _intrinsic()
{
    setProjection(P);
    setDistortion(d1, d2);
}

}
#endif	/* !__TUCamera_h */
