################################################################################
#
# koala-wifi-ble package description
#
################################################################################

KOALA_WIFI_BLE_VERSION = 1.0
KOALA_WIFI_BLE_SITE = package/koala-wifi-ble/src
KOALA_WIFI_BLE_SITE_METHOD = local
KOALA_WIFI_BLE_LICENSE = GPL-2.0
KOALA_WIFI_BLE_LICENSE_FILES = LICENSE

KOALA_WIFI_BLE_INSTALL_TARGET = YES

KOALA_WIFI_BLE_DEPENDENCIES = koala-dlt koala-capicxx-someip-runtime

# Set CMake configuration options
KOALA_WIFI_BLE_CONF_OPTS += -DCommonAPI_DIR=$(STAGING_DIR)/usr \
                            -DCMAKE_PREFIX_PATH=$(STAGING_DIR)/usr

$(eval $(cmake-package))
