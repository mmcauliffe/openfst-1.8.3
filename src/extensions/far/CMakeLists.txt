
add_library(fstfar
  sttable.cc
  stlist.cc
)
target_link_libraries(fstfar fst)
set_target_properties(fstfar PROPERTIES 
  SOVERSION "${SOVERSION}"
  FOLDER far
)
GENERATE_EXPORT_HEADER( fstfar
             BASE_NAME fstfar
             EXPORT_MACRO_NAME fstfar_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fstfar_Export.h
             STATIC_DEFINE fstfar_BUILT_AS_STATIC
)
target_compile_definitions(fstfar PRIVATE fstfar_EXPORTS)
target_include_directories(fstfar PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

install(TARGETS fstfar
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
)

if(HAVE_SCRIPT)
  add_library(fstfarscript
    compile-strings.cc 
    far-class.cc 
    farscript.cc
    getters.cc 
    script-impl.cc
  )
  if (MSVC)
GENERATE_EXPORT_HEADER( fstfarscript
             BASE_NAME fstfarscript
             EXPORT_MACRO_NAME fstfarscript_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fstfarscript_Export.h
             STATIC_DEFINE fstfarscript_BUILT_AS_STATIC
)
target_compile_definitions(fstfarscript PRIVATE fstfarscript_EXPORTS)
target_include_directories(fstfarscript PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
  endif()
    target_link_libraries(fstfarscript fstfar fstscript fst ${CMAKE_DL_LIBS})
  set_target_properties(fstfarscript PROPERTIES 
    SOVERSION "${SOVERSION}"
  )

  install(TARGETS fstfarscript
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  )
endif(HAVE_SCRIPT)

if(HAVE_BIN)
  function (add_executable2 _name)
      add_executable(${ARGV})
      if (TARGET ${_name})
        target_link_libraries(${_name} fstfarscript fstscript fst ${CMAKE_DL_LIBS})
      endif()
      install(TARGETS ${_name} RUNTIME DESTINATION bin)
  endfunction()

  add_executable2(farcompilestrings farcompilestrings.cc farcompilestrings-main.cc)
  add_executable2(farcreate  farcreate.cc farcreate-main.cc)
  add_executable2(farconvert  farconvert.cc farconvert-main.cc)
  add_executable2(farencode  farencode.cc farencode-main.cc)
  add_executable2(farequal  farequal.cc farequal-main.cc)
  add_executable2(farextract  farextract.cc farextract-main.cc)
  add_executable2(farinfo  farinfo.cc farinfo-main.cc)
  add_executable2(farisomorphic  farisomorphic.cc  farisomorphic-main.cc)
  add_executable2(farprintstrings  farprintstrings.cc farprintstrings-main.cc)
endif(HAVE_BIN)

