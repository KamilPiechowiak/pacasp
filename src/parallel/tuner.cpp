#include"tuner.hpp"
#include"general/rectangle.hpp"
#include"algorithms/bottomLeft.hpp"
#include"algorithms/shelf.hpp"
#include"algorithms/bounds.hpp"
#include"algorithms/skyline.hpp"

Tuner::Tuner(ll maxTime) : Parallel(maxTime) {} 

void Tuner::generateInstance() {
    string name = "instances/" + to_string(nodeId) + ".in";
    string instr = "./gen " + to_string(nodeId) + " " + to_string(instanceSize[nodeId]) + " > " + name;
    system(instr.c_str());
    filename = name;
}

void Tuner::getResults() {
    Recorder recorder("tune", instanceId, maxTime);
    BottomLeft bottom_left(w, rect, recorder);
    Shelf shelf(w, rect, recorder);
    Skyline skyline(w, rect, recorder);
    vector<int> p;
    if(par.size() > 2) {
        p.push_back(round(par[1]));
        p.push_back(round(par[2]));
    } else if(par.size() == 2) {
        p.push_back(round(par[0]));
        p.push_back(round(par[1]));
    } else {
        p.push_back(round(par[0]));
    }
    if(defaultAction == _BLsimulatedAnnealing) {
        bottom_left.simulatedAnnealing(exp(-1.0/par[0]), p[0], p[1]);
    } else if(defaultAction == _BLtabuSearch_ord_hash) {
        bottom_left.tabuSearch(true, p[0], p[1]);
    } else if(defaultAction == _BLtabuSearch_move_hash) {
        bottom_left.tabuSearch(false, p[0], p[1]);
    } else if(defaultAction == _SHSimulatedAnnealing) {
        shelf.simulatedAnnealing2(exp(-1.0/par[0]), p[0], p[1]);
    } else if(defaultAction == _graspBldh) {
        bottom_left.graspBldh(p[0]);
    } else if(defaultAction == _graspBldw) {
        bottom_left.graspBldw(p[0]);
    } else if(defaultAction == _graspBlda) {
        bottom_left.graspBlda(p[0]);
    } else if(defaultAction == _BLmultiStartLocalSearch) {
        bottom_left.multiStartLocalSearch(p[0]);
    } else if(defaultAction == _BLiteratedLocalSearch) {
        bottom_left.iteratedLocalSearch(p[0], p[1]);
    } else if(defaultAction == _SKsimulatedAnnealing) {
        skyline.simulatedAnnealing(exp(-1.0/par[0]), p[0], p[1]);
    } else if(defaultAction == _SKmultiStartLocalSearch) {
        skyline.multiStartLocalSearch(p[0]);
    } else if(defaultAction == _SKtabuSearch_ord_hash) {
        skyline.tabuSearch(true, p[0], p[1]);
    } else if(defaultAction == _SKtabuSearch_move_hash) {
        skyline.tabuSearch(false, p[0], p[1]);
    } else if(defaultAction == _SKiteratedLocalSearch) {
        skyline.iteratedLocalSearch(p[0], p[1]);
    } else {
        exit(-1);
    }
}

void Tuner::rec(int d, vector<pdd> &v) {
    if(d == (int)v.size()) {
        getResults();
        return;
    }
    double mult = pow(v[d].second/v[d].first, 1.0/double(steps-1));
    double val = v[d].first;
    for(int i=0; i < steps; i++) {
        par.push_back(val);
        rec(d+1, v);
        par.pop_back();
        val*=mult;
    }
}

void Tuner::tune() {
    unordered_map<ActionType, vector<pdd>, EnumClassHash> m;

    m[_BLsimulatedAnnealing] = {{2.0, 1000.0}, {1.0, 100.0}, {1.0, 10000.0}};
    m[_BLtabuSearch_ord_hash] = {{1.0, 1000.0}, {1.0, 1000.0}};
    m[_BLtabuSearch_move_hash] = {{1.0, 1000.0}, {1.0, 1000.0}};
    m[_SHSimulatedAnnealing] = {{2.0, 1000.0}, {1.0, 100.0}, {1.0, 10000.0}};
    m[_graspBldw] = {{1.0, 1000.0}};
    m[_graspBldh] = {{1.0, 1000.0}};
    m[_graspBlda] = {{1.0, 1000.0}};
    
    m[_BLmultiStartLocalSearch] = {{1.0, 1000.0}};
    m[_BLiteratedLocalSearch] = {{1.0, 1000.0}, {1.0, 1000.0}};
    m[_SKsimulatedAnnealing] = {{2.0, 1000.0}, {1.0, 100.0}, {1.0, 10000.0}};
    m[_SKmultiStartLocalSearch] = {{1.0, 1000.0}};
    m[_SKtabuSearch_ord_hash] = {{1.0, 1000.0}, {1.0, 1000.0}};
    m[_SKtabuSearch_move_hash] = {{1.0, 1000.0}, {1.0, 1000.0}};
    m[_SKiteratedLocalSearch] = {{1.0, 1000.0}, {1.0, 1000.0}};

    vector<int> sizes = {62, 125, 250, 500, 1000, 2000, 4000, 8000};
    int instancesPerTimePerSize = 10;
    // vector<ActionType> actions = {_simulatedAnnealing, _tabuSearch, _tabuSearch2, _shelfSimulatedAnnealing, _graspBldw, _graspBldh, _graspBlda};
    vector<ActionType> actions = {
        _BLmultiStartLocalSearch,
        // _BLiteratedLocalSearch,
        // _SKsimulatedAnnealing,
        _SKmultiStartLocalSearch,
        // _SKtabuSearch_ord_hash,
        // _SKtabuSearch_move_hash,
        // _SKiteratedLocalSearch
    };

    // sizes = {125};
    // instancesPerTimePerSize = 2;
    // actions = {_tabuSearch2};
    assert(SIZE(sizes)*instancesPerTimePerSize*SIZE(actions) == numberOfNodes);
    // cerr << SIZE(sizes)*instancesPerTimePerSize*SIZE(actions) << " " << numberOfNodes << "\n";

    // for(int s : sizes) {
    //     for(int i=0; i < instancesPerTimePerSize; i++) {
    //         instanceSize.push_back(s);
    //     }
    // }

    // generateInstance();
    int totalInstances = SIZE(sizes)*instancesPerTimePerSize;
    instanceId = nodeId%totalInstances;
    filename = "used_instances/tuning_new_format/" + to_string(instanceId) + string(".in");
    loadData();
    ActionType a = actions[nodeId/totalInstances];
    if(m[a].size() > 2) {
        steps = 5;
    } else {
        steps = 10;
    }
    defaultAction = a;
    rec(0, m[defaultAction]);
}

void Tuner::run(json config) {
    tune();
    destroy();
}