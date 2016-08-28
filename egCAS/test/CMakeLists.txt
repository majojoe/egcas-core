 
cmake_minimum_required(VERSION 2.8.11)

add_subdirectory(egcasTestView)
add_subdirectory(egcasTestStructure)
add_subdirectory(egcasTestParser)
add_subdirectory(egcasTestCalculations)
add_subdirectory(egcasTestScreenIterators)
add_subdirectory(egcasTestUtf)
add_subdirectory(egcasTestAdvancedTreeOps)
add_subdirectory(egcasPrintOpPrec)
add_subdirectory(egcasRearrangePrec)


#add here all tests to execute
set(tests-to-execute
        tst_print_op_precedence 
        tst_egcas_advanced_tree_ops 
        tst_egcastest_calculations 
        tst_egcastest_parser 
        tst_egcastest_scriters 
        tst_egcastest_structural 
        tst_egcastest_utf_encoding 
        tst_egcastest_view
)

add_custom_target(egcas-tests-all COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test-executer.sh "${CMAKE_BINARY_DIR}/bin" ${tests-to-execute}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "execute all tests")
add_dependencies(egcas-tests-all ${tests-to-execute} )
