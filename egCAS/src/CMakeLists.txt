cmake_minimum_required(VERSION 2.8.11)

project(egCAS)

add_subdirectory(casKernel/parser)

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
find_package(Qt5 COMPONENTS Widgets Xml PrintSupport Gui Core)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(egCAS_SOURCES
        main.cpp
        mainwindow.cpp 
        view/egcasscene.cpp 
        view/egcasiteminterface.cpp 
        view/egctextitem.cpp 
        view/egcpixmapitem.cpp 
        view/resizehandle.cpp 
        view/egcformulaitem.cpp 
        structural/specialNodes/egcexpressionnode.cpp 
        structural/specialNodes/egcbinaryexpressionnode.cpp 
        structural/specialNodes/egcunaryexpressionnode.cpp 
        structural/egcexpressionnodecreator.cpp 
        structural/visitor/egcnodevisitor.cpp 
        structural/specialNodes/egccontainernode.cpp 
        structural/egcformulaexpression.cpp 
        structural/iterator/egcexpressionnodeiterator.cpp 
        structural/specialNodes/egcbaseexpressionnode.cpp 
        structural/specialNodes/egcemptyexpressionnode.cpp 
        structural/visitor/egcmaximavisitor.cpp 
        structural/concreteNodes/egcnumberexpressionnode.cpp 
        structural/concreteNodes/egcparenthesisexpressionnode.cpp 
        structural/concreteNodes/egcrootexpressionnode.cpp 
        structural/concreteNodes/egcvariableexpressionnode.cpp 
        structural/visitor/egcmathmlvisitor.cpp 
        casKernel/egcmaximaconn.cpp 
        casKernel/egckernelconn.cpp
)

qt5_wrap_ui(EGCAS_UIS ui/mainwindow.ui)
qt5_add_resources(EGCAS_RCCS ../img/ressources.qrc)


add_executable(egCAS ${egCAS_SOURCES} ${EGCAS_RCCS} ${EGCAS_UIS} )
#only for cmake >= 3.1
#set_property(TARGET egCAS PROPERTY CXX_STANDARD 11)
#set_property(TARGET egCAS PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(egCAS mmlegcas Qt5::Xml Qt5::Widgets Qt5::PrintSupport Qt5::Gui Qt5::Core)
install (TARGETS egCAS RUNTIME DESTINATION bin)

add_custom_target(egcas_generate_node_list COMMAND cog.py -r -D BaseDirToSearch=${CMAKE_CURRENT_SOURCE_DIR} 
                  ${CMAKE_CURRENT_SOURCE_DIR}/structural/specialNodes/egcexpressionnode_gen.h 
                  DEPENDS structural/specialNodes/egcexpressionnode_gen.h COMMENT "generate node list" )
add_dependencies(egCAS egcas_generate_node_list)

add_custom_target(egcas_generate_node_creator COMMAND cog.py -r -D BaseDirToSearch=${CMAKE_CURRENT_SOURCE_DIR}
                  ${CMAKE_CURRENT_SOURCE_DIR}/structural/egcexpressionnodecreator.cpp
                  DEPENDS structural/egcexpressionnodecreator.cpp COMMENT "generate node creator" )
add_dependencies(egCAS egcas_generate_node_creator)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY " egcas is a Easy to use Graphical Computer Algebra System.")
set(CPACK_PACKAGE_NAME "egcas")
set(CPACK_DEBIAN_PACKAGE_DEPENDS libmmlegcas libqt5widgets5 libqt5xml5 libqt5printsupport5 maxima)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(PACKAGE_OS "linux")
        if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
                set(CPACK_SYSTEM_NAME "${PACKAGE_OS}-amd64")
        endif()
        if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "i386")
                set(CPACK_SYSTEM_NAME "${PACKAGE_OS}-i386")
        endif()        
endif()
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Johannes Maier <maier_jo@gmx.de>")
set(CPACK_PACKAGE_VERSION "0.0.1")

include(CPack)
