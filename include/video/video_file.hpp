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
            /// @param i: frame index
            /// @returns newly allocated texture by value
            [[nodiscard]] Texture<float, 3> get_frame(size_t i);

            /// @brief modify video frame by downloading the current state of the texture into cache
            /// @param i: frame index
            /// @param texture: texture holding the new frame data, has to be of same size
            void set_frame(size_t i, const Texture<float, 3>&);

            /// @brief retrieve the nths frame from a file
            /// @param i: frame index
            /// @returns frame as image
            [[nodiscard]] Image<float, 3> get_frame_as_image(size_t i);

            /// @brief modify video frame by assigning an image to it
            /// @param i: frame index
            /// @param image: image holding new frame data
            void set_frame(size_t i, const Image<float, 3>&);

            /// @brief download all frames into the cache at once
            void cache();

            /// @brief open a stream from a video file
            /// @param path: absolute path to file
            void load(std::string path);

            /// @brief save the current state of the video file to disk
            /// @param path: absolute path to file including file extension
            /// @note the output file is assumed to have identical encoding, resolution and depth as the original input file, regardless of any processing that took place in the mean time
            void save(std::string path);

            /// @brief get total number of frame
            /// @returns size_t
            size_t get_n_frames() const;

            /// @brief get resolution
            /// @returns vector
            Vector2ui get_size() const;

            /// @brief get frames per second
            /// @returns float
            float get_fps() const;

        private:
            void cache_frames_until(size_t i);

            Vector2ui _size;
            double _fps = 0;
            int _codec;
            int _codec_format;

            size_t _n_frames = 0;

            size_t _current_frame = 0;
            cv::VideoCapture _capture;

            size_t _frames_loaded = 0;
            std::vector<cv::Mat> _frames;
    };
}

#include ".src/video_file.inl"