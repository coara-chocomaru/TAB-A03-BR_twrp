$(call inherit-product-if-exists, vendor/sts/a03br/a03br-vendor.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product_launched_with_l_mr1.mk)
PRODUCT_FIRST_API_LEVEL := 22
PRODUCT_CHARACTERISTICS := tablet
TARGET_IS_TABLET := true
TARGET_NO_TELEPHONY := true
TARGET_HAS_NO_RADIO := true
LOCAL_PATH := device/sts/a03br
PRODUCT_BUILD_RECOVERY_IMAGE := true
PRODUCT_BUILD_SYSTEM_IMAGE := true
PRODUCT_BUILD_BOOT_IMAGE := true
PRODUCT_BUILD_VENDOR_IMAGE := false

# Product characteristics
# PRODUCT_PACKAGE_OVERLAYS += \
    $(LOCAL_PATH)/overlay \
    $(LOCAL_PATH)/overlay-lineage

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    ro.adb.secure=0 \
    ro.secure=0 \
    ro.debuggable=1 \
    ro.boot.wificountrycode=JP \
    security.perf_harden=1 \
    ro.config.donot_nosim=true \
    persist.radio.no_wait_for_card=1 \
    ro.radio.noril=1 \
    persist.bandwidth.enable=0 \
    ro.zygote=zygote64_32

ADDITIONAL_DEFAULT_PROPERTIES += \
    ro.adb.secure=0 \
    ro.secure=0 \
    ro.debuggable=1 \
    ro.boot.wificountrycode=JP \
    persist.bandwidth.enable=0 \
    security.perf_harden=1 \
    ro.oem_unlock_supported=0 \
    ro.com.android.mobiledata=false \
    ro.allow.mock.location=0 \
    ro.zygote=zygote64_32

PRODUCT_PROPERTY_OVERRIDES += \
    ro.build.selinux=0 \
    ro.telephony.disable=true \
    ro.kernel.android.checkjni=0 \
    ro.carrier=wifi-only \
    persist.sys.country=JP \
    ro.radio.noril=1 \
    config.disable_telephony=true \
    keyguard.no_require_sim=true \
    dalvik.vm.dex2oat-filter=speed-profile \
    dalvik.vm.image-dex2oat-filter=speed-profile \
    dalvik.vm.dex2oat-threads=4 \
    dalvik.vm.dex2oat-flags=--no-watch-dog

ADDITIONAL_BUILD_PROPERTIES += \
    ro.build.selinux=0 \
    persist.radio.noril=1 \
    ro.carrier=wifi-only \
    persist.bandwidth.enable=0 \
    dalvik.vm.heapstartsize=4m \
    dalvik.vm.heapgrowthlimit=192m \
    dalvik.vm.heapsize=384m \
    dalvik.vm.heaptargetutilization=0.75 \
    dalvik.vm.heapminfree=2m \
    dalvik.vm.heapmaxfree=4m \
    dalvik.vm.checkjni=false \
    ro.hwui.texture_cache_size=40 \
    ro.hwui.layer_cache_size=24 \
    ro.hwui.path_cache_size=12 \
    ro.hwui.gradient_cache_size=1 \
    ro.hwui.drop_shadow_cache_size=4 \
    ro.hwui.r_buffer_cache_size=4 \
    ro.hwui.texture_cache_flushrate=0.40 \
    ro.hwui.text_small_cache_width=1024 \
    ro.hwui.text_small_cache_height=512 \
    ro.hwui.text_large_cache_width=1280 \
    ro.hwui.text_large_cache_height=512 \
    ro.telephony.disable=true \
    persist.sys.timezone=Asia/Tokyo

PRODUCT_PACKAGES += \
    LatinIME

PRODUCT_PACKAGES += \
    libstlport

PRODUCT_PACKAGES += \
    libion

PRODUCT_PACKAGES += \
    com.android.future.usb.accessory

PRODUCT_PACKAGES += \
    e2fsck \
    fsck.f2fs \
    mkfs.f2fs \
    make_ext4fs

PRODUCT_PACKAGES += \
    fstab.mt8167 \
    init.mt8167.rc \
    init.mt8167.usb.rc \
    init.usb.rc \
    init.usb.configfs.rc \
    init.recovery.mt8167.rc \
    init.connectivity.rc \
    ueventd.mt8167.rc \
    ueventd.rc \
    init.modem.rc \
    init.zygote32.rc \
    init.zygote64_32.rc \
    init.cm.rc \
    init.rc

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/fstab.pxa1928:root/fstab.pxa1928 \
    $(LOCAL_PATH)/rootdir/init.pxa1928.power.rc:root/init.pxa1928.power.rc \
    $(LOCAL_PATH)/rootdir/init.pxa1928.rc:root/init.pxa1928.rc \
    $(LOCAL_PATH)/rootdir/init.pxa1928.sensor.rc:root/init.pxa1928.sensor.rc \
    $(LOCAL_PATH)/rootdir/init.pxa1928.usb.rc:root/init.pxa1928.usb.rc \
    $(LOCAL_PATH)/rootdir/init.usb.rc:root/init.usb.rc \
    $(LOCAL_PATH)/rootdir/init.zygote32.rc:root/init.zygote32.rc \
    $(LOCAL_PATH)/rootdir/init.zygote64_32.rc:root/init.zygote64_32.rc \
    $(LOCAL_PATH)/rootdir/lib/modules/gator.ko:root/lib/modules/gator.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/gspca_main.ko:root/lib/modules/gspca_main.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/mbt8777.ko:root/lib/modules/mbt8777.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/mbt8787.ko:root/lib/modules/mbt8787.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/mbt8887.ko:root/lib/modules/mbt8887.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/mlan8777.ko:root/lib/modules/mlan8777.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/mlan8787.ko:root/lib/modules/mlan8787.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/mlan8801.ko:root/lib/modules/mlan8801.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/mlan8887.ko:root/lib/modules/mlan8887.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/sd8777.ko:root/lib/modules/sd8777.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/sd8787.ko:root/lib/modules/sd8787.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/sd8801.ko:root/lib/modules/sd8801.ko \
    $(LOCAL_PATH)/rootdir/lib/modules/sd8887.ko:root/lib/modules/sd8887.ko \
    $(LOCAL_PATH)/rootdir/res/wifi/ed_mac_ctrl.conf:root/res/wifi/ed_mac_ctrl.conf \
    $(LOCAL_PATH)/rootdir/res/wifi/ed_mac_ctrl_8801.conf:root/res/wifi/ed_mac_ctrl_8801.conf \
    $(LOCAL_PATH)/rootdir/res/wifi/mlaninit.sh:root/res/wifi/mlaninit.sh \
    $(LOCAL_PATH)/rootdir/res/wifi/mlaninit_8801.sh:root/res/wifi/mlaninit_8801.sh \
    $(LOCAL_PATH)/rootdir/ueventd.pxa1928.rc:root/ueventd.pxa1928.rc \
    $(LOCAL_PATH)/rootdir/ueventd.rc:root/ueventd.rc

include device/sts/a03br/media.mk
include device/sts/a03br/permissions.mk
