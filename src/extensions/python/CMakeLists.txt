
add_library(pywrapfst
    pywrapfst.cpp
)

target_link_libraries(pywrapfst
    fst
    fstfarscript
    fstfar
    fstscript
)

set_target_properties(pywrapfst PROPERTIES
  SOVERSION "${SOVERSION}"
  FOLDER python
)

install(TARGETS pywrapfst 
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION lib
)