#ifndef SHELF_CACHE_HPP
#define SHELF_CACHE_HPP

#include"general/general.hpp"
#include"general/rectangle.hpp"

struct ShelfCache {
public:
    vector<Rectangle> rect;
    vector<vector<int>> levels;
    vector<vector<ll>> d;
public:
    ll matchLevels(vector<int> &a, vector<int> &b, vector<Rectangle> &rect);
    ll findWidth(vector<int> &v, vector<Rectangle> &rect);
    ShelfCache(ll w, vector<Rectangle> rect, vector<vector<int>> levels, bool cache=true);
    ll getPairHeight(int i, int j);
    vector<vector<int>> getLevels();
    vector<Rectangle> getRect();
};

#endif