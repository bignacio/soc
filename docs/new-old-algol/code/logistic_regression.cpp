#include <iostream>
#include <cmath>
#include <vector>
#include <random>


using namespace std;

// How much we assume weight and volume can vary for the same fruit
constexpr const double variance = 0.02;

// Fruit colour. We'll use a simple map to a number
constexpr const double colourRed = 1;
constexpr const double colourBlack = 2; // whatever colour black grapes are

/**
 * Sigmoid function.
 *
 * The sigmoid function is a mathematical function that maps any real-valued number to a value between 0 and 1.
 * It is commonly used in machine learning and statistics as a probability function.
 * In logistic regression, it is used to model the probability of a binary outcome given a set of input features.
 *
 * @param x - the input value to the sigmoid function.
 * @return - a value between 0 and 1.
 */
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

/**
 * Gradient Descent Algorithm for Logistic Regression.
 *
 * The Gradient Descent Algorithm is an optimization method that iteratively adjusts the model weights in order to minimize the difference between the predicted class labels and the true class labels.
 * The algorithm uses the gradient of the cost function to determine the direction in which the weights should be updated.
 * In the case of Logistic Regression, the cost function is the Log-Loss (or Cross-Entropy Loss) function.
 *
 * @param modelWeights a vector of doubles representing the current model weights.
 * @param inputData a matrix of doubles representing the input data. Each row corresponds to a sample and each column corresponds to a feature.
 * @param labels a vector of doubles representing the binary class labels of the samples.
 * @param learningRate a double representing the learning rate, which determines the step size of the weight updates.
 *
 */
void gradientDescent(vector<double>& modelWeights, vector<vector<double>> inputData, vector<double> labels, double learningRate) {
    int numOfSamples = inputData.size();
    int numOfFeatures = inputData[0].size(); // Number of features in each sample of the input data. Assume all entries are of the s ame size
    vector<double> gradient(numOfFeatures);

    for (int sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
        double dotProduct = 0;
        for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
            dotProduct += modelWeights[featureIndex] * inputData[sampleIndex][featureIndex];
        }
        // Calculate the predicted probability of the sample belonging to class 1 using the sigmoid function.
        double probability = sigmoid(dotProduct);
        // Update the gradient vector with the contribution of the current sample.
        for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
            gradient[featureIndex] += (labels[sampleIndex] - probability) * inputData[sampleIndex][featureIndex];
        }
    }
    // Update the model weights with the average gradient, scaled by the learning rate.
    for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
        modelWeights[featureIndex] += learningRate * gradient[featureIndex] / numOfSamples;
    }
}


/**
 * Logistic Regression.
 *
 * Logistic Regression is a statistical method for analyzing a dataset in which there are one or more independent variables
 * that determine an outcome. The outcome is measured with a dichotomous variable (in which there are only two possible outcomes).
 * The goal of logistic regression is to find the best coefficients that minimize the difference between the predicted and actual values
 * of the dichotomous variable.
 *
 * @param inputData a 2D matrix where each row represents a sample and each column represents a feature.
 * @param labels a vector of binary labels, where 1 represents belonging to a certain class and 0 represents belonging to another class.
 * @param learningRate a scalar that determines the step size at which the model weights are updated in each iteration.
 * @param numOfEpochs the number of times the gradient descent algorithm will be run on the data.
 * @return a vector of model weights.
 */
vector<double> logisticRegression(vector<vector<double>> inputData, vector<double> labels, double learningRate, int numOfEpochs) {
    int numOfSamples = inputData.size();
    int numOfFeatures = inputData[0].size(); // Number of features in each sample of the input data. Assume all entries are of the s ame size
    vector<double> modelWeights(numOfFeatures);

    for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
        modelWeights[featureIndex] = 0.1;
    }

    for (int epochIndex = 0; epochIndex < numOfEpochs; epochIndex++) {
        gradientDescent(modelWeights, inputData, labels, learningRate);
    }
    return modelWeights;
}

/**
 * Predict the probability of a sample belonging to a certain class.
 *
 * This function uses the logistic regression model weights and a sample's features to predict the probability
 * of the sample belonging to a certain class (represented by 1).
 *
 * @param model a vector of model weights obtained from running logistic regression on the data.
 * @param sample a vector of features for a single sample.
 * @return a scalar representing the predicted probability of the sample belonging to a certain class.
 */
double predict(vector<double> model, vector<double> sample) {
    int numOfFeatures = sample.size();
    double dotProduct = 0;

    for (int featureIndex = 0; featureIndex < numOfFeatures; featureIndex++) {
        dotProduct += model[featureIndex] * sample[featureIndex];
    }

    return sigmoid(dotProduct);
}

vector<double> makeFruit(double startWeight, double startVolume, double colour, double variance) {
    const double wdiff = startWeight * variance;
    const double vdiff = startVolume * variance;

    std::random_device rd;
    std::mt19937 rng;

    rng.seed(rd());

    std::uniform_real_distribution<double> wdist(startWeight-wdiff, startWeight+wdiff);
    std::uniform_real_distribution<double> vdist(startVolume-vdiff, startVolume+vdiff);

    vector<double> result{};

    double weightValue =  wdist(rng);
    double volmueValue =  vdist(rng);

    result.push_back(weightValue);
    result.push_back(volmueValue);
    result.push_back(colour);

    return result;
}

vector<double> makeApple() {
    const double appleWeight = 150.0;
    const double appleVol = 450.0;

    return makeFruit(appleWeight, appleVol, colourRed, variance);
}

vector<double> makeCherry() {
    const double cherryWeight = 5.0;
    const double cherryVol = 15.0;
    return makeFruit(cherryWeight, cherryVol, colourRed, variance);
}

vector<double> makeGrape() {
    const double grapeWeight = 13.0;
    const double grapeVol = 24.0;

    return makeFruit(grapeWeight, grapeVol, colourBlack, variance);
}

void populateAllFruit(vector<vector<double>>& allFruits, vector<double>& labels, int count) {

    for(int i = 0 ; i < count ; i++) {
        auto cherry = makeCherry();
        auto grape = makeGrape();
        auto apple = makeApple();


        // Stagger each fruit not to bias our training data too much
        // setting us up for multiclass classification
        allFruits.push_back(cherry);
        allFruits.push_back(grape);
        allFruits.push_back(apple);

        // We also need to label the training data with what's cherry
        labels.push_back(1.0);
        labels.push_back(0.0);
        labels.push_back(0.0);
    }
}

vector<double> train() {
    // the training data layout is
    // [weight in grams, volume in cm^3, colour]

    // our input is 3 types of truit: cherry, black grapes and apples
    // let's make some fruit
    const int count  = 1000;
    vector<vector<double>> allFruits{};
    vector<double> labels{};
    populateAllFruit(allFruits, labels, count);

    const double learningRate = 0.05;
    const int epochs = 200;

    // the training data should have been normalized before usage
    return logisticRegression(allFruits, labels, learningRate, epochs);
}

void testModel(const vector<double> model) {
    // Consider anything above 50% probability as a correct prediction
    // This should also impact the percentage of false positives
    const double threshold = 0.5;

    const int numTests = 1000;
    int aboveThreshold{0};

    // Let's check how many predictions our model can get right
    for(int i = 0 ; i < numTests ; i++) {
        vector<double> sample = makeCherry();
        if(predict(model, sample) > threshold) {
            aboveThreshold++;
        }
    }
    cout << "Percentage of correct predictions: " << ((double)aboveThreshold/(double)numTests)*100.0 << "%" << endl;

    // Now let's check for false positives
    aboveThreshold = 0;
    for(int i = 0 ; i < numTests ; i++) {
        for(auto sample: {
                    makeApple(), makeGrape()
                }) {
            if(predict(model, sample) > threshold) {
                aboveThreshold++;
            }
        }
    }

    cout << "Percentage false positives: " << ((double)aboveThreshold/(double)(numTests*2.0))*100.0 << "%" << endl;
}

int main() {
    auto model = train();
    testModel(model);

    return 0;
}
