cmake_minimum_required(VERSION 3.20)
project(rac2_client)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(pine INTERFACE)
target_include_directories(pine INTERFACE pine/)

add_executable(rac2_client main.cpp rac2_interface.cpp)

target_link_libraries(rac2_client pine)

set(MINGW OFF CACHE BOOL "" FORCE)

add_subdirectory(APCpp)
include_directories(APCpp)
target_link_libraries(rac2_client APCpp)

add_subdirectory(readerwriterqueue)
include_directories(readerwriterqueue)
target_link_libraries(rac2_client readerwriterqueue)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
