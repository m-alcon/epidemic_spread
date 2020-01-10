#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

typedef vector<int> Vector;
typedef vector<Vector> Matrix;

class Node {
    public:
        int weight;
        bool infected, new_infected, recovered;
        Node() : weight(0) {};
        Node (const int &weight_) :
            weight(weight_) {}
        void infect();
        void recover();
};

typedef vector<Node> NVector;

class Network {    
    private:
        int n, k, l;
        float delta, gamma;
        random_device device;
        mt19937 generator;
        // uniform_real_distribution<float> real_distr(0,1);
        //uniform_int_distribution<uint> stub_distr(0, n-1);
        // uniform_int_distribution<uint> nodes_distr(0, c.nodes() - 1);
        NVector nodes;
        Matrix adjacency;
    public:
        Network (const int &n_, const int &k_, const int &l_, const float &delta_, const float &gamma_);
        void write(ostream &out) const;
        void update_infecteds();
};