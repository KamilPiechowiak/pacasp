#ifndef SEGMENT_TREE_HPP
#define SEGMENT_TREE_HPP

struct SegmentTree {
private:
    vector<int> tree;
    int shift;
public:
    void init(int size);
    void update(int pos, int value);
    int get_min(int left, int right);
};

#endif