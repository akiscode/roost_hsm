set(SHARED_SM1_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/share/sm1/sm1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/share/sm1/sm1_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/share/sm2/sm2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/share/sm2/sm2_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/share/join_sm/join_sm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/share/join_sm/join_sm_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/share/simple_history/simple_history.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/share/simple_history/simple_history_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/share/ortho_history/ortho_history.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/share/ortho_history/ortho_history_common.cpp
)

set(SHARED_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/share/
    )

add_subdirectory(unit)

if (BUILD_BENCH)
    add_subdirectory(bench)
endif()
