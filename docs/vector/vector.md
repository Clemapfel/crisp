# Vectors

###[[<<< back to index]](../index.md)

## Table of Contents

## 1. Introduction

Vectors are at the heart of linear algebra and thus at the heart of crisp. One central design goal of crisp is to make all algorithms work with all types of images. This is possible through ``crisp::Vector`` which not only supports all arithmetic operations a scalar would but does so implicitely without the user having to manually convert things. While this is convenient it is still important to understand ``crisp::Vector`` especially if we're going to implement our own algorithms and need to know how things work under the hood

### 2 Construction

``crisp::Vector`` is a template that accepts two arguments:

```cpp
template<typename T, size_t N>
class Vector : public Eigen::Array</*...*/>
{
``` 
where ``T`` is the vectors *value type*, and ``N`` is what we'll call the vectors *dimension*. `T` is any arithmetic type, that is it must support all artithmetic operators that ``crisp::Vector`` supplies. 

We can create a vector multiple ways:

```cpp
// default ctor
auto vec = Vector<float, 3>();

// initializer list
auto vec = Vector<Value_t, 3>{1.f, 1.f, 1.f};

// one argument
auto vec = Vector<Value_t, 3>(1.f);
```

Where the one argument constructor assigns all components of the vector the specified value.

The initializer list ctor is enforced to a) only have elements of type ``Value_t`` (float in our above exmaple) and b) is exactly N long. 

A note for people less familiar with C++:

```cpp
// not valid code, will report error on compilation:
auto vec = Vector<float, 3>(1.f, 2.f, 3.f);
        
// instead use:
auto vec = Vector<float, 3>{1.f, 2. 3.f};
``` 

Note the use of ``{}`` braces instead of ``()``. This is called [list initialization](https://en.cppreference.com/w/cpp/language/list_initialization) and while the specifics are not important for now do keep in mind that you want to always use ``{}`` when initializing ``crisp::Vector`` this way.

### 3. Element Access

We can access a vectors elements using the following methods:

```cpp
T& operator[](size_t i);
T operator[](size_t i) const;

T& at(size_t i);
T at(size_t i) const;

// for vectors of size N = {4, 3, 2, 1}
T& x();
T x() const;

// for vectors of size n = {4, 3, 2}
T& y();
T y() const;

// for vectors of size N = {4, 3}
T& z();
T z() const;
```
As by convention ``at`` will throw an error when the index is out of bounds, ``operator[]`` will not.

## 4. Typedefs

For convenience the following typedefs for commonly used vector value types are provided and used henceforth when appropriate:

```cpp
using Vector2f = Vector<float, 2>;
using Vector2i = Vector<int, 2>;
using Vector2ui = Vector<size_t, 2>;

using Vector3f = Vector<float, 3>;
using Vector3i = Vector<int, 3>;
using Vector3ui = Vector<size_t, 3>;

using Vector4f = Vector<float, 4>;
using Vector4i = Vector<int, 4>;
using Vector4ui = Vector<size_t, 4>;
```

## 5.1 Vector-Vector Arithmetics

``crisp::Vector`` supplies the following operators for vector-vector arithmetics. Recall that our vectors are not matrices, as such all operations are *elementwise*

```cpp
// arithmetic
Vector<T, N> operator+(const Vector<T, N>& other) const;
Vector<T, N> operator-(const Vector<T, N>& other) const;
Vector<T, N> operator*(const Vector<T, N>& other) const;
Vector<T, N> operator/(const Vector<T, N>& other) const;

// bitwise 
Vector<T, N> operator&(const Vector<T, N>& other) const;
Vector<T, N> operator|(const Vector<T, N>& other) const;
Vector<T, N> operator^(const Vector<T, N>& other) const;

// boolean
Vector<T, N> operator&&(const Vector<T, N>& other) const;
Vector<T, N> operator||(const Vector<T, N>& other) const;
```

similarly the corresponding assignment operators are provided

```cpp
Vector<T, N>& operator+=(const Vector<T, N>&);
Vector<T, N>& operator-=(const Vector<T, N>&);
Vector<T, N>& operator*=(const Vector<T, N>&);
Vector<T, N>& operator/=(const Vector<T, N>&);

Vector<T, N>& operator&=(const Vector<T, N>&);
Vector<T, N>& operator|=(const Vector<T, N>&);
Vector<T, N>& operator^=(const Vector<T, N>&);
```

To illustate the property of being element-wise, a small example:

TODO

## 5.2 Vector-Scalar Arithmetics

Vector-Scalar arithmetics can be best understood by the following equivalency:

```cpp
auto vector = Vector3f{1.f, 2.f, 3.f};
auto scalar = 5.f;

vector = vector * scalar;
// is equivalent to:
vector = vector * Vector3f{scalar, scalar, scalar};
```
All vector-vector operators are likewise available as vector-scalar:

```cpp
// arithmetic
Vector<T, N> operator+(T scalar) const;
Vector<T, N> operator-(T scalar) const;
Vector<T, N> operator*(T scalar) const;
Vector<T, N> operator/(T scalar) const;

// bitwise 
Vector<T, N> operator&(T scalar) const;
Vector<T, N> operator|(T scalar) const;
Vector<T, N> operator^(T scalar) const;

// boolean
Vector<T, N> operator&&(T scalar) const;
Vector<T, N> operator||(T scalar) const;

// assignment
Vector<T, N>& operator+=(T scalar);
Vector<T, N>& operator-=(T scalar);
Vector<T, N>& operator*=(T scalar);
Vector<T, N>& operator/=(T scalar);
Vector<T, N>& operator&=(T scalar);
Vector<T, N>& operator|=(T scalar);
Vector<T, N>& operator^=(const Vector<T, N>&);
```

This is important as it means that a ``crisp::Vector<T, 1>`` is notationally equivalent to ``T``. Furthermore specifically for N = 1 the following additional methods are defined:

```cpp
// only for Vector<T, 1>
Vector(T);
operator T();
``` 
neither of which are explicit which means ``crisp::Vector<T, 1>`` tends to decay to it's value type during usage. This is intended behavior and is what makes all of crisps algorithms so generically employable. 

## 6.1 Vector-Vector Comparison

So this one might be unusual, while mathematically comparing a vector ``{1, 3, 2}`` with vector ``{3, 2, 1}`` doesn't really make sense, in ``crisp`` this is a valid operations. While the order of vectors cannot be predicted, there is always a unique and unambigous order which means we can use vectors in ordered sets or as keys. This is possible because the following hash function is defined:

```cpp
template<typename T, size_t N>
struct std::hash<crisp::Vector<T, N>>;
```
The exact implementation of this is available [here](../../.src/vector.inl) but for now just know that if we're ordering vectors, we're ordering them based on their hashs. The following comparison operators are available:

```cpp
// elementwise equality works exactly as normal
bool operator==(const Vector<T, N>& other) const;
bool operator!=(const Vector<T, N>& other) const;

// all others we're comparing a vectors hash with anothers
bool operator<(const Vector<T, N>&) const;
bool operator<=(const Vector<T, N>&) const;
bool operator>(const Vector<T, N>&) const;
bool operator>=(const Vector<T, N>&) const;
```

## 6.2 Vector-Scalar Comparison

Similarly to vector-scalar arithmetics we can also compare vectors with scalars, this is again best imagined like so:

```cpp
auto a = crisp::Vector3f{1.f, 2.f, 3.f};

a < 1.f
// is equivalent to
a < Vector3f{1.f, 1.f, 1.f}
```

As such the following operators are provided:

```cpp
bool operator==(T) const;
bool operator!=(T) const;

bool operator<(T) const;
bool operator<=(T) const;
bool operator>(T) const;
bool operator>=(T) const;
```

Again, this allows ``crisp::Vector<T, 1>`` to be notationally equivalent to a scalar of the same value type 
