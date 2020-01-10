#include "network.h"

int main() {
    Network net (10000, 20, 50);
    net.write(cout);
    return 0;
}

void simulation_SIS (Network &net, const float &beta, const float &gamma, const int &initials, const int &tmax) {
    random_device device;
    mt19937 generator(device);
    uniform_real_distribution<float> distribution (0.0f,1.0f);
    net.initialize(initials);
    for (int t = 0; t < tmax; ++t) {
        for (size_t i = 0; i < net.size(); ++i) {
            Node* node = net.get_node(i);
            if (node->infected) {
                if (distribution(generator) <= gamma) 
                    node->recover();
            }
            else {
                CVector* neighbors = net.get_neighbors(i);
                int infected_interactions = 0;
                for (size_t j = 0; j < neighbors->size(); ++j){
                    if ((*neighbors)[j].node->infected)
                        infected_interactions += (*neighbors)[j].weight;
                }
                if (distribution(generator) <= 1-pow(1-beta, infected_interactions))
                    node->infect();
            }
        }
        net.update_infecteds();
    }
    
}
