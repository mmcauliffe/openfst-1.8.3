

if(HAVE_SCRIPT)
  add_library(fstlinearscript
    linearscript.cc
  )
  set_target_properties(fstlinearscript PROPERTIES 
    SOVERSION "${SOVERSION}"
  )
    target_link_libraries(fstlinearscript
      fstscript
      fst
    )
  if (MSVC)
GENERATE_EXPORT_HEADER( fstlinearscript
             BASE_NAME fstlinearscript
             EXPORT_MACRO_NAME fstlinearscript_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fstlinearscript_Export.h
             STATIC_DEFINE fstlinearscript_BUILT_AS_STATIC
)
target_compile_definitions(fstlinearscript PRIVATE fstlinearscript_EXPORTS)
target_include_directories(fstlinearscript PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
    install(TARGETS fstlinearscript
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    )
  else()
    install(TARGETS fstlinearscript
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    )
  endif()
  
endif(HAVE_SCRIPT)

if(HAVE_BIN)
  add_executable(fstlinear
    fstlinear.cc 
    fstlinear-main.cc)
    target_link_libraries(fstlinear
      fstlinearscript
      fstscript 
      fst
      ${CMAKE_DL_LIBS}
    )

  add_executable(fstloglinearapply
    fstloglinearapply.cc 
    fstloglinearapply-main.cc)
  target_link_libraries(fstloglinearapply
    fstlinearscript
    fstscript 
    fst
    ${CMAKE_DL_LIBS}
  )
  install(TARGETS fstlinear fstloglinearapply
    RUNTIME DESTINATION bin
  )
endif(HAVE_BIN)


function (add_module _name)
    add_library(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} fst)
    endif()

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
endfunction()

add_module(linear-tagger-fst MODULE
  linear-tagger-fst.cc)

add_module(linear-classifier-fst MODULE
  linear-classifier-fst.cc)
