#include"bounds.hpp"

ll Bounds::byArea(ll w, vector<Rectangle> &rect) {
    openLog("byArea");
    startTimer();
    ll area=0;
    for(Rectangle &r : rect) {
        area+=r.width*r.height;
    }
    bestHeight = area/w;
    if(bestHeight*w < area) {
        bestHeight++;
    }
    record();
    return bestHeight;
}

ll Bounds::byWiderThanHalf(ll w, vector<Rectangle> &rect) {
    openLog("byWiderThanHalf");
    startTimer();
    bestHeight=0;
    for(Rectangle &r : rect) {
        if(r.width > w/2) {
            bestHeight+=r.height;
        }
    }
    record();
    return bestHeight;
}