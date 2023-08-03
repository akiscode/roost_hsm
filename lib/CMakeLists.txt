set(LIB_SOURCE_FILES
    src/common.cpp
    src/state_machine.cpp
    src/node.cpp
    src/constants.cpp
    src/alias.cpp
    src/transition_table.cpp
    src/spy.cpp
)

add_library(roosthsm STATIC ${LIB_SOURCE_FILES})

target_include_directories(roosthsm
    PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
    PRIVATE
	src
)

set_target_properties(roosthsm PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_options(roosthsm PRIVATE
     -DROOST_MAJOR_VERSION=${VERSION_MAJOR}
     -DROOST_MINOR_VERSION=${VERSION_MINOR}
     -DROOST_PATCH_VERSION=${VERSION_PATCH}
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>

     )

INSTALL(TARGETS roosthsm
	EXPORT lib1Targets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
	
INSTALL(DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Make the project importable from build directory
EXPORT(TARGETS roosthsm FILE roostHsmConfig.cmake)

if (BUILD_TESTS)
    add_subdirectory(test)
endif()
