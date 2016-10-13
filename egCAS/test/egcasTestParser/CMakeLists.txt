cmake_minimum_required(VERSION 2.8.11)

project(tst_egcastest_parser)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_BUILD_TYPE Release)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(cas_parser_tst ${CMAKE_CURRENT_SOURCE_DIR}/../../src/casKernel/parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(cas_scanner_tst ${CMAKE_CURRENT_SOURCE_DIR}/../../src/casKernel/parser/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp )
ADD_FLEX_BISON_DEPENDENCY(cas_scanner_tst cas_parser_tst)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/view)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/structural)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/casKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions(-std=c++11)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_definitions(-std=c++11)
endif()

# Widgets finds its own dependencies.
find_package(Qt5 COMPONENTS Core Widgets Test)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/casKernel/parser)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/structural)

file(GLOB_RECURSE tst_egcastest_parser_concrete_SOURCES "../../src/structural/concreteNodes/*.cpp")

set(tst_egcastest_parser_SOURCES
        tst_egcastest_parser.cpp 
        ../../src/casKernel/parser/interpreter.cpp 
        ../../src/casKernel/parser/egckernelparser.cpp
        ${BISON_cas_parser_tst_OUTPUTS}
        ${FLEX_cas_scanner_tst_OUTPUTS}
        ../../src/structural/specialNodes/egcunarynode.cpp
        ../../src/structural/specialNodes/egcbinarynode.cpp
        ../../src/structural/specialNodes/egcflexnode.cpp
        ../../src/structural/specialNodes/egcnode.cpp
        ../../src/structural/egcnodecreator.cpp
        ../../src/structural/specialNodes/egccontainernode.cpp
        ../../src/structural/specialNodes/egcargumentsnode.cpp
        ${tst_egcastest_parser_concrete_SOURCES}
        ../../src/structural/iterator/screenHelpers/egcidnodeiter.cpp
        ../../src/structural/iterator/screenHelpers/egcsubidnodeiter.cpp
        ../../src/structural/iterator/egcnodeiterator.cpp
        ../../src/structural/iterator/egcscrpositerator.cpp
        ../../src/structural/entities/egcformulaentity.cpp
        ../../src/structural/entities/egcentity.cpp
        ../../src/structural/specialNodes/egcbasenode.cpp
        ../../src/structural/specialNodes/egcemptynode.cpp
        ../../src/structural/visitor/egcnodevisitor.cpp
        ../../src/structural/visitor/egcmaximavisitor.cpp
        ../../src/structural/visitor/egcmathmlvisitor.cpp
        ../../src/structural/visitor/egcmathmllookup.cpp
        ../../src/structural/specialNodes/egcbinaryoperator.cpp
        ../../src/utils/egcutfcodepoint.cpp
)

#set the verbosity level of the scanner and parser
add_definitions(-DEGC_SCANNER_DEBUG=0)
add_definitions(-DEGC_PARSER_DEBUG=0)

add_executable(tst_egcastest_parser ${tst_egcastest_parser_SOURCES} )
add_dependencies(tst_egcastest_parser egCAS)
target_link_libraries(tst_egcastest_parser Qt5::Widgets Qt5::Core Qt5::Test)


