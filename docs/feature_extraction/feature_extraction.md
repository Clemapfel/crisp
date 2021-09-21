# Feature Extraction

Image Regions, Boundary Tracing, Region Signature & other Descriptors

## Table of Contents

1. [Introduction](#1-introduction)<br>
    1.1 [Extracting a Region](#11-an-example)<br>
2. [Region Boundary](#2-a-regions-boundary)<br>
    2.1 [Definition](#21-8-connectivity-and-minimal-cardinality)<br>
   2.2 [Boundary Polygon](#22-boundary-polygon)<br>
3. [Boundary Signature](#3-boundary-signatures)<br>
    3.1 [Vertex Polygon](#31-vertex-polygon)<br>
    3.2 [Slope Chain Code Signature](#32-slope-chain-code-signature)<br>
    3.3 [Radial Distance Signature](#33-radial-distance-signature)<br>
    3.4 [Complex Coordinate Signature](#34-complex-coordinate-signature)<br>
    3.5 [Farthest Point Signature](#35-farthest-point-signature)<br>
4. [Whole Region Descriptors](#4-whole-region-descriptors)<br>
    4.1 [Area, Perimeter, Compactness](#41-area--perimeter-compactness)<br>
    4.2 [Centroid](#42-centroid)<br>
    4.3 [Axis Aligned Bounding Box](#43-aabb)<br>
    4.4 [Major and Minor Axis](#44-major--minor-axis)<br>
   4.5 [Eccentricity](#45-eccentricity)<br>
   4.6 [Circularity](#46-circularity)<br>
   4.7 [Holes](#47-holes)<br>
   4.8 [N-ths Moment Invariant](#48-moment-invariants)<br>
5. [Texture Descriptors](#5-texture-descriptors)<br>
    5.1 [Co-Occurence Matrix](#51-co-occurence-matrix)<br>
   5.2 [Maximum Intensity Response](#52-maximum-response)<br>
   5.3 [Correlation](#53-intensity-correlation)<br>
   5.4 [Uniformity](#54-uniformity)<br>
   5.5 [Homogeneity](#55-homogeneity)<br>
   5.6 [Entropy](#56-entropy)<br>
   5.7 [Contrast](#57-contrast)<br>

## 1. Introduction

In the [tutorial on segmentation](../segmentation/segmentation.md) we discussed how to extract part of an image, now we find out what to do with them. Recall that an image region is a set of pixel coordinates ``Vector2ui``:

```cpp
using ImageSegment = std::set<Vector2ui, /*...*/>;
```

In ``crisp`` image segments hold now information about the pixels and indeed one image segment could be part of multiple images as long as the maximum and minimum boundaries of the pixel coordinates are small than the size of the two images. To apply a certain operation to only a segment of an image, we can do the following:

```cpp
auto lambda_operator = []<typename Image_t>(size_t x, size_t y, const Image_t& image) -> typename Image_t::Value_t
{
    auto value = image(x, y);
    // some transformation
    return value;
};

ImageSegment segment = /*...*/;
auto image = /*...*/;

for (const auto position : segment)
    image(position.x(), position.y()) = lambda_operator(position.x(), position.y(), image);
```

Here we're first defining a templated lambda that asks for 3 arguments: the x- and y-coordinate of a pixel and an image. It accesses the pixels value, does something to it and returns it. To then apply this transformation only to part of an image we can simply iterate through the segment and selective only alter pixels that are part of the segment in the original image.

It would be cumbersome and limiting to always carry a reference to the original image so instead ``crisp::ImageRegion`` copies all of that information on construction: while not a ``typedef`` like ``ImageSegment`` it can be helpful be aware that internally, ``crisp::ImageRegion`` uses a set of ``crisp::ImageRegion::Element``s like so:

```cpp
template<typename Image_t>
class ImageRegion
{   
    using Value_t = Image_t::Value_t;
    
    public:

    private:
        struct Element 
        {
            Vector2ui position;
            Value_t value;
        }
            
        std::set<Element, /*...*/> _elements;
}
```

We see that each element has the pixel ``position`` and the pixels original ``value``, which of course depends on the type of image the region is part of. Just like with ``ImageSegment`` the elements are sorted left-to-right, top-to-bottom. 

We construct an ``ImageRegion`` from an image and a ``crisp::ImageSegment``:

```cpp
#include <image_region.hpp>
using namespace crisp;

// in main
auto image = /*...*/;
auto segment = // some segmentation algorithm that returns crisp::ImageSegment

auto region = ImageRegion();
region.create_from(segment, image);

// or equivalently:
auto region = ImageRegion(segment, image);
``` 

Once ``create_from`` is called we are free to deallocate the original image and segment as ``ImageRegion`` already copied all relevent values.

## 1.1 An Example

Consider this image of the "pepper brush" as provided by [gimp](https://www.gimp.org/) :<br>

![](./pepper.png)

We want to extract the region that has the pepper. We observe the background to by very dark so a simple manual thresholding operation is guruanteed to extract the correct region. We first load the image as color, then convert it to grayscale using ``get_value_plane`` (remember that the HSV value component has the same value as the mean of all color components):

```cpp
#include <system/image_io.hpp>
#include <image/grayscale_image.hpp>
#include <segmentation.hpp>
using namespace crisp;

// in main.cpp

auto pepper_path = /*...*/ + "/crisp/docs/feature_extraction/pepper.png";

auto image = load_color_image(/*...*/ + "/crisp/docs/feature_extraction/pepper.png");
auto as_grayscale = image.get_value_plane();
auto thresholded = manual_threshold(as_grayscale, 0.01f);
```
![](./pepper_segment.png)

We then want to decompose the grayscale iamge into segments. After decomposition the segments will be ordered according to the respective left-most, top-most pixels coordinate, (0, 0) is black and there are only two segments, we expect the pepper to be the second segment extracted:

```cpp
auto segments = decomponse_into_connected_segments(thresholded);
auto pepper_segment = segments.at(1);
```

We can now construct our resulting region using the pepper segment and the original color images values:

```cpp
auto pepper = ImageRegion(pepper_segment, image);
```

We will be using this region for rest of the tutorial.

## 2. A Regions Boundary

Mathematically, ``crisp``s regions are *closed, simply connected regions*. This basically means a) its boundary is part of the set of elements and b) all elements are 4-connected. Regarding 4-connectivty, ``ImageRegion`` will throw an excpetion if the segment handed to it is not 4-connected. We can assure it is either by using ``decompose_into_connected_segments`` (as outline in the [segmentation tutorial](../segmentation/segmentation.md) or we can use ``decompose_into_regions(const ImageSegment&, const Image_t&) -> std::vector<ImageRegion>`` which automatically splits the segment into 4-connected sub-segments and then constructs a region from each. 

Now that we assured that our region is indeed handed a 4-connected segment we can concern ourselfs with the second and (in terms of feature recognition) most important property: it's boundary. In ``crisp`` boundaries have the following properties:

Let ``B = {b_0, b_1, b_2, ..., bm}`` be the set of boundary points, then:
+ i) for each ``b_i`` there exists a ``b_i-1``, ``b_i+1`` in ``B`` such that ``b_i-1`` is 8-connected to ``b_i``, ``b_i+1`` is 8-connected to ``b_i`` and ``b_i-1`` is not 8-connected to ``b_i+1``
+ ii) the set ``B`` is minimal in terms of cardinality under i), meaning if we were to remove any ``b_i`` in ``B``, property i) would be violated

``crisp`` prides itself on being somewhat easy to understand to people less proficient in math so lets explore each of these property visually:

### 2.1 8-Connectivity and Minimal Cardinality

Property i) means the region is an unbroken chain of 8-connected pixels and that the boundary forms an unbroken path such that one can jump from ``b_0`` to ``b_1``, ``b_1`` to ``b_2``, etc. up to ``b_m-1`` to ``b_m`` (the last point) and then, crucially, from ``b_m`` back to ``b_0`` completing the circle. 

Let's again consider the region of our pepper:<br>

![](./pepper_segment.png)

A simple way of tracing it's boundary would be to highlight all pixels that have at least one neighbor that is not in the region (black, in our case).

![](./pepper_boundary_bad.png)

This is a boundary that fullfills condition i), however inspecting the boundary closely we notice many redundant points:

![](./pepper_boundary_bad_zoom.png)

If we were to ask a human to remove as many points as possible without compromising condition i) we would get the following boundary, where necessary pixels are highlighted in magenta, redundant pixels in cyan:

![](./pepper_boundary_good_zoom.png)

This is what condition ii) represents, we want all pixels to be non-redundant. This vastly increases performance as for our pepper example we go from 3854 pixels for our trivial boundary to only 472 pixels using a minimal boundary:

![](./pepper_boundary.png)

``crisp``s proprietary boundary tracing algorithm assures that the computed boundary is always minimal. We can access it using: ``std::vector<Vector2ui> ImageRegion::get_boundary() const``.

### 2.2 Boundary Polygon

We can even further reduce the number of points by treating the boundary as a polygon that has vertices and straight, non-intersecting lines connecting exactly two of the vertices. Consider this part of our pepper boundary:

![](./pepper_line_zoom.png)

We note multiple straight lines, each of these lines can be respresented by just two pixels at the start and beginning of the line, shown in green ``rgb(0, 1, 0)`` here:

![](./pepper_line_zoom_polygon.png)

Using this approach we reduce the number of boundary points from 472 to only 193, again without loosing any information. The information is retained by the fact that the polygon vertices are ordered in counter-clockwise direction, this way we know exactly where to draw the straight line to the next point. 

Now that we reduced the entire information contained in the region in the shape of a pepper to just 193 pixels some may think we are done but thanks to more math we can reduce it even further and/or increase it's generality.

## 3. Boundary Signatures

A signature is a mathematical transform of the boundary points of a shape that aims to, in some way, make the description of the boundary more widely applicable. One of these ways is to make it *independent of rotation*, a signature that accomplishes this describes not only our upright pepper but all possible rotation of it at the same time. Another form of generality is *scale invariance*, meaning that the signature describes our pepper at scale 1 and the same pepper scaled by any factor > 0. Lastly *independence of translation* means that it does not matter if we were to translate all points of the signature by a constant (x, y), the signature represents all of those peppers just the same. ``crisp`` offers the following signatures:

## 3.1 Vertex Polygon

This is the simplest signature, as already mentioned it reduces the boundary to the vertices of it's polygon. It is neither scale nor rotationally invariant, it can however be computed in amortized o(1) using:

```cpp
auto polygon_signature = pepper.get_boundary_polygon();
```

## 3.2 Slope Chain Code Signature

We can generate the slope-chain-signature by simply iterating through all boundary polygon vertices and storing the *angle* of the line that connects our current polygon vertex to the next (recall that the vertices are ordered counter-clockwise). We can then transform this signature to be rotationally and translation invariant by computing the difference of angle from one vertex to the next, this way we can start at point and complete the full chain to create the shape by just drawing a line at that angle

We can generate the slop-chain-code-signature using ``std::vector<float> ImageRegion::slope_chain_code_signature() const``, it returns the angles in radian in order of boundary polygon vertices. Computing the difference from this is a trivial operation.

## 3.3 Radial Distance Signature

The radial distance signature is the distance of each vertex from the regions centroid. The centroid of a region in ``crisp`` is defined as the mean of all boundary coordinates and can be intuitively thought of as the center of mass of a region if all pixels have the same weight. This signature is translation and rotationally invariant. We can generate it using ``std::vector<float> ImageRegin::radial_distance_signature() const`` in amortized o(m) (where m the number of boundary vertices).

## 3.4 Complex Coordinate Signature

(as proposed by [El-Ghazal, Basir, Belkasim (2007)](https://ieeexplore.ieee.org/abstract/document/4378916))

This signature transforms each point in the boundary polygon into a complex number, for a point (x, y) the signature of the point is the complex number x + i*y where i is the imaginare constant, so the x-coordinate is treated as the real part and the y-coordinate is treated as the y-part. While this signature is neither invariant to scale or rotation, we can now fourier-transform the complex number achieving scale, rotational and translational invariance.

We can access the complex coordinates in O(m) (where m the number of boundary polygon points) using ``ImageRegion::complex_coordinate_signature()``. To then transform it into it's fourier descriptors we can use the fourier transform functions provided by ``crisp::FourierTransform``.

## 3.5 Farthest Point Signature

(as proposed again by [El-Ghazal, Basir, Belkasim (2009)](https://www.sciencedirect.com/science/article/abs/pii/S0923596509000393))

This signature computes for each boundary the maximum distance to any other boundary point. This signature performs betters when used for fourier descriptors [1] and achieves translational and rotational invariance.
We can generate it using ``ImageRegion::farther_point_signature``. 

[1] (Y. Hu, Z. Li, (2013): [available here](http://www.jsoftware.us/vol8/jsw0811-31.pdf)

## 4. Whole Region Descriptors

When trying to classify the shape of a region we want to somehow quantify a certain aspect about it in a way that makes it so that value is uniqe to the specific shape and that if two regions are close in that value, they close in shape. This section deals with many properties of not only a regions shape but the region including it's values. Whille only one specific property may not be too notable, combining many of them increases the specificity of classification.

## 4.1 Area & Perimeter, Compactness

One of the easiest descripters are *area*, the number of pixels in a region and *perimeter* the length of the regions boundary. Note that perimeter only deals with the outer most boundary, so increasing the number of holes in a region will decrease its area but leave it's perimeter unchanged. Area and perimeter usually are not very useful unless they are normalized or by quantifying the areas *compactness* which is equal to the square of the perimeter divided by the area. A region that has no holes will have maximum compactness while a region that has many or very big holes has a higher compactness. We can access, area, perimeter and compactness using:

```
float get_perimter() const;
float get_area() const;
float get_compactness() const;
```

## 4.2 Centroid

A regions *centroid* in ``crisp`` is defined as the mean of the value of it's boundary points coordinates. while sometimes in literature the centroid is the average of *all* points in an area, this is not the case in ``crisp``. Adding holes to a region while leaving it's boundary unchanged does not alter the position of the regions centroid.

We access a region centroid at any time using ``get_centroid()``:

![](./pepper_centroid.png)

Where the centroid is highligted using a red rgb(1, 0, 0) cross.

## 4.3 AABB

The *axis aligned bounding box* (AABB) of a region is the smallest rectangle whos sides align with the x- and y-axis that completely encloses the region. We can access it like so:

```cpp
std::vector<Vector2ui, 4> aabb_vertcies = pepper.get_axis_aligned_bounding_box(); 
```

Where the vertices are in the following order: top-left, top-right, bottom-right, bottom-left.

The value of the vertices of the rectangle are relative to the top-left corner of the image the region is from, translating them to the origin is a trivial operation.

![](./pepper_aabb.png)

## 4.4 Major & Minor Axis

The major and minor axis of a region is defined as the ellipses formed by the eigen vectors multiplied with their respective eigenvalues of the positional covariance matrix of the boundary of a region. It's not important to understand what this means, we can think of the major and minor axis as the "orientation" of an object, where the major axis is along the highest variance (in terms of spacial position) and both axis intersect with a centroid. We access the minor and major access using:

```cpp
const std::pair<Vector2f, Vector2f>& get_major_axis() const;

const std::pair<Vector2f, Vector2f>& get_minor_axis() const;
```

Each of the axis is given as two point. Visualizing the axis properly is difficult as they are sub-pixel precision. By scaling the image of the pepper we can get an idea of what they represent:

![](./pepper_eigenvectors_modified.png)

Where magenta is the major, green the minor axis and in yellow the ellipses modeled. 

## 4.5 Eccentricity
Using the minor and major axis we can furthermore compute the regions *eccentricity* which is quantifying how "conical" the region is, if the eccentricity is 0 the shape modeled by the major and minor axis is a perfect circle, the closer to 1 the eccentricity is, the more elliptical the region.

We can access the eccentricity using ``ImageRegion::get_eccentricity()``.

## 4.6 Circularity

While eccentricity measures how closely the shape of a region approximates a non-circular ellipses, *circularity* quantifies how closely the shape approximates a perfect circle. Regions with tend to have smooth features and not many sharp edges tend to have high circularity towards 1 while angular shapes or shapes with a high eccentricity tend to have a circularity closer towards 0.

We can access circularity using ``ImageRegion::get_circularity()``.

## 4.7 Holes

While already mentioned it is instructional to define what a hole is. A hole is an area of pixels who are *not* part of the region, where that are is enclosed by the region. When boundary tracing ``crisp`` implicitely computes the boundaries of each hole and thus the number of holes. We can access either using:

```cpp
size_t get_n_holes() const;
const std::vector<std::vector<Vector2ui>> get_hole_boundaries() const;
```

## 4.8 Moment Invariants

We've seen earlier that somehow respresenting a regions unique shape in a way that is invariant to scale, translation and rotation is highly valuable. A very powerful way to achieve is with the *nths moment invariant*. While some of them have a conceptual meaning it is best to just think of them as properties that may not be useful to humans but do represent the shape of a region uniquely. ``crisp`` offers the first 7 moment invariants, accessed with ``ImageRegion::get_nths_moment_invariant(size_t n)`` where n in {1, 2, 3, ..., 7}.

The following table summarizes the response of a moment invariant to translation, scale, rotation and mirroring. "Unchanged" means the value of the moment does not change, "does change" means the value is altered after the corresponding transformation:

```cpp
N       translation     scale           rotation        mirroring
--------------------------------------------------------------------
1       unchanged       unchanged       unchanged       unchanged
2       unchanged       unchanged       unchanged       unchanged
3       unchanged       unchanged       unchanged       unchanged
4       unchanged       unchanged       unchanged       unchanged
5       unchanged       does change     unchanged       unchanged
6       unchanged       does change     unchanged       does change
7       unchanged       does change     slight change   changes sign
```

We note that all first 4 moments are completely independent of translation, scale, rotation and mirroring of the region and are thus highly valuable in representing a region

## 5. Texture Descriptors

Until now we have used descpritors that either describe a regions shape or, both it's shape and values. In this section we will deal with only a regions value. *Texture* has no agreed on definition but we can intuitively describe it as rate of change, value range and periodicity of the intensity values of a region. 

## 5.1 Co-Occurence Matrix

To quantify texture we first need to construct the *co-occurence matrix*. The co-occurence matrix is a matrix of size 256x256. It counts for each intensity pair i_a, i_b the number of times two pixels a, b that are next to each other have the corresponding intensities. When constructing the co-occurence matrix we need to supply a direction. Let a = (x, y) and b = (x + i, y + j) the pixels of the corresponding intensities i_a, i_b then the direction are as follow:

```cpp
Direction           a           b
-------------------------------------
PLUS_MINUS_ZERO    (x,y)        (x,   y-1)
PLUS_45            (x,y)        (x+1, y-1)
PLUS_90            (x,y)        (x+1, y)
PLUS_125           (x,y)        (x+1, y+1)
PLUS_MINUS_180     (x,y)        (x,   y+1)
MINUS_125          (x,y)        (x-1, y+1)
MINUS_90           (x,y)        (x-1, y)
MINUS_45           (x,y)        (x-1, y-1)
```

Intuitive the represents the pattern in on direction. We can quantify the distribution of intensity value pairs occurences using the following three descriptors:

## 5.2 Maximum Response

The maximum response is the probability of the most common intensity value pair. It is a number in [0, 1], where 1 is a region that only has a single value.

We can get the maximum response directly using ``ImageRegion::get_maximum_intensity_probability(CoOccurenceDirection)``

## 5.3 Intensity Correlation

A measure of how correlated the intensities are is defined as the mean over all intensities pairs respective correlation. This measure can be thought of as quantifying how regular a pattern is, the higher the mean correlation, the more often the same intensitiy values pairs in that direction occurr. It's values are in [-1, 1]

We can the mean intensity correlation using ``get_intensity_correlation(CoOccurenceDirection)``.

## 5.4 Uniformity

Uniformity is a measure of how constant the intensity values are. For a constant region the uniformity is 1, for a more divers region the values are closer to 0.

We can access uniformity using ``get_uniformity(CoOccurenceDirection)``

## 5.5 Homogeneity

In the co-occurence matrix the diagonal resprents occurences of intensity pair i_a, i_b where i_a = i_b. Homogenity quantifies how many of the intensity pairs like this they are and how close the overall region is to achieving full homogenity. It's value is in [0, 1] and we can access the value using ``get_homogenity(CoOccurenceDirection)``. 

## 5.6 Entropy

Just like in statistics, this measures the randomness of the co-occurence distribution where a value of 1 means all pairings are uniformly distributed, the less uniform the distribution the closer the value is to 0. We compute the entropy using ``get_entropy(CoOccurenceDirection)``.

## 5.7 Contrast

Contrast measures the difference between co-occuring pixels. ``get_contrast(CoOccurenceDirection)`` computes it for the given direction and returns a value in [0, 1]




