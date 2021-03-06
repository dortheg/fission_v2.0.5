#define iterations 1000000
#define maxA 200
#define maxEng 200 /* TKE can vary from 0 to 10 MeV */
#define nEng 100  /* number of energy bins */
#define mMax 50       /* The maximum number of ejectiles per fission generated by FREYA */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
                 int* sum_nu_vs_TKE, int* sum_nusquared_vs_TKE, double* tke_grid, int* n_TKE,
                 int* n_ff, int* sum_nu_vs_Af, int* sum_nusquared_vs_Af);
FILE* openfile(char* name);
void output_nu_vs_TKE(int* sum_nu_vs_TKE, int* sum_nusquared_vs_TKE, int* n_TKE, double* tke_grid);
void output_nu_vs_Af(int* n_ff, int* sum_nu_vs_Af, int* sum_nusquared_vs_Af);

int main() {
   double energy_MeV = 25.3e-9; // thermal
   int isotope = 94239;
   int fissiontype = 1; // 0: spontaneous fission
                        // 1: neutron-induced fission

   int nisosf = 0; // Number of spontaneous fission isotopes
   int nisoif = 0; // Number of induced fission isotopes
   int niso = 0;   // Number of fission isotopes

   int** ZAs;      // ZA's of fission isotopes
   int** fistypes; // types of fission [spontaneous (0), induced (1)
   ZAs = new int*;
   fistypes = new int*;

   int maxerrorlength=10000;
   char errors[maxerrorlength];

   int n_ff[maxA+1];
   int sum_nu_vs_Af[maxA+1];
   int sum_nusquared_vs_Af[maxA+1];
   for (int i=0; i<maxA+1; i++) {
      n_ff[i] = 0.;
      sum_nu_vs_Af[i] = 0.;
      sum_nusquared_vs_Af[i] = 0.;
   }

   int sum_nu_vs_TKE[nEng];
   int sum_nusquared_vs_TKE[nEng];
   int n_TKE[nEng];
   double tke_grid[nEng+1];
   for (int i=0; i<nEng; i++) {
     tke_grid[i] = 1.*maxEng/nEng*i;
     n_TKE[i] = 0;
     sum_nu_vs_TKE[i] = 0;
     sum_nusquared_vs_TKE[i] = 0;
   }
   tke_grid[nEng] = maxEng;

   init();
   initFREYA(nisosf, nisoif, niso, ZAs, fistypes);
   niso=nisosf+nisoif;
   
   for (int i=0; i<iterations; i++) {
      if (!FREYA_event(energy_MeV, isotope, fissiontype, 
                       *ZAs, *fistypes, niso,
                       sum_nu_vs_TKE, sum_nusquared_vs_TKE, tke_grid, n_TKE,
                       n_ff, sum_nu_vs_Af, sum_nusquared_vs_Af)) {
         int errorlength=maxerrorlength;
         msfreya_geterrors_c_(&errors[0], &errorlength);
         if (errorlength>1) {
            printf("%s\n",errors);
            exit(1);
         }
      }
   }
   output_nu_vs_TKE(sum_nu_vs_TKE, sum_nusquared_vs_TKE, n_TKE, tke_grid);
   output_nu_vs_Af(n_ff, sum_nu_vs_Af, sum_nusquared_vs_Af);
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
                 int* sum_nu_vs_TKE, int* sum_nusquared_vs_TKE, double* tke_grid, int* n_TKE, 
                 int* n_ff, int* sum_nu_vs_Af, int* sum_nusquared_vs_Af
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
   int nmultff1,nmultff2;        // Number of neutrons emitted by fission fragments
   double particles [4*3*mMax];  // their momentum and kinetic energy
   int ptypes [3*mMax];          // their type: 0(g) & 1(n)
   int ptypes0 [mMax];           // pre-fission ejectile types
   int ptypes1 [mMax];           // types of 1st fission fragment ejectiles
   int ptypes2 [mMax];           // types of 2nd fission fragment ejectiles
   
   msfreya_event_c_(iK,En,eps0,&(P0[0]),&Z1,&A1,&(P1[0]),&Z2,&A2,&(P2[0]),&mult,&(particles[0]),&(ptypes[0]),&(ndir[0]));
   if (msfreya_errorflagset_c_()==1) return false;

   msfreya_getids_c_(&(ptypes0[0]),&(ptypes1[0]),&(ptypes2[0]));

   // count number of neutrons
   nmultff1=0;
   nmultff2=0;
   for (int i=0; i<mMax; i++) {
     if (1 == ptypes1[i]) nmultff1=nmultff1+1;
     else if (-1 == ptypes1[i]) break;
   }
   sum_nu_vs_Af[A1]=sum_nu_vs_Af[A1]+nmultff1;
   sum_nusquared_vs_Af[A1]=sum_nusquared_vs_Af[A1]+nmultff1*nmultff1;
   n_ff[A1]++;

   for (int i=0; i<mMax; i++) {
     if (1 == ptypes2[i]) nmultff2=nmultff2+1;
     else if (-1 == ptypes2[i]) break;
   }
   sum_nu_vs_Af[A2]=sum_nu_vs_Af[A2]+nmultff2;
   sum_nusquared_vs_Af[A2]=sum_nusquared_vs_Af[A2]+nmultff2*nmultff2;
   n_ff[A2]++;

   // Get total kinetic energy of fission fragments
   double tot_ke = 0;
   tot_ke += P1[4]-P1[0];
   tot_ke += P2[4]-P2[0];
   
   int binindex=floor(tot_ke/(1.*maxEng/nEng));

   int nmult=nmultff1+nmultff2;
   if (0<=binindex && binindex<maxEng) {
     sum_nu_vs_TKE[binindex] += nmult;
     sum_nusquared_vs_TKE[binindex] += nmult*nmult;
     n_TKE[binindex]++;
   }

   return true;
}

void output_nu_vs_TKE(int* sum_nu_vs_TKE, int* sum_nusquared_vs_TKE, int* n_TKE, double* tke_grid) {
   char filename [1024];
   double mean, variance;

   sprintf(filename, "nu_vs_TKE.res");
   FILE* fp = openfile(filename);
   for (int i=0; i<nEng; i++) 
     if (n_TKE[i] != 0) {
       mean = 1.*sum_nu_vs_TKE[i]/n_TKE[i];
       variance = 1.*sum_nusquared_vs_TKE[i]/n_TKE[i]-mean*mean;
       fprintf(fp, "%4.1f - %4.1f MeV : %f : %f\n", tke_grid[i], tke_grid[i+1],
               mean, sqrt(variance));
     }
   fclose(fp);

   return;
}


void output_nu_vs_Af(int* n_ff, int* sum_nu_vs_Af, int* sum_nusquared_vs_Af) {
   char filename [1024];
   double mean, variance;

   sprintf(filename, "nu_vs_Af.res");
   FILE* fp = openfile(filename);
   for (int i=0; i<maxA+1; i++)
     if (0 != n_ff[i]) {
       mean = 1.*sum_nu_vs_Af[i]/n_ff[i];
       variance = 1.*sum_nusquared_vs_Af[i]/n_ff[i]-mean*mean;
       fprintf(fp, "%d : %f : %f\n", i, mean, sqrt(variance));
     }
   fclose(fp);

   return;
}

