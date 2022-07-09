add_executable(test_bjl_01 test1.cpp)
target_link_libraries(test_bjl_01 PRIVATE bjlLib)

add_custom_command(TARGET test_bjl_01
POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE:test_bjl_01>"
)

