if(EXISTS "/Users/alyonashinkareva/Desktop/chance/second-chance-multitype-alyoshinkkaa/cmake-build-debug/libraries/acp/tests[1]_tests.cmake")
  include("/Users/alyonashinkareva/Desktop/chance/second-chance-multitype-alyoshinkkaa/cmake-build-debug/libraries/acp/tests[1]_tests.cmake")
else()
  add_test(tests_NOT_BUILT tests_NOT_BUILT)
endif()
