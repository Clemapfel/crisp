// 
// Copyright 2021 Clemens Cords
// Created on 06.11.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/core.hpp>

#include <vector.hpp>

#include <gpu_side/is_gpu_side.hpp>
#include <gpu_side/texture.hpp>

#include <image/color_image.hpp>

namespace crisp
{
    class VideoFile
    {
        class Iterator;
        class ConstIterator;

        public:
            VideoFile();

            /// @brief retrieve the nths frame from a file
            Texture<float, 3> get_frame(size_t i);

            /// @brief modify video frame by downloading the current state of the texture into cache
            void set_frame(size_t i, Texture<float, 3>&);

            /// @brief download all frames into vram at once
            void cache();

            /// @brief open a stream from a video file
            /// @param path: absolute path to file
            void create(std::string path);

            /// @brief get total number of frame
            size_t get_n_frames() const;

            /// @brief get current frame index
            size_t get_current_frame() const;

            /// @brief get resolution
            Vector2ui get_size() const;

        private:
            void cache_frames_until(size_t i);

            Vector2ui _size;
            size_t _n_frames = 0;

            size_t _current_frame = 0;
            cv::VideoCapture _capture;

            size_t _frames_loaded = 0;
            std::vector<cv::Mat> _frames;
    };
}

#include ".src/video_file.inl"