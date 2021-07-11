#ifndef SHELF_H
#define SHELF_H

#include"general/rectangle.hpp"
#include"general/imgSaver.hpp"
#include"algorithms/recorder.hpp"
#include"algorithms/shelf/shelfCache.hpp"

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
    pair<pii, ll> getNeighbour(int firstIndex, int secondIndex, vector<int> &ord, ShelfCache &shelfCache);
    pair<pii, ll> getRandomNeighbour(vector<int> &ord, ShelfCache &shelfCache);
    pair<vector<int>, ll> getOrder(ShelfCache &shelfCache, int numberOfLevels);
    void draw(string name, vector<int> &ord, ShelfCache &shelfCache);
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