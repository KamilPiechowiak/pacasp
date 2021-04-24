#include"bottomLeft.hpp"
#include"algorithms/local_search/tabuSearch.hpp"
#include"algorithms/local_search/hillClimber.hpp"
#include"algorithms/local_search/iteratedLocalSearch.hpp"
#include"algorithms/local_search/simulatedAnnealing.hpp"

BottomLeft::BottomLeft(ll w, vector<Rectangle> &rect, Recorder &recorder) : w(w), rect(rect), recorder(recorder), bottom_left_packing(w, rect, "BL") {}

vector<int> BottomLeft::getOrder() {
    int n = (int)rect.size();
    vector<int> ord;
    for(int i=0; i < n; i++) {
        ord.push_back(i);
    }
    return ord;
}

vector<int> BottomLeft::getRandomOrder() {
    vector<int> ord = getOrder();
    random_shuffle(ord.begin(), ord.end());
    return ord;
}

void BottomLeft::reverseSequence(int a, int b, vector<int> &ord) {
    if(a > b) swap(a, b);
    reverse(ord.begin()+a, ord.begin()+b);
}

void BottomLeft::swapElements(int a, int b, vector<int> &ord) {
    swap(ord[a], ord[b]);
}

vector<int> BottomLeft::getRandomNeighbour(vector<int> &ord) {
    vector<int> res(ord);
    int n=ord.size();
    if(n == 1) {
        return res;
    }
    int a=ri(0, n-1), b=ri(0, n-2);
    if(b >= a) {
        b++;
    }
    if(ri(0, 1)) {
        reverseSequence(a, b, res);
    } else {
        swapElements(a, b, res);
    }
    return res;
}

ll BottomLeft::blds(function<bool(const Rectangle &a, const Rectangle &b)> sortingFunction, bool reverseOrder, string name) {
    recorder.open_log("BL"+name);
    vector<int> ord = getOrder();
    sort(ord.begin(), ord.end(), [&](const int &a, const int &b){ return sortingFunction(rect[a], rect[b]); });
    if(reverseOrder) {
        reverse(ord.begin(), ord.end());
    }
    ll best_height = bottom_left_packing.run_and_save(name, ord);
    recorder.record(best_height);
    return best_height;
}

ll BottomLeft::bldw() {
    return blds(Rectangle::cmpByWidth, false, "bldw");
}

ll BottomLeft::bldh() {
    return blds(Rectangle::cmpByHeight, false, "bldh");
}

ll BottomLeft::blda() {
    return blds(Rectangle::cmpByArea, false, "blda");
}

ll BottomLeft::bliw() {
    return blds(Rectangle::cmpByWidth, true, "bliw");
}

ll BottomLeft::blih() {
    return blds(Rectangle::cmpByHeight, true, "blih");
}

ll BottomLeft::blia() {
    return blds(Rectangle::cmpByArea, true, "blia");
}

ll BottomLeft::graspBlds(int bufferSize, function<bool(const Rectangle &a, const Rectangle &b)> sortingFunction, string name) {
    recorder.open_log("BL" + name + "_" + to_string(bufferSize));

    vector<int> ord = getOrder();
    vector<int> best_ord;
    ll bestHeight = LL_INF;
    while(!recorder.should_finish()) {
        sort(ord.begin(), ord.end(), [&](const int &a, const int &b){return sortingFunction(rect[a], rect[b]); });
        int n = ord.size();
        for(int i=0; i < n; i++) {
            int a = min(n-1, i+ri(0, bufferSize-1));
            swap(ord[i], ord[a]);
        }
        ll currentHeight = bottom_left_packing.run(ord);
        if(currentHeight < bestHeight) {
            bestHeight = currentHeight;
            best_ord = ord;
            recorder.record(bestHeight);
        }
    }
    return bottom_left_packing.run_and_save(name, best_ord);
}

ll BottomLeft::graspBldw(int bufferSize) {
    return graspBlds(bufferSize, Rectangle::cmpByWidth, "graspBldw");
}

ll BottomLeft::graspBldh(int bufferSize) {
    return graspBlds(bufferSize, Rectangle::cmpByHeight, "graspBldh");
}

ll BottomLeft::graspBlda(int bufferSize) {
    return graspBlds(bufferSize, Rectangle::cmpByHeight, "graspBlda");
}

ll BottomLeft::hillClimber(bool greedy) {
    recorder.open_log("BLhc_" + to_string(greedy));
    HillClimber hill_climber(recorder, greedy);
    vector<int> ord = getRandomOrder();
    // sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width*rect[a].height > rect[b].width*rect[b].height; });
    return hill_climber.run(&bottom_left_packing, ord);
}

ll BottomLeft::simulatedAnnealing(double mult, int max_number_of_accepted, int max_number_of_rejected) {
    recorder.open_log("BLsa_" + to_string(mult) + "_" + to_string(max_number_of_accepted) + "_" + to_string(max_number_of_rejected));
    SimulatedAnnealing simulated_annealing(mult, max_number_of_accepted, max_number_of_rejected, recorder);
    vector<int> ord = getRandomOrder();
    // sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width > rect[b].width; });
    return simulated_annealing.run(&bottom_left_packing, ord);
}

ll BottomLeft::multiStartLocalSearch(int number_of_neighbors) {
    recorder.open_log("BLmls_" + to_string(number_of_neighbors));
    IteratedLocalSearch iterated_local_search(number_of_neighbors, 0, recorder);
    vector<int> ord = getRandomOrder();
    // sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width*rect[a].height > rect[b].width*rect[b].height; });
    return iterated_local_search.run(&bottom_left_packing, ord);
}

ll BottomLeft::iteratedLocalSearch(int number_of_neighbors, int perturbation_inversed_intensity) {
    recorder.open_log("BLils_" + to_string(number_of_neighbors) + "_" + to_string(perturbation_inversed_intensity));
    IteratedLocalSearch iterated_local_search(number_of_neighbors, perturbation_inversed_intensity, recorder);
    vector<int> ord = getRandomOrder();
    // sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width*rect[a].height > rect[b].width*rect[b].height; });
    return iterated_local_search.run(&bottom_left_packing, ord);
}

ll BottomLeft::tabuSearch(bool hash_from_order, int tabu_length, int number_of_neighbors) {
    recorder.open_log("BLts_" + to_string(hash_from_order) + "_" + to_string(tabu_length) + "_" + to_string(number_of_neighbors));
    TabuSearch tabu_search(tabu_length, number_of_neighbors, recorder, hash_from_order);
    vector<int> ord = getRandomOrder();
    // sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width*rect[a].height > rect[b].width*rect[b].height; });
    return tabu_search.run(&bottom_left_packing, ord);
}