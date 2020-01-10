#include "network.h"

Node::Node() {
    initialize();
}

void Node::initialize() {
    infected = new_infected = recovered = false;
}

void Node::infect() {
    new_infected = true;
    recovered = false;
}

void Node::recover() {
    recovered = true;
}

Connection create_connection(Node* node, const int &weight) {
    Connection c;
    c.node = node;
    c.weight = weight;
    return c;
}

size_t Network::size() const {
    return static_cast<size_t> (n);
}

Network::Network(const int &n_, const int &k, const int &l) :
    n(n_), generator(device()) {        
        Matrix stubs(n, Vector (k, 1));
        cerr << "generate stubs" << endl;
        // Generate k stubs for each node and distribute weights
        uniform_int_distribution<int> k_distr (0, k-1);
        for (size_t i = 0; i < n; ++i) {
            float total = 0;
            vector<float> aux (k, 0);
            for (size_t j = 0; j < k; ++j) {
                uniform_real_distribution<float> distribution(0, 1);
                aux[j] = distribution(generator);
                total += aux[j];
            }
            float sum = 0;
            for (size_t j = 0; j < k; ++j) {
                stubs[i][j] += floor((l-k)*aux[j]/total);
                sum += stubs[i][j];
            }
            for (sum; sum < l; ++sum)
                ++stubs[i][k_distr(generator)];
        }
        for (size_t i = 0; i < n; ++i) {
            int sum = 0;
            for (size_t j = 0; j < k; ++j) 
                sum += stubs[i][j];
        }

        cerr << "generate all_stubs" << endl;
        // Generate a vector with all the stubs
        Matrix all_stubs (n*k, Vector (2, 0));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < k; ++j) {
                all_stubs[k*i + j][0] = i;
                all_stubs[k*i + j][1] = j;
            }
        }
        cerr << "shuffle" << endl;
        shuffle(all_stubs.begin(), all_stubs.end(), generator);
        
        cerr << "check pairs" << endl;
        // Check that all stubs are not paired with other of the same node
        uniform_int_distribution<int> bool_distr (0, 1);
        uniform_int_distribution<int> nk_distr (0, (n*k)-1);
        Matrix adjacency_matrix (n, Vector (n, 0));
        for (size_t i = 0; i < n*k; i += 2) {
            int id1 = all_stubs[i][0];
            int id2 = all_stubs[i+1][0];
            while (id1 == id2) {
                int i_shuffle = nk_distr(generator);
                int id3 = all_stubs[i_shuffle][0];
                int id4 = all_stubs[i_shuffle + (i_shuffle%2 ? -1 : 1)][0];
                if (id1 == id3 or id1 == id4)
                    continue;
                if (bool_distr(generator)) {
                    id1 = id3;
                    swap(all_stubs[i], all_stubs[i_shuffle]);
                } else {
                    id2 = id3;
                    swap(all_stubs[i+1], all_stubs[i_shuffle]);
                }
            }
        }
        cerr << "create adjacency matrix" << endl;

        // Create adjacency matrix
        for (size_t i = 0; i < n*k; i += 2) {
            int u_id = all_stubs[i][0];
            int v_id = all_stubs[i+1][0];
            int u_weight = stubs[u_id][all_stubs[i][1]];
            int v_weight = stubs[v_id][all_stubs[i+1][1]];
            int diff = abs(u_weight - v_weight);
            if (diff <= 1 and diff <= 0.1*min(u_weight, v_weight)) {
                if (u_weight != v_weight) {
                    if (bool_distr(generator))
                        u_weight = v_weight;
                    else
                        v_weight = u_weight;
                }
                adjacency_matrix[u_id][v_id] += v_weight;
                adjacency_matrix[v_id][u_id] += u_weight;
            }
        }

        adjacency = CMatrix(n);
        nodes = NVector(n, Node());

        cerr << "move" << endl;
        // Move adjacency matrix to adjacency list
        for (size_t i = 0; i < n; ++i) {
            nodes[i].id = i;
            for (size_t j = 0; j < n; ++j)
                if (adjacency_matrix[i][j] > 0)
                    adjacency[i].push_back(create_connection(&nodes[i], adjacency_matrix[i][j]));
        }
}

Node* Network::get_node (const int &id) {
    return &nodes[id];
}

CVector* Network::get_neighbors (const int &id) {
    return &adjacency[id];
}

void Network::initialize(const int &n_infected) {
    for (size_t i = 0; i < n; ++i) {
        nodes[i].initialize();
    }
    uniform_int_distribution<int> distribution (0, n-1);
    for (int i = 0; i < n_infected; ++i) {
        int id = distribution(generator);
        while (nodes[id].infected)
            id = distribution(generator);
        nodes[id].infected = true;
    }
}


void Network::update_infecteds() {
    for (size_t i = 0; i < n; ++i) {
        if (nodes[i].new_infected) {
            nodes[i].new_infected = false;
            nodes[i].infected = true;
        } else if (nodes[i].recovered) {
            nodes[i].infected = false;
        }
    }
}

void Network::write(ostream &out) const {
    for (size_t i = 0; i < n; ++i) {
        out << adjacency[i].size();
        for (size_t j = 0; j < adjacency[i].size(); ++j) {
            int id = adjacency[i][j].node->id;
            out << " " << id << " " << nodes[id].infected;
        }
        out << endl;
    }
}