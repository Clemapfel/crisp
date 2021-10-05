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

We learned how to extract part of an image into a [segment](../segmentation/segmentation.md), then compute unique descriptors that describe the [region's](../feature_extraction) shape and texture. Now, it is finally time to use these descriptors to solve one of the most common image processing applications: classifying a region.

To classify an object means to divide the population it is from into a number of classes `{C_0, C_1, ..., C_m-1}`, then assign a label to any one specific object that represents what class it belongs to. For the sake of simplicity we assume that on object can only belong to exactly one class. While this assumption is not necessary for any of `crisp`s deep learning features, it will make things easier to follow later.

### 1.1 Features & Class Membership

For one specific region, class membership is given as a `m*1` matrix `M`:

| class index i | `M(i, 0)`
|-------------|--------
| 0           | {0, 1}
| 1           | {0, 1}
| ...         | ...
| m-1         | {0, 1}

Where if `M(4, 0) == 1` then the object is classified into class `C4`. Because an object can only belong to one class at a time, `M.sum() == 1` (as all values except for the single `1` are `0`).

In order to be able to classify an object we need to describe it somehow, we do this using a `n*1` matrix called a *feature vector*.  Each row of this matrix `N` corresponds to one feature:

| feature index | `N(i, 0)`
|---------------|--------
| 0             | [-1, 1]
| 1             | [-1, 1]
| ...           | ...
| n-1           | [-1, 1]

When classifying multiple objects, all objects have a feature vector of the same size and each row corresponds to the same number of feature in the same order. All that changes is the actual values of the features. As we saw in the table above, it is necessary to normalize the value of the feature into the range `[-1, 1]`. In the literature features are often normalized into `[0, 1]` but beause this is a subset of of `[-1, 1]` either approach works in `crisp`.

A *training dataset* in `crisp` is a set of feature vectors and their corresponding, desired classifications. In practice this set are two matrices: 

+ `FeatureMatrix_t` is a `n*x` matrix where `n` the number of features and `x` the number of samples in the population. Each row is one feature, each column is one sample. All it's values should be normalized into [-1, 1]
  
+`ClassificationMatrix_t` is a `m*x` matrix where m the number of classes and `x` the number of sample. All elements of a row should be either 1 or 0

The number of columns `x` has to be the same for both of these. If one object has it's feature vector as the column of the feature matrix at position i, the the iths column of the classification matrix holds the desired classification.

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

Here we have a set of 12 eggs from both chickens and ducks. Each egg sample has 3 data point: wether it was from a chicken or duck, it's weight and it's yolk-to-white-ratio.
We want to create a classifier can decide wether an new egg we hand it is a chicken or duck egg.

Before we can use `crisp` for classification, however, we need to arrange the data in a way that `crisp` understands.

We have `m = 2` classes and `n = 2` features. We are trying to predict the "class" propery of each egg so we create a matrix with 2 rows, each column of the matrix has one `1` depending on wether the sample was from a chicken or duck.

```cpp
auto classes = Eigen::Matrix<2, 12>();
// sample: 0  1  2  3  4  5  6  7  8  9  10 11
classes << 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // is chicken
           0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1; // is duck
```

Each egg sample gets a 2x1 feature vector that is 1 in the first row if it is from a chicken or 1 in the second row if it is from a duck.

We enter the feature values in a similar way. Each column corresponds to one sample (egg), we arbirarily decide that weight will be in row 0, yolk-ratio in row 1. Recall that features need to be normalized so we divide both by 100:
```cpp
auto features = Eigen::Matrix<2, 12>();
// sample:    0    1    2    3    4    5    6    7    8    9   10   11
features << .52, .58, .67, .62, .59, .65, .61, .63, .69, .60, .71, .75, // weight
            .31, .34, .32, .37, .32, .29, .36, .35, .39, .38, .37, .34; // yok ratio
```
We will call the first matrix is *class membership matrix* and the second *feature matrix*. 

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

The classifier works by estimating the type of distribution, mean and variance from a given population. Then, when we hand it a sample (egg) to identify, it calculates the propabilities that our egg is from a specific class. For us it would calculated 
```
p( x | chicken)  // is our current egg x from a chicken
p( x | duck)     // is our current egg x from a duck
```

The return values of the classifier are not actual probabilities (they can be outside the range of [0, 1]) however the higher the actual probablity of class-membership is, the higher a score the classifier will assign to it.

### 2.1 Training the Bayes Classifier

To estimate the values it needs for classification we offer it our training set from before like so:

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

That is all, the classifier is now primed to identify any egg we hand it. Unlike neural networks which will be discussed later in this section, the bayes classifier achieves optimality (under it's somewhat restricive assumptions) with just one training cycle. 

### 2.2 Identification using the Bayes Classifier

Let's say we just came back home and found 3 eggs around our imaginative farm. We measure their size and content:

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

We get a bunch of numbers, let's try to understand what they mean. First each row corresponds to the same row in the data set so we can label the rows by the egg index:

```cpp
// 0       1        2
0.667373  0.500235  0.0936609
0.332627  0.499765  0.906339
```

The numbers are scores representing the relative probability that the sample in column `i` is from class `j` where `j` is the row index. In our training dataset we had the chickens as `j = 0` and ducks as `j = 1`, so:

```cpp
// 0       1       2
0.667373  0.500235  0.0936609    // p(egg | chicken)
0.332627  0.499765  0.906339     // p(egg | duck)
```

We decide which egg came from which bird by simply comparing the probabilities:

+ For egg 0: 66& chance it is a chicken, 33% it is a duck
+ For egg 1: 50:50 duck or chicken
+ For egg 2: 90% chance it is a duck, 10% a chicken

Technically we would assign egg 0 and 1 as chichen, 3 as duck however the result for egg 1 is so ambigous that it is basically a cointoss. To quantify this we assign each result a confidence score using, for example, the following simple function:

```cpp
auto to_confidence = [](float a, float b) 
{
    assert(a >= 0 and a <= 1 and b >= 0 and b <= 1);
    return abs(a-b);
};
```

Because our classification output is always in [0, 1], the higher the distance between the two probabilities the more certain we can be. We summarize our results as such:

+ Egg 0: Chicken (0.33 (medium) confidence)
+ Egg 1: Chicken (0.001 (very low) confidence)
+ Egg 2: Duck    (0.84 (high) confidence)

## 3 Neural Networks

While the Bayes Classifier is useful and can lead to quite decent results, some classification processes require a more flexible approach. Since the 1970s, the state-of-the-art way of solving classification problems in a semi-supervised way has been by using neural networks. This tutorial will not cover how they work in detail, instead we will learn just enough fundamentals to understand how to use `crisp::NeuralNetwork` and how to interpret it's results.

### 3.1 Architecture

A neural network has `l` layers. Each layer `L_i` has a number of *neurons* `#L_i`. 
+ `L_0` is called the *input layer*. This layer has the same number of neurons as the feature vector has rows: `#L_0 = n`
+ `L_l` is called the *output layer*. This layer as the same number of neurons as the number of possible classes: `#L_l = m`
+ `L_i` for `i` in `{1, 2, ..., l-1}` are called *hidden layers*

Where `n` the number of features, `m` the number of classes (c.f. [Section 1](#11-features--class-membership)). 

We call the number of layers and the number of neurons in each layers a networks *architecture*. We define the architecture in crisp using the template arguments:

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

A networks architecture governs it's structure. For the same problem, different architectures can result in vastly different results. Furthermore the computationaly complexity (in terms of runtime performance) will of course increase with the total number of neurons

### 3.2 Weights and Biases

Each neuron has a number of *weights* and one *bias*. The number of weights is equal to the number of neurons in the last layer. For example, if we're in layer `L_2`, all our neurons will have `#L_1` weights and a single bias. The weights and biases govern a neurons output, thus (along with architecture) they completely define a networks performance in terms of classification.

## 3.3 Creating a Network

We can construct a fresh untrained network like so:

```cpp
#include <classification/fully_connected_neural_network.hpp>

// in main.cpp
auto nn = crisp::NeuralNetwork<2, 2, 2>();
```

This initializes all weights to `1 + eps` and all biases to `0 + eps`, where `eps`  [-0.25, +0.25]` (random, uniformly distributed). <br>

Once our network is trained we will want to export it to the disk to avoid having to retrain it everytime the application launches. We can convert a network into a string that can then be saved like so:

```cpp
auto nn = crisp::NeuralNetwork<2, 2, 2, 2>();
auto exported = nn.as_string();
// save exported to file
```

At a later point, we can then load the neural network from the disk:

```cpp
auto nn = crisp::NeuralNetwork<2, 2, 2, 2>();

auto file = std::ifstream(/*...*/ + "/crisp/docs/feature_classification/xor_nn.bin");
std::stringstream buffer;
buffer << file.rdbuf();
std::string str = buffer.str();
nn.from_string(str);
``` 
Here we're opening a file stream, moving it's content into a buffer stringstream and then creating a plain string from said stringstream. The neural network can then parse then string it generated on a previous occasion and load the weights. 
This is somewhat clumsy owing to C++s file interface so it is recommended that you save the string generated using `as_string()` as you see fit.<br>
Note that for loading to be succesfull, the exported network and the network we're trying to import have to have identical architectures. Trying to import the exported string of a network with a different architecture will trigger an exception.

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

We will use this data to train our network. Unlike bayes classifier, neural networks will have to complete many training cycles. Each cycle where we go through the entire training data once is called an *epoch*.

We can conduct a single epoch like so:
```cpp
auto nn = NeuralNetwork</*...*/>(/*...*/);

nn.train(features, classes);
```

Neural Networks work in a way that is garuanteed to converge towards a set of optimial class boundaries (functions that divide the feature space into the given classes) with enough epochs. "optimal" here means, that we're minimizing the *mean square error*. We can directly compute the error of the current network state like so:

```cpp
nn.compute_mean_squared_error(feature, classes);
```
Here it is not computing the error between `feature` and `classes`, rather it uses `feature` as an argument, computes it's own identification using the features and then compares that output to the desired output `classes`. The errors for the first few epochs with our egg data:

```cpp
| epoch | LSME 
|-------|---------
| 1     | 2.42762
| 2     | 2.08903
| 3     | 0.778278
| 4     | 0.675251
```

We see it's already decreasing. Depending on the archtecture and problem the mean square error may actuall increase at the beginning but then slowly stabilize and go back down. 

If we want to only train a certain number of epochs, we use:

```cpp
size_t n_epochs = 10000;
while (--n_epochs > 0)
   nn.train(features, classes);
```

Instead of trying to guess how many epochs should be enough we can also train until the mean sqaured error converges below a certain threshold:

```cpp
auto n_epochs = nn.train_until(features, classes, 0.15);
std::cout << n_epochs << std::endl;
```

This function returns the number of epochs it took, however this result can be discarded. For our egg set it returned `1059191` many epochs which is quite a lot.

It's always important to also export the networks as a string so we can reuse it later on. The weights and bias for a `NeuralNetwork<2, 4, 4, 2>` trained as shown above were:

```
0 7.30575 -1.03484 2.03974 -5.31141 5.80577 1.88958 6.59601 4.74493 -0.485592 1.42384 1.80752 2.70784 5.16637 3.31374 4.56878 -0.904903 1.88335 1.32096 1.80904 0.728481 5.68054 -0.569291 2.96311 1.78811 2.837 -0.384504 1.49958 6.98229 0.633096 1.17761 0.2897 -1.09701 -1.95467 3.00161 3.54201 -0.634866 -1.76444 -2.18135 1.18332 -0.43307 1.34111 0.527556 
```

## 3.5 Using the Network for Classification

Now that our network is well trained we should first compute it's *classification error*. Unlike the mean square error, the classification error is defined as the percentage of false-positive or false negatives when classifying the original feature matrix we used to train the network.<br>
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

Each of the rows correspond to one class, chicken or duck, each row corresponds to one sample (egg). We classify an egg as chicken if the value in row 0 is higher or a ducks if the value in row 1 is higher. We get the following result, where E(x) is the expected value of x as designated in the training set classification matrix `M`

| sample | Chicken       | E(Chicken) | Duck        | E(Duck)    |
|--------|---------------|------------|-------------|------------|
| 0      |  0.99638      |  1         | -0.0112014  |  0         |
| 1      |  0.993424     |  1         |  0.434531   |  0         |
| 2      |  0.988063     |  1         |  0.715863   |  0         |
| 3      |  0.856026     |  1         |  0.272235   |  0         |
| 4      |  0.998413     |  1         | -0.189113   |  0         |
| 5      |  0.999656     |  1         |  0.0802148  |  0         |
| 6      | -0.227162     |  0         |  0.891286   |  1         |
| 7      |  0.343421     |  0         |  0.900159   |  1         |
| 8      |  0.000525594  |  0         |  0.924364   |  1         |
| 9      | -0.426658     |  0         |  0.830156   |  1         |
| 10     | -0.00993148   |  0         |  0.925139   |  1         |
| 11     | -0.00718939   |  0         |  0.924666   |  1         |

We see that the neural network was able to classify all samples correctly. The least confident sample from this population was egg 2, while the most confidently classified was egg 5.

Now that we know our neural network can handle our dataset, we can use it to identify the three unknown eggs:

```cpp
auto to_identify = Eigen::MatrixXf(2, 3);
to_identify << .51, .60, .70,   // weight
               .29, .28, .38;   // yolk ratio
               
auto results = nn.identify(to_identify);
```

```cpp
//   0          1         2
 -0.021921     0.8932 -0.0119139    // chicken
  0.926603   0.918492   0.925525    // duck
```

Just like the bayes classifier eggs 0 and 2 were identified confidently. Egg 1 was previously undetermined as the bayes classifier assigned both options the same score, our neural network however leans more towards duck, if only by 0.02. This result is still not very confident but it is at least possible to choose one or the other.

## 4. Convolutional Neural Networks

(this feature is not yet implemented)

---

[[<< Back to Index]](../index.md)






