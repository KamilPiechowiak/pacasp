#ifndef SKYLINE_SEGMENT_HPP
#define SKYLINE_SEGMENT_HPP

#include"general/general.hpp"

struct SkylineSegment {
    ll left;
    ll right;
    ll height;
    ll left_height;
    ll right_height;
    SkylineSegment(ll left, ll right, ll height) : left(left), right(right), height(height) {
        left_height = LL_INF;
        right_height = LL_INF;
    }
};

#endif
