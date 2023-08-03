set(EXAMPLE1_SRC_FILES

    common.cpp
    main.cpp
    example1_sm.cpp
    b_sm.cpp
    e_sm.cpp 

    )


add_executable(example1
    ${EXAMPLE1_SRC_FILES}
    )

target_compile_options(example1 PRIVATE
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
     )


 set(EXAMPLE1_LIBS
    roosthsm
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set(EXAMPLE1_LIBS
       ${EXAMPLE1_LIBS}
       pthread
        )
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set(EXAMPLE1_LIBS
       ${EXAMPLE1_LIBS}
       pthread
        )
endif()

target_link_libraries(example1 ${EXAMPLE1_LIBS})

INSTALL(TARGETS example1 DESTINATION examples)


