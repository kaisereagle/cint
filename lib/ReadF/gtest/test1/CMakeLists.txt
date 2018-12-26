set(GTEST_ROOT $ENV{HOME}/gtest-1.7.0)
include_directories(
    ${GTEST_ROOT}/include
    )
add_executable(test1
               test1.cxx
               test2.cxx
               test3.cxx
               test4.cxx
               ../../MultiByteChar.cxx
               ../../ReadF.cxx
	)
target_link_libraries(test1
    pthread
    ${GTEST_ROOT}/build/libgtest.a
    ${GTEST_ROOT}/build/libgtest_main.a
    )

