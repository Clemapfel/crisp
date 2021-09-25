cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -lstdc++fs -pthread -lpthread")
set(CMAKE_CXX_STANDARD 20)

project(crisp)

### DEPENDENCIES ################################################################################

# SFML
set(SFML_DIR "/usr/include/SFML") # modify this SFML is located somewhere else
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

# EIGEN
include_directories("./eigen/Eigen")

### CRISP #######################################################################################

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
        .src/grayscale_image.inl

        include/spatial_filter.hpp
        .src/spatial_filter.inl

        include/morphological_transform.hpp
        .src/morphological_transform.inl

        include/whole_image_transform.hpp
        .src/whole_image_transform.inl

        include/pseudocolor_mapping.hpp
        .src/pseudocolor_mapping.inl

        .src/common.inl

        include/frequency_domain_filter.hpp
        .src/frequency_domain_filter.inl

        include/fourier_transform.hpp
        .src/fourier_transform.inl

        include/histogram.hpp
        .src/histogram.inl

        include/edge_detection.hpp
        .src/edge_detection.inl

        include/image_segment.hpp
        include/segmentation.hpp
        .src/segmentation.inl

        include/image_region.hpp
        .src/image_region.inl

        include/noise_generator.hpp
        .src/noise_generator.inl
        .src/salt_and_pepper_distribution.inl include/.classification/fully_connected_neural_network.hpp include/.classification/bayes_classifier.hpp include/.classification/sift.hpp include/.classification/convolutional_neural_network.hpp include/.classification/perceptron.hpp)

set_target_properties(crisp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(crisp sfml-graphics sfml-window sfml-system fftw3 fftw3l fftw3f)

include_directories("./include")
include_directories("./")

### EXAMPLES ####################################################################################

add_executable(EXAMPLE_COLOR docs/color/.resources/color_example.cpp)
target_link_libraries(EXAMPLE_COLOR crisp)

add_executable(EXAMPLE_FREQUENCY_DOMAIN docs/frequency_domain/.resources/frequency_domain_example.cpp)
target_link_libraries(EXAMPLE_FREQUENCY_DOMAIN crisp)

add_executable(EXAMPLE_IMAGE docs/image/.resources/image_example.cpp)
target_link_libraries(EXAMPLE_IMAGE crisp)

add_executable(EXAMPLE_MORPHOLOGICAL_TRANSFORM docs/morphological_transform/.resources/morphological_transform_example.cpp)
target_link_libraries(EXAMPLE_MORPHOLOGICAL_TRANSFORM crisp)

add_executable(EXAMPLE_SEGMENTATION docs/segmentation/.resources/segmentation_example.cpp)
target_link_libraries(EXAMPLE_SEGMENTATION crisp)

add_executable(EXAMPLE_SPATIAL_FILTER docs/spatial_filters/.resources/spatial_filter_example.cpp)
target_link_libraries(EXAMPLE_SPATIAL_FILTER crisp)

add_executable(EXAMPLE_SYSTEM docs/system/system_example.cpp)
target_link_libraries(EXAMPLE_SYSTEM crisp)

add_executable(EXAMPLE_FEATURE_EXTRACTION docs/feature_extraction/.resources/feature_extraction_example.cpp)
target_link_libraries(EXAMPLE_FEATURE_EXTRACTION crisp)

add_executable(EXAMPLE_NOISE docs/noise/.resources/noise_main.cpp)
target_link_libraries(EXAMPLE_NOISE crisp)

### TESTS #######################################################################################

add_executable(TEST .test/main.cpp)
target_link_libraries(TEST crisp)

