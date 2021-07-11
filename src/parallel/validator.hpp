#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include"parallel/parallel.hpp"

struct RectanglePosition {
    ll x1, y1, x2, y2;
};

struct Validator : Parallel {
public:
    Validator(ll maxTime);
    void run(json config);
};

#endif