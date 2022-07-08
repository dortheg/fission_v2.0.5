import numpy as np 
import matplotlib.pyplot as plt 

#file = "134Te_gammadecay_252_98.dat"
file = "134Te_gammadecay_238_92.dat"

#Read multiplicities
gmult = np.genfromtxt(file, skip_header=1, usecols=(3))

#Create and fill gamma coincidence matrix
gmult_max = int(max(gmult))
gamma_matrix = np.zeros((len(gmult),gmult_max))

for i in range(gmult_max):
	gamma_matrix[:,i] = np.genfromtxt(file, skip_header=1, usecols=(i+4))

#Create and fill gamma-spectrum
gamma_max = 15 	#MeV
gamma_binwidth = 0.001 #MeV
N_gammabins = int(gamma_max//gamma_binwidth)

gamma_spec = np.zeros(N_gammabins)

for i in range(len(gmult)):
	for j in range(int(gmult[i])):
		gamma_bin = int(gamma_matrix[i][j]//gamma_binwidth)
		gamma_spec[gamma_bin] += 1

#Plot gamma spectrum
plt.plot(np.linspace(0,gamma_max,N_gammabins), gamma_spec)
plt.title("Gamma spectrum from 134Te (product)", fontsize=14)
plt.xlabel("Energy [MeV]", fontsize=14)
plt.ylabel("Counts", fontsize=14)
plt.grid()
plt.show()


#############################################
###			Isomeric yield ratios		  ###
#############################################

print("\n")
print(" 6+ --------------- 164.1 ns")
print("         | 115              ")
print("         V                  ")
print(" 4+ ---------------		   ")
print("         |        		   ")
print("         | 297       	   ")
print("         V         		   ")
print(" 2+ ---------------		   ")
print("         |        		   ")
print("         | 1279       	   ")
print("         |        		   ")
print("         V         		   ")
print(" 0+ ---------------		   ")
print("         134Te		       ")
print("\n")

#Calculate isomeric yield ratio: 4+ & 2+ gates
isomer_populated_4p2p = 0
isomer_not_populated_4p2p = 0

for i in range(len(gmult)):
	if int(gmult[i]) >= 3:
		if(gamma_matrix[i][int(gmult[i])-2] == 0.297):
			if(gamma_matrix[i][int(gmult[i])-3] == 0.115):
				isomer_populated_4p2p += 1
			else:
				isomer_not_populated_4p2p += 1

IYR_4p2p = isomer_populated_4p2p/(isomer_populated_4p2p+isomer_not_populated_4p2p)

print(" IYR 134Te, gate 4+ & 2+: %.3f \n" % IYR_4p2p)


#Calculate isomeric yield ratio: only 2+ gate
isomer_populated_2p = 0
isomer_not_populated_2p = 0

for i in range(len(gmult)):
	if int(gmult[i]) >= 3:
		if(gamma_matrix[i][int(gmult[i])-1] == 1.279):
			if(gamma_matrix[i][int(gmult[i])-3] == 0.115):
				isomer_populated_2p += 1
			else:
				isomer_not_populated_2p += 1

IYR_2p = isomer_populated_2p/(isomer_populated_2p+isomer_not_populated_2p)

print(" IYR 134Te, gate 2+: %.3f \n" % IYR_2p)

