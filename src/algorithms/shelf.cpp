#include"shelf.hpp"
#include"general/placementSaver.hpp"

Shelf::Shelf(ll w, vector<Rectangle> &rr, Recorder &recorder) : w(w), rr(rr), recorder(recorder) {
    placement.resize(SIZE(rr));
    pref="SH-";
}

void Shelf::saveImg(string name, ll height, vector<Rectangle> &rect) {
    ImgSaver::saveImg(pref+name, w, height, rect, placement);
    PlacementSaver placementSaver;
    placementSaver.savePlacement(name, w, height, rect, placement);
}

ll Shelf::nfdh() {
    vector<Rectangle> rect = rr;
    sort(rect.begin(), rect.end(), Rectangle::cmpByHeight);
    ll currentHeight=0;
    ll widthLeft=0;
    ll stripHeight=0;
    for(Rectangle &r : rect) {
        if(widthLeft < r.width) { //new row
            currentHeight+= stripHeight;
            placement[r.id] = {0, currentHeight};
            widthLeft = w-r.width;
            stripHeight = r.height;
        } else {
            placement[r.id] = {w-widthLeft, currentHeight};
            widthLeft-= r.width;
        }
    }
    currentHeight+=stripHeight;
    saveImg("nfdh", currentHeight, rect);
    return currentHeight;
}

ll Shelf::bfdh() {
    vector<Rectangle> rect = rr;
    sort(rect.begin(), rect.end(), Rectangle::cmpByHeight);
    set<pll> s; //how much space left, position
    ll currentHeight = 0;
    for(Rectangle &r : rect) {
        auto it = s.upper_bound({r.width, -1});
        ll left=w;
        ll pos=currentHeight;
        if(it != s.end()) {
            left = (*it).first;
            pos = (*it).second;
            s.erase(it);
        } else {
            currentHeight+=r.height;
        }
        placement[r.id] = {w-left, pos};
        left-=r.width;
        s.insert({left, pos});
    }
    saveImg("bfdh", currentHeight, rect);
    return currentHeight;
}

ll Shelf::genericGreedy(ggOrder order, ggShelf shelf, bool rotateShelves) {
    vector<string> orderString = {
        "heightAsc", "heightDsc", "widthAsc", "widthDsc"
    };
    vector<string> shelfString = {
       "bestFit", "worstFit", "nextFit" 
    };
    string name = "gg_" + orderString[order] + "_" + shelfString[shelf];
    if(rotateShelves) {
        name+= "_rotated";
    }
    recorder.open_log("SH" + name);
    vector<Rectangle> rect = rr;
    function<bool(const Rectangle&, const Rectangle&)> f;
    if(order == heightAsc) {
        f = [](const Rectangle& a, const Rectangle& b) {
            return a.height < b.height;
        };
    } else if(order == heightDsc) {
        f = [](const Rectangle& a, const Rectangle& b) {
            return a.height > b.height;
        };
    } else if(order == widthAsc) {
        f = [](const Rectangle& a, const Rectangle& b) {
            return a.width < b.width;
        };
    } else if(order == widthDsc) {
        f = [](const Rectangle& a, const Rectangle& b) {
            return a.width > b.width;
        };
    }
    sort(rect.begin(), rect.end(), f);
    vector<vector<int>> shelves;
    set<pll> s; //how much space left, position
    ll spaceLeft = 0;
    for(int i=0; i < (int)rect.size(); i++) {
        Rectangle &r = rect[i];
        if(shelf == bestFit || shelf == worstFit) {            
            auto it = s.upper_bound({r.width, -1});
            if(shelf == worstFit) {
                if(it != s.end()) {
                    it = s.end();
                    it--;
                }
            }
            int id;
            ll left;
            if(it == s.end()) {
                id = shelves.size();
                left = w;
                shelves.push_back({});
            } else {
                id = (*it).second;
                left = (*it).first;
                s.erase(it);
            }
            left-=r.width;
            shelves[id].push_back(i);
            s.insert({left, id});     
        } else if(shelf == nextFit) {
            if(spaceLeft < r.width) {
                spaceLeft = w;
                shelves.push_back({});
            }
            shelves.back().push_back(i);
            spaceLeft-=r.width;
        }
    }
    ll currentHeight=0;
    if(rotateShelves == false) {
        for(vector<int> &sh : shelves) {
            ll maxHeight = 0;
            ll currentWidth = 0;
            for(int a : sh) {
                Rectangle &r = rect[a];
                placement[r.id] = {currentWidth, currentHeight};
                currentWidth+=r.width;
                maxHeight = max(maxHeight, rect[a].height);
            }
            currentHeight+=maxHeight;
        }
    } else {
        name+= "_rotated";
        int m = shelves.size();
        if(m%2 == 1) {
            shelves.push_back({});
        }
        ShelfCache shelfCache(w, rect, shelves, false);
        for(int i=0; i < (int)shelves.size(); i++) {
            ll shelfWidth = shelfCache.findWidth(shelves[i], rect);
            int id = rect.size();
            shelves[i].push_back(id);
            rect.push_back(Rectangle(w-shelfWidth, 0, id));
        }
        placement.resize(rect.size());
        for(int i=0; i < (int)shelves.size(); i+=2) {
            ll shelfHeight = shelfCache.matchLevels(shelves[i], shelves[i+1], rect);
            ll currentWidth = 0;
            for(int a=0; a < (int)shelves[i].size(); a++) {
                Rectangle &r = rect[shelves[i][a]];
                placement[r.id] = {currentWidth, currentHeight};
                currentWidth+=r.width;
            }
            currentWidth = 0;
            currentHeight+=shelfHeight;
            for(int b=shelves[i+1].size()-1; b >= 0; b--) {
                Rectangle &r = rect[shelves[i+1][b]];
                placement[r.id] = {currentWidth, currentHeight-r.height};
                currentWidth+=r.width;
            }
        }
        for(int i=0; i < (int)shelves.size(); i++) {
            rect.pop_back();
        }
    }
    ll bestHeight = currentHeight;
    recorder.record(bestHeight);
    saveImg(recorder.get_filename(), currentHeight, rect);
    return currentHeight;
}

vector<vector<int>> Shelf::getBFDHLevels(vector<Rectangle> rect) {
    sort(rect.begin(), rect.end(), Rectangle::cmpByHeight);
    set<pll> s; //how much space left, level's id
    vector<vector<int>> levels;
    for(Rectangle &r : rect) {
        auto it = s.upper_bound({r.width, -1});
        ll left=w;
        ll id=levels.size();
        if(it != s.end()) {
            left = (*it).first;
            id = (*it).second;
            s.erase(it);
        } else {
            levels.push_back({});
        }
        levels[id].push_back(r.id);
        left-=r.width;
        s.insert({left, id});
    }
    if(levels.size()%2 == 1) {
        levels.push_back({});
    }
    return levels;
}

pair<vector<int>, ll> Shelf::getOrder(ShelfCache &shelfCache, int m) {
    vector<int> ord;
    for(int i=0; i < m; i++) {
        ord.push_back(i);
    }
    ll currentHeight = 0;
    for(int i=0; i < m; i+=2) {
        currentHeight+=shelfCache.getPairHeight(ord[i], ord[i+1]);
    }
    return {ord, currentHeight};
}

pair<pii, ll> Shelf::getNeighbour(int a, int b, vector<int> &ord, ShelfCache &shelfCache) {
    int x = (a/2)*2;
    int y = (b/2)*2;
    if(x == y) {
        return {{a, b}, 0};
    }
    ll dh = -shelfCache.getPairHeight(ord[x], ord[x+1]) - shelfCache.getPairHeight(ord[y], ord[y+1]);
    if((a%2) == (b%2)) {
        dh+=shelfCache.getPairHeight(ord[x], ord[y+1]) + shelfCache.getPairHeight(ord[y], ord[x+1]);
    } else {
        dh+=shelfCache.getPairHeight(ord[x], ord[y]) + shelfCache.getPairHeight(ord[x+1], ord[y+1]);
    }
    return {{a, b}, dh};
}

pair<pii, ll> Shelf::getRandomNeighbour(vector<int> &ord, ShelfCache &shelfCache) {
    if(ord.size() < 3) {
        return {{0, 1}, 0};
    }
    int a = ri(0, ord.size()-1);
    int b = ri(0, ord.size()-3); //swap elements from different pairs
    if((a/2)*2 <= b) b+=2;
    return getNeighbour(a, b, ord, shelfCache);
}

void Shelf::draw(string name, vector<int> &ord, ShelfCache &shelfCache) {
    vector<vector<int>> levels = shelfCache.getLevels();
    vector<Rectangle> rect = shelfCache.getRect();
    placement.resize(rect.size());
    ll currentHeight=0;
    for(int i=0; i < (int)ord.size(); i+=2) {
        ll pos=0;
        for(int a : levels[ord[i]]) {
            Rectangle &r = rect[a];
            placement[r.id] = {pos, currentHeight};
            pos+=r.width;
        }
        currentHeight+=shelfCache.getPairHeight(ord[i], ord[i+1]);
        pos=0;
        reverse(levels[ord[i+1]].begin(), levels[ord[i+1]].end());
        for(int a : levels[ord[i+1]]) {
            Rectangle &r = rect[a];
            placement[r.id] = {pos, currentHeight-r.height};
            pos+=r.width;
        }
    }
    for(int i=0; i < (int)levels.size(); i++) {
        rect.pop_back(); //delete empty rectangles
    }
    saveImg(name, currentHeight, rect);
}

ll Shelf::hillClimber() {
    recorder.open_log("SHhillClimber");
    vector<Rectangle> rect = rr;
    vector<vector<int>> levels = getBFDHLevels(rect);
    ShelfCache shelfCache(w, rect, levels);
    auto x = getOrder(shelfCache, levels.size());
    vector<int> ord = x.first;
    ll bestHeight = x.second;
    recorder.record(bestHeight);
    while(true) {
        int dh=0;
        for(int i=0; i < (int)ord.size(); i+=2) {
            for(int j=i+2; j < (int)ord.size(); j++) {
                auto x = getNeighbour(i, j, ord, shelfCache);
                if(x.second < 0) {
                    dh+=x.second;
                    swap(ord[i], ord[j]);
                }
                if(recorder.should_finish()) {
                    i = ord.size();
                    j = ord.size();
                    break;
                }
            }
        }
        if(dh == 0) {
            break;
        }
        bestHeight+=dh;
        recorder.record(bestHeight);

    }
    draw(recorder.get_filename(), ord, shelfCache);
    return bestHeight;
}

ll Shelf::simulatedAnnealing() {
    auto tStart = chrono::system_clock::now();

    vector<Rectangle> rect = rr;
    vector<vector<int>> levels = getBFDHLevels(rect);
    ShelfCache shelfCache(w, rect, levels);
    auto x = getOrder(shelfCache, levels.size());
    vector<int> ord = x.first;
    ll currentHeight = x.second;
    vector<int> best = ord;
    ll bestHeight = currentHeight;

    double T = 1000.0*currentHeight;
    double mult = 0.9;
    int i=0;
    while(true) {
        auto x = getRandomNeighbour(ord, shelfCache);
        int df = x.second;
        int a = x.first.first;
        int b = x.first.second;
        if(df < 0 || exp(-(double)df/T) > rd(0, 1)) {
            swap(ord[a], ord[b]);
            currentHeight+=df;
        }
        if(currentHeight < bestHeight) {
            best = ord;
            bestHeight = currentHeight;
        }
        if(i%10 == 0) {
            if(recorder.should_finish()) {
                break;
            }
            T*=mult;
        }
        i++;
    }

    draw(recorder.get_filename(), best, shelfCache);
    return bestHeight;
}

ll Shelf::simulatedAnnealing2(double mult0, int maxNumberOfAccepted, int maxNumberOfIterations) {
    recorder.open_log("SHsa_" + to_string(mult0) + "_" + to_string(maxNumberOfAccepted) + "_" + to_string(maxNumberOfIterations));
    
    vector<Rectangle> rect = rr;
    vector<vector<int>> levels = getBFDHLevels(rect);
    ShelfCache shelfCache(w, rect, levels);
    auto x = getOrder(shelfCache, levels.size());
    vector<int> ord = x.first;
    ll currentHeight = x.second;
    vector<int> best = ord;
    ll bestHeight = currentHeight;
    recorder.record(bestHeight);
    ll initialHeight = currentHeight;

    double T = 1000.0;
    double mult = mult0;
    int numberOfAccepted = 0;
    int numberOfIterations = 0;
    int i=0;
    while(true) {
        auto x = getRandomNeighbour(ord, shelfCache);
        ll df = x.second;
        int a = x.first.first;
        int b = x.first.second;
        if(df < 0 || exp(-(double)df/(T*initialHeight)) > rd(0, 1)) {
            swap(ord[a], ord[b]);
            currentHeight+=df;
            if(df != 0) {
                numberOfAccepted++;
            }
        }
        numberOfIterations++;
        if(currentHeight < bestHeight) {
            best = ord;
            bestHeight = currentHeight;
            recorder.record(bestHeight);
        }
        if(numberOfAccepted >= maxNumberOfAccepted) {
            // cerr << "DEC\n";
            T*=mult;
            numberOfAccepted=0;
            numberOfIterations=0;
        }
        if(numberOfIterations >= maxNumberOfIterations) {
            // cerr << "INC\n";
            T/=mult;
            numberOfIterations=0;
            numberOfAccepted=0;
        }
        if(i%10 == 0) {
            if(recorder.should_finish()) {
                break;
            }
        }
        i++;
        // cerr << currentHeight << "\n";
        // cerr << T << "\n";
    }

    draw(recorder.get_filename(), best, shelfCache);
    return bestHeight;
}