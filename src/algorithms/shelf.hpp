#ifndef SHELF_H
#define SHELF_H

#include"general/rectangle.hpp"
#include"general/imgSaver.hpp"
#include"algorithms/recorder.hpp"

enum ggOrder {
    heightAsc, heightDsc, widthAsc, widthDsc
};

enum ggShelf {
    bestFit, worstFit, nextFit
};

struct Shelf {
private:
    string pref;
    ll w;
    vector<Rectangle> &rr;
    vector<pll> placement;
    Recorder &recorder;
    vector<vector<int>> getBFDHLevels(vector<Rectangle> rect);
    ll matchLevels(vector<int> &a, vector<int> &b, vector<Rectangle> &rect);
    ll findWidth(vector<int> &v, vector<Rectangle> &rect);
    vector<vector<ll>> compress(vector<Rectangle> &rect, vector<vector<int>> &levels);
    pair<pii, ll> getNeighbour(int firstIndex, int secondIndex, vector<int> &ord, vector<vector<ll>> &d);
    pair<pii, ll> getRandomNeighbour(vector<int> &ord, vector<vector<ll>> &d);
    pair<vector<int>, ll> getOrder(vector<vector<ll>> &d, int numberOfLevels);
    void draw(string name, vector<int> &ord, vector<vector<int>> &levels, vector<Rectangle> &rect, vector<vector<ll>> &d);
public:
    Shelf(ll w, vector<Rectangle> &rr, Recorder &recorder);
    void saveImg(string name, ll height, vector<Rectangle> &rect);
    ll nfdh();
    ll bfdh();
    ll genericGreedy(ggOrder order, ggShelf shelf, bool rotateShelves=false);
    ll hillClimber();
    ll simulatedAnnealing();
    ll simulatedAnnealing2(double mult0=0.9, int maxNumberOfAccepted=3, int maxNumberOfIterations=10);
};

#endif