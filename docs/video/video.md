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

A video is a series of images, each image is called a *frame*. In `crisp`, the frames are completely independent and uncompressed, a video is literally just a vector of images in a specific order. Behind the scenes there's a lot of conversion and decompression going on but for `crisp` users, all this is handled automatically. 

In this tutorial, when talking about a *video file*, we mean a video for which all frames are available at the time of construction. On the contrary, a `video stream` is a video for which not all frames are availbable immediately, for example they may be in the process of being send over the internet or being recorded on a divice with a camera. 

A word on memory, videos are extremely large in RAM (or VRAM) owning to the fact that they are uncompressed and may be quite long. A simple 10min video at 60fps will have around 36000 RGB-images which would easily fill most mid-ends machines RAM. To address this, `crisp` uses a *cache*. The cache stores all frames currently in ram, before we can access a frame we first need to load it into the cache and when we don't need the frame anymore, it is advised to erase it from the cache. We need to keep this behavior in mind to understand how to properly manage memory allocation.

## 2. Video Files

We can open a video file like so:

```cpp
#include <video/video_file.hpp>

// in main
auto video = VideoFile();
video.load(get_resource_path() + "/docs/video/horse.mp4")
```

This operation is very fast because at the moment, the video files cache is empty.

