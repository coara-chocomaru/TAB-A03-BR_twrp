#
# Copyright (C) 2024 The Android Open Source Project
# Copyright (C) 2024 SebaUbuntu's TWRP device tree generator
#
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_DEVICE),a03br)
TARGET_SCREEN_WIDTH := 1280
TARGET_SCREEN_HEIGHT := 800
JAVA_SDK_ENFORCEMENT_ERROR := false
include $(call all-subdir-makefiles,$(LOCAL_PATH))
endif
