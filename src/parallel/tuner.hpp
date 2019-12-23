#ifndef TUNER_HPP
#define TUNER_HPP

#include"general/general.hpp"
#include"parallel.hpp"

enum ActionType {
    _simulatedAnnealing,
    _multiStartLocalSearch,
    _tabuSearch,
    _tabuSearch2,
    _shelfSimulatedAnnealing,
    _graspBldw,
    _graspBldh,
    _graspBlda
};

struct EnumClassHash
{
    template <typename T>
    size_t operator()(T t) const
    {
        return static_cast<size_t>(t);
    }
};

struct Tuner : Parallel{
private:
    ActionType defaultAction;
    vector<double> par;
    int steps = 4;
    vector<int> instanceSize;
public:
    Tuner(ll maxTime);
    void run();
    void generateInstance();
    void computeParameters(vector<pdd> v);
    void rec(int d, vector<pdd> &v);
    void getResults();
    void tune();
};

#endif