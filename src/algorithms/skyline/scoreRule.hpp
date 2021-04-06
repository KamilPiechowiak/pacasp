#ifndef SCORE_RULE_HPP
#define SCORE_RULE_HPP

#include"general/general.hpp"
#include"general/rectangle.hpp"
#include"algorithms/skyline/skylineSegment.hpp"

struct RectanglePlacement {
    int id;
    bool left;
    bool any;
    bool all_placed;
    RectanglePlacement(int id, bool left, bool any, bool all_placed) : id(id), left(left), any(any), all_placed(all_placed) {}
};

struct ScoreRule {
public:
    virtual void init(vector<Rectangle> &rect, vector<int> &ord) =0;
    virtual RectanglePlacement next(SkylineSegment segment) =0;
};

#endif