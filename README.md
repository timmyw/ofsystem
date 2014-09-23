# ofsystem

ofsystem is the openFabric system library.  It comprises of platform
independant functionality, and utility functionality.

ofsystem is licensed under a modified version of the MIT license.
Please see the LICENSE file for more information.

# Building ofsystem

ofsystem uses CMake to build, and requires version 2.6.

To build ofsystem:

        cmake -G "Unix Makefiles
        make

cmake will accept a PREFIX variable as a command line argument, which
will set the installation prefix (defaults to /usr/local)

    cmake -G "Unix Makefiles" -DPREFIX=/opt/

This will set a cmake cache entry, which ultimately means if you need
to change it, you'll need to delete the CMakeCache.txt file, and
re-run the above cmake command.

# Documentation

Cmake will create a 'doc' target if Doxygen is found, which currently
generates HTML and LaTeX documentation, based on the comments in the
source.

# Supported platforms

ofsystem currently builds and runs correctly on Linux systems and
Windows (x86) systems.  On Windows it requires Visual C++ 6.0 to
build.

Both systems require an install of Perl to do a complete build.

# Documentation

Documentation can be generated using Doxygen using the source files as
input.
