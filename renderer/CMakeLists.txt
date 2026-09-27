cmake_minimum_required(VERSION 3.28)
project("renderer")

set(LIBRARY_HEADERS
	"camera_instance.h"
	"camera_resource.h"
	"combo_descriptor_manager.h"
	"constant_buffer.h"
	"constant_manager.h"
	"frame.h"
	"framework.h"
	"instance_base.h"
	"instance_handle.h"
	"instance_kind.h"
	"instance_manager.h"
	"light_instance.h"
	"light_resource.h"
	"material_resource.h"
	"mesh_resource.h"
	"pipeline_states.h"
	"renderer.h"
	"resource_handle.h"
	"resource_kind.h"
	"resource_limits.h"
	"resource_manager.h"
	"root_parameters.h"
	"solid_instance.h"
	"solid_resource.h"
	"texture_resource.h"
)

set(LIBRARY_SOURCES
	"pipeline_states.cpp"
	"renderer.cpp"
)

add_library(${PROJECT_NAME} STATIC ${LIBRARY_HEADERS} ${LIBRARY_SOURCES})

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
	${CMAKE_SOURCE_DIR}/deps/motheye_dx12/include
	${CMAKE_SOURCE_DIR}/deps/motheye_dx12/deps/DirectXTK12/inc
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
	DirectXTK12
	d3d12.lib 
	dxgi.lib 
	d3dcompiler.lib
)

target_precompile_headers(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/pch.h")

enable_testing()
add_subdirectory(tests)