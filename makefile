# Define the compiler
CC := gcc

# Define any compile-time flags
CFLAGS :=

# Define the target executable
TARGET := vice

# Directory for object files
OBJDIR := bin

# Header files directory
INCDIR := inc

# Source files directory
SRCDIR := src

# Source files
SRC := $(wildcard $(SRCDIR)/*.c)

# Object files
OBJ := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Detect OS
ifeq ($(OS),Windows_NT)
    RM = del /Q
    FIX_PATH = $(subst /,\,$1)
    RMDIR = rmdir /Q /S
else
    RM = rm -f
    FIX_PATH = $1
    RMDIR = rm -rf
endif

# Ensure output directory exists
$(shell mkdir $(OBJDIR))

# Phony targets ensure these aren't skipped if files named 'all' or 'clean' exist
.PHONY: all clean

# Default target
all: $(TARGET)

# Linking the target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compiling object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c -o $@ $<

clean:
	$(RM) $(call FIX_PATH,$(TARGET) $(TARGET).exe $(OBJDIR)/*.o)
	$(RMDIR) $(call FIX_PATH,$(OBJDIR))
