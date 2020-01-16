#include "network.h"
#include <sstream>

#define N_NETS 20
#define REPETITIONS 100

void simulation_SIS (Network &net, ofstream &out, const float &beta, const float &gamma, const int &initials, const int &tmax) {
    random_device device;
    mt19937 generator(device());
    uniform_real_distribution<float> distribution (0.0f,1.0f);
    net.initialize(initials);
    int infecteds = initials;
    out << "t infected" << endl;
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

void simulation_SIR (Network &net, ofstream &out, const float &beta, const float &gamma, const int &initials, const int &tmax) {
    random_device device;
    mt19937 generator(device());
    uniform_real_distribution<float> distribution (0.0f,1.0f);
    net.initialize(initials);
    int infecteds = initials;
    int recovereds = 0;
    out << "t infected recovered" << endl;
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
    DistrInfo info_k, info_l;
    if (argc > 1) {
        cout << "Parameters: ";
        info_k.distr = atoi(argv[1]);
        int pos_l = 0;
        switch (info_k.distr) {
            case distr::Poisson:
                info_k.iparam = atoi(argv[2]);
                pos_l = 3;
                cout << "<k> = " << info_k.iparam;
                break;
            case distr::PowerLaw:
                info_k.fparam = atof(argv[2]);
                info_k.iparam = atoi(argv[3]);
                pos_l = 4;
                cout << "lambda_k = " << info_k.fparam << " max_k = " << info_k.iparam << " ";
                break;
            default:
                return -1;
                break;
        }
        info_l.distr = atoi(argv[pos_l]);
        switch (info_l.distr) {
            case distr::Poisson:
                info_l.iparam = atoi(argv[pos_l+1]);
                cout << " <l> = " << info_l.iparam << endl;
                break;
            case distr::PowerLaw:
                info_l.fparam = atof(argv[pos_l+1]);
                info_l.iparam = atoi(argv[pos_l+2]);
                cout << " lambda_l = " << info_l.fparam << " max_l = " << info_l.iparam << endl;
                break;
            case distr::Delta:
                info_l.iparam = atof(argv[pos_l+1]);
                cout << " delta_l = " << info_l.iparam << endl;
                break;
            default:
                return -1;
                break;
        }
    } else {
        info_k.distr = distr::Poisson;
        info_k.iparam = 4;
        info_l.distr = distr::Poisson;
        info_l.iparam = 8;
    }
    for (int i = 0; i < N_NETS; ++i) {
        cout << "Generate network " << i << endl;
        Network net (10000, info_k, info_l);
        for (int j = 0; j < REPETITIONS; ++j) {
            ostringstream sir_ss, sis_ss;
            sir_ss << "data/sir/" << info_k.distr << info_l.distr << "/" << i << "_" << j << ".dat"; 
            sis_ss << "data/sis/" << info_k.distr << info_l.distr << "/" << i << "_" << j << ".dat"; 
            ofstream sir (sir_ss.str());
            ofstream sis (sis_ss.str());
            cout << "  Simulation " << j << " SIS for network " << i << endl;
            simulation_SIS(net, sis, 0.01, 0.004, 20, 1000);
            cout << "  Simulation " << j << " SIR for network " << i << endl;
            simulation_SIR(net, sir, 0.01, 0.004, 20, 1000);
        }
    }
    

    return 0;
}
