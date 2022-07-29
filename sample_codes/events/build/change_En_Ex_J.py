import matplotlib.pyplot as plt
import numpy as np 

Ex_x_energydistr = np.genfromtxt("Ex_vs_j_Z=52_238_92_energydistr.dat", skip_header=1, usecols=(2))
J_x_energydistr = np.genfromtxt("Ex_vs_j_Z=52_238_92_energydistr.dat", skip_header=1, usecols=(3))

Ex_x_constant = np.genfromtxt("Ex_vs_j_Z=52_238_92_constant.dat", skip_header=1, usecols=(2))
J_x_constant = np.genfromtxt("Ex_vs_j_Z=52_238_92_constant.dat", skip_header=1, usecols=(3))

J_max = 15
J_binwidth = 1 
N_J = int(J_max//J_binwidth)
x_J = np.linspace(0,J_max,N_J+1)

Ex_max = 30
Ex_binwidth = 0.1 
N_Ex = int(Ex_max//Ex_binwidth)
x_Ex = np.linspace(0,Ex_max,N_Ex+1)


fig, axs = plt.subplots(2)
fig.suptitle('Change in Ex and J of 134Te-precursor with change in En: constant=1.7MeV vs nuball distribution')
axs[0].plot(x_J[:-1], np.histogram(J_x_energydistr, bins=x_J)[0], label="energydistr")
axs[0].plot(x_J[:-1], np.histogram(J_x_constant, bins=x_J)[0], label="constant")
axs[0].legend()
axs[0].set(xlabel='J [hbar]', ylabel='Counts')
axs[0].grid()

axs[1].plot(x_Ex[:-1], np.histogram(Ex_x_energydistr, bins=x_Ex)[0], label="energydistr")
axs[1].plot(x_Ex[:-1], np.histogram(Ex_x_constant, bins=x_Ex)[0], label="x=constant")
axs[1].set(xlabel='Ex [MeV]', ylabel='Counts')
axs[1].legend()
axs[1].grid()
plt.show()
