import matplotlib.pyplot as plt

x = [
    [141409610, 156274587, 157588486],
    [72390471, 82876622, 84218469],
    [52547635, 61639765, 62982787],
    [45644034, 54604402, 56184063],
    [44985296, 53983785, 55499653],
    [43925443, 52555800, 54121695],
    [43361608, 51778597, 53312828],
    [43299371, 51785566, 53311959],
    [43053340, 51437053, 52971370],
    [43303546, 51760402, 53312724]
]


indexing_time = [t[2]/1000000 for t in x]


plt.plot(range(1, len(x) + 1), indexing_time)
plt.ylim(ymin=0.0)
plt.show()