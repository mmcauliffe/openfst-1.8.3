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


add_module(const8-fst MODULE const8-fst.cc)

add_module(const16-fst MODULE const16-fst.cc)

add_module(const64-fst MODULE const64-fst.cc)

add_library(fstconst 
  const8-fst.cc 
  const16-fst.cc 
  const64-fst.cc)
target_link_libraries(fstconst fst)
set_target_properties(fstconst PROPERTIES
  SOVERSION "${SOVERSION}"
)
GENERATE_EXPORT_HEADER( fstconst
             BASE_NAME fstconst
             EXPORT_MACRO_NAME fstconst_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fstconst_Export.h
             STATIC_DEFINE fstconst_BUILT_AS_STATIC
)
target_compile_definitions(fstconst PRIVATE fstconst_EXPORTS)
target_include_directories(fstconst PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

install(TARGETS fstconst 
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
 )