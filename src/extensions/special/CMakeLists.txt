

add_library(fstspecial
  phi-fst.cc
  rho-fst.cc
  sigma-fst.cc
)
GENERATE_EXPORT_HEADER( fstspecial
             BASE_NAME fstspecial
             EXPORT_MACRO_NAME fstspecial_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fstspecial_Export.h
             STATIC_DEFINE fstspecial_BUILT_AS_STATIC
)
target_compile_definitions(fstspecial PRIVATE fstspecial_EXPORTS)
target_include_directories(fstspecial PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

set_target_properties(fstspecial PROPERTIES
  SOVERSION "${SOVERSION}"
)
target_link_libraries(fstspecial
  fst
)

if(HAVE_BIN)
  add_executable(fstspecialconvert
    ../../bin/fstconvert.cc 
    ../../bin/fstconvert-main.cc
  )

  set_target_properties(fstspecialconvert PROPERTIES 
    OUTPUT_NAME fstspecialconvert
  )
  target_link_libraries(fstspecialconvert
  fstspecial
    fstscript
    fst
    ${CMAKE_DL_LIBS}
  )
endif(HAVE_BIN)


install(TARGETS fstspecial fstspecialconvert
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
)
