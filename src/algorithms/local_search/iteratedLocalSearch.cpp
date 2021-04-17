#include"algorithms/local_search/iteratedLocalSearch.hpp"
#include"algorithms/local_search/neighborhood/reverseSwapNeighborhood.hpp"
#include"algorithms/local_search/neighborhood/swapNeighborhood.hpp"

IteratedLocalSearch::IteratedLocalSearch(int number_of_neighbors, int perturbation_inversed_intensity, Recorder &recorder, Neighborhood *neighborhood) :
    number_of_neighbors(number_of_neighbors), perturbation_inversed_intensity(perturbation_inversed_intensity), recorder(recorder), neighborhood(neighborhood) {}

ll IteratedLocalSearch::run(PackingAlgorithm *packing_algorithm, vector<int> &ord) {
    if(neighborhood == NULL) {
        neighborhood = new ReverseSwapNeighborhood(SIZE(ord));
    }
    ll current_height = packing_algorithm->run(ord);
    vector<int> best_ord = ord;
    ll best_height = current_height;
    recorder.record(best_height);
    int iter=0;
    while(!recorder.should_finish()) {
        iter++;
        vector<int> candidate_ord;
        ll candidate_height = LL_INF;
        for(int i=0; i < number_of_neighbors; i++) {
            int state = neighborhood->next_random(ord);
            ll height = packing_algorithm->run(ord);
            if(height < candidate_height) {
                candidate_height = height;
                candidate_ord = ord;
            }
            neighborhood->apply(state, ord);
            if(recorder.should_finish()) {
                break;
            }
        }
        if(candidate_height >= current_height) { //perturbate
            if(perturbation_inversed_intensity == 0) {
                random_shuffle(ord.begin(), ord.end());
            } else {
                ord = best_ord;
                SwapNeighborhood swap_neighborhood(SIZE(ord));
                for(int i=0; i < SIZE(ord); i+=perturbation_inversed_intensity) {
                    swap_neighborhood.next_random(ord);
                }
            }
            current_height = packing_algorithm->run(ord);
        } else {
            current_height = candidate_height;
            ord = candidate_ord;
            if(current_height < best_height) {
                best_height = current_height;
                best_ord = ord;
                recorder.record(best_height);
            }
        }
    }
    cerr << iter << "\n";
    packing_algorithm->run_and_save("ils", best_ord);
    return best_height;
}