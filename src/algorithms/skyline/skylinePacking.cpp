#include"algorithms/skyline/skylinePacking.hpp"

SkylinePacking::SkylinePacking(ll width, vector<Rectangle> rect, string name, ScoreRule *scoreRule)
    : PackingAlgorithm(width, rect, name), scoreRule(scoreRule) {}

void SkylinePacking::setScoreRule(ScoreRule *scoreRule) {
    this->scoreRule = scoreRule;
}

ll SkylinePacking::run(vector<int> &ord) {
    scoreRule->init(rect, ord);
    // ISHScoreRule second_score_rule;
    // second_score_rule.init(rect, ord);
    list<SkylineSegment> skyline;
    skyline.push_back(SkylineSegment(0, w-1, 0));
    typedef tuple<ll, ll, list<SkylineSegment>::iterator> SegmentData;
    auto cmp = [](const SegmentData &a, const SegmentData &b) -> bool {
        return pll(get<0>(a), get<1>(a)) < pll(get<0>(b), get<1>(b));
    };
    set<SegmentData, function<bool (const SegmentData&, const SegmentData&)>> lowest_segment(cmp); //y, x, iterator
    lowest_segment.insert(make_tuple(0, 0, skyline.begin()));
    ll totalHeight = 0;
    // for(auto a : skyline) {
    //     fprintf(stderr, "%lld,%lld\n", a.left, a.right);
    // }
    int iter=0;
    while(true) {
        auto it = get<2>(*lowest_segment.begin());
        lowest_segment.erase(lowest_segment.begin());
        SkylineSegment selected_segment = *it;

        if(it != skyline.begin()) {
            it--;
            selected_segment.left_height = it->height;
            it++;
        }
        it++;
        if(it != skyline.end()) {
            selected_segment.right_height = it->height;
        }
        it--;
        auto rectangle_placement = scoreRule->next(selected_segment);
        // auto second_rectangle_placement = second_score_rule.next(selected_segment);
        // if(rectangle_placement.any) {
        //     assert(rectangle_placement.id == second_rectangle_placement.id);
        // }
        // if(rectangle_placement.any && rectangle_placement.id != second_rectangle_placement.id) {
        //     fprintf(stderr, "error at %d: %d %d\n", iter, rectangle_placement.id, second_rectangle_placement.id);
        //     fprintf(stderr, "%lld %lld\n", rect[rectangle_placement.id].width, rect[rectangle_placement.id].height);
        //     fprintf(stderr, "%lld %lld\n", rect[second_rectangle_placement.id].width, rect[second_rectangle_placement.id].height);
        //     fprintf(stderr, "segment: %lld %lld %lld\n\n", selected_segment.right-selected_segment.left+1, selected_segment.left_height-selected_segment.height, selected_segment.right_height-selected_segment.height);
        // }
        // fprintf(stderr, "%d %d %d\n", rectangle_placement.any, rectangle_placement.left, rectangle_placement.id);
        if(rectangle_placement.all_placed) {
            break;
        } else if(!rectangle_placement.any) {
            if(selected_segment.left_height == selected_segment.right_height) {
                it++;
                ll current_right = it->right;
                lowest_segment.erase(make_tuple(it->height, it->left, it));
                skyline.erase(it--);
                skyline.erase(it--);
                it->right = current_right;
            } else if(selected_segment.left_height < selected_segment.right_height) {
                ll current_right = it->right;
                skyline.erase(it--);
                it->right = current_right;
            } else {
                ll current_left = it->left;
                skyline.erase(it++);
                lowest_segment.erase(make_tuple(it->height, it->left, it));
                it->left = current_left;
                lowest_segment.insert(make_tuple(it->height, it->left, it));
            }
        } else {
            // fprintf(stderr, "%lld,%lld,%lld", it->left, it->right, it->height);
            // fprintf(stderr, "%lld <= %lld\n", rect[rectangle_placement.id].width, selected_segment.right-selected_segment.left+1);
            iter++;
            assert(rect[rectangle_placement.id].width <= selected_segment.right-selected_segment.left+1);
            Rectangle r = rect[rectangle_placement.id];
            if(rectangle_placement.left) {
                placement[r.id] = {it->left, it->height};
            } else {
                placement[r.id] = {it->right-r.width+1, it->height};
            }
            // fprintf(stderr, "\n%lld,%lld\n", r.height, r.width);
            totalHeight = max(totalHeight, r.height+it->height);
            if(r.width == it->right-it->left+1) {
                it->height+=r.height;
            } else if(rectangle_placement.left) {
                it->left+=r.width;
                lowest_segment.insert(make_tuple(it->height, it->left, it));
                it = skyline.insert(it, SkylineSegment(it->left-r.width, it->left-1, it->height+r.height));
            } else {
                it->right-=r.width;
                lowest_segment.insert(make_tuple(it->height, it->left, it));
                SkylineSegment new_segment = SkylineSegment(it->right+1, it->right+r.width, it->height+r.height);
                it++;
                it = skyline.insert(it, new_segment);
            }

            SkylineSegment &current_segment = *it;
            it++;
            if(it != skyline.end() && current_segment.height == it->height) {
                lowest_segment.erase(make_tuple(it->height, it->left, it));
                current_segment.right = it->right;
                skyline.erase(it--);
            } else {
                it--;
            }
            if(it != skyline.begin()) {
                it--;
                if(current_segment.height == it->height) {
                    it->right = current_segment.right;
                    it++;
                    skyline.erase(it);
                } else {
                    it++;
                    lowest_segment.insert(make_tuple(it->height, it->left, it));
                }
            } else {
                lowest_segment.insert(make_tuple(it->height, it->left, it));
            }
        }
        #if DEBUG
        assert(skyline.size() == lowest_segment.size());
        ll prev_x = -1, prev_height = -1;
        for(auto i = skyline.begin(); i != skyline.end(); i++) {
            // fprintf(stderr, "(%lld,%lld,%lld),", i->left,i->right,i->height);
            assert(prev_x+1 == i->left);
            prev_x = i->right;
            assert(prev_height != i->height);
            assert(lowest_segment.count({i->height, i->left, i}));
        }
        assert(prev_x+1 == w);
        // fprintf(stderr, "\n");
        #endif
    }
    // fprintf(stderr, "iter: %d\n", iter);
    return totalHeight;
}