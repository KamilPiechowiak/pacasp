import numpy as np

def getAlgorithms():
    return [
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
        "SKts_1_215_215",
]

def getGreedyAlgorithms():
    return set([
        "BLblda",
        "BLbldh",
        "BLbldw",
        "BLblia",
        "BLblih",
        "BLbliw",
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
        "SKBurke",
])

def getTimePoints():
    timePoints = []
    t = 20
    second = 1000
    hour = second*3600
    n = 500
    dt = (np.log(hour)-np.log(t))/n
    ct = np.log(t)
    for i in range(n):
        timePoints.append(np.exp(ct))
        ct+=dt
    return np.array(timePoints)

def getNames():
    return ["0-999", "300-699", "400-799", "500-999", "700-999", "1000-1999"]#, "3094-3263", "3215-3263"]

def getRange(s):
    a, b = [int(x) for x in s.split("-")]
    return [x for x in range(a, b+1)]