#ifndef SKYLINE_HPP
#define SKYLINE_HPP

#include"general/rectangle.hpp"
#include"algorithms/skyline/scoreRule.hpp"
#include"algorithms/skyline/skylineSegment.hpp"
#include"algorithms/skyline/skylinePacking.hpp"
#include"algorithms/recorder.hpp"

struct Skyline {
private:
    ll w;
    vector<Rectangle> &rect;
    Recorder &recorder;
    SkylinePacking *skyline_packing;
    vector<int> getOrder();
    vector<int> getRandomNeighbourSwap(vector<int> ord);
    void reverseSequence(int a, int b, vector<int> &ord);
    void swapElements(int a, int b, vector<int> &ord);
    void addToTabu(ll h, deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
    void removeFromTabu(deque<ll> &TabuList, unordered_map<ll, int> &TabuMap);
public:
    Skyline(ll w, vector<Rectangle> &rect, Recorder &recorder); //maxTime in milliseconds
    ll burke();
    ll ish();
    ll tabuSearch(bool hash_from_order=false, int tabu_length=30, int number_of_neighbors=100);
    ll hillClimber();
    ll simulatedAnnealing(double mult=0.9, int max_number_of_accepted=2, int max_number_of_rejected=20);
    ll multiStartLocalSearch(int number_of_neighbors = 100);
    ll iteratedLocalSearch(int number_of_neighbors=100, int perturbation_inversed_intensity=10);
};

#endif