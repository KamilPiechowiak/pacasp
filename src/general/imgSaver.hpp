#ifndef IMGSAVER_H
#define IMGSAVER_H

#include"general/rectangle.hpp"
#if DEBUG
    #include<Magick++.h>
#endif

struct HSLColor {
    double h;
    double s;
    double l;
    HSLColor(double _h, double _s, double _l) : h(_h), s(_s), l(_l) {}
};

struct ImgSaver {
private:
    static vector<HSLColor> colorPalette;
    static vector<HSLColor> getColorPalette(int n);
public:
    static void init(int numberOfElements);
    static void saveImg(string name, ll width, ll height, vector<Rectangle> &rect, vector<pll> &placement);
};

#endif