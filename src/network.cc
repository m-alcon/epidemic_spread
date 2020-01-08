#include "network.h"

Network::Network(const int &n_, const int &k_, const int &l_, const float &delta_, const float &gamma_) :
    n(n_), k(k_), l(l_), delta(delta_), gamma(gamma_), generator(device()) {
        adjacency = Matrix(n);
        
        vector<vector<float>> stubs(n, vector<float>(k, 0));

        for (int i = 0; i < n; ++i) {
            float aux_l = l;
            for (int j = 0; j < k-1; ++j) {
                uniform_real_distribution<float> distribution(0, aux_l-(k-j));
                stubs[i][j] = distribution(generator);
                aux_l -= stubs[i][j];
            }
            stubs[i][k-1] = aux_l;   
        }

        vector<vector<bool>> used_stub (n, vector<bool> (k, false));
        for (int i = 0; i < n; ++i) {
            uniform_int_distribution<float> node_distr(i, n-1);
            for (int j = 0; j < k; ++j) {
                uniform_int_distribution<float> stub_distr(j, k-1);
                int node = node_distr(generator);
                int stub = stub_distr(generator);
                float diff = abs(stubs[node][stub] - stubs[i][j]);
                while(diff > 1 or diff > 0.1*min(stubs[node][stub], stubs[i][j])) {
                    node = node_distr(generator);
                    stub = stub_distr(generator);
                    diff = abs(stubs[node][stub] - stubs[i][j]);
                }
                used_stub[node][stub] = true;
                used_stub[i][j] = true;
            }
        } 
}