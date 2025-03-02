# let make know what shell to use
SHELL = C:/Program Files/Git/bin/bash.exe
# let make know which C compiler to use
CC = gcc
# let make know to use "O2" optimization and "W(arning)all" to surface compiler warnings
CFLAGS = -O2 -Wall

# user variables
program_name 		:= newlangc
make_directory		:= mkdir -p
remove_file			:= rm -f
remove_directory	:= $(remove_file) -d
source_directory 	:= src
build_directory 	:= build
publish_directory 	:= bin
program				:= $(publish_directory)/$(program_name).exe
header_files 		:= $(shell find . -name "*.h")
source_files 		:= $(shell find . -name "*.c")
compiled_objects 	:= $(patsubst %.c,%.o,$(source_files))

# "all" must be the first target of Make
all: $(program)

# object files are built using their .c source and .h header files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# run the compiler to generate the program and move all .o files to the build directory
$(program): make_paths $(compiled_objects)
	$(CC) $(compiled_objects) $(CFLAGS) -o $@
	mv $(shell find . -name "*.o") "$(build_directory)"

# creates directories that are necessary for the build
make_paths:
	$(make_directory) "$(build_directory)"
	$(make_directory) "$(publish_directory)"

# removes directories created as the result of the build
clean:
	$(remove_file) $(build_directory)/*
	$(remove_file) "$(program)"
	$(remove_directory) "$(build_directory)"
	$(remove_directory) "$(publish_directory)"

# runs clean and then makes a new program
rebuild: clean all

# run will run the program
run: $(program)
	$<