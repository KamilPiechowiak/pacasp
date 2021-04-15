#ifndef BOTTOM_LEFT_HPP
#define BOTTOM_LEFT_HPP

#include"general/rectangle.hpp"
#include"general/imgSaver.hpp"
#include"algorithms/recorder.hpp"
#include"algorithms/bottomLeftPacking.hpp"

struct BottomLeft {
private:
    ll w;
    vector<Rectangle> &rect;
    Recorder &recorder;
    BottomLeftPacking bottom_left_packing;
    void saveImg(string name, ll height);
    vector<int> getOrder();
    vector<int> getRandomOrder();
    void reverseSequence(int a, int b, vector<int> &ord);
    void swapElements(int a, int b, vector<int> &ord);
    vector<int> getRandomNeighbour(vector<int> &ord);
    ll blds(function<bool(const Rectangle &a, const Rectangle &b)> sortingFunction, bool reverse, string name); //bottom left decreasing sth
    ll graspBlds(int bufferSize, function<bool(const Rectangle &a, const Rectangle &b)> sortingFunction, string name);
    ll getHash(vector<int> &ord);
    void addToTabu(vector<int> &ord, deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
    void addToTabu(ll h, deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
    void removeFromTabu(deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
public:
    BottomLeft(ll w, vector<Rectangle> &rect, Recorder &recorder);
    ll bldw();
    ll graspBldw(int bufferSize);
    ll bldh();
    ll graspBldh(int bufferSize);
    ll blda();
    ll bliw();
    ll blih();
    ll blia();
    ll graspBlda(int bufferSize);
    ll hillClimber(bool greedy=false);
    ll simulatedAnnealing(double mult=0.9, int max_number_of_accepted=2, int max_number_of_rejected=20);
    ll multiStartLocalSearch(int number_of_neighbors = 100);
    ll iteratedLocalSearch(int number_of_neighbors=100, int perturbation_inversed_intensity=10);
    ll tabuSearch(bool hash_from_order=false, int tabu_length=30, int number_of_neighbors=100);
};

#endif