

add_library(fst
  SHARED
  compat.cc
  encode.cc
  flags.cc
  fst-types.cc
  fst.cc
  mapped-file.cc
  properties.cc
  symbol-table.cc
  symbol-table-ops.cc
  util.cc
  weight.cc
)
if (MSVC)
GENERATE_EXPORT_HEADER( fst
             BASE_NAME fst
             EXPORT_MACRO_NAME fst_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fst_Export.h
             STATIC_DEFINE fst_BUILT_AS_STATIC
)
endif()
set_target_properties(fst PROPERTIES
  SOVERSION "${SOVERSION}"
)
target_include_directories(fst PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_include_directories(fst PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_link_libraries(fst ${CMAKE_DL_LIBS})
target_compile_definitions(fst PRIVATE fst_EXPORTS)



install(TARGETS fst
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
  )