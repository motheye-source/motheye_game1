cmake_minimum_required(VERSION 3.28)
project("motheye_game1")

add_executable(${PROJECT_NAME} WIN32

	"WinMain.cpp"

	"targetver.h"
	"resource.h"
	"app.rc"
	"app.h"
	"app.cpp"	
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	# Force output to go to the same directory as the main project.
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

set(JSONCPP_WITH_TESTS OFF)

add_subdirectory(deps/motheye)
add_subdirectory(deps/motheye_platform)
add_subdirectory(deps/motheye_dx12)
#add_subdirectory(deps/googletest)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(renderer)

add_dependencies(${PROJECT_NAME} 
	jsoncpp_lib
	DirectXTK12
	motheye_dx12
	motheye_platform
	motheye_compiler
	renderer
	engine
	game
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/engine/include
	${CMAKE_SOURCE_DIR}/deps/motheye_platform/include
	${CMAKE_SOURCE_DIR}/deps/motheye_dx12/include
	${CMAKE_SOURCE_DIR}/deps/motheye_dx12/deps/DirectXTK12/inc
	${CMAKE_SOURCE_DIR}/deps/motheye/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	jsoncpp_lib
	antlr4_static
	motheye_dx12
	motheye_platform
	motheye_blender
	motheye_compiler
	renderer
	engine
	game
)

target_precompile_headers(${PROJECT_NAME} PUBLIC "pch.h")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/renderer/shaders/ ${CMAKE_BINARY_DIR}/bin/shaders)

enable_testing()
