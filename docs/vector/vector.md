# Vectors

In crisp, vectors are *not* matrices. Vectors act like scalars in all but name, they support the same arithmetic operations for both vector-vector and vector-scalar. Working with a vector, all operations are element-wise, keep this in mind.

To access a vectors element the following functions are provided

```cpp
crisp::Vector<T, N>

// access without bounds checking
T& operator[](size_t);

// const access without bounds checking
T operator[](size_t) const;

// access with bounds checking
T& at(size_t);

// const access without bounds checking
T at(size_t) const;

// for N = 2, N = 3 or N = 4
T& x();
T x() const;

T& y(); 
T y() const;

// only for N = 3 or N = 4
T& z();
T z() const;

// only for N = 4
T& w();
T w() const;
```

Any Vector with less than 4 elements supports element-access via ``.x, .y, .z, .w``, of course a 2-vector doesn't have a z element so the operator is unavailable.

1-Vectors are a special case, they don't support any form of element-access instead they *decay* into their element type, that is at the first opportunity an object of class ``crisp::Vector<float, 1>`` will simply transform itself into a float and act as such. We don't have to worry about this, it all happens implicitely and from our perspective a 1-vector is identical to a true scalar. 

Because of the 1-vectors behavior, crisps vector only support arithmetic types like such as ``float``, ``int``, ``size_t``, ``char``, ``boost::uint256_t``, etc.. 

As mentioned before math-wise vectors behave the same as scalars. The following arithmetic operations are supported (remember that all operations happen element-wise):


The following operations are supported:

```cpp
// binary vector-vector arithmetic
Vector<T, N> operator+(const Vector<T, N>&) const;
Vector<T, N> operator-(const Vector<T, N>&) const;
Vector<T, N> operator*(const Vector<T, N>&) const;
Vector<T, N> operator/(const Vector<T, N>&) const;

// arithmetic vector-vector assignment
Vector<T, N>& operator+=(const Vector<T, N>&);
Vector<T, N>& operator-=(const Vector<T, N>&);
Vector<T, N>& operator*=(const Vector<T, N>&);
Vector<T, N>& operator/=(const Vector<T, N>&);

// boolean vector-vector comparison [1]
bool operator==(const Vector<T, N>&) const;
bool operator!=(const Vector<T, N>&) const;

// binary vector-scalar arithmetic
Vector<T, N> operator+(T scalar) const;
Vector<T, N> operator-(T scalar) const;
Vector<T, N> operator*(T scalar) const;
Vector<T, N> operator/(T scalar) const;
Vector<T, N> operator%(T scalar) const;

// vector-scalar assignment
Vector<T, N>& operator+=(T scalar);
Vector<T, N>& operator-=(T scalar);
Vector<T, N>& operator*=(T scalar);
Vector<T, N>& operator/=(T scalar);
Vector<T, N>& operator%=(T scalar);

// boolean and bitwise vector-scalar arithmetics
Vector<T, N> operator&(T t) const;
Vector<T, N> operator&&(T t) const;
Vector<T, N> operator|(T t) const;
Vector<T, N> operator||(T t) const;
Vector<T, N> operator^(T t) const;

// bitwise vector-scalar assignment
Vector<T, N>& operator&=(T t);
Vector<T, N>& operator|=(T t);
Vector<T, N>& operator^=(T t);

// boolean and bitwise unary
Vector<T, N> operator~() const;
Vector<T, N> operator!() const;

// scalar assignment
Vector<T, N>& operator=(T t); // vec = t equivalent to vec = Vector<T, N>(t)

// scalar boolean comparison
bool operator==(T) const;
bool operator!=(T) const;
```

That's a lot so let's go over the unconventional ones:

```cpp
+







```