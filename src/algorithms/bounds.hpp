#ifndef BOUNDS_H
#define BOUNDS_H

#include"general/rectangle.hpp"
#include"recordable.hpp"

struct Bounds : Recordable {
    static ll byArea(ll w, vector<Rectangle> &rect);
    static ll byWiderThanHalf(ll w, vector<Rectangle> &rect);
};

#endif