[<< Back to Index](../index.md)

---
# System Module

Loading and Saving Images, Rendering any `crisp` Object to a Window, Keyboard/Mouse Input

```cpp
#include <system/input_handler.hpp>
#include <system/image_io.hpp>
#include <system/key_id.hpp>
#include <system/render_window.hpp>
#include <system/sprite.hpp>

// collected in:
#include <system.hpp>
```

## Table of Contents

1. [**Image I/O**](#1-image-io)<br>
    1.1 [Loading an Image](#11-loading-an-image)<br>
    1.2 [Saving an Image](#12-saving-an-image)<br>
2. [**Rendering**](#2-rendering)<br>
    2.1 [Sprite & Render Window](#21-sprite--render-window)<br>
    2.2 [Mouse/Keyboard Input](#22-mouse--keyboard-input)<br>
3. [**An Example**](#3-an-example)<br>

## 1. Image I/O

### 1.1 Loading an Image

We can load an image from disk like so:

```cpp
std::string path = /*...*/ + "crisp/.test/opal_color.png";

auto as_binary = load_binary_image(path);
auto as_grayscale = load_grayscale_image(path);
auto as_color = load_color_image(path);
```

Most major file formats (`.bmp`, `.png`, `.tga`, `.jpg`, `.gif`, `.psd`, `.hdr`, `.pic`) are supported. 

Depending on what type the image is loaded as, the following behavior will occurr:

+ `load_color_image`: image data will be interpreted as 8-bit rgba and then transformed into 32-bit floating point rgb. Transparency will be ignored and assumed as 0.
+ ``load_grayscale_image``: image data will be transformed into a single intensity by averaging the rgba values, ignoring transparency.
  
+ ``load_binary_image``: image data will be transformed into grayscale, then thresholded using a threshold of 0.5

When loading as binary, if we want to apply a different threshold we can load the image as grayscale and threshold it manually using ``Segmentation::manual_threshold`` as detailed in the [segmentation tutorial](../segmentation/segmentation.md/#21-manual-threshold).
  
## 1.2 Saving an Image

We can save an image to the disk using:
```cpp
std::string path = /*...*/;
auto image = load_/*...*/_image(path);
        
if (not save_to_disk(image, path))
    // handle disk error
```

Only ``crisp::BinaryImage``, ``crisp::GrayScaleImage`` and ``crisp::ColorImage`` can be saved like this. If we want to store a different image type we first need to convert it into either of the three.

## 2. Rendering

One of ``crisps`` central design decisions is that everything can be rendered. Where "everything" actually means the following ``crisp`` objects:

+ `crisp::BinaryImage`
+ `crisp::GrayScaleImage`
+ `crisp::ColorImage`
+ `crisp::Image<T, 1>` 
+ `crisp::FourierTransform` 
+ `crisp::FrequencyDomainFilter`
+ `crisp::Kernel`
+ `crisp::StructuringElement`
+ `crisp::Histogram`
+ `Eigen::Matrix<T, /*...*/>`

### 2.1 Sprite & Render Window

We will focus on rendering images for now. To render an image we first need to create a ``crisp::Sprite``, then bind the corresponding image like so:

```cpp
auto sprite = Sprite();

ColorImage color_image = /*...*/
sprite.create_from(color_image);
```

Now we need to open a ``crisp::RenderWindow``. When creating it we supply it's resolution, the fps limit and wether or not it should be openend in fullscreen mode. We then implement a *render-loop* which has the following steps (in order):

1. Update the input component
2. Clear the window
3. Draw our entities
4. Display the window

(we will learn more about `crisp::InputComponent` later)

This loop is continously executed until the user clicks the "x" button of the window.

```cpp
// main.cpp
#include <system/sprite.hpp>
#include <system/render_window.hpp>

using namespace crisp;

int main()
{
    // create a sprite and scale it to twice it's size
    auto object = /*...*/
    auto sprite = Sprite();
    sprite.create_from(object);
    sprite.set_scale(2);
    
    // create a render window that's
    // the size of the sprite + a 100 px border
    auto window = RenderWindow(sprite.get_size().x() + 100, sprite.get_size().y() + 100);
    
    // anchor the sprite at the windows center
    sprite.set_position(window.get_resolution() * 0.5);
    
    // render loop
    while (window.is_open())
    {
        // 1. update input component
        auto time = window.update();
        
        // 2. clear the render state
        window.clear();
        
        // 3. bind the sprite to the render state
        window.draw(&sprite);
        
        // 4. push the render state to the os window
        window.display();
    }
}
```

Sprites can be scaled and moved around the window, it's position is relative to the top-left corner of the window, the sprites center is it's origin. Note that we can specify coordinates outside of the windows boundaries without problems, however we won't be able to see the sprite if it's entire area is outside the window area.

### 2.2 Mouse & Keyboard Input

Calling ``crisp::RenderWindow::update`` automatically updates the ``InputHandler`` which handles all user input, mouse and keyboard. This includes minimizing or closing the window which is why we need to update it to make it so the window closes when the user clicks the "x".

The Input component is a static entity that cannot be initialized. It holds the current and past state of the keyboard. We can query the state of a specific key by specifying it's ``KeyID``, the enum of available keys can be accessed [here](../../include/system/key_id.hpp). 

The state of a key can be either "down" (pressed) or "up" (not pressed) at any point in time. The state updates when ``RenderWindow::update()`` is called and we can access it using:

```cpp
// is key down currently
bool is_key_down(KeyID);

// was key up last cycle and down this cycle
bool was_key_pressed(KeyID);

// was key down last cycle and up this cycle
bool was_key_released(KeyID);

// was the key released or pressed
has_state_changed(KeyID);

// for how long has the key been down
Time get_hold_duration(KeyID);

// current position of the cursor relative to the windows origin
Vector2f cursor_position();
```

Where ``crisp::Time`` is the real time since elapsed from the last ``RenderWindow::update`` to the current function call of ``get_hold_duration``.

## 3. An Example

We can use this interactivity to aid in debugging (so we don't have to recompile anytime we want to change a parameter) or to build an interactive program. For example let's say we want to experimentally change the brightness of a grayscale image. One way to do this could be like so:

```cpp
#include <system.hpp>
#include <image/grayscale_image.hpp>
using namespace crisp;

int main()
{
    // load image as backup
    const auto original = load_grayscale_image(/*...*/ + "/crisp/docs/system/hidden_secret.png");
    
    // image that we will modify
    auto image = original;

    // create sprite
    auto sprite = Sprite();

    auto window = RenderWindow();
    window.create(image.get_size().x(), image.get_size().y());

    // modify the image by multiplying each pixel intensity with factor
    float factor = 1;
    auto update = [&]()
    {
        std::cout << "current factor: " << factor << std::endl;
        
        image = original;
        for (auto& px : image)
            px *= factor;

        // then rebind to sprite to update it
        sprite.create_from(image);
    };

    // render loop
    while (window.is_open())
    {
        // update input handler
        window.update();

        // press up to increase brightness
        if (InputHandler::was_key_pressed(KeyID::UP))
        {
            factor += 0.25;
            update();
        }
        
        // press down to decrease it
        else if (InputHandler::was_key_pressed(KeyID::DOWN))
        {
            factor -= 0.25;
            update();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}
```

Try executing [this little program](./system_example.cpp) by modifying ``crisp``s install path for ``load_grayscale_image`` to then test it out. You can use the up- and down-arrow keys to change the image the images brightness, revealing the actual content.

---
[<< Back to Index](../index.md)
