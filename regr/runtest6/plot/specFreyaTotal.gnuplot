if(!exists("iso")) iso = 92235
if(!exists("engindex")) engindex = 0
if(!exists("FreyaEng")) FreyaEng = 0.53
if(!exists("engCons")) engCons = 2.53e-8

set logs x
set logs y

if(!exists("message")) message = 1; print "Press e to change energy"

plot \
"../build/testFreyaSpecTotal.res" using 3:((column(1) == iso && column(2) == FreyaEng && column(4) != 0.)?column(4):1/0) with linespoints title sprintf("Total fission neutron energy, FREYA, isotope = %g FreyaEng = %g MeV", iso, FreyaEng), \
"../baseline/testFreyaSpecTotal.res.baseline" using 3:((column(1) == iso && column(2) == FreyaEng && column(4) != 0.)?column(4):1/0) with linespoints title sprintf("baseline"), \
"../build/testFreyaSpecTotal.res" using 3:((column(1) == iso && column(2) == FreyaEng && column(5) != 0.)?column(5):1/0) with linespoints title sprintf("Total photon energy, FREYA, isotope = %g FreyaEng = %g MeV", iso, FreyaEng), \
"../baseline/testFreyaSpecTotal.res.baseline" using 3:((column(1) == iso && column(2) == FreyaEng && column(5) != 0.)?column(5):1/0) with linespoints title sprintf("baseline"), \
"../../runtest3/baseline/testEngConsSpecTotal.res.baseline" using 3:((column(1) == iso && column(2) == engCons && column(5) != 0.)?column(5):1/0) with linespoints title sprintf("Total photon energy, energy conservation, isotope = %g engCons = %g MeV", iso, engCons), \
"../../runtest3/baseline/testEngConsSpecTotal.res.baseline" using 3:((column(1) == iso && column(2) == engCons && column(4) != 0.)?column(4):1/0) with linespoints title sprintf("Total fission neutron energy, energy conservation, isotope = %g engCons = %g MeV", iso, engCons)

bind e "engindex=(engindex<1)?engindex+1:0; engCons=(engindex==0)?2.53e-8:(engindex==1)?1.:1.; load 'specFreyaTotal.gnuplot'"

