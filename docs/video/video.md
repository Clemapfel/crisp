[[<< Back to Index]](../index.md)

---

# Videos

Video Files, Video Streams, Frame-by-Frame Processing

```cpp
#include <video/video_file.hpp>
#include <video/video_stream.hpp>
```

## Table of Contents

## 1. Motivation

In modern application we really need to process single images, more often it's about processing a library of images or a *video*. A video is a series of images, each image is called a *frame*. In `crisp`, the frames are completely independent and uncompressed, a video is literally just an ordered container of images. Behind the scenes there's a lot of conversion and decompression going on but for `crisp` users, all of this is handled automatically. 

In this tutorial, when talking about a *video file*, we mean a video for which all frames are available at the time of loading. On the contrary, a `video stream` is a video for which not all frames are availbable immediately, for example they may be in the process of being send over the internet or being recorded with a camera.

![](../../.test/goose.gif)

## 2. Video Files

## 2.1 Access Frames

We can open a video file like so:

```cpp
#include <video/video_file.hpp>

// in main
auto video = VideoFile();
video.load(get_resource_path() + "/docs/video/horse.mp4")
```

This operation is very fast because video frames are only allocated when needed. We can access an individual frame like so:

```cpp
auto frame = video.get_frame(10);
```

This operation returns a rgb texture `Texture<float, 3>`. Often when working with videofile we want to work in real-time, this makes the superior performance of textures necessary. If we want to work in an offline manner, we can also access the frame as an image like so:

```cpp
auto frame_as_image = video.get_frame_as_image(10);
```

Note that before either `get_frame` operation is called, the frames are stored in an unrelated format in memory. This means they first need to be converted into a texture (by simply sending the memory to the graphics card) or into an image.

## 2.2 Modifying Frames

Once we accessed the frame we can modify it like any image. After we did, we can modify the video files memory by calling `set_frame`:

```cpp
{
    auto frame = video.get_frame(10);
    /* do frame modification here */
    video.set_frame(10, frame);
}
```
We add the `{}` as a block statement to assure that `frame` will go out of memory after `set_frame` resolves.
If we want to work with images instead of textures, `VideoFile` offers an overload of `set_frame` that works the same.


## 2.3 Exporting the Video File

Let's say we want to convert the entire video into a file that 

