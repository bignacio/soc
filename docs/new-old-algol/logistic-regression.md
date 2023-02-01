---
layout: minimal
title: Logistic Regression
parent:  New Old Algorithms
---

# Logistic Regression

First on the list is a popular regression technique called Logistic Regression. I could try describe what it is in detail but it's [hard to beat wikipedia](https://en.wikipedia.org/wiki/Logistic_regression){:target="_blank"}. So I won't even try.

It uses a function that follows the logistic curve, a common S-shaped curve known as sigmoid curve.

Logistic regression is frequently used in binary classifiers, estimating the probability of something to be either 1 or 0. We give it  a bunch of data and known outputs for each sample (1 or 0) and it will try to build a curve that fits that patterns for us

We'll see next how we can implement a logistic regression algorithm that we can train and use it to make predictions.


## Training

The process for training our regression model is really where all the fun is but before we start talking about that, let's talk (very briefly) about some of the concepts.

The training data is an array of items that have the data that form the curve we want to fit to. Each entry in the array is called a **sample**.

Each item has one or more attributes - called **features** - which is also represented as a vector of values for each feature.

Finally, each feature has a label and since we're working on a binary classifier, the only possible labels are `0` or `1`.

As a silly example, let's say we're training a model to predict whether the result of a function is even or odd (I don't know why anyone in their right mind would want to do that, but it's the best I could think for now).

Say that our function is `f(a,b) = a + b + 1` and we use `1` to say the result is ven and 0 for odd. We can say we want our model to learn the function `f()` as stated.

|a|b|is even|
|-|-|-------|
|1|2|1|
|2|2|0|
|3|3|0|
|3|8|1|


Viewing this as raw inputs, our training data would look like this:
```c++
// these are our features, each line representing a sample
{
    {1, 2},
    {2, 2},
    {3, 3},
    {3, 8}
}

// and these would be the labels
{1, 0, 0, 1}
```

note that each sample can have as many features as we want but all need to have the same features. Similarly, the number of labels needs to match the number of samples.


**What on earth is a model anyway?**

The word model is one of those that became part of everyone's vocabulary in the past few years and either come across as something very simple or so exoteric that we might as well not even think about it.

Well in this case we're in luck, logistic regression models are fairly simple to understand.

Everything are vectors and matrices and the performing of algebraic operations on them.

After the training process, what we get is a vector of weights (real values or floating point numbers for the coders out there) and the length of this vector is the same as the number of features.

Using our `f(a,b) = a + b + 1` function, I like to **re-think** of it as `f(a,b) = a*w_a + b*w_b + 1` where `w_a` and `w_b` are the weights in that vector that represents our model.

Finally, these are the steps our code (yes, there's code in this article!!) will follow:

1. initialize our model weights with very small values. They will be updated during training
1. initialize a gradient vector with all zeros. This vector will be used to update the weights in the direction that best fits the data.
1. for each sample calculate the [similarity](https://en.wikipedia.org/wiki/Cosine_similarity){:target="_blank"} between the weights and the sample
This is done by calculating the dot product of the model vector and the feature vector for the sample.
1. then we take the dot product and calculate a probability using our friendly sigmoid function (the activation function) discussed above.
1. now, for each feature, we update its gradient with the contribution of the current sample given its label. Samples where the label is zero will make the gradient smaller.
1. lastly we update the model weights using the gradient adjusted by a learning rate.
1. repeat steps 3 to 6 a few times in order to optimize the model further. The number of times is also called _epoch_ and is one of the things that has to be chosen properly to calibrate the model.

Note that these are the most basic and minimum steps to train a model like this. In reality, for any practical real world application more refinement would be required.

But let's run with this and break down the new concepts in those steps.

Let's start with those gradients I talked about, more specifically, [gradient descent](https://en.wikipedia.org/wiki/Gradient_descent){:target="_blank"} (isn't Wikipedia great?).
The training process is basically trying to find weights that minimize the difference between the sample and the predicted probability for that sample and the gradient descent gives us the minimum local for that sample.

What we're actually doing is going in the negative direction of the steepest increase of the difference. It's worth noting that usually a cost function is applied in this step and the gradient descent is used to minimize that cost.
For our purposes the similarity between the vectors should be enough. To minimize the cost function, the parameters need to be updated in the opposite direction, also referred to as going in the negative gradient direction.

The dot (inner) product between the sample and the weights gives us the similarity between the actual value (the sample) and what the weights are telling us.
This value is then "activated" through a sigmoid function, which gives us the probability of being correct. The higher the probability, the smaller the changes in gradient making the weight more relevant.

Lastly, the learning rate allows us to control how much the model is updated for each step. If too high, we may overshoot and be too far from the optimal weights. If too low, training may require several repetitions (higher epochs) and take a long time to converge.

Turning all this into code, we have:
```c++
// inputData isa 2D matrix where each row represents a sample and each column represents a feature.
// labels is a vector of binary labels, where 1 represents belonging to a certain class and 0 represents belonging to another class.
// learningRate a scalar that determines the step size at which the model weights are updated in each iteration.
// numOfEpochs the number of times the gradient descent algorithm will be run on the data.
vector<double> logisticRegression(vector<vector<double>> inputData, vector<double> labels, double learningRate, int numOfEpochs) {
    int numOfSamples = inputData.size();
    int numOfFeatures = inputData[0].size(); /
    vector<double> modelWeights(numOfFeatures);

    for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
        modelWeights[featureIndex] = 0.1;
    }

    for (int epochIndex = 0; epochIndex < numOfEpochs; epochIndex++) {
        gradientDescent(modelWeights, inputData, labels, learningRate);
    }
    return modelWeights;
}

// modelWeights is a vector of doubles representing the current model weights.
// inputData is a matrix of doubles representing the input data. Each row corresponds to a sample and each column corresponds to a feature.
// labels is vector of doubles representing the binary class labels of the samples.
// learningRate a scalar that determines the step size at which the model weights are updated in each iteration.
void gradientDescent(vector<double>& modelWeights, vector<vector<double>> inputData, vector<double> labels, double learningRate) {
    int numOfSamples = inputData.size();
    int numOfFeatures = inputData[0].size();
    vector<double> gradient(numOfFeatures);

    for (int sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
        double dotProduct = 0;
        for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
            dotProduct += modelWeights[featureIndex] * inputData[sampleIndex][featureIndex];
        }
         double probability = sigmoid(dotProduct);
        for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
            gradient[featureIndex] += (labels[sampleIndex] - probability) * inputData[sampleIndex][featureIndex];
        }
    }
    for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
        modelWeights[featureIndex] += learningRate * gradient[featureIndex] / numOfSamples;
    }
}
```

You can find the [complete source code here](https://github.com/bignacio/soc/blob/main/docs/new-old-algol/code/logistic_regression.cpp){:target="_blank"}.


The result of `logisticRegression` is our model, which we can use to make predictions.

In the full code linked above, you will notice that the input data is not normalized before training and that will impact training. In practical application, the vectors in the training data would ideally be normalized prior to training.

## Prediction

The prediction mechanism uses the same concepts we saw so far. To make a prediction, we take the feature vector calculate the similarity to the model weights and take the probability of that being a positive case (a class with value 1, in our case).

```c++
double predict(vector<double> model, vector<double> sample) {
    int numOfFeatures = sample.size();
    double dotProduct = 0;

    for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
        dotProduct += model[featureIndex] * sample[featureIndex];
    }

    return sigmoid(dotProduct);
}
```

Where `model` is the vector resulting from the training we did prior and sample is the vector representing the features of the sample we're trying to predict.

Note that the result will be a value between 0.0 and 1.0 so to determine whether the prediction is positive (class 1), we probably need to set a threshold that represent the minimum probability we need to be considered positive.


## From binary to many

The logistic classifier only let's us to work with 1 positive class at a time so if we want to find which fruit our sample represents (instead of, say finding out if a fruit is a cherry or not), we can use a technique called [multiclass classification](https://en.wikipedia.org/wiki/Multiclass_classification){:target="_blank"}.

In multiclass classification, we train one model for each class - in our case, moving the label 1 to each sample accordingly - and then perform one prediction step for each model.
The prediction that with the highest probability is the one that would represent the expected class.