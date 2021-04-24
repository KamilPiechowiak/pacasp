#include"general.hpp"

mt19937 gen;

void setSeed(int seed) {
    gen.seed(seed);
}

int ri(int a, int b) {
    uniform_int_distribution<> d(a, b);
    return d(gen);
}

ll rll(ll a, ll b) {
    uniform_int_distribution<ll> d(a, b);
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

ll perimeter(ll x, ll y) {
    return x*x+y*y;
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