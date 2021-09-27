# Index

This page lists all major functions and their corresponding entries in the tutorials. You can navigate to each section by clicking the links or visiting `/crisp/docs/xyz` right here on github where xyz is the theme of the tutorial

If this is your first time using `crisp`, consider reading the tutorial in the order recommended [here](../README.md/#tutorial).

---

## Table of Contents
1. [**Images**](#images)<br>
   1.1 [Modifying Images](#modifying-images)<br>
   1.2 [Image Planes](#image-planes)<br>
   1.3 [Histgroams](#image-histograms)<br>
   1.4 [Whole Image Transforms](#whole-image-transforms)<br>
2. [**Spatial Domain Filtering**](#filtering-in-the-spatial-domainspatial_filtersspatial_domain_filteringmd)<br>
    2.1 [Kernels](#kernels)<br>
    2.2 [crisp::SpatialFilter](#filter)<br>
    2.3 [Evaluation Functions](#evaluation-function)<br>
    2.4 [Pre-Defined Kernels](#pre-defined-kernels)<br>
3. [**Morphological Transforms**](#morphological-transforms)<br>
    3.1 [Structuring Element](#structuring-element)<br>
    3.2 [Pre-Defined Structuring Elements](#pre-defined-structuring-elements)<br>
    3.3 [Types of Transforms](#transforms)<br>
4. [**Segmentation**](#segmentation)<br>
    4.1 [crisp::ImageSegment](#crispimagesegment)<br>
    4.2 [Thresholding](#thresholding)<br>
    4.3 [Edge Detection](#edge-detection)<br>
    4.4 [Clustering](#clustering)<br>
5. [**Feature Extraction**](#feature-extractionfeature)<br>
    5.1 [crisp::ImageRegion](#crispimageregion)<br>
    5.2 [Boundary](#boundary)<br>
    5.3 [Signatures](#boundary-signatures)<br>
    5.4 [Region Descriptors](#region-descriptors)<br>
    5.5 [Texture Descriptors](#texture-descriptors)<br>
6. [**Frequency Domain Filtering**](#frequency-domain)<br>
    6.1 [Fourier Transform](#fourier-transform)<br>
    6.2 [Spectral Filters](#spectral-filters)<br>
    6.3 [Pre-Defined Filter Types](#filter-types)<br>
7. [**Vectors**](#vectorsvectorvectormd)<br>
    7.1 [General](#general)<br>
    7.2 [Arithmetics & Comparison](#arithmetics--comparisons)<br>
8. [**Color**](#color)<br>
    8.1 [Color Representations](#color-representations)<br>
    8.2 [Pseudo Color](#pseudo-color)<br>
9. [**Noise**](#noisen)<br>
    9.1 [crisp::NoiseGenerator](#general)<br>
    9.2 [Types of Noise](#types-of-noise)<br>
10. [**System**](#systemsystemsystemmd)<br>
    10.1 [Image I/O](#image-io)<br>
    10.2 [Rendering](#rendering)<br>
    10.3 [User Input](#keyboardmouse-input)<br>
    
---

## [Images](./image/image.md)

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
+ [Loading Images](./image/image.md/#21-loading-images)
+ [Saving Images](./image/image.md/#22-saving-images)
+ [Rendering Images](./image/image.md/#31-rendering-images)
+ [Accessing Pixels](./image/image.md/#32-accessing-individual-pixels)
+ [Padding](./image/image.md/#32-accessing-individual-pixels)
+ [Creating Images](./image/image.md/#33-creating-images)
+ [Image Iterators](./image/image.md/#34-image-iterators)
+ [Image-Image Arithmetics](./image/image.md/#35-image-arithmetics)
+ [Artifacting](./image/image.md/#36-a-note-on-artifacting)

#### Image Planes

+ [Multi Plane Images](./image/image.md/#4-multi-dimensional-images)
+ [Accessing Planes](./image/image.md/#41-accessing-planes-directly)
  
#### Image Histograms
+ [Image Histograms](./image/image.md/#5-histograms)
+ [Histogram Equalization](./image/image.md/#62-histogram-equalization)

#### Whole Image Transforms

+ [Normalize](./image/image.md/#51-normalize)
+ [Gradient Magnitude](./image/image.md/#53-compute-gradient-magnitude)
+ [Bitplane Decomposition](./image/image.md/#54-bitplane-decomposition)
+ [Wavelet Transform](./image/image.md/#55-wavelet-transform)

---

## [Filtering in the Spatial Domain](./spatial_filters/spatial_domain_filtering.md/)

```cpp
#include <spatial_filter.hpp>
```

#### Kernels
+ [crisp::Kernel](./spatial_filters/spatial_domain_filtering.md/#2-kernels)
+ [Normalization](./spatial_filters/spatial_domain_filtering.md/#21-normalize-a-kernel)
+ [Rotation](./spatial_filters/spatial_domain_filtering.md/#22-rotate-a-kernel)
+ [Automated Seperation](./spatial_filters/spatial_domain_filtering.md/#23-separate-a-kernel)
+ [Combining two Kernels](./spatial_filters/spatial_domain_filtering.md/#24-combining-two-kernels)

#### Filter
+ [crisp::SpatialFilter](./spatial_filters/spatial_domain_filtering.md/#3-crispspatialfilter)
+ [Setting the Kernel](./spatial_filters/spatial_domain_filtering.md/#31-specifying-the-kernel)
+ [Applying the Filter](./spatial_filters/spatial_domain_filtering.md/#33-applying-the-filter)
+ [Applying the Filter in Mutiple Dimensions](./spatial_filters/spatial_domain_filtering.md/#34-applying-the-filter-in-all-dimensions)

#### Evaluation Function
+ [Convolution](./spatial_filters/spatial_domain_filtering.md/#32-specifying-the-evaluation-function)
+ [Normalized Convolution](./spatial_filters/spatial_domain_filtering.md/#32-specifying-the-evaluation-function)
+ [Mean](./spatial_filters/spatial_domain_filtering.md/#32-specifying-the-evaluation-function)
+ [Median](./spatial_filters/spatial_domain_filtering.md/#32-specifying-the-evaluation-function)
+ [Min, Max](./spatial_filters/spatial_domain_filtering.md/#32-specifying-the-evaluation-function)
+ [Median Filter Example](./spatial_filters/spatial_domain_filtering.md/#5-using-other-evaluation-functions-for-image-restoration)

#### Pre Defined Kernels
+ [Identity](./spatial_filters/spatial_domain_filtering.md/#41-identity)
+ [One](./spatial_filters/spatial_domain_filtering.md/#42-one)
+ [Zero](./spatial_filters/spatial_domain_filtering.md/#43-zero)
+ [Box](./spatial_filters/spatial_domain_filtering.md/#44-box)
+ [Normalized Box](./spatial_filters/spatial_domain_filtering.md/#45-normalized-box)
+ [Gaussian](./spatial_filters/spatial_domain_filtering.md/#46-gaussian)[](./spatial_filters/spatial_domain_filtering.md/#46-gaussian)
+ [Laplacian First Derivative](./spatial_filters/spatial_domain_filtering.md/#471-laplacian-first-derivative)
+ [Laplacian Second Derivative](./spatial_filters/spatial_domain_filtering.md/#472-laplacian-second-derivative)
+ [Laplacian of Gaussian (LoG)](./spatial_filters/spatial_domain_filtering.md/#48-laplacian-of-gaussian)
+ [Simple Gradient](./spatial_filters/spatial_domain_filtering.md/#491-simple-gradient)
+ [Roberts Gradient](./spatial_filters/spatial_domain_filtering.md/#492-roberts-gradient)
+ [Prewitt Gradient](./spatial_filters/spatial_domain_filtering.md/#493-prewitt-gradient)
+ [Sobel Gradient](./spatial_filters/spatial_domain_filtering.md/#494-sobel)
+ [Kirsch Compass](./spatial_filters/spatial_domain_filtering.md/#495-kirsch-compass)

---

## [Morphological Transforms](./morphological_transform/morphological_transform.md/)

```cpp
#include <morphological_transform.hpp>
```
#### Structuring Element
+ [Introduction](./morphological_transform/morphological_transform.md/#1-introduction)
+ [Foreground, Background, "Don't Care" elements](./morphological_transform/morphological_transform.md/#21-foreground-background-and-dont-care-elements)
+ [Binding a Structuring Element](./morphological_transform/morphological_transform.md/#22-binding-an-structuring-element)
+ [Origin of a Structuring Element](./morphological_transform/morphological_transform.md/#23-origin)
  
#### Pre-Defined Structuring Elements
+ [All Foreground](./morphological_transform/morphological_transform.md/#241-all-foreground)
+ [All Background](./morphological_transform/morphological_transform.md/#242-all-background)
+ [All "don't care"](./morphological_transform/morphological_transform.md/#243-all-dont-care)
+ [Circle](./morphological_transform/morphological_transform.md/#244-circle)
+ [Diamond](./morphological_transform/morphological_transform.md/#245-diamond)
+ [Cross](./morphological_transform/morphological_transform.md/#246-cross)

#### Transforms
+ [Erosion](./morphological_transform/morphological_transform.md/#31-erosion)
+ [Dilation](./morphological_transform/morphological_transform.md/#32-dilation)
+ [Geodesic Erosion](./morphological_transform/morphological_transform.md/#33-geodesic-erosion)
+ [Geodesic Dilation](./morphological_transform/morphological_transform.md/#34-geodesic-dilation)
+ [Closing](./morphological_transform/morphological_transform.md/#35-closing)
+ [Opening](./morphological_transform/morphological_transform.md/#36-opening)
+ [Hit-or-Miss Transform](./morphological_transform/morphological_transform.md/#37-hit-or-miss-transform)
+ [Pattern Replacement](./morphological_transform/morphological_transform.md/#38-pattern-replacement)

---

## [Segmentation](./segmentation/segmentation.md)

```cpp
#include <image_segment.hpp>
#include <segmentation.hpp>
#include <edge_detection.hpp>
```

#### crisp::ImageSegment
+ [Introduction](./segmentation/segmentation.md/#1-introduction)
+ [Connected and Unconnected Decomposition](./segmentation/segmentation.md/#11-extracting-segments)

#### Thresholding
+ [Problems with Uniform vs Non-Uniform Lighting](./segmentation/segmentation.md/#2-thresholding)
+ [Manual Thresholds](./segmentation/segmentation.md/#21-manual-threshold)
+ [Basic Automated Thresholding](./segmentation/segmentation.md/#22-basic-threshold)
+ [Otsu's Method](./segmentation/segmentation.md/#23-otsus-method)
+ [Variable Threshold](./segmentation/segmentation.md/#24-variable-threshold)
+ [Neighborhood Threshold](./segmentation/segmentation.md/#25-neighborhood-threshold)

#### Edge Detection
+ [Threshold Sobel Gradient Magnitude](./segmentation/segmentation.md/#31-threshold-sobel-gradient)
+ [Canny's Algorithm](./segmentation/segmentation.md/#32-canny)

#### Clustering
+ [k-means Clustering](./segmentation/segmentation.md/#41-k-means-clustering)
+ [Superpixel Clustering](./segmentation/segmentation.md/#42-superpixel-clustering)
+ [Clustering by Region Growing](./segmentation/segmentation.md/#43-region-growing-clustering)

---

## [Feature Extraction](./feature_extraction/feature_extraction.md)

```cpp
#include <image_region.hpp>
```

### crisp::ImageRegion
+ [Introduction](./feature_extraction/feature_extraction.md/#1-introduction)
+ [How to Extract a Region](./feature_extraction/feature_extraction.md/#11-an-example)

### Boundary
+ [Region Boundary](./feature_extraction/feature_extraction.md/#2-region-boundary)
+ [Definition](./feature_extraction/feature_extraction.md/#21-8-connectivity-and-minimal-cardinality)
+ [Boundary Polygon](./feature_extraction/feature_extraction.md/#22-boundary-polygon)

### Boundary Signatures
+ [Vertex Polygon](./feature_extraction/feature_extraction.md/#31-vertex-polygon)
+ [Slope Chain Code Signature](./feature_extraction/feature_extraction.md/#32-slope-chain-code-signature)
+ [Radial Distance Signature](./feature_extraction/feature_extraction.md/#33-radial-distance-signature)
+ [Complex Coordinate Signature](./feature_extraction/feature_extraction.md/#34-complex-coordinate-signature)
+ [Farthest Point Signature](./feature_extraction/feature_extraction.md/#35-farthest-point-signature)

### Region Descriptors
+ [Introduction](./feature_extraction/feature_extraction.md/#4-whole-region-descriptors)
+ [Area](./feature_extraction/feature_extraction.md/#41-area--perimeter-compactness)
+ [Perimeter](./feature_extraction/feature_extraction.md/#41-area--perimeter-compactness)
+ [Compactness](./feature_extraction/feature_extraction.md/#41-area--perimeter-compactness)
+ [Centroid](./feature_extraction/feature_extraction.md/#42-centroid)
+ [Axis Aligned Bounding Box](./feature_extraction/feature_extraction.md/#43-aabb)
+ [Major and Minor Axis](./feature_extraction/feature_extraction.md/#44-major--minor-axis)
+ [Eccentricity](./feature_extraction/feature_extraction.md/#45-eccentricity)
+ [Circularity](./feature_extraction/feature_extraction.md/#46-circularity)
+ [Number of Holes](./feature_extraction/feature_extraction.md/#47-holes)
+ [Hole Boundaries](./feature_extraction/feature_extraction.md/#47-holes)
+ [N-ths Moment Invariant](./feature_extraction/feature_extraction.md/#48-moment-invariants)

### Texture Descriptors
+ [Co-Occurence Matrix](./feature_extraction/feature_extraction.md/#51-co-occurence-matrix)
+ [Maximum Intensity Response](./feature_extraction/feature_extraction.md/#52-maximum-response)
+ [Correlation](./feature_extraction/feature_extraction.md/#53-intensity-correlation)
+ [Uniformity](./feature_extraction/feature_extraction.md/#54-uniformity)
+ [Homogeneity](./feature_extraction/feature_extraction.md/#55-homogeneity)
+ [Entropy](./feature_extraction/feature_extraction.md/#56-entropy)
+ [Contrast](./feature_extraction/feature_extraction.md/#57-contrast)

---

## [Frequency Domain](./frequency_domain/frequency_domain_filtering.md)
```cpp
#include <fourier_transform.hpp>
#include <frequency_domain_filter.hpp>
```

#### Fourier Transform

+ [Introduction](./frequency_domain/frequency_domain_filtering.md/#2-fourier-transform)
+ [Performance Mode](./frequency_domain/frequency_domain_filtering.md/#21-fourier-transform-performance-mode)
+ [Image -> Spectrum](./frequency_domain/frequency_domain_filtering.md/#22-transforming-an-image-into-the-spectrum)
+ [Spectrum -> Image](./frequency_domain/frequency_domain_filtering.md/#25-transforming-the-spectrum-back-into-an-image)
+ [Accessing Coefficients](./frequency_domain/frequency_domain_filtering.md/#24-accessing-coefficients)
+ [Visualizing the Spectrum](./frequency_domain/frequency_domain_filtering.md/#23-visualizing-the-spectrum)

#### Spectral Filters
+ [crisp::FrequencyDomainFilter](./frequency_domain/frequency_domain_filtering.md/#3-spectral-filters)
+ [Construction](./frequency_domain/frequency_domain_filtering.md/#31-creating-and-visualizing-filters)
+ [Visualization](./frequency_domain/frequency_domain_filtering.md/#31-creating-and-visualizing-filters)
+ [Modifying Filters](./frequency_domain/frequency_domain_filtering.md/#33-modifying-filters)
+ [Combining Filters](./frequency_domain/frequency_domain_filtering.md/#33-modifying-filters)
+ [Filter Offset & Radial Symmetry](./frequency_domain/frequency_domain_filtering.md/#33-modifying-filters)

#### Filter Types
+ [Low-Pass Filters](./frequency_domain/frequency_domain_filtering.md/#321-low-pass-filters)
  - [Ideal](./frequency_domain/frequency_domain_filtering.md/#321-low-pass-filters)
  - [Gaussian](./frequency_domain/frequency_domain_filtering.md/#321-low-pass-filters)
  - [Butterworth](./frequency_domain/frequency_domain_filtering.md/#321-low-pass-filters)
+ [High-Pass Filters](./frequency_domain/frequency_domain_filtering.md/#322-high-pass-filters)
  - [Ideal](./frequency_domain/frequency_domain_filtering.md/#322-high-pass-filters)
  - [Gaussian](./frequency_domain/frequency_domain_filtering.md/#322-high-pass-filters)
  - [Butterworth](./frequency_domain/frequency_domain_filtering.md/#322-high-pass-filters)
+ [Band-Pass Filters](./frequency_domain/frequency_domain_filtering.md/#323-band-pass-filters)
  - [Ideal](./frequency_domain/frequency_domain_filtering.md/#323-band-pass-filters)
  - [Gaussian](./frequency_domain/frequency_domain_filtering.md/#323-band-pass-filters)
  - [Butterworth](./frequency_domain/frequency_domain_filtering.md/#323-band-pass-filters)
+ [Band-Reject Filters](./frequency_domain/frequency_domain_filtering.md/#324-band-reject-filters)
  - [Ideal](./frequency_domain/frequency_domain_filtering.md/#324-band-reject-filters)
  - [Gaussian](./frequency_domain/frequency_domain_filtering.md/#324-band-reject-filters)
  - [Butterworth](./frequency_domain/frequency_domain_filtering.md/#324-band-reject-filters)
    
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

## [Color](./color/color.md)

```cpp
#include <color.hpp>
#include <pseudo_color_mapping.hpp>
```

#### Color Representations
+ [Conversion](./color/color.md/#1-introduction)
+ [RGB](./color/color.md/#2-rgb)
+ [HSV](./color/color.md/#3-hsv)
+ [HSL](./color/color.md/#4-hsl)
+ [GrayScale](./color/color.md/#5-grayscale)

#### Pseudo Color
+ [Pseudo Color Mapping](./color/color.md/#71-crisppseudocolor)
+ [Value to Hue](./color/color.md/#71-crisppseudocolor)
+ [Value Range to Hue](./color/color.md/#71-crisppseudocolor)
+ [Value Range to Hue range](./color/color.md/#71-crisppseudocolor)
+ [Value Range to inverse Hue range](./color/color.md/#71-crisppseudocolor)
+ [Multi Range Mapping](./color/color.md/#72-multi-range-mapping)

---

## [Noise](./noise/noise.md) 

```cpp
#include <noise_generator.hpp>
```

#### General
+ [crisp::NoiseGenerator](./noise/noise.md/#1-introduction)
+ [Spatially Invariant vs Periodic Noise](./noise/noise.md/#2-noise-generator-for-spatially-invariant-noise)
   
#### Types of Noise
+ [Uniform Noise](./noise/noise.md/#21-uniform-noise)
+ [Gaussian Noise](./noise/noise.md/#22-gaussian-noise)
+ [Gamma Noise](./noise/noise.md/#23-gamma-noise)
+ [Salt and Pepper Noise](./noise/noise.md/#24-salt-and-pepper-noise)

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
