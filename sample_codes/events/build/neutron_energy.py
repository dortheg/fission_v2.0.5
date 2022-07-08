import numpy as np
import matplotlib.pyplot as plt 

energy = np.genfromtxt("neutron_energy.dat", usecols=(0))

energy_max = 10
energy_binwidth = 0.1
N_energy = int(energy_max//energy_binwidth)

energy_spectrum = np.zeros(N_energy)

for i in range(len(energy)):
	binvalue = int(energy[i]//energy_binwidth)
	energy_spectrum[binvalue] += 1

plt.plot(np.linspace(0,energy_max,N_energy), energy_spectrum)
plt.grid()
plt.xlabel("Energy [MeV]")
plt.ylabel("Counts")
plt.title("Neutron energy spectrum")
plt.show()

print(np.average(energy))
