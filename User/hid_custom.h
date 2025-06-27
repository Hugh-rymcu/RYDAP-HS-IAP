/*
 * Copyright (c) 2025, hugh-rymcu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef HID_CUSTOM_H
#define HID_CUSTOM_H

#include "usbd_core.h"
#include "usbd_hid.h"
#define CONFIG_USB_HS
void hid_custom_init(uint8_t busid, uintptr_t reg_base);
void hid_ry_hid_handle(void);

#endif /* HID_CUSTOM_H*/
