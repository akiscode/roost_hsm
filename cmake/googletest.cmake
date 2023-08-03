ExternalProject_Add(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/externals/gtest
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD OFF
  LOG_CONFIGURE OFF
  LOG_BUILD OFF
  CMAKE_ARGS
  -DCMAKE_INSTALL_MESSAGE:STRING=LAZY
  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
  -DCMAKE_CXX_FLAGS="-std=gnu++11"
  -DCMAKE_CXX_COMPILE_FEATURES=

)

ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/googletest/include)
set(GMOCK_INCLUDE_DIRS ${source_dir}/googlemock/include)

ExternalProject_Get_Property(googletest binary_dir)
set(GTEST_LIBRARY_PATH ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
set(GTEST_LIBRARY gtest)

add_library(${GTEST_LIBRARY} UNKNOWN IMPORTED)
set_target_properties(${GTEST_LIBRARY} PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

add_dependencies(${GTEST_LIBRARY} googletest)

set(GTEST_MAIN_LIBRARY_PATH ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a)
set(GTEST_MAIN_LIBRARY gtest_main)

add_library(${GTEST_MAIN_LIBRARY} UNKNOWN IMPORTED)
set_target_properties(${GTEST_MAIN_LIBRARY} PROPERTIES
  "IMPORTED_LOCATION" "${GTEST_MAIN_LIBRARY_PATH}"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"

)

add_dependencies(${GTEST_MAIN_LIBRARY} googletest)

set(GMOCK_LIBRARY_PATH ${binary_dir}/googlemock/${CMAKE_FIND_LIBRARY_PREFIXES}gmock.a)
set(GMOCK_LIBRARY gmock)
add_library(${GMOCK_LIBRARY} UNKNOWN IMPORTED)
set_target_properties(${GMOCK_LIBRARY} PROPERTIES
  "IMPORTED_LOCATION" "${GMOCK_LIBRARY_PATH}"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

add_dependencies(${GMOCK_LIBRARY} googletest)

set(GMOCK_MAIN_LIBRARY_PATH ${binary_dir}/googlemock/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main.a)
set(GMOCK_MAIN_LIBRARY gmock_main)
add_library(${GMOCK_MAIN_LIBRARY} UNKNOWN IMPORTED)
set_target_properties(${GMOCK_MAIN_LIBRARY} PROPERTIES
  "IMPORTED_LOCATION" "${GMOCK_MAIN_LIBRARY_PATH}"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

add_dependencies(${GMOCK_MAIN_LIBRARY} ${GTEST_LIBRARY})
