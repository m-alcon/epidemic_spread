#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

typedef vector<int> Vector;
typedef vector<Vector> Matrix;

class Node {
    public:
        int id; 
        bool infected, new_infected, recovered;
        Node();
        void initialize();
        void infect();
        void recover();
};

struct Connection {
    Node* node;
    int weight;
};

typedef vector<Node> NVector;
typedef vector<Connection> CVector;
typedef vector<CVector> CMatrix;

class Network {    
    private:
        int n;
        random_device device;
        mt19937 generator;
        NVector nodes;
        CMatrix adjacency;
    public:
        Network (const int &n_, const int &k, const int &l);
        void write (ostream &out) const;
        size_t size () const;
        Node* get_node (const int &id);
        CVector* get_neighbors (const int &id);
        void initialize(const int &n_infected);
        void update_infecteds ();
};