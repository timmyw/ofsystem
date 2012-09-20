ver=0.3.3
platform=i386
plat=$(shell uname -m)
ifeq ($(plat),x86_64)
platform=amd64
endif
package=libofsystem

#basedir=~/ofsystem
basedir=../

include $(HOME)/buildenv/packaging.mak

deps=debian/DEBIAN/control.master

# Setup distro specific dependencies

deb_deps=libc6

#ifeq ($(distro), debian)
#	deb_deps=unixodbc, imagemagick, zlib1g, libc6, libmagick++3, libxml2, libxslt1.1
#else	
#	deb_deps=unixodbc, imagemagick, zlib1g, libc6, libmagick++2, libxml2, libxslt1.1
#endif

$(target): $(deps)
	test -d debian/usr/lib || mkdir -p debian/usr/lib
	cp $(basedir)/lib/libofsystem.so debian/usr/lib
	strip debian/usr/lib/*.so
	cp debian/DEBIAN/control.master debian/DEBIAN/control
	sed -i -e "s/Architecture: .\+/Architecture: $(platform)/" debian/DEBIAN/control
	sed -i -e "s/Version: .\+/Version: $(ver)/" debian/DEBIAN/control
	sed -i -e "s/Package: .\+/Package: $(package)/" debian/DEBIAN/control
	sed -i -e "s/Depends: .\+/Depends: $(deb_deps)/" debian/DEBIAN/control
	find debian -name "*~" -exec rm {} \;
	dpkg-deb --build debian
	mv debian.deb $(target)
