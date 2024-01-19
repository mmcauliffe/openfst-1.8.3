
cmake_minimum_required(VERSION 3.20)
project(openfst)

include(GNUInstallDirs)
include(GenerateExportHeader)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (MSVC)
    find_package(dlfcn-win32 REQUIRED)
    set(CMAKE_DL_LIBS dlfcn-win32::dl)
    set(CMAKE_ENABLE_EXPORTS ON)

    #set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
    add_definitions(-D_USE_MATH_DEFINES)
    add_compile_options(/permissive- /FS /wd4819 /EHsc /bigobj)

    # some warnings related with fst
    add_compile_options(/wd4018 /wd4244 /wd4267 /wd4291 /wd4305)
    
endif (MSVC)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(SOVERSION "26")


option(HAVE_BIN          "Build the fst binaries" ON)
option(HAVE_SCRIPT       "Build the fstscript" ON)
option(HAVE_COMPACT      "Build compact" ON)
option(HAVE_COMPRESS "Build compress" ON)
option(HAVE_CONST   "Build const" ON)
option(HAVE_FAR  "Build far" ON)
option(HAVE_GRM "Build grm" ON)
option(HAVE_PDT "Build pdt" ON)
option(HAVE_MPDT "Build mpdt" ON)
option(HAVE_LINEAR "Build linear" ON)
option(HAVE_LOOKAHEAD "Build lookahead" ON)
option(HAVE_NGRAM "Build ngram" ON)
option(HAVE_PYTHON "Build python" OFF)
option(HAVE_SPECIAL "Build special" ON)
option(OPENFST_BUILD_TEST "Build tests" ON) 

if (OPENFST_BUILD_TEST)
    include(CTest)
    enable_testing()
    set(CTEST_OUTPUT_ON_FAILURE ON)
endif()

add_subdirectory(src)
