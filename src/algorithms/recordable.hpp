#ifndef RECORDABLE_HPP
#define RECORDABLE_HPP

#include"general/general.hpp"

struct Recordable {
protected:
    static chrono::system_clock::time_point tStart;
    static ll bestHeight;
    static ofstream f;
    static int instanceId;
    static const bool verbose = true;
    static void openLog(string name);
    static void startTimer();
    static ll getTime();
    static void record();
public:
    static void setInstanceId(int _instanceId);
};

#endif