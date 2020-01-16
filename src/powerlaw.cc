#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
using namespace std;

typedef vector<int> Vector;

float powerlaw_distribution(const float &lambda, const int &min, const int max, mt19937 &generator) {
    uniform_real_distribution<float> distribution (0, 1);
    return pow(pow(max, 1-lambda) - (pow(max, 1-lambda) - pow(min, 1-lambda))*distribution(generator), 1/(1-lambda));
}

int main(int argc, char* argv[]) {
    random_device device;
    mt19937 generator(device());
    int max = 50;
    float lambda = 0.89;
    if (argc > 2) {
        max = atoi(argv[1]);
        lambda = atof(argv[2]);
    }
    for (int i = 0; i < 10000; ++i) {
        cout << powerlaw_distribution(lambda, 1, max, generator) << " ";
    }
}