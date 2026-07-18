#
# Copyright (C) 2024 The Android Open Source Project
# Copyright (C) 2024 SebaUbuntu's TWRP device tree generator
#
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_DEVICE),a03br)
JAVA_SDK_ENFORCEMENT_ERROR := false
LOCAL_STRIP_MODULE := true
include $(call all-subdir-makefiles,$(LOCAL_PATH))
endif
