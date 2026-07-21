LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := touchyfix
LOCAL_SRC_FILES := touchyfix.c

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)/sbin

LOCAL_FORCE_STATIC_EXECUTABLE := true

LOCAL_CFLAGS += -Os -ffunction-sections -fdata-sections -fno-stack-protector
LOCAL_LDFLAGS += -Wl,--gc-sections

LOCAL_STATIC_LIBRARIES += libc

include $(BUILD_EXECUTABLE)
