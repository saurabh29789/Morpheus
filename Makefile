
CC = g++
ifeq ($(DEBUG), 1)
DEBUG_MODE   = -g -O0 -DMORPHEUS_DEBUG
endif
LIBNAME      = morpheus
LIBFULLNAME  = libmorpheus.so
TEST_BINARY  = isolated_tests
SHAREDOBJ    = temp.o

CCFLAGS_COMMON = -I if -std=c++11 -Wall -pedantic
CCFLAGS     += $(CCFLAGS_COMMON) $(DEBUG_MODE) -fPIC
CCFLAGS_BIN += $(CCFLAGS_COMMON) $(DEBUG_MODE)
CCLIBFLAGS  += -shared -Wl,-soname,$(LIBFULLNAME)

SRC_DIR = src
SOURCES = $(SRC_DIR)/morpheus.cpp \
          $(SRC_DIR)/morpheus_operators.cpp \
          $(SRC_DIR)/morpheus_func.cpp 
          #$(SRC_DIR)/morpheus_internals.cpp

OBJECTS = $(SRC_DIR)/morpheus.o \
          $(SRC_DIR)/morpheus_operators.o \
          $(SRC_DIR)/morpheus_func.o 
          #$(SRC_DIR)/morpheus_internals.o
DEPS    = if/morpheus.h

%.o: %.cpp $(DEPS)
	$(CC) $(CCFLAGS) -c -o $@ $<

morpheus: $(OBJECTS)
	$(CC) $(CCFLAGS) -c -o $(SHAREDOBJ) $(OBJECTS)
	$(CC) -fPIC $(CCLIBFLAGS) -o $(LIBFULLNAME) $(OBJECTS)

install:
	cp $(LIBFULLNAME) /usr/lib/

isolated_tests: tests/isolated_test.cpp
	$(CC) $^  -L . -l $(LIBNAME) -o $@ $(CCFLAGS_BIN) $>

all: morpheus install 

clean:
	rm $(SRC_DIR)/*.o $(LIBFULLNAME) $(TEST_BINARY)
