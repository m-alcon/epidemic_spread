#include "network.h"

int main() {
    Network net (10000, 20, 50);
    // net.write(cout);
    simulation_SIS(net, 0.1, 0.06, 20, 1000);
    simulation_SIR(net, 0.1, 0.06, 20, 1000);
    return 0;
}

void simulation_SIS (Network &net, const float &beta, const float &gamma, const int &initials, const int &tmax) {
    random_device device;
    mt19937 generator(device);
    uniform_real_distribution<float> distribution (0.0f,1.0f);
    net.initialize(initials);
    int infecteds = initials;
    for (int t = 0; t < tmax; ++t) {
        for (size_t i = 0; i < net.size(); ++i) {
            Node* node = net.get_node(i);
            if (node->infected) {
                if (distribution(generator) <= gamma) {
                    node->recover();
                    --infecteds;
                }
            }
            else {
                CVector* neighbors = net.get_neighbors(i);
                int infected_interactions = 0;
                for (size_t j = 0; j < neighbors->size(); ++j){
                    if ((*neighbors)[j].node->infected)
                        infected_interactions += (*neighbors)[j].weight;
                }
                if (distribution(generator) <= 1-pow(1-beta, infected_interactions)) {
                    node->infect();
                    ++infecteds;
                }
            }
        }
        cout << t << " " << infecteds << endl;
        net.update_infecteds();
    }
}

void simulation_SIR (Network &net, const float &beta, const float &gamma, const int &initials, const int &tmax) {
    random_device device;
    mt19937 generator(device);
    uniform_real_distribution<float> distribution (0.0f,1.0f);
    net.initialize(initials);
    int infecteds = initials;
    int recovereds = 0;
    for (int t = 0; t < tmax; ++t) {
        for (size_t i = 0; i < net.size(); ++i) {
            Node* node = net.get_node(i);
            if (node->infected) {
                if (distribution(generator) <= gamma) {
                    node->recover();
                    --infecteds;
                    ++recovereds;
                }
            }
            else if (not node->recovered){
                CVector* neighbors = net.get_neighbors(i);
                int infected_interactions = 0;
                for (size_t j = 0; j < neighbors->size(); ++j){
                    if ((*neighbors)[j].node->infected)
                        infected_interactions += (*neighbors)[j].weight;
                }
                if (distribution(generator) <= 1-pow(1-beta, infected_interactions)) {
                    node->infect();
                    ++infecteds;
                }
            }
        }
        cout << t << " " << infecteds << " " << recovereds << endl;
        net.update_infecteds();
    }
}
