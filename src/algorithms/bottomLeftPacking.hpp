#ifndef BOTTOM_LEFT_PACKING_HPP
#define BOTTOM_LEFT_PACKING_HPP

#include"algorithms/skyline/scoreRule.hpp"
#include"algorithms/packing_algorithm.hpp"

class BottomLeftPacking : public PackingAlgorithm {
public:
    BottomLeftPacking(ll width, vector<Rectangle> rect, string name);
    ll run(vector<int> &ord);
};

#endif