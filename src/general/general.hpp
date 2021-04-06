#ifndef GENERAL_H
#define GENERAL_H

#include<bits/stdc++.h>
#define INF 1000000000
#define LL_INF 1000000000000000000LL
#define SIZE(a) ((int)a.size())

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef pair<double, double> pdd;

int ri(int a, int b);
double rd(double a, double b);
ll getTime(chrono::system_clock::time_point tStart);
ll perimeter(ll x, ll y);
ll fPow(ll a, ll k);
void setSeed(int seed);

#define DEBUG 0

#endif