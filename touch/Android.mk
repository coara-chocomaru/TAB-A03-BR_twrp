LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := touchyfix
LOCAL_SRC_FILES := touchyfix.c

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)/sbin
LOCAL_FORCE_STATIC_EXECUTABLE := true

LOCAL_CFLAGS += \
    -Os \
    -g0 \
    -ffunction-sections \
    -fdata-sections \
    -fno-stack-protector \
    -fomit-frame-pointer

LOCAL_LDFLAGS += \
    -Wl,--gc-sections \
    -Wl,-s
LOCAL_STRIP_MODULE := true
LOCAL_STATIC_LIBRARIES += libc

include $(BUILD_EXECUTABLE)
