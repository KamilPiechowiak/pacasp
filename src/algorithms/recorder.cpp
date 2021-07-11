#include"algorithms/recorder.hpp"

Recorder::Recorder(string path, int instance_id, ll max_time) : path(path), max_time(1000000LL*max_time), instance_id(instance_id) {}

void Recorder::open_log(string name) {
    if(f.is_open()) {
        f.close();
    }
    filename = to_string(instance_id) + "_" + name + ".log";
    f.open(path + "/" + filename);
    start_timer();
}

void Recorder::start_timer() {
    t_start = chrono::system_clock::now();
}

ll Recorder::get_time() {
    return ::getTime(t_start);
}

bool Recorder::should_finish() {
    return get_time() > max_time;
}

void Recorder::record(ll best_height) {
    ll ti = ::getTime(t_start);
    f << ti << " " << best_height << "\n";
}

string Recorder::get_filename() {
    return filename;
}