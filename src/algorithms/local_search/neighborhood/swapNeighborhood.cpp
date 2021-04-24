#include"algorithms/local_search/neighborhood/swapNeighborhood.hpp"

SwapNeighborhood::SwapNeighborhood(int n) : n(n) {}

ll SwapNeighborhood::get_size() {
    return ll(n)*ll(n-1);
}

bool SwapNeighborhood::apply(ll state, vector<int> &ord) {
    int a = state/(n-1);
    int b = state%(n-1);
    if(b >= a) {
        b++;
    } else {
        return false;
    }
    swap(ord[a], ord[b]);
    return true;
}
