#include"algorithms/local_search/tabuSearch.hpp"
#include"algorithms/local_search/neighborhood/reverseSwapNeighborhood.hpp"

TabuSearch::TabuSearch(int tabu_length, int number_of_neighbors, Recorder &recorder, bool hash_from_order, Neighborhood *neighborhood) :
    tabu_length(tabu_length), number_of_neighbors(number_of_neighbors), recorder(recorder), hash_from_order(hash_from_order), neighborhood(neighborhood) {}

ll TabuSearch::get_hash(vector<int> &ord) {
    ll p = 2137;
    ll res=0;
    ll x = 1;
    for(int a : ord) {
        res+=a*x;
        x*=p;
    }
    return res;
}

void TabuSearch::add_to_tabu(ll hash) {
    if(tabu_map.count(hash)) {
        tabu_map[hash]++;
    } else {
        tabu_map[hash] = 1;
    }
    tabu_list.push_back(hash);
}

void TabuSearch::remove_from_tabu() {
    ll hash = tabu_list.front();
    tabu_list.pop_front();
    if(tabu_map[hash] > 1) {
        tabu_map[hash]--;
    } else {
        tabu_map.erase(hash);
    }
}

ll TabuSearch::run(PackingAlgorithm *packing_algorithm, vector<int> &ord) {
    if(neighborhood == NULL) {
        neighborhood = new ReverseSwapNeighborhood(SIZE(ord));
    }
    ll best_height = packing_algorithm->run(ord);
    vector<int> best_ord = ord;
    recorder.record(best_height);
    if(hash_from_order) {
        add_to_tabu(get_hash(ord));   
    }
    int iter=0;
    while(!recorder.should_finish()) {
        iter++;
        vector<int> candidate_ord;
        ll candidate_height = LL_INF;
        ll candidate_hash = 0;
        for(int i=0; i < number_of_neighbors; i++) {
            ll state = neighborhood->next_random(ord);
            ll state_hash;
            if(hash_from_order) {
                state_hash = get_hash(ord);
            } else {
                state_hash = state;
            }
            if(tabu_map.count(state_hash)) {
                neighborhood->apply(state, ord); //reverse move on order
                continue;
            }
            ll height = packing_algorithm->run(ord);
            if(height < candidate_height) {
                candidate_height = height;
                candidate_ord = ord;
                candidate_hash = state_hash;
            }
            neighborhood->apply(state, ord); //reverse move on order
            if(recorder.should_finish()) {
                break;
            }
        }
        if(candidate_ord.size() > 0) {
            ord = candidate_ord;
            if(candidate_height < best_height) {
                best_height = candidate_height;
                best_ord = candidate_ord;
                recorder.record(best_height);
            }
            add_to_tabu(candidate_hash);
        }
        if(SIZE(tabu_list) > tabu_length) {
            remove_from_tabu();
        }
    }
    cerr << iter << "\n";
    packing_algorithm->run_and_save("ts", best_ord);
    return best_height;
}
