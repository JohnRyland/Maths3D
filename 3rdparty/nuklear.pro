
LIB=.modules/Nuklear/examples/blah.a

$(LIB): .build/install/usr/local/lib/libglfw3.a
	cd .modules/Nuklear/example && PKG_CONFIG_SYSROOT_DIR=$(abspath $(TEMP_DIR)/install) \
		      PKG_CONFIG_LIBDIR=$(abspath $(TEMP_DIR)/install)/usr/local/lib/pkgconfig make

build: $(LIB)

