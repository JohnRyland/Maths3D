
LIB=.modules/Nuklear/examples/blah.a

$(LIB): .build/install/usr/local/lib/libglfw3.a
	export PKG_CONFIG_SYSROOT_DIR=$(abspath $(TEMP_DIR)/install)
	export PKG_CONFIG_LIBDIR=${PKG_CONFIG_SYSROOT_DIR}/usr/local/lib/pkgconfig
	cd .modules/Nuklear/example && make

build: $(LIB)

