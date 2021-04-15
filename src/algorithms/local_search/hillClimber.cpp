#include"algorithms/local_search/hillClimber.hpp"
#include"algorithms/local_search/neighborhood/reverseSwapNeighborhood.hpp"

HillClimber::HillClimber(Recorder &recorder, bool greedy, Neighborhood *neighborhood) : recorder(recorder), greedy(greedy), neighborhood(neighborhood) {}

ll HillClimber::run(PackingAlgorithm *packing_algorithm, vector<int> &ord) {
    if(neighborhood == NULL) {
        neighborhood = new ReverseSwapNeighborhood(SIZE(ord));
    }
    ll best_height = packing_algorithm->run(ord);
    recorder.record(best_height);
    int iter=0;
    int neighborhood_size = neighborhood->get_size();
    vector<int> permutation(neighborhood_size);
    for(int i=0; i < neighborhood_size; i++) {
        permutation[i] = i;
    }
    random_shuffle(permutation.begin(), permutation.end());
    while(!recorder.should_finish()) {
        iter++;
        vector<int> best_neigh;
        ll current_height = best_height;
        int start = ri(0, neighborhood_size-1);
        int j = start;
        while(true) {
            int i = permutation[j];
            if(neighborhood->apply(i, ord)) {
                ll height = packing_algorithm->run(ord);
                if(height < current_height) {
                    current_height = height;
                    best_neigh = ord;
                    if(greedy) {
                        break;
                    }
                }
                neighborhood->apply(i, ord);
            }
            if(recorder.should_finish()) {
                break;
            }
            j = (j+1)%neighborhood_size;
            if(j == start) {
                break;
            }
        }
        if(current_height >= best_height) {
            break;
        }      
        ord = best_neigh;
        best_height = current_height;
        recorder.record(best_height);
    }
    cerr << iter << "\n";
    packing_algorithm->run_and_save("hc", ord);
    return best_height;
}