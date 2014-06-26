cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(cppmythtest)
include(GNUInstallDirs)

if (MSVC)
  # This option must match the settings used in your program, in particular if you
  # are linking statically
  OPTION( STATIC_CRT "Link the static CRT libraries" OFF )
endif (MSVC)

###############################################################################
# add definitions
if (MSVC)
  # Turn off Microsofts "security" warnings.
  add_definitions( "/W3 /D_USE_32BIT_TIME_T /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo" )
  if (STATIC_CRT)
    set(CMAKE_C_FLAGS_RELEASE "/MT")
    set(CMAKE_C_FLAGS_DEBUG "/MTd")
  endif (STATIC_CRT)
endif (MSVC)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_C_FLAGS "-fPIC")
   set(CMAKE_CXX_FLAGS "-fPIC")
endif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

###############################################################################
# configure

include_directories (
  ${CMAKE_BINARY_DIR}/cppmyth/include/.)

###############################################################################
# add sources
file(GLOB SRC_FILES src/*.cpp)

set(CPPMYTHTEST_SOURCES
  ${SRC_FILES})

###############################################################################
# add targets
add_executable(cppmythtest ${CPPMYTHTEST_SOURCES})
add_dependencies(cppmythtest cppmyth jansson)
if(MSVC)
  target_link_libraries(cppmythtest cppmyth jansson ws2_32.lib)
else(MSVC)
  target_link_libraries(cppmythtest cppmyth jansson rt pthread)
endif(MSVC)

###############################################################################
# install targets
install(TARGETS cppmythtest
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
