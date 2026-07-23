CXX ?= g++
CXXFLAGS := -O2 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls \
	-fno-ipa-cp -fno-ipa-icf \
	-Wall -Wextra -Wpedantic -std=c++17

BUILD_DIR := build
TARGET := $(BUILD_DIR)/perf_lab
SOURCE := examples/perf_lab.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR) results perf.data perf.data.old
