#ifndef PLACEMENTSAVER_HPP
#define PLACEMENTSAVER_HPP

#include"general/rectangle.hpp"

class PlacementSaver {
public:
    void savePlacement(string name, ll width, ll height, vector<Rectangle> &rect, vector<pll> &placement);
};

#endif