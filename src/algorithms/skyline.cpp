#include"algorithms/skyline.hpp"
#include"algorithms/skyline/burkeScoreRule.hpp"
#include"algorithms/skyline/ishScoreRule.hpp"
#include"algorithms/skyline/ishBruteScoreRule.hpp"

ll Skyline::w;
vector<Rectangle> Skyline::rect;
string Skyline::pref;
vector<pll> Skyline::placement;
ll Skyline::maxTime;

void Skyline::init(ll _w, vector<Rectangle> &_rect, ll _maxTime) {
    w = _w;
    rect = _rect;
    int n = rect.size();
    placement.resize(n);
    pref = "SK-";
    maxTime = 1000000LL*_maxTime;//in nanoseconds
}

void Skyline::saveImg(string name, ll height) {
    ImgSaver::saveImg(pref+name, w, height, rect, placement);
}

vector<int> Skyline::getOrder() {
    int n = (int)rect.size();
    vector<int> ord;
    for(int i=0; i < n; i++) {
        ord.push_back(i);
    }
    return ord;
}

ll Skyline::bestFit(vector<Rectangle> &rect, vector<int> &ord, ScoreRule *scoreRule) {
    scoreRule->init(rect, ord);
    // ISHScoreRule second_score_rule;
    // second_score_rule.init(rect, ord);
    list<SkylineSegment> skyline;
    skyline.push_back(SkylineSegment(0, w-1, 0));
    typedef tuple<ll, ll, list<SkylineSegment>::iterator> SegmentData;
    auto cmp = [](const SegmentData &a, const SegmentData &b) -> bool {
        return pll(get<0>(a), get<1>(a)) < pll(get<0>(b), get<1>(b));
    };
    set<SegmentData, function<bool (const SegmentData&, const SegmentData&)>> lowest_segment(cmp); //y, x, iterator
    lowest_segment.insert({0, 0, skyline.begin()});
    ll totalHeight = 0;
    // for(auto a : skyline) {
    //     fprintf(stderr, "%lld,%lld\n", a.left, a.right);
    // }
    int iter=0;
    while(true) {
        auto it = get<2>(*lowest_segment.begin());
        lowest_segment.erase(lowest_segment.begin());
        SkylineSegment selected_segment = *it;

        if(it != skyline.begin()) {
            it--;
            selected_segment.left_height = it->height;
            it++;
        }
        it++;
        if(it != skyline.end()) {
            selected_segment.right_height = it->height;
        }
        it--;
        auto rectangle_placement = scoreRule->next(selected_segment);
        // auto second_rectangle_placement = second_score_rule.next(selected_segment);
        // if(rectangle_placement.any) {
        //     assert(rectangle_placement.id == second_rectangle_placement.id);
        // }
        // if(rectangle_placement.any && rectangle_placement.id != second_rectangle_placement.id) {
        //     fprintf(stderr, "error at %d: %d %d\n", iter, rectangle_placement.id, second_rectangle_placement.id);
        //     fprintf(stderr, "%lld %lld\n", rect[rectangle_placement.id].width, rect[rectangle_placement.id].height);
        //     fprintf(stderr, "%lld %lld\n", rect[second_rectangle_placement.id].width, rect[second_rectangle_placement.id].height);
        //     fprintf(stderr, "segment: %lld %lld %lld\n\n", selected_segment.right-selected_segment.left+1, selected_segment.left_height-selected_segment.height, selected_segment.right_height-selected_segment.height);
        // }
        // fprintf(stderr, "%d %d %d\n", rectangle_placement.any, rectangle_placement.left, rectangle_placement.id);
        if(rectangle_placement.all_placed) {
            break;
        } else if(!rectangle_placement.any) {
            if(selected_segment.left_height == selected_segment.right_height) {
                it++;
                ll current_right = it->right;
                lowest_segment.erase({it->height, it->left, it});
                skyline.erase(it--);
                skyline.erase(it--);
                it->right = current_right;
            } else if(selected_segment.left_height < selected_segment.right_height) {
                ll current_right = it->right;
                skyline.erase(it--);
                it->right = current_right;
            } else {
                ll current_left = it->left;
                skyline.erase(it++);
                lowest_segment.erase({it->height, it->left, it});
                it->left = current_left;
                lowest_segment.insert({it->height, it->left, it});
            }
        } else {
            // fprintf(stderr, "%lld,%lld,%lld", it->left, it->right, it->height);
            // fprintf(stderr, "%lld <= %lld\n", rect[rectangle_placement.id].width, selected_segment.right-selected_segment.left+1);
            iter++;
            assert(rect[rectangle_placement.id].width <= selected_segment.right-selected_segment.left+1);
            Rectangle r = rect[rectangle_placement.id];
            if(rectangle_placement.left) {
                placement[r.id] = {it->left, it->height};
            } else {
                placement[r.id] = {it->right-r.width+1, it->height};
            }
            // fprintf(stderr, "\n%lld,%lld\n", r.height, r.width);
            totalHeight = max(totalHeight, r.height+it->height);
            if(r.width == it->right-it->left+1) {
                it->height+=r.height;
            } else if(rectangle_placement.left) {
                it->left+=r.width;
                lowest_segment.insert({it->height, it->left, it});
                it = skyline.insert(it, SkylineSegment(it->left-r.width, it->left-1, it->height+r.height));
            } else {
                it->right-=r.width;
                lowest_segment.insert({it->height, it->left, it});
                SkylineSegment new_segment = SkylineSegment(it->right+1, it->right+r.width, it->height+r.height);
                it++;
                it = skyline.insert(it, new_segment);
            }

            SkylineSegment &current_segment = *it;
            it++;
            if(it != skyline.end() && current_segment.height == it->height) {
                lowest_segment.erase({it->height, it->left, it});
                current_segment.right = it->right;
                skyline.erase(it--);
            } else {
                it--;
            }
            if(it != skyline.begin()) {
                it--;
                if(current_segment.height == it->height) {
                    it->right = current_segment.right;
                    it++;
                    skyline.erase(it);
                } else {
                    it++;
                    lowest_segment.insert({it->height, it->left, it});
                }
            } else {
                lowest_segment.insert({it->height, it->left, it});
            }
        }
        #if DEBUG
        assert(skyline.size() == lowest_segment.size());
        ll prev_x = -1, prev_height = -1;
        for(auto i = skyline.begin(); i != skyline.end(); i++) {
            // fprintf(stderr, "(%lld,%lld,%lld),", i->left,i->right,i->height);
            assert(prev_x+1 == i->left);
            prev_x = i->right;
            assert(prev_height != i->height);
            assert(lowest_segment.count({i->height, i->left, i}));
        }
        assert(prev_x+1 == w);
        // fprintf(stderr, "\n");
        #endif
    }
    // fprintf(stderr, "iter: %d\n", iter);
    return totalHeight;
}

ll Skyline::burke() {
    openLog("SKBurke");
    startTimer();
    
    auto ord = getOrder();
    BurkeScoreRule scoreRule;
    bestHeight = bestFit(rect, ord, &scoreRule);

    saveImg("Burke", bestHeight);
    return bestHeight;
}

vector<int> Skyline::getRandomNeighbourSwap(vector<int> ord) {
    int a = ri(0, SIZE(ord)-1), b = ri(0, SIZE(ord)-2);
    if(b>=a) b++;
    swap(ord[a], ord[b]);
    return ord;
}

ll Skyline::ish() {
    openLog("SKISH");
    startTimer();
    
    int n = SIZE(rect);
    auto ord = getOrder();
    vector<int> bestOrder;
    vector<function<bool(const int &a, const int &b)>> sorting_rules = {
        [&](const int &a, const int &b) {return rect[a].width*rect[a].height > rect[b].width*rect[b].height; },
        [&](const int &a, const int &b) {return rect[a].height > rect[b].height; },
        [&](const int &a, const int &b) {return rect[a].width > rect[b].width; },
        [&](const int &a, const int &b) {return perimeter(rect[a].width,rect[a].height) > perimeter(rect[b].width,rect[b].height); }
    };

    ISHScoreRule scoreRule;
    // ISHBruteScoreRule scoreRule;
    vector<pair<ll, int>> resulting_heights;
    bestHeight = LL_INF;
    for(int i=0; i < SIZE(sorting_rules); i++) {
        sort(ord.begin(), ord.end(), sorting_rules[i]);
        ll height = bestFit(rect, ord, &scoreRule);
        if(height < bestHeight) {
            bestHeight = height;
            bestOrder = ord;
            record();
        }
        resulting_heights.push_back({height, i});
        return bestHeight;
        // for(int id : ord) {
        //     printf("%d,",id);
        // }
        // printf("\n%lld,%d\n", height, i);
    }
    sort(resulting_heights.begin(), resulting_heights.end());
    while(true) {
        for(auto height_id : resulting_heights) {
            sort(ord.begin(), ord.end(), sorting_rules[height_id.second]);
            ll currentHeight = height_id.first;
            for(int i=0; i < n; i++) {
                auto neigh = getRandomNeighbourSwap(ord);
                ll height = bestFit(rect, neigh, &scoreRule);
                if(height < currentHeight) {
                    currentHeight = height;
                    ord = neigh;
                    if(height < bestHeight) {
                        bestHeight = height;
                        bestOrder = ord;
                        record();
                    }
                }
                if(getTime() > maxTime) {
                    goto endLoop;
                }
            }
        }
    }
    endLoop: saveImg("ISH", bestHeight);
    return bestHeight;
}

void Skyline::reverseSequence(int a, int b, vector<int> &ord) {
    if(a > b) swap(a, b);
    reverse(ord.begin()+a, ord.begin()+b);
}

void Skyline::swapElements(int a, int b, vector<int> &ord) {
    swap(ord[a], ord[b]);
}

void Skyline::addToTabu(ll h, deque<ll> &TabuList, unordered_map<ll, int> &TabuMap) {
    if(TabuMap.count(h)) {
        TabuMap[h]++;
    } else {
        TabuMap[h] = 1;
    }
    TabuList.push_back(h);
}

void Skyline::removeFromTabu(deque<ll> &TabuList, unordered_map<ll, int> &TabuMap) {
    ll h = TabuList.front();
    TabuList.pop_front();
    if(TabuMap[h] > 1) {
        TabuMap[h]--;
    } else {
        TabuMap.erase(h);
    }
}

ll Skyline::tabuSearch(int tabuLength, int numberOfNeighbours) {
    openLog("SKts_" + to_string(tabuLength) + "_" + to_string(numberOfNeighbours));
    startTimer();
    ISHScoreRule scoreRule;

    vector<int> ord = getOrder();
    // random_shuffle(ord.begin(), ord.end());
    sort(ord.begin(), ord.end(), [&](const int &a, const int &b) {return rect[a].width*rect[a].height > rect[b].width*rect[b].height; });
    int n = ord.size();
    ll currentHeight = bestFit(rect, ord, &scoreRule);
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
            ll a=ri(0, n-1), b=ri(0, n-1), c=ri(0, 1);
            ll hashValue = c*n*n + b*n + a;
            if(TabuMap.count(hashValue) || TabuMap.count(c*n*n+a*n+b)) {
                continue;
            }
            if(c) {
                swapElements(a, b, ord);  
            } else {
                reverseSequence(a, b, ord);
            }
            ll h = bestFit(rect, ord, &scoreRule);
            if(h < candidateHeight) {
                candidateHeight = h;
                bneigh = ord;
                candidateHashValue = hashValue;
            } else {
                if(c) {
                    swapElements(a, b, ord);  
                } else {
                    reverseSequence(a, b, ord);
                }
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
    bestFit(rect, best, &scoreRule);
    saveImg("tabuSearch2", bestHeight);
    return bestHeight;
}