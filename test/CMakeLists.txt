project(run_all_tests)

include_directories(../src)

add_executable(${PROJECT_NAME}
    run_all_tests.cpp
    duration_test.cpp
    volume_test.cpp)

target_link_libraries(${PROJECT_NAME}
    untzy_lib
    gmock
    gtest)
