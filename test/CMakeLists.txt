project(run_all_tests)

add_executable(${PROJECT_NAME}
    run_all_tests.cpp)

target_link_libraries(${PROJECT_NAME}
    gmock
    gtest)
