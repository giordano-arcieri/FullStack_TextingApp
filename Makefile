# This is the generic Makefile I use for all my projects
# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -Iinclude

# Directories
SRCDIRS = src server client
BUILDDIR = obj
TARGET = myapp

# Find all .cpp files in SRCDIRS
SOURCES = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

# Object files
OBJECTS = $(patsubst %, $(BUILDDIR)/%, $(notdir $(SOURCES:.cpp=.o)))

# Default target
all: $(TARGET)

# Link the target with all objects
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile .cpp files to .o files
define COMPILE
$(BUILDDIR)/$(notdir $(1:.cpp=.o)): $(1)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $$< -o $$@
endef

$(foreach src, $(SOURCES), $(eval $(call COMPILE, $(src))))

# Clean up
clean:
	rm -rf $(BUILDDIR) $(TARGET)

# Phony targets
.PHONY: all clean
