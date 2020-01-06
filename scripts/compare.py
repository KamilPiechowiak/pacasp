import matplotlib.pyplot as plt
import numpy as np
import re
from tqdm import tqdm
import portfolio

algorithms = portfolio.getAlgorithms()
greedyAlgorithms = portfolio.getGreedyAlgorithms()
timePoints = portfolio.getTimePoints()
n = timePoints.size

instances = [i for i in range(1000, 2207)]

portfolioNames = [(x, []) for x in portfolio.getNames()]
portfolioNames+= [
    ("sg", list(greedyAlgorithms)),
    ("met", ["BLmls_1_5", "BLsa_0.606531_1_1000", "BLts2_5_100", "SHsa_0.899661_1_10000"])
]

def generatePortfolio(portfolioName, algorithmsList=[]):
    port = []
    cost = []
    if len(algorithmsList) == 0:
        for t in tqdm(range(n)):
            p = set()
            c = 0
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
                            if algorithms[a] not in greedyAlgorithms:
                                c+=1.0
                        a+=1
            port.append(p)
            cost.append(c)
    else:
        p = set(algorithmsList)
        port = [p for t in range(n)]
        c = len(p-greedyAlgorithms)
        cost = [c for t in range(n)]
    return port, cost

portfolios = []
costs = []
for port in portfolioNames:
    p, c = generatePortfolio(port[0], port[1])
    portfolios.append(p)
    costs.append(c)

costs = np.array(costs).reshape(-1, n)
greedy_costs_per_instace = np.zeros((len(instances), len(portfolioNames), n))

bestAll = np.ndarray(shape=(len(instances), n), dtype=np.int64)+np.inf
bestPort = np.ndarray(shape=(len(portfolioNames), len(instances), n), dtype=np.int64)+np.inf
s=set()
for i in tqdm(instances):
    j = i-instances[0]
    for a in algorithms:
        name = "out/" + str(i) + "_" + a + ".log"
        t = 0
        h = np.inf
        ti_prev = 0
        with open(name) as f:
            while True:
                l = f.readline()
                if l == "":
                    ti = np.inf
                else:
                    l = l.split()
                    ti, nh = float(l[0])/1e6, int(l[1])
                while t < n and timePoints[t] < ti:
                    bestAll[j, t] = min(bestAll[j, t], h)
                    for p in range(len(portfolioNames)):
                        if a in portfolios[p][t]:
                            bestPort[p, j, t] = min(bestPort[p, j, t], h)
                            if a in greedyAlgorithms:
                                if ti_prev == 0:
                                    greedy_costs_per_instace[j, p, t] += 1.0
                                    if ti > 1000:
                                        s.add((i, a))
                                else:
                                    greedy_costs_per_instace[j, p, t]+= ti_prev/timePoints[t]
                    t+=1
                h = nh
                ti_prev = ti
                if t == n:
                    break

print("Above 1000: ", s, len(s))
print(np.max(greedy_costs_per_instace, axis=0)[:,-1])
costs+=np.max(greedy_costs_per_instace, axis=0)

print(np.count_nonzero(np.isinf(bestPort)))
rel = bestPort/bestAll
tt = np.log(timePoints[0:n])/np.log(10)
wins = np.count_nonzero(rel == 1, axis=1)/len(instances)
fig, ax = plt.subplots(2, 1)
for p in range(len(portfolioNames)):
    ax[0].plot(tt, wins[p,:], label=portfolioNames[p][0])
    ax[1].plot(tt, costs[p], label=portfolioNames[p][0])
plt.xlabel("log(T[ms])")
plt.legend()
plt.savefig("portfolios/plots/wins.pdf", bbox_inches='tight')
                
