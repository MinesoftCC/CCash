cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)
project(CCash)

add_executable(${PROJECT_NAME} backend.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC third_party)
target_sources(${PROJECT_NAME} PRIVATE src/bank.cpp PRIVATE src/BankAPI.cpp)

add_subdirectory(third_party/drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE drogon)   
