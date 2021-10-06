[[<< Back to Index]](../index.md)

---

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

1. [Introduction](#1-introduction) <br>
        1.1 [Features, Class Membership](#11-features--class-membership)<br>
2. [Bayes Statistical Classifier](#2-bayes-statistical-classifier)<br>
        2.1 [Training the Bayes Classifier](#21-training-the-bayes-classifier)<br>
        2.2 [Using the Bayes Classifier for Identification](#22-identification-using-the-bayes-classifier)<br>
3. [Neural Networks](#3-neural-networks)<br>
        3.1 [Architecture](#31-architecture)<br>
        3.2 [Weights and Biases](#32-weights-and-biases)<br>
        3.3 [Creating/Loading a Network](#33-creating-a-network)<br>
        3.4 [Training the Network](#34-training-the-network)<br>
        3.5 [Using the Network for Identification](#35-using-the-network-for-classification)<br>

## 1. Introduction

We learned how to extract part of an image into a [segment](../segmentation/segmentation.md), then compute unique descriptors that describe the selected [region's](../feature_extraction) shape and texture. Now, it is finally time to use these descriptors to solve one of the most common image processing applications: classification.

To classify an object means to divide the population it is from into a number of classes `{C_0, C_1, ..., C_m-1}`, then assign a label to any one specific object. This label represents what class it belongs to. For the sake of simplicity, for the rest of this tutorial, we assume that an object can only belong to exactly one class. While this assumption is not necessary for any of `crisp`s deep learning features, it will make things easier to follow later.

### 1.1 Features & Class Membership

For one specific region or object, class membership is given as a `m*1` matrix `M`:

| class index i | `M(i, 0)`
|-------------|--------
| 0           | {0, 1}
| 1           | {0, 1}
| ...         | ...
| m-1         | {0, 1}

Where if `M(4, 0) == 1` then the object is classified into class `C4`. Because an object can only belong to one class at a time, `M.sum() == 1` (as all values except for the single `1` are `0` and an object has to belong to at least one class).

In order to be able to classify an object we need to describe it somehow, we do this using a `n*1` matrix called a *feature vector*.  Each row of this matrix `N` corresponds to one feature:

| feature index | `N(i, 0)`
|---------------|--------
| 0             | [-1, 1]
| 1             | [-1, 1]
| ...           | ...
| n-1           | [-1, 1]

When classifying multiple objects from the same population, all objects in that population have to have a feature vector of the same size and meaning, that is for each object, each row means the same thing. All that changes is the actual values of the features. As we saw in the table above, it is necessary to normalize the value of the feature into the range `[-1, 1]`. In the literature, features are often normalized into `[0, 1]` but because this is a subset of `[-1, 1]` anyway, either approach works in `crisp`.

A *training data set* in `crisp` is a set of feature vectors and their corresponding, desired classifications. In practice, this set is represented by two matrices: 

+ `FeatureMatrix_t` is a `n*x` matrix where `n` the number of features and `x` the number of samples in the population. Each row is one feature, each column is one sample. All its values should be normalized into `[-1, 1]

+ `ClassificationMatrix_t` is a `m*x` matrix where `m` is the number of classes and `x` the number of sample. Each row is one class, each column is one sample. All elements of a row should be either 1 or 0

The number of columns, `x`, has to be identical for both matrices. If one object has its feature vector as the column of the feature matrix at position `i`, the `i`ths column of the classification matrix corresponds to that same object.

It may be instructive to consider a practical example. Consider the following data set:

| sample | class   | egg weight (g) | yolk to white ratio
|--------|---------|------------|--------------
0       | Chicken | 52         |    31%
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

Here we have a set of 12 eggs from both chickens and ducks. Each egg sample has 3 data point: whether it was from a chicken or duck, it's weight, and it's yolk-to-white-ratio.
We want to create a classifier can decide whether a new, unknown egg we hand it is from a chicken or duck.

Before we can use `crisp` for classification, we need to arrange the data in a way that `crisp` understands.

We have `m = 2` classes and `n = 2` features. We are trying to predict the "class" property of each egg, so we create a matrix with 2 rows, each column of the matrix has a single `1` depending on whether the sample was from a chicken or duck.

```cpp
auto classes = Eigen::Matrix<2, 12>();
// sample: 0  1  2  3  4  5  6  7  8  9  10 11
classes << 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // is chicken
           0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1; // is duck
```

Each egg sample gets a 2x1 feature vector that is 1 in the first row if it is from a chicken, or 1 in the second row if it is from a duck.

We transform the feature values in a similar way. Each column corresponds to one sample (egg), we arbitrarily decide that weight will be in row 0, yolk-ratio in row 1. Recall that features need to be normalized, so we divide both feature values by 100:
```cpp
auto features = Eigen::Matrix<2, 12>();
// sample:    0    1    2    3    4    5    6    7    8    9   10   11
features << .52, .58, .67, .62, .59, .65, .61, .63, .69, .60, .71, .75, // weight
            .31, .34, .32, .37, .32, .29, .36, .35, .39, .38, .37, .34; // yok ratio
```
We call the first matrix *class membership matrix* and the second a *feature matrix*. 

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

The classifier works by estimating the type of distribution, mean and variance from a given population. When we hand it a sample `x` to identify the classifier will return a score that is directly proportional to, in our chicken-or-duck example, the following probabilities:
```
p( x | chicken)  // is our current egg x from a chicken
p( x | duck)     // is our current egg x from a duck
```

The return values of the classifier are not actual probabilities (they can be outside the range `[0, 1]`), however the order and relative value of the actual probabilities are preserved. This means we can compare and quantify them, just like we would probabilities.

### 2.1 Training the Bayes Classifier

To estimate the values needed for classification, the bayes classifier needs to observe the training set population. We retrieve the feature- and classification matrix from earlier, then call `BayesClassifier::train`:

```cpp
auto classes = Eigen::Matrix<2, 12>();
classes << 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // is chicken
           0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1; // is duck

auto features = Eigen::Matrix<2, 12>();
features << .52, .58, .67, .62, .59, .65, .61, .63, .69, .60, .71, .75, // weight
            .31, .34, .32, .37, .32, .29, .36, .35, .39, .38, .37, .34; // yolk 
            
auto bayes = BayesClassifier<2, 2>(); // 2 features, 2 classes
bayes.train(feature, classes);
```

That's all, unlike deep learning procedures later in this section, the bayes classifier only needs to iterate through the training set once to achieve the maximum possible classification performance based on the evidence.

### 2.2 Identification using the Bayes Classifier

Sticking to our chicken-or-duck example, let's say we just came  home and found 3 eggs around our hypothetical farm and we really want to know whether it was the chickens or the ducks that hid them. We measure their size and content:

| egg | weight | yolk ratio
|-----|--------|-----------
| 0   |  51    |  29%
| 1   |  67    |  37%
| 2   |  70    |  38%  

We arrange these three samples just like we did with our training datas feature matrix:

```cpp
auto to_identify = Eigen::MatrixXf(2, 3);
//               0    1    2    // egg
to_identify << .51, .60, .70,   // weight
               .29, .28, .38;   // yolk ratio
```
We can then classify them using:

```cpp
auto result = bayes.identify(to_identify);
std::cout << result << std::endl;
```
```cpp
0.667373  0.500235  0.0936609
0.332627  0.499765  0.906339
```

We get a bunch of numbers, let's try to understand what they mean. Firstly, each column index corresponds to their respective egg sample:

```cpp
// 0       1        2
0.667373  0.500235  0.0936609
0.332627  0.499765  0.906339
```

The numbers are scores representing the relative probability that the sample in column `i` is from class `j` where `j` is the row index. In our training data set, we had chickens as `j = 0` and ducks as `j = 1`, so:

```cpp
// 0       1       2
0.667373  0.500235  0.0936609    // ~p(egg | chicken)
0.332627  0.499765  0.906339     // ~p(egg | duck)
```

Where `~p(x | y)` is a score that's directly proportional to the probability `p(x | y)`.

We decide which egg came from which species by simply comparing the probabilities, choosing the class membership with the higher score.


Technically we would assign egg `0` and `1` as chicken,`3` as duck, however the result for egg `1` is so ambiguous that it is basically a coin toss. To quantify this ambiguity, we assign each result a *confidence score*. One of the simplest confidence score is the absolute distance between the values:
```cpp
auto to_confidence = [](float a, float b) 
{
    assert(a >= 0 and a <= 1 and b >= 0 and b <= 1);
    return abs(a-b);
};
```
Because our classification output is always in `[0, 1]`, the higher the distance between the two probabilities, the more certain we can be that we did not make a classification error. 

We summarize our results as such:

| egg | ~p(chicken) | ~p(duck) | classification | confidence
|-----|-------------|----------|----------------|-----------
| 0   | 0.667       | 0.332    | chicken        | 0.33 (medium)
| 1   | 0.5         | 0.499    | chicken        | 0.001 (very low)
| 2   | 0.09        | 0.90     | duck           | 0.84 (high)

## 3 Neural Networks

While the Bayes Classifier is useful and can lead to quite decent results with very little computational cost, some classification processes require a more flexible approach. Since the 1970s, the state-of-the-art way of solving classification problems in a semi-supervised way has been *neural networks*. This tutorial will not cover how they work in detail, instead we will learn just enough fundamentals to understand how to use `crisp::NeuralNetwork` and how to interpret its results.

### 3.1 Architecture

A neural network has `l` layers. Each layer `L_i` has a number of *neurons* `#L_i`. 
+ `L_0` is called the *input layer*. This layer has the same number of neurons as the feature vector has rows: `#L_0 = n`
+ `L_l` is called the *output layer*. This layer as the same number of neurons as the number of possible classes: `#L_l = m`
+ `L_i` for `i` in `{1, 2, ..., l-1}` are called *hidden layers*

Where `n` the number of features, `m` the number of classes (c.f. [Section 1](#11-features--class-membership)). 

We call the number of layers and the number of neurons in each layer a network's *architecture*. We define the architecture in `crisp` using the template arguments of `crisp::NeuralNetwork`:

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

A network's architecture governs its structure. For the same problem, different architectures can result in vastly different results. Furthermore, the computational complexity (in terms of runtime performance) will of course increase with the total number of neurons. In praxis choosing the correct architecture is somewhat of an art, while heuristics exist in the literature it is often important to experiment.

### 3.2 Weights and Biases

Each neuron has a number of *weights* and one *bias*. The number of weights is equal to the number of neurons in the previous layer, for example if we're in layer `L_2`, all our neurons will have `#L_1` weights and a single bias. The weights and biases govern a neuron's output, thus (along with architecture) they completely define a network's performance in terms of classification.

## 3.3 Creating a Network

We can construct a fresh, untrained network like so:

```cpp
#include <classification/fully_connected_neural_network.hpp>

// in main.cpp
auto nn = crisp::NeuralNetwork<2, 2, 2>();
```

This initializes all weights to `1 + eps` and all biases to `0 + eps`, where `eps ∈  [-0.25, +0.25]` (random, uniformly distributed). <br>

Once our network is trained, we will want to export it to the disk to avoid having to retrain it every time the application launches. We can convert a network into a string like so:

```cpp
auto nn = crisp::NeuralNetwork<2, 2, 2, 2>();
auto exported = nn.as_string();
// save exported to file
```

We can then simply save the string to a file or otherwise store it. At a later point, we can then load the neural network from the disk using:

```cpp
auto nn = crisp::NeuralNetwork<2, 2, 2, 2>();

auto file = std::ifstream(/*...*/ + "/crisp/docs/feature_classification/chicken_or_duck_nn_2_4_4_2.txt");
std::stringstream buffer;
buffer << file.rdbuf();
std::string str = buffer.str();
nn.from_string(str);
``` 
Here we're opening a file stream, moving its content into a buffer stringstream and then create a plain string from said stringstream. The neural network can then parse the string it generated on a previous occasion and load the weights and biases. 
This is somewhat clumsy owing to C++s file interface so feel free to save the string generated using `as_string()` as you see fit.<br>
Note that for loading to be successful, the exported network and the network we're trying to import have to have identical architectures. Trying to import the exported string of a network with a different architecture will trigger an exception. It is therefore good practice to state the network's architecture in the filename (as in the above example).

## 3.4 Training the Network

Recall our egg example from [section 1](#11-features--class-membership):

```cpp
auto classes = Eigen::Matrix<2, 12>();
classes << 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // is chicken
           0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1; // is duck

auto features = Eigen::Matrix<2, 12>();
features << .52, .58, .67, .62, .59, .65, .61, .63, .69, .60, .71, .75, // weight
            .31, .34, .32, .37, .32, .29, .36, .35, .39, .38, .37, .34; // yolk 
```

We will use this data to train our network. Unlike the bayes classifier, neural networks will have to complete many training cycles. Each cycle of going through the entire training data set once is called an *epoch*.

We can conduct a single epoch like so:
```cpp
auto nn = NeuralNetwork<2, 4, 4, 2>(/*...*/);

nn.train(features, classes);
```

Neural Networks work in a way that is guaranteed to converge towards a set of optimal class boundaries (functions that divide the feature space into the given classes) with enough epochs. Here, "optimal" means, that we're minimizing the *mean square error*. We can directly compute the error of the current network state like so:

```cpp
nn.compute_mean_squared_error(feature, classes);
```
Here, we are not computing the error between `feature` and `classes`, rather, the neural network generates a classification matrix from `feature`, then measures the error between this result and `classes`.

 The errors for the first few epochs with our egg data and the architecture from the above example:

| epoch | maximum MSE 
|-------|---------
| 1     | 2.42762
| 2     | 2.08903
| 3     | 0.778278
| 4     | 0.675251


We see that the error is already decreasing rapidly. Depending on the architecture and problem, the mean square error may actually increase at the beginning, but then slowly stabilize and go back down as more epochs are completed.

If we want to only train for a certain number of epochs, we use:

```cpp
for (size_t n_epochs = 0; n_epochs < 10000; ++n_epochs)
   nn.train(features, classes);
```
If we have no idea how many epochs are needed to get to an acceptable classification error, we can instead use `NeuralNetwork::train_until`. It has 3 arguments: the feature matrix, the classification matrix and a mean square error threshold. If the network observes the current classification error to be stable and below that threshold, the function exits and returns the number of epochs it took:

```cpp
auto n_epochs = nn.train_until(features, classes, 0.15);
std::cout << n_epochs << std::endl;
```

The number of epochs is only useful for monitoring and can often be discarded. For our egg training data, it took `1059191` many epochs to get below the threshold of `0.15`. This is quite a lot, one parameter that governs the convergence behavior is called the *learning constant* (often called `μ` or `α`). This is a value in `[0, 1]` that we can optionally hand to the network on construction or by setting it later using `set_learning_constant`:

```cpp
float alpha = 0.001;
auto nn = NeuralNetwork<2, 4, 4, 2>(alpha);

// or
nn.set_learning_constant(alpha);
```

Usually values will be between `0.0001` and `0.1`, however just like the network's architecture, finding a good learning constant will require a lot of experimentation. For badly chosen values, the network can even begin to diverge, meaning the error will increase.

Now that our network is trained, we should export it, so we don't have to do another `1059191` epochs. The weights and biases for the above example with `alpha = 0.001` were:

```
1 0 7.30575 -1.03484 2.03974 -5.31141 5.80577 1.88958 6.59601 4.74493 -0.485592 1.42384 1.80752 2.70784 5.16637 3.31374 4.56878 -0.904903 1.88335 1.32096 1.80904 0.728481 5.68054 -0.569291 2.96311 1.78811 2.837 -0.384504 1.49958 6.98229 0.633096 1.17761 0.2897 -1.09701 -1.95467 3.00161 3.54201 -0.634866 -1.76444 -2.18135 1.18332 -0.43307 1.34111 0.527556 
```

The first number will always be `1`, this is the single and only weight of the input layer. It is always followed by a `0` which is the bias of the input layer. All following number correspond to the weights and biases of the subsequent layers.

## 3.5 Using the Network for Classification

Now that our network is well-trained and the mean squared error is low, we should compute its *classification error*. Unlike the MSE, the classification error is defined as the percentage of false-positive or false negatives when classifying a data set for which the actual correct classifications are known.<br>
We first load the network into memory using the string we generated earlier, then we classify the entire feature matrix from the training set:

```cpp
auto nn = NeuralNetwork<2, 4, 4, 2>();
nn.from_string(/*...*/);
std::cout << classes << std::endl;
std::cout << nn.identify(features) << std::endl;
```

```cpp
   0.99638    0.993424    0.988063    0.856026    0.998413    0.999656   -0.227162    0.343421 0.000525594   -0.426658 -0.00993148 -0.00718939
-0.0112014    0.434531    0.715863    0.272235   -0.189113   0.0802148    0.891286    0.900159    0.924364    0.830156    0.925139    0.924666
```

Like with the bayes classifier, each of the rows correspond to one class (chicken or duck), each column corresponds to one sample (egg). We classify an egg as chicken if the value in row 0 is higher, as a duck if the value in row 1 is higher. 

We get the following result, where `E(x)` is the expected value of `x` as designated in the training sets classification matrix `M`

| sample | Chicken       | E(Chicken) | Duck        | E(Duck)    | correct 
|--------|---------------|------------|-------------|------------|---------|
| 0      |  0.99638      |  1         | -0.0112014  |  0         |yes
| 1      |  0.993424     |  1         |  0.434531   |  0         |yes
| 2      |  0.988063     |  1         |  0.715863   |  0         |yes
| 3      |  0.856026     |  1         |  0.272235   |  0         |yes
| 4      |  0.998413     |  1         | -0.189113   |  0         |yes
| 5      |  0.999656     |  1         |  0.0802148  |  0         |yes
| 6      | -0.227162     |  0         |  0.891286   |  1         |yes
| 7      |  0.343421     |  0         |  0.900159   |  1         |yes
| 8      |  0.000525594  |  0         |  0.924364   |  1         |yes
| 9      | -0.426658     |  0         |  0.830156   |  1         |yes
| 10     | -0.00993148   |  0         |  0.925139   |  1         |yes
| 11     | -0.00718939   |  0         |  0.924666   |  1         |yes

We see that the neural network was able to classify all samples correctly. The least confident sample from this population was egg `2`, while the most confidently classified was egg `5`, where confidence is defined as in [section 2](#21-training-the-bayes-classifier).

Now that we know our neural network works decently well, we can use it to identify the three unknown eggs:

```cpp
auto to_identify = Eigen::MatrixXf(2, 3);
to_identify << .51, .60, .70,   // weight
               .29, .28, .38;   // yolk ratio
               
auto results = nn.identify(to_identify);
```

```cpp
//   0          1         2
 -0.021921   0.8932    -0.0119139    // chicken
  0.926603   0.918492   0.925525     // duck
```

Just like with bayes classifier, eggs `0` and `2` were identified confidently. Egg `1` was previously undetermined as the bayes classifier assigned both options the same score, our neural network however leans more towards duck, if only by a difference of `0.02`. This result is still not very confident, but it's at least possible to choose one or the other.

## 4. Convolutional Neural Networks

(this feature is not yet implemented)

---

[[<< Back to Index]](../index.md)






