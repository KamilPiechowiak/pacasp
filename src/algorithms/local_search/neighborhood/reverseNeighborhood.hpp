#ifndef REVERSE_NEIGHBORHOOH_HPP
#define REVERSE_NEIGHBORHOOH_HPP

#include"algorithms/local_search/neighborhood/neighborhood.hpp"

class ReverseNeighborhood : public virtual Neighborhood {
private:
    int n;
public:
    ReverseNeighborhood(int n);
    ll get_size();
    bool apply(ll state, vector<int> &ord);
};

#endif