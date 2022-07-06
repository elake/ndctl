/* SPDX-License-Identifier: LGPL-2.1 */
/* Copyright (C) 2020-2021, Intel Corporation. All rights reserved. */
#ifndef _LIBCXL_H_
#define _LIBCXL_H_

#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <ccan/short_types/short_types.h>

#ifdef HAVE_UUID
#include <uuid/uuid.h>
#else
typedef unsigned char uuid_t[16];
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct cxl_ctx;
struct cxl_ctx *cxl_ref(struct cxl_ctx *ctx);
void cxl_unref(struct cxl_ctx *ctx);
int cxl_new(struct cxl_ctx **ctx);
void cxl_set_log_fn(struct cxl_ctx *ctx,
		void (*log_fn)(struct cxl_ctx *ctx, int priority,
			const char *file, int line, const char *fn,
			const char *format, va_list args));
int cxl_get_log_priority(struct cxl_ctx *ctx);
void cxl_set_log_priority(struct cxl_ctx *ctx, int priority);
void cxl_set_userdata(struct cxl_ctx *ctx, void *userdata);
void *cxl_get_userdata(struct cxl_ctx *ctx);
void cxl_set_private_data(struct cxl_ctx *ctx, void *data);
void *cxl_get_private_data(struct cxl_ctx *ctx);

struct cxl_memdev;
struct cxl_memdev *cxl_memdev_get_first(struct cxl_ctx *ctx);
struct cxl_memdev *cxl_memdev_get_next(struct cxl_memdev *memdev);
int cxl_memdev_get_id(struct cxl_memdev *memdev);
const char *cxl_memdev_get_devname(struct cxl_memdev *memdev);
int cxl_memdev_get_major(struct cxl_memdev *memdev);
int cxl_memdev_get_minor(struct cxl_memdev *memdev);
struct cxl_ctx *cxl_memdev_get_ctx(struct cxl_memdev *memdev);
unsigned long long cxl_memdev_get_pmem_size(struct cxl_memdev *memdev);
unsigned long long cxl_memdev_get_ram_size(struct cxl_memdev *memdev);
const char *cxl_memdev_get_firmware_verison(struct cxl_memdev *memdev);
size_t cxl_memdev_get_lsa_size(struct cxl_memdev *memdev);
int cxl_memdev_is_active(struct cxl_memdev *memdev);
int cxl_memdev_zero_lsa(struct cxl_memdev *memdev);
int cxl_memdev_get_lsa(struct cxl_memdev *memdev, void *buf, size_t length,
		size_t offset);
int cxl_memdev_set_lsa(struct cxl_memdev *memdev, void *buf, size_t length,
		size_t offset);
int cxl_memdev_cmd_identify(struct cxl_memdev *memdev);
int cxl_memdev_get_supported_logs(struct cxl_memdev *memdev);
int cxl_memdev_get_cel_log(struct cxl_memdev *memdev);
int cxl_memdev_get_event_interrupt_policy(struct cxl_memdev *memdev);
int cxl_memdev_set_event_interrupt_policy(struct cxl_memdev *memdev, u32 int_policy);
int cxl_memdev_get_timestamp(struct cxl_memdev *memdev);
int cxl_memdev_set_timestamp(struct cxl_memdev *memdev, u64 timestamp);
int cxl_memdev_get_alert_config(struct cxl_memdev *memdev);
int cxl_memdev_set_alert_config(struct cxl_memdev *memdev, u32 alert_prog_threshold,
    u32 device_temp_threshold, u32 mem_error_threshold);
int cxl_memdev_get_health_info(struct cxl_memdev *memdev);
int cxl_memdev_get_event_records(struct cxl_memdev *memdev, u8 event_log_type);
int cxl_memdev_get_ld_info(struct cxl_memdev *memdev);
int cxl_memdev_ddr_info(struct cxl_memdev *memdev, u8 ddr_id);
int cxl_memdev_clear_event_records(struct cxl_memdev *memdev, u8 event_log_type,
    u8 clear_event_flags, u8 no_event_record_handles, u16 *event_record_handles);
/* insert here */

#define cxl_memdev_foreach(ctx, memdev) \
        for (memdev = cxl_memdev_get_first(ctx); \
             memdev != NULL; \
             memdev = cxl_memdev_get_next(memdev))

struct cxl_cmd;
const char *cxl_cmd_get_devname(struct cxl_cmd *cmd);
struct cxl_cmd *cxl_cmd_new_raw(struct cxl_memdev *memdev, int opcode);
int cxl_cmd_set_input_payload(struct cxl_cmd *cmd, void *in, int size);
int cxl_cmd_set_output_payload(struct cxl_cmd *cmd, void *out, int size);
void cxl_cmd_ref(struct cxl_cmd *cmd);
void cxl_cmd_unref(struct cxl_cmd *cmd);
int cxl_cmd_submit(struct cxl_cmd *cmd);
int cxl_cmd_get_mbox_status(struct cxl_cmd *cmd);
int cxl_cmd_get_out_size(struct cxl_cmd *cmd);
struct cxl_cmd *cxl_cmd_new_identify(struct cxl_memdev *memdev);
int cxl_cmd_identify_get_fw_rev(struct cxl_cmd *cmd, char *fw_rev, int fw_len);
unsigned long long cxl_cmd_identify_get_partition_align(struct cxl_cmd *cmd);
unsigned int cxl_cmd_identify_get_lsa_size(struct cxl_cmd *cmd);
struct cxl_cmd *cxl_cmd_new_get_health_info(struct cxl_memdev *memdev);
int cxl_cmd_get_health_info_get_health_status(struct cxl_cmd *cmd);
int cxl_cmd_get_health_info_get_media_status(struct cxl_cmd *cmd);
int cxl_cmd_get_health_info_get_ext_status(struct cxl_cmd *cmd);
int cxl_cmd_get_health_info_get_life_used(struct cxl_cmd *cmd);
int cxl_cmd_get_health_info_get_temperature(struct cxl_cmd *cmd);
int cxl_cmd_get_health_info_get_dirty_shutdowns(struct cxl_cmd *cmd);
int cxl_cmd_get_health_info_get_volatile_errors(struct cxl_cmd *cmd);
int cxl_cmd_get_health_info_get_pmem_errors(struct cxl_cmd *cmd);
struct cxl_cmd *cxl_cmd_new_get_lsa(struct cxl_memdev *memdev,
		unsigned int offset, unsigned int length);
void *cxl_cmd_get_lsa_get_payload(struct cxl_cmd *cmd);
struct cxl_cmd *cxl_cmd_new_set_lsa(struct cxl_memdev *memdev,
		void *buf, unsigned int offset, unsigned int length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
