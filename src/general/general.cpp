#include"general.hpp"

mt19937 gen;

int ri(int a, int b) {
    uniform_int_distribution<> d(a, b);
    return d(gen);
}

double rd(double a, double b) {
    uniform_real_distribution<> d(a, b);
    return d(gen);
}

ll getTime(chrono::system_clock::time_point tStart) {
    auto tCurrent = chrono::system_clock::now();
    chrono::duration<int64_t, nano> dt = tCurrent-tStart;
    return dt.count();
}

ll fPow(ll a, ll k) {
    ll res=1;
    while(k) {
        if(k%2) {
            res*=a;
        }
        a*=a;
        k/=2;
    }
    return res;
}