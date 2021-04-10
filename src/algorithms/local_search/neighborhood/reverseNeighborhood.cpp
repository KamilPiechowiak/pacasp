#include"algorithms/local_search/neighborhood/reverseNeighborhood.hpp"

ReverseNeighborhood::ReverseNeighborhood(int n) : n(n) {}

int ReverseNeighborhood::get_size() {
    return n*(n-1);
}

bool ReverseNeighborhood::apply(int state, vector<int> &ord) {
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
