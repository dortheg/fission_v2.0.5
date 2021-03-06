// #define iterations 10000000
#define iterations 1000000
#define maxA 200
#define mMax 50       /* The maximum number of ejectiles per fission generated by FREYA */

#define minEnergy 1e-5 // MeV
#define maxEnergy 1e2  // MeV
#define nbinsperspectrum 600

#include <stdio.h>
#include <math.h>
#include "fissionEvent.h"
#include "Fission.h"

extern "C" {
   extern int msfreya_setup_c_();
   extern int msfreya_event_c_(int,double,double,double*,int*,int*,double*,int*,int*,double*,int*,double*,int*,double*);
   extern int msfreya_getids_c_(int*,int*,int*);
   extern int msfreya_getniso_c_(int *,int *);
   extern int msfreya_getzas_c_(int *,int *);
   extern double msfreya_sepn_c_(int, int, int);
   extern double msfreya_gsmassn_c_(int, int);
   extern int msfreya_geterrors_c_(char *, int *);
   extern int msfreya_reseterrorflag_c_();
   extern int msfreya_errorflagset_c_();
   extern int msfreya_usehostrng_c_();
}

void init(void);
void initFREYA(int& nisosf, int& nisoif, int& niso,
               int** ZAs, int** fistypes);
bool FREYA_event(double ePart, int iso, int fissiontype,
                 int*& ZAs, int*& fistypes, int niso,
                 double* tke_dist, double* nubar_dist, double* nubar2_dist,
                 int* n_events, int* ff_dist,
                 int n_dist_L[nbinsperspectrum],
                 int n_dist_H[nbinsperspectrum]);
double getKineticEnergy(double u, double v, double w);
FILE* openfile(char* name);
void output(double* tke_dist, double* nubar_dist, double* nubar2_dist, 
            int* n_events, int* ff_dist,
            int n_dist_L[nbinsperspectrum], int n_dist_H[nbinsperspectrum]);

int main() {
   double energy_MeV = 25.3e-9;
   int isotope = 94240;
   int fissiontype = 0;

   int nisosf = 0; // Number of spontaneous fission isotopes
   int nisoif = 0; // Number of induced fission isotopes
   int niso = 0;   // Number of fission isotopes

   int** ZAs;      // ZA's of fission isotopes
   int** fistypes; // types of fission [spontaneous (0), induced (1)
   ZAs = new int*;
   fistypes = new int*;

   int maxerrorlength=10000;
   char errors[maxerrorlength];

   double tke_dist[maxA+1];
   double nubar_dist[maxA+1];
   double nubar2_dist[maxA+1];
   int n_events[maxA+1];
   int ff_dist[maxA+1];
   for (int i=0; i<maxA+1; i++) {
      tke_dist[i] = 0.;
      nubar_dist[i] = 0.;
      nubar2_dist[i] = 0.;
      n_events[i] = 0.;
      ff_dist[i] = 0.;
   }

   // neutron spectra
   int n_dist_L[nbinsperspectrum];
   int n_dist_H[nbinsperspectrum];
   for (int i=0; i<nbinsperspectrum; i++) n_dist_L[i] = 0.;
   for (int i=0; i<nbinsperspectrum; i++) n_dist_H[i] = 0.;

   init();
   initFREYA(nisosf, nisoif, niso, ZAs, fistypes);
   niso=nisosf+nisoif;
   
   for (int i=0; i<iterations; i++) {
      if (!FREYA_event(energy_MeV, isotope, fissiontype, 
                       *ZAs, *fistypes, niso,
                       tke_dist, nubar_dist, nubar2_dist, n_events, 
                       ff_dist, n_dist_L, n_dist_H)) {
         int errorlength=maxerrorlength;
         msfreya_geterrors_c_(&errors[0], &errorlength);
         if (errorlength>1) {
            printf("%s\n",errors);
            exit(1);
         }
      }
   }
   output(tke_dist, nubar_dist, nubar2_dist, n_events, ff_dist, n_dist_L, n_dist_H);
}

FILE* openfile(char* name) {
   FILE* fp = fopen(name, "w");
   if (fp == (FILE *) 0) fprintf(stderr, "Could not open %s for writing\n", name);
   return fp;
}

void init(void) {
   unsigned short int s[3] = {1234, 5678, 9012};
   int i;
   seed48(s);
   msfreya_usehostrng_c_();
   return;
}

void initFREYA(int& nisosf, int& nisoif, int& niso,
               int** ZAs, int** fistypes) {

   int maxerrorlength=10000;
   char errors[maxerrorlength];

   msfreya_reseterrorflag_c_();
   msfreya_setup_c_();
   if (msfreya_errorflagset_c_()==1) {
      int errorlength=maxerrorlength;
      msfreya_geterrors_c_(&errors[0], &errorlength);
      if (errorlength>1) {
         printf("%s\n",errors);
         exit(1);
      }
   }
   msfreya_getniso_c_(&nisosf, &nisoif);
   niso=nisosf+nisoif;

   // allocate memory to store ZA's for spontaneous and neutron-induced
   // fissions
   *ZAs = new int [niso];
   *fistypes = new int [niso];

   // Populate ZAs and fistypes
   msfreya_getzas_c_(&(*ZAs[0]),&(*fistypes[0]));
}

bool FREYA_event(double ePart, int iso, int fissiontype,
                 int*& ZAs, int*& fistypes, int niso,
                 double* tke_dist, double* nubar_dist, double* nubar2_dist, 
                 int* n_events, int* ff_dist,
                 int n_dist_L[nbinsperspectrum],
                 int n_dist_H[nbinsperspectrum]
                ) {
   // if the compound nucleus is ZA, the original nucleus was
   //   ZA for photofission
   //   Z(A-1) for neutron-induced fission
   // treat photofission as if it were neutron-induced fission
   if (fissiontype==2) iso--;
   
   // Find the index of the fission/isotope
   bool foundfission=false;
   int iKm1=0;
   for (iKm1=0; iKm1<niso; iKm1++)
      if (iso == ZAs[iKm1] && ((fissiontype==0) == (fistypes[iKm1]==0))) {
         foundfission=true;
         break;
      }
   if (!foundfission) {
      fprintf(stderr, "ABORT: fission type %d not supported for isotope %d\n", fissiontype, iso);
      exit(1);
   }

   int iK=iKm1+1; // FORTRAN indexing
   int Z=iso/1000;
   int freyaA=iso-1000*Z;
   // watch out! in freya, the A for induced fission is the A of the 
   // compound nucleus (for induced fission, add 1 neutron to the nucleus)
   freyaA+=(fissiontype==0)?0:1;
   msfreya_reseterrorflag_c_();

   // Compute nucleus excitation energy for this event
   double eps0;
   double En;
   switch (fissiontype) {
      case 0:
         // spontaneous fission
         eps0 = 0.;
         En=0.;
         break;
      case 1:
         // neutron-induced fission
      case 2:
         // photon-induced fission
         double sepni;
         sepni = msfreya_sepn_c_(iK,Z,freyaA);
         if (msfreya_errorflagset_c_()==1) return false;

         if (fissiontype==1) {
            // neutron-induced fission
            eps0 = sepni+ePart;
            En=ePart;
         } else if (fissiontype==2) {
            // photon-induced fission
            eps0 = ePart;
            En=ePart-sepni;
            if (En<0) En=0.;
         }
         break;
      default:
         fprintf(stderr, "ABORT: fission type %d not supported\n", fissiontype);
         exit(1);
         break;
   }

   // ...generate fission event
   // declare those, msfreya_event needs them
   double V0[3]; // velocity of the initial nucleus
   for (int i=0; i<3; i++) V0[i]=0; // nucleus at rest

   double P0[5]; // excited energy, momentum and kinetic energy
                 // of nucleus before interaction
   double P1[5]; // excited energy, momentum and kinetic energy
                 // of fission fragment 1
   double P2[5]; // excited energy, momentum and kinetic energy
                 // of fission fragment 2
   int Z1, A1;   // Charge & mass number of fission fragment 1
   int Z2, A2;   // Charge & mass number of fission fragment 2

   int Sf0, Sf1, Sf2; // J of CN and the two fragments

   double W0=msfreya_gsmassn_c_(Z, freyaA);  // ground-state mass of nucleus
   if (msfreya_errorflagset_c_()==1) return false;
   
   double ndir [3];              // neutron direction ((0,0,0) forces isotropic)
   for (int i=0; i<3; i++)
      ndir[i]=0.;

   P0[0]=W0+eps0;                // Rest energy of init nucleus
   double g0=1.0;                // gamma0
   P0[4]=g0*P0[0];               // Total energy of init nucleus
   for (int i=0; i<3; i++)
      P0[i+1]=P0[4]*V0[i];       // Momentum of initial nucleus

   int mult;                     // Number of particles emitted
   double particles [4*3*mMax];  // their momentum and kinetic energy
   int ptypes [3*mMax];          // their type: 0(g) & 1(n)
   int ptypes0 [mMax];           // pre-fission ejectile types
   int ptypes1 [mMax];           // types of 1st fission fragment ejectiles
   int ptypes2 [mMax];           // types of 2nd fission fragment ejectiles
   
   msfreya_event_c_(iK,En,eps0,&(P0[0]),&Z1,&A1,&(P1[0]),&Z2,&A2,&(P2[0]),&mult,&(particles[0]),&(ptypes[0]),&(ndir[0]),&Sf0,&Sf1,&Sf2);
   if (msfreya_errorflagset_c_()==1) return false;

   msfreya_getids_c_(&(ptypes0[0]),&(ptypes1[0]),&(ptypes2[0]));

   // Get total kinetic energy of fission fragments
   double tot_ke = 0;
   tot_ke += P1[4]-P1[0];
   tot_ke += P2[4]-P2[0];
   
   int AL, AH;      // Mass number of light and heavy fission fragments
   
   AL=A1;
   AH=A2;

   double f = pow(maxEnergy/minEnergy,1./nbinsperspectrum);
   double logf = log(f);

   // count number of pre-fission neutrons/photons emitted
   int npart0=0;
   for (int i=0; i<mMax; i++) {
     if (-1 == ptypes0[i]) {
       npart0=i;
       break;
     }
   }

   // count number of neutrons for fission fragment 1
   int nmultff1=0;
   // total number of secondaries from fission fragment 1
   int npart1=0;
   for (int i=0; i<mMax; i++) {
     if (1 == ptypes1[i]) {
       nmultff1++;
       double ke = getKineticEnergy(particles[(i+npart0)*4], particles[(i+npart0)*4+1], particles[(i+npart0)*4+2]);
       int energy_bin_index = (int) (log(ke/minEnergy)/logf);
       if (energy_bin_index>=0 && energy_bin_index<nbinsperspectrum) n_dist_L[energy_bin_index]++;
     } else if (-1 == ptypes1[i]) {
       npart1=i;
       break;
     }
   }

   // count number of neutrons for fission fragment 2
   int nmultff2=0;
   // total number of secondaries from fission fragment 2
   int npart2=0;
   for (int i=0; i<mMax; i++) {
     if (1 == ptypes2[i]) {
       nmultff2++;
       double ke = getKineticEnergy(particles[(i+npart0+npart1)*4], particles[(i+npart0+npart1)*4+1], particles[(i+npart0+npart1)*4+2]);
       int energy_bin_index = (int) (log(ke/minEnergy)/logf);
       if (energy_bin_index>=0 && energy_bin_index<nbinsperspectrum) n_dist_H[energy_bin_index]++;
     } else if (-1 == ptypes2[i]) {
       npart2=i;
       break;
     }
   }

   n_events[AH]++;
   tke_dist[AH] += tot_ke;
   nubar_dist[AL] += nmultff1;
   nubar2_dist[AL] += nmultff1*nmultff1;
   nubar_dist[AH] += nmultff2;
   nubar2_dist[AH] += nmultff2*nmultff2;
   ff_dist[AL]++;
   ff_dist[AH]++;

   return true;
}

void output(double* tke_dist, double* nubar_dist, double* nubar2_dist,
            int* n_events, int* ff_dist,
            int n_dist_L[nbinsperspectrum], int n_dist_H[nbinsperspectrum]) {
   char filename [1024];

   sprintf(filename, "tke_versus_AH.res");
   FILE* fp = openfile(filename);
   for (int i=0; i<maxA+1; i++) 
      if (n_events[i] != 0)
         fprintf(fp, "%d : %f\n", i, tke_dist[i]/n_events[i]);
   fclose(fp);

   sprintf(filename, "ff_yield.res");
   fp = openfile(filename);
   unsigned int sum=0;
   for (int i=0; i<maxA+1; i++) sum += ff_dist[i];
   for (int i=0; i<maxA+1; i++) fprintf(fp, "%d : %f : %f\n", i, 1.*ff_dist[i]/sum, 1.*sqrt(ff_dist[i])/sum);
   fclose(fp);

   sprintf(filename, "nubar_vs_A.res");
   fp = openfile(filename);
   for (int i=1; i<maxA+1; i++) {
     if (ff_dist[i] != 0) {
       double nubar=1.*nubar_dist[i]/ff_dist[i];
       double variance=1.*nubar2_dist[i]/ff_dist[i]-nubar*nubar;
       fprintf(fp, "%d : %f :%f\n", i, nubar,sqrt(variance));
     }
   }
   fclose(fp);

   double f = pow(maxEnergy/minEnergy,1./nbinsperspectrum);
   double logf = log(f);

   // compute sum of neutrons emitted by light/heavy fission fragments
   unsigned int sumL=0;
   for (int i=0; i<nbinsperspectrum; i++) sumL += n_dist_L[i];
   printf("Number of neutrons from light fission fragment: %d\n",sumL);

   unsigned int sumH=0;
   for (int i=0; i<nbinsperspectrum; i++) sumH += n_dist_H[i];
   printf("Number of neutrons from heavy fission fragment: %d\n",sumH);

   sum=0;
   for (int i=0; i<nbinsperspectrum; i++) sum += n_dist_L[i]+n_dist_H[i];
   printf("Total number of neutrons: %d\n",sum);

   sprintf(filename, "e_dist_L.res");
   fp = openfile(filename);
   for (int i=0; i<nbinsperspectrum; i++) fprintf(fp, "%e - %e : %f\n", minEnergy*pow(f,i), minEnergy*pow(f,i+1), 1.*n_dist_L[i]/sum);
   fclose(fp);

   sprintf(filename, "e_dist_H.res");
   fp = openfile(filename);
   // for (int i=0; i<nbinsperspectrum; i++) fprintf(fp, "%e - %e : %f\n", minEnergy*pow(f,i), minEnergy*pow(f,i+1), 1.*n_dist_H[i]/sumH);
   for (int i=0; i<nbinsperspectrum; i++) fprintf(fp, "%e - %e : %f\n", minEnergy*pow(f,i), minEnergy*pow(f,i+1), 1.*n_dist_H[i]/sum);
   fclose(fp);

   sprintf(filename, "e_dist_LH.res");
   fp = openfile(filename);
   for (int i=0; i<nbinsperspectrum; i++) fprintf(fp, "%e - %e : %f\n", minEnergy*pow(f,i), minEnergy*pow(f,i+1), 1.*(n_dist_L[i]+n_dist_H[i])/sum);
   fclose(fp);

   return;
}

double getKineticEnergy(double u, double v, double w) {
   //....calculates kinetic energy for neutron
   const double wn=939.5651828; // neutron mass in MeV
   double s=pow(u,2)+pow(v,2)+pow(w,2);
   double ke=0.5*s/wn;
   return ke;
}
