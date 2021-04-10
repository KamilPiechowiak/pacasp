#ifndef NEIGHBORHOOH_HPP
#define NEIGHBORHOOH_HPP

#include"general/general.hpp"

class Neighborhood {
public:
    virtual int get_size() =0;
    virtual bool apply(int state, vector<int> &ord) =0;
    int next_random(vector<int> &ord);
};

#endif