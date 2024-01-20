
    add_library(fstscript
      arciterator-class.cc
      stateiterator-class.cc
      weight-class.cc
      draw.cc
      getters.cc
      print.cc
      text-io.cc
      arcsort.cc
      closure.cc
      fst-class.cc
      info.cc
      info-impl.cc
      verify.cc
      map.cc
      compile.cc
      compose.cc
      concat.cc
      connect.cc
      convert.cc
      randequivalent.cc
      randgen.cc
      topsort.cc
      union.cc
      equal.cc
      equivalent.cc
      intersect.cc
      invert.cc
      encode.cc
      decode.cc
      encodemapper-class.cc
      determinize.cc
      difference.cc
      disambiguate.cc
      epsnormalize.cc
      isomorphic.cc
      minimize.cc
      project.cc
      prune.cc
      push.cc
      relabel.cc
      replace.cc
      reverse.cc
      reweight.cc
      rmepsilon.cc
      shortest-distance.cc
      shortest-path.cc
      synchronize.cc
    )

if (MSVC) # Fix for too many exported symbols in fstscript
  
GENERATE_EXPORT_HEADER( fstscript
             BASE_NAME fstscript
             EXPORT_MACRO_NAME fstscript_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/exports/fstscript_Export.h
             STATIC_DEFINE fstscript_BUILT_AS_STATIC
)
target_include_directories(fstscript PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_compile_definitions(fstscript PRIVATE fstscript_EXPORTS)
    else()

endif()
    target_link_libraries(fstscript PUBLIC fst)
target_include_directories(fstscript PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
set_target_properties(fstscript PROPERTIES
  SOVERSION "${SOVERSION}"
)
install(TARGETS fstscript
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin)