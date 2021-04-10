#include"bounds.hpp"

Bounds::Bounds(ll w, vector<Rectangle> &rect, Recorder &recorder) : w(w), rect(rect), recorder(recorder) {}

ll Bounds::byArea() {
    recorder.open_log("BoundsByArea");
    ll area=0;
    for(Rectangle &r : rect) {
        area+=r.width*r.height;
    }
    ll bestHeight = area/w;
    if(bestHeight*w < area) {
        bestHeight++;
    }
    recorder.record(bestHeight);
    return bestHeight;
}

ll Bounds::byWiderThanHalf() {
    recorder.open_log("BoundsbyWiderThanHalf");
    ll bestHeight=0;
    for(Rectangle &r : rect) {
        if(r.width > w/2) {
            bestHeight+=r.height;
        }
    }
    recorder.record(bestHeight);
    return bestHeight;
}