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
    {

    }

    void VideoFile::cache_frames_until(size_t i)
    {
        while (i >= _current_frame and _current_frame < _n_frames)
        {
            _frames.emplace_back();
            auto& frame = _frames.back();
            _capture.grab();
            _capture.retrieve(frame);
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB, 3);
            cv::normalize(frame, frame, 0.0, 1.0, cv::NORM_MINMAX, CV_32F);
            cv::flip(frame, frame, 0);
            _current_frame++;
        }

        if (_current_frame >= _n_frames)
            _capture.release();
    }

    auto VideoFile::get_fourcc(std::string str)
    {
        std::string format;
        for (auto it = str.end()-1; (*it) != '.' and it != str.begin(); it--)
            format.insert(format.begin(), *it);

        for (auto& f : format)
            f = std::tolower(f);

        format.insert(format.begin(), '.');

        if (format == ".mp4" or format == ".mp4v")
            return cv::VideoWriter::fourcc('m', 'p', '4', 'v');
        else if (format == ".mp4a")
            return cv::VideoWriter::fourcc('m', 'p', '4', 'a');
        else
        {
            std::cerr << "[WARNING] File format " << format << " is currently not supported. Please export the file as .mp4 instead" << std::endl;
            return -1;
        }
    }

    void VideoFile::load(std::string path)
    {
        _capture.open(path);

        if (not _capture.isOpened())
        {
            std::stringstream str;
            str << "[EXCEPTION] Unable to open video file at " << path << std::endl;
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
        out.open(path, get_fourcc(path), _fps, cv::Size(_size.x(), _size.y()));
        if (not out.isOpened())
        {
            std::stringstream str;
            str << "[WARNING] Unable to write video file to " << path << std::endl;
            str << "[LOG] No video file saved." << std::endl;
            return;
        }

        cache_frames_until(_n_frames);

        bool once = true;
        for (size_t i = 0; i < _frames.size(); ++i)
        {
            if (_released.find(i) != _released.end() and once)
            {
                std::cerr << "[WARNING] Writing one or more frames even though they have already been freed." << std::endl;
                once = false;
            }

            cv::Mat& frame = _frames.at(i);
            cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR, 3);
            cv::normalize(frame, frame, 0.0, 255, cv::NORM_MINMAX, CV_8U);
            cv::flip(frame, frame, 0);
            out.write(frame);
        }
    }

    void VideoFile::save(std::string path, size_t first_frame, size_t last_frame)
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

        cache_frames_until(last_frame);

        if (first_frame >= _n_frames or last_frame >= _n_frames or first_frame > last_frame)
        {
            std::stringstream str;
            str << "[ERROR] first and/or last frame out of range. Assure that first_frame < last_frame.";
            throw std::out_of_range(str.str());
        }

        bool once = true;
        for (size_t i = first_frame; i < last_frame; ++i)
        {
            if (_released.find(i) != _released.end() and once)
            {
                std::cerr << "[WARNING] Writing one or more frames even though they have already been freed." << std::endl;
                once = false;
            }

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
        if (i >= _n_frames)
        {
            std::stringstream str;
            str << "[EXCEPTION] Trying to access frame with index " << i << " in video file with " << _n_frames << " frames" << std::endl;
            throw std::out_of_range(str.str());
        }

        if (_released.find(i) != _released.end())
        {
            std::stringstream str;
            str << "[EXCEPTION] Trying to access frame " << i << " even though it has already been freed.";
            throw std::out_of_range(str.str());
        }

        cache_frames_until(i);
        return Texture<float, 3>(State::register_texture<float, 3>(_size.x(), _size.y(), &(_frames.at(i).at<float>(0, 0))));
    }

    void VideoFile::set_frame(size_t i, const Texture<float, 3>& tex)
    {
        if (_released.find(i) != _released.end())
        {
            std::stringstream str;
            str << "[EXCEPTION] Trying to modify frame " << i << " even though it has already been freed.";
            throw std::out_of_range(str.str());
        }

        cache_frames_until(i);
        auto info = State::get_texture_info(tex.get_handle());
        assert(info.width == _size.x() and info.height == _size.y());

        glBindTexture(GL_TEXTURE_2D, tex.get_handle());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, &(_frames.at(i).at<float>(0, 0)));
    }

    Image<float, 3> VideoFile::get_frame_as_image(size_t frame_i)
    {
        if (frame_i > _n_frames)
        {
            std::stringstream str;
            str << "[EXCEPTION] Trying to access frame with index " << frame_i << " in video file with " << _n_frames << " frames" << std::endl;
            throw std::out_of_range(str.str());
        }

        if (_released.find(frame_i) != _released.end())
        {
            std::stringstream str;
            str << "[EXCEPTION] Trying to access frame " << frame_i << " even though it has already been freed.";
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

                out(x, _size.y() - y) = to_push;
                i += 3;
            }
        }

        return out;
    }

    void VideoFile::set_frame(size_t frame_i, const Image<float, 3>& image)
    {
        if (_released.find(frame_i) != _released.end())
        {
            std::stringstream str;
            str << "[EXCEPTION] Trying to modify frame " << frame_i << " even though it has already been freed.";
            throw std::out_of_range(str.str());
        }

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

    void VideoFile::release_frame(size_t i)
    {
        if (i >= _frames.size() or _released.find(i) != _released.end())
            return;

        _frames.at(i).resize(0, 0);
        _released.insert(i);
    }
}