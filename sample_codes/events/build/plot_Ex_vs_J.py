import numpy as np 
import matplotlib.pyplot as plt
import matplotlib.cm as cm

#Import data from file
J = np.genfromtxt("Ex_vs_J_Z=52.dat", skip_header=1, usecols=(3))
Ex = np.genfromtxt("Ex_vs_J_Z=52.dat", skip_header=1, usecols=(2))


#Define matrix properties
J_max = 60
J_binwidth = 1
N_J = int(J_max//J_binwidth)

Ex_max = 100
Ex_binwidth = 0.5
N_Ex = int(Ex_max//Ex_binwidth)

J_vs_Ex = np.zeros((N_J,N_Ex))

#Fill matrix
for i in range(len(Ex)):
	J_bin = int(J[i]//J_binwidth)
	Ex_bin = int(Ex[i]//Ex_binwidth)

	J_vs_Ex[J_bin][Ex_bin] += 1


#Plot matrix
plt.imshow(J_vs_Ex, origin="lower", cmap=cm.cividis, extent=[0,J_max,0,Ex_max])
plt.xlabel('J [hbar]')
plt.ylabel('Ex [MeV]')

cbar = plt.colorbar()
cbar.set_label("Counts")

plt.show()


