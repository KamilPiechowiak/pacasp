#include"algorithms/local_search/neighborhood/neighborhood.hpp"

ll Neighborhood::next_random(vector<int> &ord) {
    ll neighborhood_size = this->get_size();
    while(true) {
        ll state = rll(0, neighborhood_size-1);
        if(this->apply(state, ord)) {
            return state;
        }
    }
}