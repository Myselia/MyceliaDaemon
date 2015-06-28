# Variables
CC=g++
CPPCOMMON_NAME=MyseliaCppCommon
CPPCOMMON_PATH=../$(CPPCOMMON_NAME)
CPPCOMMON_LIBNAME=$(CPPCOMMON_NAME).a
CPPCOMMON_LIB=$(CPPCOMMON_PATH)/$(CPPCOMMON_LIBNAME)
CFLAGS=-I include -I $(CPPCOMMON_PATH)/include -std=c++11 -c
LDFLAGS=
LIBFLAGS=-lboost_system -lboost_thread -lboost_filesystem
EXECUTABLE=MyceliaDaemon
SRCDIR=src
OBJDIR=obj
SOURCES = $(shell find $(SRCDIR)/ -name "*.cpp")
OBJECTS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(SOURCES:.cpp=.o))

# PHONY (non-file targets)
.PHONY: clean daemon all cppcommon cleanDep

all: daemon

daemon: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(CPPCOMMON_LIB)
	$(CC) -o $(EXECUTABLE) $(LDFLAGS) $(OBJECTS) $(CPPCOMMON_LIB) $(LIBFLAGS)

clean:
	rm -f -R $(OBJDIR) $(EXECUTABLE)

cleanDep:
	cd $(CPPCOMMON_PATH) && make clean

$(CPPCOMMON_LIB):
	cd $(CPPCOMMON_PATH) && make library

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

