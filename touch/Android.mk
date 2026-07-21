LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := touchyfix
LOCAL_SRC_FILES := touchyfix.cpp

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)/sbin

LOCAL_CPPFLAGS += -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti
LOCAL_CFLAGS   += -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti
LOCAL_LDFLAGS  += -Wl,--gc-sections
LOCAL_FORCE_STATIC_EXECUTABLE := true

include $(BUILD_EXECUTABLE)
