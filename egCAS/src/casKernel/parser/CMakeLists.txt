
cmake_minimum_required(VERSION 2.8)

project(maxima_output_parser) 

find_package(BISON)
find_package(FLEX)

BISON_TARGET(maxima_parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(maxima_scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp )
#FLEX_TARGET(maxima_scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp COMPILE_FLAGS "--header-file=${CMAKE_CURRENT_BINARY_DIR}/maxima_lex.h")
ADD_FLEX_BISON_DEPENDENCY(maxima_scanner maxima_parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(maxima_output_parser
        main.cpp
        interpreter.cpp 
        command.cpp    
        ${BISON_maxima_parser_OUTPUTS}    
        ${FLEX_maxima_scanner_OUTPUTS}
)

#target_link_libraries(maxima_output_parser 
#        m
#)



