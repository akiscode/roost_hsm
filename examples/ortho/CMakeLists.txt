set(ORTHO_SRC_FILES

    main.cpp
    common.cpp
    ortho_sm.cpp
    regiona.cpp
    regionc.cpp

    )


add_executable(ortho
    ${ORTHO_SRC_FILES}
    )

target_compile_options(ortho PRIVATE
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
     )


 set(ORTHO_LIBS
    roosthsm
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set(ORTHO_LIBS
       ${ORTHO_LIBS}
       pthread
        )
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set(ORTHO_LIBS
       ${ORTHO_LIBS}
       pthread
        )
endif()

target_link_libraries(ortho ${ORTHO_LIBS})

INSTALL(TARGETS ortho DESTINATION examples)


