import numpy as np 
import matplotlib.pyplot as plt 

S0_down = np.genfromtxt("angmom_238_92_down.dat", usecols=(0))
S1_down = np.genfromtxt("angmom_238_92_down.dat", usecols=(1))
S2_down = np.genfromtxt("angmom_238_92_down.dat", usecols=(2))

S0_close = np.genfromtxt("angmom_238_92_close.dat", usecols=(0))
S1_close = np.genfromtxt("angmom_238_92_close.dat", usecols=(1))
S2_close = np.genfromtxt("angmom_238_92_close.dat", usecols=(2))

S_max = 15
S_binwidth = 1 
N_S = int(S_max//S_binwidth)

x_S = np.linspace(0,S_max,N_S+1)

plt.plot(x_S[:-1], np.histogram(S0_down, bins=x_S)[0], color="r", label="S0_down")
plt.plot(x_S[:-1], np.histogram(S1_down, bins=x_S)[0], color="g", label="S1_down")
plt.plot(x_S[:-1], np.histogram(S2_down, bins=x_S)[0], color="b", label="S2_down")
plt.plot(x_S[:-1], np.histogram(S0_close, bins=x_S)[0], color="r", linestyle='dashed', label="S0_close", )
plt.plot(x_S[:-1], np.histogram(S1_close, bins=x_S)[0], color="g", linestyle='dashed', label="S1_close")
plt.plot(x_S[:-1], np.histogram(S2_close, bins=x_S)[0], color="b", linestyle='dashed', label="S2_close")
plt.legend(fontsize=14)
plt.xlabel("J [hbar]", fontsize=14)
plt.ylabel("Counts", fontsize=14)
plt.grid()
plt.plot()
plt.show()

