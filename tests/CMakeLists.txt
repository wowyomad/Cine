# enable_testing()

# set(UNIT_TEST glash_test)

# add_executable(${UNIT_TEST} test_glash.cpp)

# target_link_libraries(${UNIT_TEST} PRIVATE glash gtest gtest_main)

# add_test(NAME ${UNIT_TEST} COMMAND test_glash)

# add_custom_command(
#      TARGET ${UNIT_TEST}
#      COMMENT "Run tests"
#      POST_BUILD 
#      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#      COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> -R "^${UNIT_TEST}$" --output-on-failures
# )
