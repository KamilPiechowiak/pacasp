#ifndef SKYLINE_HPP
#define SKYLINE_HPP

#include"general/rectangle.hpp"
#include"general/imgSaver.hpp"
#include"algorithms/recordable.hpp"
#include"algorithms/skyline/scoreRule.hpp"
#include"algorithms/skyline/skylineSegment.hpp"

struct Skyline : Recordable {
private:
    static string pref;
    static ll w;
    static vector<Rectangle> rect;
    static vector<pll> placement;
    static ll maxTime;
    static void saveImg(string name, ll height);
    static ll bestFit(vector<Rectangle> &rect, vector<int> &ord, ScoreRule *scoreRule);
    static vector<int> getOrder();
    static vector<int> getRandomNeighbourSwap(vector<int> ord);
    static void reverseSequence(int a, int b, vector<int> &ord);
    static void swapElements(int a, int b, vector<int> &ord);
    static void addToTabu(ll h, deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
    static void removeFromTabu(deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
public:
    static void init(ll w, vector<Rectangle> &rect, ll maxTime=1000); //maxTime in milliseconds
    static ll burke();
    static ll ish();
    static ll tabuSearch(int tabuLength=30, int numberOfNeighbours=100);
};

#endif