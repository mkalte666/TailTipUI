CXXFLAGS=-O3 --std=c++14 -I./include -I./source/HoardXML/include -DDEBUG -g -c -Wall
CC=clang++
OUT=libTailTipUI.o
LIB=libTailTipUI.a
VPATH=source
BUILDDIR=build
DSTDIR=bin
SRC=$(wildcard source/*.cpp)
OBJECTS=$(patsubst source/%.cpp,build/%.o,$(SRC))

all: checkdirs $(OBJECTS)	
	ar rcs $(BUILDDIR)/$(LIB) $(OBJECTS)
	cp $(BUILDDIR)/$(LIB) ./bin/

clean: checkdirs
	-rm -rf $(BUILDDIR)/*
	-rm -rf $(DSTDIR)/*

$(BUILDDIR)/%.o: %.cpp
	$(CC) $(CXXFLAGS) $< -o $@

checkdirs: $(BUILDDIR) $(DSTDIR)

$(DSTDIR): 
	@mkdir -p $@

$(BUILDDIR): 
	@mkdir -p $@

