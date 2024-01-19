
if(HAVE_SCRIPT)
  add_library(fstmpdtscript mpdtscript.cc)
    target_link_libraries(fstmpdtscript fstscript fst)
  if (MSVC)
GENERATE_EXPORT_HEADER( fstmpdtscript
             BASE_NAME fstmpdtscript
             EXPORT_MACRO_NAME fstmpdtscript_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fstmpdtscript_Export.h
             STATIC_DEFINE fstmpdtscript_BUILT_AS_STATIC
)
target_compile_definitions(fstmpdtscript PUBLIC fstmpdtscript_EXPORTS)
target_include_directories(fstmpdtscript PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
  endif()
  set_target_properties(fstmpdtscript PROPERTIES 
    SOVERSION "${SOVERSION}"
  )
  install(TARGETS fstmpdtscript 
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  )
endif(HAVE_SCRIPT)

if(HAVE_BIN)
  function (add_executable2 _name)
      add_executable(${ARGV})
      if (TARGET ${_name})
          target_link_libraries(${_name} fstmpdtscript fstpdtscript fstscript fst ${CMAKE_DL_LIBS})
      endif()
    install(TARGETS ${_name} RUNTIME DESTINATION bin)
  endfunction()
  add_executable2(mpdtcompose  mpdtcompose.cc mpdtcompose-main.cc)
  add_executable2(mpdtexpand  mpdtexpand.cc mpdtexpand-main.cc)
  add_executable2(mpdtinfo  mpdtinfo.cc mpdtinfo-main.cc)
  add_executable2(mpdtreverse  mpdtreverse.cc mpdtreverse-main.cc)
endif(HAVE_BIN)