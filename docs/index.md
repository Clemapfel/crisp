# Index

This page lists all major functions and their corresponding entires in the tutorials. You can navigate to each section by clicking the links right here on github or visit `/crisp/docs/xyz` where xyz is the theme of the tutorial

TODO TOC:

---
## [Vectors](./vector/vector.md)

```cpp
#include <vector.hpp>
```
#### General
+ [Construction](./vector/vector.md/#2-construction)
+ [Component Access](./vector/vector.md/#3-component-access)
+ [Typedefs for Convenience](./vector/vector.md/#4-typedefs)
+ [Hash](./vector/vector.md/#53-vector-vector-comparison)
  
#### Arithmetics & Comparisons
+ [Vector-Vector Arithmetics](./vector/vector.md/#51-vector-vector-arithmetics)
+ [Vector-Scalar Arithmetics](./vector/vector.md/#52-vector-scalar-arithmetics)
+ [Vector-Vector Comparison](./vector/vector.md/#53-vector-vector-comparison)
+ [Vector-Scalar Comparison](./vector/vector.md/#54-vector-scalar-comparison)

---

## [System](./system/system.md)

```cpp
#include <system/input_handler.hpp>
#include <system/image_io.hpp>
#include <system/key_id.hpp>
#include <system/render_window.hpp>
#include <system/sprite.hpp>
```
#### Image I/O
+ [Loading an Image](./system/system.md/#11-loading-an-image)
+ [Saving an Image](./system/system.md/#12-saving-an-image)
  
#### Rendering
+ [Sprite](./system/system.md/#21-sprite--render-window)
+ [RenderWindow](./system/system.md/#21-sprite--render-window)
+ [Render Loop](./system/system.md/#3-an-example) 

#### Keyboard/Mouse Input
+ [Mouse/Keyboard Input](./system/system.md/#22-mouse--keyboard-input)


--- 

## [Images](./image/images.md)

```cpp
#include <image/multi_plane_image.hpp>
#include <image/binary_image.hpp>
#include <image/grayscale_image.hpp>
#include <image/multi_plane_image.hpp>
#include <image/padding_type>

#include <whole_image_processing.hpp>
#include <histogram.hpp>
```

#### Modifying Images
+ [Loading Images](./image/images.md/#21-loading-images)
+ [Saving Images](./image/images.md/#22-saving-images)
+ [Rendering Images](./image/images.md/#31-rendering-images)
+ [Accessing Pixels](./image/images.md/#32-accessing-individual-pixels)
+ [Padding](./image/images.md/#32-accessing-individual-pixels)
+ [Creating Images](./image/images.md/#33-creating-images)
+ [Image Iterators](./image/images.md/#34-image-iterators)
+ [Image-Image Arithmetics](./image/images.md/#35-image-arithmetics)
+ [Artifacting](./image/images.md/#36-a-note-on-artifacting)

#### Image Planes

+ [Multi Plane Images](./image/images.md/#4-multi-dimensional-images)
+ [Accessing Planes](./image/images.md/#41-accessing-planes-directly)
  
#### Image Histograms
+ [Image Histograms](./image/images.md/#5-histograms)
+ [Histogram Equalization](./image/images.md/#52-histogram-equalization)

#### Whole Image Transforms

+ [Normalize](./image/images.md/#51-normalize)
+ [Gradient Magnitude](./image/images.md/#53-compute-gradient-magnitude)
+ [Bitplane Decomposition](./image/images.md/#54-bitplane-decomposition)
+ [Wavelet Transform](./image/images.md/#55-wavelet-transform)

---

## Noise 

```cpp
#include <noise_generator.hpp>
```

#### General
+ [crisp::NoiseGenerator](#1-introduction)
+ [Spatially Invariate vs Periodic Noise](#2-noise-generator-for-spatially-invariate-noise)
   
#### Types of Noise
+ [Uniform Noise](#21-uniform-noise)
+ [Gaussian Noise](#22-gaussian-noise)
+ [Gamma Noise](#23-gamma-noise)
+ [Salt and Pepper Noise](#24-salt-and-pepper-noise)

---

## [Filtering in the Spatial Domain](./spatial_filters/spatial_domain_filtering.md)

```cpp
#include <spatial_filter.hpp>
```

#### Kernels
+ [crisp::Kernel](#2-kernels)
+ [Normalization](#21-normalize-a-kernel)
+ [Rotation](#22-rotate-a-kernel)
+ [Automated Seperation](#23-separate-a-kernel)
+ [Combining two Kernels](#24-combining-two-kernels)

#### Filter
+ [crisp::SpatialFilter](#3-crispspatialfilter)
+ [Setting the Kernel](#31-specifying-the-kernel)
+ [Applying the Filter](#33-applying-the-filter)
+ [Applying the Filter in Mutiple Dimensions](#34-applying-the-filter-in-all-dimensions)

#### Evaluation Function
+ [Convolution](#32-specifying-the-evaluation-function)
+ [Normalized Convolution](#32-specifying-the-evaluation-function)
+ [Mean](#32-specifying-the-evaluation-function)
+ [Median](#32-specifying-the-evaluation-function)
+ [Min, Max](#32-specifying-the-evaluation-function)
+ [Median Filter Example](#5-using-other-evaluation-functions-for-image-restoration)

#### Pre Defined Kernels
+ [Identity](#41-identity)
+ [One](#42-one)
+ [Zero](#43-zero)
+ [Box](#44-box)
+ [Normalized Box](#45-normalized-box)
+ [Gaussian](#46-gaussian)[](#46-gaussian)
+ [Laplacian First Derivative](#471-laplacian-first-derivative)
+ [Laplacian Second Derivative](#472-laplacian-second-derivative)
+ [Laplacian of Gaussian (LoG)](#48-laplacian-of-gaussian)
+ [Simple Gradient](#491-simple-gradient)
+ [Roberts Gradient](#492-roberts-gradient)
+ [Prewitt Gradient](#493-prewitt-gradient)
+ [Sobel Gradient](#494-sobel)
+ [Kirsch Compass](#495-kirsch-compass)
