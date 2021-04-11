#include<bits/stdc++.h>
#include"algorithms/bounds.hpp"
#include"algorithms/recorder.hpp"

using namespace std;
typedef pair<double, double> pdd;

mt19937 gen;

int ri(int a, int b) {
    uniform_int_distribution<> d(a, b);
    return d(gen);
}

double rd(double a, double b) {
    uniform_real_distribution<> d(a, b);
    return d(gen);
}

struct Shape {
    double lowerBound;
    double width;
    double height;
    Shape(double _lowerBound, double _width, double _height) : lowerBound(_lowerBound), width(_width), height(_height) {}
};

struct Type {
    int lowerBound;
    vector<Shape> F;
    Type(int _lowerBound, vector<Shape> _F) : lowerBound(_lowerBound), F(_F) {}
    void generateRectangle() {
        double r = rd(0, 1);
        int id;
        for(id=0; id < (int)F.size(); id++) {
            if(r < F[id].lowerBound) {
                break;
            }
        }
        if(id == (int)F.size()) {
            id--;
        }
        int h = ri(1, (int)F[id].height);
        int w = ri(1, (int)F[id].width);
        cout << w << " " <<  h << endl;
    }
};

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    int s=0;
    if(argc > 1) {
        s = stoi(argv[1]);
    }
    gen.seed(s); //learning instances
    // gen.seed(s+1000); //testing instances
    int w = ri(1000, 1000);
    int n = ri(100, 100);
    if(argc > 2) {
        n = stoi(argv[2]);
    }
    if(n == 0) {
        vector<int> nv = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};
        s = s%1000;
        n = nv[s/100];
    
        s = s%100;
        vector<Type> t;
        t.push_back(Type(0, {Shape(1.0, w, w)}));
        t.push_back(Type(70, {Shape(1.0, w/2, w/2)}));
        t.push_back(Type(75, {Shape(0.75, w/2, w/2), Shape(1.0, w, w)}));
        t.push_back(Type(80, {Shape(1.0, w/10, 2*w)}));
        t.push_back(Type(85, {Shape(0.5, w/10, 2*w), Shape(1.0, w, w)}));
        t.push_back(Type(90, {Shape(0.33, w, w/5), Shape(0.67, w/5, w), Shape(1.0, w, w)}));

        int id;
        for(id=0; id < (int)t.size(); id++) {
            if(s < t[id].lowerBound) {
                break;
            }
        }
        id--;
        cout << n << " " << w << "\n";
        for(int i=0; i < n; i++) {
            t[id].generateRectangle();
        }
    } else {
        Type t = Type(0, {Shape(1.0, w, w)});
        cout << n << " " << w << "\n";
        for(int i=0; i < n; i++) {
            t.generateRectangle();
        }
    }
}