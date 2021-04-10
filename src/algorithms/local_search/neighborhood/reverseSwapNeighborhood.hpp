#ifndef REVERSE_SWAP_NEIGHBORHOOH_HPP
#define REVERSE_SWAP_NEIGHBORHOOH_HPP

#include"algorithms/local_search/neighborhood/neighborhood.hpp"
#include"algorithms/local_search/neighborhood/reverseNeighborhood.hpp"
#include"algorithms/local_search/neighborhood/swapNeighborhood.hpp"

class ReverseSwapNeighborhood : public virtual Neighborhood {
private:
    int n;
    int swap_size;
    ReverseNeighborhood reverse_neighborhood;
    SwapNeighborhood swap_neighborhood;
public:
    ReverseSwapNeighborhood(int n);
    int get_size();
    bool apply(int state, vector<int> &ord);
};

#endif