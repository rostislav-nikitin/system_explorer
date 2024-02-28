cmake_minimum_required(VERSION 3.14)

project(WildCardExpr)

include(FetchContent)

FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0)

FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()

add_executable(test_runner wild_card_expr.cpp test_runner.cpp)
target_link_libraries(test_runner gmock gtest)

add_executable(use_wild_card_expr wild_card_expr.cpp use_wild_card_expr.cpp)
