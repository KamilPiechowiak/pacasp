#ifndef ISH_SCORE_RULE_HPP
#define ISH_SCORE_RULE_HPP

#include"general/general.hpp"
#include"general/rectangle.hpp"
#include"algorithms/skyline/skylineSegment.hpp"
#include"algorithms/skyline/scoreRule.hpp"
#include"algorithms/utils/segmentTree.hpp"

struct ISHScoreRule : public ScoreRule {
private:
    set<tuple<ll, ll, int>> set_w_h_id;
    set<pair<ll, int>> set_w_id;
    SegmentTree tree_h_w_id;
    vector<tuple<ll, ll, int>> vector_h_w_id;
    SegmentTree tree_w_id;
    vector<pair<ll, int>> vector_w_id;
    vector<int> place_mapping_h_w_id;
    vector<int> place_mapping_w_id;
    vector<Rectangle> rect;
    vector<int> ord;
public:
    void init(vector<Rectangle> &rect, vector<int> &ord);
    RectanglePlacement next(SkylineSegment segment);
};

#endif