cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -lstdc++fs -pthread -lpthread")
set(CMAKE_CXX_STANDARD 20)

project(crisp)
include_directories(./)

set(CMAKE_CXX_STANDARD 20)

### DEPENDENCIES & INCLUDE DIRECTORIES ##############

# SFML
set(SFML_DIR "/usr/include/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

# EIGEN
include_directories("./eigen/Eigen")

### CRISP ############################################

add_library(crisp STATIC

        include/vector.hpp
        .src/vector.inl

        include/image/multi_plane_image.hpp
        .src/multi_plane_image.inl

        include/image/binary_image.hpp
        .src/binary_image.inl

        include/image/color_image.hpp
        .src/color_image.inl

        include/image/image_conversion.hpp
        .src/image_conversion.inl

        include/color.hpp .src/color.inl)

set_target_properties(crisp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(crisp sfml-graphics fftw3 fftw3l fftw3f)

# TODO: REMOVE
include_directories("./include")
include_directories("./.src")
include_directories("./.test")

add_executable(TEST .test/main.cpp)
target_link_libraries(TEST sfml-graphics sfml-window sfml-system)

