################################################################################
# 	
# vsomeip package
#
################################################################################

KOALA_VSOMEIP_VERSION = 65a9300840b0f5e91f44f0f28b54d6304b623231
KOALA_VSOMEIP_SITE = $(call github,dnguyen24498,vsomeip,$(KOALA_VSOMEIP_VERSION))
KOALA_VSOMEIP_LICENSE = MIT
KOALA_VSOMEIP_LICENSE_FILES = LICENSE_boost
KOALA_VSOMEIP_DEPENDENCIES = boost

# Define the build commands
KOALA_VSOMEIP_INSTALL_STAGING = YES
KOALA_VSOMEIP_INSTALL_TARGET = YES
KOALA_VSOMEIP_CONF_OPTS += 	-DENABLE_SIGNAL_HANDLING=ON \
							-DENABLE_MULTIPLE_ROUTING_MANAGERS=OFF \
							-DCMAKE_BUILD_TYPE=Debug \
							-DBoost_DIR=$(STAGING_DIR)/usr/include/boost

$(eval $(cmake-package))
