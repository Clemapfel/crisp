[<< Back to Index](../index.md)

---
# Filtering in the Spatial Domain

Filter Kernels & Convolution 

## Table of Contents

# 1. Introduction

Spatial filters are central to the field of image processing and features in many of the most use algorithms both in the literature and in ``crisp``. They are represented by ``crisp::SpatialFilter``. A filter in ``crisp`` has to components: it's *kernel* and *evaluation function*. 

# 2. Kernels

A kernel is a m*n matrix of floats. It behaves exactly like a matrix and unlike images operations on it are matrix operations. Indeed, looking in [spatial_filter.hpp] we find that filter kernels are just a typedef for ``Eigen::Matrix``:

```cpp
using Kernel = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;
```

Keep this in mind, the link to Eigen is explicitely mentioned here because it opens a vast array of numerical techniques that are not obvious just through it's ``crisp`` interface. Please consult the [eigen documentation](https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html) in the case that their matrix class is not yet familiar. Henceforth we assume that any reader is familiar with comma initialization and element-access of ``Eigen::Matrix``.

Other than functions supplied by eigen, ``crisp`` offers these external functions for operating on them:

```cpp
// make it sum of elements is 1
void normalize(Kernel&);

// rotate a kernel 90°
void rotate(Kernel&, size_t n_90_degrees);

// convolute one kernel with another
Kernel convolute(Kernel left, Kernel right);

// seperate a kernel numerically
bool seperate(const Kernel& original, Kernel* out_left, Kernel* out_right);
```

# 2.1 Normalize a Kernel

``normalize`` modifies the kernels element so it's sum-of-elements is 1. This can be useful to control a spatial filters response. To use it we first contstruct a kernel

```cpp
// in main.cpp
#include <spatial_filter.hpp>
#include <iostream>

using namespace crisp;

auto kernel = Kernel();
kernel.resize(3, 3);
kernel << 1, 1, 1,
          1, 1, 1,
          1, 1, 1;

normalize(kernel);

std::cout << kernel << std::endl;

float sum_of_elements = 0;
for (size_t x = 0; x < kernel.rows(); ++x)
for (size_t y = 0; y < kernel.cols(); ++y)
    sum_of_elements += kernel(x, y);

std::cout << "sum after normalization: " << sum_of_elements << std::endl;
```

```
0.111111 0.111111 0.111111
0.111111 0.111111 0.111111
0.111111 0.111111 0.111111

sum after normalization: 1
```

# 2.2 Rotate a Kernel

We can use ``crisp::rotate`` a kernel by 90° *counter-clockwise*. We can specify a number n such that the kernel is rotate by n * 90° degree:

```cpp
kernel << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;

for (size_t i = 0; i < 4; ++i)
{
    rotate(kernel, i);
    std::cout << kernel << "\n" << std::endl;
}
```

```
3 6 9
2 5 8
1 4 7

9 8 7
6 5 4
3 2 1

7 4 1
8 5 2
9 6 3

1 2 3
4 5 6
7 8 9
```

## 2.3 Seperate a Kernel

It is often preferrable to convolute a kernel with an image after seperating it, for a 3x3 kernel M we may seperate them into kernel A (3x1) and B (1x3) such that A * B = M (this is matrix-matrix multiplication). 

```crisp::seperate``` takes three arguments:
+ ``const Kernel& original`` is the kernel we want to seperate
+ ``Kernel* out_left`` is the left side of the expression, A in our example 
+ ``Kernel* out_right`` is the right side of the expression, B in our example

The function furthermore returns ``true`` if seperation was possible in which case ``out_left`` and ``out_right`` will be assigned to, if ``false`` is returned the kernel is not seperable and both ``out_left`` and ``out_right`` will be set to ``nullptr``.

Let's consider the following kernel:

```
 1  0 -1
 2  0 -2
 1  0 -1
```
This kernel is seperable, [wikipedia lists](https://en.wikipedia.org/wiki/Sobel_operator#Formulation) it's resulting A and B as {{1}, {2}, {3}} (a 1x3 kernel, A) and {+1, 0, -1} (a 3x1 kernel, B). Multiplying them in our head we can verify that this is indeed a valid seperation. 

We now try to seperate the kernel in ``crisp``:

```cpp
kernel << 1, 0, -1,
          2, 0, -2,
          1, 0, -1;

Kernel left, right;
if (not seperate(kernel, &left, &right))
    // handle non-seperable kernels

std::cout << left << "\n" << std::endl;
std::cout << right << "\n" << std::endl;

// verify result
kernel = left * right;
std::cout << kernel << std::endl;
```

```
// left
-1.41421
-2.82843
-1.41421

// right
-0.707107 0 0.707107

// left * right
 1 -0 -1
 2 -0 -2
 1 -0 -1
```
``crisp`` happened to find a different seperation, yet validating it by computing ``left * right`` (A*B) confirms that it is also valid. *Seperations are not unique* because a) we're numerically approximating them. For this reason it is often preferrable to find an analytical seperation on that uses simpler values on paper if possible, if not then we can use ``crisp::seperate`` to automate the process.

For beginners in the field of image processing or linear algebra or for kernels that are very big or numerically assembled it is often unclear wether they are seperable or not. ``crisp::seperate`` will immediately exit once it determined that a kernel is not seperable so it can also be used to simply determine if it can be done with not performance overhead.

## 2.4 Combining two Kernels

Convolution is associative, that is for Kernels K1, K2 and Image I where ``°`` is the convolution operator:
``K1 ° (K2 ° I) = (K1 ° K2) ° I``<br>

or in plainer english: the convoluting I with kernel K1 and then convolution I with kernel K2 is the same as first convoluting the two kernels K1, K2 and then convoluting the resulting kernel with the image. 
This is important as our kernels are usually 3x3 of size while the image is much larger so we want to combine our kernels as much as possible. We can do so with ``crisp::convolute(Kernel, Kernel)``

```cpp
kernel << -1, -1, -1, 
          -1,  8, -1,
          -1, -1, -1;

kernel = convolute(kernel, kernel);
std::cout << kernel << std::endl;
```

```
-14 -12 -14
-12  72 -12
-14 -12 -14
```

## 3. ``crisp::SpatialFilter``
## 3.1 Specifying the Kernel
Now that we know how to create and modify a kernel we can assign it to a filter. First we need to create the filter, then we can assign a kernel to it via ``set_kernel``

```cpp
auto filter = SpatialFilter();
filter.set_kernel(kernel);
``` 

After binding the kernel to the filter we can modify it using:

float& operator()(size_t x, size_t y);

```cpp
// members of crisp::SpatialFilter
float& operator()(size_t x, size_t y);
float operator()(size_t x, size_t y) const;
``` 

These are simply the same access operators as those use on the kernel itself. 

### 3.2 Specifying the Evaluation Function

Before we can apply this kernel to an image we need to specify the evaluation function. By default this is the familiar convolution however ``crisp`` offers 4 other functions in addition to that. To illustrate how they work consider the following 3x3 kernel, image segment of image ``in`` and resulting image ``out``

```cpp
// Kernel
0.5  0.5  0.5 
0.5 (0.5) 0.5
0.5  0.5  0.5

// in(i, j) = 5
1  2  3 
4 (5) 6
7  8  9
```
<br>

+ ``CONVOLUTION`` computes sum of elements weighted by the kernel at a specified image position, while the kernel is centered on that position<br>
```  
0.5 * 1 + 0.5 * 2 + ... + 0.5 * 9 = 22.5
out(i, j) = 22.5
```
+ ``NORMALIZED_CONVOLUTION`` computes the convolution and then devides the weighted sum by the sum of kernel elements <br>

```
0.5 * 1 + 0.5 * 2 + ... + 0.5 * 9 = 22.5
22.5 / (0.5 + 0.5 + ... + 0.5) = 5
out(i, j) = 5
```
+ ``MINIMUM`` returns the minimum of elements, weighted by the kernel

```
min{0.5 * 1, 0.5 * 2, ..., 0.5 * 9} = 0.5 * 1 = 0.5
out(i, j) = 0.5
```

+ ``MAXIMUM`` returns the maximum of elements, weighted by the kernel

```
max{0.5 * 1, 0.5 * 2, ..., 0.5 * 9} = 0.5 * 9 = 4.5
out(i, j) = 4.5
```

+ ``MEDIAN`` returns the median of the elements, again weighted by the kernel

```
median{0.5 * 1, 0.5 * 2, ..., 0.5 * 9} = 
median{0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5} = mean(2, 2.5) = 2.25
out(i, j) = 2.25
```
<br>
We can specify the evaluation function by using:

```cpp
auto filter = SpatialFilter();
filter.set_evaluation_function(SpatialFilter::EvaluationFunction::NORMALIZED_CONVOLUTION);
``` 
For most scenarios convolution or normalized convolution will be sufficient but some tasks can be simplified by using the more specialized evaluations functions.

## 3.3 Applying the Filter
We now have all the pieces together to apply a filter. Let's consider this image of a bird:

![](./color_opal.png)

After loading it as a grayscale immage we can apply the filter to it:

```cpp
// load image
auto image = load_grayscale_image("/home/clem/Workspace/crisp/docs/spatial_filters/color_opal.png");

// create kernel
auto kernel = Kernel();
kernel.resize(5, 5);
kernel.setConstant(1);  // 5x5 kernel where all values are 1

// set filter and specify kernel
auto filter = SpatialFilter();
filter.set_kernel(kernel);
filter.set_evaluation_function(filter.CONVOLUTION);

filter.apply_to(image);
```

![](./opal_initial_filter_artifacting.png)

The image is completely distorted, this is because the sum of elements for the filter is not 1, rather as ever element is 1 and the filter is of size 5x5, it's sum of elements is 25. This results in the resulting image values being far outside the [0, 1] required for rendering. If we instead specify the evaluation function as 

```cpp
filter.set_evaluation_function(filter.NORMALIZED_CONVOLUTION);

// or equivalently:
normalize(kernel);
filter.set_kernel(kernel);
filter.set_evaluation_function(filter.CONVOLUTION);
```

![](./opal_initial_filter.png)

The operation results in the expected result, a blurred image

## 3.4 Applying the Filter in All Dimensions

We've previously loaded a colored image as grayscale and then filtered the grayscale image. In ``crisp`` we can filter all images, regardless of value type or dimensionality. For example we can repeat the previous operations but on the color image instead:

```cpp
// load image as color
auto image = load_color_image("/home/clem/Workspace/crisp/docs/spatial_filters/color_opal.png");

auto kernel = Kernel();
kernel.resize(5, 5);
kernel.setConstant(1);

auto filter = SpatialFilter();
filter.set_kernel(kernel);
filter.set_evaluation_function(filter.NORMALIZED_CONVOLUTION);

filter.apply_to(image);
```

Literally nothing else changes, all kernels are applicable to all images. The result is exactly what we expect it to be, each plane of the image was blurred individually:

![](./opal_initial_filter_color.png)

# 4. Filter Kernel Types

It would of course be quite laborious to specify each kernel manually, instead ``crisp`` provides a breadth of kernels such that users can conveniently access the most used ones. Instead of listing them all, as the names can be quite hard to recognize for people who aren't as familiar with the literature, a demonstration of each available kernel will follow here. Each kernel is assumed to be of size n*n unless stated otherwise. We're furthermore applying ``_CONVOLUTION`` (with no normalization) in each of these examples unless stated otherwise. Furthermore we will state for each kernel wether they can be seperated and again emphasize how necessary it is to do so if performance is of importance

## 4.1 Identity

(is seperable)

The simplest kernel is the identity kernel, it projects and image onto itself. It takes one argument: n, such that the resulting kernel will be of size n*n

```cpp
filter.set_kernel(filter.identity(3));

0 0 0
0 1 0
0 0 0
```
![](./identity.png)

## 4.2 One

(is seperable)

This kernel is again of size n*n and each element is assigned as 1. This filter is sometimes also called "box filter" or "box blur" because it tends to blur images it is applied to (after normalization). We can intensify the blur by increasing the kernels size, for n = 3 the blur is very slight.

```cpp
filter.set_kernel(filter.one(3));

1 1 1
1 1 1
1 1 1
```

![](./one.png)<br>

And after normalization:<br>
![](./one_normalized.png)



## 4.3 Zero

(is seperable)

This kernel is similar to ``one`` but instead of all elements being 1, all elements are instead set to 0. This filter is rarely applied to an image as it will simply result in an all-black result.

```cpp
filter.set_kernel(filter.zero(3));

0 0 0
0 0 0
0 0 0
``` 

![](./zero.png)

## 4.4 Box

(is seperable)

Not to be confused with ``one``. ``box`` takes two arguments, it's dimensions n and a constant c such that all elements will be set to c. This means ``box(n, 1)`` is equivalent to ``one(n)`` and ``box(n, 0)`` is equivalent to ``zero(n)``.

```cpp
filter.set_kernel(filter.box(3, 0.5f));

0.5 0.5 0.5
0.5 0.5 0.5
0.5 0.5 0.5
``` 

![](./box.png)<br>

And after normalization:<br>
![](./box_normalized.png)

## 4.5 Normalized box

(is seperable)

Unlike the box kernel, the normalized box kernel takes only n as the argument, all components are assigned such that the sum of components is 1

```cpp
filter.set_kernel(filter.normalized_box(3));

0.333333 0.333333 0.333333
0.333333 0.333333 0.333333
0.333333 0.333333 0.333333
```

![](./normalized_box.png)

## 4.6 Gaussian

(is seperable)

The gaussian kernel is popular for smoothly blurring images, while it does incurr a performance overhead due to floating point values, the resulting blurred image is less "blocky" compared to a box filter.

The function takes 1 argument, it's size n and returns an already normalized kernel sampling a gaussian distribution, with it's center being the center of the kernel.

```cpp
filter.set_kernel(filter.gaussian(5))

0.0362704 0.0384838 0.0394031 0.0384838 0.0362704
0.0384838 0.0417802 0.0435471 0.0417802 0.0384838
0.0394031 0.0435471  0.048127 0.0435471 0.0394031
0.0384838 0.0417802 0.0435471 0.0417802 0.0384838
0.0362704 0.0384838 0.0394031 0.0384838 0.0362704
```
As the number can be quite hard to parse visually, we create a 150x150 kernel and render it via ``crisp::Sprite``:<br>

![](./gaussian_visualized.png)<br>

We note the expected gaussian dome.
Applying the 5x5 filter above to the image we get:<br>

![](./gaussian.png)

## 4.6 Laplacian First Derivative

(not seperable)

Also called "laplacian" computes the discrete first derivative of a 2d image functnormalize(image)
ion. Because it is the derivative ewe expect it to be zero in areas of constant intensity, non-zero at the start of an intensity ramp and nonzero along said ramps. This is identical to the analytical first derivative.  normalize(image)

```cpp
filter.set_kernel(filter.laplacian_first_derivate());

-1 -1 -1
-1  8 -1
-1 -1 -1
``` 

The resulting image is initially hard to read: <br>
![](laplacian_first.png)<br>

But after normalization:<br>
![](laplacian_first_normalized.png)<br>
We note the typical sharp mask. Multipliying this image with the original result in sharpening of the original.

## 4.6 Laplacian Second Derivative

(not seperable)

The second derivative is the derivative of the first derivative. Just like in analysis we expect the second derivative to be zero in areas of constant intensity, nonzero at the onset *and* end of intensity ramps, zero along inensity ramps. These properties make it quite useful for edge detection.

```cpp
filter.set_kernel(filter.laplacian_second_derivative())

-9 -1 -9
-1 72 -1
-9 -1 -9
```

Due to the overall noisyness of the image the image is almost illegible:

![](./laplacian_second.png)<br>

But after normalization:<br>

![](./laplacian_second_normalized.png)

## 4.7 Laplacian of Gaussian

(not seperable)

The Laplacian of Guassian, often called "LoG" is a commonly used kernel that is the derivative of a guassian:<br>

![](https://homepages.inf.ed.ac.uk/rbf/HIPR2/figs/logcont.gif)<br>
source: [https://homepages.inf.ed.ac.uk/rbf/HIPR2/log.htm](https://homepages.inf.ed.ac.uk/rbf/HIPR2/log.htm)

Applying it by multiplying the resulting image with original can be interpreted as first blurring, then sharpening the image. It thus finds wide applicability in edge enhancement. Just like the gaussian, the LoG kernel can have arbitrary size:

```cpp
filter.set_kernel(filter.laplacian_of_gaussian(5));

0.171415  0.108386    0.111149   0.108386    0.171415
0.108386  0.00697621  0.0132247  0.00697621  0.108386
0.111149  0.0132247   0.0437069  0.0132247   0.111149
0.108386  0.00697622  0.0132247  0.00697622  0.108386
0.171415  0.108386    0.111149   0.108386    0.171415
```
After normalization: <br>
![](./log_normalized.png)

## 4.8 Gradient Kernels

Gradient kernels attempt to compute the [image gradient](https://en.wikipedia.org/wiki/Image_gradient) best understood as the rate of change along a specified direction. There are multiple kernels to do this and it is important to keep in mind that we always have to specify the direction of the gradient. In ``crisp`` "x-direction" referers the left-to-right gradient, "y-direction" to top-to-bottom

## 4.8.1 Simple Gradient

(not seperable)

The simplest gradient kernels in the x and y direction respectively are:

```cpp
filter.set_kernel(filter.simple_gradient_x())

-1
 1
``` 
![](simple_gradient_x.png)

```cpp
filter.set_kernel(filter.simple_gradient_y())

-1  1
``` 
![](simple_gradient_y.png)

<br>
White or black areas correspond to a strong positive or negative increase in intensity respectively while grey (around 0.5) corresponds to relatively constant areas. 

### 4.8.2 Roberts Gradient

(is seperable)

Roberts tried to iterate on the gradient by specifying two kernels that represent change in 4 instead of two:

```cpp
filter.set_kernel(filter.roberts_x());

-1  0
 0  1
```
![](./roberts_x.png)

```cpp
filter.set_kernel(filter.roberts_y());

 0 -1
 1  0
```
![](./roberts_y.png)

### 4.8.3 Prewitt Gradient

(is seperable)

Prewitt improved on Robets with a bigger kernel in an attempt to get a more detailed gradient response:

```cpp
filter.set_kernel(filter.prewitt_x());

-1 -1 -1
 0  0  0
 1  1  1
```

![](./prewitt_x.png)

```cpp
filter.set_kernel(filter.prewitt_y());

-1  0  1
-1  0  1
-1  0  1
```

### 4.8.4 Sobel

(is seperable)

Sobel combined the roberts kernel with a smoothing kernel in an attempt to get a gradient response less susceptible to noise

```cpp
filter.set_kernel(filter.sobel_x());

 1  2  1
 0  0  0
-1 -2 -1
```

![](./sobel_x.png)

```cpp
filter.set_kernel(filter.sobel_y());

 1  0 -1
 2  0 -2
 1  0 -1
```

![](./sobel_y.png)<br>

In the end it is up to the end user to decide which of these gradients should be user. When bleeding edge performance isn't necessary, ``crisp`` prefers to use the sobel kernel over roberts or prewitt due to it's comparable gradient response while being more stable in noisy conditions

### 4.8.5 Kirsch Compass

(is seperable)

Kirsch designed a set of kernels that measure the gradient response in a direction. The direction is given similar to the directions on a compass: north (n), north east (ne), east (e), south east (se), south (s), south west(sw) and west (w)). This set of kernel is useful if the direction of the gradient is of importance to the curren task

```cpp
// north
filter.set_kernel(filter.kirsch_compass_n());

 5 -3 -3
 5  0 -3
 5 -3 -3
```

![](./kirsch_n.png)

```cpp
// south
filter.set_kernel(filter.kirsch_compass_s());

-3 -3  5
-3  0  5
-3 -3  5
```

![](./kirsch_s.png)

```cpp
// east
filter.set_kernel(filter.kirsch_compass_e());

-3 -3 -3
-3  0 -3
 5  5  5
```

![](./kirsch_e.png)

```cpp
// west
filter.set_kernel(filter.kirsch_compass_w());

 5  5  5
-3  0 -3
-3 -3 -3
```

![](./kirsch_e.png)

## 4.9 In Summary

This concludes the overview of kernels available in crisp. Common operations such as image blurring, sharpening and edge detection and gradient direction are covered out of the box while optimization through seperation or combining kernels is made not only possible but easy. 

## 5. Using Other Evaluation Functions for Image Restoration

So far we have dealt with kernels that were applied via convolution or normalized convolution. This is the most common way to alter and image however as mentioned ``crisp`` supports other types of evaluation functions and their use will be illustrated in this section. 

Consider the following image, [manually corrupted](../../include/noise_generator.hpp) with noise:

![](./opal_salt_and_pepper.png)

The image is heavily with non-uniform noise. Closer examination reveals that the black and white dots are peaks of negative and positive intensitiy of up to 5 times the previous maximum sensor response. We could blur this image using a gaussian filter but the results would be poor as it does not address the noise directly. 

Instead we can use one of ``crisp``s other evaluation functions like so:

```cpp
auto image = /* corrupted image */;

auto filter = SpatialFilter();
filter.evaluation_function(filter.MEAN);
filter.set_kernel(filter.one(3));
filter.apply_to(image);

// render or save to disk
```

Here we're using a 3x3 kernel of all ones and using it to compute the mean of a all pixels in a 3x3 neighborhood. We choose ``one`` because we don't want the kernel to weight the image in any way. The resulting image looks much better:

![](./opal_salt_and_pepper_mean.png)

However on close examinatin we still not some noise representing, one particular noticeable one is around the lower abdomen of the bird. This is because taking the mean of a limited dataset tends to be quite sensitive to high spikes in intensity. If instead we apply the median, which tends to be more resistant:

```cpp
filter.evaluation_function(filter.MEDIAN);
filter.set_kernel(filter.one(3));
filter.apply_to(image);
```

![](./opal_salt_and_pepper_median.png)

We get a much better result with minimal distortion. There are still some pixels left, we can eliminate these by simply applying the same median-filter again:

```cpp
filter.apply_to(image);
filter.apply_to(image);
```

![](./opal_salt_and_pepper_median_2.png)

resulting in a fully noise-free image.

While not used here, ``MAXIMUM`` and ``MINIMUM`` also have their applications, most notably in non-maxima surpression and for certain types of image signal distortions.

---


























