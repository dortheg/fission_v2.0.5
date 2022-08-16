import numpy as np 
import matplotlib.pyplot as plt
import matplotlib.cm as cm

file = "Ex_vs_J_Z=52_238_92.dat"

#Import data from file
J = np.genfromtxt(file, skip_header=1, usecols=(2))
Ex = np.genfromtxt(file, skip_header=1, usecols=(3))
Erot = np.genfromtxt(file, skip_header=1, usecols=(4))
Estat = np.genfromtxt(file, skip_header=1, usecols=(5))


#################################
###       Ex vs J matrix      ###
#################################

#Define matrix properties
J_max = 20
J_binwidth = 1
N_J = int(J_max//J_binwidth)

Ex_max = 30
Ex_binwidth = 0.5
N_Ex = int(Ex_max//Ex_binwidth)

J_vs_Ex = np.zeros((N_J,N_Ex))


#Fill matrix
for i in range(len(Ex)):
	J_bin = int(J[i]//J_binwidth)
	Ex_bin = int(Ex[i]//Ex_binwidth)

	J_vs_Ex[J_bin][Ex_bin] += 1


# #Plot matrix
plt.imshow(J_vs_Ex, origin="lower", cmap=cm.cividis, extent=[0,J_max,0,Ex_max])
plt.xlabel('J [hbar]', fontsize=12)
plt.ylabel('Ex [MeV]', fontsize=12)
plt.title("J vs Ex Te isotopes")

cbar = plt.colorbar()
cbar.set_label("Counts", fontsize=12)

plt.show()


#################################
###      Average Ex vs J      ###
#################################

J_array = np.linspace(0,J_max,J_max+1)
event_counter = np.zeros(len(J_array))
avg_Ex = np.zeros(len(J_array))
avg_Erot = np.zeros(len(J_array))
avg_Estat = np.zeros(len(J_array))

for i in range(len(J)):
	J_bin = int(J[i])
	event_counter[J_bin] += 1
	avg_Ex[J_bin] += Ex[i]
	avg_Erot[J_bin] += Erot[i]
	avg_Estat[J_bin] += Estat[i]

for i in range(J_max+1):
	if event_counter[i] > 0:
		avg_Ex[i] = avg_Ex[i]/event_counter[i]
		avg_Erot[i] = avg_Erot[i]/event_counter[i]
		avg_Estat[i] = avg_Estat[i]/event_counter[i]
	else:
		avg_Ex[i] = 0

plt.plot(J_array, event_counter, "ro-")
plt.title("Te-precursor: counts of Sf2", fontsize=14)
plt.xlabel("J [hbar]", fontsize=14)
plt.ylabel("Counts", fontsize=14)
plt.grid()
plt.show()

plt.plot(J_array, avg_Ex, "o-", color="purple", label="Ex")
plt.plot(J_array, avg_Erot, "o-", color="royalblue", label="Erot")
plt.plot(J_array, avg_Estat, "o-", color="deeppink", label="Estat")
plt.title("Te-precursor: Average Ex", fontsize=14)
plt.xlabel("J [hbar]", fontsize=14)
plt.ylabel("Energy [MeV]", fontsize=14)
plt.legend(fontsize=14)
plt.grid()
plt.show()









