/*
 * test1394camera: test program controlling an IIDC 1394-based Digital Camera
 * Copyright (C) 2003 Toshio UESHIBA
 *   National Institute of Advanced Industrial Science and Technology (AIST)
 *
 * Written by Toshio UESHIBA <t.ueshiba@aist.go.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  $Id: createMenubar.cc,v 1.10 2008-10-17 06:31:43 ueshiba Exp $
 */
#if HAVE_CONFIG_H
#  include <config.h>
#endif
#include "My1394Camera.h"
#include "MyDialog.h"
#include <iostream>
#include <iomanip>
#include <fstream>

#define DEFAULT_IMAGE_FILE_NAME	"test1394camera.pbm"

namespace TU
{
/************************************************************************
*  local data								*
************************************************************************/
/*!
  $B%+%a%i$,%5%]!<%H$9$k2hA|%U%)!<%^%C%H$H$=$NL>>N!%(B
*/
struct MyFormat
{
    const Ieee1394Camera::Format	format;		//!< $B2hA|%U%)!<%^%C%H(B
    const char* const			name;		//!< $B$=$NL>>N(B
    
};
static const MyFormat	format[] =
{
    {Ieee1394Camera::YUV444_160x120,   "160x120-YUV(4:4:4)"},
    {Ieee1394Camera::YUV422_320x240,   "320x240-YUV(4:2:2)"},
    {Ieee1394Camera::YUV411_640x480,   "640x480-YUV(4:1:1)"},
    {Ieee1394Camera::YUV422_640x480,   "640x480-YUV(4:2:2)"},
    {Ieee1394Camera::RGB24_640x480,    "640x480-RGB"},
    {Ieee1394Camera::MONO8_640x480,    "640x480-Y(mono)"},
    {Ieee1394Camera::MONO16_640x480,   "640x480-Y(mono16)"},
    {Ieee1394Camera::YUV422_800x600,   "800x600-YUV(4:2:2)"},
    {Ieee1394Camera::RGB24_800x600,    "800x600-RGB"},
    {Ieee1394Camera::MONO8_800x600,    "800x600-Y(mono)"},
    {Ieee1394Camera::YUV422_1024x768,  "1024x768-YUV(4:2:2)"},
    {Ieee1394Camera::RGB24_1024x768,   "1024x768-RGB"},
    {Ieee1394Camera::MONO8_1024x768,   "1024x768-Y(mono)"},
    {Ieee1394Camera::MONO16_800x600,   "800x600-Y(mono16)"},
    {Ieee1394Camera::MONO16_1024x768,  "1024x768-Y(mono16)"},
    {Ieee1394Camera::YUV422_1280x960,  "1280x960-YUV(4:2:2)"},
    {Ieee1394Camera::RGB24_1280x960,   "1280x960-RGB"},
    {Ieee1394Camera::MONO8_1280x960,   "1280x960-Y(mono)"},
    {Ieee1394Camera::YUV422_1600x1200, "1600x1200-YUV(4:2:2)"},
    {Ieee1394Camera::RGB24_1600x1200,  "1600x1200-RGB"},
    {Ieee1394Camera::MONO8_1600x1200,  "1600x1200-Y(mono)"},
    {Ieee1394Camera::MONO16_1280x960,  "1280x960-Y(mono16)"},
    {Ieee1394Camera::MONO16_1600x1200, "1600x1200-Y(mono16)"},
    {Ieee1394Camera::Format_7_0,       "Format_7_0"},
    {Ieee1394Camera::Format_7_1,       "Format_7_1"},
    {Ieee1394Camera::Format_7_2,       "Format_7_2"},
    {Ieee1394Camera::Format_7_3,       "Format_7_3"},
    {Ieee1394Camera::Format_7_4,       "Format_7_4"},
    {Ieee1394Camera::Format_7_5,       "Format_7_5"},
    {Ieee1394Camera::Format_7_6,       "Format_7_6"},
    {Ieee1394Camera::Format_7_6,       "Format_7_7"}
};
static const int	NFORMATS = sizeof(format)/sizeof(format[0]);

/*!
  $B%+%a%i$,%5%]!<%H$9$k%U%l!<%`%l!<%H$H$=$NL>>N!%(B
*/
struct MyFrameRate
{
    const Ieee1394Camera::FrameRate	frameRate;	//!< $B%U%l!<%`%l!<%H(B
    const char* const			name;		//!< $B$=$NL>>N(B
};
static const MyFrameRate	frameRate[] =
{
    {Ieee1394Camera::FrameRate_1_875, "1.875fps"},
    {Ieee1394Camera::FrameRate_3_75,  "3.75fps"},
    {Ieee1394Camera::FrameRate_7_5,   "7.5fps"},
    {Ieee1394Camera::FrameRate_15,    "15fps"},
    {Ieee1394Camera::FrameRate_30,    "30fps"},
    {Ieee1394Camera::FrameRate_60,    "60fps"},
    {Ieee1394Camera::FrameRate_120,   "120fps"},
    {Ieee1394Camera::FrameRate_240,   "240fps"},
    {Ieee1394Camera::FrameRate_x,     "custom"}
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

/*!
  $B%+%a%i$H(Bfile selection widget$B$N(B2$B%DAH!%%3!<%k%P%C%/4X?t(B:
  CBfileSelectionOK() $B$N0z?t$H$7$FEO$5$l$k!%(B
 */
struct CameraAndFileSelection
{
    My1394Camera*		camera;
    GtkWidget*			filesel;
};
static CameraAndFileSelection	cameraAndFileSelection;

/************************************************************************
*  static functions							*
************************************************************************/
static std::ostream&
operator <<(std::ostream& out, const My1394Camera& camera)
{
    using namespace	std;
    
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
	u_int	inq = camera.inquireFeatureFunction(feature[i].feature);
	if ((inq & Ieee1394Camera::Presence) &&
	    (inq & Ieee1394Camera::Manual))
	{
	    out << ' ' << feature[i].name << ' ';

	    if ((inq & Ieee1394Camera::Auto) && 
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
    if (fmtAndFRate->format >= Ieee1394Camera::Format_7_0)
    {
	MyDialog	dialog(fmtAndFRate->camera->
			       getFormat_7_Info(fmtAndFRate->format));
	u_int		u0, v0, width, height;
	Ieee1394Camera::PixelFormat
			pixelFormat = dialog.getROI(u0, v0, width, height);
	fmtAndFRate->camera->
	    setFormat_7_ROI(fmtAndFRate->format, u0, v0, width, height).
	    setFormat_7_PixelFormat(fmtAndFRate->format, pixelFormat);
    }
    fmtAndFRate->camera->setFormatAndFrameRate(fmtAndFRate->format,
					       fmtAndFRate->frameRate);
}

//! $BA*Br$5$l$?%U%!%$%k$K2hA|$r%;!<%V$9$k$?$a$N%3!<%k%P%C%/4X?t!%(B
/*!
  \param userdata	My1394Camera (IEEE1394$B%+%a%i(B)
*/
static void
CBfileSelectionOK(GtkWidget* filesel, gpointer userdata)
{
    CameraAndFileSelection*	camAndFSel = (CameraAndFileSelection*)userdata;
    std::ofstream	out(gtk_file_selection_get_filename(
				GTK_FILE_SELECTION(camAndFSel->filesel)));
    if (out)
	camAndFSel->camera->save(out);
    gtk_widget_destroy(camAndFSel->filesel);
}

//! $B2hA|$r%;!<%V$9$k%U%!%$%k$rA*Br$9$k(Bdialog$B$rI=<($9$k$?$a$N%3!<%k%P%C%/4X?t!%(B
/*!
  \param userdata	My1394Camera (IEEE1394$B%+%a%i(B)
*/
static void
CBsave(GtkMenuItem*, gpointer userdata)
{
    GtkWidget*		filesel = gtk_file_selection_new("Save image");
    gtk_signal_connect(GTK_OBJECT(filesel), "destroy",
		       GTK_SIGNAL_FUNC(gtk_main_quit), filesel);
    cameraAndFileSelection.camera  = (My1394Camera*)userdata;
    cameraAndFileSelection.filesel = filesel;
    gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button),
		       "clicked", (GtkSignalFunc)CBfileSelectionOK,
		       &cameraAndFileSelection);
    gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION(filesel)
					 ->cancel_button), "clicked",
			      (GtkSignalFunc)gtk_widget_destroy,
			      GTK_OBJECT(filesel));
    gtk_file_selection_set_filename(GTK_FILE_SELECTION(filesel),
				    DEFAULT_IMAGE_FILE_NAME);
    gtk_widget_show(filesel);
    gtk_main();
}

//! $B%+%a%i$N@_DjCM$rI8=`=PNO$K=q$-=P$7$F=*N;$9$k$?$a$N%3!<%k%P%C%/4X?t!%(B
/*!
  \param userdata	My1394Camera (IEEE1394$B%+%a%i(B)
*/
static void
CBexit(GtkMenuItem*, gpointer userdata)
{
    using namespace	std;
    
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
    GtkWidget*	item = gtk_menu_item_new_with_label("Save");
    gtk_signal_connect(GTK_OBJECT(item), "activate",
		       GTK_SIGNAL_FUNC(CBsave), &camera);
    gtk_menu_append(GTK_MENU(menu), item);
    item = gtk_menu_item_new_with_label("Quit");
    gtk_signal_connect(GTK_OBJECT(item), "activate",
		       GTK_SIGNAL_FUNC(CBexit), &camera);
    gtk_menu_append(GTK_MENU(menu), item);
    item = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), menu);
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
	u_int		inq = camera.inquireFrameRate(format[i].format);
	GtkWidget*	submenu = 0;
	for (int j = 0; j < NRATES; ++j) // $BA4$F$N%U%l!<%`%l!<%H$K$D$$$F(B...
	{
	  // $B$3$N%U%l!<%`%l!<%H$,%5%]!<%H$5$l$F$$$k$+D4$Y$k!%(B
	    if (inq & frameRate[j].frameRate)
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
		++nitems;
	    }
	}
	
      // $B>/$J$/$H$b(B1$B$D$N%U%l!<%`%l!<%H$,%5%]!<%H$5$l$F$$$l$P!$$3$N(B
      // $B%U%)!<%^%C%H$,%5%]!<%H$5$l$F$$$k$3$H$K$J$k!%(B
	if (submenu != 0)
	{
	    GtkWidget*	item = gtk_menu_item_new_with_label(format[i].name);
	    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
	    gtk_menu_append(GTK_MENU(menu), item);
	}
	
    }
    item = gtk_menu_item_new_with_label("Format");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), menu);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), item);

    gtk_widget_show_all(menubar);
    
    return menubar;
}
 
}
