#include"algorithms/local_search/neighborhood/reverseSwapNeighborhood.hpp"

ReverseSwapNeighborhood::ReverseSwapNeighborhood(int n) :
    n(n), reverse_neighborhood(ReverseNeighborhood(n)), swap_neighborhood(SwapNeighborhood(n)) {
    swap_size = swap_neighborhood.get_size();
}

ll ReverseSwapNeighborhood::get_size() {
    return reverse_neighborhood.get_size() + swap_neighborhood.get_size();
}

bool ReverseSwapNeighborhood::apply(ll state, vector<int> &ord) {
    if(state >= swap_size) {
        state-=swap_size;
        return reverse_neighborhood.apply(state, ord);
    } else {
        return swap_neighborhood.apply(state, ord);
    }
}
