cmake_minimum_required(VERSION 2.8.11)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_BUILD_TYPE Release)

if(CMAKE_COMPILER_IS_GNUCXX)
#        set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary        
        add_definitions(-std=c++11)
endif()

# Widgets finds its own dependencies.
find_package(Qt5 COMPONENTS Widgets Core Test)

include_directories(${CMAKE_CURRENT_BINARY_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/../../src/")

set(tst_egcastest_structural_SOURCES
        tst_egcastest_structural.cpp 
        ../../src/structural/specialNodes/egcunarynode.cpp
        ../../src/structural/specialNodes/egcbinarynode.cpp
        ../../src/structural/specialNodes/egcflexnode.cpp
        ../../src/structural/specialNodes/egcnode.cpp
        ../../src/structural/concreteNodes/egcrootnode.cpp
        ../../src/structural/concreteNodes/egcnumbernode.cpp
        ../../src/structural/egcnodecreator.cpp
        ../../src/structural/concreteNodes/egcparenthesisnode.cpp
        ../../src/structural/specialNodes/egccontainernode.cpp 
        ../../src/structural/concreteNodes/egcvariablenode.cpp
        ../../src/structural/concreteNodes/egcfunctionnode.cpp
        ../../src/structural/iterator/egcnodeiterator.cpp
        ../../src/structural/egcformulaexpression.cpp 
        ../../src/structural/specialNodes/egcbasenode.cpp
        ../../src/structural/specialNodes/egcemptynode.cpp
        ../../src/structural/visitor/egcnodevisitor.cpp 
        ../../src/structural/visitor/egcmaximavisitor.cpp 
        ../../src/structural/visitor/egcmathmlvisitor.cpp
)

add_executable(tst_egcastest_structural ${tst_egcastest_structural_SOURCES} )

target_link_libraries(tst_egcastest_structural mmlegcas Qt5::Widgets Qt5::Core Qt5::Test)
