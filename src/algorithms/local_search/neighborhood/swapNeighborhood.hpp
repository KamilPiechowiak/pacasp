#ifndef SWAP_NEIGHBORHOOH_HPP
#define SWAP_NEIGHBORHOOH_HPP

#include"algorithms/local_search/neighborhood/neighborhood.hpp"

class SwapNeighborhood : public virtual Neighborhood {
private:
    int n;
public:
    SwapNeighborhood(int n);
    ll get_size();
    bool apply(ll state, vector<int> &ord);
};

#endif