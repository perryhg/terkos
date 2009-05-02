Here's what I did to build the TinyXML shared library:

1) Fetched code from SVN:

      svn checkout http://ticpp.googlecode.com/svn/trunk/ ticpp

2) Downloaded Premake 3.x for Linux:

      curl http://downloads.sourceforge.net/premake/premake-linux-3.7.tar.gz > premake-linux-3.7.tar.gz

   I saved the tarball in the /tinyxml/ticpp directory created above by the SVN checkout

3) Expanded the Premake tarball:

      tar zxf premake-linux-3.7.tar.gz

4) Exited the ticpp.lua file to set the output directory for builds.  I changed these two lines:

      package.bindir                                                          = "../dist"
      package.libdir                                                          = "../dist"

5) Ran premake:

      ./premake --target gnu --unicode --dynamic-runtime --ticpp-shared

6) Added the following line to the beginning of tinyxml.h to enable support for STL:

      #define TIXML_USE_STL
      
7) Ran make:

      make CONFIG=Release CXX=arm-linux-g++

Note that you can fully clean everything by doing:

      make clean
      ./premake --clean