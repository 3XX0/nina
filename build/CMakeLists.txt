cmake_minimum_required (VERSION 2.6)

# Project configuration

project (NINA)
set (VERSION "0.2 Beta")

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../bin/${CMAKE_BUILD_TYPE})
set (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${CMAKE_BUILD_TYPE})
set (UTESTS_AND_EXAMPLES OFF CACHE BOOL "Choose whether unit-tests and examples should be generated or not")

# Cmake includes

include (CMakeWin32.cmake)
include (CMakeUnix.cmake)
include (CMakeLinux.cmake)

# Directories of inclusion

include_directories (
		${CMAKE_CURRENT_SOURCE_DIR}/../inc/
)

# NINA Library configuration

add_library (
		nina
		SHARED
		${NINA_SRC_FILES}
)

set_source_files_properties (
		${NINA_SRC_FILES}
		PROPERTIES
		COMPILE_FLAGS
		"-DNINA_DLLEXPORT"
)

# Configuration of the binary unit test

if (UTESTS_AND_EXAMPLES)

file (
		GLOB_RECURSE
		UNIT_TEST_SRC_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/../unit-tests/*.cpp
)

add_executable (
		nina_unit_test
		${UNIT_TEST_SRC_FILES}
)

target_link_libraries (nina_unit_test nina)
include (CMakeWin32.cmake)
include (CMakeLinux.cmake)

# Build of examples

add_executable (
		sock_acceptor
		${CMAKE_CURRENT_SOURCE_DIR}/../examples/sock_acceptor.cpp
)
target_link_libraries (sock_acceptor nina)

add_executable (
		sock_connector
		${CMAKE_CURRENT_SOURCE_DIR}/../examples/sock_connector.cpp
)
target_link_libraries (sock_connector nina)

add_executable (
		echo_server
		${CMAKE_CURRENT_SOURCE_DIR}/../examples/echo_server.cpp
)
target_link_libraries (echo_server nina)

endif()

# Documentation generation

find_package (Doxygen)

if (DOXYGEN_FOUND STREQUAL "NO")
    message (WARNING "Doxygen not found. Please get a copy http://www.doxygen.org")
else ()
	configure_file (
			${CMAKE_CURRENT_SOURCE_DIR}/../doc/doxyfile.in
			${CMAKE_CURRENT_SOURCE_DIR}/../doc/doxyfile
	)

	add_custom_target (
			doxygen
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../doc/doxyfile
	)

	set_property (
			DIRECTORY
			APPEND PROPERTY
			ADDITIONAL_MAKE_CLEAN_FILES
			${CMAKE_CURRENT_SOURCE_DIR}/../doc/api-doc
	)

	get_target_property (DOC_TARGET doc TYPE)
	if (NOT DOC_TARGET)
		add_custom_target (doc)
	endif ()
	add_dependencies (doc doxygen)
endif ()
