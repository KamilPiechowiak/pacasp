#include<mpi.h>
#include"parallel.hpp"
#include"algorithms/bottomLeft.hpp"
#include"algorithms/shelf.hpp"
#include"algorithms/bounds.hpp"

Parallel::Parallel(ll _maxTime) : maxTime(_maxTime) {
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &nodeId);
    filename = "";   
}

Parallel::~Parallel() {
}

void Parallel::destroy() {
    MPI_Finalize();
}

void Parallel::loadData() {
    vector<Rectangle> rect;
    ifstream f;
    f.open(filename);
    int n; ll w;
    f >> n >> w;
    for(int i=0; i < n; i++) {
        ll a, b;
        f >> a >> b;
        rect.push_back(Rectangle(a, b, i));
    }
    Recordable::setInstanceId(nodeId);
    cerr << maxTime << "\n";
    BottomLeft::init(w, rect, maxTime);
    Shelf::init(w, rect, maxTime);
    Bounds::byArea(w, rect);
    Bounds::byWiderThanHalf(w, rect);
}