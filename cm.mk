# Inherit device configuration
$(call inherit-product, device/sts/a03br/lineage_a03br.mk)
$(call inherit-product, vendor/cm/config/common_full_tablet_wifionly.mk)
TARGET_LOCALES := ja_JP en_US en_AU
