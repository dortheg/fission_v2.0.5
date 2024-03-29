///////////////////////////////////////////////////
// Converts a FREYA event file into a ROOT tree  //
// Author: P. Papka                              //
///////////////////////////////////////////////////

#include <TMath.h>
#include <TF1.h>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


//g++ EventToRoot_compilable_v205.C -o EventToRoot_compilable_v205 `root-config --cflags --libs` -std=c++14
// ./EventToRoot_compilable_v205


int main(){
    //The root tree is based on the event file name. Just adding .root onto it
    char filename[128]="252Cf.dat";
    char rootfile[128];
    char command[128];

    int i,j,k,n,m;
    double PI=3.14159265359;
    double tPI=2.*PI;
    
    std::ifstream eventfile;
    double Z,A,Elab;
    int nbevent;
    
    int k0,iZ0,iA0,n0,m0;
    double E0;
    double p0[4][10],q0[4][20];
    double P0[10],Q0[20],P0x[10],P0y[10],P0z[10],Q0x[20],Q0y[20],Q0z[20];// these are duplicates of the matrices p0 and q0
    double th0,ph0;// azimuthal and polar angles calculated for the parent nucleus
    
    int k1,iZ1,iAf1,iAp1,n1,m1;
    double E1;
    double p1[4][10],q1[4][20];
    double P1[10],Q1[20],P1x[10],P1y[10],P1z[10],Q1x[20],Q1y[20],Q1z[20];
    double th1,ph1;// azimuthal and polar angles calculated for the fission products
    
    int k2,iZ2,iAf2,iAp2,n2,m2;
    double E2;
    double p2[4][10],q2[4][20];
    double P2[10],Q2[20],P2x[10],P2y[10],P2z[10],Q2x[20],Q2y[20],Q2z[20];
    double th2,ph2;// azimuthal and polar angles calculated for the fission products

    int first = 0;
    int second = 0;
    int third = 0;
    int m_first_total=0;

    int m_first, m_second, m_third;
    int n_first, n_second, n_third;
    
    //Photon spectrum
    double Q_first[20], Q_second[20], Q_third[20];
    //Neutron spectrum
    double P_first[20], P_second[20], P_third[20];
    
    int Mn=10;
    int Mg=20;

    sprintf(rootfile,"%s.root",filename);
    
    TFile *f1=new TFile(rootfile,"RECREATE");
    TTree *t1=new TTree("FreyaTree","Calculated Data");
    
    t1->Branch("iZ0", &iZ0, "iZ0/I" );
    t1->Branch("iZ1", &iZ1, "iZ1/I" );
    t1->Branch("iZ2", &iZ2, "iZ2/I" );
    
    t1->Branch("n0", &n0, "n0/I" );
    t1->Branch("n1", &n1, "n1/I" );
    t1->Branch("n2", &n2, "n2/I" );
    
    t1->Branch("m0", &m0, "m0/I" );
    t1->Branch("m1", &m1, "m1/I" );
    t1->Branch("m2", &m2, "m2/I" );

    t1->Branch("m_first", &m_first, "m_first/I" );
    t1->Branch("m_second", &m_second, "m_second/I" );
    t1->Branch("m_third", &m_third, "m_third/I" );

    t1->Branch("n_first", &n_first, "n_first/I" );
    t1->Branch("n_second", &n_second, "n_second/I" );
    t1->Branch("n_third", &n_third, "n_third/I" );

    t1->Branch("iA0", &iA0, "iA0/I" );
    t1->Branch("iAf1", &iAf1, "iAf1/I" );
    t1->Branch("iAf2", &iAf2, "iAf2/I" );
    t1->Branch("iAp1", &iAp1, "iAp1/I" );
    t1->Branch("iAp2", &iAp2, "iAp2/I" );
    
    t1->Branch("A", &A, "A/D" );
    t1->Branch("Z", &Z, "Z/D" );
    t1->Branch("Elab", &Elab, "Elab/D" );
    
    t1->Branch("E0", &E0, "E0/D" );
    t1->Branch("E1", &E1, "E1/D" );
    t1->Branch("E2", &E2, "E2/D" );
    
    t1->Branch("th0", &th0, "th0/D" );
    t1->Branch("th1", &th1, "th1/D" );
    t1->Branch("th2", &th2, "th2/D" );
    
    t1->Branch("ph0", &ph0, "ph0/D" );
    t1->Branch("ph1", &ph1, "ph1/D" );
    t1->Branch("ph2", &ph2, "ph2/D" );
    
    t1->Branch("Mn", &Mn, "Mn/I"    );
    t1->Branch("Mg", &Mg, "Mg/I"    );
    
    t1->Branch("P0",P0,"P0[Mn]/D");
    t1->Branch("P1",P1,"P1[Mn]/D");
    t1->Branch("P2",P2,"P2[Mn]/D");
    
    t1->Branch("P0x",P0x,"P0x[Mn]/D");
    t1->Branch("P1x",P1x,"P1x[Mn]/D");
    t1->Branch("P2x",P2x,"P2x[Mn]/D");
    
    t1->Branch("P0y",P0y,"P0y[Mn]/D");
    t1->Branch("P1y",P1y,"P1y[Mn]/D");
    t1->Branch("P2y",P2y,"P2y[Mn]/D");
    
    t1->Branch("P0z",P0z,"P0z[Mn]/D");
    t1->Branch("P1z",P1z,"P1z[Mn]/D");
    t1->Branch("P2z",P2z,"P2z[Mn]/D");
    
    t1->Branch("Q0",Q0,"Q0[Mg]/D");
    t1->Branch("Q1",Q1,"Q1[Mg]/D");
    t1->Branch("Q2",Q2,"Q2[Mg]/D");

    t1->Branch("Q_first",Q_first,"Q_first[Mg]/D");
    t1->Branch("Q_second",Q_second,"Q_second[Mg]/D");
    t1->Branch("Q_third",Q_third,"Q_third[Mg]/D");

    t1->Branch("P_first",P_first,"P_first[Mg]/D");
    t1->Branch("P_second",P_second,"P_second[Mg]/D");
    t1->Branch("P_third",P_third,"Q_third[Mg]/D");
    
    t1->Branch("Q0x",Q0x,"Q0x[Mg]/D");
    t1->Branch("Q1x",Q1x,"Q1x[Mg]/D");
    t1->Branch("Q2x",Q2x,"Q2x[Mg]/D");
    
    t1->Branch("Q0y",Q0y,"Q0y[Mg]/D");
    t1->Branch("Q1y",Q1y,"Q1y[Mg]/D");
    t1->Branch("Q2y",Q2y,"Q2y[Mg]/D");
    
    t1->Branch("Q0z",Q0z,"Q0z[Mg]/D");
    t1->Branch("Q1z",Q1z,"Q1z[Mg]/D");
    t1->Branch("Q2z",Q2z,"Q2z[Mg]/D");

    sprintf(command, "sed -i -e 's/ events/ /g' %s", filename);
    system(command);
    
    sprintf(command, "sed -i -e 's/:/ /g' %s", filename);
    system(command);
    
    eventfile.open(filename);

    //Read fission conditions
    eventfile >> Z >> A >> Elab >> nbevent;
    //std::cout << "Z: " << Z << " A: " << A << " Elab: " << Elab << " nbevent: " << nbevent << std::endl; 

    for (i=0;i<nbevent;i++){
        
        // zero everything
        for(j=0;j<4;j++) for(k=0;k<Mn;k++)  {p0[j][k]=0.;p1[j][k]=0.;p2[j][k]=0.;}
        for(j=0;j<4;j++) for(k=0;k<Mg;k++)  {q0[j][k]=0.;q1[j][k]=0.;q2[j][k]=0.;}
        for(k=0;k<Mn;k++)  {P0[k]=0.;P1[k]=0;P2[k]=0.;P0x[k]=0.;P1x[k]=0;P2x[k]=0.;P0y[k]=0.;P1y[k]=0;P2y[k]=0.;P0z[k]=0.;P1z[k]=0;P2z[k]=0.;P_first[k]=0.;P_second[k]=0.;P_third[k]=0.;}
        for(k=0;k<Mg;k++)  {Q0[k]=0.;Q1[k]=0.;Q2[k]=0.;Q0x[k]=0.;Q1x[k]=0;Q2x[k]=0.;Q0y[k]=0.;Q1y[k]=0;Q2y[k]=0.;Q0z[k]=0.;Q1z[k]=0;Q2z[k]=0.;Q_first[k]=0.;Q_second[k]=0.;Q_third[k]=0.;}

        /////////////////////////////////////////////////////////////
        // #0:	Read pre-fission information:	-----------------
        // k0	The number of this event (k0=1,...,KK)
        // iZ0 	Charge number of the fissioning nucleus
        // iA0 	Mass number of the fissioning nucleus
        // E0	Excitation energy of the fissioning nucleus
        // n0	The number of pre-fission neutrons emitted
        // m0	The number of pre-fission photons emitted
        // p0(i=0,n)  Kinetic energy of pre-fission neutron #n
        // p0(1:3,n)  Direction of pre-fission neutron #n
        // q0(i=0,m)  Kinetic energy of pre-fission photon #m
        // q0(1:3,m)  Direction of pre-fission photon #m
        /////////////////////////////////////////////////////////////
        
        eventfile >> k0 >> iZ0 >> iA0 >> E0 >> n0 >> m0;
    	//std::cout << "k0: " << k0 << " iZ0: " << iZ0 << " iA0: " << iA0 << " E0: " << E0 << " n0: " << n0 << " m0: " << m0 << std::endl; 
        
        
        //then ! read neutrons from #0:
        if (n0 > 0) for(n=1;n<=n0;n++){
         eventfile >> p0[0][n] >> p0[1][n] >> p0[2][n]>> p0[3][n];
         //std::cout << "p0[0]: " << p0[0][n] << "p0[1]: " << p0[1][n] << "p0[2]: " << p0[2][n] << "p0[3]: " << p0[3][n] << std::endl;
        }
        
        // then ! read photons from #0:
        if (m0>0) for(m=1;m<=m0;m++) eventfile >> q0[0][m] >> q0[1][m] >> q0[2][m]>> q0[3][m];


        /////////////////////////////////////////////////////////////
        // #1:	Read information for fragment #1:	
        // k1	The number of this event (k1=1,...,KK)
        // iZ1 	Charge number of primary fragment #1
        // iAf1 	Mass number of primary fragment #1
        // E1	Excitation energy of primary fragment #1
        // n1	The number of neutrons emitted from #1
        // m1	The number of photons emitted from #1
        // p1(i=0,n)  Kinetic energy of neutron #n from source #1
        // p1(1:3,n)  Direction of neutron #n from source #1
        // q1(i=0,m)  Kinetic energy of photon #m from source #1
        // q1(1:3,m)  Direction of photon #m from source #1
        /////////////////////////////////////////////////////////////

        eventfile >> k1 >> iZ1 >> iAf1 >> E1 >> n1 >> m1;
        //std::cout << "k1: " << k1 << " iZ1: " << iZ1 << " iAf1: " << iAf1 << " E1: " << E1 << " n1: " << n1 << " m1: " << m1 << std::endl;

        iAp1=iAf1-n1;//			! Product mass #1
        
        // then ! read neutrons from #1:
        if (n1>0) for(n=1;n<=n1;n++){
        eventfile >> p1[0][n] >> p1[1][n] >> p1[2][n]>> p1[3][n];
        }
        
        // then! read photons from #1:
        if (m1>0) for(m=1;m<=m1;m++) eventfile >> q1[0][m] >> q1[1][m] >> q1[2][m]>> q1[3][m];
        

        /////////////////////////////////////////////////////////////
        // #2:	Read information for fragment #2:	-----------------
        // k2	The number of this event (k2=1,...,KK)
        // iZ2 	Charge number of primary fragment #2
        // iAf2 	Mass number of primary fragment #2
        // E2	Excitation energy of primary fragment #2
        // n2	The number of neutrons emitted from #2
        // m2	The number of photons emitted from #2
        // p2(i=0,n)  Kinetic energy of neutron #n from source #2
        // p2(1:3,n)  Direction of neutron #n from source #2
        // q2(i=0,m)  Kinetic energy of photon #m from source #2
        // q2(1:3,m)  Direction of photon #m from source #2
        /////////////////////////////////////////////////////////////
        
        eventfile >> k2 >> iZ2 >> iAf2 >> E2 >> n2 >> m2 ;
       	//std::cout << "k2: " << k2 << " iZ2: " << iZ2 << " iAf2: " << iAf2 << " E2: " << E2 << " n2: " << n2 << " m2: " << m2 << std::endl;

        iAp2=iAf2-n2;//			! Product mass #2
        
        // then ! read neutrons from #2:
        if (n2>0) for(n=1;n<=n2;n++){
        eventfile >> p2[0][n] >> p2[1][n] >> p2[2][n]>> p2[3][n];
        }
        
        // then ! read photons from #2:
        if (m2>0) for(m=1;m<=m2;m++) eventfile >> q2[0][m] >> q2[1][m] >> q2[2][m]>> q2[3][m];
       
        //Handle multi-chance fission, n0 determines which chance
        if(n0==0){
            //First chance fission, count photons and neutrons
            m_first = m1 + m2;
            n_first = n1 + n2;
            first += 1;

            for(n=1;n<=m1;n++) {
                Q_first[n]=q1[0][n];
            }
            for(n=1;n<=m2;n++) {
                Q_first[m1+n]=q2[0][n];
            }

            for(n=1;n<=n1;n++) {
                P_first[n]=p1[0][n];
            }
            for(n=1;n<=n2;n++) {
                P_first[n1+n]=p2[0][n];
            }
        }

        if(n0==1){
            //Second chance fission
            m_second = m1 + m2;
            n_second = n1 + n2;
            second += 1;

            for(n=1;n<=m1;n++) {
                Q_second[n]=q1[0][n];
            }
            for(n=1;n<=m2;n++) {
                Q_second[m1+n]=q2[0][n];
            }

            for(n=1;n<=n1;n++) {
                P_second[n]=p1[0][n];
            }
            for(n=1;n<=n2;n++) {
                P_second[n1+n]=p2[0][n];
            }
        }

        if(n0==2){
            //Third chance fission
            m_third = m1 + m2;
            n_third = n1 + n2;
            third += 1;

            for(n=1;n<=m1;n++) {
                Q_third[n]=q1[0][n];
            }
            for(n=1;n<=m2;n++) {
                Q_third[m1+n]=q2[0][n];
            }

            for(n=1;n<=n1;n++) {
                P_third[n]=p1[0][n];
            }
            for(n=1;n<=n2;n++) {
                P_third[n1+n]=p2[0][n];
            }
        }


        // neutron and gamma energies + vector components
        for(n=1;n<=n0;n++) {P0[n]=p0[0][n];P0x[n]=p0[1][n];P0y[n]=p0[2][n];P0z[n]=p0[3][n];}
        for(n=1;n<=n1;n++) {P1[n]=p1[0][n];P1x[n]=p1[1][n];P1y[n]=p1[2][n];P1z[n]=p1[3][n];}
        for(n=1;n<=n2;n++) {P2[n]=p2[0][n];P2x[n]=p2[1][n];P2y[n]=p2[2][n];P2z[n]=p2[3][n];}
        
        for(n=1;n<=m0;n++) {Q0[n]=q0[0][n];Q0x[n]=q0[1][n];Q0y[n]=q0[2][n];Q0z[n]=q0[3][n];}
        for(n=1;n<=m1;n++) {Q1[n]=q1[0][n];Q1x[n]=q1[1][n];Q1y[n]=q1[2][n];Q1z[n]=q1[3][n];}
        for(n=1;n<=m2;n++) {Q2[n]=q2[0][n];Q2x[n]=q2[1][n];Q2y[n]=q2[2][n];Q2z[n]=q2[3][n];}

        // Fill the Tree
        t1->Fill();
        
    }// for all nbevents				! =======================


    eventfile >> k0>>k1>>k2;
    if(k0+k1+k2 != 0) std::cout << " Something went wrong with the event file " << k0 << " " << k1 << " " << k2 << std::endl;
    else std::cout << nbevent << " events written in " << rootfile << std::endl;

    f1=t1->GetCurrentFile();
    f1->Write();
    f1->Close();
    
    eventfile.close();

    return 0;

}

