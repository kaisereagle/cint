cmake_minimum_required(VERSION 2.8)
add_executable(Main
	test1/test1.cxx
)

set(GTEST_ROOT $ENV{HOME}/gtest-1.7.0)
include_directories(
    ${GTEST_ROOT}/include
    )
add_executable(Main
               test1.cxx
               test2.cxx
               test3.cxx
               ../../MultiByteChar.cxx
               ../../ReadF.cxx
	)
target_link_libraries(
    pthread
    ${GTEST_ROOT}/build/libgtest.a
    ${GTEST_ROOT}/build/libgtest_main.a
    )


