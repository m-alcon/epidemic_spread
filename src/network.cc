#include "network.h"

Network::Network(const int &n_, const int &k_, const int &l_, const float &delta_, const float &gamma_) :
    n(n_), k(k_), l(l_), delta(delta_), gamma(gamma_), generator(device()) {
        adjacency = NMatrix(n);
        
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
            Node u, v;
            u.id = all_stubs[i][0];
            v.id = all_stubs[i+1][0];
            u.weight = stubs[u.id][all_stubs[i][1]];
            v.weight = stubs[v.id][all_stubs[i+1][1]];
            int diff = abs(u.weight - v.weight);
            if (diff <= 1 and diff <= 0.1*min(u.weight, v.weight)) {
                if (u.weight != v.weight) {
                    if (bool_distr(generator))
                        u.weight = v.weight;
                    else
                        v.weight = u.weight;
                }
                adjacency_matrix[u.id][v.id] += v.weight;
                adjacency_matrix[v.id][u.id] += u.weight;
            }
        }

        cerr << "move" << endl;
        // Move adjacency matrix to adjacency list
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j)
                if (adjacency_matrix[i][j] > 0)
                    adjacency[i].push_back(Node(j, adjacency_matrix[i][j]));
        }

        cerr << "print" << endl;
        // print
        for (size_t i = 0; i < n; ++i) {
            cout << adjacency[i].size();
            for (size_t j = 0; j < adjacency[i].size(); ++j)
                cout << " " << adjacency[i][j].id << " " << adjacency[i][j].weight;
            cout << endl;
        }
}