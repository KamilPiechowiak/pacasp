#ifndef MULTI_START_LOCAL_SEARCH_HPP
#define MULTI_START_LOCAL_SEARCH_HPP

#include"algorithms/recorder.hpp"
#include"algorithms/local_search/neighborhood/neighborhood.hpp"
#include"algorithms/packing_algorithm.hpp"

class IteratedLocalSearch {
private:
    int number_of_neighbors;
    int perturbation_inversed_intensity; //the bigger the less intense the perturbation
    Recorder &recorder;
    Neighborhood *neighborhood;
public:
    IteratedLocalSearch(int number_of_neighbors, int perturbation_inversed_intensity, Recorder &recorder, Neighborhood *neighborhood=NULL);
    ll run(PackingAlgorithm *packing_algorithm, vector<int> &ord);
};

#endif