/*
 *  $Id: createCommands.cc,v 1.2 2002-08-01 05:04:02 ueshiba Exp $
 */
#include "My1394Camera.h"

namespace TU
{
/************************************************************************
*  local data								*
************************************************************************/
/*!
  $B%+%a%i$,%5%]!<%H$9$k5!G=$H$=$NL>>N!%(B
*/
struct MyFeature
{
    const Ieee1394Camera::Feature	feature;	//!< $B%+%a%i$N5!G=(B
    const char* const			name;		//!< $B$=$NL>>N(B
};
static const MyFeature	feature[] =
{
    {Ieee1394Camera::BRIGHTNESS,	"Brightness:"},
    {Ieee1394Camera::AUTO_EXPOSURE,	"Auto exposure:"},
    {Ieee1394Camera::SHARPNESS,		"Sharpness:"},
    {Ieee1394Camera::WHITE_BALANCE,	"White bal.(U/B):"},
    {Ieee1394Camera::WHITE_BALANCE,	"White bal.(V/R):"},
    {Ieee1394Camera::HUE,		"Hue:"},
    {Ieee1394Camera::SATURATION,	"Saturation:"},
    {Ieee1394Camera::GAMMA,		"Gamma:"},
    {Ieee1394Camera::SHUTTER,		"Shutter:"},
    {Ieee1394Camera::GAIN,		"Gain:"},
    {Ieee1394Camera::IRIS,		"Iris:"},
    {Ieee1394Camera::FOCUS,		"Focus:"},
    {Ieee1394Camera::TEMPERATURE,	"Temperature:"},
    {Ieee1394Camera::ZOOM,		"Zoom:"}
};
static const int		NFEATURES = sizeof(feature)/sizeof(feature[0]);

/*!
  $B%+%a%i$H$=$N5!G=$N(B2$B%DAH!%(B3$B$D$N%3!<%k%P%C%/4X?t(B: CBturnOnOff(),
  CBsetAutoManual(), CBsetValue() $B$N0z?t$H$7$FEO$5$l$k!%(B
 */
struct CameraAndFeature
{
    Ieee1394Camera*		camera;		//!< $B%+%a%i(B
    Ieee1394Camera::Feature	feature;	//!< $BA`:n$7$?$$5!G=(B
};
static CameraAndFeature		cameraAndFeature[NFEATURES];

/************************************************************************
*  callback functions							*
************************************************************************/
//! $B%-%c%W%A%c%\%?%s$,(Bon$B$N4VDj4|E*$K8F$P$l$k(Bidle$BMQ%3!<%k%P%C%/4X?t!%(B
/*!
  $B%+%a%i$+$i2hA|$r<h$j9~$s$G(Bcanvas$B$KI=<($9$k!%(B
  \param userdata	My1394Camera (IEEE1394$B%+%a%i(B)
  \return		TRUE$B$rJV$9(B
*/
static gint
CBidle(gpointer userdata)
{
    My1394Camera*	camera = (My1394Camera*)userdata;
    camera->idle();
    return TRUE;
}

//! $B%-%c%W%A%c%\%?%s$N>uBV$,JQ99$5$l$k$H8F$P$l$k%3!<%k%P%C%/4X?t!%(B
/*!
  timer$B$r(B activate/deactivate $B$9$k!%(B
  \param toggle		$B%-%c%W%A%c%\%?%s(B
  \param userdata	My1394Camera (IEEE1394$B%+%a%i(B)
*/
static void
CBcontinuousShot(GtkWidget* toggle, gpointer userdata)
{
    static gint		idleTag;
    My1394Camera*	camera = (My1394Camera*)userdata;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle)))
    {
	idleTag = gtk_idle_add(CBidle, camera);	// idle$B=hM}$r3+;O$9$k!%(B
	camera->continuousShot();	// $B%+%a%i$+$i$N2hA|=PNO$r3+;O$9$k!%(B
    }
    else
    {
	gtk_idle_remove(idleTag);	// idle$B=hM}$rCf;_$9$k!%(B
	camera->stopContinuousShot();	// $B%+%a%i$+$i$N2hA|=PNO$rDd;_$9$k!%(B
    }
}

//! on/off $B%\%?%s$N>uBV$,JQ99$5$l$k$H8F$P$l$k%3!<%k%P%C%/4X?t!%(B
/*!
  $B$"$k%+%a%i5!G=$r(B on/off $B$9$k!%(B
  \param toggle		on/off $B%\%?%s(B
  \param userdata	CameraAndFeature (IEEE1394$B%+%a%i$H(B on/off
			$B$7$?$$5!G=$N(B2$B%DAH(B)
*/
static void
CBturnOnOff(GtkWidget* toggle, gpointer userdata)
{
    CameraAndFeature*	cameraAndFeature = (CameraAndFeature*)userdata;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle)))
	cameraAndFeature->camera->turnOn(cameraAndFeature->feature);
    else
	cameraAndFeature->camera->turnOff(cameraAndFeature->feature);
}

//! auto/manual $B%\%?%s$N>uBV$,JQ99$5$l$k$H8F$P$l$k%3!<%k%P%C%/4X?t!%(B
/*!
  $B$"$k%+%a%i5!G=$r(B auto/manual $B%b!<%I$K$9$k!%(B
  \param toggle		on/off $B%\%?%s(B
  \param userdata	CameraAndFeature (IEEE1394$B%+%a%i$H(B auto/manual
			$B%b!<%I$K$7$?$$5!G=$N(B2$B%DAH(B)
*/
static void
CBsetAutoManual(GtkWidget* toggle, gpointer userdata)
{
    CameraAndFeature*	cameraAndFeature = (CameraAndFeature*)userdata;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle)))
	cameraAndFeature->camera->setAutoMode(cameraAndFeature->feature);
    else
	cameraAndFeature->camera->setManualMode(cameraAndFeature->feature);
}

//! adjustment widget $B$,F0$+$5$l$k$H8F$P$l$k%3!<%k%P%C%/4X?t!%(B
/*!
  $B$"$k%+%a%i5!G=$NCM$r@_Dj$9$k!%(B
  \param adj		$B@_DjCM$rM?$($k(B adjuster
  \param userdata	CameraAndFeature (IEEE1394$B%+%a%i$HCM$r@_Dj$7$?$$(B
			$B5!G=$N(B2$B%DAH(B)
*/
static void
CBsetValue(GtkAdjustment* adj, gpointer userdata)
{
    CameraAndFeature*	cameraAndFeature = (CameraAndFeature*)userdata;
    cameraAndFeature->camera->setValue(cameraAndFeature->feature, adj->value);
}

//! U/B$BCMMQ(B adjustment widget $B$,F0$+$5$l$k$H8F$P$l$k%3!<%k%P%C%/4X?t!%(B
/*!
  $B%[%o%$%H%P%i%s%9$N(BU/B$BCM$r@_Dj$9$k!%(B
  \param adj		$B@_DjCM$rM?$($k(B adjuster
  \param userdata	My1394Camera (IEEE1394$B%+%a%i(B)
*/
static void
CBsetWhiteBalanceUB(GtkAdjustment* adj, gpointer userdata)
{
    My1394Camera*	camera = (My1394Camera*)userdata;
    u_int		ub, vr;
    camera->getWhiteBalance(ub, vr);
    ub = u_int(adj->value);
    camera->setWhiteBalance(ub, vr);
}

//! V/R$BCMMQ(B adjustment widget $B$,F0$+$5$l$k$H8F$P$l$k%3!<%k%P%C%/4X?t!%(B
/*!
  $B%[%o%$%H%P%i%s%9$N(BV/R$BCM$r@_Dj$9$k!%(B
  \param adj		$B@_DjCM$rM?$($k(B adjuster
  \param userdata	My1394Camera (IEEE1394$B%+%a%i(B)
*/
static void
CBsetWhiteBalanceVR(GtkAdjustment* adj, gpointer userdata)
{
    My1394Camera*	camera = (My1394Camera*)userdata;
    u_int		ub, vr;
    camera->getWhiteBalance(ub, vr);
    vr = u_int(adj->value);
    camera->setWhiteBalance(ub, vr);
}

/************************************************************************
*  global functions							*
************************************************************************/
//! $B%+%a%i$N3F<o5!G=$K@_Dj$9$kCM$r;XDj$9$k$?$a$N%3%^%s%I72$r@8@.$9$k!%(B
/*!
  IEEE1394$B%+%a%i$,%5%]!<%H$7$F$$$k5!G=$rD4$Y$F@8@.$9$k%3%^%s%I$r7hDj$9$k!%(B
  \param camera		IEEE1394$B%+%a%i(B
  \return		$B@8@.$5$l$?%3%^%s%I72$,E=$j$D$1$i$l$?%F!<%V%k(B
*/
GtkWidget*
createCommands(My1394Camera& camera)
{
    GtkWidget*	commands = gtk_table_new(4, 1 + NFEATURES, FALSE);
    
  // $B%+%a%i$+$i$N2hA|<h$j9~$_$r(Bon/off$B$9$k(Btoggle button$B$r@8@.!%(B
    GtkWidget* toggle = gtk_toggle_button_new_with_label("Capture");
  // $B%3!<%k%P%C%/4X?t$NEPO?!%(B
    gtk_signal_connect(GTK_OBJECT(toggle), "toggled",
		       GTK_SIGNAL_FUNC(CBcontinuousShot), &camera);
  // $B%+%a%i$N8=:_$N2hA|<h$j9~$_>uBV$r(Btoggle button$B$KH?1G!%(B
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
				 (camera.inContinuousShot() ? TRUE : FALSE));
    gtk_widget_show(toggle);
    gtk_table_attach_defaults(GTK_TABLE(commands), toggle,
			      1, 2, 0, 1);

    u_int	y = 1;
    for (int i = 0; i < NFEATURES; ++i)
    {
	u_int	inq = camera.inquireFeatureFunction(feature[i].feature);
	if (inq & Ieee1394Camera::Presence)  // $B$3$N5!G=$,B8:_!)(B
	{
	    u_int	x = 2;
	    
	    if (inq & Ieee1394Camera::OnOff)  // on/off$BA`:n$,2DG=!)(B
	    {
	      // on/off$B$r@Z$jBX$($k(Btoggle button$B$r@8@.!%(B
		GtkWidget* toggle = gtk_toggle_button_new_with_label("On");
	      // $B%3!<%k%P%C%/4X?t$NEPO?!%(B
		cameraAndFeature[i].camera = &camera;
		cameraAndFeature[i].feature = feature[i].feature;
		gtk_signal_connect(GTK_OBJECT(toggle), "toggled",
				   GTK_SIGNAL_FUNC(CBturnOnOff),
				   (gpointer)&cameraAndFeature[i]);
	      // $B%+%a%i$N8=:_$N(Bon/off$B>uBV$r(Btoggle button$B$KH?1G!%(B
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
		    (camera.isTurnedOn(feature[i].feature) ? TRUE : FALSE));
		gtk_widget_show(toggle);
		gtk_table_attach_defaults(GTK_TABLE(commands), toggle,
					  x, x+1, y, y+1);
		++x;
	    }

	    if (inq & Ieee1394Camera::Manual)  // manual$BA`:n$,2DG=!)(B
	    {
		if (inq & Ieee1394Camera::Auto)  // $B<+F0@_Dj$,2DG=!)(B
		{
		  // manual/auto$B$r@Z$jBX$($k(Btoggle button$B$r@8@.!%(B
		    GtkWidget*	toggle
				  = gtk_toggle_button_new_with_label("Auto");
		  // $B%3!<%k%P%C%/4X?t$NEPO?!%(B
		    cameraAndFeature[i].camera = &camera;
		    cameraAndFeature[i].feature = feature[i].feature;
		    gtk_signal_connect(GTK_OBJECT(toggle), "toggled",
				       GTK_SIGNAL_FUNC(CBsetAutoManual),
				       (gpointer)&cameraAndFeature[i]);
		  // $B%+%a%i$N8=:_$N(Bauto/manual$B>uBV$r(Btoggle button$B$KH?1G!%(B
		    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
			(camera.isAuto(feature[i].feature) ? TRUE : FALSE));
		    gtk_widget_show(toggle);
		    gtk_table_attach_defaults(GTK_TABLE(commands), toggle,
					      x, x+1, y, y+1);
		}
		
		GtkWidget*	label = gtk_label_new(feature[i].name);
		gtk_widget_show(label);
		gtk_table_attach_defaults(GTK_TABLE(commands), label,
					  0, 1, y, y+1);
	      // $B$3$N5!G=$,<h$jF@$kCM$NHO0O$rD4$Y$k!%(B
		u_int	min, max;
		camera.getMinMax(feature[i].feature, min, max);
		if (feature[i].feature == Ieee1394Camera::WHITE_BALANCE)
		{
		  // white balance$B$N8=:_$NCM$rD4$Y$k!%(B
		    u_int	ub, vr;
		    camera.getWhiteBalance(ub, vr);
		  // white balance$B$N(BUB$BCM$rM?$($k$?$a$N(Badjustment widget$B$r@8@.!%(B
		    GtkObject*	adj = gtk_adjustment_new(ub, min, max,
							 1.0, 1.0, 0.0);
		  // $B%3!<%k%P%C%/4X?t$NEPO?!%(B
		    gtk_signal_connect(GTK_OBJECT(adj), "value_changed",
				       GTK_SIGNAL_FUNC(CBsetWhiteBalanceUB),
				       &camera);
		  // adjustment$B$rA`:n$9$k$?$a$N(Bscale widget$B$r@8@.!%(B
		    GtkWidget*	scale = gtk_hscale_new(GTK_ADJUSTMENT(adj));
		    gtk_scale_set_digits(GTK_SCALE(scale), 0);
		    gtk_widget_set_usize(GTK_WIDGET(scale), 200, 30);
		    gtk_widget_show(scale);
		    gtk_table_attach_defaults(GTK_TABLE(commands), scale,
					      1, 2, y, y+1);
		    ++i;
		    ++y;
		    GtkWidget*	label = gtk_label_new(feature[i].name);
		    gtk_widget_show(label);
		    gtk_table_attach_defaults(GTK_TABLE(commands), label,
					      0, 1, y, y+1);
		  // white balance$B$N(BVR$BCM$rM?$($k$?$a$N(Badjustment widget$B$r@8@.!%(B
		    adj = gtk_adjustment_new(vr, min, max, 1.0, 1.0, 0.0);
		  // $B%3!<%k%P%C%/4X?t$NEPO?!%(B
		    gtk_signal_connect(GTK_OBJECT(adj), "value_changed",
				       GTK_SIGNAL_FUNC(CBsetWhiteBalanceVR),
				       &camera);
		  // adjustment$B$rA`:n$9$k$?$a$N(Bscale widget$B$r@8@.!%(B
		    scale = gtk_hscale_new(GTK_ADJUSTMENT(adj));
		    gtk_scale_set_digits(GTK_SCALE(scale), 0);
		    gtk_widget_set_usize(GTK_WIDGET(scale), 200, 30);
		    gtk_widget_show(scale);
		    gtk_table_attach_defaults(GTK_TABLE(commands), scale,
					      1, 2, y, y+1);
		}
		else
		{
		  // $B$3$N5!G=$N8=:_$NCM$rD4$Y$k!%(B
		    int		val = camera.getValue(feature[i].feature);
		  // $B$3$N5!G=$KCM$rM?$($k$?$a$N(Badjustment widget$B$r@8@.!%(B
		    GtkObject*	adj = gtk_adjustment_new(val, min, max,
							 1.0, 1.0, 0.0);
		  // $B%3!<%k%P%C%/4X?t$NEPO?!%(B
		    cameraAndFeature[i].camera = &camera;
		    cameraAndFeature[i].feature = feature[i].feature;
		    gtk_signal_connect(GTK_OBJECT(adj), "value_changed",
				       GTK_SIGNAL_FUNC(CBsetValue),
				       (gpointer)&cameraAndFeature[i]);
		  // adjustment$B$rA`:n$9$k$?$a$N(Bscale widget$B$r@8@.!%(B
		    GtkWidget*	scale = gtk_hscale_new(GTK_ADJUSTMENT(adj));
		    gtk_scale_set_digits(GTK_SCALE(scale), 0);
		    gtk_widget_set_usize(GTK_WIDGET(scale), 200, 30);
		    gtk_widget_show(scale);
		    gtk_table_attach_defaults(GTK_TABLE(commands), scale,
					      1, 2, y, y+1);
		}
	    }

	    ++y;
	}
    }

    gtk_widget_show(commands);

    return commands;
}
 
}
