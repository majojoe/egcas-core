cmake_minimum_required(VERSION 2.8.11)


add_subdirectory(casKernel/parser)

#set the project name
add_definitions(-DEGC_PROJ_NAME=egcas)

if(POLICY CMP0071)
        cmake_policy(SET CMP0071 NEW)
endif()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(debug_maxima_kernel "switch debugging for maxima kernel on" OFF)
if(debug_maxima_kernel)
        add_definitions(-DDEBUG_MAXIMA_KERNEL )
endif()

option(debug_mathml_generation "switch debugging for mathml generation (mathml visitor) on" OFF)
if(debug_mathml_generation)
        add_definitions(-DDEBUG_MATHML_GENERATION )
endif()

option(debug_kernel_command_generation "switch debugging for kernel command generation (maxima visitor) on" OFF)
if(debug_kernel_command_generation)
        add_definitions(-DDEBUG_KERNEL_COMMAND_GENERATION )
endif()

option(debug_scene_rendering_pos "switch debugging for scene position rendering on" OFF)
if(debug_scene_rendering_pos)
        add_definitions(-DDEBUG_SCENE_RENDERING_POS )
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_definitions(-DLOAD_EGCAS_EXAMPLES_FILE="${CMAKE_SOURCE_DIR}/examples/default.egc")
endif()

set_property(TARGET mathmlview PROPERTY CXX_STANDARD 11)
set_property(TARGET mathmlview PROPERTY CXX_STANDARD_REQUIRED ON)

#copy license file to res directory
file(COPY ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/res)

# Widgets finds its own dependencies.
find_package(Qt5 COMPONENTS Widgets Xml PrintSupport Gui Core Multimedia)

include_directories(casKernel)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(structural)
include_directories(view)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${ANTLR4_INCLUDE_DIRS})
include_directories(${ANTLR_egcGrammar_OUTPUT_DIR})


set(MAXIMA_BINARY_PATH ${CMAKE_BINARY_DIR}/${MAXIMA_INST_PATH}/bin/)
add_definitions(-DMAXIMA_BINARY_PATH="${MAXIMA_BINARY_PATH}")

#this is for showing up abstract interfaces and other sources in the IDE
add_custom_target(egcas_other_srcs ALL SOURCES
        structural/document/egcabstractdocument.h
        structural/entities/egcabstractformulaentity.h
        structural/entities/egcabstracttextentity.h
        structural/entities/egcabstractpixmapentity.h
        structural/entities/egcabstractentitylist.h
        structural/entities/egcabstractentity.h
        structural/actions/egcoperations.h
        structural/actions/egcaction.h
        view/egcabstractformulaitem.h
        view/egcabstracttextitem.h
        view/egcabstractpixmapitem.h
        menu/mathelement.h
        structural/abstractserializer.h

)

set(egcas_concrete_node_SOURCES
        structural/concreteNodes/egcnumbernode.cpp
        structural/concreteNodes/egcparenthesisnode.cpp
        structural/concreteNodes/egclparenthesisnode.cpp
        structural/concreteNodes/egcrparenthesisnode.cpp
        structural/concreteNodes/egcrootnode.cpp
        structural/concreteNodes/egcvariablenode.cpp
        structural/concreteNodes/egcfunctionnode.cpp
        structural/concreteNodes/egcunaryminusnode.cpp
        structural/concreteNodes/egclognode.cpp
        structural/concreteNodes/egcnatlognode.cpp
        structural/concreteNodes/egcplusnode.cpp
        structural/concreteNodes/egcminusnode.cpp
        structural/concreteNodes/egcexponentnode.cpp
        structural/concreteNodes/egcdivisionnode.cpp
        structural/concreteNodes/egcmultiplicationnode.cpp
        structural/concreteNodes/egcdefinitionnode.cpp
        structural/concreteNodes/egcequalnode.cpp
        structural/concreteNodes/egcintegralnode.cpp
        structural/concreteNodes/egcdifferentialnode.cpp
        structural/concreteNodes/egcalnumnode.cpp
        structural/concreteNodes/egcbinemptynode.cpp
        structural/concreteNodes/egcfnccontainernode.cpp
        structural/concreteNodes/egcconstantnode.cpp
        structural/concreteNodes/egcmatrixnode.cpp
)

set(egcas_SOURCES
        main.cpp
        menu/mainwindow.cpp
        view/egcasscene.cpp 
        view/egcasiteminterface.cpp 
        view/egctextitem.cpp 
        view/egcpixmapitem.cpp 
        view/resizehandle.cpp 
        view/egcformulaitem.cpp 
        view/egcabstractitem.cpp
        view/egcscreenpos.cpp
        structural/specialNodes/egcnode.cpp
        structural/specialNodes/egcbinarynode.cpp
        structural/specialNodes/egcunarynode.cpp
        structural/specialNodes/egcflexnode.cpp
        structural/egcnodecreator.cpp
        structural/visitor/egcnodevisitor.cpp 
        structural/visitor/egcmathmllookup.cpp
        structural/visitor/formulascrvisitor.cpp
        structural/visitor/formulascrelement.cpp
        structural/visitor/visitorhelper.cpp
        structural/specialNodes/egccontainernode.cpp 
        structural/entities/egcformulaentity.cpp
        structural/iterator/egcnodeiterator.cpp
        structural/iterator/formulascriter.cpp
        structural/specialNodes/egcbasenode.cpp
        structural/specialNodes/egcemptynode.cpp
        structural/visitor/egcmaximavisitor.cpp 
        ${egcas_concrete_node_SOURCES}
        structural/visitor/egcmathmlvisitor.cpp 
        casKernel/egcmaximaconn.cpp 
        casKernel/egckernelconn.cpp
        structural/entities/egcentitylist.cpp
        structural/entities/egcentity.cpp
        structural/entities/egctextentity.cpp
        structural/entities/egcpixmapentity.cpp
        structural/document/egcdocument.cpp
        structural/entities/formulamodificator.cpp
        formulagenerator.cpp
        menu/egclicenseinfo.cpp
        structural/document/egccalculation.cpp
        structural/specialNodes/egcargumentsnode.cpp
        structural/specialNodes/egcbinaryoperator.cpp
        utils/egcutfcodepoint.cpp
        view/egccrossitem.cpp
        structural/actions/egcactionmapper.cpp
        view/egcworksheet.cpp
        view/grid.cpp
        menu/egcgraphicsview.cpp
        menu/collapsablesectionwidget.cpp
        menu/elementbar.cpp
        menu/mathsection.cpp
        menu/actionwrapper.cpp
        menu/precisionbox.cpp
        menu/resulttype.cpp
        menu/mathfont.cpp
        menu/textfont.cpp
        menu/richtexteditor.cpp
)

qt5_wrap_ui(EGCAS_UIS
            menu/mainwindow.ui
            menu/egclicenseinfo.ui
)
qt5_add_resources(EGCAS_RCCS res/ressources.qrc)

#set the verbosity level of the scanner and parser
add_definitions(-DEGC_SCANNER_DEBUG=0)
add_definitions(-DEGC_PARSER_DEBUG=0)

set(ALWAYS_BUILD ${CMAKE_CURRENT_BINARY_DIR}/always_build.txt)
add_custom_command(OUTPUT ${ALWAYS_BUILD} COMMAND cmake -E touch ${ALWAYS_BUILD})

set (PYTHON_HELPER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/../tools/)

set(EGC_GEN_NODE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/structural/specialNodes/egcnode_gen.h.in)
set(EGC_GEN_NODE_HEADER_GENERATED ${CMAKE_CURRENT_SOURCE_DIR}/structural/specialNodes/egcnode_gen.h)
set(EGC_GEN_NODES ${CMAKE_CURRENT_SOURCE_DIR}/structural/egcnodes.h.in)
set(EGC_GEN_NODES_GENERATED ${CMAKE_CURRENT_SOURCE_DIR}/structural/egcnodes.h)
if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
        set(EGC_PYTHON_INTERPRETER python3)
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        set(EGC_PYTHON_INTERPRETER py)
endif()

add_custom_command(OUTPUT ${EGC_GEN_NODE_HEADER_GENERATED} ${EGC_GEN_NODES_GENERATED}
        COMMAND ${EGC_PYTHON_INTERPRETER} -m cogapp -d -D BaseDirToSearch=${CMAKE_CURRENT_SOURCE_DIR} -D PythonHelper=${PYTHON_HELPER_SCRIPT}
        -o ${EGC_GEN_NODE_HEADER_GENERATED} ${EGC_GEN_NODE_HEADER}
        COMMAND ${EGC_PYTHON_INTERPRETER} -m cogapp -d -D BaseDirToSearch=${CMAKE_CURRENT_SOURCE_DIR}  -D PythonHelper=${PYTHON_HELPER_SCRIPT}
        -o ${EGC_GEN_NODES_GENERATED} ${EGC_GEN_NODES}
        DEPENDS ${EGC_GEN_NODES} ${EGC_GEN_NODE_HEADER} ${PYTHON_HELPER_SCRIPT}
        COMMENT "generate node list and node header files"
        VERBATIM
)
add_custom_target(generate_egc_node_header
        DEPENDS ${EGC_GEN_NODE_HEADER_GENERATED} ${EGC_GEN_NODES_GENERATED}
)


set(EGC_GEN_NODE_CREATOR ${CMAKE_CURRENT_SOURCE_DIR}/structural/egcnodecreator.cpp.in)
set(EGC_GEN_NODE_CREATOR_GENERATED ${CMAKE_CURRENT_SOURCE_DIR}/structural/egcnodecreator.cpp)
add_custom_command(OUTPUT ${EGC_GEN_NODE_CREATOR_GENERATED}
        COMMAND ${EGC_PYTHON_INTERPRETER} -m cogapp -d -D BaseDirToSearch=${CMAKE_CURRENT_SOURCE_DIR} -D PythonHelper=${PYTHON_HELPER_SCRIPT}
        -o ${EGC_GEN_NODE_CREATOR_GENERATED} ${EGC_GEN_NODE_CREATOR}
        DEPENDS ${EGC_GEN_NODE_CREATOR} ${PYTHON_HELPER_SCRIPT} ${ALWAYS_BUILD}
        COMMENT "generate node creator"
        VERBATIM
)

set(WIN_RESOURCE_FILE res/egcas.rc)

add_executable(egcas WIN32 ${egcas_SOURCES} ${EGCAS_RCCS} ${EGCAS_UIS}
                     ${EGC_GEN_NODE_HEADER_GENERATED}
                     ${EGC_GEN_NODE_CREATOR_GENERATED}
                     ${EGC_GEN_NODES_GENERATED}
                     ${WIN_RESOURCE_FILE})
add_dependencies(egcas mmlegcas generate_egc_node_header)

include(DeployQt)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        #execute windeployqt after building egcas
        run_windeployqt(egcas)

        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
                if(${CMAKE_FIND_ROOT_PATH} MATCHES "/mxe/") # mxe environment
                        set(RUNTIME_PATH ${CMAKE_FIND_ROOT_PATH}/../../../runtime/)
                        if(NOT "${CMAKE_SIZEOF_VOID_P}" EQUAL "8") # 32 bit platform
                                set( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
                                        ${RUNTIME_PATH}/libbz2.dll
                                        ${RUNTIME_PATH}/libgcc_s_sjlj-1.dll
                                        ${RUNTIME_PATH}/libharfbuzz-0.dll
                                        ${RUNTIME_PATH}/libiconv-2.dll
                                        ${RUNTIME_PATH}/libpcre16-0.dll
                                        ${RUNTIME_PATH}/libpng16-16.dll
                                        ${RUNTIME_PATH}/Qt5Gui.dll
                                        ${RUNTIME_PATH}/Qt5Multimedia.dll
                                        ${RUNTIME_PATH}/Qt5Network.dll
                                        ${RUNTIME_PATH}/Qt5Test.dll
                                        ${RUNTIME_PATH}/Qt5Xml.dll
                                        ${RUNTIME_PATH}/libfreetype-6.dll
                                        ${RUNTIME_PATH}/libglib-2.0-0.dll
                                        ${RUNTIME_PATH}/libharfbuzz-icu-0.dll
                                        ${RUNTIME_PATH}/libintl-8.dll
                                        ${RUNTIME_PATH}/libpcre-1.dll
                                        ${RUNTIME_PATH}/libstdc++-6.dll
                                        ${RUNTIME_PATH}/Qt5Core.dll
                                        ${RUNTIME_PATH}/Qt5Svg.dll
                                        ${RUNTIME_PATH}/Qt5Widgets.dll
                                        ${RUNTIME_PATH}/zlib1.dll
                                        ${RUNTIME_PATH}/libpcre2-16-0.dll
                                        ${RUNTIME_PATH}/libcrypto-1_1.dll
                                        ${RUNTIME_PATH}/libssl-1_1.dll
                                )
                                install(FILES ${RUNTIME_PATH}/platforms/qminimal.dll ${RUNTIME_PATH}/platforms/qoffscreen.dll ${RUNTIME_PATH}/platforms/qwindows.dll DESTINATION bin/platforms)
                        endif()
                endif()
        endif()
        if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
                add_qt_lib_for_installation(Widgets)
                add_qt_lib_for_installation(Core)
                add_qt_lib_for_installation(Xml)
                add_qt_lib_for_installation(PrintSupport)
                add_qt_lib_for_installation(Gui)
                add_qt_lib_for_installation(Multimedia)
                add_qt_lib_for_installation(Network)
                if (CMAKE_CXX_COMPILER_ID STREQUAL "MinGW")
                        add_lib_for_installation(libgcc_s_dw2-1.dll)
                        add_lib_for_installation(libstdc++-6.dll)
                        add_lib_for_installation(libwinpthread-1.dll)
                endif()
        endif()
        INCLUDE(InstallRequiredSystemLibraries)
        INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)
        INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/platforms/qwindows.dll" DESTINATION bin/platforms)
endif()

#only for cmake >= 3.1
set_property(TARGET egcas PROPERTY CXX_STANDARD 11)
set_property(TARGET egcas PROPERTY CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MinGW")
        set_target_properties(egcas PROPERTIES LINK_FLAGS "-mwindows")
endif()

target_link_libraries(egcas mmlegcas mrichtextedit Qt5::Xml Qt5::Widgets Qt5::PrintSupport Qt5::Gui Qt5::Core Qt5::Multimedia egcas_parser)
install (TARGETS egcas RUNTIME DESTINATION bin)
