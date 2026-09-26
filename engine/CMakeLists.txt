cmake_minimum_required(VERSION 3.28)
project("engine")

add_library(${PROJECT_NAME} STATIC
	"engine.h"
	"engine.cpp"
	"clock_source.h"

	#"input/input_device.h"
	#"input/controller_state.h"
	#"input/mouse_keyboard_state.h"
	#"input/input_state.h"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	# Force output to go to the same directory as the main project.
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/engine
)

target_precompile_headers(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/pch.h")

target_link_libraries(${PROJECT_NAME} PUBLIC
	motheye_platform
)

