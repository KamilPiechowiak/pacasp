#include"portfolio.hpp"

Portfolio::Portfolio(int _timeId, vector<pll> instances) : instances(instances) {
    timeId = _timeId-1;
}

void Portfolio::run() {
    getTimeLimit();
    getAlgorithmsList();
    getInstances();
    getData();
    getWinners();
    // computeStats();
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
                string name = "out/" + to_string(i) + "_" + algorithms[a] + ".log";
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
    string directory = "portfolios/dat/";
    system(("mkdir -p " + directory).c_str());
    fname = directory + to_string(timeId) + ".dat";
    f.open(fname);
    f << "NBA=" << numberOfAlgorithms << ";\n";
    f << "NBI=" << numberOfInstances << ";\n";
    f << "timeId=\"" << timeId << "\";\n";
    f << "path=\"" << path << "\";\n";
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

    string directory = "portfolios/" + path + "/";
    system(("mkdir -p " + directory).c_str());
    gname =  directory + to_string(timeId) + ".cost";
    oplrun("MinCost.mod");
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

    string directory = "portfolios/" + path + "/";
    system(("mkdir -p " + directory).c_str());
    gname = directory + to_string(timeId) + ".regr";
    oplrun("MxRegr.mod");
}

void Portfolio::oplrun(string name) {
    string opl = "/opt/ibm/ILOG/CPLEX_Studio128/opl/bin/x86-64_linux/oplrun";
    string cmd = opl + " portfolios/" + name + " " + fname;
    // cmd+=" 1> /dev/null 2> /dev/null";
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
        "BLblda",
        "BLbldh",
        "BLbldw",
        "BLblia",
        "BLblih",
        "BLbliw",
        "BLgraspBlda_5",
        "BLgraspBldh_5",
        "BLgraspBldw_5",
        "BLhc_0",
        "BLhc_1",
        "BLmls_215",
        "BLsa_0.606531_1_1000",
        "BLts_0_5_46",
        "BLts_1_22_46",
        "SHgg_heightAsc_bestFit",
        "SHgg_heightAsc_bestFit_rotated",
        "SHgg_heightAsc_nextFit",
        "SHgg_heightAsc_nextFit_rotated",
        "SHgg_heightAsc_worstFit",
        "SHgg_heightAsc_worstFit_rotated",
        "SHgg_heightDsc_bestFit",
        "SHgg_heightDsc_bestFit_rotated",
        "SHgg_heightDsc_nextFit",
        "SHgg_heightDsc_nextFit_rotated",
        "SHgg_heightDsc_worstFit",
        "SHgg_heightDsc_worstFit_rotated",
        "SHgg_widthAsc_bestFit",
        "SHgg_widthAsc_bestFit_rotated",
        "SHgg_widthAsc_nextFit",
        "SHgg_widthAsc_nextFit_rotated",
        "SHgg_widthAsc_worstFit",
        "SHgg_widthAsc_worstFit_rotated",
        "SHgg_widthDsc_bestFit",
        "SHgg_widthDsc_bestFit_rotated",
        "SHgg_widthDsc_nextFit",
        "SHgg_widthDsc_nextFit_rotated",
        "SHgg_widthDsc_worstFit",
        "SHgg_widthDsc_worstFit_rotated",
        "SHhillClimber",
        "SHsa_0.899661_1_10000",
        "SKBurke",
        "SKhc_0",
        "SKhc_1",
        "SKISH",
        "SKmls_1000",
        "SKsa_0.606531_1_1000",
        "SKts_0_1000_215",
        "SKts_1_215_215"
    };
    //ls out/0_* | cut -c 1-6 --complement | rev | cut -c 1-4 --complement | rev | xargs -I {} echo \"{}\"
    numberOfAlgorithms = algorithms.size();
}


void Portfolio::getInstances() {
    numberOfInstances=0;
    path = "";
    for(pii x : instances) {
        numberOfInstances+=x.second-x.first+1;
        path=to_string(x.first)+"-"+to_string(x.second);
    }
}

void Portfolio::computeStats() {
    vector<pair<int, int>> totalWins(numberOfAlgorithms);
    for(int a=0; a < numberOfAlgorithms; a++) {
        totalWins[a].second = a;
    }
    for(int i=0; i < numberOfInstances; i++) {
        for(int a : wins[i]) {
            totalWins[a].first++;
        }
    }
    sort(totalWins.begin(), totalWins.end());
    reverse(totalWins.begin(), totalWins.end());
    for(auto x : totalWins) {
        cout << x.first << " " << algorithms[x.second] << "\n";
    }
}