#ifndef HILL_CLIMBER_HPP
#define HILL_CLIMBER_HPP

#include"algorithms/recorder.hpp"
#include"algorithms/local_search/neighborhood/neighborhood.hpp"
#include"algorithms/packing_algorithm.hpp"

class HillClimber {
private:
    Recorder &recorder;
    bool greedy;
    Neighborhood *neighborhood;
public:
    HillClimber(Recorder &recorder, bool greedy=false, Neighborhood *neighborhood=NULL);
    ll run(PackingAlgorithm *packing_algorithm, vector<int> &ord);
};

#endif