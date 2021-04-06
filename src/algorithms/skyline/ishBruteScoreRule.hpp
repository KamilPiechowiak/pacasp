#ifndef ISH_BRUTE_SCORE_RULE_HPP
#define ISH_BRUTE_SCORE_RULE_HPP

#include"general/general.hpp"
#include"general/rectangle.hpp"
#include"algorithms/skyline/skylineSegment.hpp"
#include"algorithms/skyline/scoreRule.hpp"
#include"algorithms/utils/segmentTree.hpp"

struct ISHBruteScoreRule : public ScoreRule {
private:
    vector<Rectangle> rect;
    vector<bool> visited;
    vector<int> ord;
public:
    void init(vector<Rectangle> &rect, vector<int> &ord);
    RectanglePlacement next(SkylineSegment segment);
};

#endif