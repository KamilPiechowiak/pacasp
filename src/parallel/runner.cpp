#include"runner.hpp"
#include"algorithms/bottomLeft.hpp"
#include"algorithms/shelf.hpp"

Runner::Runner(ll maxTime) : Parallel(maxTime) {}

void Runner::generateInstance() {
    int seed = nodeId+1000;
    string name = "instances/" + to_string(nodeId) + ".in";
    string instr = "./gen " + to_string(seed) + " 0 > " + name;
    if(nodeId < 2000) {
        system(instr.c_str());
    }
    filename = name;
}

void Runner::run() {
    generateInstance();
    loadData();
    
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
    BottomLeft::tabuSearch(); //dynamic parameters
    BottomLeft::tabuSearch2(30, 13); //short time
    BottomLeft::tabuSearch2(30, 20); //long time

    destroy();
}