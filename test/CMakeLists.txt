# This is the project CMakeLists.txt file for the test subproject
cmake_minimum_required(VERSION 3.5)

# Include the components directory of the main application:
#
#set(EXTRA_COMPONENT_DIRS "../components", "${IDF_PATH}/tools/unit-test-app/components" CACHE STRING "List of extra components")
#
set(EXTRA_COMPONENT_DIRS "../components", "${IDF_PATH}/tools/unit-test-app/components" CACHE STRING "List of extra components")



# Set the components to include the tests for.
# This can be overriden from CMake cache:
# - when invoking CMake directly: cmake -D TEST_COMPONENTS="xxxxx" ..
# - when using idf.py: idf.py -T xxxxx build
#
set(TEST_COMPONENTS "appgraphics" "appsleep" "mqtt" CACHE STRING "List of components to test")
#
#set(TEST_COMPONENTS "AppGraphics" CACHE STRING "List of components to test")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
#include($ENV{IDF_PATH}/tools/unit-test-app/components)
project(hygienie_project_test)
