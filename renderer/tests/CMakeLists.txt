cmake_minimum_required(VERSION 3.21)
project("renderer_tests")

add_executable(${PROJECT_NAME}
	"resource_handle_tests.cpp"	
)

add_dependencies(${PROJECT_NAME}
	renderer
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

target_link_libraries(${PROJECT_NAME}
	renderer
	motheye_dx12
	gtest_main
	gtest
)
	
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
