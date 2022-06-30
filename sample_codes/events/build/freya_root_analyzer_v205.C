#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TROOT.h>
#include <iostream>
#include <cstdio>
#include "TFile.h"
#include "TChain.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

// root -l freya_root_analyzer_v205.C


TFile *vetsex = new TFile("252Cf.dat.root", "READ");
TTree *mytree = (TTree *) gROOT->FindObject("FreyaTree");


/*
List of meanings:

iAf1: fission fragment 1, mass number
iAf2: fission fragment 2, mass number
iAp1: product fission fragment 1, mass number (after n emission)
iAp2: product fission fragment 2, mass number (after n emission)
E1kin: kinetic energy of 1 product
E2kin: kinetic energy of second product
n0: neutrons from compound nuclei
n1: neutrons from first fission fragment
n2: neutrons from second fission fragment
m0: # pre-fission photons
m1: # photons from fragment 1
m2: # photons from fragment 2
P0: energy of neutrons emmited pre-fission (?)
P1: energy of neutrons emmited by F1
P2: energy of neutrons emitted by F2
Q0: energy of gammas emitted pre-fission
Q1: energy of gammas emitted by F1
Q2: energy of gamma emitted by F2

m_first: # photons from first chance fission
m_second: # photons from second chance fission
m_second: # photons from third chance fission

Q_first: energy from PFG's from first chance
Q_second: energy from PFG's from second chance
Q_third: energy from PFG's from third chance
*/

//Fragment mass yields
TH1D *hframe_iAf1;
TH1D *hframe_iAf2;

void create_frames();


void freya_root_analyzer_v205() {

	create_frames();

	//////////////////////////////////////
	//  Fission fragment mass distribution
	/////////////////////////////////////

	TCanvas *A_yield = new TCanvas("A_yield","Fragment Yield",150,10,990,660);
	mytree->Draw("iAf1>>hframe_iAf1");
	hframe_iAf1->SetLineColor(1);
	mytree->Draw("iAf2>>hframe_iAf2");
	hframe_iAf1->SetLineColor(2);
	hframe_iAf1->GetXaxis()->SetTitle("Mass [A]");
	hframe_iAf1->GetYaxis()->SetTitle("Counts");
	hframe_iAf1->SetTitle("Fission fragment mass distribution");
	hframe_iAf1->Draw();
	hframe_iAf2->Draw("same");

	auto legend_A_yield = new TLegend(0.7,0.75,0.9,0.9);
	legend_A_yield->SetTextSize(0.03);
	legend_A_yield->AddEntry(hframe_iAf1,     "FF1, light,","l");
	legend_A_yield->AddEntry(hframe_iAf2,     "FF2, heavy","l");
	legend_A_yield->Draw();


	cout << "Initial mass FF1 [A]: " << hframe_iAf1->GetMean() << " Initial mass FF2 [A]: " << hframe_iAf2->GetMean() << endl;
	cout << "\n" << endl;
}


void create_frames() {

	gStyle->SetOptStat(0);
	int nbins;
	int maxbin;

	hframe_iAf1 = new TH1D("hframe_iAf1","",250,0,249);
	hframe_iAf2 = new TH1D("hframe_iAf2","",250,0,249);
}




