[[<< Back to Index]](../index.md)

---

# Videos

Video Files, Video Streams, Frame-by-Frame Processing

```cpp
#include <video/video_file.hpp>
#include <video/video_stream.hpp>
```

## Table of Contents

1. [**Motivation**](#1-motivation)<br>
2. [**Video Files**](#2-video-files)<br>
    2.1 [**Accessing Frames**](#21-accessing-frames)<br>
    2.2 [**Modifying Frames**](#22-modifying-frames)<br>
    2.3 [**Exporting the File**](#23-exporting-the-video-file)<br>
    2.3 [**Example**](#24-an-example)<br>
3. [**Video Streams**](#3-video-streams)<br>

## 1. Motivation

In modern application we rarely need to process single images, more often it's about processing a library of images or a *video*. A video is a series of images, each image is called a *frame*. In `crisp`, the frames are completely independent and uncompressed, a video is literally just an ordered container of images. Behind the scenes, there's a lot of conversion and decompression going on, but for `crisp` users, all of this is handled automatically. 

In this tutorial, when talking about a *video file*, we mean a video for which all frames are available at the time of loading. A *video stream*, then, is a video for which not all frames are available immediately. For example, they may be in the process of being sent over the internet or being recorded with a camera.

## 2. Video Files

We will be working with the following video:

![](./goose_starting.gif)<br>

This video is store in the *gif* format, this format was chosen because it is easy to embed in markdown files such as this. Video file formats are a tricky subject, `crisp` uses the [OpenCV](https://docs.opencv.org/3.4/d0/da7/videoio_overview.html) video file interface, which itself is partially based on [FFMPEG](https://www.ffmpeg.org/). When reading files with `crisp`, many common formats are supported, however for writing video files, as of `crisp` version 1.0 (or earlier), only `.mp4` is supported.

In any case, we open the video file like so:

```cpp
#include <video/video_file.hpp>

// in main
auto video = VideoFile();
video.load(get_resource_path() + "/docs/video/goose_starting.gif")
```

This operation is very fast because video frames are only loaded and allocated when accessed, similar to *lazy evaluation*.

We can access basic information about the file using the following member functions:

```cpp
std::cout << "Number of Frames: " << video.get_n_frames() << std::endl;
std::cout << "Resolution: " << video.get_size() << std::endl;
std::cout << "Frames per Second: " << video.get_fps() << std::endl;
```

```
Number of Frames: 13
Resolution: 381 205
Frames per Second: 16
```

## 2.1 Accessing Frames

We can access an individual frame like so:

```cpp
auto frame = video.get_frame(10);
```

This operation returns a RGB texture `Texture<float, 3>`. Because video processing often happens in real-time, it is preferable to work with textures which benefit from hardware acceleration. If this functionality is not needed, we can load the frame as an image instead:

```cpp
auto frame_as_image = video.get_frame_as_image(10);
```

![](.resources/frame_10.png)

Note that before either `get_frame` version is called, the frames are stored in an unrelated format in memory. This means they first require to be converted into a `crisp::Texture` (by sending the memory to the graphics card) or into a `crisp::Image`. It is therefore preferable to only call `get_frame` once and storing the frame manually, as every time we call `get_frame`, this triggers reallocation and conversion.

## 2.2 Modifying Frames

Once we accessed the frame, we can modify it like any image. We can then modify the video file itself by calling `set_frame`:

```cpp
{
    auto frame = video.get_frame(10);
    /* do frame modification here */
    video.set_frame(10, frame);
}
```
We add the `{}` as a block statement to assure that `frame` will go out of scope after `set_frame` resolves, automatically deallocating it.
If we want to work with images instead of textures, `VideoFile` offers an overload of `set_frame` that works just the same.

## 2.3 Exporting the Video File

Once we modified the file, we can export it. `VideoFile` offers two functions for this:

+ `save(std::string path)` saves the current state of the entire file
+ `save(std::string path, size_t first_frame, size_t last_frame)` only saves part of the file, starting at frame `first_frame` and up to (but excluding) `last_frame`

If we only modified part of the file, let's say frames 0 to 10 (where the original file has 13 frames), then if we call the first overload the resulting file will be 13 frames long where 10 of them are the modified ones, 3 of them are kept as original.

## 2.4 An example

As a practical example, let's modify our video. For no specific reason, we'll compute the gradient magnitude of each frame, then export the video:

```cpp
#include <video/video_file.hpp>
#include <gpu_side/whole_texture_transform.hpp>

// in main

// load
auto video = VideoFile();
video.load(get_resource_path() + "/docs/video/goose_starting.gif");

// iterate through frames
for (size_t i = 0; i < video.get_n_frames(); ++i)
{
    // get frame
    auto frame = video.get_frame(i);
    
    // modify frame to gradient magnitude
    compute_gradient_magnitude(frame);
    
    // save frame to video file
    video.set_frame(i, frame);
}

// export as .mp4
video.save(/* ... */ + "/gradient.mp4");
return 0;
```

![](.resources/gradient.gif)<br>

As you can see, working with video files isn't much different to working with images.

# 3. Video Streams

(this feature is not yet implemented)









