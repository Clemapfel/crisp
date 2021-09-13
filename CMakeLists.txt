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

        include/color.hpp
        .src/color.inl

        include/system/image_io.hpp
        .src/image_io.inl

        include/system/render_window.hpp
        .src/render_window.inl

        include/system/input_handler.hpp
        .src/input_handler.inl

        include/system/key_id.hpp

        include/system/sprite.hpp
        .src/sprite.inl

        include/system.hpp

        include/image/grayscale_image.hpp
        .src/grayscale_image.inl include/image/image_segment.hpp include/spatial_filter.hpp .src/spatial_filter.inl include/morphological_transform.hpp .src/morphological_transform.inl include/whole_image_transform.hpp include/pseudocolor_mapping.hpp .src/pseudocolor_mapping.inl .src/common.inl include/frequency_domain_filter.hpp include/fourier_transform.hpp .src/fourier_transform.inl .src/frequency_domain_filter.inl include/histogram.hpp .src/histogram.inl include/edge_detection.hpp .src/edge_detection.inl include/image_segment.hpp include/segmentation.hpp .src/segmentation.inl .src/whole_image_transform.inl include/image_region.hpp)

set_target_properties(crisp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(crisp sfml-graphics fftw3 fftw3l fftw3f)

# TODO: REMOVE
include_directories("./include")
#include_directories("./.src")
#include_directories("./.test")

add_executable(TEST .test/main.cpp)
target_link_libraries(TEST sfml-graphics sfml-window sfml-system fftw3 fftw3l fftw3f)

