
cmake_minimum_required(VERSION 2.8)

project(cas_output_parser)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(cas_parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(cas_scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp )
ADD_FLEX_BISON_DEPENDENCY(cas_scanner cas_parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX)
        add_definitions(-std=c++11)
endif()

# Widgets finds its own dependencies.
find_package(Qt5 COMPONENTS Core)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(../../structural/specialNodes)
include_directories(../../structural/concreteNodes)

add_executable(cas_output_parser
        main.cpp
        interpreter.cpp 
        egckernelparser.cpp
        ${BISON_cas_parser_OUTPUTS}
        ${FLEX_cas_scanner_OUTPUTS}
        ../../structural/specialNodes/egcunaryexpressionnode.cpp
        ../../structural/specialNodes/egcbinaryexpressionnode.cpp
        ../../structural/specialNodes/egcexpressionnode.cpp
        ../../structural/concreteNodes/egcrootexpressionnode.cpp
        ../../structural/concreteNodes/egcnumberexpressionnode.cpp
        ../../structural/egcexpressionnodecreator.cpp
        ../../structural/concreteNodes/egcparenthesisexpressionnode.cpp
        ../../structural/specialNodes/egccontainernode.cpp
        ../../structural/concreteNodes/egcvariableexpressionnode.cpp
        ../../structural/iterator/egcexpressionnodeiterator.cpp
        ../../structural/egcformulaexpression.cpp
        ../../structural/specialNodes/egcbaseexpressionnode.cpp
        ../../structural/specialNodes/egcemptyexpressionnode.cpp
        ../../structural/visitor/egcnodevisitor.cpp
        ../../structural/visitor/egcmaximavisitor.cpp
        ../../structural/visitor/egcmathmlvisitor.cpp
)

#set the verbosity level of the scanner and parser
add_definitions(-DEGC_SCANNER_DEBUG=0)
add_definitions(-DEGC_PARSER_DEBUG=0)

target_link_libraries(cas_output_parser Qt5::Core)


