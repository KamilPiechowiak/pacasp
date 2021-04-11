#ifndef TUNER_HPP
#define TUNER_HPP

#include"general/general.hpp"
#include"parallel.hpp"

enum ActionType {
    _BLsimulatedAnnealing,
    _BLmultiStartLocalSearch,
    _BLtabuSearch_ord_hash,
    _BLtabuSearch_move_hash,
    _SHSimulatedAnnealing,
    _graspBldw,
    _graspBldh,
    _graspBlda,
    _BLiteratedLocalSearch,
    _SKsimulatedAnnealing,
    _SKmultiStartLocalSearch,
    _SKtabuSearch_ord_hash,
    _SKtabuSearch_move_hash,
    _SKiteratedLocalSearch,
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
    int instanceId;
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