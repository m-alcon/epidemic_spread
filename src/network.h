#include <iostream>
#include <vector>
#include <random>
using namespace std;

typedef vector<int> Vector;
typedef vector<Vector> Matrix;

class Network {    
    private:
        int n, k, l;
        float delta, gamma;
        random_device device;
        mt19937 generator;
        // uniform_real_distribution<float> real_distr(0,1);
        //uniform_int_distribution<uint> stub_distr(0, n-1);
        // uniform_int_distribution<uint> nodes_distr(0, c.nodes() - 1);
        Matrix adjacency;
    public:
        Network (const int &n_, const int &k_, const int &l_, const float &delta_, const float &gamma_);
};