#ifndef RECTANGLE_H
#define RECTANGLE_H

#include"general/general.hpp"

struct Rectangle {
    ll width;
    ll height;
    int id;
    Rectangle(ll a, ll b, int i);
    static bool cmpByHeight(const Rectangle &a, const Rectangle &b);
    static bool cmpByWidth(const Rectangle &a, const Rectangle &b);
    static bool cmpByArea(const Rectangle &a, const Rectangle &b);
};

#endif