// 
// Copyright 2021 Clemens Cords
// Created on 08.11.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <exception>

#include <video/video_file.hpp>

#include <opencv4/opencv2/videoio.hpp>

namespace crisp
{
    VideoFile::VideoFile()
        : _capture()
    {}

    void VideoFile::create(std::string path)
    {
        _capture.open(path);

        if (not _capture.isOpened())
        {
            std::stringstream str;
            str << "[ERROR] Unable to open video file at " << path << std::endl;
            throw std::invalid_argument(str.str());
        }

        _n_frames = size_t(_capture.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_COUNT));
        _size.x() = size_t(_capture.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH));
        _size.y() = size_t(_capture.get(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT));
    }

    size_t VideoFile::get_n_frames() const
    {
        return _n_frames;
    }

    size_t VideoFile::get_current_frame() const
    {
        return _current_frame;
    }

    Vector2ui VideoFile::get_size() const
    {
        return _size;
    }

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

    void VideoFile::set_frame(size_t i, Texture<float, 3>& tex)
    {
        cache_frames_until(i);
        auto info = State::get_texture_info(tex.get_handle());
        assert(info.width == _size.x() and info.height == _size.y());

        glBindTexture(GL_TEXTURE_2D, tex.get_handle());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_RGB, &_frames.at(i).at<float>(0, 0));
    }

    void VideoFile::cache()
    {
        cache_frames_until(_n_frames);
    }










}