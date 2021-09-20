# Segmentation and Edge Detection

Thresholding, Clustering, Edge Detection & Linking

## Table of Contents

1. [Introduction](#1-introduction)<br>
    1.1 [Extracting Segments](#11-extracting-segments)<br>
2. [Thresholding](#2-thresholding)<br>
    2.1 [Manual Thresholds](#21-manual-threshold)<br>
    2.2 [Basic Automated Thresholding](#22-basic-threshold)<br>
    2.3 [Otsu's Method](#23-otsus-method)<br>
    2.4 [Variable Threshold](#24-variable-threshold)<br>
    2.5 [Neighborhood Threshold](#25-neighborhood-threshold)<br>
3. [Edge Detection](#3-edge-detection)<br>
    3.1 [Threshold Sobel Gradient Magnitude](#31-threshold-sobel-gradient)<br>
    3.2 [Canny's Algorithm](#32-canny)<br>
4. [Clustering](#4-clustering)<br>
    4.1 [k-means Clustering](#41-k-means-clustering)<br>
    4.2 [Superpixel Clustering](#42-superpixel-clustering)<br>
    4.3 [Clustering by Region Growing](#43-region-growing-clustering)<br>
   
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

Consider the following two images of a bird:<br>

```cpp
#include <segmentation.hpp>
#include <system/image_io.hpp>

auto uniform = load_grayscale_image(/*...*/ + "</crisp/docs/segmentation/opal_clean.png");
auto non_uniform = load_grayscale_image(/*...*/ + "/crisp/docs/segmentation/opal_non_uniform.png");
```

![](./uniform.png)
![](./non_uniform.png)

While the first version is clear the second image was manually shaded with a radial gradient, this simulates non-uniform lighting which is a common occurence in praxis. We will threshold both of these images to compare the relative performance of each thresholding algorithm, each of which has a different way of determine the threshold.

## 2.1 Manual Threshold

The computationally easiest way to threshold an image is to just have a human guess a decent threshold and specify it. We can do so using ``crisp::manual_threshold(const Image<T, 1>&, float)``:

```cpp
auto uniform_thresholded = manual_threshold(uniform, 0.5f);
auto non_uniform_thresholded = manual_threshold(non_uniform, 0.5f);
```

![](./manual_01.png)
![](./manual_02.png)

``manual_threshold`` is the fastest of the thresholding methods and especially if there is ample control over the type of image it should not be discarded in favor of the automated thresholding operations.

## 2.2 Basic Threshold

``crisp::basic_threshold`` finds a threshold by iteratively moving the threshold to the median between the two clusters until convergence is achieved. Unlike later methods the resulting threshold is not guruanteed to be optimal in terms of in-between cluster variance but it's relative computational simplicity and relative good results make it a decent choice nonetheless

![](./basic_01.png)
![](./basic_02.png)

We do note that for non-uniform lighting this method was not able to discern both the "S" and "A" of the text in the image.

## 2.3 Otsu's Method

``crisp::otsu_threshold`` uses otsu's method which is guruanteed to find the threshold that maximizies in-between class variance. While it is statistically optimally in praxis in tends to be slightly slower than ``basic_threshold`` but if high quality results are of importance it Otsu's method should be preferred

![](./otsu_01.png)
![](./otsu_02.png)

We again weren't able to isolate all letters. This tends to be a problem with global thresholding algorithms, that is an algorithm that applies the same threshold to all pixels in the image. When non-uniform lighting is present we instead have to resort to the following two methods:

## 2.4 Variable Threshold

This methods visits each pixel in the image in a spiral pattern, a certain amount of already-visited pixels is used for estimating the current threshold. Because the "tail" of the current pixel changes orientation as the algorithms progresses, this thresholding method is more resistant to non-uniform lightning. 
The algorithm takes one parameter called that governs the length of the "tail". It is recommended to keep this in the range [0.01, 1], the lower the number the shorter the tail and the shorter the tail the more resisent the method is to non-uniform lighting. 

```cpp
auto uniform_threshold = variable_threshold(uniform, 0.5f);
auto non_uniform_thresholded = variable_threshold(uniform, 0.1f);
```

While it is computationally still quite fast the results overall are far lower quality compared to the global thresholding methods as evident from the prominent artifacting:

![](./variable_01.png)
![](./variable_02.png)

We note however that this is the first method that came very close to succesfully isolating all letters of the word "sample". 

## 2.5 Neighborhood Threshold

This method computes a local threshold for each pixle based on samples from the pixels neighborhood. Like ``variable_threshold``, this time it governs the size of the neighborhood sampled. The bigger it is the more accuracte each local threshold will be but this will cause a significant decrease in performance. When left unspecified (as in the following example) a heuristic chooses the neighborhood size automatically:

```cpp
auto uniform_threshold = neighborhood_threshold(uniform);
auto non_uniform_thresholded = neighborhood_threshold(uniform);
```

![](./neighbor_01.png)
![](./neighbor_02.png)

While we notice strong artifacting in areas of relative constant intensity the method isolated both the boundaries of the bird and each letter of the text clearly. It is therefore recommend as the most robust for both uniform and non-uniform lighting, however in applications where performance is important it may be more appropriate to choose any of the other thresholding methods and pre-process the image to negate effects of non-uniform lighting

## 3. Edge Detection

Edge detection is the highlighting elements of the image where the gradient (the change in intensity along a specific direction) is highest. This ideally results in binary image who's lines follow the outer perimeter of the objects in the image. ``crisp`` offers 3 different methods for edge detection, to demonstrate them let's consider this familiar image of a bird:

![](./opal_clean.png)

Here we are operating on a color image because unlike thresholding, edge detectiong works in an arbitrary number of dimensions

## 3.1 Threshold Sobel Gradient

For this method we first gradient response in both direction of the image using the sobel operator as provided by ``SpatialFilter::sobel_gradient_/*...*/``. Then we compute the gradient magnitude and threshold the resulting image, leaving only the strongest edge responses. 

```cpp
#include <edge_detection.hpp>
using namespace crisp;

auto image = load_color_image(/*...*/ + /crisp/docs/segmentation/opal_clean.png);

auto edges = EdgeDetection::threshold_gradient(image);
```
If we want to we can manually specify a threshold using the overload ``threshold_gradient(Image<T, N>, float threshold)`` or we can leave it unspecified and instead let otsus method find the optimal threshold of the gradient magnitude image. Having done the latter we get:

![](./edge_sobel.png)

While the edges of the letters were well preserved and the bird is reasonably accurately represented the rope in the left of the image and especially the plant material object the bird is sitting on are too noisy. It may be possible to pre-process the image to reduce this response, however ``crisp`` offers another edge detection algorithm for just this occasion.

## 3.2 Canny

Cannys algorithm combines threshold the gradient response with post-processing the edges. A somewhat complicated scheme first eliminates edges least likely to be part of the boundary and then post-processes them to a) reduce their width to 1 pixel and b) link them if possible. 

```cpp
auto edges = EdgeDetection::canny(image);
```

![](./edge_canny.png)

While we notice some distortion of the letter boundaries, each individual straw is accurately resprented and it is clear that this algorithm returns much cleaner results. However this comes with a slight performance decrease as this operation took about twice as long as simply thresholding the sobel gradient. However to have the best change at generating high quality edges, the canny algorithm is recommended.

## 4. Clustering

Clustering in image processing means to assign each pairwise different value in the image to a cluster. Then the algorithm returns an image where each images intensity was replaced by it's clusters intensity. Ideally our clusters should adhere to boundaries and the clusters assigned to each intensity should make sense conceptually. Consider the following image:<br>

![](./rainbow_lorikeet.jpg)<br>

If we were to ask a human to isolate relevant regions of this image they would likely consider them to `e:
+ the out of focus background
+ the branch
+ each different region of coloration on the birds plumage
+ the birds beak

``crisp`` offers two automated algorithms for image clustering, we will judge their effectiveness against our conceptual version of what the image should be in the following examples

## 4.1 k-means Clustering

[k-means clustering](https://en.wikipedia.org/wiki/K-means_clustering) in theory should assign each pixel to a cluster such that for the sum of all pixels to their respective cluster is minimal. Properly solving this problem is NP-hard which is why a wide variety of heuristics are available. ``crisp`` uses the euclidian distance in n-dimensions (where n the number of planes for the image, 3 in our case) as a distance and employs a custom heuristic for color images. We still need to manually specify the number of clusters ourself however:

```cpp
auto image = load_color_image(/*...*/ + "/crisp/docs/segmentation/rainbow_lorikeet.jpg");

auto result = k_means_clustering(image, 5);
```

![](./k_means_inf.png)

We observe that both the orange, green and blue plumange are represented, the branch was isolate from the background however the feet of the bird have been assigned to the two shaders representing the branch. 

While this result is satisfactory this this function call took about 20s to complete. This is because by default ``crisp`` clustering will run until convergence is achieved. We can limit the time it takes to finish by specifying an optional third argument ``number_of_iterations``. This limits the number of passes the algorithm is allowed to do, vastly improving runtime but resulting in a lower quality result. 

```cpp
auto result = k_means_cluster(image, 5, 1);
```
![](./k_means_1.png)<br>
```cpp
auto result = k_means_cluster(image, 5, 2);
```
![](./k_means_2.png)<br>
```cpp
auto result = k_means_cluster(image, 5, 3);
```
![](./k_means_3.png)<br>
```cpp
auto result = k_means_cluster(image, 5, 5);
```
![](./k_means_5.png)<br>
```cpp
auto result = k_means_cluster(image, 5, 10);
```
![](./k_means_5.png)<br>

We note that after 10 iterations we are already pretty close to the final result, even though the algorithm took 34 iterations to finish when left at default configuration.

## 4.2 Superpixel Clustering

Superpixel clustering is a variant of the k-means algorithm where cluster centers are chosen as located on a regular grid, then for each "superpixel" (cluster) we investigate pixels in a limited area around it. This way as the algorithm reaches convergence each superpixel ideally adheres to the boundary and shapes that happen to intersect with it's boundaries.

Like k-means we need to specify the number of superpixels, for n superpixels each resulting superpixel will have ``image.get_size().x() * image.get_size().y() / n`` many pixels. We can again specify an optional third argument limiting the number of iterations. Executing the algorithm without limiting the number of iterations result in the following image:

![](./superpixel_inf.png)

We note an almost higher quality result than the k-means version, each plumage color is represented, the beak and even eyes are seperate from their surrounding. The improve result is because of the higher resultion in terms of number of clusters, without sacrificing performance we get a much higher resolution result. 

## 4.3 Region Growing Clustering

TODO






