[[<< back to index]](../index.md)

---
# [Vectors](../../include/vector.hpp)

n-dimensional vectors of artihmetic value type, vector-vector and vector-scalar arithmetics, comparison operators

```cpp
#include <vector.hpp>
```

## Table of Contents

1. [**Introduction**](#1-introduction)<br>
2. [**Construction**](#2-construction)<br>
3. [**Component Access**](#3-component-access)<br>
4. [**Typedefs for Convenience**](#4-typedefs)<br>
5. [**Arithmetics & Comparison Operators**](#5-arithmetics-and-comparison-operators)<br>
    5.1 [Vector-Vector Arithmetics](#51-vector-vector-arithmetics)<br>
    5.2 [Vector-Scalar Arithmetics](#52-vector-scalar-arithmetics)<br>
    5.3 [Vector-Vector Comparison](#53-vector-vector-comparison)<br>
    5.4 [Vector-Scalar Comparison](#54-vector-scalar-comparison)<br>
   

## 1. Introduction

Vectors are at the heart of linear algebra and thus at the heart of crisp. One central design goal of crisp is to make many algorithms and methods work with any type of image. This is made possible by ``crisp::Vector``, it behaves exactly like a scalar in notation which means there is no difference when dealing with a vector of 4 floats and just a float. While most of this functionality happens implicitly it is still important to know how `crisp::Vector` works under the hood, should we ever need it.

## 2. Construction

``crisp::Vector`` is a template that accepts two arguments:

```cpp
template<typename T, size_t N>
class Vector : public Eigen::Array</*...*/>
{
``` 
where ``T`` is the vectors *value type*, and ``N`` is what we'll call the vectors *dimension* (sometimes called rank in the literature). `T` can be any arithmetic type such as `int`, `float`, `double`, `bool`, `boost::uint256_t`, etc..

We can access a vectors value types using the exposed public member typedef `Value_t`.

We can create a vector in multiple ways:

```cpp
#include <vector.hpp>

// default ctor
auto vec = Vector<float, 3>();

// initializer list
auto vec = Vector<Value_t, 3>{1.f, 1.f, 1.f};

// one argument
auto vec = Vector<Value_t, 3>(1.f);

// assignment from scalar (only for 1 dimension)
Vector<float, 1> one_d_vec = 1.f;
```

Where the one argument constructor assigns all components of the vector the same value as specified.

The initializer list constructor is enforced to only have elements of type ``Value_t`` (``float`` in our above example) and to be exactly ``N`` long. 

A note for people less familiar with C++:

```cpp
// not valid code, will report "no valid constructor" on compilation
auto vec = Vector<float, 3>(1.f, 2.f, 3.f);
        
// valid
auto vec = Vector<float, 3>{1.f, 2.f 3.f};
``` 

Note the use of ``{}`` braces instead of ``()``. This is called [list initialization](https://en.cppreference.com/w/cpp/language/list_initialization) and while the specifics are not important for now, do keep in mind that you want to always use ``{}`` when initializing ``crisp::Vector`` this way.

## 3. Component Access

We can access a vectors components using the following methods:

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

// for vectors of size N = {4}
T& w();
T w() const;
```
As by convention ``at`` will throw an exception when the index is out of bounds, ``operator[]`` will not. Operators ``x()``, `y()`, `z()` and `w()` are available as syntactic sugar.

## 4. Typedefs

For convenience, the following typedefs for commonly used value types are provided:

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

## 5. Arithmetics and Comparison Operators

### 5.1 Vector-Vector Arithmetics

``crisp::Vector`` offers the following operators for vector-vector arithmetics. Recall that our vectors are not matrices, as such all operations are applied *elementwise*. 

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

The corresponding assignment operators are also provided:

```cpp
Vector<T, N>& operator+=(const Vector<T, N>&);
Vector<T, N>& operator-=(const Vector<T, N>&);
Vector<T, N>& operator*=(const Vector<T, N>&);
Vector<T, N>& operator/=(const Vector<T, N>&);

Vector<T, N>& operator&=(const Vector<T, N>&);
Vector<T, N>& operator|=(const Vector<T, N>&);
Vector<T, N>& operator^=(const Vector<T, N>&);
```

### 5.2 Vector-Scalar Arithmetics

Vector-Scalar arithmetics can be best understood by the following equivalency:

```cpp
auto vector = Vector3f{1.f, 2.f, 3.f};
auto scalar = 5.f;

vector = vector * scalar;
// is equivalent to:
vector = vector * Vector3f{scalar, scalar, scalar};
```
That is, scalar-operators are applied to all components at the same time.
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

Furthermore, specifically for `N = 1` the following additional methods are defined:

```cpp
// only for Vector<T, 1>
explicit (N != 1) Vector(T);
explicit (N != 1) operator T();
``` 

Neither of which are explicit, which means ``crisp::Vector<T, 1>`` tends to decay to its value type during usage. This means we can use it like so:

```cpp
float scalar = 1.f;
Vector<float, 1> vector = 1.f;

scalar = vector;
vector = scalar;
vector = vector * scalar;
scalar += vector * vector;
//etc.
```

This is what makes all of crisps algorithms generically employable.

### 5.3 Vector-Vector Comparison

#### 5.3.1 Hash Comparison and Collision


While mathematically comparing vector ``{1, 3, 2}`` with vector ``{3, 2, 1}`` is usually undefined, in ``crisp`` this is a valid operation. Which of these vectors is larger than the other one cannot necessarily be predicted, however there is always a unique and unambiguous answer which means we can use vectors in ordered sets or as keys as they can be sorted just like scalars. 

When comparing vectors, we are not comparing them element wise, rather we're converting them to their unique hash value and comparing the hashes. To understand why this is possible, we first need to consider the following hash function:

```cpp
template<typename T, size_t N>
struct std::hash<crisp::Vector<T, N>>;

// or as member:
template<typename T, size_t N>
size_t crisp::Vector<T, N>::to_hash() const;
```

The hash is defined for all valid `T` including floating point. Depending on the value type, hash collisions are guaranteed to not happen only for a specific value range and N as such:

| value type T | N | no hash collisions if all components in|
|--------------|---|-----------------------|
| uint32_t     | 1 | `[0, 2^64]`
| uint64_t     | 1 | `[0, 2^64]`
| uint32_t     | 2 | `[0, 2^32]`
| uint64_t     | 2 | `[0, 2^32]`
| uint32_t     | 3 | `[0, 2^16]`
| uint64_t     | 3 | `[0, 2^16]`
| uint32_t     | 4 | `[0, 2^16]`
| uint64_t     | 4 | `[0, 2^16]`
|              |   |
| int32_t      | 1 | `[-2^64, +2^64]`
| int64_t      | 1 | `[-2^64, +2^64]`
| int32_t      | 2 | `[-2^32, +2^32]`
| int64_t      | 2 | `[-2^32, +2^32]`
| int32_t      | 3 | `[-2^16, +2^16]`
| int64_t      | 3 | `[-2^16, +2^16]`
| int32_t      | 4 | `[-2^16, +2^16]`
| int64_t      | 4 | `[-2^16, +2^16]`
|              |   |
| float        | any | `[0, 1]`
| double       | any | `[0, 1]`

Illustrating usage of the table: If we construct a `crisp::Vector3f` then we're guaranteed to not have any hash collisions as long as all components of that vector are in `0 <= x <= 1`. If one component was to be `1.2`, we would be at risk of hash collisions. Similarly, if we create a vector of size 4 with value type `int64_t` (aka. `long int`) we need to keep the values bigger than `-1 * 2^32` and smaller than `1 * 2^32` or risk hash collisions. For vectors of type `bool` or arithmetic types with a bit-length of 16 or lower, hash collision are guaranteed to not occur for any possible value as long as N < 4. 

### 5.3.2 Comparison Operators

The following comparison operators are available:

```cpp
// element-wise equality works exactly as normal
bool operator==(const Vector<T, N>& other) const;
bool operator!=(const Vector<T, N>& other) const;

// all others we're comparing the two vectors hashes
bool operator<(const Vector<T, N>&) const;
bool operator<=(const Vector<T, N>&) const;
bool operator>(const Vector<T, N>&) const;
bool operator>=(const Vector<T, N>&) const;
```

### 5.4 Vector-Scalar Comparison

Similarly to vector-scalar arithmetics we can also compare vectors with scalars, this is again best imagined like so:

```cpp
auto a = crisp::Vector3f{1.f, 2.f, 3.f};

a < 1.f

// is equivalent to
a < Vector3f{1.f, 1.f, 1.f}

// which returns:
a.to_hash() < Vector3f{1.f, 1.f, 1.f}.to_hash()
```

Therefore the following operators are provided:

```cpp
bool operator==(T) const;
bool operator!=(T) const;

bool operator<(T) const;
bool operator<=(T) const;
bool operator>(T) const;
bool operator>=(T) const;
```

This means that for `N = 1`, `crisp::Vector` again behaves just like a scalar even in comparisons.

---
###[[<< back to index]](../index.md)