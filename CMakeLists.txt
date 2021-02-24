cmake_minimum_required(VERSION 3.10)
project(CCash)
set (CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(${PROJECT_NAME} backend.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC third_party)
target_sources(${PROJECT_NAME} PRIVATE src/bank.cpp PRIVATE src/BankAPI.cpp)

add_subdirectory(third_party/drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE drogon)   
