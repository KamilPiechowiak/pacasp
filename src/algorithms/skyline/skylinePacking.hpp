#ifndef SKYLINE_PACKING_HPP
#define SKYLINE_PACKING_HPP

#include"algorithms/skyline/scoreRule.hpp"
#include"algorithms/packing_algorithm.hpp"

class SkylinePacking : public PackingAlgorithm {
    ScoreRule *scoreRule;
public:
    SkylinePacking(ll width, vector<Rectangle> rect, string name, ScoreRule *scoreRule);
    ll run(vector<int> &ord);
    void setScoreRule(ScoreRule *scoreRule);
};

#endif