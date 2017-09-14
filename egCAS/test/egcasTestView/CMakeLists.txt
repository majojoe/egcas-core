cmake_minimum_required(VERSION 2.8.11)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_BUILD_TYPE Release)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions(-std=c++11)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#        set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
        add_definitions(-std=c++11)
endif()

# Widgets finds its own dependencies.
find_package(Qt5 COMPONENTS Widgets Core Test)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/structural)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/view)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)

set(tst_egcastest_view_SOURCES
        tst_egcastest_view.cpp 
        ../../src/view/egcformulaitem.cpp 
        ../../src/view/egcasscene.cpp 
        ../../src/view/egcpixmapitem.cpp 
        ../../src/view/egctextitem.cpp 
        ../../src/view/resizehandle.cpp 
        ../../src/view/egcabstractitem.cpp
        ../../src/view/egcscreenpos.cpp
        ../../src/view/egcasiteminterface.cpp
        ../../src/view/egccrossitem.cpp
        ../../src/view/egcworksheet.cpp
        ../../src/view/grid.cpp
        ../../src/menu/egcgraphicsview.cpp
        ../../src/structural/actions/egcactionmapper.cpp
)

add_executable(tst_egcastest_view ${tst_egcastest_view_SOURCES} )
add_dependencies(tst_egcastest_view mmlegcas)
target_link_libraries(tst_egcastest_view mmlegcas Qt5::Widgets Qt5::Core Qt5::Test)
