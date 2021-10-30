[[<< Back to Index]](../index.md)

---

# Hardware Acceleration and GPU-side execution

Textures, Shaders, Graphics Card Interface, Hardware Accelerated Versions of Spatial Filter, Spectral Filters, Morphological Operations

```cpp
#include <gpu_side/textures.hpp>
#include <gpu_side/texture_workspace.hpp>
#include <gpu_Side/state.hpp>
#include <gpu_side/is_gpu_side.hpp>
#include <gpu_side/native_handle.hpp>

// all of the above collected in:
#include <gpu_side.hpp>

// separate:
#include <benchmark.hpp>
```

## Table of Contents

## 1. Benchmarks

When reproducing some of the examples in the past chapters, you may have noticed that it's runtime can vary and that some algorithms are quite slow. This is not necessarily due to the implementation, rather, some operations are just inherently very costly. Consider an image of size 1920x1080, this image has 2 073 600 pixels. Each of them has to be allocated, move from the disk to the ram, transformed into a color, etc.. If we now want to operate on the image, each operation will have to be executed 2073600 times. 

To investigate the nature of this behavior, `crisp` offers a very compact benchmarking object:

```cpp
template<typename Function_t>
struct Benchmark
{
    Benchmark(Function_t&& lambda);
    
    template<typename... Args_t>
    float execute(Args_t...);
}
```

When creating the object, we hand it a lambda which is saved in the benchmark object. When we call `execute`, we forward the arguments to the lambda and lambda is then itself executed. The benchmark object calculates the time it takes for the lambda to return and reports the time in microseconds.

Using our familiar image of a bird:<br>

![](.resources/color_opal.png)<br>

We want to measure the time it takes to create a deep-copy of this image. The image is 483*483 = 233289 pixels in size and each pixel has three, 32-bit floats (one of each RGB color plane).

```cpp





