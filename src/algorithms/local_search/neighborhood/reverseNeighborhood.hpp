#ifndef REVERSE_NEIGHBORHOOH_HPP
#define REVERSE_NEIGHBORHOOH_HPP

#include"algorithms/local_search/neighborhood/neighborhood.hpp"

class ReverseNeighborhood : public virtual Neighborhood {
private:
    int n;
public:
    ReverseNeighborhood(int n);
    int get_size();
    bool apply(int state, vector<int> &ord);
};

#endif