#
# Copyright (C) 2024 The Android Open Source Project
# Copyright (C) 2024 SebaUbuntu's TWRP device tree generator
#
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_DEVICE),a03br)
TARGET_STRIP=true
TARGET_CFLAGS+="-Os -ffunction-sections -fdata-sections -fno-unwind-tables"
TARGET_LDFLAGS+="-Wl,--gc-sections -Wl,--strip-all"
JAVA_SDK_ENFORCEMENT_ERROR := false
include $(call all-subdir-makefiles,$(LOCAL_PATH))
endif
