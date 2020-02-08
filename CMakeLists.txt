# MIT License
# 
# Copyright (c) 2019 D0D0
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# Add this directory to the library include path.
include_directories(".")

# Set project configuration file name if not already set by upper cmake file.
if(NOT PROJECT_CONFIG_FILENAME)
    set(PROJECT_CONFIG_FILENAME "project_config.h")
endif()

# Get all include directories.
get_property(INC_DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

# Check if any headers in include directories matches project config filename.
set(PROJ_CONF_IS_INCLUDED 0)
foreach(DIR ${INC_DIRS})
    file(GLOB _PROJ_CONF ${DIR}/${PROJECT_CONFIG_FILENAME})
    if(_PROJ_CONF)
        set(PROJ_CONF_IS_INCLUDED 1)
    endif()
endforeach()


if(PROJ_CONF_IS_INCLUDED)

    # Add this definition to signal to lib that there is global configuration.
    add_compile_definitions(__GLOBAL_PROJECT_CONFIGURATION_FILE_EXISTS__=1)
    # This enables custom project filename.
    add_compile_definitions(__GLOBAL_PROJECT_CONFIGURATION_FILENAME__="${PROJECT_CONFIG_FILENAME}")

else()

    add_compile_definitions(__GLOBAL_PROJECT_CONFIGURATION_FILE_EXISTS__=0)

    # Find all header files in project directory.
    file(GLOB_RECURSE HDRS ${CMAKE_SOURCE_DIR}/*.h)

    # Check if any headers in project directories matches project config filename.
    set(PROJ_CONF_EXISTS 0)
    foreach(HDR ${HDRS})
        get_filename_component(HEADER_NAME ${HDR} NAME)
        if(HEADER_NAME STREQUAL ${PROJECT_CONFIG_FILENAME})
            set(PROJ_CONF_EXISTS 1)
            get_filename_component(PROJ_CONF_PATH ${HDR} DIRECTORY)
        endif()
    endforeach()

endif()

# If project configuration file exists but is not in any of the include directories
# warn the user about it.
if(NOT PROJ_CONF_IS_INCLUDED AND PROJ_CONF_EXISTS)
    message("WARNING: '${PROJECT_CONFIG_FILENAME}' file exists in project folder "
            "'${PROJ_CONF_PATH}' "
            "but is not in any of the include directories. Is it on purpose?")
endif()

# Compile library.
add_library(thread thread.c)
