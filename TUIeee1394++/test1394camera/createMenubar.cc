/*
 *  $Id: createMenubar.cc,v 1.1.1.1 2002-07-25 02:14:15 ueshiba Exp $
 */
#include "My1394Camera.h"
#include <iomanip>

namespace TU
{
/************************************************************************
*  local data								*
************************************************************************/
/*!
  $B%+%a%i$,%5%]!<%H$9$k2hA|%U%)!<%^%C%H$H$=$NL>>N!%(B
*/
struct Format
{
    const Ieee1394Camera::Format	format;		//!< $B2hA|%U%)!<%^%C%H(B
    const char* const			name;		//!< $B$=$NL>>N(B
    
};
static const Format	format[] =
{
    {Ieee1394Camera::Format_0_0, "160x120-YUV(4:4:4)"},
    {Ieee1394Camera::Format_0_1, "320x240-YUV(4:2:2)"},
    {Ieee1394Camera::Format_0_2, "640x480-YUV(4:1:1)"},
    {Ieee1394Camera::Format_0_3, "640x480-YUV(4:2:2)"},
    {Ieee1394Camera::Format_0_4, "640x480-RGB"},
    {Ieee1394Camera::Format_0_5, "640x480-Y(mono)"},
    {Ieee1394Camera::Format_1_0, "800x600-YUV(4:2:2)"},
    {Ieee1394Camera::Format_1_1, "800x600-RGB"},
    {Ieee1394Camera::Format_1_2, "800x600-Y(mono)"},
    {Ieee1394Camera::Format_1_3, "1024x768-YUV(4:2:2)"},
    {Ieee1394Camera::Format_1_4, "1024x768-RGB"},
    {Ieee1394Camera::Format_1_5, "1024x768-Y(mono)"},
    {Ieee1394Camera::Format_2_0, "1280x960-YUV(4:2:2)"},
    {Ieee1394Camera::Format_2_1, "1280x960-RGB"},
    {Ieee1394Camera::Format_2_2, "1280x960-Y(mono)"},
    {Ieee1394Camera::Format_2_3, "1600x1200-YUV(4:2:2)"},
    {Ieee1394Camera::Format_2_4, "1600x1200-RGB"},
    {Ieee1394Camera::Format_2_5, "1600x1200-Y(mono)"}
};
static const int	NFORMATS = sizeof(format)/sizeof(format[0]);

/*!
  $B%+%a%i$,%5%]!<%H$9$k%U%l!<%`%l!<%H$H$=$NL>>N!%(B
*/
struct FrameRate
{
    const Ieee1394Camera::FrameRate	frameRate;	//!< $B%U%l!<%`%l!<%H(B
    const char* const			name;		//!< $B$=$NL>>N(B
};
static const FrameRate	frameRate[] =
{
    {Ieee1394Camera::FrameRate_0, "1.875fps"},
    {Ieee1394Camera::FrameRate_1, "3.75fps"},
    {Ieee1394Camera::FrameRate_2, "7.5fps"},
    {Ieee1394Camera::FrameRate_3, "15fps"},
    {Ieee1394Camera::FrameRate_4, "30fps"},
    {Ieee1394Camera::FrameRate_5, "60fps"}
};
static const int	NRATES=sizeof(frameRate)/sizeof(frameRate[0]);

/*!
  $B%+%a%i(B, $B2hA|%U%)!<%^%C%H(B, $B%U%l!<%`%l!<%H$N(B3$B%DAH!%%3!<%k%P%C%/4X?t(B:
  CBmenuitem() $B$N0z?t$H$7$FEO$5$l$k!%(B
 */
struct FormatAndFrameRate
{
    My1394Camera*		camera;
    Ieee1394Camera::Format	format;
    Ieee1394Camera::FrameRate	frameRate;
    
};
static FormatAndFrameRate	fmtAndFRate[NFORMATS * NRATES];

/************************************************************************
*  static functions							*
************************************************************************/
static ostream&
operator <<(ostream& out, const Ieee1394Camera& camera)
{
    for (int i = 0; i < NFORMATS; ++i)
	if (camera.getFormat() == format[i].format)
	{
	    out << format[i].name;
	    break;
	}

    for (int i = 0; i < NRATES; ++i)
	if (camera.getFrameRate() == frameRate[i].frameRate)
	{
	    out << ' ' << frameRate[i].name;
	    break;
	}

    struct
    {
	Ieee1394Camera::Feature	feature;
	const char*			name;
    } feature[] =
    {
	{Ieee1394Camera::BRIGHTNESS,	"BRIGHTNESS"},
	{Ieee1394Camera::AUTO_EXPOSURE, "AUTO_EXPOSURE"},
	{Ieee1394Camera::SHARPNESS,	"SHARPNESS"},
	{Ieee1394Camera::WHITE_BALANCE, "WHITE_BALANCE"},
	{Ieee1394Camera::HUE,		"HUE"},
	{Ieee1394Camera::SATURATION,	"SATURATION"},
	{Ieee1394Camera::GAMMA,		"GAMMA"},
	{Ieee1394Camera::SHUTTER,	"SHUTTER"},
	{Ieee1394Camera::GAIN,		"GAIN"},
	{Ieee1394Camera::IRIS,		"IRIS"},
	{Ieee1394Camera::FOCUS,		"FOCUS"},
	{Ieee1394Camera::TEMPERATURE,	"TEMPERATURE"},
	{Ieee1394Camera::ZOOM,		"ZOOM"},
	{Ieee1394Camera::PAN,		"PAN"},
	{Ieee1394Camera::TILT,		"TILT"}
    };
    const int	nfeatures = sizeof(feature) / sizeof(feature[0]);
		    
    for (int i = 0; i < nfeatures; ++i)
    {
	u_int	inq = camera.inquireFeature(feature[i].feature);
	if ((inq & Ieee1394Camera::Presence_Inq) &&
	    (inq & Ieee1394Camera::Manual_Inq))
	{
	    out << ' ' << feature[i].name << ' ';

	    if ((inq & Ieee1394Camera::Auto_Inq) && 
		camera.isAuto(feature[i].feature))
		out << -1;
	    else if (feature[i].feature == Ieee1394Camera::WHITE_BALANCE)
	    {
		u_int	ub, vr;
		camera.getWhiteBalance(ub, vr);
		out << ub << ' ' << vr;
	    }
	    else
		out << camera.getValue(feature[i].feature);
	}
    }

    return out << endl;
}

/************************************************************************
*  callback functions							*
************************************************************************/
//! $B%U%)!<%^%C%H$H%U%l!<%`%l!<%H$r@_Dj$9$k$?$a$N%3!<%k%P%C%/4X?t!%(B
/*!
  \param userdata	FormatAndFrameRate ($B%+%a%i(B, $B@_Dj$9$Y$-%U%)!<%^%C%H(B,
			$B@_Dj$9$Y$-%U%l!<%`%l!<%H$N(B3$B%DAH(B)
*/
static void
CBmenuitem(GtkMenuItem*, gpointer userdata)
{
    FormatAndFrameRate*	fmtAndFRate = (FormatAndFrameRate*)userdata;
    fmtAndFRate->camera->setFormatAndFrameRate(fmtAndFRate->format,
					       fmtAndFRate->frameRate);
}

//! $B%+%a%i$N@_DjCM$rI8=`=PNO$K=q$-=P$7$F=*N;$9$k$?$a$N%3!<%k%P%C%/4X?t!%(B
/*!
  \param userdata	My1394Camera (IEEE1394$B%+%a%i(B)
*/
static void
CBexit(GtkMenuItem*, gpointer userdata)
{
    My1394Camera*	camera = (My1394Camera*)userdata;
    camera->stopContinuousShot();
    cout << "0x" << hex << setw(16) << setfill('0')
	 << camera->globalUniqueId() << dec << ' ' << *camera;
    gtk_exit(0);
}

/************************************************************************
*  global functions							*
************************************************************************/
//! $B%a%K%e!<%P!<$r@8@.$9$k!%(B
/*!
  IEEE1394$B%+%a%i$,%5%]!<%H$7$F$$$k2hA|%U%)!<%^%C%H$H%U%l!<%`%l!<%H$rD4$Y$F(B
  $B%a%K%e!<9`L\$r7hDj$9$k!%(B
  \param camera		IEEE1394$B%+%a%i(B
  \return		$B@8@.$5$l$?%a%K%e!<%P!<(B
*/
GtkWidget*
createMenubar(My1394Camera& camera)
{
    GtkWidget*	menubar	= gtk_menu_bar_new();

  // "File"$B%a%K%e!<$r@8@.!%(B
    GtkWidget*	menu = gtk_menu_new();
    GtkWidget*	item = gtk_menu_item_new_with_label("Quit");
    gtk_signal_connect(GTK_OBJECT(item), "activate",
		       GTK_SIGNAL_FUNC(CBexit), &camera);
    gtk_widget_show(item);
    gtk_menu_append(GTK_MENU(menu), item);
    gtk_widget_show(menu);
    item = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), menu);
    gtk_widget_show(item);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), item);

  // "Format"$B%a%K%e!<$r@8@.!%(B
    menu = gtk_menu_new();
  // $B8=:_;XDj$5$l$F$$$k2hA|%U%)!<%^%C%H$*$h$S%U%l!<%`%l!<%H$rD4$Y$k!%(B
    Ieee1394Camera::Format	current_format = camera.getFormat();
    Ieee1394Camera::FrameRate	current_rate   = camera.getFrameRate();
    int	nitems = 0;
    for (int i = 0; i < NFORMATS; ++i)	// $BA4$F$N%U%)!<%^%C%H$K$D$$$F(B...
    {
      // $B$3$N%U%)!<%^%C%H$,%5%]!<%H$5$l$F$$$k$+D4$Y$k!%(B
	u_int		inq = camera.inquireFormat(format[i].format);
	GtkWidget*	submenu = 0;
	for (int j = 0; j < NRATES; ++j) // $BA4$F$N%U%l!<%`%l!<%H$K$D$$$F(B...
	{
	  // $B$3$N%U%l!<%`%l!<%H$,%5%]!<%H$5$l$F$$$k$+D4$Y$k!%(B
	    if (inq & (u_int(Ieee1394Camera::FrameRate_0_Inq) >> j))
	    {
	      // $B%U%l!<%`%l!<%H$r;XDj$9$k$?$a$N%5%V%a%K%e!<$r:n$k!%(B
		if (submenu == 0)
		    submenu = gtk_menu_new();
		GtkWidget* item
		    = gtk_menu_item_new_with_label(frameRate[j].name);
		gtk_menu_append(GTK_MENU(submenu), item);
		fmtAndFRate[nitems].camera = &camera;
		fmtAndFRate[nitems].format = format[i].format;
		fmtAndFRate[nitems].frameRate = frameRate[j].frameRate;
		gtk_signal_connect(GTK_OBJECT(item), "activate",
				   GTK_SIGNAL_FUNC(CBmenuitem),
				   (gpointer)&fmtAndFRate[nitems]);
		gtk_widget_show(item);
		++nitems;
	    }
	}
	
      // $B>/$J$/$H$b(B1$B$D$N%U%l!<%`%l!<%H$,%5%]!<%H$5$l$F$$$l$P!$$3$N(B
      // $B%U%)!<%^%C%H$,%5%]!<%H$5$l$F$$$k$3$H$K$J$k!%(B
	if (submenu != 0)
	{
	    gtk_widget_show(submenu);
	    GtkWidget*	item = gtk_menu_item_new_with_label(format[i].name);
	    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
	    gtk_menu_append(GTK_MENU(menu), item);
	    gtk_widget_show(item);
	}
	
    }
    item = gtk_menu_item_new_with_label("Format");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), menu);
    gtk_widget_show(item);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), item);

    gtk_widget_show(menubar);
    
    return menubar;
}
 
}
