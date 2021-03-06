/*
 * ParaStation
 *
 * Copyright (C) 2010 ParTec Cluster Competence Center GmbH, Munich
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined in the file LICENSE.QPL included in the packaging of this
 * file.
 *
 * Author:	Jens Hauke <hauke@par-tec.com>
 */
/**
 * psextoll.c: EXTOLL communication
 */

#ifndef _PSEXTOLL_H_
#define _PSEXTOLL_H_

#include <stdint.h>
#include <stdio.h>
#include <sys/uio.h>

#define DISABLE_RMA2 1

// Compat stuff for missing Extoll includes:
//typedef struct RMA2_Connection_s RMA2_Connection;
//typedef struct RMA2_Endpoint_s RMA2_Endpoint;
//typedef struct RMA2_Region_s RMA2_Region;
#ifndef DISABLE_RMA2
#include "rma2.h" /* Extoll librma interface */
#endif
#include "velo2.h" /* Extoll libvelo interface */

/* rma2.h includes extoll/include/list.h which clash with pscom/list.h.
   If so, do not include pscom/list.h again:*/
#ifdef _LINUX_LIST_H /* extoll/include/list.h included? */
#define _LIST_H_ /* dont include pscom/list.h again */
#endif

typedef struct psex_con_info psex_con_info_t;
typedef struct hca_info hca_info_t;

// contact endpoint info
typedef struct psex_info_msg_s {
#ifndef DISABLE_RMA2
	RMA2_Nodeid	rma2_nodeid;
	RMA2_VPID	rma2_vpid;
	RMA2_NLA	rbuf_nla;
#endif

	velo2_nodeid_t	velo2_nodeid;
	velo2_vpid_t	velo2_vpid;
} psex_info_msg_t;


/* Initialise libextoll. This must be called first. */
int psex_init(void);


/*
 * Connection handling:
 *
 * Live cycle of a connection is:
 * ci = psex_con_create(void);
 * psex_con_init(ci, hca_info);
 * psex_con_get_info_msg(ci, my_info);
 * write(peer, my_info);
 * read(peer, info), peer called psex_con_get_info_msg() a transmit the result to us.
 * psex_con_connect(ci, info); // both sides have to call connect
 * (psex_sendv()/psex_recvlook and done)*
 * psextoll_send_eof(ci);
 * psex_con_cleanup(ci);
 * psex_con_free(ci);
 */

/* Create a con_info. usable for psex_con_init().
 * return NULL on error */
psex_con_info_t *psex_con_create(void);
void	psex_con_free(psex_con_info_t *con_info);


int	psex_con_init(psex_con_info_t *con_info, hca_info_t *hca_info, void *priv);
int	psex_con_connect(psex_con_info_t *con_info, psex_info_msg_t *info_msg);
void	psex_con_cleanup(psex_con_info_t *con_info);

void	psex_con_get_info_msg(psex_con_info_t *con_info /* in */, psex_info_msg_t *info /* out */);


/* returnvalue like read() , except on error errno is negative return
 * Start receiving.
 * return:
 * number of bytes received or
 * 0 for EOF (from psextoll_send_eof()) or
 * -EAGAIN nothing received or
 * -EPIPE broken connction.
 * (call psex_recvdone after usage of *buf!)
 */
int psex_recvlook(psex_con_info_t *con_info, void **buf);
void psex_recvdone(psex_con_info_t *con_info);


/* returnvalue like write(), except on error errno is negative return
 * send size bytes from iov through ci.
 * return number of bytes send or:
 * -EAGAIN if ci is busy or
 * -EPIPE in case of a broken connection.
 *
 * (sending with size = 0, will send a 0 message. But you will not
 * receive this message with psex_recvlook! To send EOF, use
 * psextoll_send_eof().) *
 */
int psex_sendv(psex_con_info_t *con_info, struct iovec *iov, int size);
void psex_send_eof(psex_con_info_t *con_info);

/* Suggest a value for psex_pending_tokens. Result depends on psex_recvq_size. */
unsigned psex_pending_tokens_suggestion(void);


/*
 * VELO2 IO
 */

/* returnvalue like write(), except on error errno is negative return
 * send size bytes from iov through ci.
 * return number of bytes send or:
 * -EAGAIN if ci is busy or
 * -EPIPE in case of a broken connection.
 *
 * (sending with size = 0, will send a 0 message. But you will not
 * receive this message with psex_recvlook! To send EOF, use
 * psextoll_send_eof().) *
 */
int psex_velo2_sendv(psex_con_info_t *con_info, struct iovec *iov, int size);
void psex_velo2_send_eof(psex_con_info_t *con_info);

/* recv up to msg_len bytes into msg. return bytes read. 0 on eof and
 * -errno on error.  *priv is set to priv from psex_con_init(..priv)
 * of the receiving connection and stays unchanged in case of no
 * receive (returning -EAGAIN).
 */
int psex_velo2_recv(hca_info_t *hca_info, void **priv, void *msg, size_t msglen);


/*
 * Configuration
 */
extern int psex_debug;
extern FILE *psex_debug_stream; /* Stream to use for debug output */
extern unsigned int psex_sendq_size; /* sendqueue size. Used when psex_global_sendq == 0 */
extern unsigned int psex_gsendq_size; /* Global sendqueue size. Used when psex_global_sendq == 1 */
extern unsigned int psex_recvq_size;
extern unsigned int psex_pending_tokens;
extern int psex_global_sendq; /* bool. Use one sendqueue for all connections? */
extern int psex_event_count; /* bool. Be busy if outstanding_cq_entries is to high? */

/*
 * Information
 */
extern unsigned psex_pending_global_sends; /* counter. Used only with psex_global_sendq == 1 */

#define PSEX_VELO2_MTU	64 /* it is 64, but with newer hardware this might change */

#endif /* _PSEXTOLL_H_ */
