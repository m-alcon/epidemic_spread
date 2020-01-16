#include "network.h"

void simulation_SIS (Network &net, const float &beta, const float &gamma, const int &initials, const int &tmax) {
    ofstream out ("data/info_sis.dat");
    random_device device;
    mt19937 generator(device());
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
        out << t << " " << infecteds << endl;
        net.update_infecteds();
    }
}

void simulation_SIR (Network &net, const float &beta, const float &gamma, const int &initials, const int &tmax) {
    ofstream out ("data/info_sir.dat");
    random_device device;
    mt19937 generator(device());
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
                    //cout << "prob: " << 1-pow(1-beta, infected_interactions) << " weights:" << infected_interactions << endl;
                    node->infect();
                    ++infecteds;
                }
            }
        }
        out << t << " " << infecteds << " " << recovereds << endl;
        net.update_infecteds();
    }
}

int main(int argc, char* argv[]) {
    int k, l;
    if (argc != 3) {
        k = 6;
        l = 12;
    }
    else {
        k = atoi(argv[1]);
        l = atoi(argv[2]);
    }
    cout << "Parameters: k = " << k << " l = " << l << endl;
    cout << "Generate network" << endl;
    Network net (10000, k, l);
    ofstream sis ("data/net_sis.dat");
    ofstream sir ("data/net_sir.dat");
    //cout << "SIS" << endl;
    // simulation_SIS(net, 0.01, 0.004, 20, 1000);
    // net.write(sis);
    cout << "SIR" << endl;
    simulation_SIR(net, 0.01, 0.004, 20, 1000);
    net.write(sir);

    return 0;
}
