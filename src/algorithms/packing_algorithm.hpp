#ifndef PACKING_ALGORITHM_HPP
#define PACKING_ALGORITHM_HPP

#include"general/general.hpp"
#include"general/rectangle.hpp"

class PackingAlgorithm {
protected:
    ll w;
    vector<Rectangle> rect;
    vector<pll> placement;
    string name;
public:
    PackingAlgorithm(ll width, vector<Rectangle> rect, string name);
    virtual ll run(vector<int> &ord) =0;
    ll run_and_save(string name, vector<int> &ord);
};

#endif