[[<< Back to Index]](../index.md)

---

# Hardware Acceleration and GPU-side execution

Textures, Shaders, Graphics Card Interface, Hardware Accelerated Versions of Spatial Filter, Spectral Filters, Morphological Operations

```cpp
#include <gpu_side/textures.hpp>
#include <gpu_side/texture_workspace.hpp>
#include <gpu_Side/state.hpp>
#include <gpu_side/is_gpu_side.hpp>
#include <gpu_side/native_handle.hpp>

// all of the above collected in:
#include <gpu_side.hpp>

// separate:
#include <benchmark.hpp>
```

## Table of Contents

## 1. Motivation & Benchmarks

When reproducing some of the examples in the past chapters, you may have noticed that it's runtime can vary and that some algorithms are quite slow. This is not necessarily due to the implementation, rather, some operations are just inherently very costly. Consider an image of size 1920x1080, this image has 2 073 600 pixels. Each of them has to be allocated, move from the disk to the ram, transformed into a color, etc.. If we now want to operate on the image, each operation will have to be executed 2073600 times. 

To investigate the nature of this behavior, `crisp` offers a very compact benchmarking object:

```cpp
template<typename Function_t>
struct Benchmark
{
    Benchmark(Function_t&& lambda);
    
    template<typename... Args_t>
    float execute(size_t number_of_cycles, Args_t...);
}
```

When creating the object, we hand it a lambda which is saved in the benchmark object. When we call `execute`, we forward the arguments to the lambda and lambda is then itself executed. The benchmark object calculates the time it takes for the lambda to return and reports the time in microseconds.

Using our familiar image of a bird:<br>

![](.resources/color_opal.png)<br>

We want to measure the time it takes to create a deep-copy of this image. The image is `483*483 = 233289` pixels in size and each pixel has three, 32-bit floats (one of each RGB color plane).

```cpp
#include <benchmark.hpp>

auto image = load_color_image(get_resource_path() * "/docs/hardware_acceleration/.resources/color_opal.png");

auto copy_benchmark = Benchmark([&](){
    volatile auto deep_copy = image;
});

std::cout << copy_benchmark.execute(1000) << std::endl;
```

Here we're creating a lambda that allocates a copy of the image and the deep-copies each pixel value into the new image. The `volatile` specified here is used to prevent the compiler from optimizing out the unused variable. 
We then execute the benchmark 1000 times and print the average time per cycle to the console. On this machine, the benchmark reports a time of `86404.3` microseconds on average which is about 0.09s. This doesn't sound that bad but is relativey slow considering the relatively low size of the image. 

To address this runtime issue, `crisp` offers what is basically a high-performance mode. Using this mode, all operations take place on the graphics card which is optimized for image operations on a hardware level. The gpu-side equivalent of an image is called a *texture*. We will learn more about how it works exactly soon but for now we just want to see how much faster it is:

```cpp
#include <benchmark.hpp>
#include <gpu_side/texture.hpp>

auto image = load_color_image(get_resource_path() * "/docs/hardware_acceleration/.resources/color_opal.png");

// create a texture from the image
auto texture = Texture<float, 3>(image);

auto copy_benchmark = Benchmark([&](){
    volatile auto texture_deep_copy = texture;
});

std::cout << copy_benchmark.execute(1000) << std::endl;
```
Here we're first loading the image, then creating a texture from the image. Inside the lambda we then deep copy this texture once per cycle. The benchmark reports an average time (in microseconds) of:

```
2.675
```

That's... a lot faster. This result is not erronous, each call did indeed allocated a new image of the same size and copy all data from the original into the new image. Comparing 2.675 microseconds to 86404.3 microseconds we get a sense of just how much faster gpu-side computation is. Now, dealing with many 60fps streams at the same time seems a lot more doable.

## 2. Introduction to GPU-Side Computing

To understand why textures are so much faster and how to properly utilize them, we need to achieve a certain amount of base-knowledge about how graphics cards work. Similary to the [deep learning tutorial](../feature_classification/feature_classification_and_deep_learning.md), this article will only offer the minimum amount of knowledge needed to use and understand `crisp`s functions. A proper introduction to graphics-card interaction is available on the authors [blog](www.clemens-cords.com/https://clemens-cords.com/post/).

All computers have a CPU (central processing unit), this is the hardware unit that does the work of moving and combining memory using *operations*. Each CPU architecture has a certain set of operations and hardware is structured in a way that makes these operations very fast. Because we need objects to use for the operations, we need a memory. The CPU has a small amount of memory called a *cache*, this memory is extremely fast to access. Slightly slower but much, much larger memory is called the RAM, this where our program and most of the data we handle live during runtime. When loading an image, we load the data from the harddrive into the RAM. Then, when we operate on the image the CPU loads the data from the RAM into the cache, transforms it and then puts it back into the RAM so we can use it for other things.
The graphics card is similarly structured, it has a processing unit now called the GPU (graphics processing unit) which again has a set of instructions. Unlike the CPU, these instructions are more limited but specialized. Similarly to RAM the graphics card has it's own memory, usually completely separate fromt he computers RAM. To distinguish the two, we will refer to the gpu-side RAM as [VRAM](https://en.wikipedia.org/wiki/Video_RAM_(dual-ported_DRAM)#Video_DRAM_(VRAM)), note however that VRAM is only one type of gpu-side memory, other types include (WRAM, MDRAM, SGRAM, etc.). 

It is important to realize this separation, the graphics card has no way of accessing both the disk as well as the RAM and similarly the CPU has no way of accessing the GPU or the VRAM. To allow for gpu-side computation, we first need to move objects into the VRAM. This can be relatively costly but the GPU makes up for this by being extremely good at matrix-operations. Recall that images can be conceptualized as a matrix of pixel values. On the CPU each of these values would have to processed one-by-one, while the GPU can often move and compute comparatively large matrices with a single, hardware-level operation.

## 2.1 `crisp::State`

Interacting with the graphics card through OpenGL can be quite cumbersome and frankly hard, users need to know a lot of things to do basic operations like assignment. To make this easier, `crisp` offers an interface class called `crisp::State` (in `gpu_side/state.hpp`). This class has only static members and functions and is a representation of the current state of the graphics card context. 

## 2.2 Handles

When dealing with gpu-side objects, we do not have access to the memory and we thus do not have a reference or pointer to the object, instead the objects are identified cpu-side using a *handle*:

```cpp
// in gpu_side/native_handle.hpp

using GLNativeHandle = unsigned int; // 1, 2, ...
using ProxyID = int; // -1, -2, ...

#define NONE 0
```

There are two types of handles in crisp, `GLNativeHandle` are handles generated by OpenGL, the library used to interface with the graphics card. `GLNativeHandle` start at 1 and continue upwards, they are always positive. To not accidentally confuse the two, `crisp` furthermore defines a `ProxyID` which is the handle used by objects generated with `crisp` alone. `ProxyID`s start at -1 and continue downwards. Because of this, the two types of handles cannot be mixed and during debugging, it's easy to distinguish the two by simple looking at the value.
For better legibility, `crisp` offers a c-constant `NONE` which is the handle referring to the gpu-side NULL object (similar to C++s `nullptr`). If we want a variable to be bound to nothing, we set the handle of it's value `NONE`. 

In summary, when we interact with a gpu-side object in C++, we are only interacting with it's *handle*. If we want to change the value of the object, we need use to a *shader* if the object is a texture or we need to manually send the value over to the graphics card if the object is a trivial type such as a single number or a vector.

## 2.3 Value Types

For an object of type `foo` and value `value:
+ `auto foo_handle = State::register_foo(value)` allocates the objects gpu-side, assigns it the value `value` and returns it's handle which is saved in the variable `foo_handle`.
+ `State::free_foo(foo_handle)` deallocates the object with the handle `foo_handle`

So for example if we want to create a gpu-side variable that is a simple bool:

```cpp
{
auto cpu_side_bool = true;
auto gpu_side_bool_handle = State::register_bool(cpu_side_bool);
} // here, C++ deallocates cpu_side_bool, however the memory on the graphic card still persists

// manually deallocate it to actually free the memory
State::free_bool(gpu_side_bool_handle);
```

It's important to remember that both the value and ownership of gpu-side variables are completely separate from it's cpu-side handle. If we somehow loose the handle, the memory on the graphic card persists which can lead to memory piling up so it is important to be mindful of that.

This section talks about what types of objects there are, due to the specific set of gpu operations, the number of types is very limited compared to C++s. The following list is exhaustive:

### Scalars: `int`, `uint`, `bool`, `float`
There are only 32-bit integers and 32-bit floats. For convenience, `crisp` offers an interface that also supports `bools`, even though gpu-side they not actual 1-bit objects.

We can allocate and free numbers using:
```cpp
State::register_int(int);
State::register_uint(size_t);
State::register_bool(bool);
State::register_float(float);

State::free_int(int);
State::free_uint(size_t);
State::free_bool(bool);
State::free_float(float);
```

### Vectors: `vec2`, `vec3`, `vec4`
All vectors are vectors of 32-bit floats. There are only 3 size of vectors: 2, 3 and 4

We allocate and free vectors using:

```cpp
State::register_vec2(crisp::Vector<T, 2>);
State::register_vec3(crisp::Vector<T, 3>);
State::register_vec4(crisp::Vector<T, 4>);

State::free_vec2(crisp::Vector<T, 2>);
State::free_vec3(crisp::Vector<T, 3>);
State::free_vec4(crisp::Vector<T, 4>);
```
Note that while `State`s functions take vectors of any value type, the are cast to 32-bit float before being send to the graphics card.

### Matrices: `Mat2`, `Mat3`, `Mat4`, `Mat2x3`, `Mat3x2`, `Mat2x4`, `Mat4x2`, etc.

Matrixes can only have a size of `m*n` where m, n in {2, 3, 4}. Matrices of size 2x2 are called `mat2`, 3x3 are called `mat3` and 4x4 `mat4`. For non-square matrices, the types name is `matnxm` where n is the number of columns and m is the number of rows. For example, a 2-column, 4-row matrix has the type `Mat2x4` while a 4-column, 2-row matrix is of type `Mat4x2`. If we want a matrix with only one row or one column, we will have to use a vector instead.

We allocate and free matrices using:

```cpp
State::register_matrix(Matrix<T>);
State::free_matrix(Matrix<T>);
```
We do not need to specify the size of the matrix, `crisp` automatically detects it and allocates the corresponding gpu-side typed matrix. If the matrix handed to the state is not size `N`, `M` where `N, M in {2, 3, 4}` an exception will be raised.

### Arrays: `float[]`, `vec2[]`, `mat4x2[]`, etc.

All value types mentioned so far can also be in arrays. This means that we cannot have arrays of array, only arrays of `float`, `int`, `bool`, all 4 vector types and all 16 matrix types.

We register an array using:

```cpp
```

## 2.4 Shaders

Along the trivial types mentioned above, `crisp::State` offers two more variables to bind: *textures* (which we will learn in the next section), and *shaders*. A shader is a program, it's source code is stored somewhere on the disk and when it is registered, source code is send to OpenGL and compiled into essentially a binary. We can do so like this:

```cpp
auto shader_source = State::register_shader("/path/to/shader.glsl");
```

In `crisp`, all user-defined shaders are fragment shaders. This is because 2d image processing does not need any 3D functionality like depth, vertices, stencils, etc. so it is all handled implicitely by `crisp` in the background. When we refer to shaders, henceforth we always mean fragment shaders. Each shader has the following form:

```glsl
#version 330 core

// shader input/output
in vec2 _tex_coord;
out vec4 _out;

// uniforms
uniform sampler2D _texture;

// main
void main()
{
    vec4 fragment_color = texture2D(_texture, _tex_coord);
    // do something
    _out = fragment_color;
}

```

We see that each fragment shader gets handed exactly one argument from the vertex shader called `_tex_coord`. This is the spatial position of the fragment currently modified using the shader. After the shader has done it's job, `_out` is written to which will make the corresponding fragment appear the color specified in the shader. Each fragment shader furthermore has at least one uniform: `_texture`. Note that this uniform is compulsory and `crisp` assumes that it is always present and always spelled exactly like that. If we want more than one texture uniform we can do so, however one of them will have to be called `_texture`.

After compiling the shader via `register_shader`, we need to package the binary into a full *shader program* that the graphics card can actually use. We do so using:

```cpp
auto shader_source = State::register_shader("/path/to/shader.glsl");
auto shader_program = State::register_program(shader);
State::free_shader(shader_source);
```

After we registered the program, the shader is now no longer needed so it can be safely freed. We now have a functioning shader program, to use it we simply call `State::bind_shader_program(shader_program)` which will make it 




























