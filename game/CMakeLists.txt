cmake_minimum_required(VERSION 3.28)
project("game")

add_library(${PROJECT_NAME} STATIC
	"action_frame.h"
	"action_resolver.h"
	"controller_binder.h"
	"entity_factory.cpp"
	"entity_factory.h"
	"example_model.h"
	"fps_camera.cpp"
	"fps_camera.h"
	"game.cpp"
	"game.h"
	"input_context.h"
	"mouse_keyboard_binder.h"
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
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/engine/include
	${CMAKE_SOURCE_DIR}/deps/motheye/include
	${CMAKE_SOURCE_DIR}/deps/motheye_platform/include
	${CMAKE_SOURCE_DIR}/deps/motheye_dx12/include
	${CMAKE_SOURCE_DIR}/deps/motheye_dx12/deps/DirectXTK12/inc
)

target_precompile_headers(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/pch.h")
