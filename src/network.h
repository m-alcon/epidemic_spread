#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

typedef vector<int> Vector;
typedef vector<Vector> Matrix;

class Node {
    public:
        bool infected, new_infected, recovered;
        Node() : infected(false), new_infected(false), recovered(false) {}
        void infect();
        void recover();
};

struct Connection {
    int id, weight;
};

typedef vector<Node> NVector;
typedef vector<Connection> CVector;
typedef vector<CVector> CMatrix;

class Network {    
    private:
        int n, k, l;
        float delta, gamma;
        random_device device;
        mt19937 generator;
        NVector nodes;
        CMatrix adjacency;
    public:
        Network (const int &n_, const int &k_, const int &l_, const float &delta_, const float &gamma_);
        void write(ostream &out) const;
        Node* get_node(const int &id) const;
        CVector* get_neighbors(const int &id) const;

        void update_infecteds();
};