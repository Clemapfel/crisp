cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -lstdc++fs -pthread -lpthread -lGL")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

project(crisp)

### DEPENDENCIES ################################################################################

# SFML
set(SFML_DIR "/usr/include/SFML") # modify this if SFML is located somewhere else
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

# EIGEN
include_directories("./eigen/Eigen")

# OpenGL
FIND_PACKAGE(OpenGL REQUIRED)

# sol2
include_directories("./sol2/include/sol")
include_directories("./sol2/include")
include_directories("/usr/include/lua5.3")
find_package(Lua REQUIRED)

# OpenCV
SET("OpenCV_DIR" "/usr/share/OpenCV")
find_package(OpenCV REQUIRED)

### CRISP #######################################################################################

set(RESOURCE_PATH ${CMAKE_SOURCE_DIR})
configure_file(/home/clem/Workspace/crisp/.src/resource_path.hpp.in /home/clem/Workspace/crisp/resource_path.hpp @ONLY)

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
        .src/pseudocolor_mapping_gpu.inl

        .src/common.inl

        include/frequency_domain_filter.hpp
        .src/frequency_domain_filter.inl
        .src/frequency_domain_filter_gpu.inl

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
        .src/salt_and_pepper_distribution.inl

        include/classification/fully_connected_neural_network.hpp
        .src/fully_connected_neural_network.inl

        include/classification/bayes_classifier.hpp
        .src/bayes_classifier.inl

        include/gpu_side/native_handle.hpp
        include/gpu_side/state.hpp
        .src/state.inl

        include/gpu_side/texture_workspace.hpp
        .src/texture_workspace.inl foo.h .src/texture.inl

        include/benchmark.hpp
        .src/benchmark.inl

        include/gpu_side/is_gpu_side.hpp

        include/video/video_file.hpp
        .src/video_file.inl

        include/gpu_side/hardware_accelerated_matrix.hpp
        .src/hardware_accelerated_matrix.inl

        include/video/video_stream.hpp

        include/gpu_side/whole_texture_transform.hpp
        .src/whole_texture_transform.inl

        include/audio/audio_file.hpp
        .src/audio_file.inl)

set_target_properties(crisp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(crisp
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        fftw3
        fftw3l
        fftw3f
        ${OPENGL_LIBRARIES}
        ${LUA_LIBRARIES}
        ${OpenCV_LIBS})

include_directories("./include")
include_directories("./")
include_directories("/usr/include/opencv4")

add_executable(AUDIO .test/main.cpp)
target_link_libraries(AUDIO crisp)


