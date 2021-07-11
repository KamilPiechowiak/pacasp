#ifndef PARALLEL_HPP
#define PARALLEL_HPP

#include"includes/json.hpp"

#include"general/general.hpp"
#include"general/rectangle.hpp"

using json = nlohmann::json;

struct Parallel {
protected:
    int numberOfNodes;
    int nodeId;
    ll maxTime;
    string filename;
    vector<Rectangle> rect;
    ll w;
public:
    Parallel(ll maxTime);
    ~Parallel();
    virtual void run(json config) =0;
    void destroy();
    void loadData();
};

#endif