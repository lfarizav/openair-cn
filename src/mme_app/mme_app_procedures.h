/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */
#ifndef FILE_MME_APP_PROCEDURES_SEEN
#define FILE_MME_APP_PROCEDURES_SEEN

/*! \file mme_app_procedures.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "queue.h"

//typedef int (*mme_app_pdu_in_resp_t)(void *arg);
//typedef int (*mme_app_pdu_in_rej_t)(void *arg);
//typedef int (*mme_app_time_out_t)(void *arg);
//typedef int (*mme_app_sdu_out_not_delivered_t)(void *arg);


typedef enum {
  MME_APP_BASE_PROC_TYPE_NONE = 0,
  MME_APP_BASE_PROC_TYPE_S1AP,
  MME_APP_BASE_PROC_TYPE_S10,
  MME_APP_BASE_PROC_TYPE_S11
} mme_app_base_proc_type_t;


typedef struct mme_app_base_proc_s {
  // PDU interface
  //pdu_in_resp_t              resp_in;
  //pdu_in_rej_t               fail_in;
  time_out_t                 time_out;
  mme_app_base_proc_type_t   type;
} mme_app_base_proc_t;

/* S10 */
typedef enum {
  MME_APP_S10_PROC_TYPE_NONE = 0,
  MME_APP_S10_PROC_TYPE_INTER_MME_HANDOVER
} mme_app_s10_proc_type_t;

typedef struct mme_app_s10_proc_s {
  mme_app_base_proc_t         proc;
  mme_app_s10_proc_type_t     type;
  struct mme_app_timer_t      timer;
  uintptr_t                   s10_trxn;
  LIST_ENTRY(mme_app_s10_proc_s) entries;      /* List. */
} mme_app_s10_proc_t;

/*
 * S10 Procedure for Handover only on the target-MME side.
 * On the source MME we don't need a procedure, a timer is enough.
 */
typedef struct mme_app_s10_proc_inter_mme_handover_s {
  mme_app_s10_proc_t            proc;

  time_out_t                   *s10_inter_mme_handover_timeout;

  uintptr_t                     forward_relocation_trxn;
  /** Peer Information. */
  uint16_t                      peer_port;
  struct in_addr               *peer_ip; // todo: if they differ etc..
  target_identification_t       target_id;
  F_Container_t                *source_to_target_eutran_container;
  F_Cause_t                    *f_cause;
  /** NAS context information. */
  nas_s10_context_t             nas_s10_context;

  LIST_ENTRY(mme_app_handover_proc_s) entries;      /* List. */
} mme_app_s10_proc_inter_mme_handover_t;

/* S11 */
typedef enum {
  MME_APP_S11_PROC_TYPE_NONE = 0,
  MME_APP_S11_PROC_TYPE_CREATE_BEARER
} mme_app_s11_proc_type_t;

typedef struct mme_app_s11_proc_s {
  mme_app_base_proc_t         proc;
  mme_app_s11_proc_type_t     type;
  uintptr_t                   s11_trxn;
  LIST_ENTRY(mme_app_s11_proc_s) entries;      /* List. */
} mme_app_s11_proc_t;

typedef enum {
  S11_PROC_BEARER_UNKNOWN  = 0,
  S11_PROC_BEARER_PENDING  = 1,
  S11_PROC_BEARER_FAILED   = 2,
  S11_PROC_BEARER_SUCCESS  = 3
} s11_proc_bearer_status_t;

typedef struct mme_app_s11_proc_create_bearer_s {
  mme_app_s11_proc_t           proc;
  int                          num_bearers;
  int                          num_status_received;
  // TODO here give a NAS/S1AP/.. reason -> GTPv2-C reason
  s11_proc_bearer_status_t     bearer_status[BEARERS_PER_UE];
} mme_app_s11_proc_create_bearer_t;

typedef enum {
  MME_APP_S1AP_PROC_TYPE_NONE = 0,
  MME_APP_S1AP_PROC_TYPE_INITIAL
} mme_app_s1ap_proc_type_t;

void mme_app_delete_s11_procedures(ue_context_t * const ue_context_p);
mme_app_s11_proc_create_bearer_t* mme_app_create_s11_procedure_create_bearer(ue_context_t * const ue_context_p);
mme_app_s11_proc_create_bearer_t* mme_app_get_s11_procedure_create_bearer(ue_context_t * const ue_context_p);
void mme_app_delete_s11_procedure_create_bearer(ue_context_t * const ue_context_p);
void mme_app_s11_procedure_create_bearer_send_response(ue_context_t * const ue_context_p, mme_app_s11_proc_create_bearer_t* s11_proc_create);

/*
 * - Creating handover procedure in intra-MME and inter-MME handover
 * - Creating handover procedure in source & target MME todo: create same timer but different callback methods.
 * - Since we don't have a valid EMM UE context, we need to create an MME_APP context.
 */
void mme_app_delete_s10_procedures(ue_context_t * const ue_context_p);
//------------------------------------------------------------------------------
void mme_app_delete_s10_procedures(ue_context_t * const ue_context_p);
mme_app_s10_proc_inter_mme_handover_t* mme_app_create_s10_procedure_inter_mme_handover(ue_context_t * const ue_context_p);
mme_app_s10_proc_inter_mme_handover_t* mme_app_get_s10_procedure_inter_mme_handover(ue_context_t * const ue_context_p);
void mme_app_delete_s10_procedure_inter_mme_handover(ue_context_t * const ue_context_p);

#endif