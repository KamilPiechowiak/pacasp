#include"runner.hpp"
#include"algorithms/bottomLeft.hpp"
#include"algorithms/shelf.hpp"
#include"algorithms/bounds.hpp"
#include"algorithms/skyline.hpp"

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
    Recorder recorder("out", nodeId, maxTime);

    Bounds bounds(w, rect, recorder);
    BottomLeft bottom_left(w, rect, recorder);
    Shelf shelf(w, rect, recorder);
    Skyline skyline(w, rect, recorder);
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
    shelf.simulatedAnnealing2(0.606531, 10, 10000);
    shelf.simulatedAnnealing2(0.899661, 1, 10000);
    shelf.simulatedAnnealing2(0.977887, 10, 10000);

    bottom_left.bldh();
    bottom_left.bldw();
    bottom_left.blda();
    bottom_left.blih();
    bottom_left.bliw();
    bottom_left.blia();

    bottom_left.graspBldh(5);
    bottom_left.graspBldh(10);
    bottom_left.graspBlda(5);
    bottom_left.graspBldw(2);
    bottom_left.graspBldw(5);
    bottom_left.graspBldw(1000);
    
    bottom_left.hillClimber();

    bottom_left.simulatedAnnealing(0.606531, 1, 1000);
    bottom_left.simulatedAnnealing(0.606531, 1, 10000);
    bottom_left.simulatedAnnealing(0.899661, 3, 1000);
    bottom_left.simulatedAnnealing(0.995282, 1, 10000);

    // bottom_left.multiStartLocalSearch(1, 1);
    // bottom_left.multiStartLocalSearch(1, 2);
    // bottom_left.multiStartLocalSearch(1, 5);

    bottom_left.tabuSearch(true, 2, 22);
    bottom_left.tabuSearch(true, 22, 46);
    bottom_left.tabuSearch(true, 100, 46);
    bottom_left.tabuSearch(true, 215, 215);
    
    bottom_left.tabuSearch(false, 1, 46);
    bottom_left.tabuSearch(false, 5, 46);
    bottom_left.tabuSearch(false, 5, 100);
    bottom_left.tabuSearch(false, 22, 22);

    destroy();
}