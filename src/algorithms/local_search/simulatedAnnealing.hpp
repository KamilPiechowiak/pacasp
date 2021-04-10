#ifndef SIMULATED_ANNEALING_HPP
#define SIMULATED_ANNEALING_HPP

#include"algorithms/recorder.hpp"
#include"algorithms/local_search/neighborhood/neighborhood.hpp"
#include"algorithms/packing_algorithm.hpp"

class SimulatedAnnealing {
private:
    double multiplier;
    int max_number_of_accepted;
    int max_number_of_rejected;
    Recorder &recorder;
    Neighborhood *neighborhood;
public:
    SimulatedAnnealing(double multiplier, int max_number_of_accepted, int max_number_of_rejected, Recorder &recorder, Neighborhood *neighborhood=NULL);
    ll run(PackingAlgorithm *packing_algorithm, vector<int> &ord);
};

#endif