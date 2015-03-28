CXXFLAGS=--std=c++14 -I./include -I./source/HoardXML/include -DDEBUG -g -c -Wall
RELEASEFLAGS=-O3 --std=c++14 -I./include -I./source/HoardXML/include -DRELEASE -c -Wall
CC=clang++
OUT=libTailTipUI.o
LIB=libTailTipUI.a
VPATH=source
BUILDDIR=build
DSTDIR=bin
SRC=$(wildcard source/*.cpp)
OBJECTS=$(patsubst source/%.cpp,build/%.o,$(SRC))
ROBJECTS=$(patsubst source/%.cpp,build/%.R.o,$(SRC))

all: debug

release: checkdirs $(ROBJECTS)
	ar rcs $(BUILDDIR)/$(LIB) $(ROBJECTS)
	cp $(BUILDDIR)/$(LIB) ./bin/

debug: checkdirs $(OBJECTS)	
	ar rcs $(BUILDDIR)/$(LIB) $(OBJECTS)
	cp $(BUILDDIR)/$(LIB) ./bin/

clean: checkdirs
	-rm -rf $(BUILDDIR)/*
	-rm -rf $(DSTDIR)/*

$(BUILDDIR)/%.o: %.cpp
	$(CC) $(CXXFLAGS) $< -o $@

$(BUILDDIR)/%.R.o: %.cpp
	$(CC) $(RELEASEFLAGS) $< -o $@

checkdirs: $(BUILDDIR) $(DSTDIR)

$(DSTDIR): 
	@mkdir -p $@

$(BUILDDIR): 
	@mkdir -p $@

