#include"algorithms/bottomLeftPacking.hpp"

struct Segment {
    ll left;
    ll right;
    ll height;
    Segment(ll left, ll right, ll height) : left(left), right(right), height(height) {}
    bool operator==(const Segment b) {
        return (left == b.left && right == b.right && height == b.height) ? 1 : 0;
    }
};

bool operator<(const Segment a, const Segment b) {
    if(a.left < b.left) {
        return true;
    } else if(a.left > b.left) {
        return false;
    } else if(a.right < b.right) {
        return true;
    }
    return false;
}

BottomLeftPacking::BottomLeftPacking(ll width, vector<Rectangle> rect, string name)
    : PackingAlgorithm(width, rect, name) {}

ll BottomLeftPacking::run(vector<int> &ord) {
    ll currentHeight=0;
    set<Segment> levels;
    levels.insert(Segment(0, w-1, 0));
    deque<Segment> d;
    for(int id : ord) {
        // cerr << d.size() << "\n";
        Rectangle &r = rect[id];
        ll minHeight = currentHeight+1;
        ll start=0;
        auto i = levels.begin();
        auto j = levels.begin();
        for(; i != levels.end(); i++) {
            if(w < (*i).left+r.width) {
                break;
            }
            while(j != levels.end() && (*j).left < (*i).left+r.width) {
                while(d.empty() == false && d.back().height <= (*j).height) {
                    d.pop_back();
                }
                d.push_back((*j));
                j++;
            }
            if(d.front().height < minHeight) {
                start = (*i).left;
                minHeight = d.front().height;
            }
            if(d.front() == (*i)) {
                d.pop_front();
            }
        }
        Segment a(start, start+r.width-1, minHeight+r.height);
        i = levels.upper_bound(Segment(start, -1, 0));
        while(i != levels.end() && (*i).right <= start+r.width-1) {
            levels.erase(i++);
        }
        if(i != levels.end() && (*i).left <= start+r.width-1) {
            Segment b(start+r.width, (*i).right, (*i).height);
            levels.erase(i);
            levels.insert(b);
        }
        levels.insert(a);
        placement[r.id] = {start, minHeight};
        currentHeight = max(currentHeight, minHeight+r.height);
    }
    return currentHeight;
}