[<< Back to Index](../index.md)

---
# System Module

Keyboard/Mouse Input, Loading and Saving Images 

## Table of Contents

# 1. Image I/O

## 1.1 Loading an Image

To load an image from a disk use:

```cpp
#include <system/image_io.hpp>

std::string path = /*...*/ + "crisp/.test/opal_color.png";

auto as_binary = load_binary_image(path);
auto as_grayscale = load_grayscale_image(path);
auto as_color = load_color_image(path);
```

Most major files formats (bmp, png, tga, jpg, gif, psd, hdr, pic) are supported. The following behavior will occur depending on the image type (assuming the image on the disk is a color image):

+ Color
  The image will be interpreted as an 8-bit color image and transform into ``crisp::ColorImage`` (which has 32-bit float rgb pixels). Transparency will be ignored and assumed as 0. 
  
+ GrayScale
  The image will be read and transformed into ``crisp::GrayScaleImage`` by averaging the rgba values as ``(red + green + blue + 0*alpha) / 3``.
  
+ Binary
  The image will thresholded into ``crisp::BinaryImage`` by letting each pixel be ``(red + green + blue + 0*alpha) / 3 > 0.5``, that is if the equivalent grayscale value of the images pixel is bigger than 0.5, it will be treated as ``true`` (white), ``false`` (black) otherwise. If you wish to use a different threshold, load the image as grayscale and threshold it manually using ``crisp::segmentation::manual_threshold``
  
## 1.2 Saving an Image

You can save an image to the disk like this:

```cpp
#include <system/image_io.hpp>

std::string path = /*...*/ + ".png";
auto image = /*...*/
        
if (not save_to_disk(image, path))
{
    // handle disk error
}
```

Only ``crisp::BinaryImage``, ``crisp::GrayScaleImage`` and ``crisp::ColorImage`` can be saved. If you wish to save a custom or high-dimensional image, please convert it to rgb first.``

# 2 Rendering and User Input

# 2.1 Rendering
To render an image or many other crisp entites such as fourier spectra or filter kernels it needs to be bound to a ``crisp::Sprite``. Sprite is a versatile class that generates a texture from whatever you give it and can then be displayed later. You can create a sprite like this:

```cpp
#include <system/sprite.hpp>

auto sprite = Sprite();

ColorImage color_image = /*...*/
sprite.create_from(color_image);

FilterKernel kernel = /*...*/
sprite.create_from(kernel);

FourierSpectrum spectrum = /*...*/
sprite.create_from(spectrum);
``` 

The fact you can render almost everything for inspection means human-interfacing systems or even just debugging becomes so much easier and more intuitive with crisp because you can see all the math.

To render a sprite we need to create a ``crisp::RenderWindow`` and setup a render loop:

```cpp
// main.cpp
#include <system/render_window.hpp>

using namespace crisp;

int main()
{
    // create a sprite and scale it to twice it's size
    auto object = /*...*/
    auto sprite = Sprite();
    sprite.create_from(object);
    sprite.set_scale(2);
    
    // create a render window thats
    // the size of the sprite + a 100 px border
    auto window = RenderWindow(sprite.get_size().x() + 100, sprite.get_size().y() + 100);
    
    // anchor the sprite at the windows center
    sprite.set_position(window.get_resolution() * 0.5);
    
    // render loop
    while (window.is_open())
    {
        auto time = window.update();
        window.clear();
        window.draw(&sprite);
        window.display();
    }
}
```

Sprites can be scaled and moved around the window, it's position is relative to the top-left corner of the window, and the sprites center is it's origin. Note that you can specify coordinates outside of the windows boundaries so be wary of that if you're not sure why your sprite is not showing up, it might be improperly positioned.

# 2.2 Mouse & Keyboard Input

Calling ``crisp::RenderWindow::update`` automatically updates the ``InputComponent`` which handles all user input through mouse and keyboard. This includes minimizing or closing the window, so be sure to call ``RenderWindow::update`` during your renderloop or you have no way to exit your program.

The Input component 

---
[<< Back to Index](../index.md)
