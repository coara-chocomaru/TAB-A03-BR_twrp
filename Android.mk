#
# Copyright (C) 2024 The Android Open Source Project
# Copyright (C) 2024 SebaUbuntu's TWRP device tree generator
#
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_DEVICE),a03br)
ALL_PREBUILT += $(INSTALLED_KERNEL_TARGET)
KERNEL_OUT := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ

$(KERNEL_OUT):
	mkdir -p $(KERNEL_OUT)

JAVA_SDK_ENFORCEMENT_ERROR := false
include $(call all-subdir-makefiles,$(LOCAL_PATH))
endif
