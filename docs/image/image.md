# Images

## Table of Content

1. **Introduction**</br>
2. **Image I/O**</br>
    2.1 Loading Images</br>
    2.2 Saving Images</br>
3. **Working with Images**</br>
    3.1 Rendering Images</br>
    3.2 Padding & Accessing Pixels</br>
    3.3 Creating Images</br>
    3.4 Image Iterators</br>
    3.5 Image-Image arithmetic operators</br>
4. **Multi Dimensional Images**</br>
    4.1 Accessing Planes</br>
5. **A final example**</br>

## 1. Introduction

Images are obviously at the heart of image processing so it's important to understand them thoroughly. In ``crisp``, images are n-dimensional matrices of arbitrary value and arbitrary (but arithmetic) value type:

```cpp
template<typename InnerValue_t, size_t N>
class Image
{
```

We see it takes two template arguments: 
+ ``InnerValue_t`` is the value type of the elements in all planes
+ ``N`` is the number of dimensions

We will henceforth call each dimensional matrix of an image a "plane", for example in a 3-dimensional rgb image we have 3 planes: red, green and blue.

Most of the time we won't be dealing with ``crisp::Image`` directly but rather with one of it's 3 implementations:
+ [``BinaryImage``](/include/image/binary_image.hpp) is a 1-dimension image where each value can either be white (true) or black (false). Because the term "black and white image" is colloqially ill-defined we will call them *binary images*
+ [``GrayScaleImage``](/include/image/grayscale_image.hpp) is a 1-dimensional image where each value is a 32-bit float
+ [``crisp::ColorImage``](/include/image/binary_image.hpp) is a 3-dimensional image where each value is a vector representing color in the RGB format. Each component of the vector (each planes pixels) are a 32-bit float

For all images that use a floating point arithmetic type such as ``float`` or ``double`` we assume the valid values are in [0, 1]. This means that each color value in our color image is, too, in [0, 1]. Remember this as it differs from the often used [0, 255] interval in literature instead.

It may be somewhat hard to imaging what an n-dimensional matrix looks like so let's talk about to actually get an image to show up on your screen. 

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

Loading functions are provided for the three main image types and implicitly converting a file into any of the three is guruanteed to suceed so ``crisp`` has no problem loading a for example a color image as grayscale for example or vice-versa. ``crisp`` supports the following file extensions:``.bmp``,``.png``,``.tga``,``.jpg``,``.gif``,``.psd``,``.hdr`` and ``.pic``.

When loading non-binary images (images with more than just ``rgb(0, 0, 0)`` and ``rgb(1, 1, 1)``) as binary the image is automatically thresholded with a threshold of ``0.5f``. If this threshold doesn't work for a specific application we can simply threshold it manually using any of the various thresholding methods provided in [``crisp::Segmentation``](../segmentation/segmentation.md). 

### 2.2 Saving Images

To store an image* we use [``save_to_disk``](/include/image/binary_image.hpp):

```cpp
#include <image/image_io.hpp>

std::string path = /*...*/
save_to_disk(color, path + "/my_color.png");
save_to_disk(grayscale, path + "/my_grayscale.png");
save_to_disk(binary, path + "/my_binary.png");
``` 

For both loading and saving the following underlying image types are supported natively, for all image implementations not mentioned here you will need to convert them into one of the following:

+ ``crisp::BinaryImage``
+ ``crisp::GrayScaleImage``
+ ``crisp::ColorImage``
+ ``crisp::Image<T, 1>`` for any arithmetic ``T``
+ ``crisp::Image<T, 3>`` for any arithmetic ``T``

For the last 2 cases crisp will convert them to grayscale and color respectively.

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

### 3.1.1 A Note on Artifacting

Recall that our images are matrices of a type that is assumed to be in [0, 1] however there is no mechanism in place to actually enforce this. If we don't normalize an image back into that interval we will see artifacting that may throw off newer user. Consider this example:

```cpp
auto image = load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");

float factor = M_PI;
for (auto& pixel : image)
{
    pixel *= factor;
    factor *= 1.000001;
}
```

Here we're taking each pixel and multiplying it by PI (3.14159..) while also slowly increasing that factor the more pixels we visit. It's not important what exactly this code does for now all you need to know is that it maps our color values from [0, 1] to [0, PI+). This is what the image looks like now:

![](./opal_pi_artifact.png)

The image almost unrecognizable even though all we did was multiply it by a deterministic factor. If you see something like this happen your values are most likely outside [0, 1]. You can simply call ``crisp::normalize`` as provided in ``whole_image_processing.hpp`` to map the values back into [0, 1]:

```
#include <whole_image_processing.hpp>

// in main
normalize(image)
```

![](./opal_pi_artifact_clean.png)

Doing so reveals that the actual effect of the loop on the image is adding a simple gradient.

## 3.2 Accessing Individual pixels

Recall that our images are matrices of a specific value type. We will stick to grayscale images for now so our matrices have 1 dimension in which we have m*n elements of 32-bit floats. m is the x-dimension of the image, n the y-dimensions. Images provide the following access operators:

```cpp
// for crisp::Image<float, 1>
float at(size_t x, size_t y) const;
float& at(size_t x, size_t y);

float  operator(int x, int y) const;
float& operator(int x, int y);
```

Image coordinates are in the right-hand coordinate system where the positive x-axis extends to the right and the positive y-axis extends downward. This means the top-most left-most pixel in an image is at index (0, 0), the one right to it is (1, 0), the one directly below (0, 0) is (1, 0), etc. The top-most right-most pixel is (m, 0), bottom-most left-most is (0, n) and the pixel in the bottom right corner of the image is at position (m, n). 

Using the above mentioned operators we can access any of the pixels. When using ``at``, it will bounds-check the index and throw an exception if a pixel coordinate (a, b) is supplied  that does **not** fullfill ``0 <= a < m and 0 <= b < n``. Using the const version of at simply returns the pixels value, using the non-const version returns a reference to the pixels value that can be modified to modify the corresponding pixel in the image. This is true for both access operators.

``operator()(int, int)`` does not check for bounds, instead, if the coordinates are out of bounds it accesses what is called [*Padding*](../../include/image/padding_type.hpp). You can think of padding like the frame of an image that extends outwards into infinity in all directions. The values on that frame depend on the ``crisp::PaddingType`` specified for the image:

+ ``ZERO``: Simply makes it so all calls to out of bounds areas will return 0 (or the equivalent value type such as rgb(0, 0, 0) for a color image)<br>
![](./zero_padding.png)

+ ``ONE``: Similar to ``ZERO`` but returns 1 (or the appropriate value type equivalent, rgb(1, 1, 1) for example) instead<br>
![](./one_padding.png)
+ ``REPEAT``: "tiles" the image such that if you leave off the right side you "overflow" and reenter the image from the left and vice-versa in all directions<br>
![](./repeat_padding.png)
+ ``MIRROR``: Mirrors the image along the connected boundary<br>
![](./mirror_padding.png)
+ ``STRETCH``: Extends the values on the outer most row/column of the sides infinitely<br>
![](./stretch_padding.png)
  
By default, any image regardless of type will have it's padding set to ``STRETCH``. To change it, simply call:

```cpp
#include <image/padding_type.hpp>
image.set_padding_type(PaddingType::MIRROR);
```

The padding-type can have significant effects on your processing pipelines. It will often modify the behavior of filters and other transforms around the edges of the image so be sure to keep in mind the current padding type and evaluate how appropriate it is for your application regularly. 

Now that we know how to access pixels individually we can use them to modify them like so:

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

As expected we set the horizontal elements along the trace of the matrix to 0, creating a 1-pixel thick black line from the top left corner to the bottom right corner of the image. 

## 3.3 Creating Images

Now that we have a way to fill them we might as well start with an empty canvas. We create an image like so:

```cpp
#include <images.hpp>

// in main.cpp
auto image = GrayScaleImage();
image.create(500, 500);

// or equivalently:
auto image = GrayScaleImage(500, 500);
```

This creates an image of size 500, 500. If you want the image to have an initial value you can specify a third optional element:

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
While this is sometimes necessary image also provides a range operator similar to ``std::vector``s. When using this range expression the pixels are iterated on left to right, top to bottom. The following behavior is exaclty equivalent to the code just above:

```
for (auto& px : image)
    // do something here
``` 

Remember that px is the value of the pixel so in the ranged-for loop we have no way to access it's coordinates but sometimes that isn't necessary. To reinforce in what order pixels are iterated through, consider the following:

```
#include <images.hpp>
#include <color.hpp>

auto image = ColorImage(300, 50)

float step = 1.f / float(image.get_size().x() * image.get_size().y());

size_t i = 0;
for (auto& px : image)
{
    auto color = HSV{float(i), 1, 1};
    px = color.to_rgb();
    i++;
}
```
Here we assign each now colored pixel an HSV value where the hue is increasing as we visit more pixels. What will the resulting image look like? Recall that color elements are float in [0, 1] and that the number of pixels in an image of size m, n is m*n

![](hue_y_then_x.png)

Because the iterator starts at the top left, then goes left until it meets the end of the line and so jumps to the next y index and resets x back to 0 the resulting pattern is a vertical rainbow. Usin the above for-in-loop is equivalent to:

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

Note the right-to-left, top-to-bottom order of the elements.
If we were to instead iterate top-to-bottom, right-to-left, we would get:

```
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

Keep this order of elements in mind when writing loops and when using the range expression, by default iterator iterates *top-to-bottom, left-to-right*

## 3.5 Image Arithmetics
Images support at least the following arithmetic operators:
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
These are all element-wise for both images, this is *not* matrix multiplication (outer product), these simply iterate through each element at x_i, y_j and then adding, subtracting, etc. the element at x_i, y_j in the other image.

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
These are bitwise operators so to for example invert each pixel in a binary image so black becomes white and white becomes black we can simply call ``image = not image`` because ``operator!`` is defined.

## 4. Multi Dimensional Images

We've already used a color image in the last example as as you saw very little changed in terms of notation and what functions we can use. Indeed almost all functions, filters and algorithms in ``crisp`` work on all images, regardless of dimensionality or value-type (though both may have a very significant impact on performance). 
Realizing what the value-type ``Value_t`` of an image is is important and it may be somewhat hard to realize just from the code because the return type depends on the template arguments. Consider this table:

```
InnerValue_t        N       Value_t
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

For N = 1 the value type internally is a 1-dimensional vector, however as mentioned in the [vector tutorial](TODO), 1-dimensional vectors decay into their value type so for most applications you can assume that ``image<x, 1>::Value_t == x``. ColorImage has it's own template specialization to make working with colors easy without worrying for conversions. The only difference between ``crisp::ColorImage`` and ``crisp::Image<float, 3>`` is that you will have to ``dynamic_cast`` all the ``Vector<float, 3>`` to ``crisp::RGB`` if you use ``Image<float, 3>``. ``ColorImage`` does this for you making it a more convenient option, however maybe you want a color image that treats it's vectors as hsv instead of rgb, in that case ``Image<float, 3>`` is the better choice.

## 4.1 Access Planes directly

When working with n-dimensional images we sometimes don't want our function to act on all elements at the same time. Due to vector arithmetics in crisp this is not directly possible, instead crisp provides a function available to every image called

``            
template<typename InnerValue_t, size_T N>
tempalte<size_t PlaneIndex>
Image<InnerValue_t, 1> Image<InnerValue_t, N>::get_nths_plane() const;
``

This function takes an index and returns the nths plane as a 1-dimensional image, for an easy example let's consider ``Crisp::ColorImage``. We recall that it's value type ``Value_t`` is ``crisp::RGB`` which has 3 dimensions: red, green and blue. Therefore if we want to isolate the blue-plane we only need to call:

```cpp
auto image = load_color_image(/*...*/
auto blue_plane = image.get_nths_plane<2>();
``` 
because of rgb, the first component (index 0) is red, the second (index 1) is green and the third (index 2) is blue. 

After we modified the isolated plane we can reinsert it into the image by using:

```cpp
// do something with blue plane
image.set_nths_plane<2>(blue_plane);
```

## 5. A final example

As an example of how to use  let's consider the following familiar image of a bird:

![](./opal_blue_corrupted.png)

We observe very noticable corruption, the overall image has a green tint and in numerous spots we see isolated magenta drops in intensity. Since both of these artifacts are colored, we investigate each color plane individually:

```cpp
#include <images.hpp>

// in main.cpp
auto image = load_color_image(/*...*/ + "/crisp/docs/image/opal_blue_corrupted.png");
auto blue_plane = image.get_nths_plane<2>();

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

red and blue look fine but green shows clear problems. We observe that it is slightly lighter in parts that aren't completely corrupted by the noise. If the green component of color is lowered we would expect the final color to go closer to it's complement: magenta. This is indeed the case so we can ascertain that the magenta dots in the original assembled image are what's called "drop-out-noise", small spots where at that point the value of only green goes way down.

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















