#ifndef BURKE_SCORE_RULE_HPP
#define BURKE_SCORE_RULE_HPP

#include"general/general.hpp"
#include"general/rectangle.hpp"
#include"algorithms/skyline/skylineSegment.hpp"
#include"algorithms/skyline/scoreRule.hpp"

struct BurkeScoreRule : public ScoreRule {
private:
    set<tuple<ll, ll, int>> s; //width, height, id
public:
    void init(vector<Rectangle> &rect, vector<int> &ord);
    RectanglePlacement next(SkylineSegment segment);
};

#endif