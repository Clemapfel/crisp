# Segmentation and Edge Detection

Thresholding, Clustering, Edge Detection & Linking

## Table of Contents

## 1. Introduction

Segmentation is the act of dividing image space into *segments*, best though of a a set of unique pixel coordinates. In ``crisp``, segments are literally just that:

```cpp
struct PixelCoordCompare
{
    bool operator()(const Vector2ui& a, const Vector2ui& b) const
    {
        return a.y() != b.y() ? a.y() < b.y() : a.x() < b.x();
    }
};

using ImageSegment = std::set<Vector2ui, PixelCoordCompare>;
```

Here we see that an ImageSegment is just an std::set of pixel coordinates (2-element vectors) that are sorted left-to-right, top-to-bottom. ``crisp::ImageSegment`` should not be confused with ``crisp::ImageRegion`` which is more than just a set of pixel coordinates, to learn more about the difference of segments and regions, consider [feature extraction tutorial](../feature_extraction/feature_extraction.md).

## 1.1 Extracting Segments

We can *decompose* any image into it's segments using the following functions 

```cpp
template<typename Image_t>
std::vector<ImageSegment> decompose_into_segments(const Image_t&, size_t);

template<typename Image_t>
std::vector<ImageSegment> decompose_into_connected_segments(const Image_t&, size_t);
```

``decompose_into_segments`` returns a set of segments that only fullfill the following condition: All pixels pointed to by the pixel coordinates in the segment must have the same value. So for a binary image we would get 2 segments, one for all white pixel and one for all black pixels. For a grayscale image that has 255 different values we would get 255 different segments. Similarly in color and higher dimensions.

``decomponse_into_connected_segments`` decomposes an image into segments such that not only do all the pixel associated with each segment have to have the same value but there also must be a path of *4-connected* pixels between any two pixels in the segment. 

We will illustrate the difference using an example. Consider the following binary image:

![](./blobs.png)

We spot multiple disconnected "blobs" in white on a black background. 

We now decompose this image and then create a colored image that for each different segment colors them in a different color:

```cpp
// load 
auto blobs = load_binary_image(/*...*/ + "/crisp/docs/segmentation/blobs.png");

ColorImage final_segmentation;
final_segmentatin.create(image.get_size().x(), image.get_size().y());

// decompose
auto segments_unconnected = crisp::decompose_into_segments(image);

// color in fo visualization
float step = 1.f / segments_unconnected.size();
size_t i = 0;

for (auto& segment : segments_unconnected)
{
    for (auto& px : segment)
        segmented(px.x(), px.y()) = HSV{step * i, 1, 1}.to_rgb();

    i++;
}
```

Can you guess what the resulting image will look like?<br>

![](./unconnected_segments.png)

The algorithm returned two segments, one marked in red and one marked in cyan. These indeed correspond to the black and white pixels in the image respectively. If we repeat the same process but now want *connected segments*:

```cpp
auto segments_unconnected = crisp::decompose_into_connected_segments(image);
``` 
The result is the following:<br>
![](./connected_segments.png)

The algorithm here found 4 different segments, the background in red, 3 blobs in green, cyan and purple and the purple blob is barely connected enough for the "ball" towards the bottom of the image to still count towards the same segment.

We can use the segments like this in various way but this is not the purpose of this tutorial. Rather, we want to know how we can simplify an image in a way that makes segmentation sensible and easy. (we will learn how to then process the segments in the before mentioned [feature extraction tutorial](../feature_extraction/feature_extraction.md))

## 2. Thresholding

Thresholding means to translate a 1-dimensional (grayscale) image ``I`` into a binary image ``B`` by applying a threshold ``t`` to it such that:

```cpp
if (I(x, y) > t)
    B(x, y) = true;
else 
    B(x, y) = false;
```

The algorithms presented here all concern themself with how to determine t. 

## 2.1 Manual Thresholding

Consider this image of a chessboard:

![](./chessboard.jpg)

We note the non-uniform illumination from the cameras flash as well as the shadows on top side of it.

We want to threshold it ideally such that the white pieces and squares are in the ``true`` (white) region of the resulting binary image while the black pieces and squares are in the ``false`` (black region). The computationally easiest way to do this is to just have a human guess a decent threshold. We can do this using ``crisp::Segmentation::manual_threshold(const Image<T, 1>&, float)``

```cpp
auto image = load_grayscale_image(/*...*/ + "/crisp/docs/segmentation/chessboard.png");

// apply 0.5 threshold
auto binary = Segmentation::manual_threshold(image, 0.5);



