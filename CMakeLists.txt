cmake_minimum_required(VERSION 3.4)
project(rac2_client)

add_executable(rac2_client main.cpp pine.h)

set(WIN32 ON CACHE BOOL "" FORCE)
set(MINGW ON CACHE BOOL "" FORCE)
set(USE_ZLIB OFF CACHE BOOL "" FORCE)

add_subdirectory(APCpp)
include_directories(APCpp)
target_link_libraries(rac2_client APCpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
