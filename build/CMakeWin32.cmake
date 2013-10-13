if (WIN32)

# NINA Library configuration

set (
		NINA_SRC_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaPacket.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaAddr.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaInetAddr.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaIOContainer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaOS.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaPollPolicy.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaSAP.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaSelectPolicy.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaSock.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaSockIO.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaSockStream.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaSockDatagram.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaSockAcceptor.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaSockConnector.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaTime.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/../src/NinaError.cpp
)

if (WIN32_PREREQUISITE)
target_link_libraries (nina ws2_32 iphlpapi)
target_link_libraries (nina optimized msvcrt)
target_link_libraries (nina debug msvcrtd)

# Configuration of the binary unit test

set_source_files_properties (
			${UNIT_TEST_SRC_FILES}
			PROPERTIES
			COMPILE_FLAGS
			"-D_CRT_SECURE_NO_WARNINGS"
)
endif ()

# Install settings

#install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc/api-doc/* DESTINATION Documentation/)
#install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/../inc/* DESTINATION Includes/)
#install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${CMAKE_BUILD_TYPE}/* DESTINATION Library/)


set (WIN32_PREREQUISITE "true")
endif (WIN32)
