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

# Set the target binary file name and source files
KOALA_WIFI_BLE_BIN = koala-wifi-ble
KOALA_WIFI_BLE_SOURCE = main.cpp  # Ensure correct reference

KOALA_WIFI_BLE_CFLAGS = -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d
KOALA_WIFI_BLE_LDFLAGS = -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

# Define the build steps for the package, incorporating the custom CFLAGS and LDFLAGS
define KOALA_WIFI_BLE_BUILD_CMDS
	$(TARGET_CXX) $(TARGET_CXXFLAGS) $(KOALA_WIFI_BLE_CFLAGS) -o $(TARGET_DIR)/usr/bin/$(KOALA_WIFI_BLE_BIN) \
	$(KOALA_WIFI_BLE_SITE)/$(KOALA_WIFI_BLE_SOURCE) $(KOALA_WIFI_BLE_LDFLAGS)
endef

# Register the package in Buildroot
$(eval $(generic-package))
