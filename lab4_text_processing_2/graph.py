import matplotlib.pyplot as plt

x = [
    [141954391, 157883754, 158855766],
    [78156828, 91228613, 92197650],
    [59311685, 71494711, 72548496],
    [55439142, 67103259, 68191068],
    [52424727, 64237937, 65291793],
    [50926182, 62304838, 63395408],
    [49353395, 60720515, 61779654],
    [48533577, 59727837, 60815610],
    [48097617, 59379628, 60477559],
    [47375954, 58515231, 59637407]]


indexing_time = [t[2]/1000000 for t in x]


plt.plot(range(1, len(x) + 1), indexing_time)
plt.ylim(ymin=0.0)
plt.show()