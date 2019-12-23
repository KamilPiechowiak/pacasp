#include"recordable.hpp"

chrono::system_clock::time_point Recordable::tStart;
ll Recordable::bestHeight;
ofstream Recordable::f;
const bool Recordable::verbose;
int Recordable::instanceId;

void Recordable::openLog(string name) {
    if(verbose) {
        f.close();
        f.open("out/" + to_string(instanceId) + "_" + name + ".log");
    }
}

void Recordable::startTimer() {
    tStart = chrono::system_clock::now();
}

ll Recordable::getTime() {
    return ::getTime(tStart);
}

void Recordable::record() {
    if(verbose) {
        ll ti = ::getTime(tStart);
        f << ti << " " << bestHeight << "\n";
    }
}

void Recordable::setInstanceId(int _instanceId) {
    instanceId = _instanceId;
}