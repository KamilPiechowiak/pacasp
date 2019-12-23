#ifndef RUNNER_HPP
#define RUNNER_HPP

#include"parallel.hpp"

struct Runner : Parallel {
    Runner(ll maxTime);
    void run();
    void generateInstance();
};

#endif