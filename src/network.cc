#include "network.h"

#define GET_DISTR_L get_random_poisson(l);
#define GET_DISTR_K get_random_poisson(k);
#define MAX_ZERO_ACCEPTED 2

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

Vector Network::get_random_poisson(const int &mean) {
    poisson_distribution<int> distribution(mean-1);
    Vector rvalues (n, 1);
    for (size_t i = 0; i < n; ++i) {
        rvalues[i] += distribution(generator);
    }
    return rvalues;
}

Vector Network::get_random_powerlaw() {
    ifstream distr_in("data/power_law.dat");
    Vector rvalues (n, 0);
    for (size_t i = 0; i < n; ++i) {
        distr_in >> rvalues[i];
    }
    return rvalues;
}

Network::Network(const int &n_, const int &global_k, const int &global_l) :
    n(n_), generator(device()) {        
        Matrix stubs(n);
        Vector k_values = get_random_poisson(global_k);
        Vector l_values = get_random_poisson(global_l);
        
        // Generate k stubs for each node and distribute weights
        int sum_k = 0;
        for (size_t i = 0; i < n; ++i) {
            int k = k_values[i];
            int l = l_values[i];
            sum_k += k;
            cout << k << endl;
            uniform_int_distribution<int> k_distr (0, k-1);
            float total = 0;
            vector<float> aux (k, 0);
            for (size_t j = 0; j < k; ++j) {
                uniform_real_distribution<float> distribution(0, 1);
                aux[j] = distribution(generator);
                total += aux[j];
            }
            float sum = 0;
            stubs[i] = Vector (k);
            for (size_t j = 0; j < k; ++j) {
                stubs[i][j] += floor((l-k)*aux[j]/total);
                sum += stubs[i][j];
            }
            for (sum; sum < l; ++sum)
                ++stubs[i][k_distr(generator)];
        }
        for (size_t i = 0; i < n; ++i) {
            int sum = 0;
            for (size_t j = 0; j < stubs[i].size(); ++j) 
                sum += stubs[i][j];
        }

        cout << "generate all stubs" << endl;

        // Generate a vector with all the stubs
        Matrix all_stubs (sum_k, Vector (2, 0));
        int pos = 0;
        for (size_t i = 0; i < n; ++i) {
            int k = stubs[i].size();
            for (size_t j = 0; j < k; ++j) {
                all_stubs[pos][0] = i;
                all_stubs[pos][1] = j;
                ++pos;
            }
        }
        Matrix rejected_stubs;
        Matrix adjacency_matrix (n, Vector (n, 0));
        int accepted = 0;
        int times_zero_accepted = 0;
        uniform_int_distribution<int> bool_distr (0, 1);
        while (times_zero_accepted > MAX_ZERO_ACCEPTED and rejected_stubs.size() > 1) {
            size_t n_stubs = all_stubs.size();
            uniform_int_distribution<int> nk_distr (0, all_stubs.size()-1);
            shuffle(all_stubs.begin(), all_stubs.end(), generator);
            
            // Check that all stubs are not paired with other of the same node
            cout << "check all stubs" << endl;
            for (size_t i = 0; i < n_stubs; i += 2) {
                if (i+1 >= n_stubs)
                    break;
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

            // Create adjacency matrix
            for (size_t i = 0; i < n_stubs; i += 2) {
                if (i+1 >= n_stubs) {
                    rejected_stubs.push_back(all_stubs[i]);
                    break;
                }
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
                    ++accepted;
                } else {
                    rejected_stubs.push_back(all_stubs[i]);
                    rejected_stubs.push_back(all_stubs[i+1]);
                }
            }
            times_zero_accepted = accepted > 0 ? 0 : times_zero_accepted+1;
            all_stubs = rejected_stubs;
            rejected_stubs = Matrix();
        }

        adjacency = CMatrix(n);
        nodes = NVector(n, Node());

        // Move adjacency matrix to adjacency list
        for (size_t i = 0; i < n; ++i) {
            nodes[i].id = i;
            for (size_t j = 0; j < n; ++j)
                if (adjacency_matrix[i][j] > 0)
                    adjacency[i].push_back(create_connection(&nodes[j], adjacency_matrix[i][j]));
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