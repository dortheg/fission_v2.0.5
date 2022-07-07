import numpy as np 
import matplotlib.pyplot as plt 

#Read multiplicities
gmult = np.genfromtxt("134Te_gammadecay.dat", skip_header=1, usecols=(3))

#Create and fill gamma coincidence matrix
gmult_max = int(max(gmult))
print(gmult_max)
gamma_matrix = np.zeros((len(gmult),gmult_max))

for i in range(gmult_max):
	gamma_matrix[:,i] = np.genfromtxt("134Te_gammadecay.dat", skip_header=1, usecols=(i+4))

#Create and fill gamma-spectrum
gamma_max = 15 	#MeV
gamma_binwidth = 0.1 #MeV
N_gammabins = int(gamma_max//gamma_binwidth)

gamma_spec = np.zeros(N_gammabins)

for i in range(len(gmult)):
	for j in range(int(gmult[i])):
		gamma_bin = int(gamma_matrix[i][j]//gamma_binwidth)
		gamma_spec[gamma_bin] += 1


plt.plot(np.linspace(0,gamma_max,N_gammabins), gamma_spec)
plt.title("Gamma spectrum from 134Te (product)", fontsize=14)
plt.xlabel("Energy [MeV]", fontsize=14)
plt.ylabel("Counts", fontsize=14)
plt.grid()
plt.show()


