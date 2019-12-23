#include"portfolio.hpp"

Portfolio::Portfolio(int _timeId) {
    timeId = _timeId-1;
}

void Portfolio::run() {
    getTimeLimit();
    getAlgorithmsList();
    getInstances();
    getData();
    getWinners();
    computeMinCost();
    // computeMinRegret();
    // getResult();
}

void Portfolio::getTimeLimit() {
    vector<ll> timePoints;
    ll t = 20*1000*1000;
    ll hour = 3600LL*1000000000;
    ll second = 1000000000;
    int numberOfTimePoints = 500;
    double dt = (log(hour) - log(t))/double(numberOfTimePoints-1);
    double ct = log(t);
    for(int i=0; i < numberOfTimePoints; i++) {
        timePoints.push_back(exp(ct));
        ct+=dt;
    }
    // ll a = 1000*1000;
    // while(t <= second) {
    //     timePoints.push_back(t);
    //     if(100*a <= t) {
    //         a*=10;
    //     }
    //     t+=a;
    // }
    // for(ll t : timePoints) {
    //     cout << t << "\n";
    // }
    // cout << timePoints.size() << "\n";
    maxTime = timePoints[timeId];
}

void Portfolio::getData() {
    data.resize(numberOfAlgorithms);
    for(int a=0; a < numberOfAlgorithms; a++) {
        data[a].resize(numberOfInstances);
        int j=0;
        for(pll ins : instances) {
            for(int i=ins.first; i <= ins.second; i++) {
                data[a][j] = {-1, -1};
                string name = "out3/" + to_string(i) + "_" + algorithms[a] + ".log";
                ifstream f;
                f.open(name);
                ll ti, qu;
                while(f >> ti >> qu) {
                    if(ti > maxTime) {
                        break;
                    }
                    data[a][j] = {ti, qu};
                }
                j++;
            }
        }
    }
}

void Portfolio::getWinners() {
    qmin.resize(numberOfInstances);
    wins.resize(numberOfInstances);
    for(int i=0; i < numberOfInstances; i++) {
        qmin[i] = LL_INF;
        for(int a=0; a < numberOfAlgorithms; a++) {
            // if(a == 5) {
            //     cerr << data[a][i].second << "\n";
            // }
            if(data[a][i].second == -1) {
                continue;
            }
            if(data[a][i].second < qmin[i]) {
                qmin[i] = data[a][i].second;
                wins[i].clear();
                wins[i].insert(a);
            } else if(data[a][i].second == qmin[i]) {
                wins[i].insert(a);
            }
        }
        if(qmin[i] == LL_INF) { //no feasible solution found
            cerr << maxTime << "\n";
            cerr << "No feasible solution from instance: " << i << "\n";
            exit(-1);
        }
        // cerr << qmin[i] << "\n";
        // cerr << i << " ";
        // for(int a : wins[i]) {
        //     cerr << algorithms[a] << " ";
        // }
        // cerr << "\n";
    }
}

void Portfolio::computeMinSth() {
    fname = "portfolios/dat/" + to_string(timeId) + ".dat";
    f.open(fname);
    f << "NBA=" << numberOfAlgorithms << ";\n";
    f << "NBI=" << numberOfInstances << ";\n";
    f << "timeId=\"" << timeId << "\";\n";
}

void Portfolio::computeMinCost() {
    computeMinSth();

    f << "WIN=[ \n";
    for(int a=0; a < numberOfAlgorithms; a++) {
        f << "[";
        for(int i=0; i < numberOfInstances; i++) {
            if(wins[i].count(a)) {
                f << "1 ";
            } else {
                f << "0 ";
            }
        }
        f << "]\n";
    }
    f << "];\n\n";

    f << "t=[ \n";
    for(int a=0; a < numberOfAlgorithms; a++) {
        f << "[";
        for(int i=0; i < numberOfInstances; i++) {
            ll ti = data[a][i].first;
            if(ti == -1) {
                ti = maxTime;
            }
            f << ti << " ";
        }
        f << "]\n";
    }
    f << "];\n\n";

    f.close();

    oplrun("MinCost.mod");
    gname = "portfolios/res/" + to_string(timeId) + ".cost";
}

void Portfolio::computeMinRegret() {
    computeMinSth();
    cost = 4*maxTime;
    f << "Cost=" << cost << ";\n";

    f << "RGRt=[ \n";
    for(int a=0; a< numberOfAlgorithms; a++) {
        f << "[";
        for(int i=0; i < numberOfInstances; i++) {
            ll qu = data[a][i].second;
            if(qu == -1) {
                qu = 100.0*qmin[i];
            }
            f << double(qu)/double(qmin[i]) << " ";
        }
        f << "]\n";
    }
    f << "];\n\n";

    f << "t=[ \n";
    for(int a=0; a < numberOfAlgorithms; a++) {
        f << "[";
        for(int i=0; i < numberOfInstances; i++) {
            ll ti = data[a][i].first;
            if(ti == -1) {
                ti = 100*maxTime;
            }
            f << ti << " ";
        }
        f << "]\n";
    }
    f << "];\n\n";

    f.close();

    oplrun("MxRegr.mod");
    gname = "portfolios/res/" + to_string(timeId) + ".regr";
}

void Portfolio::oplrun(string name) {
    string opl = "/opt/ibm/ILOG/CPLEX_Studio128/opl/bin/x86-64_linux/oplrun";
    string cmd = opl + " portfolios/" + name + " " + fname;
    cmd+=" 1> /dev/null 2> /dev/null";
    system(cmd.c_str());
}

void Portfolio::getResult() {
    g.open(gname);
    double obj;
    g >> obj;
    cout << obj << "\n";
    for(int i=0; i < numberOfAlgorithms; i++) {
        string x;
        g >> x;
        if(x == "[1" || x == "1" || x == "1]") {
            cout << algorithms[i] << "\n";
        }
    }
    g.close();
}

void Portfolio::getAlgorithmsList() {
    algorithms = {
        "bldh",
        "bldw",
        "BLhillClimber",
        "BLils",
        "BLmls_10_100",
        "BLsa_2_3500",
        "BLsa_4_400",
        "BLts2_30_0",
        "BLts2_30_13",
        "BLts2_30_20",
        "BLts_30_0",
        "BLts_30_115",
        "BLts_30_145",
        "BLts_30_35",
        "BLts_30_55",
        "gg_heightAsc_bestFit",
        "gg_heightAsc_bestFit_rotated",
        "gg_heightAsc_nextFit",
        "gg_heightAsc_nextFit_rotated",
        "gg_heightAsc_worstFit",
        "gg_heightAsc_worstFit_rotated",
        "gg_heightDsc_bestFit",
        "gg_heightDsc_bestFit_rotated",
        "gg_heightDsc_nextFit",
        "gg_heightDsc_nextFit_rotated",
        "gg_heightDsc_worstFit",
        "gg_heightDsc_worstFit_rotated",
        "gg_widthAsc_bestFit",
        "gg_widthAsc_bestFit_rotated",
        "gg_widthAsc_nextFit",
        "gg_widthAsc_nextFit_rotated",
        "gg_widthAsc_worstFit",
        "gg_widthAsc_worstFit_rotated",
        "gg_widthDsc_bestFit",
        "gg_widthDsc_bestFit_rotated",
        "gg_widthDsc_nextFit",
        "gg_widthDsc_nextFit_rotated",
        "gg_widthDsc_worstFit",
        "gg_widthDsc_worstFit_rotated",
        "graspBldh_1000",
        "graspBldh_100",
        "graspBldh_10",
        "graspBldw_1000",
        "graspBldw_100",
        "graspBldw_10",
        "SHhillClimber",
        "SHsa_1_45",
        "SHsa_3_17"
    };
    numberOfAlgorithms = algorithms.size();
}

void Portfolio::getInstances() {
    instances = {{1, 1000}};
    numberOfInstances=0;
    for(pii x : instances) {
        numberOfInstances+=x.second-x.first+1;
    }
}