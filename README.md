# CRISP: Clems Real-Time Interactive Signal Processing Library

## What is `crisp`?
`crisp` is a library that implements most major image-processing needs. This includes:
+ Spatial Filtering
+ Morphological Transform
+ Fourier Transform
+ Filter Design
+ Automated Kernel Seperation
+ Superpixel and k-Means Clustering
+ Edge Detection
+ Feature Extraction
+ Boundary and Texture Descriptors
+ Color Representations
+ Noise Modelling
+ Intensity Transforms
+ Histogram Equalization & Specialization
+ Wavelet Transforms
+ Bitplane Decomposition

## Why `crisp`?

`crisp` is **visual**<br>
Render anything! Really, *anything*? <br>
It's called visual computing, afterall. Here's a non-exhaustive list of things you can draw on your screen or save to an image at any time:

+ `crisp::Image` of any type
+ `crisp::FourierTransform`
+ `crisp::FrequencyDomainFilter`
+ `crisp::Kernel`
+ `crisp::StructuringElement`
+ `crisp::Histogram`
+ `Eigen::Matrix<T, /*...*/>`
+ and more!

This makes debugging, filter design or just understanding/explaining your own algorithms much easier and more intuitive.

`crisp` is **well documented**<br>
+ [full tutorials](#tutorial) are available for all features
+ examples with images are provided whenever possible
+ in-line documentation for IDE support
+ user-facing code is polished to a mirror sheen
+ language is kept simple, aimed at people without a math degree

`crisp` is **math-less**<br>
+ Algorithms choose their own parameters heuristically
    - no more trolling through papers trying to find out what parameter `α` does.<br>
+ complex things like k-means clustering or creating the fourier transform of an image can be done with just one line of code

`crisp` is **generic**
+ filter, segment, cluster in any number of dimensions on any image value type
+ convert any image type to any other image type
+ convert any color representation to any other color representation
+ compare/combine vectors with vectors, vectors with scalars

`crisp` is **portable**<br>
+ supports Mac, Linux and Windows
+ runs on either ram/cpu or vram/gpu

`crisp` is **interactive**<br>
All SFML functionality is included, this means keyboard/mouse support, rendering to a window, sound, etc.

## Installation
### Dependencies
+ C++20 or newer
+ [gcc10](https://gcc.gnu.org/install/download.html) or newer 
+ [OpenGL 3.2](https://developer.nvidia.com/opengl-driver) or newer
+ [cmake](https://cmake.org/install/)
+ [Eigen](https://eigen.tuxfamily.org/dox/GettingStarted.html)
+ [SFML](https://www.sfml-dev.org/tutorials/1.6/start-linux.php)
+ [fftw3](https://www.fftw.org/) (included)

### Installation
`crisp` is header only, to add it to your project, simply use:

```bash
TODO
```

Add the corresponding include path to your cmake script and link against `crisp`:
```cmake
include_directories(/path/to/.../crisp/include/)
add_executable(MY_EXECUTABLE path/to/my/main.cpp)
target_link_libraries(MY_EXECUTABLE crisp)
```

## Tutorial

Consider reading the tutorials in order, this will teach important principles of image processing and `crisp` step-by-step:

1. [System](./docs/system/system.md)
3. [Vector](./docs/vector/vector.md) (optional)
4. [Images](./docs/image/image.md)
2. [Color](./docs/color/color.md) (optional)
6. [Noise](./docs/noise/noise.md) (optional)
7. [Spatial Domain Filters](./docs/spatial_filters/spatial_domain_filtering.md)
8. [Morphological Transform](./docs/morphological_transform/morphological_transform.md)
9. [Frequency Domain Filters](./docs/frequency_domain/frequency_domain_filtering.md)
10. [Segmentation](./docs/segmentation/segmentation.md)
11. [Feature Extraction](./docs/feature_extraction/feature_extraction.md)

## Documentation

Documentation is stored as interactive markdown files, right here on github. An index for easy navigation can be found [here](./docs/index.md) or in `/crisp/docs/index.md`.








