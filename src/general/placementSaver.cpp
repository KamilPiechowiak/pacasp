#include"general/placementSaver.hpp"

void PlacementSaver::savePlacement(string name, ll width, ll height, vector<Rectangle> &rect, vector<pll> &placement) {
    ofstream f;
    f.open("placements/"+name);
    int n = SIZE(rect);
    f << n << " " << width << " " << height << "\n";
    for(int i=0; i < n; i++) {
        Rectangle &r = rect[i];
        pll &pos = placement[r.id];
        f << pos.first << " " << pos.second << " " << pos.first+r.width-1 << " " << pos.second+r.height-1 << "\n";
    }
    f.close();
}