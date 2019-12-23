#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include"general/general.hpp"

struct Portfolio {
    int timeId;
    ll maxTime;
    vector<string> algorithms;
    int numberOfAlgorithms;
    vector<pll> instances;
    int numberOfInstances;
    vector<vector<pll>> data;

    vector<ll> qmin;
    vector<unordered_set<int>> wins;

    ll cost;

    ofstream f;
    string fname;
    ifstream g;
    string gname;

    Portfolio(int timeId);
    void run();
    void getTimeLimit();
    void getAlgorithmsList();
    void getInstances();
    void getData();
    void getWinners();
    void computeMinSth();
    void computeMinCost();
    void computeMinRegret();
    void oplrun(string name);
    void getResult();
};

#endif