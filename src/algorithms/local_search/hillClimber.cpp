#include"algorithms/local_search/hillClimber.hpp"
#include"algorithms/local_search/neighborhood/reverseSwapNeighborhood.hpp"

HillClimber::HillClimber(Recorder &recorder, Neighborhood *neighborhood) : recorder(recorder), neighborhood(neighborhood) {}

ll HillClimber::run(PackingAlgorithm *packing_algorithm, vector<int> &ord) {
    if(neighborhood == NULL) {
        neighborhood = new ReverseSwapNeighborhood(SIZE(ord));
    }
    ll best_height = packing_algorithm->run(ord);
    recorder.record(best_height);
    int iter=0;
    while(!recorder.should_finish()) {
        iter++;
        vector<int> best_neigh;
        ll current_height = LL_INF;
        int neighborhood_size = neighborhood->get_size();
        for(int i=0; i < neighborhood_size; i++) {
            if(!neighborhood->apply(i, ord)) {
                continue;
            }
            ll height = packing_algorithm->run(ord);
            if(height < current_height) {
                current_height = height;
                best_neigh = ord;
            }
            neighborhood->apply(i, ord);
            if(recorder.should_finish()) {
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