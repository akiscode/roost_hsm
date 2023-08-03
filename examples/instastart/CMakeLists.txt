add_executable(instastart
    main.cpp
    )

target_compile_options(instastart PRIVATE
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
     )


set(INSTASTART_LIBS
    roosthsm
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set(INSTASTART_LIBS
       ${INSTASTART_LIBS}
       pthread
        )
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set(INSTASTART_LIBS
       ${INSTASTART_LIBS}
       pthread
        )
endif()

target_link_libraries(instastart ${INSTASTART_LIBS})

INSTALL(TARGETS instastart DESTINATION examples)


