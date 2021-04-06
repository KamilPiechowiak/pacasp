#ifndef BOTTOM_LEFT_HPP
#define BOTTOM_LEFT_HPP

#include"general/rectangle.hpp"
#include"general/imgSaver.hpp"
#include"algorithms/recordable.hpp"

struct BottomLeft : Recordable {
private:
    static string pref;
    static ll w;
    static vector<Rectangle> rect;
    static vector<pll> placement;
    static ll maxTime;
    static void saveImg(string name, ll height);
    static vector<int> getOrder();
    static vector<int> getRandomOrder();
    static void reverseSequence(int a, int b, vector<int> &ord);
    static void swapElements(int a, int b, vector<int> &ord);
    static vector<int> getRandomNeighbour(vector<int> &ord);
    static ll blds(function<bool(const Rectangle &a, const Rectangle &b)> sortingFunction, bool reverse, string name); //bottom left decreasing sth
    static ll graspBlds(int bufferSize, function<bool(const Rectangle &a, const Rectangle &b)> sortingFunction, string name);
    static ll getHash(vector<int> &ord);
    static void addToTabu(vector<int> &ord, deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
    static void addToTabu(ll h, deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
    static void removeFromTabu(deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
public:
    static void init(ll w, vector<Rectangle> &rect, ll maxTime=1000); //maxTime in milliseconds
    static ll bldw();
    static ll graspBldw(int bufferSize);
    static ll bldh();
    static ll graspBldh(int bufferSize);
    static ll blda();
    static ll bliw();
    static ll blih();
    static ll blia();
    static ll graspBlda(int bufferSize);
    static ll bl(vector<Rectangle> &rect, vector<int> &ord, bool debug=false);
    static ll hillClimber();
    static ll simulatedAnnealing(double mult0=0.9, int maxNumberOfAccepted=2, int maxNumberOfIterations=20);
    static ll multiStartLocalSearch(int numberOfLists = 10, int numberOfNeighbours = 100);
    static ll iteratedLocalSearch();
    static ll tabuSearch(int tabuLength=30, int numberOfNeighbours=0);
    static ll tabuSearch2(int tabuLength=30, int numberOfNeighbours=0);
};

#endif