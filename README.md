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

+ `crisp::BinaryImage`
+ `crisp::GrayScaleImage`
+ `crisp::ColorImage`
+ `crisp::Image<T, 1>` for any numerical T
+ `crisp::Image<T, 3>` for any numerical T
+ `crisp::FourierTransform`
+ `crisp::FrequencyDomainFilter`
+ `crisp::Kernel`
+ `crisp::StructuringElement`
+ `crisp::Histogram`
+ `Eigen::Matrix<T, /*...*/>`

If you also were to list all classes supplied by `crisp` you would end up with the same list!<br>
This makes debugging, filter design or just understanding/explaining your own algorithms much easier and more intuitive.

`crisp` is **well documented**<br>
+ full tutorials are available for all features
+ examples with images are provided whenever possible
+ in-line documentation for IDE support
+ user-facing code is polished to a mirror sheen
+ language is kept simple, aimed at people without a math degree

`crisp` is **math-less**<br>
+ Algorithms choose their own parameters heuristically
    - no more trolling through papers trying to find out what parameter `α` does.<br>
+ creating the fourier transform of an image is one line of code 
+ k-means clustering in n-dimensions is one line of code
+ computing the gradient magnitude is one line of code

`crisp` is **generic**
+ filter, segment, cluster in any number of dimensions on any image value type using the same function
+ convert any image type to any other image type
+ convert any color representation to any other color representation
+ vectors are notationally equivalent to scalars

`crisp` is **portable**<br>
+ supports Mac, Linux and Windows
+ images live in RAM and are operated on by the CPU
+ textures live vRAM and are operated on by the GPU

`crisp` is **interactive**<br>
All SFML functionality is included, this means keyboard/mouse support, rendering to a window, sound, etc.

## Installation
### Dependencies
+ C++20 or newer
+ gcc10 or newer 
+ OpenGL 3.2 or newer
+ cmake
+ Eigen
+ SFML
+ fftw3 (included)

### Installation
`crisp` is header only, to add it to your project, simply use:

```bash
git submodule add TODO
```

Add the corresponding include path to your cmake script and linke against `crisp`:
```cmake
include_directories(/path/to/.../crisp/include/)
add_executable(MY_EXECUTABLE path/to/my/main.cpp)
target_link_libraries(MY_EXECUTABLE crisp)
```
## Documentation

Documentation is stored as interactive markdown files, right here on github. An index for easy navigation can be found here or in `/crisp/docs/index.md`.






