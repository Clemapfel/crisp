# System Module

The system module handles interfacing with the OS and disk. 

## Loading an Image

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
  
## Saving and Image

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

Only ``crisp::BinaryImage``, ``crisp::GrayScaleImage`` and ``crisp::ColorImage`` can be saved. If you wish to save a custom or high-dimensional image, please convert it to rgb first.