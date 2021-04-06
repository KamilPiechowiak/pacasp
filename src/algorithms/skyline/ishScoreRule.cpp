#include"algorithms/skyline/ishScoreRule.hpp"

void ISHScoreRule::init(vector<Rectangle> &rect, vector<int> &ord) {
    this->rect = rect;
    this->ord = ord;
    int n = SIZE(rect);
    set_w_h_id.clear();
    set_w_id.clear();
    tree_h_w_id.init(n);
    vector_h_w_id.clear();
    place_mapping_h_w_id.resize(n);
    tree_w_id.init(n);
    vector_w_id.clear();
    place_mapping_w_id.resize(n);
    for(int i=0; i < n; i++) {
        Rectangle &r = rect[ord[i]];
        set_w_h_id.insert({r.width, r.height, i});
        set_w_id.insert({r.width, i});
        vector_h_w_id.push_back({r.height, r.width, i});
        vector_w_id.push_back({r.width, i});
    }
    sort(vector_h_w_id.begin(), vector_h_w_id.end());
    sort(vector_w_id.begin(), vector_w_id.end());
    for(int i=0; i < n; i++) {
        tree_h_w_id.update(i, get<2>(vector_h_w_id[i]));
        place_mapping_h_w_id[get<2>(vector_h_w_id[i])] = i;
        tree_w_id.update(i, vector_w_id[i].second);
        place_mapping_w_id[vector_w_id[i].second] = i;
    }
}

RectanglePlacement ISHScoreRule::next(SkylineSegment segment) {
    if(set_w_h_id.size() == 0) {
        return RectanglePlacement(0, false, false, true);
    }
    RectanglePlacement res = RectanglePlacement(-1, false, false, false);
    int score = 0;
    //score 3 or 2
    ll w = segment.right-segment.left+1;
    auto it = set_w_h_id.upper_bound({w, segment.left_height-segment.height, -1});
    if(it != set_w_h_id.end() && get<0>(*it) == w && get<1>(*it) == segment.left_height-segment.height) {
        res = RectanglePlacement(get<2>(*it), true, true, false);
        score = 2;
    }
    if(segment.left_height != segment.right_height) {
        auto it = set_w_h_id.upper_bound({w, segment.right_height-segment.height, -1});
        if(it != set_w_h_id.end() && get<0>(*it) == w && get<1>(*it) == segment.right_height-segment.height) {
            if(!res.any || res.id > get<2>(*it)) {
                res = RectanglePlacement(get<2>(*it), true, true, false);
                score = 2;
            }
        }
    }
    if(!res.any) { //score 1
        auto it = set_w_id.upper_bound({w, -1});
        if(it != set_w_id.end() && it->first == w) {
            res = RectanglePlacement(it->second, true, true, false);
            score = 1;
        }
        pair<ll, bool> heights[] = {{segment.left_height-segment.height, true}, {segment.right_height-segment.height, false}};
        for(auto height_left : heights) {
            auto height = height_left.first;
            tuple<ll, ll, int> left_value = {height, 0, -1};
            auto left = upper_bound(vector_h_w_id.begin(), vector_h_w_id.end(), left_value);
            tuple<ll, ll, int> right_value = {height, w+1, -1};
            auto right = upper_bound(vector_h_w_id.begin(), vector_h_w_id.end(), right_value);
            int left_pos = left-vector_h_w_id.begin();
            int right_pos = (right-vector_h_w_id.begin())-1;
            if(left_pos <= right_pos) {
                int id = tree_h_w_id.get_min(left_pos, right_pos);
                if(id != INF && (!res.any || id < res.id)) {
                    res = RectanglePlacement(id, height_left.second, true, false);
                    score = 1;
                }
            }
        }
    }
    if(!res.any) { //score 0
        pair<ll, int> right_value = {w+1, -1};
        auto right = upper_bound(vector_w_id.begin(), vector_w_id.end(), right_value);
        int right_pos = (right-vector_w_id.begin())-1;
        if(right_pos >= 0) {
            int id = tree_w_id.get_min(0, right_pos);
            if(id != INF) {
                res = RectanglePlacement(id, true, true, false);
            }
        }
    }

    if(res.any) {
        int id = res.id;
        Rectangle &r = rect[ord[id]];
        set_w_h_id.erase({r.width, r.height, id});
        set_w_id.erase({r.width, id});
        tree_h_w_id.update(place_mapping_h_w_id[id], INF);
        tree_w_id.update(place_mapping_w_id[id], INF);
        res.id = ord[res.id];
        // fprintf(stderr, "ish score: %d %d\n", score, res.id);
    }
    return res;
}
