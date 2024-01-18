#
# This is a project Makefile for the test subproject.
#

PROJECT_NAME := hygienie_proejct_test

# Include the components directory of the main application:
#
EXTRA_COMPONENT_DIRS := $(realpath ../components)

echo '$realpath'
# Set the components to include the tests for.
# This can be overriden from the command line
# (e.g. 'make TEST_COMPONENTS=xxxx flash monitor')
#

TEST_COMPONENTS=appgraphics
include $(IDF_PATH)/make/project.mk
