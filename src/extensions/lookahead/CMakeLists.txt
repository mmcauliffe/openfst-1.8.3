add_library(fstlookahead
  arc_lookahead-fst.cc 
  ilabel_lookahead-fst.cc
  olabel_lookahead-fst.cc
)
target_link_libraries(fstlookahead fst)
set_target_properties(fstlookahead PROPERTIES 
  SOVERSION "${SOVERSION}"
)

install(TARGETS fstlookahead 
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
)
  
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
  
add_module(arc_lookahead-fst MODULE
  arc_lookahead-fst.cc)
 
add_module(ilabel_lookahead-fst MODULE
  ilabel_lookahead-fst.cc)

add_module(olabel_lookahead-fst MODULE
  olabel_lookahead-fst.cc)