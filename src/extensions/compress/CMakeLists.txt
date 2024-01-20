
add_library(fstcompressscript
  compressscript.cc
 )
if (MSVC)
  target_link_libraries(fstcompressscript
    fstscript
    fst
  )
GENERATE_EXPORT_HEADER( fstcompressscript
             BASE_NAME fstcompressscript
             EXPORT_MACRO_NAME fstcompressscript_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fstcompressscript_Export.h
             STATIC_DEFINE fstcompressscript_BUILT_AS_STATIC
)
target_compile_definitions(fstcompressscript PRIVATE fstcompressscript_EXPORTS)
target_include_directories(fstcompressscript PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
else()
  target_link_libraries(fstcompressscript
    fstscript
    fst
  )
endif()
set_target_properties(fstcompressscript PROPERTIES
  SOVERSION "${SOVERSION}"
)
install(TARGETS fstcompressscript
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
 )

if(HAVE_BIN)
  add_executable(fstcompress
    fstcompress.cc
    fstcompress-main.cc)

    if (MSVC)
      target_link_libraries(fstcompress
        fstcompressscript
        fstscript
        fst
      )
    else()
      target_link_libraries(fstcompress
        fstcompressscript
        fstscript
        fst
        ${CMAKE_DL_LIBS}
      )
    endif()

  install(TARGETS fstcompress
	        LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
		)
endif(HAVE_BIN)