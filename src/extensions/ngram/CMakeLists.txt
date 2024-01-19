
add_library(fstngram
    bitmap-index.cc 
    ngram-fst.cc 
    nthbit.cc
)

target_link_libraries(fstngram
    fst
)
GENERATE_EXPORT_HEADER( fstngram
             BASE_NAME fstngram
             EXPORT_MACRO_NAME fstngram_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fstngram_Export.h
             STATIC_DEFINE fstngram_BUILT_AS_STATIC
)
target_compile_definitions(fstngram PRIVATE fstngram_EXPORTS)
target_include_directories(fstngram PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

set_target_properties(fstngram PROPERTIES
  SOVERSION "${SOVERSION}"
)

install(TARGETS fstngram 
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
)

add_library(ngram_fst MODULE
    bitmap-index.cc 
    ngram-fst.cc 
    nthbit.cc
)

target_link_libraries(ngram_fst
    fst
)
target_compile_definitions(ngram_fst PRIVATE fstngram_EXPORTS)
target_include_directories(ngram_fst PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

if (MSVC)
install(TARGETS ${_name} 
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION bin
        RUNTIME DESTINATION bin)
else()
install(TARGETS ${_name} 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
endif()