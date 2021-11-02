# CRISP: Real-Time Interactive Signal Processing Library (v0.8.3)

## What is `crisp`?
`crisp` is a library implementing most image-processing needs from scratch. This includes:
+ Spatial Filtering
+ Morphological Transform
+ Fourier Transform & Spectral Filtering
+ Edge Detection
+ Segmentation & Clustering
+ Feature Extraction
+ Boundary and Texture Descriptors
+ Neural Networks (fully connected and convolutional)
+ GPU-side hardware acceleration
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
+ `crisp::ConvolutionalNeuralNetwork`
+ and more!

This makes debugging, filter design or just understanding/explaining your own algorithms much easier and more intuitive.

`crisp` is **fast AND flexible**
+ images live in ram and are operated upon by the CPU
+ very easy to understand and interface with. Allows for high flexibility
+ textures live in vram and are operated upon by the GPU
+ harder to interact with, textures make up for this by being extremely fast as they make full use of hardware-acceleration
+ all algorithms work on textures and images
+ textures can be transformed into images and vice-versa

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
+ runs on tiny 1-cpu machines like embedded systems
+ also makes full use of strong machines' multiple processors, hardware-acceleration

## Installation
### Dependencies
+ C++20 or newer
+ [cmake](https://cmake.org/install/)
+ [gcc10](https://gcc.gnu.org/install/download.html) or newer 
    - `libgcc-10-dev`
+ [OpenGL 3.3](https://developer.nvidia.com/opengl-driver) or newer
    - `libopengl-dev`
+ [Eigen](https://eigen.tuxfamily.org/dox/GettingStarted.html)
    - provided as submodule
    - `libeigen3-dev`
+ [SFML](https://www.sfml-dev.org/tutorials/1.6/start-linux.php)
    - `libsfml-system2.5`
    - `libsfml-window2.5`
    - `libsfml-graphics2.5`
+ [fftw3](https://www.fftw.org/)
    - `libfftw3-bin` 
    - `libfftw3-single3`
    - `libfftw3-double3` 
    - `libfftw3-long3`
    - `libfftw3-quad3`
+ [sol2](https://github.com/ThePhD/sol2/)
    - provided as submodule
+ [lua5.3](https://www.lua.org/download.html)
    - `lua5.3` package
    
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
Be sure to compile with gcc10 or newer, as many needed C++20 features are not supported in older versions. If linker errors appear, make sure all dependencies and packages listed above are installed on a system level.

## Tutorials

Consider reading the tutorials in order, this will teach important principles of `crisp` (and image processing in general) step-by-step:

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
13. [Hardware Acceleration](./docs/hardware_acceleration/textures.md)

## [Documentation](./docs/index.md)

Documentation is stored as interactive markdown files, right here on github. An index for easy navigation can be found [here](./docs/index.md) or in `/crisp/docs/`.
