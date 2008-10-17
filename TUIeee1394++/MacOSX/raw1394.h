/*
 *  $Id: raw1394.h,v 1.10 2008-10-17 00:24:48 ueshiba Exp $
 */
/*!
  \mainpage	libraw1394 - Linux$B>e$N(Blibraw1394$B$K8_49$J5!G=$r(BMacOS X$B>e$GDs6!$9$k%(%_%e%l!<%7%g%s%i%$%V%i%j(B
  \anchor	libraw1394

  \section copyright $BCx:n8"(B
  Copyright (C) 2003-2006 Toshio UESHIBA
  National Institute of Advanced Industrial Science and Technology (AIST)
 
  Written by Toshio UESHIBA <t.ueshiba@aist.go.jp>
 
  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA

  \section functions $B5!G=(B
  $B<!$N%$%s%?%U%'!<%9$rDs6!$7$F$$$k!%(B
  - raw1394_new_handle()
  - raw1394_destroy_handle()
  - raw1394_set_userdata()
  - raw1394_get_userdata()
  - raw1394_command_register_base()
  - raw1394_read()
  - raw1394_write()
  - raw1394_loop_iterate()
  - raw1394_iso_recv_init()
  - raw1394_iso_shutdown()
  - raw1394_iso_recv_start()
  - raw1394_iso_stop()
  - raw1394_iso_recv_flush()
  
  \file		raw1394.h
  \brief	$B%0%m!<%P%k$JL>A06u4V$KCV$+$l$?(BC$B8~$1$N7?Dj5A$H%$%s%?%U%'!<%9(B
*/
#ifndef _LIBRAW1394_RAW1394_H
#define _LIBRAW1394_RAW1394_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif
/************************************************************************
*  type definitions							*
************************************************************************/
/*! ::raw1394$B9=B$BN$X$N%O%s%I%k(B */
typedef struct raw1394*		raw1394handle_t;

typedef unsigned short		nodeid_t;
typedef unsigned long long	nodeaddr_t;

/*! asynchronous$BE>Aw%G!<%?$NC10L(B(4 bytes) */
typedef unsigned long		quadlet_t;

/*! ishochronous$BAw(B/$B<u?.%O%s%I%i$N=*N;>uBV$rI=$7!$$5$i$K$3$l$r8F$s$@(B
    raw1394_loop_iterate()$B$K$I$N$h$&$J=hM}$r$5$;$k$+$r;XDj$9$k%3!<%I(B
*/
enum raw1394_iso_disposition
{
  /*! $B%O%s%I%i$O@5>o$K=*N;!$<!$N%Q%1%C%H$r=hM}(B. */
    RAW1394_ISO_OK		= 0,
  /*! $B%O%s%I%i$O@5>o$K=*N;!$(B#raw1394_loop_iterate()$B$+$iD>$A$K%j%?!<%s(B */
    RAW1394_ISO_DEFER		= 1,
  /*! $B%O%s%I%i$K%(%i!<$,H/@8!$(B#raw1394_loop_iterate()$B$+$iD>$A$K%j%?!<%s(B */
    RAW1394_ISO_ERROR		= 2,
  /*! #raw1394_loop_iterate()$B$+$iD>$A$K%j%?!<%s$7$F(Bisochronous$B<u?.$rDd;_(B */
    RAW1394_ISO_STOP		= 3,
  /*! (isochronous$BAw?.;~$N$_;HMQ(B) */
    RAW1394_ISO_STOP_NOSYNC	= 4,
  /*! (isochronous$BAw?.;~$N$_;HMQ(B) */
    RAW1394_ISO_AGAIN		= 5
};

/*! isochronous$B<u?.$N%b!<%I(B */    
enum raw1394_iso_dma_recv_mode
{
    RAW1394_DMA_DEFAULT		  = -1,	/*!< $B%G%U%)%k%H%b!<%I(B */
    RAW1394_DMA_BUFFERFILL	  =  1,	/*!< BUFFER_FILL$B%b!<%I(B */
    RAW1394_DMA_PACKET_PER_BUFFER =  2	/*!< PACKET_PER_BUFFER$B%b!<%I(B */
};

/*! $B%+!<%M%k$,<u?.$7$?3F%Q%1%C%H$KBP$7$F<B9T$5$l$k%O%s%I%i(B
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \param data		$B%Q%1%C%HCf$N%G!<%?$X$N%]%$%s%?(B($B%X%C%@4^$^$:(B)
  \param len		$B%Q%1%C%HCf$N%G!<%?$N%P%$%H?t(B($B%X%C%@4^$^$:(B)
  \param channel	isochronous$B<u?.$N%A%c%s%M%k(B
  \param tag		$B%Q%1%C%H$N%X%C%@Cf$N(Btag$B%S%C%H(B
  \param sy		$B%Q%1%C%H$N%X%C%@Cf$N(Bsy$B%S%C%H(B
  \param cycle		$B%Q%1%C%H$N(Bcycle$BHV9f(B
  \param dropped	$B$H$j$3$\$7$?%Q%1%C%H?t(B
  \return
*/
typedef raw1394_iso_disposition	(*raw1394_iso_recv_handler_t)(
    raw1394handle_t	handle,
    unsigned char*	data,
    unsigned int	len,
    unsigned char	channel,
    unsigned char	tag,
    unsigned char	sy,
    unsigned int	cycle,
    unsigned int	dropped);

/************************************************************************
*  wrapper C functions							*
************************************************************************/
//! IEEE1394$B%$%s%?!<%U%'!<%9$G$"$k(B::raw1394$B9=B$BN$r@8@.$9$k(B
/*!
  \param unit_spec_ID	$B$3$N9=B$BN$,I=$9(BIEEE1394$B%N!<%I$N<oN`$r<($9(BID
  \param uniqId		$B8D!9$N5!4o8GM-$N(B64bit ID
  \return		$B@8@.$5$l$?(B::raw1394$B9=B$BN$X$N%O%s%I%k(B
*/
raw1394handle_t
	raw1394_new_handle(unsigned int unit_spec_ID,
			   unsigned long long uniqId);

//! IEEE1394$B%$%s%?!<%U%'!<%9$G$"$k(B::raw1394$B9=B$BN$rGK2u$9$k(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
*/
void	raw1394_destroy_handle(raw1394handle_t handle);

//! ::raw1394$B9=B$BN$K%f!<%6$,;XDj$7$?%G!<%?$X$N%]%$%s%?$rE=IU$1$k(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \param data		$BE=IU$1$?$$%G!<%?$X$N%]%$%s%?(B
*/
void	raw1394_set_userdata(raw1394handle_t handle, void* data);

//! ::raw1394$B9=B$BN$KE=IU$1$?%G!<%?$X$N%]%$%s%?$rF@$k(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \return		$BE=IU$1$?%G!<%?$X$N%]%$%s%?(B
*/
void*	raw1394_get_userdata(raw1394handle_t handle);

//! ::raw1394$B9=B$BN$,I=$9%N!<%I$N%3%^%s%I%l%8%9%?$N%Y!<%9%"%I%l%9$rJV$9(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \return		$B%3%^%s%I%l%8%9%?$N%Y!<%9%"%I%l%9(B
*/
nodeaddr_t
	raw1394_command_register_base(raw1394handle_t handle);

//! $B;XDj$7$?(BFireWire$B%"%I%l%9$+$iG$0U%P%$%H?t$N%G!<%?$r(Basynchronous$BE>Aw$GFI$_9~$`(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \param node		($B%@%_!<0z?t(B)
  \param addr		$BFI$_9~$_85$N(BFireWire$B%"%I%l%9(B
  \param length		$BFI$_9~$_%G!<%?$N%P%$%H?t(B
  \param quad		$BFI$_9~$_@h$N%P%C%U%!%"%I%l%9(B
  \return		$BFI$_9~$_$,@.8y$9$l$P(B0$B!$$=$&$G$J$1$l$P(B-1
*/
int	raw1394_read(raw1394handle_t handle, nodeid_t node,
		     nodeaddr_t addr, size_t length, quadlet_t* quad);

//! $B;XDj$7$?(BFireWire$B%"%I%l%9$KG$0U%P%$%H?t$N%G!<%?$r(Basynchronous$BE>Aw$G=q$-9~$`(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \param node		($B%@%_!<0z?t(B)
  \param addr		$B=q$-9~$_@h$N(BFireWire$B%"%I%l%9(B
  \param length		$B=q$-9~$_%G!<%?$N%P%$%H?t(B
  \param quad		$B=q$-9~$_85$N%P%C%U%!%"%I%l%9(B
  \return		$B=q$-9~$_$,@.8y$9$l$P(B0$B!$$=$&$G$J$1$l$P(B-1
*/
int	raw1394_write(raw1394handle_t handle, nodeid_t node,
		      nodeaddr_t addr, size_t length, quadlet_t* quad);

//! isochronous$BE>Aw$N%k!<%W$r(B1$B2s<B9T$9$k(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \return		$B%k!<%W$N<B9T$,@5>o$K=*N;$9$l$P(B0$B!$$=$&$G$J$1$l$P(B-1
*/
int	raw1394_loop_iterate(raw1394handle_t handle);

//! isochronous$B<u?.$N=i4|@_Dj$r9T$&(B
/*!
  \param handle			::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \param handler		$B%+!<%M%k$,<u?.$7$?3F%Q%1%C%H$KBP$7$F<B9T$5$l$k(B
				$B%O%s%I%i(B
  \param buf_packets		$B<u?.$9$k%Q%1%C%H?t(B
  \param max_packet_size	$B<u?.%Q%1%C%H$N:GBg%P%$%H?t(B
  \param channel		ishochronous$B<u?.$N%A%c%s%M%k(B
  \param mode			($B%@%_!<0z?t(B)
  \param irq_interval		$B%+!<%M%k$,3d$j9~$_$r$+$1$k4V3V$r;XDj$9$k(B
				$B%Q%1%C%H?t(B
  \return			$B=i4|@_Dj$,@.8y$9$l$P(B0$B!$$=$&$G$J$1$l$P(B-1
*/
int	raw1394_iso_recv_init(raw1394handle_t		     handle,
			      raw1394_iso_recv_handler_t     handler,
			      unsigned int		     buf_packets,
			      unsigned int		     max_packet_size,
			      unsigned char&		     channel,
			      raw1394_iso_dma_recv_mode	     mode,
			      int			     irq_interval);

//! isochronous$BE>Aw$rDd;_$7$FI,MW$J%j%=!<%9$r2rJ|$9$k(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
*/
void	raw1394_iso_shutdown(raw1394handle_t handle);

//! isochronous$B<u?.$r3+;O$9$k(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \param start_on_cycle	($B%@%_!<0z?t(B)
  \param tag_mask	($B%@%_!<0z?t(B)
  \param sync		($B%@%_!<0z?t(B)
  \return		$B3+;O$,@.8y$9$l$P(B0$B!$$=$&$G$J$1$l$P(B-1
*/
int	raw1394_iso_recv_start(raw1394handle_t handle,
			       int start_on_cycle, int tag_mask,
			       int sync);
//! isochronous$B<u?.$rDd;_$9$k(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
*/
void	raw1394_iso_stop(raw1394handle_t handle);

//! $B%+!<%M%k6u4V$KJ];}$5$l$F$$$k(Bisochronous$B<u?.%G!<%?$r%f!<%66u4V$KE>Aw$9$k(B
/*!
  \param handle		::raw1394$B9=B$BN$X$N%O%s%I%k(B
  \return		$BE>Aw$,@.8y$9$l$P(B0, $B$=$&$G$J$1$l$P(B-1
*/
int	raw1394_iso_recv_flush(raw1394handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* _LIBRAW1394_RAW1394_H */
