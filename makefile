# let make know what shell to use
SHELL = bash
# if target os is windows (i.e. %OS% is defined), use git bash
ifdef OS
SHELL = C:/Program Files/Git/bin/bash.exe
endif
# let make know which C compiler to use
CC = gcc
# let make know to use "O2" optimization and "W(arning)all" to surface compiler warnings
CFLAGS = -std=c17 -pedantic -O2 -Wall

# user variables
program_name 			:= lsharpc
make_directory			:= mkdir -p
remove_file				:= rm -f
remove_directory		:= $(remove_file) -d
open_file				:= open
ifdef OS
open_file				:= start
endif
source_directory 		:= src
build_directory 		:= build
publish_directory 		:= bin
docs_directory			:= docs
tests_directory			:= tests
tests_build_directory	:= build/tests
program					:= $(publish_directory)/$(program_name).exe
program_tests			:= $(publish_directory)/$(program_name)-tests.exe
header_files 			:= $(shell find $(source_directory) -name "*.h")
source_files	 		:= $(shell find $(source_directory) -name "*.c")
test_dependencies		:= $(shell find $(source_directory)/*/ -name "*.c")
test_files	 			:= $(shell find $(tests_directory) -name "*.c")
compiled_objects 		:= $(patsubst %.c,%.o,$(source_files))
compiled_test_objects 	:= $(patsubst %.c,%.o,$(test_files)) $(patsubst %.c,%.o,$(test_dependencies))

# "all" must be the first target of Make
all: $(program)

# object files are built using their .c source and .h header files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# generate the program and move all .o files to the build directory
$(program): make_build_paths $(compiled_objects)
	$(CC) $(compiled_objects) $(CFLAGS) -o $@
	mv $(shell find $(source_directory) -name "*.o") "$(build_directory)"

# creates the unit tests
tests: $(program_tests)

# generate the unit test program
$(program_tests): make_tests_paths $(compiled_test_objects)
	$(CC) $(compiled_test_objects) $(CFLAGS) -o $@
	mv $(shell find $(source_directory) -name "*.o") "$(tests_build_directory)"
	mv $(shell find $(tests_directory) -name "*.o") "$(tests_build_directory)"

# creates directories that are necessary for the build
make_build_paths:
	$(make_directory) "$(build_directory)"
	$(make_directory) "$(publish_directory)"

# creates directories that are necessary for the unit tests build
make_tests_paths:
	$(make_directory) "$(tests_build_directory)"
	$(make_directory) "$(publish_directory)"

# creates directories that are necessary for docs generation
make_docs_paths:
	$(make_directory) "$(docs_directory)"

# creates and opens the documentation for the program
docs: make_docs_paths $(program) $(compiled_objects)
	@doxygen
	@$(open_file) "$(docs_directory)/html/index.html"

# removes all generated files and directories
clean: clean_tests clean_docs clean_build

# removes files and directories created as the result of the build
clean_build:
	$(remove_file) $(build_directory)/*
	$(remove_file) $(publish_directory)/*
	$(remove_directory) "$(build_directory)"
	$(remove_directory) "$(publish_directory)"

# removes files and directories as a result of the unit tests
clean_tests:
	$(remove_file) $(tests_build_directory)/*
	$(remove_directory) "$(tests_build_directory)"

# removes files and directories as a result of docs
clean_docs:
	$(remove_file) $(docs_directory)/html/search/*
	$(remove_directory) "$(docs_directory)/html/search"
	$(remove_file) $(docs_directory)/html/*
	$(remove_directory) "$(docs_directory)/html"
	$(remove_file) $(docs_directory)/*
	$(remove_directory) "$(docs_directory)"

# runs clean and then makes a new program
rebuild: clean all