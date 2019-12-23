#ifndef SHELF_H
#define SHELF_H

#include"general/rectangle.hpp"
#include"general/imgSaver.hpp"
#include"algorithms/recordable.hpp"

enum ggOrder {
    heightAsc, heightDsc, widthAsc, widthDsc
};

enum ggShelf {
    bestFit, worstFit, nextFit
};

struct Shelf : Recordable {
private:
    static string pref;
    static ll maxTime;
    static ll w;
    static vector<Rectangle> rr;
    static vector<pll> placement;
    static vector<vector<int>> getBFDHLevels(vector<Rectangle> rect);
    static ll matchLevels(vector<int> &a, vector<int> &b, vector<Rectangle> &rect);
    static ll findWidth(vector<int> &v, vector<Rectangle> &rect);
    static vector<vector<ll>> compress(vector<Rectangle> &rect, vector<vector<int>> &levels);
    static pair<pii, ll> getNeighbour(int firstIndex, int secondIndex, vector<int> &ord, vector<vector<ll>> &d);
    static pair<pii, ll> getRandomNeighbour(vector<int> &ord, vector<vector<ll>> &d);
    static pair<vector<int>, ll> getOrder(vector<vector<ll>> &d, int numberOfLevels);
    static void draw(string name, vector<int> &ord, vector<vector<int>> &levels, vector<Rectangle> &rect, vector<vector<ll>> &d);
public:
    static void init(ll w, vector<Rectangle> &rr, ll maxTime=1000);
    static void saveImg(string name, ll height, vector<Rectangle> &rect);
    static ll nfdh();
    static ll bfdh();
    static ll genericGreedy(ggOrder order, ggShelf shelf, bool rotateShelves=false);
    static ll hillClimber();
    static ll simulatedAnnealing();
    static ll simulatedAnnealing2(double mult0=0.9, int maxNumberOfAccepted=3, int maxNumberOfIterations=10);
};

#endif