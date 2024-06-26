# CMakeLists.txt

# Copyright (C) 2018 Cosmin Truta
# Copyright (C) 2007,2009-2018 Glenn Randers-Pehrson
# Written by Christian Ehrlicher, 2007
# Revised by Roger Lowman, 2009-2010
# Revised by Clifford Yapp, 2011-2012,2017
# Revised by Roger Leigh, 2016
# Revised by Andreas Franek, 2016
# Revised by Sam Serrels, 2017
# Revised by Vadim Barkov, 2017
# Revised by Vicky Pfau, 2018
# Revised by Cameron Cawley, 2018
# Revised by Cosmin Truta, 2018
# Revised by Kyle Bentley, 2018

# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h

cmake_minimum_required(VERSION 3.6)
cmake_policy(VERSION 3.6)

project(libpng C ASM)

# Define the relative position of this file in the SDL App project
set(CMAKE_CURRENT_DEPTH "${PROJECT_SOURCE_DIR}/../../..")

if(NOT DEFINED PROJECT_DIR_OFFSET)
	set(PROJECT_DIR_OFFSET "../..")
endif()

if(NOT DEFINED SDL2_IMAGE_OFFSET)
	set(SDL2_IMAGE_OFFSET "/components/SDL_image")
endif()

if(NOT DEFINED PNG_DIR)
	set(PNG_DIR "${CMAKE_CURRENT_DEPTH}/${PROJECT_DIR_OFFSET}${SDL2_IMAGE_OFFSET}/external/libpng")
endif()

# Resume the normal SDL CMake project

enable_testing()

set(PNGLIB_MAJOR 1)
set(PNGLIB_MINOR 6)
set(PNGLIB_RELEASE 37)
set(PNGLIB_NAME libpng${PNGLIB_MAJOR}${PNGLIB_MINOR})
set(PNGLIB_VERSION ${PNGLIB_MAJOR}.${PNGLIB_MINOR}.${PNGLIB_RELEASE})

include(GNUInstallDirs)

# needed packages

# Allow users to specify location of Zlib.
# Useful if zlib is being built alongside this as a sub-project.
option(PNG_BUILD_ZLIB "Custom zlib Location, else find_package is used" OFF)

if(NOT PNG_BUILD_ZLIB)
  find_package(ZLIB REQUIRED)
  include_directories(${ZLIB_INCLUDE_DIR})
endif()

if(UNIX AND NOT APPLE AND NOT BEOS AND NOT HAIKU)
  find_library(M_LIBRARY m)
else()
  # libm is not needed and/or not available
  set(M_LIBRARY "")
endif()

# COMMAND LINE OPTIONS
option(PNG_SHARED "Build shared lib" ON)
option(PNG_STATIC "Build static lib" ON)

# Many more configuration options could be added here
option(PNG_FRAMEWORK "Build OS X framework" OFF)
option(PNG_DEBUG "Build with debug output" OFF)
option(PNG_HARDWARE_OPTIMIZATIONS "Enable hardware optimizations" ON)

set(PNG_PREFIX "" CACHE STRING "Prefix to add to the API function names")
set(DFA_XTRA "" CACHE FILEPATH "File containing extra configuration settings")

if(PNG_HARDWARE_OPTIMIZATIONS)

# set definitions and sources for arm
# Fixes on macOS
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOS TRUE)
endif()

if(MACOS)
	add_compile_options(-DHAVE_UNISTD_H -Wno-deprecated-non-prototype)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm" OR
  CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
  set(PNG_ARM_NEON_POSSIBLE_VALUES check on off)
  if (MACOS)
	  set(PNG_ARM_NEON "off" CACHE STRING "Enable ARM NEON optimizations:
		 check: (default) use internal checking code;
		 off: disable the optimizations;
		 on: turn on unconditionally.")
  else()
	  set(PNG_ARM_NEON "check" CACHE STRING "Enable ARM NEON optimizations:
		 check: (default) use internal checking code;
		 off: disable the optimizations;
		 on: turn on unconditionally.")
  endif()
  set_property(CACHE PNG_ARM_NEON PROPERTY STRINGS
     ${PNG_ARM_NEON_POSSIBLE_VALUES})
  list(FIND PNG_ARM_NEON_POSSIBLE_VALUES ${PNG_ARM_NEON} index)
  if(index EQUAL -1)
    message(FATAL_ERROR
            "PNG_ARM_NEON must be one of [${PNG_ARM_NEON_POSSIBLE_VALUES}]")
  elseif(NOT ${PNG_ARM_NEON} STREQUAL "off")
    set(libpng_arm_sources
      ${PNG_DIR}/arm/arm_init.c
      ${PNG_DIR}/arm/filter_neon.S
      ${PNG_DIR}/arm/filter_neon_intrinsics.c
      ${PNG_DIR}/arm/palette_neon_intrinsics.c)

    if(${PNG_ARM_NEON} STREQUAL "on")
      add_definitions(-DPNG_ARM_NEON_OPT=2)
    elseif(${PNG_ARM_NEON} STREQUAL "check")
      add_definitions(-DPNG_ARM_NEON_CHECK_SUPPORTED)
    endif()
  else()
    add_definitions(-DPNG_ARM_NEON_OPT=0)
  endif()
endif()

# set definitions and sources for powerpc
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^powerpc*" OR
   CMAKE_SYSTEM_PROCESSOR MATCHES "^ppc64*")
  set(PNG_POWERPC_VSX_POSSIBLE_VALUES on off)
  set(PNG_POWERPC_VSX "on" CACHE STRING "Enable POWERPC VSX optimizations:
     off: disable the optimizations.")
  set_property(CACHE PNG_POWERPC_VSX PROPERTY STRINGS
     ${PNG_POWERPC_VSX_POSSIBLE_VALUES})
  list(FIND PNG_POWERPC_VSX_POSSIBLE_VALUES ${PNG_POWERPC_VSX} index)
  if(index EQUAL -1)
    message(FATAL_ERROR
            "PNG_POWERPC_VSX must be one of [${PNG_POWERPC_VSX_POSSIBLE_VALUES}]")
  elseif(NOT ${PNG_POWERPC_VSX} STREQUAL "off")
    set(libpng_powerpc_sources
      ${PNG_DIR}/powerpc/powerpc_init.c
      ${PNG_DIR}/powerpc/filter_vsx_intrinsics.c)
    if(${PNG_POWERPC_VSX} STREQUAL "on")
      add_definitions(-DPNG_POWERPC_VSX_OPT=2)
    endif()
  else()
    add_definitions(-DPNG_POWERPC_VSX_OPT=0)
  endif()
endif()

# set definitions and sources for intel
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^i?86" OR
   CMAKE_SYSTEM_PROCESSOR MATCHES "^x86_64*")
  set(PNG_INTEL_SSE_POSSIBLE_VALUES on off)
  set(PNG_INTEL_SSE "on" CACHE STRING "Enable INTEL_SSE optimizations:
     off: disable the optimizations")
  set_property(CACHE PNG_INTEL_SSE PROPERTY STRINGS
     ${PNG_INTEL_SSE_POSSIBLE_VALUES})
  list(FIND PNG_INTEL_SSE_POSSIBLE_VALUES ${PNG_INTEL_SSE} index)
  if(index EQUAL -1)
    message(FATAL_ERROR
            "PNG_INTEL_SSE must be one of [${PNG_INTEL_SSE_POSSIBLE_VALUES}]")
  elseif(NOT ${PNG_INTEL_SSE} STREQUAL "off")
    set(libpng_intel_sources
      ${PNG_DIR}/intel/intel_init.c
      ${PNG_DIR}/intel/filter_sse2_intrinsics.c)
    if(${PNG_INTEL_SSE} STREQUAL "on")
      add_definitions(-DPNG_INTEL_SSE_OPT=1)
    endif()
  else()
    add_definitions(-DPNG_INTEL_SSE_OPT=0)
  endif()
endif()

# set definitions and sources for MIPS
if(CMAKE_SYSTEM_PROCESSOR MATCHES "mipsel*" OR
   CMAKE_SYSTEM_PROCESSOR MATCHES "mips64el*")
  set(PNG_MIPS_MSA_POSSIBLE_VALUES on off)
  set(PNG_MIPS_MSA "on" CACHE STRING "Enable MIPS_MSA optimizations:
     off: disable the optimizations")
  set_property(CACHE PNG_MIPS_MSA PROPERTY STRINGS
     ${PNG_MIPS_MSA_POSSIBLE_VALUES})
  list(FIND PNG_MIPS_MSA_POSSIBLE_VALUES ${PNG_MIPS_MSA} index)
  if(index EQUAL -1)
    message(FATAL_ERROR
            "PNG_MIPS_MSA must be one of [${PNG_MIPS_MSA_POSSIBLE_VALUES}]")
  elseif(NOT ${PNG_MIPS_MSA} STREQUAL "off")
    set(libpng_mips_sources
      ${PNG_DIR}/mips/mips_init.c
      ${PNG_DIR}/mips/filter_msa_intrinsics.c)
    if(${PNG_MIPS_MSA} STREQUAL "on")
      add_definitions(-DPNG_MIPS_MSA_OPT=2)
    endif()
  else()
    add_definitions(-DPNG_MIPS_MSA_OPT=0)
  endif()
endif()

else(PNG_HARDWARE_OPTIMIZATIONS)

# set definitions and sources for arm
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm" OR
   CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
  add_definitions(-DPNG_ARM_NEON_OPT=0)
endif()

# set definitions and sources for powerpc
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^powerpc*" OR
   CMAKE_SYSTEM_PROCESSOR MATCHES "^ppc64*")
  add_definitions(-DPNG_POWERPC_VSX_OPT=0)
endif()

# set definitions and sources for intel
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^i?86" OR
   CMAKE_SYSTEM_PROCESSOR MATCHES "^x86_64*")
  add_definitions(-DPNG_INTEL_SSE_OPT=0)
endif()

# set definitions and sources for MIPS
if(CMAKE_SYSTEM_PROCESSOR MATCHES "mipsel*" OR
   CMAKE_SYSTEM_PROCESSOR MATCHES "mips64el*")
  add_definitions(-DPNG_MIPS_MSA_OPT=0)
endif()

endif(PNG_HARDWARE_OPTIMIZATIONS)

# SET LIBNAME
set(PNG_LIB_NAME png${PNGLIB_MAJOR}${PNGLIB_MINOR})

# to distinguish between debug and release lib
set(CMAKE_DEBUG_POSTFIX "d")

include(CheckCSourceCompiles)
option(ld-version-script "Enable linker version script" ON)
if(ld-version-script AND NOT APPLE)
  # Check if LD supports linker scripts.
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/conftest.map" "VERS_1 {
        global: sym;
        local: *;
};

VERS_2 {
        global: sym2;
                main;
} VERS_1;
")
  set(CMAKE_REQUIRED_FLAGS_SAVE ${CMAKE_REQUIRED_FLAGS})
  set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} "-Wl,--version-script='${CMAKE_CURRENT_BINARY_DIR}/conftest.map'")
  check_c_source_compiles("void sym(void) {}
void sym2(void) {}
int main(void) {return 0;}
" HAVE_LD_VERSION_SCRIPT)
  if(NOT HAVE_LD_VERSION_SCRIPT)
    set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_SAVE} "-Wl,-M -Wl,${CMAKE_CURRENT_BINARY_DIR}/conftest.map")
  check_c_source_compiles("void sym(void) {}
void sym2(void) {}
int main(void) {return 0;}
" HAVE_SOLARIS_LD_VERSION_SCRIPT)
  endif()
  set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_SAVE})
  file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/conftest.map")
endif()

# Find symbol prefix.  Likely obsolete and unnecessary with recent
# toolchains (it's not done in many other projects).
function(symbol_prefix)
  set(SYMBOL_PREFIX)

  execute_process(COMMAND "${CMAKE_C_COMPILER}" "-E" "-"
                  INPUT_FILE /dev/null
                  OUTPUT_VARIABLE OUT
                  RESULT_VARIABLE STATUS)

  if(CPP_FAIL)
    message(WARNING "Failed to run the C preprocessor")
  endif()

  string(REPLACE "\n" ";" OUT "${OUT}")
  foreach(line ${OUT})
    string(REGEX MATCH "^PREFIX=" found_match "${line}")
    if(found_match)
      string(REGEX REPLACE "^PREFIX=(.*\)" "\\1" prefix "${line}")
      string(REGEX MATCH "__USER_LABEL_PREFIX__" found_match "${prefix}")
      if(found_match)
        string(REGEX REPLACE "(.*)__USER_LABEL_PREFIX__(.*)" "\\1\\2" prefix "${prefix}")
      endif()
      set(SYMBOL_PREFIX "${prefix}")
    endif()
  endforeach()

  message(STATUS "Symbol prefix: ${SYMBOL_PREFIX}")
  set(SYMBOL_PREFIX "${SYMBOL_PREFIX}" PARENT_SCOPE)
endfunction()

if(UNIX)
  symbol_prefix()
endif()

find_program(AWK NAMES gawk awk)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(NOT AWK OR ANDROID)
  # No awk available to generate sources; use pre-built pnglibconf.h
  configure_file(${PNG_DIR}/scripts/pnglibconf.h.prebuilt
                 ${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h)
  add_custom_target(genfiles) # Dummy
else()
  include(CMakeParseArguments)
  # Generate .chk from .out with awk
  # generate_chk(INPUT inputfile OUTPUT outputfile [DEPENDS dep1 [dep2...]])
  function(generate_chk)
    set(options)
    set(oneValueArgs INPUT OUTPUT)
    set(multiValueArgs DEPENDS)
    cmake_parse_arguments(_GC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(NOT _GC_INPUT)
      message(FATAL_ERROR "generate_chk: Missing INPUT argument")
    endif()
    if(NOT _GC_OUTPUT)
      message(FATAL_ERROR "generate_chk: Missing OUTPUT argument")
    endif()

    add_custom_command(OUTPUT "${_GC_OUTPUT}"
                       COMMAND "${CMAKE_COMMAND}"
                               "-DINPUT=${_GC_INPUT}"
                               "-DOUTPUT=${_GC_OUTPUT}"
                               -P "${CMAKE_CURRENT_BINARY_DIR}/scripts/genchk.cmake"
                       DEPENDS "${_GC_INPUT}" ${_GC_DEPENDS}
                       WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
  endfunction()

  # Generate .out from .c with awk
  # generate_out(INPUT inputfile OUTPUT outputfile [DEPENDS dep1 [dep2...]])
  function(generate_out)
    set(options)
    set(oneValueArgs INPUT OUTPUT)
    set(multiValueArgs DEPENDS)
    cmake_parse_arguments(_GO "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(NOT _GO_INPUT)
      message(FATAL_ERROR "generate_out: Missing INPUT argument")
    endif()
    if(NOT _GO_OUTPUT)
      message(FATAL_ERROR "generate_out: Missing OUTPUT argument")
    endif()

    add_custom_command(OUTPUT "${_GO_OUTPUT}"
                       COMMAND "${CMAKE_COMMAND}"
                               "-DINPUT=${_GO_INPUT}"
                               "-DOUTPUT=${_GO_OUTPUT}"
                               -P "${CMAKE_CURRENT_BINARY_DIR}/scripts/genout.cmake"
                       DEPENDS "${_GO_INPUT}" ${_GO_DEPENDS}
                       WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
  endfunction()

  # Generate specific source file with awk
  # generate_source(OUTPUT outputfile [DEPENDS dep1 [dep2...]])
  function(generate_source)
    set(options)
    set(oneValueArgs OUTPUT)
    set(multiValueArgs DEPENDS)
    cmake_parse_arguments(_GSO "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(NOT _GSO_OUTPUT)
      message(FATAL_ERROR "generate_source: Missing OUTPUT argument")
    endif()

    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${_GSO_OUTPUT}"
                       COMMAND "${CMAKE_COMMAND}"
                               "-DOUTPUT=${_GSO_OUTPUT}"
                               -P "${CMAKE_CURRENT_BINARY_DIR}/scripts/gensrc.cmake"
                       DEPENDS ${_GSO_DEPENDS}
                       WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
  endfunction()

  # Copy file
  function(generate_copy source destination)
    add_custom_command(OUTPUT "${destination}"
                       COMMAND "${CMAKE_COMMAND}" -E remove "${destination}"
                       COMMAND "${CMAKE_COMMAND}" -E copy "${source}"
                                                          "${destination}"
                       DEPENDS "${source}")
  endfunction()

  # Generate scripts/pnglibconf.h
  generate_source(OUTPUT "scripts/pnglibconf.c"
                  DEPENDS "${PNG_DIR}/scripts/pnglibconf.dfa"
                          "${PNG_DIR}/scripts/options.awk"
                          "${PNG_DIR}/pngconf.h")

  # Generate pnglibconf.c
  generate_source(OUTPUT "pnglibconf.c"
                  DEPENDS "${PNG_DIR}/scripts/pnglibconf.dfa"
                          "${PNG_DIR}/scripts/options.awk"
                          "${PNG_DIR}/pngconf.h")

  if(PNG_PREFIX)
    set(PNGLIBCONF_H_EXTRA_DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/scripts/prefix.out"
        "${PNG_DIR}/scripts/macro.lst")
    set(PNGPREFIX_H_EXTRA_DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/scripts/intprefix.out")
  endif()

  generate_out(INPUT "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.c"
               OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.out")

  # Generate pnglibconf.h
  generate_source(OUTPUT "pnglibconf.h"
                  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.out"
                          ${PNGLIBCONF_H_EXTRA_DEPENDS})

  generate_out(INPUT "${PNG_DIR}/scripts/intprefix.c"
               OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/scripts/intprefix.out"
               DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h")

  generate_out(INPUT "${PNG_DIR}/scripts/prefix.c"
               OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/scripts/prefix.out"
               DEPENDS "${PNG_DIR}/png.h"
                       "${PNG_DIR}/pngconf.h"
                       "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.out")

  # Generate pngprefix.h
  generate_source(OUTPUT "pngprefix.h"
                  DEPENDS ${PNGPREFIX_H_EXTRA_DEPENDS})

  generate_out(INPUT "${PNG_DIR}/scripts/sym.c"
               OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/scripts/sym.out"
               DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h")

  generate_out(INPUT "${PNG_DIR}/scripts/symbols.c"
               OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/scripts/symbols.out"
               DEPENDS "${PNG_DIR}/png.h"
                       "${PNG_DIR}/pngconf.h"
                       "${PNG_DIR}/scripts/pnglibconf.h.prebuilt")

  generate_out(INPUT "${PNG_DIR}/scripts/vers.c"
               OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/scripts/vers.out"
               DEPENDS "${PNG_DIR}/png.h"
                       "${PNG_DIR}/pngconf.h"
                       "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h")

  generate_chk(INPUT "${CMAKE_CURRENT_BINARY_DIR}/scripts/symbols.out"
               OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/scripts/symbols.chk"
               DEPENDS "${PNG_DIR}/scripts/checksym.awk"
                       "${PNG_DIR}/scripts/symbols.def")

  add_custom_target(symbol-check DEPENDS
                    "${CMAKE_CURRENT_BINARY_DIR}/scripts/symbols.chk")

  generate_copy("${CMAKE_CURRENT_BINARY_DIR}/scripts/sym.out"
                "${CMAKE_CURRENT_BINARY_DIR}/libpng.sym")
  generate_copy("${CMAKE_CURRENT_BINARY_DIR}/scripts/vers.out"
                "${CMAKE_CURRENT_BINARY_DIR}/libpng.vers")

  add_custom_target(genvers DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/libpng.vers")
  add_custom_target(gensym DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/libpng.sym")

  add_custom_target("genprebuilt"
                    COMMAND "${CMAKE_COMMAND}"
                            "-DOUTPUT=scripts/pnglibconf.h.prebuilt"
                            -P "${CMAKE_CURRENT_BINARY_DIR}/scripts/gensrc.cmake"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

  # A single target handles generation of all generated files.  If
  # they are depended upon separately by multiple targets, this
  # confuses parallel make (it would require a separate top-level
  # target for each file to track the dependencies properly).
  add_custom_target(genfiles DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/libpng.sym"
    "${CMAKE_CURRENT_BINARY_DIR}/libpng.vers"
    "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.c"
    "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h"
    "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.out"
    "${CMAKE_CURRENT_BINARY_DIR}/pngprefix.h"
    "${CMAKE_CURRENT_BINARY_DIR}/scripts/intprefix.out"
    "${CMAKE_CURRENT_BINARY_DIR}/scripts/pnglibconf.c"
    "${CMAKE_CURRENT_BINARY_DIR}/scripts/prefix.out"
    "${CMAKE_CURRENT_BINARY_DIR}/scripts/sym.out"
    "${CMAKE_CURRENT_BINARY_DIR}/scripts/symbols.chk"
    "${CMAKE_CURRENT_BINARY_DIR}/scripts/symbols.out"
    "${CMAKE_CURRENT_BINARY_DIR}/scripts/vers.out")
endif(NOT AWK OR ANDROID)

# OUR SOURCES
set(libpng_public_hdrs
  ${PNG_DIR}/png.h
  ${PNG_DIR}/pngconf.h
  "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h"
)
set(libpng_private_hdrs
  ${PNG_DIR}/pngpriv.h
  ${PNG_DIR}/pngdebug.h
  ${PNG_DIR}/pnginfo.h
  ${PNG_DIR}/pngstruct.h
)
if(AWK AND NOT ANDROID)
  list(APPEND libpng_private_hdrs "${CMAKE_CURRENT_BINARY_DIR}/pngprefix.h")
endif()
set(libpng_sources
  ${libpng_public_hdrs}
  ${libpng_private_hdrs}
  ${PNG_DIR}/png.c
  ${PNG_DIR}/pngerror.c
  ${PNG_DIR}/pngget.c
  ${PNG_DIR}/pngmem.c
  ${PNG_DIR}/pngpread.c
  ${PNG_DIR}/pngread.c
  ${PNG_DIR}/pngrio.c
  ${PNG_DIR}/pngrtran.c
  ${PNG_DIR}/pngrutil.c
  ${PNG_DIR}/pngset.c
  ${PNG_DIR}/pngtrans.c
  ${PNG_DIR}/pngwio.c
  ${PNG_DIR}/pngwrite.c
  ${PNG_DIR}/pngwtran.c
  ${PNG_DIR}/pngwutil.c
  ${libpng_arm_sources}
  ${libpng_intel_sources}
  ${libpng_mips_sources}
  ${libpng_powerpc_sources}
)
set(pngtest_sources
  ${PNG_DIR}/pngtest.c
)
set(pngvalid_sources
  ${PNG_DIR}/contrib/libtests/pngvalid.c
)
set(pngstest_sources
  ${PNG_DIR}/contrib/libtests/pngstest.c
)
set(pngunknown_sources
  ${PNG_DIR}/contrib/libtests/pngunknown.c
)
set(pngimage_sources
  ${PNG_DIR}/contrib/libtests/pngimage.c
)
set(pngfix_sources
  ${PNG_DIR}/contrib/tools/pngfix.c
)
set(png_fix_itxt_sources
  ${PNG_DIR}/contrib/tools/png-fix-itxt.c
)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

if(PNG_DEBUG)
  add_definitions(-DPNG_DEBUG)
endif()

# NOW BUILD OUR TARGET
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ZLIB_INCLUDE_DIR})

unset(PNG_LIB_TARGETS)

if(PNG_SHARED)
  add_library(png SHARED ${libpng_sources})
  set(PNG_LIB_TARGETS png)
  set_target_properties(png PROPERTIES OUTPUT_NAME ${PNG_LIB_NAME})
  add_dependencies(png genfiles)
  if(MSVC)
    # msvc does not append 'lib' - do it here to have consistent name
    set_target_properties(png PROPERTIES PREFIX "lib")
    set_target_properties(png PROPERTIES IMPORT_PREFIX "lib")
  endif()
  target_link_libraries(png ${ZLIB_LIBRARY} ${M_LIBRARY})

  if(UNIX AND AWK)
    if(HAVE_LD_VERSION_SCRIPT)
      set_target_properties(png PROPERTIES LINK_FLAGS
        "-Wl,--version-script='${CMAKE_CURRENT_BINARY_DIR}/libpng.vers'")
    elseif(HAVE_SOLARIS_LD_VERSION_SCRIPT)
      set_target_properties(png PROPERTIES LINK_FLAGS
        "-Wl,-M -Wl,'${CMAKE_CURRENT_BINARY_DIR}/libpng.vers'")
    endif()
  endif()
endif()

if(PNG_STATIC)
  # does not work without changing name
  set(PNG_LIB_NAME_STATIC png_static)
  add_library(png_static STATIC ${libpng_sources})
  add_dependencies(png_static genfiles)
  # MSVC doesn't use a different file extension for shared vs. static
  # libs.  We are able to change OUTPUT_NAME to remove the _static
  # for all other platforms.
  if(NOT MSVC)
    set_target_properties(png_static PROPERTIES
      OUTPUT_NAME "${PNG_LIB_NAME}"
      CLEAN_DIRECT_OUTPUT 1)
  else()
    set_target_properties(png_static PROPERTIES
      OUTPUT_NAME "${PNG_LIB_NAME}_static"
      CLEAN_DIRECT_OUTPUT 1)
  endif()
  list(APPEND PNG_LIB_TARGETS png_static)
  if(MSVC)
    # msvc does not append 'lib' - do it here to have consistent name
    set_target_properties(png_static PROPERTIES PREFIX "lib")
  endif()
  target_link_libraries(png_static ${ZLIB_LIBRARY} ${M_LIBRARY})
endif()

if(PNG_FRAMEWORK)
  set(PNG_LIB_NAME_FRAMEWORK png_framework)
  add_library(png_framework SHARED ${libpng_sources})
  add_dependencies(png_framework genfiles)
  list(APPEND PNG_LIB_TARGETS png_framework)
  set_target_properties(png_framework PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION ${PNGLIB_VERSION}
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PNGLIB_MAJOR}.${PNGLIB_MINOR}
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${PNGLIB_VERSION}
    MACOSX_FRAMEWORK_IDENTIFIER org.libpng.libpng
    XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
    PUBLIC_HEADER "${libpng_public_hdrs}"
    OUTPUT_NAME png)
  target_link_libraries(png_framework ${ZLIB_LIBRARY} ${M_LIBRARY})
endif()

if(NOT PNG_LIB_TARGETS)
  message(SEND_ERROR
    "No library variant selected to build. "
    "Please enable at least one of the following options: "
    "PNG_STATIC, PNG_SHARED, PNG_FRAMEWORK")
endif()

if(PNG_SHARED AND WIN32)
  set_target_properties(png PROPERTIES DEFINE_SYMBOL PNG_BUILD_DLL)
endif()

if(PNG_SHARED)
  add_executable(pngfix ${pngfix_sources})
  target_link_libraries(pngfix png)
  set(PNG_BIN_TARGETS pngfix)

  add_executable(png-fix-itxt ${png_fix_itxt_sources})
  target_link_libraries(png-fix-itxt ${ZLIB_LIBRARY} ${M_LIBRARY})
  list(APPEND PNG_BIN_TARGETS png-fix-itxt)
endif()

# Set a variable with CMake code which:
# Creates a symlink from src to dest (if possible) or alternatively
# copies if different.
include(CMakeParseArguments)

function(create_symlink DEST_FILE)

  cmake_parse_arguments(S "" "FILE;TARGET" "" ${ARGN})

  if(NOT S_TARGET AND NOT S_FILE)
    message(FATAL_ERROR "create_symlink: Missing TARGET or FILE argument")
  endif()

  if(S_TARGET AND S_FILE)
    message(FATAL_ERROR "create_symlink: Both source file ${S_FILE} and build target ${S_TARGET} arguments are present; can only have one.")
  endif()

  if(S_FILE)
    # If we don't need to symlink something that's coming from a build target,
    # we can go ahead and symlink/copy at configure time.
    if(CMAKE_HOST_WIN32 AND NOT CYGWIN)
      execute_process(
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${S_FILE} ${DEST_FILE}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
    else()
      execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${S_FILE} ${DEST_FILE}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
    endif()
  endif()

  if(S_TARGET)
    # We need to use generator expressions, which can be a bit tricky, so for
    # simplicity make the symlink a POST_BUILD step and use the TARGET
    # signature of add_custom_command.
    if(CMAKE_HOST_WIN32 AND NOT CYGWIN)
      add_custom_command(TARGET ${S_TARGET} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different $<TARGET_LINKER_FILE_NAME:${S_TARGET}> $<TARGET_LINKER_FILE_DIR:${S_TARGET}>/${DEST_FILE})
    else()
      add_custom_command(TARGET ${S_TARGET} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E create_symlink $<TARGET_LINKER_FILE_NAME:${S_TARGET}> $<TARGET_LINKER_FILE_DIR:${S_TARGET}>/${DEST_FILE})
    endif()
  endif()

endfunction()

# Create source generation scripts.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scripts/genchk.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/scripts/genchk.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scripts/genout.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/scripts/genout.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scripts/gensrc.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/scripts/gensrc.cmake @ONLY)

# libpng is a library so default to 'lib'
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR lib)
endif()

# CREATE PKGCONFIG FILES
# We use the same files like ./configure, so we have to set its vars.
# Only do this on Windows for Cygwin - the files don't make much sense outside
# of a UNIX look-alike.
if(NOT WIN32 OR CYGWIN OR MINGW)
  set(prefix      ${CMAKE_INSTALL_PREFIX})
  set(exec_prefix ${CMAKE_INSTALL_PREFIX})
  set(libdir      ${CMAKE_INSTALL_FULL_LIBDIR})
  set(includedir  ${CMAKE_INSTALL_FULL_INCLUDEDIR})
  set(LIBS        "-lz -lm")
  configure_file(${PNG_DIR}/libpng.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PNGLIB_NAME}.pc @ONLY)
  create_symlink(libpng.pc FILE ${PNGLIB_NAME}.pc)

  configure_file(${PNG_DIR}/libpng-config.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PNGLIB_NAME}-config @ONLY)
  create_symlink(libpng-config FILE ${PNGLIB_NAME}-config)
endif()

# SET UP LINKS
if(PNG_SHARED)
  set_target_properties(png PROPERTIES
#   VERSION 16.${PNGLIB_RELEASE}.1.6.37
    VERSION 16.${PNGLIB_RELEASE}.0
    SOVERSION 16
    CLEAN_DIRECT_OUTPUT 1)
endif()