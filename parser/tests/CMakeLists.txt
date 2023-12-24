enable_testing()

add_executable(
  parserTest
  parserTest.cpp
)

target_include_directories(parserTest PRIVATE ../)

target_link_libraries(
  parserTest
  parser
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(parserTest)