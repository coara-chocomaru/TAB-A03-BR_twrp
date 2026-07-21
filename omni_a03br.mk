#
# Copyright (C) 2022 The Android Open Source Project
# Copyright (C) 2022 SebaUbuntu's TWRP device tree generator
#
# SPDX-License-Identifier: Apache-2.0
$(call inherit-product, $(SRC_TARGET_DIR)/product/base.mk)
$(call inherit-product, vendor/omni/config/common.mk)
$(call inherit-product, build/target/product/embedded.mk)
$(call inherit-product, device/sts/a03br/device.mk)

PRODUCT_DEVICE := a03br
PRODUCT_NAME := omni_a03br
PRODUCT_RELEASE_NAME := omni_a03br
PRODUCT_BRAND := benesse
PRODUCT_MODEL := TAB-A03-BR
PRODUCT_MANUFACTURER := STS-TOTTORI
PRODUCT_GMS_CLIENTID_BASE := android-benesse
PRODUCT_LOCALES := ja_JP en_US en_AU
PRODUCT_DEFAULT_LANGUAGE := ja
PRODUCT_DEFAULT_REGION := JP
PRODUCT_AAPT_CONFIG := large mdpi hdpi
PRODUCT_AAPT_PREF_CONFIG := mdpi
TARGET_SCREEN_WIDTH := 1280
TARGET_SCREEN_HEIGHT := 800
PRODUCT_RESTRICT_VENDOR_FILES := false
TEMPORARY_DISABLE_PATH_RESTRICTIONS := true
PRODUCT_PACKAGES := $(filter-out healthd,$(PRODUCT_PACKAGES))


PRODUCT_BUILD_PROP_OVERRIDES += \
    PRIVATE_BUILD_DESC="TAB-A03-BR-user 5.1 02.04.000 02.04.000.20191125.213021 release-keys"

BUILD_FINGERPRINT := benesse/TAB-A03-BR/TAB-A03-BR:5.1/02.04.000/02.04.000.20191125.213021:user/release-keys
