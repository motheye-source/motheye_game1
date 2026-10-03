cmake_minimum_required(VERSION 3.28)
project("engine")

add_library(${PROJECT_NAME} STATIC
	"include/motheye/engine/input/controller_state.h"
	"include/motheye/engine/input/frame_input.h"
	"include/motheye/engine/input/input_device.h"
	"include/motheye/engine/input/mouse_keyboard_state.h"

	"include/motheye/engine/world/entity.h"
	"include/motheye/engine/world/i_entity_factory.h"
	"include/motheye/engine/world/i_game_context.h"
	"include/motheye/engine/world/tick_context.h"
	"include/motheye/engine/world/transform.h"
	
	"src/engine.h"
	"src/engine.cpp"
	"src/clock_source.h"

	"src/input/controller_handler.h"
	"src/input/input_handler.h"
	"src/input/input_handler.cpp"
	"src/input/mouse_keyboard_handler.h"
	"src/input/mouse_keyboard_handler.cpp"

	"src/world/axes_geometry.h"
	"src/world/entity_manager.h"
	"src/world/entity_manager.cpp"
	"src/world/node.h"
	"src/world/resource_map.h"
	"src/world/transform.cpp"
	"src/world/world.h"
	"src/world/world.cpp"
	"src/world/world_loader.h"
	"src/world/world_loader.cpp"
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
	${CMAKE_SOURCE_DIR}/engine/include
	${CMAKE_SOURCE_DIR}/deps/motheye/include
	${CMAKE_SOURCE_DIR}/deps/motheye_dx12/include
	${CMAKE_SOURCE_DIR}/deps/motheye_dx12/deps/DirectXTK12/inc
	${CMAKE_SOURCE_DIR}/deps/motheye_platform/include
)

target_precompile_headers(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/pch.h")

target_link_libraries(${PROJECT_NAME} PUBLIC
	motheye_platform
)

