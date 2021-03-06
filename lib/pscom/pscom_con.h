/*
 * ParaStation
 *
 * Copyright (C) 2007 ParTec Cluster Competence Center GmbH, Munich
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined in the file LICENSE.QPL included in the packaging of this
 * file.
 *
 * Author:	Jens Hauke <hauke@par-tec.com>
 */

#ifndef _PSCOM_CON_H_
#define _PSCOM_CON_H_
#include "pscom_priv.h"

pscom_err_t pscom_con_connect_via_tcp(pscom_con_t *con, int nodeid, int portno);
pscom_err_t pscom_con_connect_loopback(pscom_con_t *con);
void pscom_con_accept(ufd_t *ufd, ufd_funcinfo_t *ufd_info);
// start send and receive queue
void pscom_con_setup(pscom_con_t *con);

// clear all recvq's of this connection. finish all recv requests
// of this connection with error. (keep recv any!)
void pscom_con_terminate_recvq(pscom_con_t *con);
void pscom_con_close(pscom_con_t *con);

// {read,write}_{start,stop} default hook.
void pscom_no_rw_start_stop(pscom_con_t *con);

int pscom_tcp_connect(int nodeid, int portno);
int pscom_is_local(pscom_socket_t *socket, int nodeid, int portno);

pscom_con_t *pscom_ondemand_find_con(pscom_sock_t *sock, const char name[8]);
pscom_con_t *pscom_ondemand_get_con(pscom_sock_t *sock, const char name[8]);
void pscom_ondemand_indirect_connect(pscom_con_t *con);

#endif /* _PSCOM_CON_H_ */
