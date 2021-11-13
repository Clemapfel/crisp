// 
// Copyright 2021 Clemens Cords
// Created on 08.11.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <exception>

#include <video/video_file.hpp>
#include <image/color_image.hpp>

#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/core.hpp>

namespace crisp
{
    VideoFile::VideoFile()
        : _capture()
    {}

    void VideoFile::cache_frames_until(size_t i)
    {
        while (i >= _current_frame)
        {
            _frames.emplace_back();
            auto& frame = _frames.back();
            _capture.grab();
            _capture.retrieve(frame);
            cv::normalize(frame, frame, 0.0, 1.0, cv::NORM_MINMAX, CV_32F);
            cv::flip(frame, frame, 0);
            _current_frame++;
        }

        if (_current_frame >= _n_frames)
            _capture.release();
    }

    void VideoFile::load(std::string path)
    {
        _capture.open(path);

        if (not _capture.isOpened())
        {
            std::stringstream str;
            str << "[ERROR] Unable to open video file at " << path << std::endl;
            throw std::invalid_argument(str.str());
        }

        _fps = size_t(_capture.get(cv::VideoCaptureProperties::CAP_PROP_FPS));
        _codec = _capture.get(cv::VideoCaptureProperties::CAP_PROP_FOURCC);
        _n_frames = size_t(_capture.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_COUNT));
        _size.x() = size_t(_capture.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH));
        _size.y() = size_t(_capture.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT));
    }

    void VideoFile::save(std::string path)
    {
        auto out = cv::VideoWriter();
        out.open(path, _codec, _fps, cv::Size(_size.x(), _size.y()));
        if (not out.isOpened())
        {
            std::stringstream str;
            str << "[WARNING] Unable to write video file to " << path << std::endl;
            str << "[LOG] No video file saved." << std::endl;
            return;
        }

        for (size_t i = 0; i < _frames.size(); ++i)
        {
            cv::Mat& frame = _frames.at(i);
            cv::normalize(frame, frame, 0.0, 255, cv::NORM_MINMAX, CV_8U);
            cv::flip(frame, frame, 0);
            out.write(frame);
        }
    }

    size_t VideoFile::get_n_frames() const
    {
        return _n_frames;
    }

    Vector2ui VideoFile::get_size() const
    {
        return _size;
    }

    float VideoFile::get_fps() const
    {
        return _fps;
    }

    Texture<float, 3> VideoFile::get_frame(size_t i)
    {
        if (i > _n_frames)
        {
            std::stringstream str;
            str << "[ERROR] Trying to access frame with index " << i << " in video file with " << _n_frames << " frames" << std::endl;
            throw std::out_of_range(str.str());
        }

        cache_frames_until(i);
        return Texture<float, 3>(State::register_texture<float, 3>(_size.x(), _size.y(), &(_frames.at(i).at<float>(0, 0))));
    }

    void VideoFile::set_frame(size_t i, const Texture<float, 3>& tex)
    {
        cache_frames_until(i);
        auto info = State::get_texture_info(tex.get_handle());
        assert(info.width == _size.x() and info.height == _size.y());

        glBindTexture(GL_TEXTURE_2D, tex.get_handle());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, _frames.at(i).data);
    }

    Image<float, 3> VideoFile::get_frame_as_image(size_t frame_i)
    {
        if (frame_i > _n_frames)
        {
            std::stringstream str;
            str << "[ERROR] Trying to access frame with index " << frame_i << " in video file with " << _n_frames << " frames" << std::endl;
            throw std::out_of_range(str.str());
        }

        cache_frames_until(frame_i);
        auto out = Image<float, 3>(_size.x(), _size.y());
        
        auto data = &(_frames.at(frame_i).at<float>(0, 0));

        for (size_t y = 0, i = 0; y < _size.y(); ++y)
        {
            for (size_t x = 0; x < _size.x(); ++x)
            {
                RGB to_push;
                to_push.red() = data[i];
                to_push.green() = data[i + 1];
                to_push.blue() = data[i + 2];

                out(x, y) = to_push;
                i += 3;
            }
        }

        return out;
    }

    void VideoFile::set_frame(size_t frame_i, const Image<float, 3>& image)
    {
        cache_frames_until(frame_i);
        auto data = &(_frames.at(frame_i).at<float>(0, 0));

        for (size_t y = 0, i = 0; y < _size.y(); ++y)
        {
            for (size_t x = 0; x < _size.x(); ++x)
            {
                auto rgb = image(x, y);

                data[i] = rgb.red();
                data[i + 1] = rgb.green();
                data[i + 2] = rgb.blue();

                i += 3;
            }
        }
    }

    void VideoFile::cache()
    {
        cache_frames_until(_n_frames);
    }
}