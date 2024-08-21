############################# Makefile variables ###############################

# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -fPIC -ansi -O3

# Target
TARGET = calc.out

# Objects
OBJS = $(BPATH)calc.o

# Shared objects
SOBJS = $(LPATH)libsorted_list.so

# Additional shared library path
libstack_PATH = ./lib/

# Additional shared library flags
libstack_FLAG = -lstack

############################## Custom variables ################################

# Header Path
HPATH = -I ./include

# Bin Path
BPATH = ./bin/

# Src Path
SPATH = ./src/

# Test Path
TPATH = ./test/

# Lib Path
LPATH = ./lib/

################################### RULES ######################################

# Default target
all: $(TARGET)

$(TARGET): $(BPATH)calc.o $(BPATH)calc_test.o
	$(CC) $(CFLAGS) -o $@ $(BPATH)calc.o $(BPATH)calc_test.o -Wl,-rpath,$(libstack_PATH) -L$(libstack_PATH) $(libstack_FLAG)

$(BPATH)calc.o: $(SPATH)calc.c
	$(CC) $(CFLAGS) -c $<  $(HPATH) -o $@

$(BPATH)calc_test.o: $(TPATH)calc_test.c
	$(CC) $(CFLAGS) -c $<  $(HPATH) -o $@


clear: 
	rm $(BPATH)*.o

run:
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)
