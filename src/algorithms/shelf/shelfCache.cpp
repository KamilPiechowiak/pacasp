#include"algorithms/shelf/shelfCache.hpp"

ll ShelfCache::matchLevels(vector<int> &a, vector<int> &b, vector<Rectangle> &rect) {
    int i=0, j=b.size()-1;
    ll aw=0, bw=0;
    ll h=0;
    while(i < (int)a.size() && j >= 0) {
        Rectangle &ra = rect[a[i]], &rb = rect[b[j]];
        h = max(h, ra.height + rb.height);
        if(aw+ra.width < bw+rb.width) {
            i++;
            aw+=ra.width;
        } else {
            j--;
            bw+=rb.width;
        }
    }
    return h;
}

ll ShelfCache::findWidth(vector<int> &v, vector<Rectangle> &rect) {
    ll res=0;
    for(int a : v) {
        res+=rect[a].width;
    }
    return res;
}

ShelfCache::ShelfCache(ll w, vector<Rectangle> _rect, vector<vector<int>> _levels, bool cache) : rect(_rect), levels(_levels) {
    int n = levels.size();
    if(cache) {
        d.resize(n, vector<ll>(n, -1));
    }
    for(int i=0; i < n; i++) {
        ll levelWidth = findWidth(levels[i], rect);
        int id = rect.size();
        levels[i].push_back(id);
        rect.push_back(Rectangle(w-levelWidth, 0, id));
    }
}

ll ShelfCache::getPairHeight(int i, int j) {
    if(d[i][j] == -1) {
        d[i][j] = matchLevels(levels[i], levels[j], rect);
        d[j][i] = d[i][j];
    }
    return d[i][j];
}

vector<vector<int>> ShelfCache::getLevels() {
    return levels;
}

vector<Rectangle> ShelfCache::getRect() {
    return rect;
}