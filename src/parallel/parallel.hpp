#ifndef PARALLEL_HPP
#define PARALLEL_HPP

#include"general/general.hpp"

struct Parallel {
protected:
    int numberOfNodes;
    int nodeId;
    ll maxTime;
    string filename;
public:
    Parallel(ll maxTime);
    ~Parallel();
    virtual void run() =0;
    void destroy();
    virtual void generateInstance() =0;
    void loadData();
};

#endif