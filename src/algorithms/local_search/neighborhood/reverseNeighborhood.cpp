#include"algorithms/local_search/neighborhood/reverseNeighborhood.hpp"

ReverseNeighborhood::ReverseNeighborhood(int n) : n(n) {}

ll ReverseNeighborhood::get_size() {
    return ll(n)*ll(n-1);
}

bool ReverseNeighborhood::apply(ll state, vector<int> &ord) {
    int a = state/(n-1);
    int b = state%(n-1);
    if(b >= a) {
        b++;
    } else {
        return false;
    }
    reverse(ord.begin()+a, ord.begin()+b+1);
    return true;
}
