HOST_GCC = g++
TARGET_GCC = g++

PLUGIN_SRC = plugin.cpp
GCCPLUGINS_DIR := $(shell $(TARGET_GCC) -print-file-name=plugin)

TESTFILE_DIR := $(shell pwd)/test
TESTFILE_SRC := test.cpp

BINARY_DIR = $(shell pwd)/build
TESTFLAGS += -fplugin=$(BINARY_DIR)/plugin.ext -Wall -Wextra

CXXFLAGS += -I$(GCCPLUGINS_DIR)/include -fPIC -fno-rtti -O2

plugin.ext: $(PLUGIN_SRC)
	$(HOST_GCC) -shared $(CXXFLAGS) $^ -o $(BINARY_DIR)/$@

test: plugin.ext
	$(TARGET_GCC) $(TESTFLAGS) $(TESTFILE_DIR)/$(TESTFILE_SRC) -o $(BINARY_DIR)/$@

.PHONY: createDir

createDir:
	mkdir -p $(BINARY_DIR)

clean:
	-rm -rf $(BINARY_DIR)/*
