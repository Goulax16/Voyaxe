# CMakeLists.txt for Voyaxe project

cmake_minimum_required(VERSION 3.16)
project(Voyaxe)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/deps)

# Collect all .cpp, .h, and .hpp files
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/*.h ${CMAKE_SOURCE_DIR}/*.hpp)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Add dependencies
if(EXISTS ${CMAKE_SOURCE_DIR}/deps/glad)
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/glad)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/deps/glfw)
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/glfw)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/deps/glm)
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/glm)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/deps/json)
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/json)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/deps/khr)
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/khr)
endif()
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_SOURCE_DIR}/deps)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC glm glad glfw json khr entt stb_image)