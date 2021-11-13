[[<< Back to Index]](../index.md)

---

# Hardware Accelerated Image Processing

Benchmarks, Textures, Shaders, Hardware Accelerated Versions of Spatial Filter, Spectral Filters, Morphological Operations, Segmentation, Thresholding, Deep Learning

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

0. [**Foreword**](#0-foreword)<br>
1. [**Motivation & Benchmarking**](#1-motivation--benchmarking)<br>
2. [**Textures**](#2-textures)<br>
    2.1 [Loading Texture](#2-textures)<br>
    2.2 [Saving Textures](#22-saving-textures)<br>
    2.3 [Texture Transforms](#23-modifying-textures)<br>
3. [**GPU-Side Spatial Filters**](#3-spatial-filtering)<br>
4. [**GPU-Side Morphological Transforms**](#4-morphological-transforms)<br>
5. [**GPU-Side Segmentation**](#5-segmentation)<br>
   5.1 [Thresholding](#51-thresholding)<br>
   5.2 [Segmentation](#52-segmentation)<br>
6. [**GPU-Side Spectral Filters**](#6-spectral-filtering)<br>
7. [**GPU-Side PseudoColor**](#7-pseudo-color-mapping)<br>


## 0. Foreword

The inner workings of GPU-side computation on a hardware level can be quite complicated and require a serious amount of knowledge to fully grasp. This tutorial is not intended to introduce GPU-side image processing in general, rather, it will give just enough context for a user unfamiliar with with GPU-side computation but familiar with `crisp` to use the new functions introduced in the GPU-side module. <br>
If you want to learn how to get down in the weeds and do everything yourself, consider reading the graphics card interface tutorial `crisp::State` hosted on the [author's blog](www.clemens-cords.com/posts/gpu_side).

## 1. Motivation & Benchmarking

When reproducing some examples in the past chapters, you may have noticed that the runtime can vary a lot depending on the type of image and that some algorithms can be quite slow. This is not necessarily due to flaws in implementation, rather, some operations are just inherently very costly. Consider an image of size 1920x1080, this image has 2073600 pixels. Each of them has to be allocated, moved from the disk to the ram, transformed into a color, etc. If we now want to operate on the image, each operation will have to be executed at least 2073600 times, sometimes twice as much if a temporary buffer image is needed. 

To illustrate the nature of this behavior, `crisp` offers a very compact benchmarking object:

```cpp
template<typename Function_t>
struct Benchmark
{
    Benchmark(Function_t&& lambda);
    
    template<typename... Args_t>
    float execute(size_t number_of_cycles, Args_t...);
}
```

When creating the object, we hand it a lambda capture, which is saved in the benchmark object. When we call `execute`, we forward the arguments to the lambda and lambda is then itself executed `number_of_cycles` many times. The time for the lambda to return is measure and the average time per cycle in microseconds is returned.

Using our familiar image of a bird:<br>

![](.resources/color_opal.png)<br>

We want to measure the time it takes to create a deep-copy of this image. This image is `483*483 = 233289` pixels in size and each pixel has three, 32-bit floats (one for each RGB color plane) so at least `233289 * 32 * 3` bits or about 2.8mb have to be moved around in memory (as image in crisp are uncompressed while in RAM).

```cpp
#include <benchmark.hpp>

auto image = load_color_image(get_resource_path() * "/docs/hardware_acceleration/.resources/color_opal.png");

auto copy_benchmark = Benchmark([&](){
    volatile auto deep_copy = image;
});

std::cout << copy_benchmark.execute(1000) << "μs" << std::endl;
```

Here we're creating a lambda that allocates a copy of the image, then deep-copies each pixel value into it. The `volatile` specifier here is used to prevent the compiler from "optimizing out" the unused variable.<br>
We execute the benchmark 1000 times and print the average time per cycle to the console. On this machine, the benchmark reports a time of `86404.3 μs` on average, which is about `0.09s`. This doesn't sound that bad, but it is relatively slow considering the relatively low resolution of the image. If our application runs at 60fps, we could only copy two images per frame, even if no other operation was taking place at the same time.

To address this runtime issue, `crisp` offers what is basically a high-performance version of images called *textures*. Handling of textures is done on the graphics card, which is optimized for such operations on a hardware level. We will learn more about how to create texture and how they work exactly soon, but for now we just want to see how much faster it is:

```cpp
#include <benchmark.hpp>
#include <gpu_side/texture.hpp>

auto image = load_color_image(get_resource_path() * "/docs/hardware_acceleration/.resources/color_opal.png");

// create a RGB texture from the image
auto texture = Texture<float, 3>(image);

auto copy_benchmark = Benchmark([&](){
    volatile auto texture_deep_copy = texture;
});

std::cout << copy_benchmark.execute(1000) << "μs" << std::endl;
```
Here we're first loading the image, then creating a texture from the image. Inside the lambda, we then deep-copy this texture once per cycle. The benchmark reports an average time (in microseconds) of:

```
2.675 μs
```

That's... a lot faster. This result is not erroneous, each call did indeed allocate a new image of the same size and copy all data from the original into the new image, the difference is that it all happened in the graphics card's memory. Comparing 2.675 microseconds to 86404.3 microseconds, we get a sense of just how much faster GPU-side computation is. Now, dealing with many images during a single frame at 60fps sounds very feasible.

# 2. Textures

## 2.1 Loading Textures

Textures are the GPU-side equivalent of images. Similar to images, they have a *value type* and a number of *planes*. The template for textures looks familiar:

```cpp
// in gpu_side/texture.hpp

template<typename T, size_t N>
class Texture
{
    static_assert(std::is_same_v<T, float> std::is_same_v<T, bool>);
    static_assert(1 <= N and N <= 4);

    /* ... */
}
```

We see that textures, like multi-plane images, take two template parameters: the value type `T` and the number of planes `N`. Unlike images, however, we are somewhat limited here. There are only two possible value types: `bool` and 32-bit `float`. Furthermore, we can only have up to a maximum amount of 4 planes. This is because only textures of these types fully benefit from hardware-acceleration and are supported by the graphics card, if we need a higher number of planes we either need to use `crisp::Image` or represent the image using multiple textures.

We can create a texture from an image like so:

```cpp
// create render window
auto render_window = /* ... */;
render_window.set_active(true);

// create texture from image
auto image = crisp::load_color_image(get_resource_path() + "docs/hardware_acceleration/.resources/color_opal.png");
auto texture = Texture<float, 3>(image);

// bind texture to window
State::bind_texture(NONE, "_texture", texture.get_handle());

// display the texture on the screen
State::display();
```

![](.resources/texture_window.png)<br>

As expected, rendering the texture, we see that it is identical to our by now familiar image of a bird.

When creating the texture, we need to make sure that it has the same number of planes as the original image, otherwise a static assertion is raised.

We now want to create a 1-plane texture using the grayscale version of our image of a bird:

```cpp
auto grayscale = crisp::load_grayscale_image(get_resource_path() + "docs/hardware_acceleration/.resources/color_opal.png");
auto grayscale_texture = Texture<float, 1>(image);

// render
State::bind_texture(NONE, "_texture", grayscale_texture.get_handle());
State::display();
```

![](.resources/texture_red.png)<br>

Instead of a grayscale image, we see that the image is appears red. This is correct behavior, when rendering a texture to the screen, it is always first converted into rgba format. Since we only have one plane in the actual data of the texture, the graphics card sets the red component to our intensity data and keeps all other components at 0. This is why when working with less than 3 or 4-plane images where the 4th component is not the alpha component can be quite confusing when rendering them to the screen. It is important to always remember that in memory, the data layout is correct and the confusion only arises once the data is rendered to the screen and displayed as if it was in rgba.

# 2.2 Saving Textures

We often want to save our texture to the disk, however this is not possible directly. We first need to move the texture back from the graphics card's memory into RAM. The easiest way to do this is to just convert it back to an image and save that image as usual:

```cpp
Texture<float, 3> texture = /* ... */
        
auto as_image = texture.to_image(); 
// image will be Image<float, 3>

save_to_disk(as_image, "path/to/image.png");
```

Textures can be quite inflexible, so it is sometime necessary to move them back and forth between graphics card memory and RAM to get the full functionality of `crisp` like this. While this is expensive, it can sometimes be worth it, as the increase in hardware acceleration can make up that lost time quite easily.

# 2.3 Modifying Textures

While textures sport a lot of the same functions as `crisp::Image` such as `get_size`, and move- and copy assignment/construction, we have no way to modify the texture directly once it is loaded into the graphics card's memory. All modification has to be done through `crisp::State` or free functions supplied by `crisp`. It is therefore recommended for most users to do any manual modification to the image *before* exporting it as a texture, however we will go to some of the functions offered here:

(this feature is not yet implemented)

# 3. Spatial Filtering

With the release of the GPU-side module, `crisp::SpatialFilter` has a new member function overload:

```cpp
// old, still exists
template<typename Image_t>
void apply_to(Image_t&);

// new
template<typename T, size_t N>
void apply_to(Texture<T, N>&);
```

All other function work exactly the same. Let's say we want to gaussian blur the image with a 3x3 kernel. We measure the performance of doing so with both an image and a texture:

```cpp
auto filter = SpatialFilter();
filter.set_kernel(filter.gaussian(3));

auto filter_image = Benchmark([&](){
    filter.apply_to(image);
});

auto filter_texture = Benchmark([&](){
    filter.apply_to(texture);
});

std::cout << "image: " << filter_image.execute(100) << "μs" << std::endl;
std::cout << "texture: " << filter_texture.execute(100) << "μs" << std::endl;
```
```
image: 1.701680.0μs
texture:   4410.4μs
```

We note a vast performance increase when using textures. Indeed, a CPU-side spatial filter triples its runtime when working with color images compared to grayscale images. While this is technically true GPU-side, the actual impact of this is negligible, meaning a 4-plane image is processed in about the same time as 1-plane image. 

Inspecting the texture after a single blurring cycle:

```cpp
filter.apply_to(texture);

State::bind_texture(NONE, "_texture", texture.get_handle());
State::display();
```

![](.resources/blurred.png)<br>

We confirm that the texture has indeed been blurred slightly. 

A word on optimization, GPU-side filtering is fastest with kernels of size `n*m` where `n, m in {2, 3, 4}`. For larger kernels, significant overhead can be introduced and for very large kernels (`n, m > 100`), the performance increase may not be significant enough to warrant GPU-side computation at all.

# 4. Morphological Transforms

Similar to spatial filters, `crisp` now also supports morphological operations on textures. With the GPU-side module, the following functions have been added to `crisp::MorphologicalTransform`:

```cpp
/// erode
template<typename T, size_t N>
void erode(Texture<T, N>& texture);

/// geodesically erode
template<typename T, size_t N>
void erode(Texture<T, N>& image, const Texture<T, N>& mask);

/// dilate
template<typename T, size_t N>
void dilate(Texture<T, N>& texture);

/// geodesically dilate
template<typename T, size_t N>
void dilate(Texture<T, N>& image, const Texture<T, N>& mask);

/// erode, then dilate
template<typename T, size_t N>
void open(Texture<T, N>& texture);

/// dilate, then erode
template<typename T, size_t N>
void close(Texture<T, N>& texture);
```

We recognize these functions as the texture-versions of the most common morphological operations. We can again illustrate their performant nature using a small benchmark:

```cpp
auto texture = Texture<float, 3>(image);

auto transform = MorphologicalTransform();
transform.set_structuring_element(transform.all_foreground(4, 4));

auto filter_image = Benchmark([&](){
    transform.open(image);
});

auto filter_texture = Benchmark([&](){
    transform.open(texture);
});

std::cout << "image: " << filter_image.execute(100) << "μs" << std::endl;
std::cout << "texture: " << filter_texture.execute(100) << "μs" << std::endl;
```
```
image:  9385510.0μs
texture:   6261.3μs
```
Once again, the performance increase is significant. If you are curious what the texture looks like after being opened 100 times:

![](./.resources/opened.png)<br>

# 5. Segmentation

## 5.1 Thresholding

Two new thresholding functions were added with the release of the GPU-side module:

```cpp
template<typename T, size_t N>
Texture<T, N> manual_threshold(const Texture<T, N>&, float threshold);

template<typename T, size_t N>
Texture<T, N> local_threshold(const Texture<T, N>&, size_t neighborhood_size, size_t correction);
```

`manual_threshold` is self-explanatory, it works just like the CPU-side version. `local_threshold` replaces all other thresholding operations for textures, it is specifically designed to be extremely fast and highly resistant to non-uniform lightning. Recall our corrupted image from the [segmentation tutorial](../segmentation/segmentation.md):

![](../segmentation/.resources/non_uniform.png)<br>

Filtering this image with CPU-side otsu's method showed significant corruption due to the non-uniform lighting:

![](../segmentation/.resources/otsu_02.png)<br>

However even using a thresholding method that was specifically designed for non-uniform lighting, the result was not quite perfect:

![](../segmentation/.resources/neighborhood_01.png)<br>

While the text was succesfully isolated, the background still displays a large amount of noise. Let's see how the new GPU-side algorithm performs in comparison.

Transforming the original image into a 1-plane texture, we skip comparing the runtime, just like before the GPU-side computation is far faster. 

The `local_threshold` takes two arguments:

+ `neighborhood_size`: is a measure of the area the local mean is computed in. The larger the size, the more resistant the algorithm is to non-uniform lighting
+ `correction`: correction applies an erode-like noise reduction step at the end of the thresholding operation for no additional cost

```cpp
auto image = load_grayscale_image(get_resource_path() + "docs/segmentation/.resources/non_uniform.png")
auto texture = Texture<float, 1>(image);

auto thresholded = Segmentation::local_threshold(texture, 7, 0);
```

![](.resources/threshold_c0.png)<br>

We note that the "SAMPLE" text was isolated flawlessly with minimal artifacting. Overall the GPU-side thresholding operation is optimized for edge detection and in this case both the bird, text, and straw the bird is perching on were clearly defined and isolated. This thresholding operation was conducted with a neighborhood-size scale factor of 7 and a correction of 0. Setting the correction to 1 we get the following:

![](.resources/threshold_c1.png)<br>

We note that the shapes were smoothed out and much of the noise of the background is gone. If we raise the correction further to let's say 4, the image starts to become negatively effected:

![](.resources/threshold_c4.png)<br>

Parts of the bird were thinned to the point that edges are now missing. 

Textures only have a single thresholding operation because it is very flexible. By fine-tuning the two parameters, a vast array of images can be successfully thresholded, especially if the intention is to highlight solid shapes like letters or regions of a single color. 

Recommended values for the neighborhood size scale factor are `{1, 2, 3, ..., 25}`, while the correction should be kept lower than `5`. 

## 5.2 Segmentation

(this feature is not yet implemented)

# 6. Spectral Filtering

While it is currently (as of `crisp` 0.9) not possible to fourier transform GPU-side, it is possible to *filter the spectrum* GPU side. With the new gpu-side module, `crisp::FrequencyDOmainFilter` now has a new template argument:

```cpp
template<typename side = CPU_SIDE>
class FrequencyDomainFilter;
```

`FrequencyDomainFilter<CPU_SIDE>` works just like before, however `FrequencyDomainFilter<GPU_SIDE>` is a partial specialization that offers a subset of the functions of the CPU-side version:

```cpp
struct FrequencyDomainFilter<GPU_SIDE>
{
    // create from size
    FrequencyDomainFilter(size_t width, size_t height);
    
    // create from spectrum by measuring size
    template<FourierTransformMode Mode>
    FrequencyDomainFilter(const FourierTransform<Mode>&);
    
    // apply to spectrum GPU-side
    template<FourierTransformMode Mode>
    void apply_to(FourierTransform<Mode>&) const;
    
    // set offset, symmetry always enforced
    void set_offset(double x_dist_from_center, double y_dist_from_center);
    
    // all filter shaping functions are of course available
    void as_ideal_lowpass(float, float, float);
    void as_gaussian_lowpass(float, float, float);
    void as_butterworth_lowpass(float, size_t, float, float);
    etc.
}
```

We see that while the functionality in terms of constructors and the apply function is the same, we can no longer manually edited specific parts of the filter using `operator()(size_t, size_t)`. Furthermore, we can no longer combine filters using arithmetic operations. However, this trade-off will be worth it if the performance-increase is significant enough.

We write a small benchmark that simply creates a band pass filter and applies it to the fourier spectrum. We use the above shown image of a bird which is of size `483x483`, thus the fourier spectrum will have `2*483 x 2*483 = 933156` coefficients (pixels):

```cpp
auto texture = Texture<float, 1>(image);
auto spectrum = FourierTransform<SPEED>();
spectrum.transform_from(image);

auto cpu_side = Benchmark([&](){
    auto filter = FrequencyDomainFilter<CPU_SIDE>(spectrum);
    filter.as_gaussian_bandpass(0.1, 0.3, 1, 0);
    filter.apply_to(spectrum);
});

auto gpu_side = Benchmark([&](){
    auto filter = FrequencyDomainFilter<GPU_SIDE>(spectrum);
    filter.as_gaussian_bandpass(0.1, 0.3, 1, 0);
    filter.apply_to(spectrum);
});

std::cout << "cpu-side: " << cpu_side.execute(10) << "μs" << std::endl;
std::cout << "gpu-side: " << gpu_side.execute(10) << "μs" << std::endl;
```

We choose the `SPEED` configuration of the fourier transform because, as mentioned in the [frequency domain tutorial](../frequency_domain/frequency_domain_filtering.md), it is the only configuration to be feasible to run in real-time on mid-end systems. The benchmark reports the following times:

```
cpu-side: 1391010μs
gpu-side:    4197μs
```
Once again, the performance increase is extremely significant.

Note that we can simulate combining different filter shapes by copying the spectrum, apply one shape to the first, a second shape to the other and then combining the result using the functions noted in section 2.3. While this may sound cumbersome, it is still far faster than doing the entire process CPU-side. If we want optimum performance, however, we will have to write a custom shader that implements the desired filter shape and apply the shader directly.

# 7. Pseudo Color Mapping

When using Pseudo Color to make an image easier to visually parse for humans, we won't be working with a static image but with a video (usually the video stream of a medical instrument, for example). Because of this real-time requirement, speed is paramount for a fluid user experience. To achieve this, `crisp` offers a GPU-side version of pseudo colors:

```cpp
/// old, cpu-side (c.f. color tutorial)
template<>
class PseudoColor<CPU_SIDE> { // unchanged
}

/// new:
template<>
class PseudoColor<GPU_SIDE> 
{
    PseudoColor();
    
    Texture<float, 3> apply_to(const Texture<float, 1>&) const;
    Texture<float, 3>& apply_in_place(Texture<float, 3>&) const;
    
    void add_value_to_hue(float gray, float hue);
    void add_value_range_to_hue(float min_gray, float max_gray, float hue);
    void add_value_range_to_hue_range(float min_gray, float max_gray, float min_hue, float max_hue);
    void add_value_range_to_inverse_hue_range(float min_gray, float max_gray, float min_hue, float max_hue);
}
```

We first note that `PseudoColor<GPU_SIDE>` is no longer a static-only class, we now need to instance it before usage. We do recognize most of the functions as analogues to `PseudoColor<CPU_SIDE>::RangeMapping` and indeed they work in exactly the same, `PseudoColor<GPU_SIDE>` can be seen as an extension of the instanced range mapping concept. 

```cpp
// instance
auto pseudo_color = PseudoColor<GPU_SIDE>();

// add ranges
pseudo_color.add_value_range_to_hue_range(0, 0.3, 0, 1);
```

Now that we configured our mapping to map gray-values from [0, 0.3] to hue values [0, 1] we apply it to a texture. As shown earlier, two functions are offered for this:

+ `apply_to` takes a const reference to a grayscale texture and returns a newly allocated texture. This leaves the old texture unmodified but decreases performance as a new texture of the same size and higher dimensionality has to be allocated
+ `apply_in_place` takes a non-const reference to a color texture. This function interprets the texture as grayscale and modifies it directly. No allocation has to take place, making this method extremely fast.

```cpp
auto grayscale = Texture<float, 1>(/* ... */);
auto now_color = pseudo_color.apply_to(grayscale);  // grayscale unchanged

auto color = Texture<float, 3>(/* ... */);
pseudo_color.apply_in_place(color);                 // color modified
```



