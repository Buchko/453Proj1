# Compiler and flags
CC = gcc
CFLAGS = -Wall -g


# Source files
SRCS = src/main.c src/darr.c src/process.c src/dll.c

# Object files
OBJS = $(SRCS:.c=.o)

# Build the executable
all: $(OBJS)
	$(CC) $(CFLAGS) -o scheduler $^


# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) all
