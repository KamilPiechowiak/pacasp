#include"algorithms/skyline/burkeScoreRule.hpp"

void BurkeScoreRule::init(vector<Rectangle> &rect, vector<int> &ord) {
    for(int i=0; i < SIZE(rect); i++) {
        s.insert(make_tuple(rect[i].width, rect[i].height, i));
    }
}

RectanglePlacement BurkeScoreRule::next(SkylineSegment segment) {
    ll w = segment.right-segment.left+1;
    auto it = s.upper_bound(make_tuple(w+1, -INF, 0));
    if(s.size() == 0) {
        return RectanglePlacement(0, false, false, true);
    }
    if(it==s.begin()) {
        return RectanglePlacement(0, false, false, false);
    } else {
        it--;
        int id = get<2>(*it);
        s.erase(it);
        return RectanglePlacement(id, true, true, false);
    }
}
