import matplotlib.pyplot as plt
import numpy as np
import re
from tqdm import tqdm
import portfolio

algorithms = portfolio.getAlgorithms()
timePoints = portfolio.getTimePoints()
n = timePoints.size
names = portfolio.getNames()

def generateStats(portfolioName, instances):
    port = []
    for t in tqdm(range(n)):
        p = set()
        name = "portfolios/" + portfolioName + "/" + str(t) + ".cost"
        with open(name) as f:
            f.readline()
            a = 0
            while True:
                l = f.readline()
                if l == "":
                    break
                l = re.findall(r'\d', l)
                for d in l:
                    if d == "1":
                        p.add(algorithms[a])
                    a+=1
        port.append(p)

    bestAll = np.ndarray(shape=(len(instances), n), dtype=np.int64)+np.inf
    bestPort = np.ndarray(shape=bestAll.shape, dtype=np.int64)+np.inf

    for i in tqdm(instances):
        j = i-instances[0]
        for a in algorithms:
            name = "out/" + str(i) + "_" + a + ".log"
            t = 0
            h = np.inf
            with open(name) as f:
                while True:
                    l = f.readline()
                    if l == "":
                        ti = np.inf
                    else:
                        l = l.split()
                        ti, nh = float(l[0])/1e6, int(l[1])
                    while t < n and timePoints[t] < ti+1e-4:
                        bestAll[j, t] = min(bestAll[j, t], h)
                        if a in port[t]:
                            bestPort[j, t] = min(bestPort[j, t], h)
                        t+=1
                    h = nh
                    if t == n:
                        break

    print(np.count_nonzero(np.isinf(bestPort)))
    rel = bestPort/bestAll
    q = np.quantile(rel, [0.25, 0.5, 0.75, 1.0], axis=0).transpose()
    tt = np.log(timePoints[0:n])/np.log(10)
    lab = ["Q1", "Q2", "Q3", "Q4"]
    for i in range(q.shape[1]):
        plt.plot(tt, q[:,i], label=lab[i])
    plt.legend()
    plt.xlabel("log(T[ms])")
    plt.savefig("portfolios/plots/" + portfolioName + "rel.pdf", bbox_inches='tight')
    plt.clf()
    wins = np.count_nonzero(rel == 1, axis=0)/len(instances)
    plt.plot(tt, wins)
    plt.xlabel("log(T[ms])")
    plt.savefig("portfolios/plots/" + portfolioName + "wins.pdf", bbox_inches='tight')
    plt.clf()
    print(sum(np.ones(shape=bestAll.shape)[rel == 1])/len(instances)/n)
                
instances = [i for i in range(1000, 2207)]
for name in names:
    generateStats(name, instances)