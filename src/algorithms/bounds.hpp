#ifndef BOUNDS_H
#define BOUNDS_H

#include"general/rectangle.hpp"
#include"algorithms/recorder.hpp"

struct Bounds {
private:
    ll w;
    vector<Rectangle> &rect;
    Recorder &recorder;
public:
    Bounds(ll w, vector<Rectangle> &rect, Recorder &recroder);
    ll byArea();
    ll byWiderThanHalf();
};

#endif