
cmake_minimum_required(VERSION 2.8)

project(cas_output_parser)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(cas_parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(cas_scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp )
ADD_FLEX_BISON_DEPENDENCY(cas_scanner cas_parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(cas_output_parser
        main.cpp
        interpreter.cpp 
        command.cpp    
        ${BISON_cas_parser_OUTPUTS}
        ${FLEX_cas_scanner_OUTPUTS}
)

#set the verbosity level of the scanner and parser
add_definitions(-DEGC_SCANNER_DEBUG=0)
add_definitions(-DEGC_PARSER_DEBUG=0)

#target_link_libraries(maxima_output_parser 
#        m
#)



