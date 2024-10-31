################################################################################
#
# koala-keyboard package description
#
################################################################################

KOALA_KEYBOARD_VERSION = 1.0
KOALA_KEYBOARD_SITE = package/koala-keyboard/src
KOALA_KEYBOARD_SITE_METHOD = local
KOALA_KEYBOARD_LICENSE = GPL-2.0
KOALA_KEYBOARD_LICENSE_FILES = LICENSE

# Set the target binary file name and source files
KOALA_KEYBOARD_BIN = koala-keyboard
KOALA_KEYBOARD_SOURCE = main.cpp

# Define additional CFLAGS and LDFLAGS
KOALA_KEYBOARD_CFLAGS = -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d
KOALA_KEYBOARD_LDFLAGS = -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

# Define the build steps for the package, incorporating the custom CFLAGS and LDFLAGS
define KOALA_KEYBOARD_BUILD_CMDS
	$(TARGET_CXX) $(TARGET_CXXFLAGS) $(KOALA_KEYBOARD_CFLAGS) -o $(TARGET_DIR)/usr/bin/$(KOALA_KEYBOARD_BIN) \
	$(KOALA_KEYBOARD_SITE)/$(KOALA_KEYBOARD_SOURCE) $(KOALA_KEYBOARD_LDFLAGS)
endef

# Register the package in Buildroot
$(eval $(generic-package))
