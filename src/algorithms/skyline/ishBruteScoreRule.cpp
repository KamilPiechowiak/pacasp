#include"algorithms/skyline/ishBruteScoreRule.hpp"

void ISHBruteScoreRule::init(vector<Rectangle> &rect, vector<int> &ord) {
    this->rect = rect;
    this->ord = ord;
    int n = SIZE(rect);
    visited.clear();
    visited.resize(n, false);
}

RectanglePlacement ISHBruteScoreRule::next(SkylineSegment segment) {
    ll w = segment.right-segment.left+1;
    RectanglePlacement res(-1, true, false, false);
    bool any = false;
    int score = -1;
    for(int i : ord) {
        if(visited[i]) {
            continue;
        }
        any = true;
        if(w < rect[i].width) {
            continue;
        }
        if(score < 3 && w == rect[i].width && segment.left_height-segment.height == rect[i].height && segment.right_height-segment.height == rect[i].height) {
            score = 3;
            res = RectanglePlacement(i, true, true, false);
        }
        if(score < 2 && w == rect[i].width) {
            if(segment.left_height-segment.height == rect[i].height) {
                score = 2;
                res = RectanglePlacement(i, true, true, false);
            } else if(segment.right_height-segment.height == rect[i].height) {
                score = 2;
                res = RectanglePlacement(i, false, true, false);
            }
        }
        if(score < 1) {
            if(w == rect[i].width || segment.left_height-segment.height == rect[i].height) {
                score = 1;
                res = RectanglePlacement(i, true, true, false);
            }
            if(segment.right_height-segment.height == rect[i].height) {
                score = 1;
                res = RectanglePlacement(i, false, true, false);
            }
        }
        if(score < 0) {
            if(rect[i].width < w) {
                score = 0;
                res = RectanglePlacement(i, true, true, false);
            }
        }
    }
    if(!any) {
        return RectanglePlacement(0, false, false, true);
    }
    if(res.id > -1) {
        visited[res.id] = true;
        // fprintf(stderr, "brute score: %d %d\n", score, res.id);
    }
    return res;
}
