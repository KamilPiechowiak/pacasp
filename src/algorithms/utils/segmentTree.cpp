#include"general/general.hpp"
#include"algorithms/utils/segmentTree.hpp"

void SegmentTree::init(int size) {
    int n = 1;
    while(n < size) {
        n*=2;
    }
    shift = n;
    tree.resize(2*n, INF);
}

void SegmentTree::update(int pos, int value) {
    pos+=shift;
    tree[pos] = value;
    pos/=2;
    while(pos) {
        tree[pos] = min(tree[2*pos], tree[2*pos+1]);
        pos/=2;
    }
}

int SegmentTree::get_min(int left, int right) {
    int res = INF;
    left+=shift, right+=shift;
    res = min(res, tree[left]);
    res = min(res, tree[right]);
    while(left/2 != right/2) {
        if(left%2 == 0) {
            res = min(res, tree[left+1]);
        }
        if(right%2 == 1) {
            res = min(res, tree[right-1]);
        }
        left/=2;
        right/=2;
    }
    return res;
}