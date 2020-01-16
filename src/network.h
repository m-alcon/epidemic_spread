#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
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

enum distr {Poisson, PowerLaw, Delta};

struct DistrInfo {
    int distr;
    float fparam;
    int iparam;
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
        Vector get_random_powerlaw(const float &lambda, const int &max);
        Vector get_random_powerlaw_vector(const float &lambda, const int &max, const Vector &min);
        Vector get_random_poisson(const int &k);
        Vector get_random_poisson_vector(const int &mean, const Vector &min);
    public:
        Network (const int &n_, const DistrInfo &k, const DistrInfo &l);
        void write (ostream &out) const;
        size_t size () const;
        Node* get_node (const int &id);
        CVector* get_neighbors (const int &id);
        void initialize(const int &n_infected);
        void update_infecteds ();
};