# Makefile to build class 'gametrak' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.
PLATFORM=x86_64-w64-mingw32
PDDIR=/mnt/c/Program\ Files/Pd
PDINCLUDEDIR=/mnt/c/Program\ Files/Pd/src
PDBINDIR=/mnt/c/Program\ Files/Pd/bin
PDLIBDIR=/mnt/c/AddData/Roaming/Pd

# build this pd-external using c++
#cflags += -stdlib=libc++ 

# enable to build under WSL
cflags += -I ~/dev/xwin/sdk/include

# library name
lib.name = gametrak

# input source file (class name == source file basename)
transform.class.sources = transform.cpp Quaternion.cpp vecteur3d.cpp
calibrate.class.sources = calibrate.cpp

# all extra files to be included in binary distribution of the library
datafiles = transform-help.pd calibrate-help.pd README.md
#ldlibs = -lsetupapi

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder

# used so that `make list` shows a list of make targets
# useful for debugging
.PHONY: list
list:
	@LC_ALL=C $(MAKE) -pRrq -f $(firstword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/(^|\n)# Files(\n|$$)/,/(^|\n)# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | grep -E -v -e '^[^[:alnum:]]' -e '^$@$$'
# IMPORTANT: The line above must be indented by (at least one) 
#            *actual TAB character* - *spaces* do *not* work.