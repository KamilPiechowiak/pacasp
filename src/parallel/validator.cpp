#include"validator.hpp"

Validator::Validator(ll maxTime) : Parallel(maxTime) {}

void Validator::run(json config) {
    int instance_id = stoi((string)config[to_string(nodeId)]["instance_id"]);
    for(string name : config[to_string(nodeId)]["algorithms"]) {
        ifstream f("placements/" + to_string(instance_id) + "_" + name + ".log");
        int n;
        ll width, height;
        f >> n >> width >> height;
        vector<RectanglePosition> rectangles_positions;
        for(int i=0; i < n; i++) {
            RectanglePosition rp;
            f >> rp.x1 >> rp.y1 >> rp.x2 >> rp.y2;
            rectangles_positions.push_back(rp);
            if(!(0 <= rp.x1 && 0 <= rp.y1 && 0 <= rp.x2 && 0 <= rp.y2
                && rp.x1 < width && rp.y1 < height && rp.x2 < width && rp.y2 < height)) {
                throw logic_error(to_string(instance_id) + "_" + name + ": rectangle " + to_string(i) + " does not fit inside the strip");
            }
        }
        for(int i=0; i < n; i++) {
            RectanglePosition &rp_i = rectangles_positions[i];
            for(int j=i+1; j < n; j++) {
                RectanglePosition &rp_j = rectangles_positions[j];
                ll dx = min(rp_i.x2, rp_j.x2) - max(rp_i.x1, rp_j.x1);
                ll dy = min(rp_i.y2, rp_j.y2) - max(rp_i.y1, rp_j.y1);
                if(dx >= 0 && dy >= 0) { //non-empty intersection
                    throw logic_error(to_string(instance_id) + "_" + name + ": rectangles " + to_string(i) + ", " + to_string(j) + " intersect");
                } 
            }
        }
    }
    destroy();
}