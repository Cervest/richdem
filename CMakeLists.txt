cmake_minimum_required(VERSION 3.9)

project(richdem
  VERSION 2.3.1
  DESCRIPTION "High-performance terrain analysis"
  LANGUAGES CXX
)

cmake_policy(SET CMP0077 NEW)

find_package(GDAL)
find_package(OpenMP REQUIRED)
find_package(MPI)
find_package(JlCxx REQUIRED)

set(CMAKE_MACOSX_RPATH 1)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")



add_subdirectory(apps)

if(MPI_CXX_FOUND)
  add_subdirectory(programs/parallel_priority_flood)
  add_subdirectory(programs/parallel_d8_accum)
else()
  message(WARNING "MPI not found; will not compile parallel programs for large-scale datasets.")
endif()

add_subdirectory(tests)

option(USE_GDAL "Whether or not to compile with GDAL." ON)
option(RICHDEM_NO_PROGRESS "Whether or not to show progress bars." OFF)
option(RICHDEM_LOGGING "Whether or not to show logs." OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC RICHDEM_GIT_HASH)

STRING(TIMESTAMP RICHDEM_TIMESTAMP UTC)

add_library(richdem
  src/richdem.cpp
  src/random.cpp
  src/gdal.cpp

  src/terrain_generation/terrain_generation.cpp
  src/terrain_generation/PerlinNoise.cpp
)

add_library(jlrichdem
  src/jlrichdem/common/jlcommon.cpp
  src/jlrichdem/misc/jlmisc.cpp
  src/jlrichdem/depressions/jldepressions.cpp)

# set_target_properties(richdem PROPERTIES VERSION ${PROJECT_VERSION})
# set_target_properties(richdem PROPERTIES SOVERSION 2)
target_include_directories(richdem
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(richdem
  PUBLIC
  cxx_auto_type
  cxx_std_17
)

target_compile_features(jlrichdem PUBLIC cxx_auto_type cxx_std_17)

if(OpenMP_CXX_FOUND)
  target_link_libraries(richdem PUBLIC OpenMP::OpenMP_CXX)
endif()

target_compile_options(richdem
  PUBLIC
  -DRICHDEM_GIT_HASH="${RICHDEM_GIT_HASH}"
  -DRICHDEM_COMPILE_TIME="${RICHDEM_TIMESTAMP}"
)

if(RICHDEM_NO_PROGRESS)
  target_compile_options(richdem
    PUBLIC
    -DRICHDEM_NO_PROGRESS
  )
endif()

if(RICHDEM_LOGGING)
  target_compile_options(richdem PUBLIC -DRICHDEM_LOGGING)
endif()

if(USE_GDAL AND GDAL_FOUND)
  message(STATUS "Compiling RichDEM with GDAL.")
  target_link_libraries(richdem PUBLIC ${GDAL_LIBRARY})
  target_include_directories(richdem PUBLIC ${GDAL_INCLUDE_DIR})
  target_compile_options(richdem PUBLIC -DUSEGDAL)
else()
  message(WARNING "Compiling RichDEM without GDAL!")
endif()


if(JlCxx_FOUND)
  message(STATUS "Compiling jlRichDEM with JlCxx.")
  get_target_property(JlCxx_location JlCxx::cxxwrap_julia LOCATION)
  get_filename_component(JlCxx_location ${JlCxx_location} DIRECTORY)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${JlCxx_location}")
  target_link_libraries(jlrichdem PUBLIC JlCxx::cxxwrap_julia JlCxx::cxxwrap_julia_stl)
else()
  message(FATAL_ERROR "JlCxx library not found. jlRichDEM cannot be compiled to link with julia")
endif()
target_link_libraries(jlrichdem PRIVATE richdem)

add_executable(richdem_unittests
  tests/tests.cpp
  tests/test_main.cpp
  tests/fsm_tests.cpp
)
target_link_libraries(richdem_unittests PRIVATE richdem)
target_compile_features(richdem_unittests PRIVATE cxx_std_17)

install(
  TARGETS richdem
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  RUNTIME DESTINATION "bin"
)

install(
  TARGETS jlrichdem
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  RUNTIME DESTINATION "bin"
)

install(DIRECTORY include/richdem DESTINATION include)
