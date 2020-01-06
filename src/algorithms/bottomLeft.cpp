#include"bottomLeft.hpp"

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

ll BottomLeft::w;
vector<Rectangle> BottomLeft::rect;
string BottomLeft::pref;
vector<pll> BottomLeft::placement;
ll BottomLeft::maxTime;

void BottomLeft::init(ll _w, vector<Rectangle> &_rect, ll _maxTime) {
    w = _w;
    rect = _rect;
    int n = rect.size();
    placement.resize(n);
    pref = "BL-";
    maxTime = 1000000LL*_maxTime;//in nanoseconds
}

void BottomLeft::saveImg(string name, ll height) {
    ImgSaver::saveImg(pref+name, w, height, rect, placement);
}

ll BottomLeft::bl(vector<Rectangle> &rect, vector<int> &ord) {
    ll currentHeight=0;
    set<Segment> levels;
    levels.insert(Segment(0, w-1, 0));
    deque<Segment> d;
    for(int id : ord) {
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
        i = levels.upper_bound(Segment(start, 0, 0));
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

vector<int> BottomLeft::getOrder() {
    int n = (int)rect.size();
    vector<int> ord;
    for(int i=0; i < n; i++) {
        ord.push_back(i);
    }
    return ord;
}

vector<int> BottomLeft::getRandomOrder() {
    vector<int> ord = getOrder();
    random_shuffle(ord.begin(), ord.end());
    return ord;
}

void BottomLeft::reverseSequence(int a, int b, vector<int> &ord) {
    if(a > b) swap(a, b);
    reverse(ord.begin()+a, ord.begin()+b);
}

void BottomLeft::swapElements(int a, int b, vector<int> &ord) {
    swap(ord[a], ord[b]);
}

vector<int> BottomLeft::getRandomNeighbour(vector<int> &ord) {
    vector<int> res(ord);
    int n=ord.size();
    if(n == 1) {
        return res;
    }
    int a=ri(0, n-1), b=ri(0, n-2);
    if(b >= a) {
        b++;
    }
    if(ri(0, 1)) {
        reverseSequence(a, b, res);
    } else {
        swapElements(a, b, res);
    }
    return res;
}

ll BottomLeft::blds(function<bool(const Rectangle &a, const Rectangle &b)> sortingFunction, bool reverseOrder, string name) {
    openLog(name);
    startTimer();
    vector<int> ord = getOrder();
    vector<Rectangle> rr = rect;
    sort(rr.begin(), rr.end(), sortingFunction);
    if(reverseOrder) {
        reverse(rr.begin(), rr.end());
    }
    bestHeight = bl(rr, ord);
    record();
    saveImg(name,bestHeight);
    return bestHeight;
}

ll BottomLeft::bldw() {
    return blds(Rectangle::cmpByWidth, false, "bldw");
}

ll BottomLeft::bldh() {
    return blds(Rectangle::cmpByHeight, false, "bldh");
}

ll BottomLeft::blda() {
    return blds(Rectangle::cmpByArea, false, "blda");
}

ll BottomLeft::bliw() {
    return blds(Rectangle::cmpByWidth, true, "bliw");
}

ll BottomLeft::blih() {
    return blds(Rectangle::cmpByHeight, true, "blih");
}

ll BottomLeft::blia() {
    return blds(Rectangle::cmpByArea, true, "blia");
}

ll BottomLeft::graspBlds(int bufferSize, function<bool(const Rectangle &a, const Rectangle &b)> sortingFunction, string name) {
    openLog(name + "_" + to_string(bufferSize));
    startTimer();

    vector<int> ord = getOrder();
    vector<Rectangle> bestRect;
    bestHeight = LL_INF;
    while(true) {
        vector<Rectangle> rr = rect;
        sort(rr.begin(), rr.end(), sortingFunction);
        int n = rr.size();
        for(int i=0; i < n; i++) {
            int a = min(n-1, i+ri(0, bufferSize-1));
            swap(rr[i], rr[a]);
        }
        ll currentHeight = bl(rr, ord);
        if(currentHeight < bestHeight) {
            bestHeight = currentHeight;
            bestRect = rr;
            record();
        }
        ll ti = getTime();
        if(ti > maxTime) {
            break;
        }
    }
    bl(bestRect, ord);
    saveImg(name, bestHeight);
    return bestHeight;
}

ll BottomLeft::graspBldw(int bufferSize) {
    return graspBlds(bufferSize, Rectangle::cmpByWidth, "graspBldw");
}

ll BottomLeft::graspBldh(int bufferSize) {
    return graspBlds(bufferSize, Rectangle::cmpByHeight, "graspBldh");
}

ll BottomLeft::graspBlda(int bufferSize) {
    return graspBlds(bufferSize, Rectangle::cmpByHeight, "graspBlda");
}

ll BottomLeft::hillClimber() {
    openLog("BLhillClimber");
    startTimer();
    
    vector<int> ord = getRandomOrder();
    vector<pll> placement;
    bestHeight = bl(rect, ord);
    record();
    int iter=0;
    while(true) {
        iter++;
        vector<int> bneigh = getRandomNeighbour(ord);
        ll currentHeight = bl(rect, bneigh);
        for(int a=0; a < (int)ord.size(); a++) {
            for(int b=a+1; b < (int)ord.size(); b++) {
                for(int t=0; t < 2; t++) {
                    vector<int> neigh = ord;
                    if(t == 0) {
                        swapElements(a, b, neigh);
                    } else {
                        reverseSequence(a, b, neigh);
                    }
                    ll h = bl(rect, neigh);
                    if(h < currentHeight) {
                        currentHeight = h;
                        bneigh = neigh;
                    }
                    ll ti = getTime();
                    if(ti > maxTime) {
                        t=2;
                        a = ord.size();
                        b = ord.size();
                        break;
                    }
                }
            }
        }
        if(currentHeight >= bestHeight) {
            break;
        }      
        ord = bneigh;
        bestHeight = currentHeight;
        record();
    }
    //cout << iter << "\n";
    bl(rect, ord);
    saveImg("hillClimber", bestHeight);
    return bestHeight;
}

ll BottomLeft::simulatedAnnealing(double mult0, int maxNumberOfAccepted, int maxNumberOfIterations) {
    openLog("BLsa_" + to_string(mult0) + "_" + to_string(maxNumberOfAccepted) + "_" + to_string(maxNumberOfIterations));
    startTimer();

    vector<int> ord = getRandomOrder();
    ll currentHeight = bl(rect, ord);
    vector<int> best = ord;
    bestHeight = currentHeight;
    record();
    ll initialHeight = currentHeight;

    double T = 1000.0;
    double mult = mult0;

    int iter=0;
    int numberOfAccepted=0;
    int numberOfIterations=0;
    while(true) {
        iter++;
        vector<int> neigh = getRandomNeighbour(ord);
        ll h = bl(rect, neigh);
        double df = (h-currentHeight)/double(initialHeight);
        numberOfIterations++;
        if(df < 0 || exp(-df/T) > rd(0, 1)) {
            if(h != currentHeight) {
                numberOfAccepted++;
            }
            ord = neigh;
            currentHeight = h;
        }
        if(currentHeight < bestHeight) {
            best = ord;
            bestHeight = currentHeight;
            record();
        }
        if(numberOfAccepted >= maxNumberOfAccepted) {
            T*=mult;
            numberOfAccepted=0;
            numberOfIterations=0;
        }
        if(numberOfIterations >= maxNumberOfIterations) {
            T/=mult;
            numberOfIterations=0;
            numberOfAccepted=0;
        }
        ll ti = getTime();
        if(ti > maxTime) {
            break;
        }
    }
    bl(rect, best);
    saveImg("simulatedAnnealing", bestHeight);
    return bestHeight;
}

ll BottomLeft::multiStartLocalSearch(int numberOfLists, int numberOfNeighbours) {
    openLog("BLmls_" + to_string(numberOfLists) + "_" + to_string(numberOfNeighbours));
    startTimer();
    vector<int> *ord = new vector<int> [numberOfLists];
    ll *currentHeight = new ll[numberOfLists];
    bestHeight = LL_INF;
    ll bestId = 0;
    for(int i=0; i < numberOfLists; i++) {
        ord[i] = getRandomOrder();
        currentHeight[i] = bl(rect, ord[i]);
    }
    int iter=0;
    while(true) {
        iter++;
        for(int i=0; i < numberOfLists; i++) {
            vector<int> bneigh = getRandomNeighbour(ord[i]);
            ll candidateHeight = bl(rect, bneigh);
            for(int j=0; j < numberOfNeighbours; j++) {
                vector<int> neigh = getRandomNeighbour(ord[i]);
                ll h = bl(rect, neigh);
                if(h < candidateHeight) {
                    candidateHeight = h;
                    bneigh = neigh;
                }
                if(getTime() > maxTime) {
                    break;
                }
            }
            if(candidateHeight <= currentHeight[i]) {
                ord[i] = bneigh;
                currentHeight[i] = candidateHeight;
                if(candidateHeight < bestHeight) {
                    bestHeight = candidateHeight;
                    bestId = i;
                    record();
                }
            }
            if(getTime() > maxTime) {
                break;
            }
        }
        ll ti = getTime();
        if(ti > maxTime) {
            break;
        }
    }
    bl(rect, ord[bestId]);
    saveImg("multiStartLocalSearch", bestHeight);
    delete [] ord;
    delete [] currentHeight;
    return bestHeight;
}

ll BottomLeft::iteratedLocalSearch() {
    openLog("BLils");
    startTimer();

    vector<int> ord = getRandomOrder();
    int n = ord.size();
    ll currentHeight = bl(rect, ord);
    vector<int> best = ord;
    bestHeight = currentHeight;
    record();
    int numberOfNeighbours = 400;
    int numberOfReversals = 2;
    int reversalMaxSize = max(2, n/20);
    int iter=0;
    while(true) {
        iter++;
        vector<int> bneigh = getRandomNeighbour(ord);
        ll candidateHeight = bl(rect, bneigh);
        for(int i=0; i < numberOfNeighbours; i++) {
            vector<int> neigh = getRandomNeighbour(ord);
            ll h = bl(rect, neigh);
            if(h < candidateHeight) {
                candidateHeight = h;
                bneigh = neigh;
            }
        }
        if(candidateHeight >= currentHeight) { //perturbate
            ord = best;
            for(int i=0; i < numberOfReversals; i++) {
                int a = ri(0, n-1);
                int b = ri(1, reversalMaxSize);
                b = min(a+b, n-1);
                b = ri(0, n-1);
                swapElements(a, b, ord);
            }
            currentHeight = bl(rect, ord);
        } else {
            currentHeight = candidateHeight;
            ord = bneigh;
            if(candidateHeight < bestHeight) {
                bestHeight = candidateHeight;
                best = bneigh;
                record();
            }
        }
        ll ti = getTime();
        if(ti > maxTime) {
            break;
        }
    }
    //cerr << iter << "\n";
    bl(rect, best);
    saveImg("iteratedLocalSearch", bestHeight);
    return bestHeight;
}

ll BottomLeft::getHash(vector<int> &ord) {
    ll p = 2137;
    ll res=0;
    ll x = 1;
    for(int a : ord) {
        res+=a*x;
        x*=p;
    }
    return res;
}

void BottomLeft::addToTabu(vector<int> &ord, deque<ll> &TabuList, unordered_map<ll, int> &TabuMap) {
    ll h = getHash(ord);
    addToTabu(h, TabuList, TabuMap);
}

void BottomLeft::addToTabu(ll h, deque<ll> &TabuList, unordered_map<ll, int> &TabuMap) {
    if(TabuMap.count(h)) {
        TabuMap[h]++;
    } else {
        TabuMap[h] = 1;
    }
    TabuList.push_back(h);
}

void BottomLeft::removeFromTabu(deque<ll> &TabuList, unordered_map<ll, int> &TabuMap) {
    ll h = TabuList.front();
    TabuList.pop_front();
    if(TabuMap[h] > 1) {
        TabuMap[h]--;
    } else {
        TabuMap.erase(h);
    }
}

ll BottomLeft::tabuSearch(int tabuLength, int numberOfNeighbours) {
    openLog("BLts_" + to_string(tabuLength) + "_" + to_string(numberOfNeighbours));
    startTimer();

    vector<int> ord = getRandomOrder();
    ll currentHeight = bl(rect, ord);
    vector<int> best = ord;
    bestHeight = currentHeight;
    record();
    deque<ll> TabuList;
    unordered_map<ll, int> TabuMap;
    addToTabu(ord, TabuList, TabuMap);
    int iter=0;
    while(true) {
        iter++;
        vector<int> bneigh = {};
        ll candidateHeight = LL_INF;
        for(int i=0; i < numberOfNeighbours; i++) {
            vector<int> neigh = getRandomNeighbour(ord);
            ll nhash = getHash(neigh);
            if(TabuMap.count(nhash)) {
                continue;
            }
            ll h = bl(rect, neigh);
            if(h < candidateHeight) {
                candidateHeight = h;
                bneigh = neigh;
            }
        }
        if(bneigh.size() > 0) {
            ord = bneigh;
            currentHeight = candidateHeight;
            if(candidateHeight < bestHeight) {
                bestHeight = candidateHeight;
                best = bneigh;
                record();
            }
            addToTabu(ord, TabuList, TabuMap);
        } else {
            //cerr << "ERR ";
        }
        if((int)TabuList.size() > tabuLength) {
            removeFromTabu(TabuList, TabuMap);
        }
        ll ti = getTime();
        if(ti > maxTime) {
            break;
        }
        // cerr << ti << "\n";
        // cerr << numberOfNeighbours << "\n";
    }
    // cerr << iter << "\n";
    bl(rect, best);
    saveImg("tabuSearch", bestHeight);
    return bestHeight;
}

ll BottomLeft::tabuSearch2(int tabuLength, int numberOfNeighbours) {
    openLog("BLts2_" + to_string(tabuLength) + "_" + to_string(numberOfNeighbours));
    startTimer();

    vector<int> ord = getRandomOrder();
    int n = ord.size();
    ll currentHeight = bl(rect, ord);
    vector<int> best = ord;
    bestHeight = currentHeight;
    record();
    deque<ll> TabuList;
    unordered_map<ll, int> TabuMap;
    int iter=0;
    while(true) {
        iter++;
        vector<int> bneigh = {};
        ll candidateHeight = LL_INF;
        ll candidateHashValue = 0;
        for(int i=0; i < numberOfNeighbours; i++) {
            vector<int> neigh = ord;
            ll a=ri(0, n-1), b=ri(0, n-1), c=ri(0, 1);
            ll hashValue = c*n*n + b*n + a;
            if(TabuMap.count(hashValue) || TabuMap.count(c*n*n+a*n+b)) {
                continue;
            }
            if(c) {
                swapElements(a, b, neigh);  
            } else {
                reverseSequence(a, b, neigh);
            }
            ll h = bl(rect, neigh);
            if(h < candidateHeight) {
                candidateHeight = h;
                bneigh = neigh;
                candidateHashValue = hashValue;
            }
        }
        if(bneigh.size() > 0) {
            ord = bneigh;
            currentHeight = candidateHeight;
            if(candidateHeight < bestHeight) {
                bestHeight = candidateHeight;
                best = bneigh;
                record();
            }
            addToTabu(candidateHashValue, TabuList, TabuMap);
        } else {
            //cerr << "ERR ";
        }
        if((int)TabuList.size() > tabuLength) {
            removeFromTabu(TabuList, TabuMap);
        }
        ll ti = getTime();
        if(ti > maxTime) {
            break;
        }
    }
    // cerr << iter << "\n";
    bl(rect, best);
    saveImg("tabuSearch2", bestHeight);
    return bestHeight;
}