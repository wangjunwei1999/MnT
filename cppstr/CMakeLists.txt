cmake_minimum_required(VERSION 3.20)
project(mntstr LANGUAGES CXX)

add_library(mntstr INTERFACE)
target_include_directories(mntstr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(mntstr INTERFACE cxx_std_20)

add_executable(test_basic tests/test.cpp)
target_link_libraries(test_basic PRIVATE mntstr)

enable_testing()
add_test(NAME test_basic COMMAND test_basic)