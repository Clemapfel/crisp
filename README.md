# CRISP: Real-Time Interactive Signal Processing Library (v0.6.2)

## What is `crisp`?
`crisp` is a library implementing most image-processing needs from scratch. This includes:
+ Spatial Filtering
+ Morphological Transform
+ Fourier Transform & Spectral Filtering
+ Edge Detection
+ Segmentation & Clustering
+ Feature Extraction
+ Boundary and Texture Descriptors
+ Fully Connected Neural Networks
+ Convolutional Neural Networks
+ Bayes Classification 
+ SIFT & Key Point Classification
+ Color Representations
+ Noise Modelling
+ Histogram Equalization & Specialization
+ Wavelet Transforms
+ Bitplane Decomposition & Watermarking

## Why `crisp`?

`crisp` is **visual**<br>
Render anything! Really, *anything*? <br>
It's called visual computing, after all. Here's a non-exhaustive list of things you can draw on your screen or save to an image at any time:

+ `crisp::Image` of any type
+ `crisp::FourierTransform`
+ `crisp::FrequencyDomainFilter`
+ `crisp::Kernel`
+ `crisp::StructuringElement`
+ `crisp::Histogram`
+ `Eigen::Matrix<T, /*...*/>`
+ `crisp::FullyConnectedNeuralNetwork`
+ `crisp::ConvolutionalNeuralNetwork`
+ and more!

This makes debugging, filter design or just understanding/explaining your own algorithms much easier and more intuitive.

`crisp` is **well documented**<br>
+ [full tutorials](#tutorial) are available for all features
+ examples with images are provided whenever possible
+ in-line documentation for IDE support
+ user-facing code is polished to a mirror sheen
+ language is kept simple, aimed at people without a math degree

`crisp` is **generic**
+ filter, segment, cluster in any number of dimensions, any image value type
+ convert any image type to any other image type
+ convert any color representation to any other color representation
+ compare/combine vectors with vectors, vectors with scalars, vectors with matrices all using the same operators

`crisp` is **portable**<br>
+ supports Mac, Linux and Windows
+ Distinction between Images and Textures:
    - Images live in RAM and are operated upon by the CPU
    - Textures live inVRAM and are operated upon by the GPU
+ allows for full flexibility with optional hardware acceleration

`crisp` is **interactive**<br>
+ All SFML functionality is included, this means keyboard/mouse support, rendering to a window, shapes, sounds, etc.

## Installation
### Dependencies
+ C++20 or newer
+ [gcc10](https://gcc.gnu.org/install/download.html) or newer 
+ [OpenGL 3.2](https://developer.nvidia.com/opengl-driver) or newer
+ [cmake](https://cmake.org/install/)
+ [Eigen](https://eigen.tuxfamily.org/dox/GettingStarted.html)
+ [SFML](https://www.sfml-dev.org/tutorials/1.6/start-linux.php)
+ [fftw3](https://www.fftw.org/)

### Installation
`crisp` is header only, to add it to your project, simply use:

```bash
git clone https://github.com/Clemapfel/crisp.git
```

or if you already have a project folder:
```
git submodule add https://github.com/Clemapfel/crisp.git
```
Add the corresponding include path to your cmake script and link against `crisp` like so:

```cmake
include_directories(/path/to/.../crisp/include/)
add_executable(MY_EXECUTABLE path/to/my/main.cpp)
target_link_libraries(MY_EXECUTABLE crisp)
```

Be sure to compile with gcc10 or newer, as many needed C++20 features are not supported in older versions. 

For linux system, make sure the following packages are installed:
+ `libfftw3-bin`, `libfftw3-double3`, `libfftw3-long3`, `libfftw3-quad3`, `libfftw3-single3`

## Tutorials

Consider reading the tutorials in order, this will teach important principles of `crisp` and image processing in general step-by-step:

1. [System](./docs/system/system.md)
3. [Vector](./docs/vector/vector.md)
4. [Images](./docs/image/image.md)
2. [Color](./docs/color/color.md)
6. [Noise](./docs/noise/noise.md) (optional)
7. [Spatial Domain Filters](./docs/spatial_filters/spatial_domain_filtering.md)
8. [Morphological Transform](./docs/morphological_transform/morphological_transform.md)
9. [Frequency Domain Filters](./docs/frequency_domain/frequency_domain_filtering.md)
10. [Segmentation](./docs/segmentation/segmentation.md)
11. [Feature Extraction](./docs/feature_extraction/feature_extraction.md)
12. [Deep Learning](./docs/feature_classification/feature_classification_and_deep_learning.md)

## [Documentation](./docs/index.md)

Documentation is stored as interactive markdown files, right here on github. An index for easy navigation can be found [here](./docs/index.md) or in `/crisp/docs/index.md`.
