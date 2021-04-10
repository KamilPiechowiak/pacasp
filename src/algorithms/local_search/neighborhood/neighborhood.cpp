#include"algorithms/local_search/neighborhood/neighborhood.hpp"

int Neighborhood::next_random(vector<int> &ord) {
    int neighborhood_size = this->get_size();
    while(true) {
        int state = ri(0, neighborhood_size-1);
        if(this->apply(state, ord)) {
            return state;
        }
    }
}