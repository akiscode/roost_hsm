set(QUICKSTART_SRC_FILES

    main.cpp
    quickstart_sm.cpp
    common.cpp

    )


add_executable(quickstart
    ${QUICKSTART_SRC_FILES}
    )

target_compile_options(quickstart PRIVATE
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
     )


 set(QUICKSTART_LIBS
    roosthsm
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set(QUICKSTART_LIBS
       ${QUICKSTART_LIBS}
       pthread
        )
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set(QUICKSTART_LIBS
       ${QUICKSTART_LIBS}
       pthread
        )
endif()

target_link_libraries(quickstart ${QUICKSTART_LIBS})

INSTALL(TARGETS quickstart DESTINATION examples)


