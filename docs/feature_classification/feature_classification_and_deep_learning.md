# Feature Classification & Deep Learning

Bayes Classifier, Fully Connected Neural Networks, Convolutional Neural Networks, SIFT

```cpp
#include <classification/bayes_classifier.hpp>
#include <classification/sift.hpp>
#include <classification/fully_connected_neural_networks.hpp>
#include <classification/convolutional_neural_networks.hpp>

// all of the above collected in:
#include <classification.hpp>
```

## Table of Contents

## 1. Introduction

We learned how to first extract part of an image into a [segment](../segmentation/segmentation.md), then compute unique descriptors that describe the [regions](../feature_extraction) shape and texture. Now it is finally time to use these descriptors to solve one of the most common image processing applications: classifying a region.

To classify an object means to divide it into a number of classes `{C_0, C_1, ..., C_m-1}`. For the sake of simplicity we assume that on object can only belong to exaclty one class. While this assumption is not necessary for any of `crisp`s deep learning features, it will make things easier to follow later.

### 1.1 Features & Class Membership

For one specific region, class membership is given as a `m*1` matrix `M`:

| class index | `M(i, 0)`
|-------------|--------
| 0           | {0, 1}
| 1           | {0, 1}
| ...         | ...
| m-1         | {0, 1}

Where if `M(4, 0) == 1` then the object is classified into class `C4`. Because an object can only belong to one class at a time, `M.sum() == 1` (as all values except for the single `1` are `0`).

To classify an object we need to describe it somehow, we do this using a `n*1` matrix called a *feature vector*. Each row of this matrix `N` corresponds to one feature:

| feature index | `N(i, 0)`
|---------------|--------
| 0             | [-1, 1]
| 1             | [-1, 1]
| ...           | ...
| n-1           | [-1, 1]

When classifying multiple objects, all objects have a feature vector of the same size and each row corresponds to the same type of feature. All that changes is the value of the feature. As we saw in the table above, it is necessary to normalize the value of the feature into range `[-1, 1]`. In the literature features are often normalized into `[0, 1]` but beause this is a subset of of `[-1, 1]` either approach works in `crisp`.

A *training dataset* in `crisp` is a set of feature vectors and their corresponding, desired classifications. In practice this set are two matrices: 

+ `FeatureMatrix_t` is a n*x matrix where n the number of features and x the number of samples in the population. Each row is one feature, each column is one sample. All it's values should be normalized into [-1, 1]
  
+`ClassificationMatrix_t` is a m*x matrix where m the number of classes and x the number of sample. All elements of a row should be either 1 or 0

The number of columns `x` has to be the same for both of these. If one object has it's feature vector as the column of the feature matrix at position i, the the iths column of the classification matrix holds the desired classification.

It may be instructive to consider a practical example. Consider the following data set:

ducks: 38
chickens: 32


| sample | class   | egg weight | yolk to white ratio
|--------|---------|------------|--------------
|0       | Chicken | 52         |    31%
1       | Chicken | 58         |    34%
2       | Chicken | 67         |    32%
3       | Chicken | 62         |    37%
4       | Chicken | 59         |    32%
5       | Chicken | 65         |    29%
        |         |            |
6       | Duck    | 61         |    36%
7       | Duck    | 63         |    35%
8       | Duck    | 69         |    39%
9       | Duck    | 60         |    38%
10       | Duck    | 71        |    37%
11       | Duck    | 75        |    34%

Here we have a set of 12 eggs from both chickens and ducks. Each egg sample has 3 data point: wether it was from a chicken or duck, it's weight and it's yolk-to-white-ratio.
We want to create a classifier that uses this dataset and decides wether an egg we hand it is a chicken or duck egg.

Before we can use `crisp` for classification, however, we need to arrange the data in a way that `crisp` understands.

We have `m = 2` classes and `n = 2` features. We are trying to predict the "class" propery of each egg:

```cpp
auto classes = Eigen::Matrix<2, 12>();

classes << 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, // is chicken
           0, 0, 0, 0, 0, 1, 1, 1, 1, 1; // is duck
```

Each egg sample gets a 2x1 feature vector that is 1 in the first row if it is from a chicken, 1 in the second row if it is from a duck. We now enter features in a similar matrix keeping the column order the same. Because `crisp` needs features to be in the range [-1, 1] we also divide all percentages and weights by 100:

```cpp
auto features = Eigen::Matrix<2, 12>();

features << .52, .58, .67, .62, .59, .65, .61, .63, .69, .60, .71, .75, // weight
            .31, .34, .32, .37, .32, .29, .36, .35, .39, .38, .37, .34; // yok ratio
```

We now have converted our table into a form that `crisps` algorithms understand.


### 2. Bayes Statistical Classifier

The simplest classifier in `crisp` is `crisp::BayesClassifier`. This class has two template parameters:

```cpp
template<size_t FeatureN, size_t ClassN>
class BayesClassifier
{
```
+ `FeatureN` is the number of features (`n` in our notation)
+ `ClassN` is the number of classes (`m` in our notation)

Both need to be available at compile time. 

The classifier works by estimating the type of distribution, mean and variance from a given population. Then when we hand it a sample (egg) to identify, it calculates the propabilities that our egg is from a specific class. For us it would calculated 
```
p(x | chicken)  // is our current egg x from a chicken
p(x | duck)     // is our current egg x from a duck
```

The return values of the classifier are not actual probabilities (they can be outside the range of [0, 1]) however the higher the actual probablity of class-membership is, the higher a score the classifier will assign to it.

### 2.1 Training the Bayes Classifier

To estimate the values it needs for classification we offer it our training set from before like so:

```cpp
auto classes = Eigen::Matrix<2, 12>();

classes << 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, // is chicken
           0, 0, 0, 0, 0, 1, 1, 1, 1, 1; // is duck
           
auto features = Eigen::Matrix<2, 12>();

features << .52, .58, .67, .62, .59, .65, .61, .63, .69, .60, .71, .75, // weight
            .31, .34, .32, .37, .32, .29, .36, .35, .39, .38, .37, .34; // yok ratio

auto bayes = BayesClassifier<2, 2>(); // 2 features, 2 classes
bayes.train(feature, classes);
```

That is all, the classifier is now primed to identify any egg we hand it. Unlike neural networks which will be discussed later in this section, the bayes classifier achieves optimality (under it's heavily restricive assumptions) with just one training cycle. 

### 2.2 Identification using the Bayes Classifier

We just came back and found 3 eggs around our imaginative farm. We measure their size and content:

| egg | weight | yolk ratio
|-----|--------|-----------
| 0   |  51    |  29%
| 1   |  67    |  37%
| 2   |  70    |  38%  

We could ask one-by-one but it is often easier to instead convert them to a feature matrix just like we did with the training data:

```cpp
auto to_identify = Eigen::MatrixXf(2, 3);

//               0    1    2    // egg
to_identify << .51, .67, .70,   // weight
               .29, .37, .38;   // yolk ratio
```

We can identify this set using just one operation:

```cpp
auto result = bayes.identify(to_identify);
std::cout << result << std::endl;
```
     




### 2.2 Architecture

A neural network has `l` layers. Each layer `L_i` has a number of *neurons* `#L_i`. 
+ `L_0` is called the *input layer*. This layer has the same number of neurons as the feature vectors size: `#L_0 = n`
+ `L_l` is called the *output layer*. This layer as the same number of neurons as the number of possible classes: `#L_l = m`
+ `L_i` for `i` in `{1, 2, ..., l-1}` are called *hidden layers*

The number of layers and the number of neurons per layer completely define a networks *architecture*. We can specify it in `crisp` using the following template:

```cpp
template<size_t... NeuronsPerLayer>
class NeuralNetwork;

auto nn = NeuralNetwork<n, #L_1, #L_2, ..., m>();
```

Where `n`, `m`, `#L_i` as defined above. For example:

```cpp
// binary feature vector: n = 2
// two possible classes: m = 2
// one hidden layer with 3 neurons: #L_1 = 3
auto nn = NeuralNetwork<2, 3, 2>();

// 20 features, 2 hidden layers with 3 neurons each, 2 classes
auto nn = NeuralNetwork<20, 3, 3, 2>();

// 2 features, 5 layers with 10 neurons each, 19 classes
auto nn = NeuralNetwork<2, 10, 10, 10, 10, 10, 19>();
```

A networks architecture governs it's structure. Each neuron has a number of *weights* and one *bias*. It is not important what exactly these do, just now that the networks architexture along with each neurons weights and biases completely define a network. There are no other members or parameters that change how a network classifies a given feature vector.

## 2.3 Creating a Network

We can construct a fresh network like so:

```cpp
#include <classification/fully_connected_neural_network.hpp>

// in main.cpp
auto nn = crisp::NeuralNetwork<2, 2, 2>();
```

This initializes all weights to `1 + eps` and all biases to `0 + eps`, where `eps in [-0.25, +0.25]` (random, uniformly distributed). <br>
Often we don't want to have to re-train a network anytime our appliciation restarts. We can export a neural networks at any time using `NeuralNetwork::as_string()`.

```cpp
auto nn = crisp::NeuralNetwork<2, 2, 2, 2>();
auto exported = nn.as_string();
// save exported to file
```

We can then load a neural network from the disk like so:

```cpp
auto nn = crisp::NeuralNetwork<2, 2, 2, 2>();

auto file = std::ifstream(/*...*/ + "/crisp/docs/feature_classification/xor_nn.bin");
std::stringstream buffer;
buffer << file.rdbuf();
std::string str = buffer.str();
nn.from_string(str);
``` 
Here we're opening a file stream, moving it's content into a buffer stringstream and then creating a plain string from said stringstream. The neural network can then parse then string and load the weights. This is somewhat clumsy owing to C++s file interface so it is recommended that you save the string generated using `as_string()` as you see fit.<br>
Note that for loading to be succesfull, the exported network and the network we're trying to import have to have identical architectures. The architecture has to be available at compile time and can thus not be loaded later on so we need to keep track of it some other way.

## 2.4 Classifying a Feature Vector

Next we will illustrate the networks usage using one of the simplest non-linearly seperable classification problems. For two booleans `A` and `B`, `A xor B` is `false` if `A == B` and `true` if `A != B`. We want a network that gets two arbitrarily valued features `f1`, `f2` and decides wether `f1 xor f2` should return `true` or `false`:

| A  | B  | A xor B |
|----|----|---------|
| -1 |  1 | true    |
|  1 | -1 | true    |
| -1 | -1 | false   |
|  1 |  1 | false   |

Here we transform the features from the value range {true, false} into `[-1, 1]`as stated earlier. 

Let's first conceptually write out what the correct output should be. We recall that feature vectors are n*1 matrix, 2*1 in our case:

```cpp
Eigen::Matrix<2, 1> feature;
feature << f1, 
           f2;
```

When handing this feature vector to a neural network, it will return a classification that describes the likelihood of which of the two classes (true or false) it belongs to. Let's says index 0 (c0) corresponds to `true` and index 1 (c1) corresponds to `false`. Then:

```cpp
Eigen::Matrix<2, 1> class;
class << c0,
         c1;
```

If `c0 > c1` we say that the feature vector belongs to `true`, if `c1 > c0` we sasy it belongs to `false`.

We load the already trained neural network into memory, then we create a feature vector and call `identify()`:

```cpp
auto nn = crisp::NeuralNetwork<2, 2, 2, 2>();
nn.from_string(str);

Eigen::MatrixXf feature;
feature.resize(2, 1);
feature << -1, 
            1;
            
auto result = nn.identify(feature);
std::cout << result << std::endl;
```
```
0.986431
0.020584
```

As expected we get two values, the nn decided that the score it assigns to the feature vector `{-1, 1}^T` belonging to `c0` (true) is `0.986431` and the score assigned to it belong to `c1` is `0.020584`. `c0` > `c1` so we would assign this feature sample to c0.

Recall that the output values are normalized into the range [0, 1] so a score of `~0.99` is fairly high while a score of `~0.02` is fairly low. This gives us a lot of confidence that the nn is sure that this classification is correct. 

Repeating the above process with different feature vectors:

```cpp
feature << -1,
           -1;
```
```
0.0887615
0.983745
```

We get a very confident result that this feature vector should be assigned class `c1`.

```cpp
feature << 0, 
           0;
```

```
0.924916
0.739695
```

This time the two values of c0, c1 are much closer together. While we would still assing {0, 0}^T to c0 because `0.924916 > 0.739695` this result is much less obvious.

It is laborious to identify one feature vector at a time, to alleviate this `crisp::NeuralNetwork::identify()` can take a matrix of size `n*x` where `n` the number of features and `x` the number of samples. That is each column is one individual feature vector and each row is one feature. We can do that above three calls all at once like so:

```cpp
Eigen::MatrixXf multiple_features;
multiple_features << -1, -1, 0,
                      1, -1, 0;

auto result = nn.identify(multiple_features);
```
```
0.0997334 0.0887615  0.924916
 0.983745  0.984512  0.739695
```
We recognize the previous results in the columns of the output matrix. Each column corresponds to it's respective feature sample, each row corrensponds to that feature samples classification.

## 2.5 Training a Network

Of couse in praxis we will often have to train the networks ourself. For this we need a *training set*  which is a set of features and classifications that a human made sure are true positives. In `crisp` all the features can be packed into one `n*x` matrix (where, again, each column is one sample and each row one feature) and their corresponding desired classifications go into an `m*x` matrix where each colum of the desired output corresponds to the feature sample in the same column. 

Translating our table from earlier:

| A  | B  | A xor B |
|----|----|---------|
| -1 |  1 | true    |
|  1 | -1 | true    |
| -1 | -1 | false   |
|  1 |  1 | false   |

```cpp
Eigen::MatrixXf features;
features << -1,  1, -1,  1   // A
             1, -1,  1,  1;  // B

Eigen::MatrixXf desired;
features <<  1,  1,  0,  0,  // true
             0,  0,  1,  1;  // false
```

Each column in `features` should only have a single 1 that governs which class the same column in `features` should be assigned to.

Now that we have our tiny training set we first create a fresh network:

```cpp
auto nn = NeuralNetwork<2, 2, 2>();
```

Note how it's architecture differs, previously it was `NeuralNetwork<2, 2, 2, 2>()`. 

`crisp::NeuralNetwork` offers two training functions:

```cppic








