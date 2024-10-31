################################################################################
#
# vsomeip package
#
################################################################################

KOALA_VSOMEIP_VERSION = b20a39e236fd5e1952f19daa00ab3eb89c326768
KOALA_VSOMEIP_SITE = $(call github,dnguyen24498,vsomeip,$(KOALA_VSOMEIP_VERSION))
KOALA_VSOMEIP_LICENSE = MIT
KOALA_VSOMEIP_LICENSE_FILES = LICENSE_boost
KOALA_VSOMEIP_DEPENDENCIES = boost

KOALA_VSOMEIP_PATCHES = 
# Define the build commands
KOALA_VSOMEIP_INSTALL_STAGING = YES
KOALA_VSOMEIP_INSTALL_TARGET = YES
KOALA_VSOMEIP_CONF_OPTS = 	-DENABLE_SIGNAL_HANDLING=ON \
							-DENABLE_MULTIPLE_ROUTING_MANAGERS=OFF \
							-DCMAKE_BUILD_TYPE=Debug

# Custom post-installation commands
define KOALA_VSOMEIP_INSTALL_EXTRA_CONFIG
	cp $(KOALA_VSOMEIP_DIR)/examples/routingmanagerd/routingmanagerd \
		$(TARGET_DIR)/usr/bin/routingmanagerd
endef

KOALA_VSOMEIP_POST_INSTALL_TARGET_HOOKS += KOALA_VSOMEIP_INSTALL_EXTRA_CONFIG

$(eval $(cmake-package))
