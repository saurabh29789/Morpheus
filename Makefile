
CC = g++
ifeq ($(DEBUG), 1)
DEBUG_MODE = -g -O0
endif
LIBNAME = libmorpheus.so
BINARY  = morpheus_test
SHAREDOBJ = temp.o

CCFLAGS_COMMON = -I if -std=c++11 -Wall -pedantic
CCFLAGS     += $(CCFLAGS_COMMON) $(DEBUG_MODE) -fPIC
CCFLAGS_BIN += $(CCFLAGS_COMMON) $(DEBUG_MODE)
CCLIBFLAGS  += -shared -Wl,-soname,$(LIBNAME)

SRC_DIR = src
SOURCES = $(SRC_DIR)/morpheus.cpp \
			 $(SRC_DIR)/morpheus_operators.cpp \
			 $(SRC_DIR)/morpheus_func.cpp

OBJECTS = $(SRC_DIR)/morpheus.o \
			 $(SRC_DIR)/morpheus_operators.o \
			 $(SRC_DIR)/morpheus_func.o
DEPS    = if/morpheus.h

%.o: %.cpp $(DEPS)
	$(CC) $(CCFLAGS) -c -o $@ $<

library: $(OBJECTS)
	$(CC) $(CCFLAGS) -c -o $(SHAREDOBJ) $(OBJECTS)
	$(CC) -fPIC $(CCLIBFLAGS) -o $(LIBNAME) $(OBJECTS)

install:
	cp $(LIBNAME) /usr/lib/

morpheus_test: src/main.cpp
	$(CC) $^  -L . -l morpheus -o $(BINARY) $(CCFLAGS_BIN) $>

all: library install 

clean:
	rm $(SRC_DIR)/*.o $(LIBNAME) $(BINARY)
