add_test([=[HelloTest.BasicAssertions]=]  /home/cs300-user/cemu/build/g_test [==[--gtest_filter=HelloTest.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[HelloTest.BasicAssertions]=]  PROPERTIES WORKING_DIRECTORY /home/cs300-user/cemu/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  g_test_TESTS HelloTest.BasicAssertions)
