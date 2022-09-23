CC=g++
CXX=g++
RANLIB=ranlib

LIBSRC= Barrier.cpp MapReduceFramework.cpp
LIBOBJ=$(LIBSRC:.cpp=.o)

INCS=-I.
CFLAGS = -Wall -std=c++11 -fpermissive  -g $(INCS) 
CXXFLAGS = -Wall -std=c++11 -fpermissive  -g $(INCS) 

MAFLIB = libMapReduceFramework.a
TARGETS = $(MAFLIB)

TAR=tar
TARFLAGS=-cvf
TARNAME=ex3.tar
TARSRCS=$(LIBSRC) Makefile README Barrier.h

all: $(TARGETS)

$(TARGETS): $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) $(TARGETS) $(MAFLIB) $(OBJ) $(LIBOBJ) *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
