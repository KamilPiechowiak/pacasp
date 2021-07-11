#ifndef RECORDER_HPP
#define RECORDER_HPP

#include"general/general.hpp"

class Recorder {
private:
    string path;
    string filename;
    chrono::system_clock::time_point t_start;
    ofstream f;
    ll max_time;
    int instance_id;
    ll get_time();
    void start_timer();
public:
    Recorder(string path, int instance_id, ll max_time);
    void open_log(string name);
    bool should_finish();
    void record(ll best_height);
    string get_filename();
};

#endif