CXX ?= g++
COMMON_FLAGS := -std=c++17 -O0 -g -fno-omit-frame-pointer \
	-Wall -Wextra -Wpedantic
SOURCE := examples/memory_errors.cpp
BUILD_DIR := build

.PHONY: all normal asan ubsan clean

all: normal asan ubsan

normal: $(BUILD_DIR)/memory_errors

asan: $(BUILD_DIR)/memory_errors_asan

ubsan: $(BUILD_DIR)/memory_errors_ubsan

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/memory_errors: $(SOURCE) | $(BUILD_DIR)
	$(CXX) $(COMMON_FLAGS) $< -o $@

$(BUILD_DIR)/memory_errors_asan: $(SOURCE) | $(BUILD_DIR)
	$(CXX) $(COMMON_FLAGS) -fsanitize=address $< -o $@

$(BUILD_DIR)/memory_errors_ubsan: $(SOURCE) | $(BUILD_DIR)
	$(CXX) $(COMMON_FLAGS) -fsanitize=undefined \
		-fno-sanitize-recover=all $< -o $@

clean:
	rm -rf $(BUILD_DIR)

