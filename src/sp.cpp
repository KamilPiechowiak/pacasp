#include"general/rectangle.hpp"
#include"general/imgSaver.hpp"
#include"algorithms/shelf.hpp"
#include"algorithms/bounds.hpp"
#include"algorithms/bottomLeft.hpp"
#include"parallel/runner.hpp"
#include"parallel/tuner.hpp"
#include"parallel/portfolio.hpp"

void showAll(ll w, vector<Rectangle> &rect, ll maxTime) {
    Recordable::setInstanceId(0);

    Shelf::init(w, rect, maxTime);
    BottomLeft::init(w, rect, maxTime);

    vector<ll> v = {
        BottomLeft::tabuSearch2(30, 115),
        BottomLeft::tabuSearch(30, 115),
        BottomLeft::simulatedAnnealing(0.993, 4, 400),
        Shelf::simulatedAnnealing2(0.8, 1, 45)
    };
    for(ll r : v) {
        cout << r << "\n";
    }
    exit(0);

    Shelf::init(w, rect, maxTime);
    for(int i=0; i < 4; i++) {
        for(int j=0; j < 3; j++) {
            for(int k=0; k < 2; k++) {
                Shelf::genericGreedy((ggOrder)i, (ggShelf)j, (bool)k);
            }
        }
    }
    Shelf::hillClimber();
    Shelf::simulatedAnnealing2(0.8, 1, 45);
    Shelf::simulatedAnnealing2(0.999, 3, 17);

    BottomLeft::init(w, rect, maxTime);
    BottomLeft::bldw();
    BottomLeft::graspBldw(10);
    BottomLeft::graspBldw(100);
    BottomLeft::graspBldw(1000);
    BottomLeft::bldh();
    BottomLeft::graspBldh(10);
    BottomLeft::graspBldh(100);
    BottomLeft::graspBldh(1000);
    BottomLeft::hillClimber();
    BottomLeft::simulatedAnnealing(0.8, 2, 3500); //bigger instances, smaller limits
    BottomLeft::simulatedAnnealing(0.993, 4, 400);
    BottomLeft::multiStartLocalSearch();
    BottomLeft::iteratedLocalSearch();
    BottomLeft::tabuSearch(30, 55); //small instances, short time
    BottomLeft::tabuSearch(30, 145); //small instances, long time
    BottomLeft::tabuSearch(30, 35); //big instances, short time
    BottomLeft::tabuSearch(30, 115); //big instances, long time
    BottomLeft::tabuSearch2(30, 13); //short time
    BottomLeft::tabuSearch2(30, 20); //long time
}

void localMinCost() {
    system("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/ibm/ILOG/CPLEX_Studio128/opl/bin/x86-64_linux");
    for(int i=1; i <= 500; i++) {
        Portfolio p(i);
        p.run();
        cerr << "\r" << i;
    }
    cerr << "\n";
}

void defaultProcessing() {
    vector<Rectangle> rect;
    int n;
    ll w;
    cin >> n >> w;
    ImgSaver::init(n);
    for(int i=0; i < n; i++) {
        ll a, b;
        cin >> a >> b;
        rect.push_back(Rectangle(a, b, i));
    }
    showAll(w, rect, 1000);
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    string option = "default";
    if(argc == 2) {
        option = string(argv[1]);
    }
    if(option == "portfolio") {
        localMinCost();
    } else if(option == "run") {
        Runner r(1000);
        r.run();
    } else if(option == "tune") {
        Tuner t(1000*60*4);
        t.run();
    } else {
        defaultProcessing();
    }
}