#include"tuner.hpp"
#include"general/rectangle.hpp"
#include"algorithms/bottomLeft.hpp"
#include"algorithms/shelf.hpp"
#include"algorithms/bounds.hpp"

Tuner::Tuner(ll maxTime) : Parallel(maxTime) {} 

void Tuner::generateInstance() {
    string name = "instances/" + to_string(nodeId) + ".in";
    string instr = "./gen " + to_string(nodeId) + " " + to_string(instanceSize[nodeId]) + " > " + name;
    system(instr.c_str());
    filename = name;
}

void Tuner::getResults() {
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
    if(defaultAction == _simulatedAnnealing) {
        BottomLeft::simulatedAnnealing(exp(-1.0/par[0]), p[0], p[1]);
    } else if(defaultAction == _multiStartLocalSearch) {
        BottomLeft::multiStartLocalSearch(p[0], p[1]);
    } else if(defaultAction == _tabuSearch) {
        BottomLeft::tabuSearch(p[0], p[1]);
    } else if(defaultAction == _tabuSearch2) {
        BottomLeft::tabuSearch2(p[0], p[1]);
    } else if(defaultAction == _shelfSimulatedAnnealing) {
        Shelf::simulatedAnnealing2(exp(-1.0/par[0]), p[0], p[1]);
    } else if(defaultAction == _graspBldh) {
        BottomLeft::graspBldh(p[0]);
    } else if(defaultAction == _graspBldw) {
        BottomLeft::graspBldw(p[0]);
    } else if(defaultAction == _graspBlda) {
        BottomLeft::graspBlda(p[0]);
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

    m[_simulatedAnnealing] = {{2.0, 1000.0}, {1.0, 100.0}, {1.0, 10000.0}};
    m[_multiStartLocalSearch] = {{1.0, 1000.0}, {1.0, 1000.0}};
    m[_tabuSearch] = {{1.0, 1000.0}, {1.0, 1000.0}};
    m[_tabuSearch2] = {{1.0, 1000.0}, {1.0, 1000.0}};
    m[_shelfSimulatedAnnealing] = {{2.0, 1000.0}, {1.0, 100.0}, {1.0, 10000.0}};
    m[_graspBldw] = {{1.0, 1000.0}};
    m[_graspBldh] = {{1.0, 1000.0}};
    m[_graspBlda] = {{1.0, 1000.0}};
    
    vector<int> sizes = {62, 125, 250, 500, 1000, 2000, 4000, 8000};
    int instancesPerTimePerSize = 10;
    vector<ActionType> actions = {_simulatedAnnealing, _multiStartLocalSearch, _tabuSearch, _tabuSearch2, _shelfSimulatedAnnealing, _graspBldw, _graspBldh, _graspBlda};

    // sizes = {125};
    // instancesPerTimePerSize = 2;
    // actions = {_tabuSearch2};
    assert(int(sizes.size())*instancesPerTimePerSize == numberOfNodes);

    for(int s : sizes) {
        for(int i=0; i < instancesPerTimePerSize; i++) {
            instanceSize.push_back(s);
        }
    }

    generateInstance();
    loadData();
    for(ActionType a : actions) {
        if(m[a].size() > 2) {
            steps = 5;
        } else {
            steps = 10;
        }
        defaultAction = a;
        rec(0, m[defaultAction]);
    }
}

void Tuner::run() {
    tune();
    destroy();
}