set(ROOST_UNIT_TEST_SRC_FILES
    main.cpp
    roost_test.cpp
    ${SHARED_SM1_FILES}
)    

enable_testing()

include_directories(${SHARED_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS})
add_executable(roostUnitTests ${ROOST_UNIT_TEST_SRC_FILES})

set_target_properties(roostUnitTests PROPERTIES OUTPUT_NAME roostHsmUnitTests)


target_compile_options(roostUnitTests PRIVATE
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
     )



add_test(
    NAME roostUnitTests
    COMMAND roostHsmUnitTests
)


set(ROOST_UNIT_TEST_LIBS
    gtest
    roosthsm
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set(ROOST_UNIT_TEST_LIBS
       ${ROOST_UNIT_TEST_LIBS}
       pthread
        )
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set(ROOST_UNIT_TEST_LIBS
       ${ROOST_UNIT_TEST_LIBS}
       pthread
        )
endif()

add_dependencies(roostUnitTests gtest roosthsm)
target_link_libraries(roostUnitTests ${ROOST_UNIT_TEST_LIBS})

if (NOT PRODUCTION_BUILD)
        INSTALL(TARGETS roostUnitTests DESTINATION unit)
endif()

	
