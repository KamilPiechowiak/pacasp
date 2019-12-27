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
    Shelf::simulatedAnnealing2(0.606531, 10, 10000);
    Shelf::simulatedAnnealing2(0.899661, 1, 10000);
    Shelf::simulatedAnnealing2(0.977887, 10, 10000);

    BottomLeft::bldh();
    BottomLeft::bldw();
    BottomLeft::blda();
    BottomLeft::blih();
    BottomLeft::bliw();
    BottomLeft::blia();

    BottomLeft::graspBldh(5);
    BottomLeft::graspBldh(10);
    BottomLeft::graspBlda(5);
    BottomLeft::graspBldw(2);
    BottomLeft::graspBldw(5);
    BottomLeft::graspBldw(1000);
    
    BottomLeft::hillClimber();

    BottomLeft::simulatedAnnealing(0.606531, 1, 1000);
    BottomLeft::simulatedAnnealing(0.606531, 1, 10000);
    BottomLeft::simulatedAnnealing(0.899661, 3, 1000);
    BottomLeft::simulatedAnnealing(0.995282, 1, 10000);

    BottomLeft::multiStartLocalSearch(1, 1);
    BottomLeft::multiStartLocalSearch(1, 2);
    BottomLeft::multiStartLocalSearch(1, 5);

    BottomLeft::tabuSearch(2, 22);
    BottomLeft::tabuSearch(22, 46);
    BottomLeft::tabuSearch(100, 46);
    BottomLeft::tabuSearch(215, 215);
    
    BottomLeft::tabuSearch2(1, 46);
    BottomLeft::tabuSearch2(5, 46);
    BottomLeft::tabuSearch2(5, 100);
    BottomLeft::tabuSearch2(22, 22);

    destroy();
}