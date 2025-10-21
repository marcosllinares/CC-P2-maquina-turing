# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
DEBUG_FLAGS = -g -DDEBUG -O0

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/pda

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS = $(OBJECTS:.o=.d)

# Header files (for dependency tracking)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp)

# Include directory flag
INCLUDE_FLAGS = -I$(INCLUDE_DIR)

# Default target
all: $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -MMD -MP -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create bin directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Clean and rebuild
rebuild: clean all

# Run the program with example data
run: $(TARGET)
	@echo "Running PDA simulator..."
	@echo "Usage: ./$(TARGET) <definition_file> [--trace] [--mode=APv|APf]"
	@echo "Example files available in data/ directory"

# Test with example files
test-apf: $(TARGET)
	@echo "Testing with APf examples..."
	@for file in data/APf/*.txt; do \
		echo "Testing $$file"; \
		./$(TARGET) "$$file" --mode=APf; \
	done

test-apv: $(TARGET)
	@echo "Testing with APv examples..."
	@for file in data/APv/*.txt; do \
		echo "Testing $$file"; \
		./$(TARGET) "$$file" --mode=APv; \
	done

test: test-apf test-apv

# Install (copy to system location)
install: $(TARGET)
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(TARGET) $(DESTDIR)/usr/local/bin/

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  debug    - Build with debug flags"
	@echo "  clean    - Remove build artifacts"
	@echo "  rebuild  - Clean and build"
	@echo "  run      - Build and show usage information"
	@echo "  test     - Run tests with example files"
	@echo "  test-apf - Test with APf example files"
	@echo "  test-apv - Test with APv example files"
	@echo "  install  - Install to system"
	@echo "  help     - Show this help"

# Include dependency files
-include $(DEPS)

# Declare phony targets
.PHONY: all debug clean rebuild run test test-apf test-apv install help
