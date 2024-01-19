
add_executable(fst_test
        fst_test.cc
        ../include/fst/test/compactors.h
        ../include/fst/test/fst_test.h
        )

target_link_libraries(fst_test fst)
if (MSVC)
        set_target_properties(fst_test 
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "TMPDIR=${CMAKE_BINARY_DIR}/tmp"
        )
elseif (APPLE)
        set_target_properties(fst_test 
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "DYLD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
else()
        set_target_properties(fst_test 
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
endif()
add_test(NAME fst_test_test COMMAND fst_test --tmpdir=${CMAKE_BINARY_DIR}/tmp WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/lib)

add_executable(weight_test
        weight_test.cc
        ../include/fst/test/weight-tester.h
        )
target_link_libraries(weight_test fst ${CMAKE_DL_LIBS})
if (MSVC)
        set_target_properties(weight_test 
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "PATH=${test_dll_path}"
        )
elseif (APPLE)
        set_target_properties(weight_test
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "DYLD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
else()
        set_target_properties(weight_test
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
endif()
add_test(NAME weight_test_test COMMAND weight_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/lib)


add_executable(algo_test_log algo_test.cc ../include/fst/test/algo_test.h ../include/fst/test/rand-fst.h)
target_link_libraries(algo_test_log fst ${CMAKE_DL_LIBS})
target_compile_definitions(algo_test_log
        PRIVATE TEST_LOG=1)
if (MSVC)
        set_target_properties(algo_test_log 
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "PATH=${test_dll_path}"
        )
elseif (APPLE)
        set_target_properties(algo_test_log
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "DYLD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
else()
        set_target_properties(algo_test_log
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
endif()
add_test(NAME algo_test_log_test COMMAND algo_test_log WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/lib)


add_executable(algo_test_tropical algo_test.cc ../include/fst/test/algo_test.h ../include/fst/test/rand-fst.h)
target_link_libraries(algo_test_tropical fst ${CMAKE_DL_LIBS})
target_compile_definitions(algo_test_tropical
        PRIVATE TEST_TROPICAL=1)

if (MSVC)
        set_target_properties(algo_test_tropical 
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "PATH=${test_dll_path}"
        )
elseif (APPLE)
        set_target_properties(algo_test_tropical
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "DYLD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
else()
        set_target_properties(algo_test_tropical
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
endif()
add_test(NAME algo_test_tropical_test COMMAND algo_test_tropical WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/lib)


add_executable(algo_test_minmax algo_test.cc ../include/fst/test/algo_test.h ../include/fst/test/rand-fst.h)
target_link_libraries(algo_test_minmax fst ${CMAKE_DL_LIBS})
target_compile_definitions(algo_test_minmax
        PRIVATE TEST_MINMAX=1)
if (MSVC)
        set_target_properties(algo_test_minmax 
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "PATH=${test_dll_path}"
        )
elseif (APPLE)
        set_target_properties(algo_test_minmax
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "DYLD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
else()
        set_target_properties(algo_test_minmax
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
endif()
add_test(NAME algo_test_minmax_test COMMAND algo_test_minmax WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/lib)


add_executable(algo_test_lexicographic algo_test.cc ../include/fst/test/algo_test.h ../include/fst/test/rand-fst.h)
target_link_libraries(algo_test_lexicographic fst ${CMAKE_DL_LIBS})
target_compile_definitions(algo_test_lexicographic
        PRIVATE TEST_LEXICOGRAPHIC=1)
if (MSVC)
        set_target_properties(algo_test_lexicographic 
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "PATH=${test_dll_path}"
        )
elseif (APPLE)
        set_target_properties(algo_test_lexicographic
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "DYLD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
else()
        set_target_properties(algo_test_lexicographic
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
endif()
add_test(NAME algo_test_lexicographic_test COMMAND algo_test_lexicographic WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/lib)


add_executable(algo_test_power algo_test.cc ../include/fst/test/algo_test.h ../include/fst/test/rand-fst.h)
target_link_libraries(algo_test_power fst ${CMAKE_DL_LIBS})
target_compile_definitions(algo_test_power
        PRIVATE TEST_POWER=1)
if (MSVC)
        set_target_properties(algo_test_power 
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "PATH=${test_dll_path}"
        )
elseif (APPLE)
        set_target_properties(algo_test_power
                PROPERTIES  
                        FOLDER test 
                        ENVIRONMENT "DYLD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
else()
        set_target_properties(algo_test_power
                PROPERTIES 
                        FOLDER test 
                        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/lib;${CMAKE_INSTALL_PREFIX}/lib"
        )
endif()
add_test(NAME algo_test_power_test COMMAND algo_test_power WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/lib)