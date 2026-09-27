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

add_subdirectory(deps/googletest)
add_subdirectory(deps/motheye_platform)

add_subdirectory(engine)
add_subdirectory(game)

add_dependencies(${PROJECT_NAME} 
	motheye_platform
	engine
	game
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/deps/motheye_platform/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
	motheye_platform
	engine
	game
)

target_precompile_headers(${PROJECT_NAME} PUBLIC "pch.h")

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_directory
#                   ${CMAKE_SOURCE_DIR}/renderer/shaders/ ${CMAKE_BINARY_DIR}/bin/shaders)

enable_testing()
