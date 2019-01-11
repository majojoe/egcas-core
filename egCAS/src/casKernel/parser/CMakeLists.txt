# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be 11 or 14
set(CMAKE_CXX_STANDARD 11)

set(ANTLR4_GRAMMAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Egc.g4)
set(ANTLR4_GRAMMAR_NAME "Egc")

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

set(ANTLR4_ZIP_REPOSITORY ${CMAKE_CURRENT_SOURCE_DIR}/antlr4/antlr4_cpp_runtime)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/antlr4/antlr-4.7.2-complete.jar)
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(egcGrammar ${ANTLR4_GRAMMAR_FILE} VISITOR)

# include generated files in project environment
include_directories(${ANTLR_egcGrammar_OUTPUT_DIR})

# add generated grammar to demo binary target
add_library(egcas_parser ${ANTLR_egcGrammar_CXX_OUTPUTS})
target_link_libraries(egcas_parser antlr4_static) 

#########################################
#generate the java gui
#########################################
find_package(Java COMPONENTS Development)

if (NOT ${Java_FOUND})
        message( FATAL_ERROR  "Java compiler and/or runtime has not been found. Please install an appropriate java environment for your OS")
endif()


if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
        set(ENV{CLASSPATH} ".:${ANTLR_EXECUTABLE}")
        set(ANTLR4_RUNTIME java -Xmx500M -cp "$ENV{CLASSPATH}")
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        set(ENV_CLASSPATH ".$<SEMICOLON>${ANTLR_EXECUTABLE}")
        file(TO_NATIVE_PATH "${ENV_CLASSPATH}" ENV_CLASSPATH)
        set(ENV{CLASSPATH} "${ENV_CLASSPATH}")
        set(ANTLR4_RUNTIME java -Xmx500M -cp "${ENV_CLASSPATH}")
        file(TO_NATIVE_PATH "${ANTLR4_RUNTIME}" ANTLR4_RUNTIME)
endif()

if( NOT ANTLR4_GRAMMAR_FILE)
        message( FATAL_ERROR  "Variable ANTLR4_GRAMMAR_FILE not set, set ANTLR4_GRAMMAR_FILE to hold the location of the grammar file (*.g4) from within your program.")        
endif()
if( NOT ANTLR4_GRAMMAR_NAME)
        message( FATAL_ERROR  "Variable ANTLR4_GRAMMAR_NAME not set, set ANTLR4_GRAMMAR_NAME to hold the grammar name as defined in your *.g4 grammar file.")        
endif()

set(PARSER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/parser_gen)
set(ANTLR4_JAVA_BASE_NAME "${PARSER_OUTPUT_DIR}/${ANTLR4_GRAMMAR_NAME}")
set(ANTLR4_JAVA_SOURCE "${ANTLR4_JAVA_BASE_NAME}BaseListener.java" "${ANTLR4_JAVA_BASE_NAME}Listener.java" "${ANTLR4_JAVA_BASE_NAME}Lexer.java" "${ANTLR4_JAVA_BASE_NAME}Parser.java")                  
                  
add_custom_target(antlr4_run
                  COMMAND ${ANTLR4_RUNTIME} org.antlr.v4.Tool ${ANTLR4_GRAMMAR_FILE} -o ${PARSER_OUTPUT_DIR}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "execute antr4")
                  
add_custom_target(antlr4_grun COMMAND ${Java_JAVAC_EXECUTABLE} -cp "${CMAKE_CURRENT_SOURCE_DIR}/antlr-${ANTLR4_VERSION}-complete.jar:$ENV{CLASSPATH}" ${ANTLR4_JAVA_SOURCE}
                   WORKING_DIRECTORY ${PARSER_OUTPUT_DIR}
                   COMMENT "compile grammar (java)"
                   VERBATIM)                                 
add_dependencies(antlr4_grun antlr4_run)                  

                  
add_custom_target(egc_parser_antlr4_gui_run COMMAND ${ANTLR4_RUNTIME} org.antlr.v4.gui.TestRig ${ANTLR4_GRAMMAR_NAME} "formula" -gui
                  WORKING_DIRECTORY ${PARSER_OUTPUT_DIR} COMMENT "execute antr grun: Enter text and press Enter followed by Ctrl + D (Unix) or Ctrl + Z (Windows) (means EOF) - to see the gui")
add_dependencies(egc_parser_antlr4_gui_run antlr4_grun)
