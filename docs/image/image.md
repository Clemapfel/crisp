[<< Back to Index](../index.md)

---

# Images

Multi Dimensional Images of Arbitrary Type

## Table of Content

1. [**Introduction**](#1-introduction)</br>
2. [**Image I/O**](#2-image-io)</br>
    2.1 [Loading Images](#21-loading-images)</br>
    2.2 [Saving Images](#22-saving-images)</br>
3. [**Working with Images**](#3-working-with-images)</br>
    3.1 [Rendering Images](#31-rendering-images)</br>
    3.2 [Padding & Accessing Pixels](#32-accessing-individual-pixels)</br>
    3.3 [Creating Images](#33-creating-images)</br>
    3.4 [Image Iterators](#34-image-iterators)</br>
    3.5 [Image-Image arithmetic operators](#35-image-arithmetics)</br>
4. [**Multi Dimensional Images**](#4-multi-dimensional-images)</br>
    4.1 [Accessing Planes Directly](#41-accessing-planes-directly)</br>
5. [**A Final Example**](#5-a-final-example)</br>

## 1. Introduction

Images are obviously at the heart of image processing so it's important to understand them thoroughly. In ``crisp``, images are n-dimensional matrices of arbitrary value and arithmetic value type. Rather than storing n-matrices, crisp stores one matrix with each of it's element an n-dimensional vector:

```cpp
template<typename InnerValue_t, size_t N>
class Image
{
```

This class takes two template arguments: 
+ ``InnerValue_t`` is the value type of the elements of the vectors
+ ``N`` is the number of dimensions (the size of the vectors)

Conceptually it's easier to think of each image as n matrices, we will call each of these matrices a "plane". For example a 3-dimensional rgb image has three planes: red, green and blue.

Most of the time we won't be dealing with ``crisp::Image`` directly but rather with one of it's 3 implementations:
+ [``BinaryImage``](/include/image/binary_image.hpp) (inherits from ``Image<bool, 1>``)<br>
  a 1-dimension image where each value can either be white (true) or black (false). 
+ [``GrayScaleImage``](/include/image/grayscale_image.hpp) (inherits from ``Image<float, 1>``)<br>
  a 1-dimensional image where each value is a 32-bit float
+ [``crisp::ColorImage``](/include/image/binary_image.hpp) (inherits from ``Image<float, 3>``)<br>
  a 3-dimensional image where each value is a vector representing color in the RGB format. Each component of the vector (each planes pixels) are a 32-bit float

For all images we assume that all values (that is all components of the vectors that are the elements of the images matrix') are in the range [0, 1]. So for example ``ColorImage`` assumes that for each pixel both red, green and blue are in [0, 1].

It may be easier to just look at the imagine instead of having to imagine them so let's work on getting one to show up on our screen

## 2. Image I/O
### 2.1 Loading Images

To load an image from the disk we use:

```cpp
#include <image/image_io.hpp>
using namespace crisp;

// in main.cpp
std::string path = /*...*/ + "/crisp/.test/opal_color.png";

ColorImage color = load_color_image(path);
GrayScaleImage grayscale = load_grayscale_image(path);
BinaryImage binary = load_binary_image(path);
```
![](./color_opal.png) ![](./grayscale_opal.png) ![](./binary_opal.png)

Loading functions are provided for the three main image types and implicitly converting a file into any of the these is no problem in ``crisp``, for example loading a color image as grayscale or vice-versa. ``crisp`` supports the following file extensions:``.bmp``,``.png``,``.tga``,``.jpg``,``.gif``,``.psd``,``.hdr`` and ``.pic``.

When loading non-binary images as binary the image is automatically thresholded with a threshold of ``0.5f``. If this threshold doesn't work for a specific application we can simply threshold it manually using any of the various thresholding methods provided in [``crisp::Segmentation``](../segmentation/segmentation.md).

Note that this conversion can also be done after loading. To convert a binary image into a visually identical color or grayscale image we use:

```cpp
#include <images.hpp>
using namespace crips;

auto binary = load_binary_image(/*...*/);
GrayScaleImage = binary.conver_to_grayscale()
ColorImage as_color = binary.convert_to_color();
```

Similarly we can also convert a grayscale image to a visually identical color image using ``GrayScaleImage::conver_to_color()``.  To convert from color to grayscale we simple take the mean of all elements and to convert from either color or grayscale to binary we use the aforementioned thresholding operations provided.

### 2.2 Saving Images

To save an image we use [``save_to_disk``](/include/image/binary_image.hpp):

```cpp
#include <image/image_io.hpp>

std::string path = /*...*/
save_to_disk(color, path + "/my_color.png");
save_to_disk(grayscale, path + "/my_grayscale.png");
save_to_disk(binary, path + "/my_binary.png");
``` 

For both loading and saving the following underlying image types are supported without further processing necessary, for all image implementations not mentioned here you will need to convert them into one of the following:

+ ``crisp::BinaryImage``
+ ``crisp::GrayScaleImage``
+ ``crisp::ColorImage``
+ ``crisp::Image<T, 1>`` for any arithmetic ``T``
+ ``crisp::Image<T, 3>`` for any arithmetic ``T``

For the last two cases crisp will convert them to grayscale and color respectively.

## 3. Working with Images
### 3.1 Rendering Images
To render an image we need to bind it to a ``crisp::Sprite`` via ``Sprite::create_from`` and then display it by binding it to a window during the familiar render loop:

```cpp
#include <images.hpp>
#include <system.hpp>
using namespace crisp;

int main() 
{
    // load the image
    auto path = /*...*/ + "/crisp/.test/opal_color.png";
    auto image = load_grayscale_image(path);
    
    // bind it to a sprite
    auto sprite = Sprite();
    sprite.create_from(image);
    sprite.set_scale(2); // scales only the sprite, not the image
    
    // create a render window
    auto window = RenderWindow(sprite.get_size().x(), sprite.get_size().y());
    
    // render loop
    while (window.is_open())
    {
        auto time = window.update();

        window.clear();
        window.draw(sprite);    // draw the image
        window.display();
    }
}
``` 
A more detailed explanation of what is happening here can be found in the [system tutorial](../system/system.md), for now just know that all we're doing is creating a window that is the same size as the sprite and rendering the image into it so we can view it easily without having to export it to the disk first.

![](./full_window_view.png)

Note that to render images we need to move them from the ram to the graphics card, operations that scale or move the image may result in artifacting that is only visible in the render window. Always remember: modifying the sprite does not modify the image in any way.


## 3.2 Accessing Individual pixels

Recall that our images are matrices of vectors of a specific value type. We will stick to grayscale images for now so our matrices have 1 dimension in which we have m*n  32-bit floats, where m is the x-dimension of the image, n the y-dimensions. Images provide the following access operators:

```cpp
// for crisp::Image<float, 1>
float  at(size_t x, size_t y) const;
float& at(size_t x, size_t y);

float  operator(int x, int y) const;
float& operator(int x, int y);
```

Image coordinates are in the right-hand coordinate system where the positive x-axis extends to the right and the positive y-axis extends downward. This means the top-most left-most pixel in an image is at index (0, 0), the one right to it is (1, 0), the one directly below (0, 0) is (1, 0), etc.. The top-most right-most pixel is (m, 0), bottom-most left-most is (0, n) and the pixel in the bottom right corner of the image is at position (m, n). 

Using the above mentioned operators we can access any of the pixels. When using ``at`` bounds checking occurrs and an exception will be thrown if a pixel coordinate (a, b) is supplied  that does not fullfill ``0 <= a < m and 0 <= b < n``. If we want to modify the image we should use the non-const vesion of either ``at`` or ``operator()``, in all other cases the non-const versions should be preferred.

``operator()(int, int)`` does not check bounds, instead if the coordinates are out of bounds it accesses what is called [*Padding*](../../include/image/padding_type.hpp). You can think of padding like the frame of an image that extends outwards into infinity in all directions. The values on that frame depend on the ``crisp::PaddingType`` specified for the image:

+ ``ZERO`` Simply makes it so all calls to out of bounds areas will return 0 (or the equivalent value type such as rgb(0, 0, 0) for a color image)<br>
![](./zero_padding.png)

+ ``ONE`` is similar to ``ZERO`` but returns 1 (or the appropriate value type equivalent, rgb(1, 1, 1) for example) instead<br>
![](./one_padding.png)
+ ``REPEAT`` "tiles" the image such that if you leave off the right side you "overflow" and reenter the image from the left and vice-versa in all directions<br>
![](./repeat_padding.png)
+ ``MIRROR`` mirrors the image along it's boundaries<br>
![](./mirror_padding.png)
+ ``STRETCH``: Extends the values on the outer most row/column of the sides infinitely (the "SAMPLE" text has been moved slightly towards the right side to illustrate this effect only in this example)<br>
![](./stretch_padding.png)
  
By default, any image regardless of type will have it's padding set to ``STRETCH``. To change it, simply call:

```cpp
#include <image/padding_type.hpp>
image.set_padding_type(PaddingType::MIRROR);
```

The padding-type can have significant effects on your processing pipelines. It will often modify the behavior of filters and other transforms around the edges of the image so be sure to keep in mind the current padding type and evaluate how appropriate it is for your application regularly. 

Now that we know how to access pixels individually we can use the discussed operators like so:

```cpp
#include <images.hpp>
using namespace crisp;

int main() 
{
    // load the image
    auto image = load_grayscale_image(/*...*/ + "/crisp/.test/opal_color.png");
    
    for (size_t i = 0; i < image.get_size().x() and i < image.get_size().y(); ++i)
        image(i, i) = 0.f;
    
    // render cycle
    /* ... */
}
```

Can you guess what this short loop will draw?

![](./drawn_line.png)

As expected we set the horizontal elements along the trace of the matrix that is the image to 0, creating a 1-pixel thick black line from the top left corner to the bottom right corner of the image. 

## 3.3 Creating Images

Now that we have a way to fill images ourself we might want to start with an empty canvas. We create an image using:

```cpp
#include <images.hpp>

// in main.cpp
auto image = GrayScaleImage();
image.create(500, 230);

// or equivalently:
auto image = GrayScaleImage(500, 230);
```

This creates an image of size 500x230. If we want the image to have an initial value we can specify a third optional argument:

```cpp
// create image and set all pixels to 0.5
auto image = GrayScaleImage();
image.create(500, 230, 0.5f);
``` 

![](gray_only.png)

## 3.4 Image Iterators

To iterate through an entire image we could do:

```cpp
for (size_t y = 0; y < image.get_size().y(); ++y
for (size_t x = 0; x < image.get_size().x(); ++x)
{        
    auto& px = image(x, y)
    // do something here
}
```
While this is sometimes necessary image in crisp also provides a range operator similar to ``std::vector``s. When using this range expression the pixels are iterated left-to-right, top-to-bottom (sometimes also called row-major order). The following code is exactly equivalent to the code just above:

```cpp
for (auto& px : image)
    // do something here
``` 

Remember that px is only the value of the pixel so in the ranged for loop we have no way to access it's coordinates but sometimes this isn't necessary. To reinforce in what order pixels are iterated through, consider the following:

```cpp
#include <images.hpp>
#include <color.hpp>

// create and image
auto image = ColorImage(300, 50)

// fill the image with colors increasing in hue from 0 to 1
float step = 1.f / float(image.get_size().x() * image.get_size().y());

size_t i = 0;
for (auto& px : image)
{
    // current color in HSV
    auto color = HSV{float(i) * step, 1, 1};
    
    // convert to rgb because ColorImage uses RGB
    px = color.to_rgb();
    
    // proceed to the next hue level
    i++;
}
```
Here we assign each colored pixel an HSV value with the hue increasing as we visit more pixels. What will the resulting image look like? Recall that color elements are float in [0, 1] and that the number of pixels in an image of size m, n is m*n. We get:

![](hue_y_then_x.png)

Because the iterator starts at the top left, then goes left until it meets the end of the line, then jumps to the next y index and resets x back to 0 the resulting pattern is a vertical rainbow. Using the regular for-loop with pixel coordinates we get:

```cpp
size_t i = 0;
for (size_t y = 0; y < image.get_size().y(); ++x)
for (size_t x = 0; x < image.get_size().x(); ++y)
{
    auto& px = image(x, y);
    auto color = HSV{float(i), 1, 1};
    px = color.to_rgb();
    i++;
}
```
![](hue_y_then_x.png)

Illustrating ``crisp::Image::Iterator`` is indeed iterating in row-major order.

So far we've been iterating top-to-bottom, left-to-right. What would the image look like if we instead iterator left-to-right, top-to-bottom?

```cpp
#include <images.hpp>
#include <color.hpp>

auto image = ColorImage(300, 50)

float step = 1.f / float(image.get_size().x() * image.get_size().y());

size_t i = 0;

for (size_t x = 0; x < image.get_size().x(); ++x)
for (size_t y = 0; y < image.get_size().y(); ++y)
{
    auto color = HSV{float(i), 1, 1};
    image(x, y) = color.to_rgb();
    i++;
}
```
![](hue_x_then_y.png)

As expected the rainbow gradient was rotated 90° and is now left-to-right instead of top-to-bottom.

## 3.5 Image Arithmetics
Images regardless of type support following arithmetic operators:
```cpp
Image<InnerValue_t, N> operator+(const Image<InnerValue_t, N>&) const;
Image<InnerValue_t, N> operator-(const Image<InnerValue_t, N>&) const;
Image<InnerValue_t, N> operator*(const Image<InnerValue_t, N>&) const;
Image<InnerValue_t, N> operator/(const Image<InnerValue_t, N>&) const;

Image<InnerValue_t, N>& operator+=(const Image<InnerValue_t, N>&);
Image<InnerValue_t, N>& operator-=(const Image<InnerValue_t, N>&);
Image<InnerValue_t, N>& operator*=(const Image<InnerValue_t, N>&);
Image<InnerValue_t, N>& operator/=(const Image<InnerValue_t, N>&);
```
These are all element-wise for both images, that is each element of image A is added/subtracted/multiplied/divided by each element in image B. This is **not** matrix-matrix multiplication.

BinaryImages furthermore provide the following operators:

```cpp
BinaryImage operator!() const;
BinaryImage operator&(const BinaryImage&) const;
BinaryImage operator|(const BinaryImage&) const;
BinaryImage operator^(const BinaryImage&) const;

BinaryImage& operator&=(const BinaryImage&);
BinaryImage& operator|=(const BinaryImage&);
BinaryImage& operator^=(const BinaryImage&);
```
All of these are again element-wise, bitwise operators so if we for example want to change every white pixel to black and every black pixel to white in a binary image we can simply ``image = not image`` because ``operator!`` is defined.

### 3.6 A Note on Artifacting

Recall that our images are matrices of a type that is assumed to be in [0, 1] however there is no mechanism in place to actually enforce this. If we don't normalize an image back into that interval before rendering it we will see artifacting that may throw off newer user. Consider this example:

```cpp
auto image = load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");

float factor = M_PI;
for (auto& pixel : image)
{
    pixel *= factor;
    factor *= 1.000001;
}
```

Here we're taking each pixel and multiplying it by PI (3.14159..) while also slowly increasing that factor the more pixels we visit. This maps our color values from [0, 1] to [0, PI+). This is what the image looks like now:

![](./opal_pi_artifact.png)

The image almost unrecognizable even though all we did was multiply it by a deterministic factor. If we see something like this happen our values are most likely outside [0, 1]. You can simply call ``crisp::normalize`` as provided in ``whole_image_processing.hpp`` to map the values back into [0, 1]:

```cpp
#include <whole_image_processing.hpp>

// in main
normalize(image)
```

![](./opal_pi_artifact_clean.png)

Doing so reveals that the actual effect of the loop on the image is adding a simple gradient.

## 4. Multi Dimensional Images

While the dimensionality of images i relevant to performance, in terms of notation very little changes. Indeed almost all functions, filters and algorithms in ``crisp`` work on all images, regardless of dimensionality or value-type (though both may have a very significant impact on performance). 

``crisp::Image`` provides a member ``crisp::Image::Value_t`` that exposes the images pixels type. The type it deduces to depends on the template arguments as such:

```
InnerValue_t        N       Value_t
-------------------------------------------------------
char                1       char
uint64_t            1       uint64_t
BinaryImage         1       bool
GrayScaleImage      1       float

bool                3       Vector<bool, 3>
float               3       Vector<float, 3>
ColorImage          3       crisp::RGB

float               4       Vector<float, 4>
Vector<float, 5>    7       Vector<Vector<float, 5>, 7>
``` 

For N = 1 the value type internally is a 1-dimensional vector, however (as mentioned in the [vector tutorial](../vector/vector.md)) 1-dimensional vectors decay into their value type. 
``ColorImage`` specifically has it's own template specialization to make working with colors easy without worrying about conversions. The only difference between ``ColorImage`` and ``Image<float, 3>`` is a slight performance overhead that comes from the need to cast ``crisp::RGB`` to ``crisp::Vector<float, 3>`` everytime we assign a color to a pixel value. ``ColorImage`` does this for automatically making it a more convenient option, however if want a color image that interprets it's values in a different color representation we are forced to use ``Image<float, 3>`` and do the conversion ourself.

## 4.1 Accessing Planes directly

When working with n-dimensional images we sometimes don't want our function to act on all elements at the same time. Due to vector arithmetics in ``crisp`` being elementwise in all dimensions this is not directly possible, instead ``crisp`` provides the following function available to all images:

```cpp
template<typename InnerValue_t, size_t N>
template<size_t PlaneIndex>
Image<InnerValue_t, 1> Image<InnerValue_t, N>::get_nths_plane() const;
```

This function takes an index and returns the nths plane as a 1-dimensional image, for an easy example let's consider ``Crisp::ColorImage``. We recall that it's value type ``Value_t`` is ``crisp::RGB`` which has 3 dimensions: red, green and blue. Therefore if we want to isolate the blue-plane we only need to call:

```cpp
auto image = load_color_image(/*...*/
auto blue_plane = image.get_nths_plane<2>();
``` 
because in rgb representation the first component (index 0) is red, the second (index 1) is green and the third (index 2) is blue. 

After we modified the isolated plane we can reinsert it into the image by using:

```cpp
image.set_nths_plane<2>(blue_plane);
```

## 5. A Final Example

To summarize all functions available for images let's consider the following familiar but corrupted image of a bird:

![](./opal_corrupted.png)

We observe very noticable change, the overall image has a green tint and in numerous spots we see isolated magenta drops in intensity. Since both of these artifacts are colored, we investigate each color plane individually:

```cpp
#include <images.hpp>

// in main.cpp
auto image = load_color_image(/*...*/ + "/crisp/docs/image/opal_blue_corrupted.png");

// investigate planes
auto red_plane = (image.get_nths_plane<0>());
save_to_disk(red_plane, /*...*/);

auto green_plane = (image.get_nths_plane<1>());
save_to_disk(green_plane, /*...*/);

auto blue_plane = (image.get_nths_plane<2>());
save_to_disk(blue_plane, /*...*/);
```

![](opal_corrupted_0.png)
![](opal_corrupted_1.png)
![](opal_corrupted_2.png)

Both the red and blue plane look fine but green shows clear problems. Even in parts where the image isn't corrupted by noise we observe that it is significantly lighter in terms of intensity. If the green component of color is lowered we would expect the final color to go closer to it's complement: magenta. This is indeed the case so we can ascertain that the magenta dots in the original assembled image are what's called "drop-out-noise", small spots where at that point the value of only green goes way down.

Both of these issues, the overall higher values and the drop-out-noise, can be fixed with a single filter operation. If you're not sure what this means, please reference the [tutorial on filters](TODO):

```cpp
// median filter the green plane
auto median_filter = SpatialFilter();
median_filter.set_kernel(median_filter.box(3, 1));
median_filter.set_evaluation_function(SpatialFilter::MEDIAN);
median_filter.apply_to(green_plane);

// and normalize the results to remove artifacting
normalize(green_plane);
```

![](opal_restored_1.png)

This is what our green plane now looks like. Much better; not perfect, but better. Now we can reassemble the entire image again:

```cpp
image.set_nths_plane<1>(green_plane);
```

![](opal_restored_2.png)
![](color_opal.png)
On the left (top depending on your devices screen) we have the restored picture, on the right (bottom) the restored one. The restored picture looks much more like the original, but we still note a slight green tint. We also notice small artifacting around the boundaries of the letters but overall the major issues have been fixed by a simple filter operation on only one of the 3 planes.

To clean up the tint we can simply do:

```cpp
for (auto& px : green_plane)
    px -= 0.035f
```

before reassembling:

![](opal_restored_3.png)
![](color_opal.png)

And we're done.

# Post Scriptum

You may ask how we got that arbitrary seeming value of 0.035f. It was determined experimentally by writing and interactive program where pressing up adds 0.005 to the value and pressing down substracts it and then simply feeling around which one was best. This is the true advantage of crisp, thanks to the game-like interactivity you can do it truly interactively, not just by changing a value and recompiling again. The full script for our entire example can be found here:

---
[<< Back to Index](../index.md)