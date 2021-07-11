#include"runner.hpp"


#include"algorithms/bottomLeft.hpp"
#include"algorithms/shelf.hpp"
#include"algorithms/bounds.hpp"
#include"algorithms/skyline.hpp"


Runner::Runner(ll maxTime) : Parallel(maxTime) {}

vector<string> split(string s, char split_on) {
    vector<string> res;
    string acc;
    for(char c : s) {
        if(c == split_on) {
            if(acc != "") {
                res.push_back(acc);
                acc = "";
            }
        } else {
            acc+= c;
        }
    }
    if(acc != "") {
        res.push_back(acc);
    }
    return res;
}

void Runner::run(json config) {
    int instance_id = stoi((string)config[to_string(nodeId)]["instance_id"]);
    filename =  "instances/" + to_string(instance_id) + ".in";  
    loadData();
    Recorder recorder(config["output_path"], instance_id, maxTime);

    Bounds bounds(w, rect, recorder);
    BottomLeft bottom_left(w, rect, recorder);
    Shelf shelf(w, rect, recorder);
    Skyline skyline(w, rect, recorder);
    for(string name : config[to_string(nodeId)]["algorithms"]) {
        vector<string> arr = split(name, '_');
        string pref = name.substr(0, 2);
        if(name == "BLblda") bottom_left.blda();
        else if(name == "BLbldh") bottom_left.bldh();
        else if(name == "BLbldw") bottom_left.bldw();
        else if(name == "BLblia") bottom_left.blia();
        else if(name == "BLblih") bottom_left.blih();
        else if(name == "BLbliw") bottom_left.bliw();
        else if(arr[0] == "BLgraspBlda") bottom_left.graspBlda(stoi(arr[1]));
        else if(arr[0] == "BLgraspBldh") bottom_left.graspBldh(stoi(arr[1]));
        else if(arr[0] == "BLgraspBldw") bottom_left.graspBldw(stoi(arr[1]));
        else if(arr[0] == "BLhc") bottom_left.hillClimber(stoi(arr[1]));
        else if(arr[0] == "BLmls") bottom_left.multiStartLocalSearch(stoi(arr[1]));
        else if(arr[0] == "BLsa") bottom_left.simulatedAnnealing(stod(arr[1]), stoi(arr[2]), stoi(arr[3]));
        else if(arr[0] == "BLts") bottom_left.tabuSearch(stoi(arr[1]), stoi(arr[2]), stoi(arr[3])); 
        else if(name == "BoundsByArea") bounds.byArea();
        else if(name == "BoundsbyWiderThanHalf") bounds.byWiderThanHalf();
        else if(arr[0] == "SHgg") {
            map<string, ggOrder> m_order {
                {"heightAsc", heightAsc},
                {"heightDsc", heightDsc},
                {"widthAsc", widthAsc},
                {"widthDsc", widthDsc}
            };
            map<string, ggShelf> m_fit {
                {"bestFit", bestFit},
                {"worstFit", worstFit},
                {"nextFit", nextFit}
            };
            shelf.genericGreedy(m_order[arr[1]], m_fit[arr[2]], arr.size() > 3 && arr[3] == "rotated");
        }
        else if(arr[0] == "SHhillClimber") shelf.hillClimber();
        else if(arr[0] == "SHsa") shelf.simulatedAnnealing2(stod(arr[1]), stoi(arr[2]), stoi(arr[3]));
        else if(arr[0] == "SKBurke") skyline.burke();
        else if(arr[0] == "SKhc") skyline.hillClimber(stoi(arr[1]));
        else if(arr[0] == "SKISH") skyline.ish();
        else if(arr[0] == "SKmls") skyline.multiStartLocalSearch(stoi(arr[1]));
        else if(arr[0] == "SKsa") skyline.simulatedAnnealing(stod(arr[1]), stoi(arr[2]), stoi(arr[3]));
        else if(arr[0] == "SKts") skyline.tabuSearch(stod(arr[1]), stoi(arr[2]), stoi(arr[3]));
        else { throw runtime_error("no such algorithm like: " + name); }
    }

    destroy();
}