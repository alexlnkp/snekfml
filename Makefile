#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# Note: for smallest filesize use after build: `strip -x -X -s -R .gnu.version -R .gnu.version_r. output/main && upx --best --compress-icons=3 --ultra-brute output/main`

# Debug mode is disabled by default. To build in debug, use `make DEBUG=1`
DEBUG=

# Graphical debug. Made for debugging graphics.
GRDBG=

# CXX compiler to use
CXX = g++

LIBRARIES := sfml-graphics

CFLAGS_DEBUG=-Og -g

# NOTE: difference between -O3 and -Oz in size on average is 18.8948%
# `-fno-exceptions` DOES reduce the COMPILED file size,
# but UPX makes it not as small as the version with no `-fno-exceptions`, for some reason
CFLAGS_RELEASE=-O3 -flto -fno-gcse
# Another note: Apparently github actions don't support g++ with -Oz for some reason...
# Would recommend using it when building locally though.

CXXFLAGS_COMMON := `pkg-config --cflags $(LIBRARIES)` -std=c++17 -Wall -Wextra
CXXFLAGS_DEBUG := $(CXXFLAGS_COMMON) $(CFLAGS_DEBUG)
CXXFLAGS_RELEASE := $(CXXFLAGS_COMMON) $(CFLAGS_RELEASE)

LFLAGS    = `pkg-config --libs $(LIBRARIES)`

OUTPUT	:= output
SRC		:= src
INCLUDE	:= include
LIB		:= lib

ifeq ($(OS),Windows_NT)
MAIN	    := main.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
FIXPATH      = $(subst /,\,$1)
RM			:= del /q /f
MD	        := mkdir
else
MAIN	    := main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH      = $1
RM           = rm -f
MD	        := mkdir -p
endif

INCLUDES   := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
LIBS	   := $(patsubst %,-L%, $(LIBDIRS:%/=%))
SOURCES	   := $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))
OBJECTS	   := $(SOURCES:.cpp=.o)
DEPS	   := $(OBJECTS:.o=.d)

OUTPUTMAIN := $(call FIXPATH,$(OUTPUT)/$(MAIN))

# Debug mode is for compiling with debug symbols.
ifeq ($(DEBUG), 1)
CXXFLAGS:=$(CXXFLAGS) $(CXXFLAGS_DEBUG) -gsplit-dwarf
MODESTR := Debug
else
CXXFLAGS:=$(CXXFLAGS) $(CXXFLAGS_RELEASE)
MODESTR := Release
endif

# Graphical Debug mode is for compiling with Graphic-Debug.
# NOTE: You can build with BOTH.
ifeq ($(GRDBG), 1)
CXXFLAGS := $(CXXFLAGS) -DGRAPHIC_DEBUG
MODESTR  := GraphicDebug
endif

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!
	$(info Building in $(MODESTR) mode)

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LIBS) $(LFLAGS)

-include $(DEPS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -MMD $<  -o $@

.PHONY: clean run

clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	@echo Cleanup complete!

run: all
	@./$(OUTPUTMAIN)
