#ifndef NEIGHBORHOOH_HPP
#define NEIGHBORHOOH_HPP

#include"general/general.hpp"

class Neighborhood {
public:
    virtual ll get_size() =0;
    virtual bool apply(ll state, vector<int> &ord) =0;
    ll next_random(vector<int> &ord);
};

#endif