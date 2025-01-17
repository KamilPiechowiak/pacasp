#include"general/rectangle.hpp"
#include"general/imgSaver.hpp"
#include"algorithms/shelf.hpp"
#include"algorithms/bounds.hpp"
#include"algorithms/bottomLeft.hpp"
#include"algorithms/skyline.hpp"
#include"parallel/runner.hpp"
#include"parallel/tuner.hpp"
#include"parallel/portfolio.hpp"
#include"parallel/validator.hpp"

void showAll(ll w, vector<Rectangle> &rect, ll maxTime) {
    Recorder recorder("out_local", 0, maxTime);

    Bounds bounds(w, rect, recorder);
    Shelf shelf(w, rect, recorder);
    BottomLeft bottom_left(w, rect, recorder);
    Skyline skyline(w, rect, recorder);

    cout << max(bounds.byArea(), bounds.byWiderThanHalf()) << "\n";\
    return;

    vector<ll> v = {
        bounds.byArea(),    
        bounds.byWiderThanHalf()                                                                                                                                                                                                                                                                                    ,
        // skyline.burke(),
        // skyline.ish(),
        // skyline.tabuSearch(true),
        // skyline.tabuSearch(false),
        // skyline.hillClimber(),
        // skyline.hillClimber(true),
        // skyline.simulatedAnnealing(),
        // skyline.multiStartLocalSearch(),
        // skyline.iteratedLocalSearch(),
        // bottom_left.tabuSearch(30, 55),
        // bottom_left.hillClimber(),
        // bottom_left.hillClimber(true),
        // bottom_left.simulatedAnnealing(),
        // bottom_left.tabuSearch(true),
        // bottom_left.tabuSearch(false),
        // bottom_left.iteratedLocalSearch(),
        // bottom_left.multiStartLocalSearch(),
        // shelf.simulatedAnnealing2(0.8, 1, 45)
    };
    for(ll r : v) {
        cout << r << "\n";
    }
    
    return;

    bounds.byArea();
    bounds.byWiderThanHalf();

    for(int i=0; i < 4; i++) {
        for(int j=0; j < 3; j++) {
            for(int k=0; k < 2; k++) {
                shelf.genericGreedy((ggOrder)i, (ggShelf)j, (bool)k);
            }
        }
    }
    shelf.hillClimber();
    shelf.simulatedAnnealing2(0.8, 1, 45);
    shelf.simulatedAnnealing2(0.999, 3, 17);

    bottom_left.bldw();
    bottom_left.graspBldw(10);
    bottom_left.graspBldw(100);
    bottom_left.graspBldw(1000);
    bottom_left.bldh();
    bottom_left.graspBldh(10);
    bottom_left.graspBldh(100);
    bottom_left.graspBldh(1000);
    bottom_left.hillClimber();
    bottom_left.simulatedAnnealing(0.8, 2, 3500); //bigger instances, smaller limits
    bottom_left.simulatedAnnealing(0.993, 4, 400);
    bottom_left.multiStartLocalSearch();
    bottom_left.iteratedLocalSearch();
    bottom_left.tabuSearch(true, 30, 55); //small instances, short time
    bottom_left.tabuSearch(true, 30, 145); //small instances, long time
    bottom_left.tabuSearch(true, 30, 35); //big instances, short time
    bottom_left.tabuSearch(true, 30, 115); //big instances, long time
    bottom_left.tabuSearch(false, 30, 13); //short time
    bottom_left.tabuSearch(false, 30, 20); //long time
}

void computePortfolio(vector<pll> instances) {
    for(int i=1; i <= 500; i++) {
        Portfolio p(i, instances);
        p.run();
        cerr << "\r" << i;
    }
    cerr << "\n";
}

void localMinCost() {
    // system("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/ibm/ILOG/CPLEX_Studio128/opl/bin/x86-64_linux");
    // computePortfolio({{0, 999}});
    // computePortfolio({{300, 699}});
    // computePortfolio({{400, 799}});
    // computePortfolio({{500, 999}});
    // computePortfolio({{700, 999}});
    // computePortfolio({{1000, 1999}});
    computePortfolio({{3094, 3263}});
    computePortfolio({{3215, 3263}});
}

void defaultProcessing() {
    vector<Rectangle> rect;
    int n;
    ll w;
    ll lb;
    cin >> w >> lb >> n;
    // cin >> n >> w;
    ImgSaver::init(n);
    for(int i=0; i < n; i++) {
        ll a, b;
        cin >> a >> b;
        rect.push_back(Rectangle(a, b, i));
    }
    showAll(w, rect, 2000);
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    setSeed(13);
    string option = "default";
    if(argc >= 2) {
        option = string(argv[1]);
    }
    json config;
    if(argc >= 3) {
        ifstream file(argv[2]);
        file >> config;
        file.close();
    }
    if(option == "portfolio") {
        localMinCost();
    } else if(option == "run") {
        Runner r(config["max_time"]);
        r.run(config);
    } else if(option == "tune") {
        Tuner t(config["max_time"]);
        t.run(config);
    } else if(option == "validate") {
        Validator v(config["max_time"]);
        v.run(config);
    } else {
        defaultProcessing();
    }
}