#include"rectangle.hpp"

Rectangle::Rectangle(ll a, ll b, int i) : width(a), height(b), id(i) {}

bool Rectangle::cmpByHeight(const Rectangle &a, const Rectangle &b) {
    return a.height > b.height;
}

bool Rectangle::cmpByWidth(const Rectangle &a, const Rectangle &b) {
    return a.width > b.width;
}

bool Rectangle::cmpByArea(const Rectangle &a, const Rectangle &b) {
    return a.height*a.width > b.height*b.width;
}