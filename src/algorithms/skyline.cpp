#include"algorithms/skyline.hpp"
#include"algorithms/skyline/burkeScoreRule.hpp"
#include"algorithms/skyline/ishScoreRule.hpp"
#include"algorithms/skyline/ishBruteScoreRule.hpp"
#include"algorithms/packing_algorithm.hpp"
#include"algorithms/recorder.hpp"
#include"algorithms/local_search/tabuSearch.hpp"
#include"algorithms/local_search/hillClimber.hpp"
#include"algorithms/local_search/iteratedLocalSearch.hpp"
#include"algorithms/local_search/simulatedAnnealing.hpp"

Skyline::Skyline(ll w, vector<Rectangle> &rect, Recorder &recorder) : w(w), rect(rect), recorder(recorder) {
    skyline_packing = new SkylinePacking(w, rect, "SK", NULL);
}

vector<int> Skyline::getOrder() {
    int n = (int)rect.size();
    vector<int> ord;
    for(int i=0; i < n; i++) {
        ord.push_back(i);
    }
    return ord;
}

ll Skyline::burke() {
    recorder.open_log("SKBurke");
    
    auto ord = getOrder();
    BurkeScoreRule scoreRule;
    skyline_packing->setScoreRule(&scoreRule);

    ll best_height = skyline_packing->run(ord);
    recorder.record(best_height);
    skyline_packing->save(recorder.get_filename(), best_height);
    return best_height;
}

vector<int> Skyline::getRandomNeighbourSwap(vector<int> ord) {
    int a = ri(0, SIZE(ord)-1), b = ri(0, SIZE(ord)-2);
    if(b>=a) b++;
    swap(ord[a], ord[b]);
    return ord;
}

ll Skyline::ish() {
    recorder.open_log("SKISH");
    
    int n = SIZE(rect);
    auto ord = getOrder();
    vector<int> bestOrder;
    vector<function<bool(const int &a, const int &b)>> sorting_rules = {
        [&](const int &a, const int &b) {return rect[a].width*rect[a].height > rect[b].width*rect[b].height; },
        [&](const int &a, const int &b) {return rect[a].height > rect[b].height; },
        [&](const int &a, const int &b) {return rect[a].width > rect[b].width; },
        [&](const int &a, const int &b) {return perimeter(rect[a].width,rect[a].height) > perimeter(rect[b].width,rect[b].height); }
    };

    ISHScoreRule scoreRule;
    // ISHBruteScoreRule scoreRule;
    skyline_packing->setScoreRule(&scoreRule);
    vector<pair<ll, int>> resulting_heights;
    ll bestHeight = LL_INF;
    for(int i=0; i < SIZE(sorting_rules); i++) {
        sort(ord.begin(), ord.end(), sorting_rules[i]);
        ll height = skyline_packing->run(ord);
        if(height < bestHeight) {
            bestHeight = height;
            bestOrder = ord;
            recorder.record(bestHeight);
        }
        resulting_heights.push_back({height, i});
        // for(int id : ord) {
        //     printf("%d,",id);
        // }
        // printf("\n%lld,%d\n", height, i);
    }
    sort(resulting_heights.begin(), resulting_heights.end());
    // cout << resulting_heights[0].second << "\n";
    while(true) {
        for(auto height_id : resulting_heights) {
            sort(ord.begin(), ord.end(), sorting_rules[height_id.second]);
            ll currentHeight = height_id.first;
            for(int i=0; i < n; i++) {
                auto neigh = getRandomNeighbourSwap(ord);
                ll height = skyline_packing->run(neigh);
                if(height < currentHeight) {
                    currentHeight = height;
                    ord = neigh;
                    if(height < bestHeight) {
                        bestHeight = height;
                        bestOrder = ord;
                        recorder.record(bestHeight);
                    }
                }
                if(recorder.should_finish()) {
                    goto endLoop;
                }
            }
        }
    }
    endLoop: //saveImg("ISH", bestHeight);
    skyline_packing->run_and_save(recorder.get_filename(), bestOrder);
    return bestHeight;
}

void Skyline::reverseSequence(int a, int b, vector<int> &ord) {
    if(a > b) swap(a, b);
    reverse(ord.begin()+a, ord.begin()+b);
}

void Skyline::swapElements(int a, int b, vector<int> &ord) {
    swap(ord[a], ord[b]);
}

ll Skyline::tabuSearch(bool hash_from_order, int tabu_length, int number_of_neighbors) {
    recorder.open_log("SKts_" + to_string(hash_from_order) + "_" + to_string(tabu_length) + "_" + to_string(number_of_neighbors));
    ISHScoreRule scoreRule;
    skyline_packing->setScoreRule(&scoreRule);
    TabuSearch tabu_search(tabu_length, number_of_neighbors, recorder, hash_from_order);
    vector<int> ord = getOrder();
    sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width > rect[b].width; });
    return tabu_search.run(skyline_packing, ord);
}

ll Skyline::hillClimber(bool greedy) {
    recorder.open_log("SKhc_" + to_string(greedy));
    ISHScoreRule scoreRule;
    skyline_packing->setScoreRule(&scoreRule);
    HillClimber hill_climber(recorder, greedy);
    vector<int> ord = getOrder();
    sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width > rect[b].width; });
    return hill_climber.run(skyline_packing, ord);
}

ll Skyline::simulatedAnnealing(double mult, int max_number_of_accepted, int max_number_of_rejected) {
    recorder.open_log("SKsa_" + to_string(mult) + "_" + to_string(max_number_of_accepted) + "_" + to_string(max_number_of_rejected));
    ISHScoreRule scoreRule;
    skyline_packing->setScoreRule(&scoreRule);
    SimulatedAnnealing simulated_annealing(mult, max_number_of_accepted, max_number_of_rejected, recorder);
    vector<int> ord = getOrder();
    sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width > rect[b].width; });
    return simulated_annealing.run(skyline_packing, ord);
}

ll Skyline::multiStartLocalSearch(int number_of_neighbors) {
    recorder.open_log("SKmls_" + to_string(number_of_neighbors));
    ISHScoreRule scoreRule;
    skyline_packing->setScoreRule(&scoreRule);
    IteratedLocalSearch iterated_local_search(number_of_neighbors, 0, recorder);
    vector<int> ord = getOrder();
    sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width > rect[b].width; });
    return iterated_local_search.run(skyline_packing, ord);
}

ll Skyline::iteratedLocalSearch(int number_of_neighbors, int perturbation_inversed_intensity) {
    recorder.open_log("SKils_" + to_string(number_of_neighbors) + "_" + to_string(perturbation_inversed_intensity));
    ISHScoreRule scoreRule;
    skyline_packing->setScoreRule(&scoreRule);
    IteratedLocalSearch iterated_local_search(number_of_neighbors, perturbation_inversed_intensity, recorder);
    vector<int> ord = getOrder();
    sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width > rect[b].width; });
    return iterated_local_search.run(skyline_packing, ord);
}