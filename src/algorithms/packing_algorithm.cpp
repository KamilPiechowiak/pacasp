#include"algorithms/packing_algorithm.hpp"
#include"general/imgSaver.hpp"

PackingAlgorithm::PackingAlgorithm(ll width, vector<Rectangle> rect, string name) : w(width), rect(rect), name(name) {
    placement.resize(SIZE(rect));
}

ll PackingAlgorithm::run_and_save(string name, vector<int> &ord) {
    ll height = this->run(ord);
    ImgSaver::saveImg(this->name+"-"+name, w, height, rect, placement);
    return height;
}