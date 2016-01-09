cmake_minimum_required(VERSION 2.8.11)

project(tst_egcastest_utf_encoding)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_BUILD_TYPE Release)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/utils)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions(-std=c++11)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_definitions(-std=c++11)
endif()

# Widgets finds its own dependencies.
find_package(Qt5 COMPONENTS Core Widgets Test)


set(tst_egcastest_utf_encoding_SOURCES
        tst_egcastest_utf_encoding.cpp 
        ../../src/utils/egcutfcodepoint.cpp
)

add_executable(tst_egcastest_utf_encoding ${tst_egcastest_utf_encoding_SOURCES} )
add_dependencies(tst_egcastest_utf_encoding egCAS)
target_link_libraries(tst_egcastest_utf_encoding Qt5::Widgets Qt5::Core Qt5::Test)


