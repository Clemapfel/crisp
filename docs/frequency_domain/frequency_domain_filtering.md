[<< Back to Index](../index.md)

---

# Frequency Domain

Fourier- and Discrete Cosine Transforms and their Filters

## Table of Content

1. [Introduction](#1-introduction)<br>
2. [Fourier Transform](#2-fourier-transform)<br>
    2.1 [Fourier Transform Performance Mode](#21-fourier-transform-performance-mode)<br>
    2.2 [Creating the Transform](#22-creating-the-transform)<br>
    2.3 [Visualizing the Spectrum](#23-visualizing-the-spectrum)<br>
    2.4 [Accessing Coefficients](#24-accessing-coefficients)<br>
    2.5 [Transforming Back into an Image](#25-transforming-back-into-an-image)<br>
3. []()

## 1. Introduction

While working on images themself in the so called *spatial domain* provides a variety of advantages some techniques are much easier to accomplish in the *frequency domain*. These *spectral* techniques, in ``crisp`` are performed on two transforms of images: 
+ The [Fourier Transform](https://en.wikipedia.org/wiki/Fourier_transform) treats our Images as 2D continous functions, regularly sampled at the pixel coordinates, and translates them into a number of [complex](https://en.wikipedia.org/wiki/Complex_number) coefficients of sinoid functions. Combining the functions weight by their coefficients returns the original image
+ The [Discrete Cosine Transform](https://en.wikipedia.org/wiki/Discrete_cosine_transform) similarly express a 2D continous functions, regularly sample (our images) as coefficients of specifically cosine functions oscillating at different frequencies

Both these transforms have numerous uses in image processing and ``crisp`` saves users all the math and provides and easy interface for both viewing and filtering the transformed images.

## 2. Fourier Transform
### 2.1 Fourier Transform Performance Mode
Computing the fourier transform is taxing and can be quite slow, to alleviate this ``crisp`` offers three different ``FourierTransformModes`` that can be handed to ``crisp::FourierTransform`` as a template argument:

```cpp
enum FourierTransformMode {SPEED, BALANCED, ACCURACY};

template<FourierTransformMode Mode>
class FourierTransform
{
```

+ ``SPEED`` is the most optimal version offered by crisp, it uses 32-bit float coefficients and employs a suboptimal heuristic. While artifacting due to low precision is common, this is the only mode that can realistically be run in real time on most at-home machines.
+ ``BALANCE`` is about 2 times slower than ``SPEED`` and uses 64-bit double coefficients. It's heuristic is also suboptimal, however is much closer to optimum than ``SPEED``s. For applications where easier a high performance CPU is available or for smaller images this mode is recommended
+ ``ACCURACY`` is about 10 times slower than ``SPEED`` and uses 64-bit double coefficients, like ``BALANCE``. The significant slow down comes from first computing the optimal heuristic, this assures that results are as accurate as possible, however the difference between it's results and ``BALANCED``s result are often not noticeable. Nonetheless in situations that do not care about real-time interactivie and where high precision is necessary, this is the mode of choice

### 2.2 Creating the Transform

To transforme a 1-dimensional image (henceforth assumed to be a ``crisp::GrayScaleImage``) we first allocate the transform itself and then call ``FourierTransform::transform_from(const Image<T, 1>&)``:

```cpp
auto image = load_grayscale_image(/*...*/ + "/crisp/docs/frequency_domain/cube.png");

auto fourier = FourierTransform<BALANCED>();
fourier.transform_from(image);
```

### 2.3 Visualizing the Spectrum

Before we continue it is instructive to note that just like pretty much any class in ``crisp`` we can simply bind the transform to a sprite and render it:

```cpp
auto spectrum_image = fourier.as_image();
auto phase_angle_image = fourier.phase_angle_as_image();

// render or save to disk
```

This is the image we started out with, a simple rectangle: <br>
![](./cube.png)

This is it's spectrum:<br>
![](./cube_spectrum.png)<br>

And it's phase angle (potential seizure warning) is available [here](#cube-phase-angle).

We note that the spectrum is four times the size of the image and that it is already *centered*, that is, it's largest component (called the *dc component*) is at the center of the image. This is done to faciliate easier viewing, being a series of oscillating functions the spectrum is of course periodic so we can simply shift the view such that the dc component lines up with the center.

When rendering the spectrum the coefficients magnitudes are log-scaled and normalized into [0,1] *only for viewing*. This is to make inspection easier, the actual dc component is commonly 1000x high than any other components which in image form would make it very hard to inspect the full spectrum.

### 2.4 Accessing Coefficients

Now that we know what the data looks like, this is how we access it:

```cpp
// get a coefficient as std::complex
std::complex<Value_t> get_coefficient(size_t x, size_t y) const;

// get the magnitude component of a coefficient
Value_t& get_component(size_t x, size_t y);
Value_t get_component(size_t x, size_t y) const;

// get the phase angle component of a coefficient
Value_t get_phase_angle(size_t x, size_t y) const;
Value_t& get_phase_angle(size_t x, size_t y);

// access the dc components magnitude directly
Value_t get_dc_component() const;
```

Where x and y are the row and column index of the matrix that we just rendered as an image. We can get the size of the spectrum with ``FourierSpectrum::get_size()`` and since the dc component is at the center, we can also access it like so:

```cpp
auto dc_one = fourier.get_dc_component();
auto dc_two = fourier.get_component(fourier.get_size().x() / 2, fourier.get_size().y() / 2);

assert(dc_one == dc_two);
```

### 2.5 Transforming Back Into an Image

We can transform the spectrum back into an image using ``Image<Inner_t, 1> transform_to() const`` but before we do that let's alter the spectrum so we can notice it's effects. We set part of the spectrum to zero resulting in a modified spectrum that looks like this:

```cpp
auto dc = spectrum.get_dc_component();
for (size_t x = 0; x < spectrum.get_size().x(); ++x)
for (size_t y = 0; y < spectrum.get_size().y(); ++y)
{
    if (x == spectrum.get_size().x() * 0.5 or y == spectrum.get_size().y() * 0.5)
        spectrum.get_component(x, y) = dc;
}
```
Here we set each component along the center axis to the same value as the dc component. The spectrum now looks like this:

![](cube_spectrum_modified.png)

There is no real mathematical purpose to this, it is simply to demonstrate the stark difference once we transform the spectrum back into an image:

```cpp
auto result = spectrum.transform_to<GrayScaleImage>();
// save or render
```

![](cube_modified.png)

The original image is unrecognizable but we do note the typical periodicity that is inherent to all spectral techniques.

## 2. Spectral Filters
## 2.1 Creating and Visualizing Filters

Spectral filters can be best thought of as floating point valued matrices of the same size as the fourier spectrum (2*m*2*n as mentioned before) we're trying to filter. Applying the filter usually means multiplying it by the spectrum. 

Similar to spatial filters, ``crisp`` offers a class called ``FrequencyDomainFilter``. We create a filter by specifying it's size:

```cpp
auto filter = FrequencyDomainFilter(2*m, 2*n);

// or let the filter determine the size from a spectrum
auto filter = FrequencyDomainFilter(spectrum);
```

We then need to specify the filters *shape* via ``set_function``. Remember that our filters are 2d matrices so (like everything in crisp) we can visualize them by binding them to a ``crisp::Sprite``:

```cpp
auto sprite = Sprite();
sprite.create_from(filter);
// render

// or

auto image = filter.as_image<GrayScaleImage>();
// save to disk
```
## 2.2 Filter Shapes

The simplest filter shape is ``FrequencyDomainFilter::identity()``. In order to not modify the spectrum after applying (multiplying) the filter we would expect the filter to be valued 1 at all positions:

```
filter.set_function(filter.identity());
```

![](./identity.png)

Visualization confirms this is indeed the case.

## 2.2.1 Low-Pass Filters

Low-Pass filters attenuate (diminish) higher frequencies and pass (do not modify) lower frequencies. For our fourier spectrum the lower frequency is the dc component at the center, the higher frequencies are more towards the outer edges of it. 

``crisp`` provides three types of low-pass filters:

+ ``ideal_lowpass`` has a sharp cutoff point between is attenuating and passing regions:<br>
![](./ideal_lowpass.png)<br><br>
+ ``gaussian_lowpass`` has a smooth transition between attenuating and passing regions that follows a [gaussian distribution](https://en.wikipedia.org/wiki/Gaussian_filter) :<br>
![](./gaussian_lowpass.png)<br><br>
+ ``butterworth_lowpass`` of order n is a filter that also has a smooth distribution that follows the [butterworth distribution of order n](https://en.wikipedia.org/wiki/Butterworth_filter). This filter approaches the ideal lowpass for order n -> infinity and approaches the gaussian lowpass for order n -> 0:
![](./butterworth_0_lowpass.png)<br>
![](./butterworth_2_lowpass.png)<br>
![](./butterworth_10_lowpass.png)<br>












---
#### Cube Phase Angle
![](./cube_angle.png)<br>
[back to section](#23-visualizing-the-spectrum)


