cmake_minimum_required(VERSION 3.12.2)

project (thread_example)

# If you dont have CC and CXX global variables set you can set compilers manualy
#set(CMAKE_C_COMPILER "/path/to/c/compiler")
#set(CMAKE_CXX_COMPILER "path/to/cpp/compiler")

# Add libraries to compilation
# NOTE: if copying this code to your project cmake you should probably remove
# second argument of add_subdirectory. Here it is needed because library
# directory is actualy a parent directory of the main project directory. Usually
# this will not be the case.
add_subdirectory ("${PROJECT_SOURCE_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}/thread")
include_directories ("${PROJECT_SOURCE_DIR}/..")
set (EXTRA_LIBS ${EXTRA_LIBS} thread)


add_executable(thread_example example1.c ${SOURCES})

# Link libraries to this project
target_link_libraries (thread_example ${EXTRA_LIBS})
