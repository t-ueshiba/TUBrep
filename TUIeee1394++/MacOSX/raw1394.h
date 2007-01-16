/*
 * libTUIeee1394++: C++ Library for Controlling IIDC 1394-based Digital Cameras
 * Copyright (C) 2003-2006 Toshio UESHIBA
 *   National Institute of Advanced Industrial Science and Technology (AIST)
 *
 * Written by Toshio UESHIBA <t.ueshiba@aist.go.jp>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  $Id: raw1394.h,v 1.5 2007-01-16 08:18:43 ueshiba Exp $
 */
/*!
 \file
 \brief	libraw1394�Ɍ݊��ȋ@�\��MacOS X��Œ񋟂���G�~�����[�V�������C�u����
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
/*! ::raw1394�\���̂ւ̃n���h�� */
typedef struct raw1394*		raw1394handle_t;

typedef unsigned short		nodeid_t;
typedef unsigned long long	nodeaddr_t;

/*! asynchronous�]���f�[�^�̒P��(4 bytes) */
typedef unsigned long		quadlet_t;

/*! ishochronous��/��M�n���h���̏I����Ԃ�\���C����ɂ�����Ă�
    raw1394_loop_iterate()�ɂǂ̂悤�ȏ����������邩���w�肷��R�[�h
*/
enum raw1394_iso_disposition
{
  /*! �n���h���͐���ɏI���C���̃p�P�b�g������. */
    RAW1394_ISO_OK		= 0,
  /*! �n���h���͐���ɏI���C#raw1394_loop_iterate()���璼���Ƀ��^�[�� */
    RAW1394_ISO_DEFER		= 1,
  /*! �n���h���ɃG���[�������C#raw1394_loop_iterate()���璼���Ƀ��^�[�� */
    RAW1394_ISO_ERROR		= 2,
  /*! #raw1394_loop_iterate()���璼���Ƀ��^�[������isochronous��M���~ */
    RAW1394_ISO_STOP		= 3,
  /*! (isochronous���M���̂ݎg�p) */
    RAW1394_ISO_STOP_NOSYNC	= 4,
  /*! (isochronous���M���̂ݎg�p) */
    RAW1394_ISO_AGAIN		= 5
};

/*! isochronous��M�̃��[�h */    
enum raw1394_iso_dma_recv_mode
{
    RAW1394_DMA_DEFAULT		  = -1,	/*!< �f�t�H���g���[�h */
    RAW1394_DMA_BUFFERFILL	  =  1,	/*!< BUFFER_FILL���[�h */
    RAW1394_DMA_PACKET_PER_BUFFER =  2	/*!< PACKET_PER_BUFFER���[�h */
};

/*! �J�[�l������M�����e�p�P�b�g�ɑ΂��Ď��s�����n���h��
  \param handle		::raw1394�\���̂ւ̃n���h��
  \param data		�p�P�b�g���̃f�[�^�ւ̃|�C���^(�w�b�_�܂܂�)
  \param len		�p�P�b�g���̃f�[�^�̃o�C�g��(�w�b�_�܂܂�)
  \param channel	isochronous��M�̃`�����l��
  \param tag		�p�P�b�g�̃w�b�_����tag�r�b�g
  \param sy		�p�P�b�g�̃w�b�_����sy�r�b�g
  \param cycle		�p�P�b�g��cycle�ԍ�
  \param dropped	�Ƃ肱�ڂ����p�P�b�g��
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
//! IEEE1394�C���^�[�t�F�[�X�ł���::raw1394�\���̂𐶐�����
/*!
  \param unit_spec_ID	���̍\���̂��\��IEEE1394�m�[�h�̎�ނ�����ID
  \param uniqId		�X�̋@��ŗL��64bit ID
  \return		�������ꂽ::raw1394�\���̂ւ̃n���h��
*/
raw1394handle_t
	raw1394_new_handle(unsigned int unit_spec_ID,
			   unsigned long long uniqId);

//! IEEE1394�C���^�[�t�F�[�X�ł���::raw1394�\���̂�j�󂷂�
/*!
  \param handle		�j�󂷂�::raw1394�\���̂ւ̃n���h��
*/
void	raw1394_destroy_handle(raw1394handle_t handle);

//! ::raw1394�\���̂Ƀ��[�U���w�肵���f�[�^�ւ̃|�C���^��\�t����
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
  \param data		�\�t�������f�[�^�ւ̃|�C���^
*/
void	raw1394_set_userdata(raw1394handle_t handle, void* data);

//! ::raw1394�\���̂ɓ\�t�����f�[�^�ւ̃|�C���^�𓾂�
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
  \return		�\�t�����f�[�^�ւ̃|�C���^
*/
void*	raw1394_get_userdata(raw1394handle_t handle);

//! ::raw1394�\���̂��\���m�[�h�̃R�}���h���W�X�^�̃x�[�X�A�h���X��Ԃ�
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
  \return		�R�}���h���W�X�^�̃x�[�X�A�h���X
*/
nodeaddr_t
	raw1394_command_register_base(raw1394handle_t handle);

//! �w�肵��FireWire�A�h���X����C�Ӄo�C�g���̃f�[�^��asynchronous�]���œǂݍ���
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
  \param node		(�_�~�[����)
  \param addr		�ǂݍ��݌���FireWire�A�h���X
  \param length		�ǂݍ��݃f�[�^�̃o�C�g��
  \param quad		�ǂݍ��ݐ�̃o�b�t�@�A�h���X
  \return		�ǂݍ��݂����������0�C�����łȂ����-1
*/
int	raw1394_read(raw1394handle_t handle, nodeid_t node,
		     nodeaddr_t addr, size_t length, quadlet_t* quad);

//! �w�肵��FireWire�A�h���X�ɔC�Ӄo�C�g���̃f�[�^��asynchronous�]���ŏ�������
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
  \param node		(�_�~�[����)
  \param addr		�������ݐ��FireWire�A�h���X
  \param length		�������݃f�[�^�̃o�C�g��
  \param quad		�������݌��̃o�b�t�@�A�h���X
  \return		�������݂����������0�C�����łȂ����-1
*/
int	raw1394_write(raw1394handle_t handle, nodeid_t node,
		      nodeaddr_t addr, size_t length, quadlet_t* quad);

//! isochronous�]���̃��[�v��1����s����
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
  \return		���[�v�̎��s������ɏI�������0�C�����łȂ����-1
*/
int	raw1394_loop_iterate(raw1394handle_t handle);

//! isochronous��M�̏����ݒ���s��
/*!
  \param handle			::raw1394�\���̂ւ̃n���h��
  \param handler		�J�[�l������M�����e�p�P�b�g�ɑ΂��Ď��s�����
				�n���h��
  \param buf_packets		��M����p�P�b�g��
  \param max_packet_size	��M�p�P�b�g�̍ő�o�C�g��
  \param channel		ishochronous��M�̃`�����l��
  \param mode			(�_�~�[����)
  \param irq_interval		�J�[�l�������荞�݂�������Ԋu���w�肷��
				�p�P�b�g��
  \return			�����ݒ肪���������0�C�����łȂ����-1
*/
int	raw1394_iso_recv_init(raw1394handle_t		     handle,
			      raw1394_iso_recv_handler_t     handler,
			      unsigned int		     buf_packets,
			      unsigned int		     max_packet_size,
			      unsigned char&		     channel,
			      raw1394_iso_dma_recv_mode	     mode,
			      int			     irq_interval);

//! isochronous�]�����~���ĕK�v�ȃ��\�[�X���������
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
*/
void	raw1394_iso_shutdown(raw1394handle_t handle);

//! isochronous��M���J�n����
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
  \param start_on_cycle	(�_�~�[����)
  \param tag_mask	(�_�~�[����)
  \param sync		(�_�~�[����)
  \return		�J�n�����������0�C�����łȂ����-1
*/
int	raw1394_iso_recv_start(raw1394handle_t handle,
			       int start_on_cycle, int tag_mask,
			       int sync);
//! isochronous��M���~����
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
*/
void	raw1394_iso_stop(raw1394handle_t handle);

//! �J�[�l����Ԃɕێ�����Ă���isochronous��M�f�[�^�����[�U��Ԃɓ]������
/*!
  \param handle		::raw1394�\���̂ւ̃n���h��
  \return		�]�������������0, �����łȂ����-1
*/
int	raw1394_iso_recv_flush(raw1394handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* _LIBRAW1394_RAW1394_H */
