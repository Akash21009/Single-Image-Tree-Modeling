# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/e/OpenGl/labs/lab7/singleimagetreemodelling

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/OpenGl/labs/lab7/singleimagetreemodelling

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/e/OpenGl/labs/lab7/singleimagetreemodelling/CMakeFiles /mnt/e/OpenGl/labs/lab7/singleimagetreemodelling//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/e/OpenGl/labs/lab7/singleimagetreemodelling/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named project

# Build rule for target.
project: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 project
.PHONY : project

# fast build rule for target.
project/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/build
.PHONY : project/fast

depends/imgui/imgui.o: depends/imgui/imgui.cpp.o
.PHONY : depends/imgui/imgui.o

# target to build an object file
depends/imgui/imgui.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui.cpp.o
.PHONY : depends/imgui/imgui.cpp.o

depends/imgui/imgui.i: depends/imgui/imgui.cpp.i
.PHONY : depends/imgui/imgui.i

# target to preprocess a source file
depends/imgui/imgui.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui.cpp.i
.PHONY : depends/imgui/imgui.cpp.i

depends/imgui/imgui.s: depends/imgui/imgui.cpp.s
.PHONY : depends/imgui/imgui.s

# target to generate assembly for a file
depends/imgui/imgui.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui.cpp.s
.PHONY : depends/imgui/imgui.cpp.s

depends/imgui/imgui_demo.o: depends/imgui/imgui_demo.cpp.o
.PHONY : depends/imgui/imgui_demo.o

# target to build an object file
depends/imgui/imgui_demo.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_demo.cpp.o
.PHONY : depends/imgui/imgui_demo.cpp.o

depends/imgui/imgui_demo.i: depends/imgui/imgui_demo.cpp.i
.PHONY : depends/imgui/imgui_demo.i

# target to preprocess a source file
depends/imgui/imgui_demo.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_demo.cpp.i
.PHONY : depends/imgui/imgui_demo.cpp.i

depends/imgui/imgui_demo.s: depends/imgui/imgui_demo.cpp.s
.PHONY : depends/imgui/imgui_demo.s

# target to generate assembly for a file
depends/imgui/imgui_demo.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_demo.cpp.s
.PHONY : depends/imgui/imgui_demo.cpp.s

depends/imgui/imgui_draw.o: depends/imgui/imgui_draw.cpp.o
.PHONY : depends/imgui/imgui_draw.o

# target to build an object file
depends/imgui/imgui_draw.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_draw.cpp.o
.PHONY : depends/imgui/imgui_draw.cpp.o

depends/imgui/imgui_draw.i: depends/imgui/imgui_draw.cpp.i
.PHONY : depends/imgui/imgui_draw.i

# target to preprocess a source file
depends/imgui/imgui_draw.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_draw.cpp.i
.PHONY : depends/imgui/imgui_draw.cpp.i

depends/imgui/imgui_draw.s: depends/imgui/imgui_draw.cpp.s
.PHONY : depends/imgui/imgui_draw.s

# target to generate assembly for a file
depends/imgui/imgui_draw.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_draw.cpp.s
.PHONY : depends/imgui/imgui_draw.cpp.s

depends/imgui/imgui_impl_glfw.o: depends/imgui/imgui_impl_glfw.cpp.o
.PHONY : depends/imgui/imgui_impl_glfw.o

# target to build an object file
depends/imgui/imgui_impl_glfw.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_impl_glfw.cpp.o
.PHONY : depends/imgui/imgui_impl_glfw.cpp.o

depends/imgui/imgui_impl_glfw.i: depends/imgui/imgui_impl_glfw.cpp.i
.PHONY : depends/imgui/imgui_impl_glfw.i

# target to preprocess a source file
depends/imgui/imgui_impl_glfw.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_impl_glfw.cpp.i
.PHONY : depends/imgui/imgui_impl_glfw.cpp.i

depends/imgui/imgui_impl_glfw.s: depends/imgui/imgui_impl_glfw.cpp.s
.PHONY : depends/imgui/imgui_impl_glfw.s

# target to generate assembly for a file
depends/imgui/imgui_impl_glfw.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_impl_glfw.cpp.s
.PHONY : depends/imgui/imgui_impl_glfw.cpp.s

depends/imgui/imgui_impl_opengl3.o: depends/imgui/imgui_impl_opengl3.cpp.o
.PHONY : depends/imgui/imgui_impl_opengl3.o

# target to build an object file
depends/imgui/imgui_impl_opengl3.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_impl_opengl3.cpp.o
.PHONY : depends/imgui/imgui_impl_opengl3.cpp.o

depends/imgui/imgui_impl_opengl3.i: depends/imgui/imgui_impl_opengl3.cpp.i
.PHONY : depends/imgui/imgui_impl_opengl3.i

# target to preprocess a source file
depends/imgui/imgui_impl_opengl3.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_impl_opengl3.cpp.i
.PHONY : depends/imgui/imgui_impl_opengl3.cpp.i

depends/imgui/imgui_impl_opengl3.s: depends/imgui/imgui_impl_opengl3.cpp.s
.PHONY : depends/imgui/imgui_impl_opengl3.s

# target to generate assembly for a file
depends/imgui/imgui_impl_opengl3.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_impl_opengl3.cpp.s
.PHONY : depends/imgui/imgui_impl_opengl3.cpp.s

depends/imgui/imgui_widgets.o: depends/imgui/imgui_widgets.cpp.o
.PHONY : depends/imgui/imgui_widgets.o

# target to build an object file
depends/imgui/imgui_widgets.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_widgets.cpp.o
.PHONY : depends/imgui/imgui_widgets.cpp.o

depends/imgui/imgui_widgets.i: depends/imgui/imgui_widgets.cpp.i
.PHONY : depends/imgui/imgui_widgets.i

# target to preprocess a source file
depends/imgui/imgui_widgets.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_widgets.cpp.i
.PHONY : depends/imgui/imgui_widgets.cpp.i

depends/imgui/imgui_widgets.s: depends/imgui/imgui_widgets.cpp.s
.PHONY : depends/imgui/imgui_widgets.s

# target to generate assembly for a file
depends/imgui/imgui_widgets.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/depends/imgui/imgui_widgets.cpp.s
.PHONY : depends/imgui/imgui_widgets.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/utils.o: src/utils.cpp.o
.PHONY : src/utils.o

# target to build an object file
src/utils.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/src/utils.cpp.o
.PHONY : src/utils.cpp.o

src/utils.i: src/utils.cpp.i
.PHONY : src/utils.i

# target to preprocess a source file
src/utils.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/src/utils.cpp.i
.PHONY : src/utils.cpp.i

src/utils.s: src/utils.cpp.s
.PHONY : src/utils.s

# target to generate assembly for a file
src/utils.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/project.dir/build.make CMakeFiles/project.dir/src/utils.cpp.s
.PHONY : src/utils.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... project"
	@echo "... depends/imgui/imgui.o"
	@echo "... depends/imgui/imgui.i"
	@echo "... depends/imgui/imgui.s"
	@echo "... depends/imgui/imgui_demo.o"
	@echo "... depends/imgui/imgui_demo.i"
	@echo "... depends/imgui/imgui_demo.s"
	@echo "... depends/imgui/imgui_draw.o"
	@echo "... depends/imgui/imgui_draw.i"
	@echo "... depends/imgui/imgui_draw.s"
	@echo "... depends/imgui/imgui_impl_glfw.o"
	@echo "... depends/imgui/imgui_impl_glfw.i"
	@echo "... depends/imgui/imgui_impl_glfw.s"
	@echo "... depends/imgui/imgui_impl_opengl3.o"
	@echo "... depends/imgui/imgui_impl_opengl3.i"
	@echo "... depends/imgui/imgui_impl_opengl3.s"
	@echo "... depends/imgui/imgui_widgets.o"
	@echo "... depends/imgui/imgui_widgets.i"
	@echo "... depends/imgui/imgui_widgets.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/utils.o"
	@echo "... src/utils.i"
	@echo "... src/utils.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

