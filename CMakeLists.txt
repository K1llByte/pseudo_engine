####### CMake stuff #######
cmake_minimum_required(VERSION 3.19)
set(CMAKE_TOOLCHAIN_FILE "./vcpkg/scripts/buildsystems/vcpkg.cmake")
set(TESTING_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/src")

####### Project Setup #######
project(pseudo_engine)

file(GLOB_RECURSE TESTING_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
# file(GLOB_RECURSE TESTING_SOURCES "${PROJECT_SOURCE_DIR}/src/pe/*.cpp")

add_executable(pseudo_engine ${TESTING_SOURCES})
target_include_directories(pseudo_engine PRIVATE "${PROJECT_SOURCE_DIR}/src")

####### Dependencies #######

# NOTE: ALL PACKAGES NEED TO BE INSTALLED AS package_name:x64-windows

find_package(fmt CONFIG REQUIRED)
target_link_libraries(pseudo_engine PRIVATE fmt::fmt)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(pseudo_engine PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

find_package(glm CONFIG REQUIRED)
target_link_libraries(pseudo_engine PRIVATE glm::glm)

# find_package(EnTT CONFIG REQUIRED)
# target_link_libraries(pseudo_engine PRIVATE EnTT::EnTT)

####### Compilation settings #######
if(MSVC)
    # Compiling with Microsoft Visual C++.
    list(
        APPEND
        WARNING_FLAGS
        "/W4"
        "/Zc:preprocessor")
else()
    # Compiling with g++/clang++/etc.
    list(
        APPEND
        WARNING_FLAGS
        "-Wall"
        "-Wextra"
        "-Wdouble-promotion"
        "-Wconversion"
        "-Wsign-conversion"
    )
endif()

target_compile_options(
    pseudo_engine
    PRIVATE ${WARNING_FLAGS}
)
target_compile_features(pseudo_engine PRIVATE cxx_std_20) # C++20