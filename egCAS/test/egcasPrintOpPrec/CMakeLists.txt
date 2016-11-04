cmake_minimum_required(VERSION 2.8.11)

project(tst_print_op_precedence)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions(-std=c++11)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_definitions(-std=c++11)
endif()

# Widgets finds its own dependencies.
find_package(Qt5 COMPONENTS Core Widgets Test)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/structural)

file(GLOB_RECURSE tst_print_op_precedence_concrete_SOURCES "../../src/structural/concreteNodes/*.cpp")

set(tst_print_op_precedence_SOURCES
        tst_print_op_precedence.cpp 
        ../../src/structural/specialNodes/egcunarynode.cpp
        ../../src/structural/specialNodes/egcbinarynode.cpp
        ../../src/structural/specialNodes/egcflexnode.cpp
        ../../src/structural/specialNodes/egcnode.cpp
        ../../src/structural/specialNodes/egcargumentsnode.cpp
        ../../src/structural/specialNodes/egcbinaryoperator.cpp
        ../../src/structural/specialNodes/egccontainernode.cpp
        ../../src/structural/specialNodes/egcbasenode.cpp
        ../../src/structural/specialNodes/egcemptynode.cpp
        ../../src/structural/egcnodecreator.cpp
        ../../src/utils/egcutfcodepoint.cpp
        ${tst_print_op_precedence_concrete_SOURCES}
)

add_executable(tst_print_op_precedence ${tst_print_op_precedence_SOURCES} )
target_link_libraries(tst_print_op_precedence Qt5::Core Qt5::Test)


add_custom_target(tst_print_opprec_creator COMMAND cog.py -r -D BaseDirToSearch=${CMAKE_CURRENT_SOURCE_DIR}/../../src/
                  ${CMAKE_CURRENT_SOURCE_DIR}/tst_print_op_precedence.cpp
                  DEPENDS tst_print_op_precedence.cpp COMMENT "generate list for printing operator precedence." )
add_dependencies(tst_print_op_precedence tst_print_opprec_creator)
