#ifndef TABU_SEARCH_HPP
#define TABU_SEARCH_HPP

#include"algorithms/recorder.hpp"
#include"algorithms/local_search/neighborhood/neighborhood.hpp"
#include"algorithms/packing_algorithm.hpp"

class TabuSearch {
private:
    int tabu_length;
    int number_of_neighbors;
    Recorder &recorder;
    Neighborhood *neighborhood;
    bool hash_from_order;
    deque<ll> tabu_list;
    unordered_map<ll, int> tabu_map;
    ll get_hash(vector<int> &ord);
    void add_to_tabu(ll hash);
    void remove_from_tabu();
public:
    TabuSearch(int tabu_length, int number_of_neighbors, Recorder &recorder, bool hash_from_order=false, Neighborhood *neighborhood=NULL);
    ll run(PackingAlgorithm *packing_algorithm, vector<int> &ord);
};

#endif