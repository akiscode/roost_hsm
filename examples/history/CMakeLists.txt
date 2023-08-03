set(HISTORY_SRC_FILES

    main.cpp
    history_sm.cpp
    common.cpp

    )


add_executable(history
    ${HISTORY_SRC_FILES}
    )

target_compile_options(history PRIVATE
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
     )


 set(HISTORY_LIBS
    roosthsm
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set(HISTORY_LIBS
       ${HISTORY_LIBS}
       pthread
        )
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set(HISTORY_LIBS
       ${HISTORY_LIBS}
       pthread
        )
endif()

target_link_libraries(history ${HISTORY_LIBS})

INSTALL(TARGETS history DESTINATION examples)


