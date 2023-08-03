set(ROOST_BENCH_SRC_FILES
    sample_bench.cpp
    ${SHARED_SM1_FILES}
)

include_directories(${HAYAI_INCLUDE_DIRS} ${SHARED_INCLUDE_DIR} )

add_executable(roostBench ${ROOST_BENCH_SRC_FILES})

set_target_properties(roostBench PROPERTIES OUTPUT_NAME roostHsmBench)

add_dependencies(roostBench hayai )


set(ROOST_BENCH_LIBS
    hayai
    roosthsm
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set(ROOST_BENCH_LIBS
       ${ROOST_BENCH_LIBS}
       pthread
        )
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set(ROOST_BENCH_LIBS
       ${ROOST_BENCH_LIBS}
       pthread
        )
endif()

target_link_libraries(roostBench ${ROOST_BENCH_LIBS})

INSTALL(TARGETS roostBench DESTINATION bench )

