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
 *  $Id: MyDialog.cc,v 1.4 2008-10-17 06:31:42 ueshiba Exp $
 */
#if HAVE_CONFIG_H
#  include <config.h>
#endif
#include "MyDialog.h"

namespace TU
{
/************************************************************************
*  local data								*
************************************************************************/
/*!
  Format_7_x$B$G%5%]!<%H$5$l$k2hAG%U%)!<%^%C%H$H$=$NL>>N!%(B
*/
struct MyPixelFormat
{
    const Ieee1394Camera::PixelFormat	pixelFormat;	//!< $B2hAG%U%)!<%^%C%H(B
    const char* const			name;		//!< $B$=$NL>>N(B
    Ieee1394Camera::PixelFormat*	pixelFormatDst;	//!< $B%U%)!<%^%C%H@_Dj@h(B
};
static MyPixelFormat	pixelFormat[] =
{
    {Ieee1394Camera::MONO_8,		"Y(mono)",		0},
    {Ieee1394Camera::YUV_411,		"YUV(4:1:1)",		0},
    {Ieee1394Camera::YUV_422,		"YUV(4:2:2)",		0},
    {Ieee1394Camera::YUV_444,		"YUV(4:4:4)",		0},
    {Ieee1394Camera::RGB_24,		"RGB",			0},
    {Ieee1394Camera::MONO_16,		"Y(mono16)",		0},
    {Ieee1394Camera::RGB_48,		"RGB(color48)",		0},
    {Ieee1394Camera::SIGNED_MONO_16,	"Y(signed mono16)",	0},
    {Ieee1394Camera::SIGNED_RGB_48,	"RGB(signed color48)",	0},
    {Ieee1394Camera::RAW_8,		"RAW(raw8)",		0},
    {Ieee1394Camera::RAW_16,		"RAW(raw16)",		0}
};
static const int NPIXELFORMATS = sizeof(pixelFormat)/sizeof(pixelFormat[0]);

/************************************************************************
*  callback functions							*
************************************************************************/
//! ROI$B$N86E@$d%5%$%:$rM?$($i$l$?%9%F%C%WC10L$GJQ2=$5$;$k$?$a$N%3!<%k%P%C%/4X?t!%(B
/*!
  \param adj		ROI$B$N86E@$d%5%$%:$rJ];}$7$F$$$k(B adjuster
  \param userdata	$BJQ2=$N%9%F%C%W(B
*/
static void
CBadjust(GtkAdjustment* adj, gpointer userdata)
{
    u_int	step = *(u_int*)userdata;
    gfloat	val = gfloat(step * ((u_int(adj->value) + step/2) / step));
    gtk_adjustment_set_value(adj, val);
}
    
//! $B2hAG7A<0$r;XDj$9$k$?$a$N%3!<%k%P%C%/4X?t!%(B
/*!
  \param userdata	SettingPixelFormat ($B2hAG7A<0$N@_DjCM$H@_Dj@h$N(B2$B%DAH(B)
*/
static void
CBmenuitem(GtkMenuItem*, gpointer userdata)
{
    MyPixelFormat*	pixelFormat = (MyPixelFormat*)userdata;
    *(pixelFormat->pixelFormatDst) = pixelFormat->pixelFormat;
}
    
/************************************************************************
*  class MyDialog							*
************************************************************************/
//! ROI$B$r;XDj$9$k$?$a$N(Bdialog window$B$r@8@.$9$k(B
/*!
  \param fmt7info	ROI$B$r;XDj$7$?$$(BFormat_7_x$B$K4X$9$k>pJs!%(B
*/
MyDialog::MyDialog(const Ieee1394Camera::Format_7_Info& fmt7info)
    :_fmt7info(fmt7info),
     _dialog(gtk_dialog_new()),
     _u0(gtk_adjustment_new(_fmt7info.u0, 0, _fmt7info.maxWidth,
			    _fmt7info.unitU0, _fmt7info.unitU0, 0.0)),
     _v0(gtk_adjustment_new(_fmt7info.v0, 0, _fmt7info.maxHeight,
			    _fmt7info.unitV0, _fmt7info.unitV0, 0.0)),
     _width(gtk_adjustment_new(_fmt7info.width, 0, _fmt7info.maxWidth,
			       _fmt7info.unitWidth, _fmt7info.unitWidth, 0.0)),
     _height(gtk_adjustment_new(_fmt7info.height, 0, _fmt7info.maxHeight,
				_fmt7info.unitHeight, _fmt7info.unitHeight,
				0.0)),
     _pixelFormat(_fmt7info.pixelFormat)
{
  // dialog$B$NB0@-$r@_Dj!%(B
    gtk_window_set_modal(GTK_WINDOW(_dialog), TRUE);
    gtk_window_set_title(GTK_WINDOW(_dialog), "Setting ROI for Format_7_x");
    gtk_window_set_policy(GTK_WINDOW(_dialog), FALSE, FALSE, TRUE);
    
  // $BJQ2=$r%9%F%C%WC10L$K6/@)$9$k$?$a!$%3!<%k%P%C%/4X?t$rEPO?!%(B
    gtk_signal_connect(GTK_OBJECT(_u0), "value_changed",
		       GTK_SIGNAL_FUNC(CBadjust),
		       (gpointer)&_fmt7info.unitU0);
    gtk_signal_connect(GTK_OBJECT(_v0), "value_changed",
		       GTK_SIGNAL_FUNC(CBadjust),
		       (gpointer)&_fmt7info.unitV0);
    gtk_signal_connect(GTK_OBJECT(_width), "value_changed",
		       GTK_SIGNAL_FUNC(CBadjust),
		       (gpointer)&_fmt7info.unitWidth);
    gtk_signal_connect(GTK_OBJECT(_height), "value_changed",
		       GTK_SIGNAL_FUNC(CBadjust),
		       (gpointer)&_fmt7info.unitHeight);
    
  // widget$B$rJB$Y$k$?$a$N(Btable$B!%(B
    GtkWidget*	table = gtk_table_new(2, 5, FALSE);

  // 4$B$D$N(Badjustment$B$N$?$a$N(Blabel$B$HA`:nMQ(Bscale$B$r@8@.$7$F(Btable$B$K%Q%C%/!%(B
    GtkWidget*	label = gtk_label_new("u0:");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);
    GtkWidget*	scale = gtk_hscale_new(GTK_ADJUSTMENT(_u0));
    gtk_scale_set_digits(GTK_SCALE(scale), 0);
    gtk_widget_set_usize(GTK_WIDGET(scale), 200, 30);
    gtk_table_attach_defaults(GTK_TABLE(table), scale, 1, 2, 0, 1);
    
    label = gtk_label_new("v0:");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);
    scale = gtk_hscale_new(GTK_ADJUSTMENT(_v0));
    gtk_scale_set_digits(GTK_SCALE(scale), 0);
    gtk_widget_set_usize(GTK_WIDGET(scale), 200, 30);
    gtk_table_attach_defaults(GTK_TABLE(table), scale, 1, 2, 1, 2);
    
    label = gtk_label_new("width:");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 2, 3);
    scale = gtk_hscale_new(GTK_ADJUSTMENT(_width));
    gtk_scale_set_digits(GTK_SCALE(scale), 0);
    gtk_widget_set_usize(GTK_WIDGET(scale), 200, 30);
    gtk_table_attach_defaults(GTK_TABLE(table), scale, 1, 2, 2, 3);
    
    label = gtk_label_new("height:");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 3, 4);
    scale = gtk_hscale_new(GTK_ADJUSTMENT(_height));
    gtk_scale_set_digits(GTK_SCALE(scale), 0);
    gtk_widget_set_usize(GTK_WIDGET(scale), 200, 30);
    gtk_table_attach_defaults(GTK_TABLE(table), scale, 1, 2, 3, 4);

  // PixelFormat$B@_DjMQ$N%*%W%7%g%s%a%K%e!<$r@8@.$7$F(Btable$B$K%Q%C%/!%(B
    GtkWidget*	menu = gtk_menu_new();
    guint	current = 0;
    for (int nitems = 0, i = 0; i < NPIXELFORMATS; ++i)
	if (_fmt7info.availablePixelFormats & pixelFormat[i].pixelFormat)
	{
	    GtkWidget*	item
		= gtk_menu_item_new_with_label(pixelFormat[i].name);
	    gtk_menu_append(GTK_MENU(menu), item);
	    pixelFormat[i].pixelFormatDst = &_pixelFormat;
	    gtk_signal_connect(GTK_OBJECT(item), "activate",
			       GTK_SIGNAL_FUNC(CBmenuitem),
			       (gpointer)&pixelFormat[i]);
	    if (_fmt7info.pixelFormat == pixelFormat[i].pixelFormat)
		current = nitems;
	    ++nitems;
	}
    GtkWidget*	option_menu = gtk_option_menu_new();
    gtk_option_menu_set_menu(GTK_OPTION_MENU(option_menu), menu);
    gtk_option_menu_set_history(GTK_OPTION_MENU(option_menu), current);
    gtk_table_attach_defaults(GTK_TABLE(table), option_menu, 1, 2, 4, 5);
    
  // ROI$B@_DjMQ$N%\%?%s$r@8@.$7$F(Btable$B$K%Q%C%/!%(B
    GtkWidget*	button = gtk_button_new_with_label("Set");
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
		       GTK_SIGNAL_FUNC(gtk_main_quit), _dialog);
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 2, 5, 6);

  // table$B$r(Bdialog$B$K<}$a$k!%(B
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(_dialog)->action_area), table);

  // $B%a%C%;!<%8$r(Bdialog$B$K<}$a$k!%(B
    label = gtk_label_new("Set the Region of Interest(ROI).");
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(_dialog)->vbox), label);
    
    gtk_widget_show_all(_dialog);

    gtk_main();			// ROI$B@_DjMQ$N%\%?%s$r2!$9$H%k!<%W$rH4$1=P$9!%(B
}

//! ROI$B$r;XDj$9$k$?$a$N(Bdialog window$B$rGK2u$9$k(B
MyDialog::~MyDialog()
{
    gtk_widget_destroy(_dialog);
}

//! $B;XDj$5$l$?(BROI$B$r(Bdialog window$B$+$iFI$_<h$k(B
/*!
  \param u0	ROI$B$N:8>e6y$N2#:BI8!%(B
  \param v0	ROI$B$N:8>e6y$N=D:BI8!%(B
  \param width	ROI$B$NI}!%(B
  \param height	ROI$B$N9b$5!%(B
 */
Ieee1394Camera::PixelFormat
MyDialog::getROI(u_int& u0, u_int& v0, u_int& width, u_int& height)
{
    u0	   = u_int(GTK_ADJUSTMENT(_u0)->value);
    v0	   = u_int(GTK_ADJUSTMENT(_v0)->value);
    width  = u_int(GTK_ADJUSTMENT(_width)->value);
    height = u_int(GTK_ADJUSTMENT(_height)->value);

    return _pixelFormat;
}
    
};
