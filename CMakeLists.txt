cmake_minimum_required(VERSION 3.10)
project(multicurve)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(multicurve_mainframe
    src/core/main.cpp
    src/api/multiserver.cpp)

target_include_directories(multicurve_mainframe PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/include)


find_path(HTTPLIB_INCLUDE_DIR httplib.h ./include/)
if(HTTPLIB_INCLUDE_DIR)
    message(STATUS "Found httplib: ${HTTPLIB_INCLUDE_DIR}")
else()
    # Download httplib if not found
    message(STATUS "Downloading httplib...")
    include(FetchContent)
    FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG        v0.12.1
    )
    FetchContent_MakeAvailable(httplib)
endif()








# find_package(httplib REQUIRED)
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(HTTPLIB REQUIRED httplib)


# target_link_libraries(multicurve_mainframe PRIVATE httplib)
