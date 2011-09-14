ver=0.3.1
platform=i386
plat=$(shell uname -m)
ifeq ($(plat),x86_64)
platform=amd64
endif
package=libofsystem

basedir=~/ofsystem

include $(HOME)/buildenv/packaging.mak

deps=debian/DEBIAN/control.master

$(target): $(deps)
	test -d debian/usr/lib || mkdir -p debian/usr/lib
	cp $(basedir)/lib/libofsystem.so debian/usr/lib
	strip debian/usr/lib/*.so
	cp debian/DEBIAN/control.master debian/DEBIAN/control
	sed -i -e "s/Architecture: .\+/Architecture: $(platform)/" debian/DEBIAN/control
	sed -i -e "s/Version: .\+/Version: $(ver)/" debian/DEBIAN/control
	sed -i -e "s/Package: .\+/Package: $(package)/" debian/DEBIAN/control
	find debian -name "*~" -exec rm {} \;
	dpkg-deb --build debian
	mv debian.deb $(target)
