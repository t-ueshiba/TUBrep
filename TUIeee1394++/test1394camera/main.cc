/*
 *  $Id: main.cc,v 1.8 2009-05-13 01:15:01 ueshiba Exp $
 */
/*!
  \mainpage	test1394camera - program for testing an IIDC 1394-based Digital Camera
  \anchor	test1394camera

  \section copyright $BCx:n8"(B
  Copyright (C) 2003 Toshio UESHIBA
  National Institute of Advanced Industrial Science and Technology (AIST)

  Written by Toshio UESHIBA <t.ueshiba@aist.go.jp>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

  \section functions $B5!G=(B
  IEEE1394$B%G%8%?%k%+%a%i$N%F%9%H%W%m%0%i%`!%(B1$BBf$N%+%a%i$KBP$7$F(B
  $B<o!9$N@_Dj$r9T$C$?$j!$;#1F$7$?2hA|%9%H%j!<%`$r%j%"%k%?%$%`$G(B
  X window$B>e$KI=<($7$?$j$G$-$k!%(BLINUX$B>e$G(B
  <a href="http://www.1394ta.com/">IEEE1394</a>$B%G%P%$%9$*$h$S(B
  <a href="http://www.1394ta.com/Technology/Specifications/Descriptions/IIDC_Spec_v1_30.htm">IEEE1394$B%G%8%?%k%+%a%i(B</a>$B$r;HMQ$9$k$?$a$N%3%s%H%m!<%k%i%$%V%i%j(B:
  \ref libTUIeee1394 "libTUIeee1394++"$B$rMQ$$$F$$$k!%(B

  \section invocation $B%3%^%s%I8F$S=P$7$N7A<0(B
  \verbatim
  test1394camera [-b] [uniqueID]\endverbatim

    - [<tt>-b</tt>] IEEE1394b (FireWire 800)$B%b!<%I$r;HMQ(B
    - [<tt>uniqueID</tt>] $B%+%a%i$,J#?t$"$k>l9g$KFCDj$N%+%a%i$r;XDj$9$k$?$a$N(Bglobal
	uniqne ID$B$r(B16$B?J7A<0(B(0x####)$B$GM?$($k(B

  $B%W%m%0%i%`=*N;;~$K!$%+%a%i$N(Bglobal unique ID$B$H@_DjCM$,I8=`=PNO$K=PNO$5$l$k!%(B	
*/
#if HAVE_CONFIG_H
#  include <config.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include "My1394Camera.h"

namespace TU
{
GtkWidget*	createMenubar(My1394Camera& camera)			;
GtkWidget*	createCommands(My1394Camera& camera)			;

/************************************************************************
*  static functions							*
************************************************************************/
//! $B;HMQK!$r@bL@$9$k(B
/*!
  \param s	$B%3%^%s%IL>(B
*/
static void
usage(const char* s)
{
    using namespace	std;
    
    cerr << "\nControl an IEEE1394 digital camera.\n"
	 << endl;
    cerr << " Usage: " << s << " [-b] [uniqueID]"
         << endl;
    cerr << " arguments.\n"
         << "  -b:       IEEE1394b(800Mbps) mode\n"
         << "  uniqueID: camera unique-ID in hex format"
	 << " (i.e. 0x####, default: arbitrary)\n"
         << endl;
}

}
/************************************************************************
*  global functions							*
************************************************************************/
//! $B%a%$%s4X?t(B
/*!
  "-p <port num>" $B$G(BIEEE1394$B%]!<%H(B($B%$%s%?!<%U%'!<%9%+!<%I(B)$B$NHV9f$r;XDj$9$k!%(B
  \param argc	$B0z?t$N?t(B($B%3%^%s%IL>$r4^$`(B)
  \param argv   $B0z?tJ8;zNs$NG[Ns(B
*/
int
main(int argc, char* argv[])
{
    using namespace	std;
    using namespace	TU;
    
    gtk_init(&argc, &argv);	// GTK+ $B$N=i4|2=(B.

  // IEEE1394$B%]!<%H(B($B%$%s%?!<%U%'!<%9%+!<%I(B)$B$NHV9f$r%3%^%s%I9T$+$iFI$_9~$`!%(B
    bool		i1394b = false;		// default: $BHs(BIEEE1394b$B%b!<%I(B
    extern char*	optarg;
    for (int c; (c = getopt(argc, argv, "bh")) != EOF; )
	switch (c)
	{
	  case 'b':
	    i1394b = true;
	    break;
	  case 'h':
	    usage(argv[0]);
	    return 1;
	}
    extern int	optind;
    u_int64	uniqId = 0;
    if (optind < argc)
	uniqId = strtoull(argv[optind], 0, 0);
    
  // $BK\6H$r9T$&!%(B
    try
    {
	My1394Camera	camera(i1394b, uniqId);		// $B%+%a%i$r3+$/!%(B

	GtkWidget*	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "IEEE1394 camera controller");
	gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, TRUE);
	gtk_signal_connect(GTK_OBJECT(window), "destroy",
			   GTK_SIGNAL_FUNC(gtk_exit), NULL);
	gtk_signal_connect(GTK_OBJECT(window), "delete_event",
			   GTK_SIGNAL_FUNC(gtk_exit), NULL);

	GtkWidget*	table = gtk_table_new(2, 2, FALSE);
	gtk_container_add(GTK_CONTAINER(window), table);
	gtk_table_attach(GTK_TABLE(table), createMenubar(camera), 0, 2, 0, 1,
			 GTK_FILL, GTK_SHRINK, 0, 0);
	gtk_table_attach(GTK_TABLE(table), createCommands(camera), 1, 2, 1, 2,
			 GTK_SHRINK, GTK_SHRINK, 5, 0);
	gtk_table_attach(GTK_TABLE(table), camera.canvas(), 0, 1, 1, 2,
			 GTK_SHRINK, GTK_SHRINK, 0, 0);
	gtk_widget_show_all(window);

	gtk_main();
    }
    catch (exception& err)
    {
	cerr << err.what() << endl;
	return 1;
    }

    return 0;
}
