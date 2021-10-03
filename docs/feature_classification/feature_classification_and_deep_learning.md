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

To classify a region means to assign it a label. Usually this means to label it as belonging to a certain group, let's assume for the sake of simplicity that we have 3 possible groups: `c0`, `c1` and `c2`, where any one region can only belong to exactly one group. This restriction is not technically necessary but it will keep things more easy to follow later. 


It may be instructional to consider an example. Say that we're trying to classify white, round balls into three classes: Field-Hockey balls, Golfballs and Tabletennis Balls:


![](https://upload.wikimedia.org/wikipedia/commons/thumb/2/2f/Balle_de_hockey_sur_gazon_avec_pi%C3%A8ce_de_5_francs.jpg/1920px-Balle_de_hockey_sur_gazon_avec_pi%C3%A8ce_de_5_francs.jpg)
<br>
A field hockey ball (source: wikimedia)<br><br>
![](https://upload.wikimedia.org/wikipedia/commons/e/e9/Golf_ball_titleist_1.jpg)
A golf ball (source: wikimedia)<br><br>

![](https://m.media-amazon.com/images/I/81crdESgtLL._AC_SL1500_.jpg)
Three table-tennis balls (source: schildkröt online store)<br>

In image-processing we rarely have information like weight and relative size available so we will have to work with purely visual elements.



## 2. Fully Connected Neural Network

### 2.1 Feature Vectors and Class Affiliation

We formalize the approach as such:

Each region has a series of descriptors called a *feature vector*:

```cpp
f1,
f2,
/*...*/,
fn
```

This is a n*1 matrix where each row f1, f2, ... is the numerical value of a feature. It is recommended to normalize all features into the range of [0, 1] (or [-1, 1] depending on the network architecture).

For each feature vector we want to assign it to one of m classes. Class-affiliation is given as a m*1 matrix

```cpp
c1, 
c2,
/*...*/,
cm
```

Where each row ci is either `0` or `1` and `c1 + c2 + ... + cm = 1` (that is there is only exaclty one ci that has the value 1, all others are 0). Note that `m` and `n` can be different sizes, it's reasonable to assign objects with 20 features into only 2 classes or (less reasonably) assign objects with 2 features into 20 classes.

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

```cpp








