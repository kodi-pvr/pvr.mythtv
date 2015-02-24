cmake_minimum_required (VERSION 2.6 FATAL_ERROR)
cmake_policy (VERSION 2.6)

project (cppmyth)

enable_language(CXX)
enable_language(C)

# Options
if (MSVC)
  # This option must match the settings used in your program, in particular if you
  # are linking statically
  OPTION (STATIC_CRT "Link the static CRT libraries" OFF)
endif ()

###############################################################################
# set lib version here
set (CPPMYTH_LIB_VERSION "1.1.10")
set (CPPMYTH_LIB_SOVERSION "1.1")

###############################################################################
# add definitions
if (MSVC)
  # Turn off Microsofts "security" warnings.
  add_definitions("/W3 /D_USE_32BIT_TIME_T /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo")
  if (STATIC_CRT)
    set (CMAKE_C_FLAGS_RELEASE "/MT")
    set (CMAKE_C_FLAGS_DEBUG "/MTd")
  endif ()
endif ()

if (NOT WIN32 AND (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX))
   add_definitions ("-fPIC")
endif ()

###############################################################################
# configure
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include (CheckFunctionExists)
include (CheckFunctionKeywords)

check_function_exists (timegm CHK_TIMEGM)
if (CHK_TIMEGM)
    set (HAVE_TIMEGM 1)
else ()
    set (HAVE_TIMEGM 0)
endif ()

check_function_exists (localtime_r CHK_LOCALTIME_R)
if (CHK_LOCALTIME_R)
    set (HAVE_LOCALTIME_R 1)
else ()
    set (HAVE_LOCALTIME_R 0)
endif ()

check_function_exists (gmtime_r CHK_GMTIME_R)
if (CHK_GMTIME_R)
    set (HAVE_GMTIME_R 1)
else ()
    set (HAVE_GMTIME_R 0)
endif ()

# Check what the inline keyword is.
check_function_keywords ("inline")
check_function_keywords ("__inline")
check_function_keywords ("__inline__")
if (HAVE_INLINE)
   set (CC_INLINE inline)
elseif (HAVE___INLINE)
   set (CC_INLINE __inline)
elseif (HAVE___INLINE__)
   set (CC_INLINE __inline__)
else ()
   # no inline on this platform
   set (CC_INLINE)
endif ()

# configure the public config file
configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cppmyth_config.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cppmyth_config.h)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}/src/.)

###############################################################################
# add sources
file (GLOB SRC_FILES
  src/private/mythdto/*.cpp
  src/private/*.c
  src/private/*.cpp
  src/proto/*.cpp
  src/*.cpp)

file (GLOB HDR_FILES
  src/proto/*.h src/*.h)

if (MSVC)
  file (GLOB PLATFORM_SRC_FILES
    src/private/platform/windows/*.cpp)
endif ()

set (CPPMYTH_SOURCES
  ${SRC_FILES} ${HDR_FILES} ${PLATFORM_SRC_FILES})

###############################################################################
# add targets
add_library(cppmyth STATIC ${CPPMYTH_SOURCES})

if(MSVC)
  target_link_libraries(cppmyth ws2_32)
endif()

