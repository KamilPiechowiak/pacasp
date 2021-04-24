#include"algorithms/local_search/simulatedAnnealing.hpp"
#include"algorithms/local_search/neighborhood/reverseSwapNeighborhood.hpp"

SimulatedAnnealing::SimulatedAnnealing(double multiplier, int max_number_of_accepted, int max_number_of_rejected, Recorder &recorder, Neighborhood *neighborhood) :
    multiplier(multiplier), max_number_of_accepted(max_number_of_accepted), max_number_of_rejected(max_number_of_rejected), recorder(recorder), neighborhood(neighborhood) {}

ll SimulatedAnnealing::run(PackingAlgorithm *packing_algorithm, vector<int> &ord) {
    if(neighborhood == NULL) {
        neighborhood = new ReverseSwapNeighborhood(SIZE(ord));
    }
    ll current_height = packing_algorithm->run(ord);
    vector<int> best_ord = ord;
    ll best_height = current_height;
    recorder.record(best_height);
    ll initial_height = current_height;

    double T = 1000.0;

    int iter=0;
    int numberOfAccepted=0;
    int numberOfIterations=0;
    while(!recorder.should_finish()) {
        iter++;
        ll state = neighborhood->next_random(ord);
        ll height = packing_algorithm->run(ord);
        double df = (height-current_height)/double(initial_height);
        numberOfIterations++;
        if(df < 0 || exp(-df/T) > rd(0, 1)) {
            if(height != current_height) {
                numberOfAccepted++;
            }
            current_height = height;
        } else {
            neighborhood->apply(state, ord);
        }
        if(current_height < best_height) {
            best_ord = ord;
            best_height = current_height;
            recorder.record(best_height);
        }
        if(numberOfAccepted >= max_number_of_accepted) {
            T*=multiplier;
            numberOfAccepted=0;
            numberOfIterations=0;
        }
        if(numberOfIterations >= max_number_of_rejected) {
            T/=multiplier;
            numberOfIterations=0;
            numberOfAccepted=0;
        }
    }
    cerr << iter << "\n";
    packing_algorithm->run_and_save("sa", best_ord);
    return best_height;
}