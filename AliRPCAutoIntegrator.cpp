//
// AliRPCAutoIntegrator.cpp
//
//   Created by Gabriele Gaetano Fronzé
//   Copyright © 2016 Gabriele Gaetano Fronzé. All rights reserved.
//

#include "AliRPCAutoIntegrator.h"
// #include "TClass.h"
#include <fstream>
#include <iostream>
#include "AliCDBManager.h"
#include "AliGRPObject.h"
#include "AliCDBStorage.h"
#include "AliMpCDB.h"
#include "AliCDBEntry.h"
#include "AliMpDDLStore.h"
#include "AliDCSValue.h"
#include "AliRPCValueDCS.h"
#include "AliRPCValueCurrent.h"
#include "AliRPCValueVoltage.h"
#include "AliRPCValueScaler.h"
#include "AliRPCOverflowStatistics.h"
#include "AliMUONTriggerScalers.h"
#include "TList.h"
#include "TClonesArray.h"
#include "TGraph.h"
#include "TH1F.h"

using namespace std;

ClassImp(AliRPCAutoIntegrator);

////////////////////////////////////////////////////////////////////////////////
// Filling of data members (const) needed for conversione notation.
// This method allows a direct filling of arrays even if defined as const void*
////////////////////////////////////////////////////////////////////////////////

static const Int_t RPCIndexes[] = {5,6,7,8,9,9,8,7,6,5,4,3,2,1,1,2,3,4};
const Int_t *AliRPCAutoIntegrator::kRPCIndexes = RPCIndexes;

static const Int_t RPCSides[] = {0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0};
const Int_t *AliRPCAutoIntegrator::kRPCSides = RPCSides;

static const TString fSides_[] = {"INSIDE","OUTSIDE"};
const TString *AliRPCAutoIntegrator::fSides = fSides_;
static const Int_t fPlanes_[] = {11,12,21,22};
const Int_t *AliRPCAutoIntegrator::fPlanes = fPlanes_;
static const TString fCathodes_[] = {"BENDING","NOT BENDING"};
const TString *AliRPCAutoIntegrator::fCathodes = fCathodes_;

static const Int_t fColors_[] = {kBlack,kRed,kGreen,kBlue,kViolet+7,kMagenta,kCyan,kGray,kOrange};
const Int_t *AliRPCAutoIntegrator::fColors = fColors_;
static const Int_t fStyles_[]={20,24,21,25};
const Int_t *AliRPCAutoIntegrator::fStyles = fStyles_;

// Default constructor
AliRPCAutoIntegrator::AliRPCAutoIntegrator(){
    for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            if(iRPC==5){
                if(iPlane<2){
                    fRPCAreas[iRPC][iPlane]=16056.;
                } else {
                    fRPCAreas[iRPC][iPlane]=18176.;
                }
            }else if(iRPC==4 || iRPC==6){
                if(iPlane<2){
                    fRPCAreas[iRPC][iPlane]=19728./28.*27.;
                } else {
                    fRPCAreas[iRPC][iPlane]=22338./28.*27.;
                }
            }else {
                if(iPlane<2){
                    fRPCAreas[iRPC][iPlane]=19728.;
                } else {
                    fRPCAreas[iRPC][iPlane]=22338.;
                }
            }
        }
    }
};

// Class constructor
AliRPCAutoIntegrator::AliRPCAutoIntegrator(TString RunListFileName, TString AMANDAInputFileName, TString OutputFileName, Bool_t updateOCDB, Bool_t updateAMANDA):
fRunListFileName(RunListFileName),
fAMANDAInputFileName(AMANDAInputFileName),
fOutputFileName(OutputFileName),
fUpdateOCDB(updateOCDB),
fUpdateAMANDA(updateAMANDA){

    // The update data variable allows the user to decide wether the data has to
    // be reloaded in the *DataContainer.root files or not (data already there)
    if(fUpdateOCDB){
        fOCDBDataContainer= new TFile("OCDBDataContainer.root","UPDATE");
        //fAMANDADataContainer= new TFile("AMANDADataContainer.root","RECREATE");
    } else {
        fOCDBDataContainer= new TFile("OCDBDataContainer.root","READ");
        //fAMANDADataContainer= new TFile("AMANDADataContainer.root","READ");
    }

    if(fUpdateAMANDA){
        //fOCDBDataContainer= new TFile("OCDBDataContainer.root","RECREATE");
        fAMANDADataContainer= new TFile("AMANDADataContainer.root","UPDATE");
    } else {
        //fOCDBDataContainer= new TFile("OCDBDataContainer.root","READ");
        //fAMANDADataContainer= new TFile("AMANDADataContainer.root","READ");
        fAMANDADataContainer= new TFile("AMANDADataContainer.root","UPDATE");
    }

    //fGlobalDataContainer= new TFile(Form("%s",OutputFileName.Data()),"RECREATE");
    fGlobalDataContainer= new TFile(Form("%s",OutputFileName.Data()),"UPDATE");

    fGlobalDataContainer->cd();
    fGlobalDataContainer->mkdir("TLists");
    fGlobalDataContainer->mkdir("AMANDA_iNet_Graphs");
    fGlobalDataContainer->mkdir("AMANDA_integrated_charge_Graphs");

    // Calling this method to preload the runs of which the OCDB data has to be
    // downloaded
    OCDBRunListReader();

    for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            if(iRPC==5){
                if(iPlane<2){
                    fRPCAreas[iRPC][iPlane]=16056.;
                } else {
                    fRPCAreas[iRPC][iPlane]=18176.;
                }
            }else if(iRPC==4 || iRPC==6){
                if(iPlane<2){
                    fRPCAreas[iRPC][iPlane]=19728./28.*27.;
                } else {
                    fRPCAreas[iRPC][iPlane]=22338./28.*27.;
                }
            }else {
                if(iPlane<2){
                    fRPCAreas[iRPC][iPlane]=19728.;
                } else {
                    fRPCAreas[iRPC][iPlane]=22338.;
                }
            }
        }
    }
}

//  Destructor will dellaocate any data member allocated in the heap
AliRPCAutoIntegrator::~AliRPCAutoIntegrator(){
    fOCDBDataContainer->Close();
    fAMANDADataContainer->Close();
    fGlobalDataContainer->Close();

    // delete fOCDBDataContainer;
    // delete fAMANDADataContainer;
    // delete fGlobalDataContainer;
    // delete fSides;
    // delete fPlanes;
    // delete fCathodes;
    // delete kRPCIndexes;
    // delete kRPCSides;
}

// This method is the only public one and is a wrapper of the various methods
// which have to be called in sequence to
void AliRPCAutoIntegrator::RunAutoIntegrator(){

    // If user wants to update the data then the macro has to connect to OCDB.
    // Note that if data are already in the file there is nothing else to do.
    if (fUpdateOCDB){
        cout<<"Starting OCDB data retrieving...\n";
        OCDBDataToCParser();
        cout<<"DONE\n"<<endl;
    }

    if (fUpdateAMANDA){
        cout<<"Starting AMANDA data parsing...\n";
        AMANDATextToCParser();

        cout<<"Setting Amanda RunNumbers...\n";
        AMANDASetDataMembers();
        cout<<"DONE\n"<<endl;
    }

    cout<<"Starting data aggregation...\n";
    Aggregator();
    cout<<"DONE\n"<<endl;

    cout<<"Starting plot generation...\n";
    GeneratePlots();
    cout<<"DONE\n"<<endl;

    cout<<"Starting data elaboration...\n";
    Subtractor();
    cout<<"DONE\n"<<endl;

    cout<<"Starting integrated charge calculation...\n";
    Integrator();
    cout<<"DONE\n"<<endl;

    cout<<"Starting filling AliRPCdata...\n";
    FillAliRPCData();
    cout<<"DONE\n"<<endl;

    fOCDBDataContainer->Close();
    fAMANDADataContainer->Close();
    fGlobalDataContainer->Close();
}

// Method to parse a text file containing the run list for OCDB downloader
void AliRPCAutoIntegrator::OCDBRunListReader(){
    ifstream fin;
    OCDBRun runBuffer;
    runBuffer.year = 0000;
    fin.open(fRunListFileName.Data());
    while(kTRUE){
        fin >> runBuffer.runNumber;
        fOCDBRunList.push_back(runBuffer);
        if(fin.eof())break;
    }
    fin.close();
}

// Method to merge the two files: AMANDA data and OCDB data.
// This procedure is needed to have chronologically ordered data with continuous
// current measurements (AMANDA) and "dark current" measurements from without
// beam runs (OCDB). This allows one to use subtract the dark current from
// AMANDA data in the most detailed way.
void AliRPCAutoIntegrator::Aggregator(){
    TList *listBufferAMANDA = 0x0;
    TList *listBufferOCDB = 0x0;
    TList *mergedData[kNSides][kNPlanes][kNRPC];

    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){

                fAMANDADataContainer->GetObject(Form("AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1), listBufferAMANDA);
                fOCDBDataContainer->GetObject(Form("OCDB_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1), listBufferOCDB);

                // if any data list is missing, then the channel
                // (aka {iSide,iPlane,iRPC}) is skipped from the whole following
                // analysis
                if (!listBufferAMANDA) {
                    printf("AMANDA_Data_MTR_%s_MT%d_RPC%d NOT FOUND\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);
                    continue;
                }
                if (!listBufferOCDB) {
                    printf("OCDB_Data_MTR_%s_MT%d_RPC%d NOT FOUND\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);
                    continue;
                }

                mergedData[iSide][iPlane][iRPC]=new TList();
                mergedData[iSide][iPlane][iRPC]->SetName(Form("OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1));

                TIter iterValueAMANDA(listBufferAMANDA);
                while(iterValueAMANDA()){
                    ((AliRPCValueCurrent*)*iterValueAMANDA)->SetIsAMANDA(kTRUE);
                    mergedData[iSide][iPlane][iRPC]->Add(*iterValueAMANDA);
                }


                TIter iterValueOCDB(listBufferOCDB);
                // UInt_t previousRun = 0;
                // Int_t measuresCounter = 0;
                // Double_t measuresCumulus = 0.;
                // while(iterValueOCDB()){
                //     //((AliRPCValueCurrent*)*iterValueAMANDA)->SetIsAMANDA(kFALSE);
                //     if (previousRun == 0) previousRun = ((AliRPCValueCurrent*)*iterValueOCDB)->GetRunNumber();
                //
                //     if (previousRun == ((AliRPCValueCurrent*)*iterValueOCDB)->GetRunNumber()) {
                //         measuresCounter++;
                //         measuresCumulus+=((AliRPCValueCurrent*)*iterValueOCDB)->GetITot();
                //     } else {
                //         mergedData[iSide][iPlane][iRPC]->Add(*iterValueOCDB);
                //         if ( ((AliRPCValueCurrent*)*iterValueOCDB)->IsCurrent() ){
                //         if (((AliRPCValueCurrent*)*iterValueOCDB)->GetTimeStamp()>8000 && ((AliRPCValueCurrent*)*iterValueOCDB)->GetITot()>0.)
                //             OCDBPlotsIDark[iSide][iPlane][iRPC]->SetPoint(counter++, ((AliRPCValueCurrent*)*iterValueOCDB)->GetTimeStamp(), measuresCumulus/(Double_t)measuresCounter);
                //         }
                //         measuresCounter = 0;
                //         measuresCumulus = 0.;
                //         previousRun = ((AliRPCValueCurrent*)*iterValueOCDB)->GetRunNumber();
                //     }
                // }

                while(iterValueOCDB()){
                    //((AliRPCValueCurrent*)*iterValueAMANDA)->SetIsAMANDA(kFALSE);
                    mergedData[iSide][iPlane][iRPC]->Add(*iterValueOCDB);
                }

                // the sorting will take place with respect to the timestamp of
                // each entry
                mergedData[iSide][iPlane][iRPC]->Sort();

                fGlobalDataContainer->cd("TLists");
                mergedData[iSide][iPlane][iRPC]->Write(Form("OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kSingleKey | TObject::kOverwrite);
                WhichRPC(iRPC, iSide, iPlane);

                listBufferAMANDA = 0x0;
                listBufferOCDB = 0x0;
            }
        }
    }
}

void AliRPCAutoIntegrator::GeneratePlots() {
    TGraph *PlotsITot[kNSides][kNPlanes][kNRPC];
    TGraph *PlotsIDark[kNSides][kNPlanes][kNRPC];
    TGraph *PlotsVoltage[kNSides][kNPlanes][kNRPC];
    TList *listBuffer;

    fGlobalDataContainer->mkdir("AMANDA_iTot_Graphs");
    fGlobalDataContainer->mkdir("OCDB_iDark_Graphs");
    fGlobalDataContainer->mkdir("Voltage_Graphs");


    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                fGlobalDataContainer->GetObject(Form("TLists/OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1), listBuffer);

                if (!listBuffer) {
                    printf("TLists/OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d NOT FOUND\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);
                    continue;
                }

                PlotsITot[iSide][iPlane][iRPC] = new TGraph();
                PlotsITot[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsITot[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsITot[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsITot[iSide][iPlane][iRPC]->SetMarkerSize(0.15);

                PlotSomethingVersusTime(PlotsITot[iSide][iPlane][iRPC],&AliRPCValueDCS::IsOkForITot, listBuffer, AliRPCValueCurrent::kITot);

                fGlobalDataContainer->cd("AMANDA_iTot_Graphs");
                PlotsITot[iSide][iPlane][iRPC]->Write(Form("AMANDA_iTot_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1));


                PlotsIDark[iSide][iPlane][iRPC]=new TGraph();
                PlotsIDark[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsIDark[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsIDark[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsIDark[iSide][iPlane][iRPC]->SetMarkerSize(0.15);

                PlotSomethingVersusTime(PlotsIDark[iSide][iPlane][iRPC],&AliRPCValueDCS::IsOkForIDark, listBuffer, AliRPCValueCurrent::kIDark);

                fGlobalDataContainer->cd("OCDB_iDark_Graphs");
                PlotsIDark[iSide][iPlane][iRPC]->Fit("pol0","Q");
                PlotsIDark[iSide][iPlane][iRPC]->Write(Form("OCDB_iDark_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1));


                PlotsVoltage[iSide][iPlane][iRPC]=new TGraph();
                PlotsVoltage[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsVoltage[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsVoltage[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsVoltage[iSide][iPlane][iRPC]->SetMarkerSize(0.15);

                PlotSomethingVersusTime(PlotsVoltage[iSide][iPlane][iRPC],&AliRPCValueDCS::IsVoltage,listBuffer);

                fGlobalDataContainer->cd("Voltage_Graphs");
                PlotsVoltage[iSide][iPlane][iRPC]->Write(Form("Voltage_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1));

                WhichRPC(iRPC,iSide,iPlane);

                listBuffer=0x0;
            }
        }
    }
}

// This method is one of the possible ones for the dark current subtraction. the
// basic idea is to subtract to AMANDA total currents the dark currents
// retrieved from OCDB. Since the OCDB has way less granularity than AMANDA, one
// should choose a method to interpolate the dark current readings:
//    * first possibility is to read the first dark current value and keep it
//      flat up to the following OCDB current value. In that case the dark
//      current plot will present a "stepping" trend;
//    * second possibility is to interpolate linearly two adjacent OCDB dark
//      current readings and compute, using a linear function evaluation, a
//      dark current value for each AMANDA value falling between the two
//      interpolated dark current values.
void AliRPCAutoIntegrator::Subtractor(){
    TList *buffer;
    TGraph *AMANDAPlotsINet[kNSides][kNPlanes][kNRPC];

    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                fGlobalDataContainer->GetObject(Form("/TLists/OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),buffer);

                // if any data list is missing, then the channel
                // (aka {iSide,iPlane,iRPC}) is skipped from the whole following
                // analysis
                if (!buffer){
                    printf("OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d NOT FOUND\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);
                    continue;
                }

                AMANDAPlotsINet[iSide][iPlane][iRPC]=new TGraph();
                AMANDAPlotsINet[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                AMANDAPlotsINet[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                AMANDAPlotsINet[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                AMANDAPlotsINet[iSide][iPlane][iRPC]->SetMarkerSize(0.15);

                TIter iterValueGlobal(buffer);
                Double_t darkCurrentValue = 0.;
                Double_t startTimeStamp = 0;
                Int_t counter=0;
                while(iterValueGlobal()){

                    // if the read value is an AMANDA reading, then the dark
                    // current subtraction must take place. To do that via
                    // interpolation, having stored the previous dark current
                    // reading in darkCurrentValue, one should look for the
                    // following dark current value and the interpolate.
                    if ( ((AliRPCValueDCS*)*iterValueGlobal)->IsAMANDA() ){

                        // Looking for the following not AMANDA (aka OCDB)
                        // dark current reading.
                        TIter iterValueGlobalNext = iterValueGlobal;
                        while ( iterValueGlobalNext() ){
                            if ( !((AliRPCValueDCS*)*iterValueGlobalNext)->IsAMANDA() ) break;
                        }

                        // whenever a good OCDB reading is found then proceed
                        // with linear interpolation.
                        if (*iterValueGlobalNext) {
                            Double_t iDarkt0 = darkCurrentValue;
                            Double_t t0 = startTimeStamp;
                            Double_t iDarkt1 = ((AliRPCValueCurrent*)*iterValueGlobalNext)->GetITot();
                            Double_t t1 = ((AliRPCValueCurrent*)*iterValueGlobalNext)->GetTimeStamp();
                            Double_t tnow = ((AliRPCValueCurrent*)*iterValueGlobal)->GetTimeStamp();

                            Double_t darkCurrent = tnow * (iDarkt1-iDarkt0)/(t1-t0) + iDarkt0;

                            // the dark current value if forced to be positive
                            if (darkCurrent<0.) darkCurrent=0.;

                            // the subtraction is not direct: the dark current
                            // value is set for each AMANDA reading.
                            // The subtraction will take place at the moment of
                            // asking the AMANDA reading the iNET value
                            // (since it returns iTOT-iDARK).
                            ((AliRPCValueCurrent*)*iterValueGlobal)->SetIDark(darkCurrent);
                            //cout<<((AliRPCValueCurrent*)*iterValueGlobal)->GetINet()<<endl;
                            if (((AliRPCValueCurrent*)*iterValueGlobal)->GetTimeStamp()>8000 && ((AliRPCValueCurrent*)*iterValueGlobal)->GetINet()>0.)
                                AMANDAPlotsINet[iSide][iPlane][iRPC]->SetPoint(counter++, ((AliRPCValueCurrent*)*iterValueGlobal)->GetTimeStamp(), ((AliRPCValueCurrent*)*iterValueGlobal)->GetINet()/fRPCAreas[iRPC][iPlane]);
                        }
                    }
                    // if a new dark current reding is found (non AMANDA = OCDB)
                    // then the dark current value is updated (as well as the
                    // timestamp)
                    else{
                        darkCurrentValue = ((AliRPCValueCurrent*)*iterValueGlobal)->GetITot();
                        startTimeStamp = ((AliRPCValueCurrent*)*iterValueGlobal)->GetTimeStamp();
                    }
                }

                WhichRPC(iRPC, iSide, iPlane);

                fGlobalDataContainer->cd("AMANDA_iNet_Graphs");
                AMANDAPlotsINet[iSide][iPlane][iRPC]->Write(Form("AMANDA_iNet_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1));
            }
        }
    }
}

// This method integrates vs time the iNET valueDCS in order to get the
// integrated charge for each channel. And plots it.
void AliRPCAutoIntegrator::Integrator(){
    TGraph *buffer;
    TGraph *AMANDAPlotsIntegratedCharge[kNSides][kNPlanes][kNRPC];



    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                fGlobalDataContainer->GetObject(Form("AMANDA_iNet_Graphs/AMANDA_iNet_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),buffer);

                // if any data list is missing, then the channel
                // (aka {iSide,iPlane,iRPC}) is skipped from the whole following
                // analysis
                if (!buffer){
                    printf("AMANDA_iNet_Graphs/AMANDA_iNet_Graph_MTR_%s_MT%d_RPC%d NOT FOUND\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);
                    continue;
                }

                AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]=new TGraph();
                AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]->SetMarkerSize(0.15);

                Double_t previousTimestamp = 0.;
                Double_t integratedCharge = 0.;
                Int_t counter = 0;

                // The integration takes place running on the TGraph points. The
                // adopted integration method is the trapezoidal rule.
                for(Int_t iPoint=0; iPoint<buffer->GetN()-1; iPoint++){
                    Double_t timestamp0,timestamp1;
                    Double_t iNet0,iNet1;
                    buffer->GetPoint(iPoint, timestamp0, iNet0);
                    buffer->GetPoint(iPoint+1, timestamp1, iNet1);

                    // the integrated charge is an incremental value
                    integratedCharge+=(timestamp1-timestamp0)*(iNet0+iNet1)/2.;

                    AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]->SetPoint(counter++, (timestamp0+timestamp1)/2., integratedCharge);
                }

                printf("MTR_%s_MT%d_RPC%d %f\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1,integratedCharge);

                // for(Int_t iPoint=0; iPoint<buffer->GetN(); iPoint++){
                //     Double_t timestamp;
                //     Double_t iNet;
                //     buffer->GetPoint(iPoint, timestamp, iNet);
                //     if (previousTimestamp == 0.) previousTimestamp = timestamp;
                //     integratedCharge+=(timestamp-previousTimestamp)*iNet;
                //     AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]->SetPoint(counter++, timestamp, integratedCharge);
                // }

                fGlobalDataContainer->cd("AMANDA_integrated_charge_Graphs");
                AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]->Write(Form("AMANDA_integrated_charge_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1));

                WhichRPC(iRPC, iSide, iPlane);

            }
        }
    }
}

void AliRPCAutoIntegrator::AMANDATextToCParser(){

    Int_t mts[23];
    mts[11]=0;
    mts[12]=1;
    mts[21]=2;
    mts[22]=3;

    TList *bufferOutputList;

	TList *data[kNSides][kNPlanes][kNRPC];
    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                // The idea is to try to get the corresponding object from the
                // root file. If it is already there the new data will be
                // added to the list after the data already there. If not the
                //  object will be created.
                fAMANDADataContainer->cd();
                fAMANDADataContainer->GetObject(Form("AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),bufferOutputList);
                if (!bufferOutputList) {
                    data[iSide][iPlane][iRPC]=new TList();
                    data[iSide][iPlane][iRPC]->SetName(Form("AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1));
                } else {
                    data[iSide][iPlane][iRPC] = bufferOutputList;
                }
                bufferOutputList = 0x0;
                //cout<<"created "<<data[iSide][iPlane][iRPC]->GetName()<<endl;
            }
        }
    }

    Bool_t isZero=kFALSE;

    Double_t dummyTimeStamp=0.;
    Double_t timeStamp=0.;
    Double_t current=0.;
    Int_t MT=0;
    Int_t RPC=0;
    char InsideOutside='I';

    string line;
    ifstream fin (fAMANDAInputFileName.Data());
    if (fin.is_open())
    {
        while (! fin.eof() )
        {
          getline (fin,line);
          if (fin.eof()) break;
          const char *charbuffer = (char*)line.c_str();
          if (!charbuffer) continue;
          sscanf(charbuffer,"%lf;MTR_%c",&dummyTimeStamp,&InsideOutside);
          char pattern[200];
          sprintf(pattern,"%%lf;MTR_%sSIDE_MT%%d_RPC%%d_HV.actual.iMon;%%lf",(InsideOutside=='I'?"IN":"OUT"));
          sscanf(charbuffer,pattern,&timeStamp,&MT,&RPC,&current);
        //   cout<<current<<endl;

          if((current!=0. || (current==0. && isZero)) && timeStamp>8000000.){
            //printf("%f %c=%d %d %d %.17f\n\n",timeStamp,InsideOutside,(InsideOutside=='I'?0:1),MT,RPC,current);
            //cout<<timeStamp<<endl;
            AliRPCValueCurrent *currentBuffer = new AliRPCValueCurrent(0, timeStamp, current, 0., kFALSE,"",0.,"", 0, kTRUE);
            currentBuffer->SetIsAMANDA(kTRUE);
            // //if (timeStamp<8000000) continue;
            //if (!(data[(InsideOutside=='I'?0:1)][mts[MT]][RPC-1])) continue;
            data[(InsideOutside=='I'?0:1)][mts[MT]][RPC-1]->Add(currentBuffer);
            currentBuffer = 0x0;
        } //else cout<<"#### skip ###"<<endl;
        }
        fin.close();
    }
    else cout << "Unable to open file";

    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                fAMANDADataContainer->cd();
                if (data[iSide][iPlane][iRPC]->GetEntries()==0) continue;
                data[iSide][iPlane][iRPC]->Write(Form("AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kSingleKey | TObject::kOverwrite);

            }
        }
    }
}

void AliRPCAutoIntegrator::OCDBDataToCParser(){

    //flag che è kTRUE se l'evento è di calibrazione
    Bool_t isCalib=kFALSE;
    Bool_t isBeamPresent=kFALSE;

    AliCDBManager *managerYearCheck = AliCDBManager::Instance();
    for (std::vector<OCDBRun>::iterator runIteratorYearChecker = fOCDBRunList.begin(); runIteratorYearChecker != fOCDBRunList.end(); ++runIteratorYearChecker) {
        for (Int_t year = 2016; year>2009; year--){
            managerYearCheck->SetDefaultStorage(Form("alien://folder=/alice/data/%d/OCDB",year));
            AliCDBStorage *defStorageYear = managerYearCheck->GetDefaultStorage();
            defStorageYear->QueryCDB((*runIteratorYearChecker).runNumber);
            TObjArray* arrCDBIDYear = defStorageYear->GetQueryCDBList();
            TIter nxt(arrCDBIDYear);
            AliCDBId* cdbIDYear = 0;
            Bool_t hasGRP = kFALSE;
            while ((cdbIDYear=(AliCDBId*)nxt())) {
                if (cdbIDYear->GetPath() == "GRP/GRP/Data") {hasGRP = kTRUE; break;}
            }
            if(!hasGRP){
                printf("\n\n### Can't find run %d\n\n",(*runIteratorYearChecker).runNumber);
                (*runIteratorYearChecker).year = 0000;
                continue;
            } else {
                printf("\n\n### Run %d belongs to year %d\n\n",(*runIteratorYearChecker).runNumber,year);
                (*runIteratorYearChecker).year = year;
                break;
            }
        }
    }

    //manager per interfacciarsi con gli OCDB
    AliCDBManager *managerCurrent = AliCDBManager::Instance();
    AliCDBManager *managerVoltage = AliCDBManager::Instance();
    AliCDBManager *managerRunType = AliCDBManager::Instance();
    AliCDBManager *managerScaler  = AliCDBManager::Instance();


    //array 3D di liste di dati. le TList sono già ordinate dopo ogni inserimento
    TList *dataList[kNSides][kNPlanes][kNRPC];
    TList *scalersDataList[2][kNSides][kNPlanes][kNRPC];
    TList *scalersLocalBoardList[kNCathodes][kNPlanes][kNLocalBoards];

    TList *bufferDataList;
    TList *bufferScalersDataList0;
    TList *bufferScalersDataList1;

    for (Int_t plane=0; plane<kNPlanes; plane++) {
        for (Int_t side=0; side<kNSides; side++) {
            for (Int_t RPC=1; RPC<=kNRPC; RPC++) {
                // The idea is to try to get the corresponding object from the
                // root file. If it is already there the new data will be
                // added to the list after the data already there. If not the
                //  object will be created.
                fOCDBDataContainer->cd();
                fOCDBDataContainer->GetObject(Form("OCDB_Data_MTR_%s_MT%d_RPC%d",(fSides[side]).Data(),fPlanes[plane],RPC),bufferDataList);
                fOCDBDataContainer->GetObject(Form("OCDB_Scalers_MTR_%s_%s_MT%d_RPC%d",(fSides[side]).Data(),(fCathodes[0]).Data(),fPlanes[plane],RPC),bufferScalersDataList0);
                fOCDBDataContainer->GetObject(Form("OCDB_Scalers_MTR_%s_%s_MT%d_RPC%d",(fSides[side]).Data(),(fCathodes[1]).Data(),fPlanes[plane],RPC),bufferScalersDataList1);

                if (!bufferDataList) {
                    dataList[side][plane][RPC-1]=new TList();
                    dataList[side][plane][RPC-1]->SetName(Form("OCDB_Data_MTR_%s_MT%d_RPC%d",(fSides[side]).Data(),fPlanes[plane],RPC));
                } else {
                    dataList[side][plane][RPC-1] = bufferDataList;
                }
                bufferDataList = 0x0;

                if (!bufferScalersDataList0) {
                    scalersDataList[0][side][plane][RPC-1]=new TList();
                    scalersDataList[0][side][plane][RPC-1]->SetName(Form("OCDB_Scalers_MTR_%s_%s_MT%d_RPC%d",(fSides[side]).Data(),(fCathodes[0]).Data(),fPlanes[plane],RPC));
                } else {
                    scalersDataList[0][side][plane][RPC-1] = bufferScalersDataList0;
                }
                bufferScalersDataList0 = 0x0;

                if (!bufferScalersDataList1) {
                    scalersDataList[1][side][plane][RPC-1]=new TList();
                    scalersDataList[1][side][plane][RPC-1]->SetName(Form("OCDB_Scalers_MTR_%s_%s_MT%d_RPC%d",(fSides[side]).Data(),(fCathodes[1]).Data(),fPlanes[plane],RPC));
                } else {
                    scalersDataList[1][side][plane][RPC-1] = bufferScalersDataList1;
                }
                bufferScalersDataList1 = 0x0;


            }
        }
    }

    bufferDataList = 0x0;
    bufferScalersDataList0 = 0x0;
    bufferScalersDataList1 = 0x0;

    TList *bufferScalersLocalBoardList1;

    for(Int_t cathode=0;cathode<kNCathodes;cathode++){
        for(Int_t plane=0;plane<kNPlanes;plane++){
            for(Int_t local=0;local<kNLocalBoards;local++){
                // The idea is to try to get the corresponding object from the
                // root file. If it is already there the new data will be
                // added to the list after the data already there. If not the
                //  object will be created.
                fOCDBDataContainer->cd();
                fOCDBDataContainer->GetObject(Form("OCDB_Scalers_MTR_%s_MT%d_LB%d",(fCathodes[cathode]).Data(),fPlanes[plane],local+1),bufferScalersLocalBoardList1);

                if (!bufferScalersLocalBoardList1) {
                    scalersLocalBoardList[cathode][plane][local]=new TList();
                    scalersLocalBoardList[cathode][plane][local]->SetName(Form("OCDB_Scalers_MTR_%s_MT%d_LB%d",(fCathodes[cathode]).Data(),fPlanes[plane],local+1));
                } else {
                    scalersLocalBoardList[cathode][plane][local] = bufferScalersLocalBoardList1;
                }
                bufferScalersLocalBoardList1 = 0x0;
            }
        }
    }

    bufferScalersLocalBoardList1 = 0x0;

    //loop sui run inseriti
    for (std::vector<OCDBRun>::iterator runIterator = fOCDBRunList.begin(); runIterator != fOCDBRunList.end(); ++runIterator) {

        if ((*runIterator).year == 0) continue;

        //inizializzazione dei manager
        managerCurrent->SetDefaultStorage(Form("alien://folder=/alice/data/%d/OCDB",(*runIterator).year));
        managerVoltage->SetDefaultStorage(Form("alien://folder=/alice/data/%d/OCDB",(*runIterator).year));
        managerRunType->SetDefaultStorage(Form("alien://folder=/alice/data/%d/OCDB",(*runIterator).year));
        managerScaler->SetDefaultStorage(Form("alien://folder=/alice/data/%d/OCDB",(*runIterator).year));

        //i manager puntano al run desiderato
        managerCurrent->SetRun((*runIterator).runNumber);
        managerVoltage->SetRun((*runIterator).runNumber);
        managerRunType->SetRun((*runIterator).runNumber);
        managerScaler->SetRun((*runIterator).runNumber);

        AliCDBStorage *defStorage = managerCurrent->GetDefaultStorage();
        defStorage->QueryCDB((*runIterator).runNumber);
        TObjArray* arrCDBID = defStorage->GetQueryCDBList();
        TIter nxt(arrCDBID);
        AliCDBId* cdbID = 0;
        Bool_t hasGRP = kFALSE;
        while ((cdbID=(AliCDBId*)nxt())) {
            if (cdbID->GetPath() == "GRP/GRP/Data") {hasGRP = kTRUE; break;}
        }
        if(!hasGRP){
            printf("\n\nSkipping run %d\n\n",(*runIterator).runNumber);
            continue;
        }

        if(!AliMpCDB::LoadDDLStore()) continue;
        AliMpDDLStore *ddlStore=AliMpDDLStore::Instance();

        //inizializzazione dell'entry contente il runtype
        AliCDBEntry *entryRunType = managerRunType->Get("GRP/GRP/Data");
        if(CheckPointer((TNamed*)entryRunType)) continue;

        //retrievering delle informazioni sul run
        AliGRPObject *grpObj=(AliGRPObject*)entryRunType->GetObject();
        if(CheckPointer((TNamed*)grpObj)) continue;
        TString *runType=new TString(grpObj->GetRunType());
        TString *beamType=new TString(grpObj->GetBeamType());
        Float_t beamEnergy = grpObj->GetBeamEnergy();
        TString *LHCState = new TString(grpObj->GetLHCState());
        Long64_t SOR=(Long64_t)grpObj->GetTimeStart();
        Long64_t EOR=(Long64_t)grpObj->GetTimeEnd();

        //settaggio del flag isCalib
        if(runType->Contains("PHYSICS")){
            isCalib=kFALSE;
            //cout<<(*runIterator).runNumber<<" is phys"<<endl;
        } else if(runType->Contains("CALIBRATION")){
            isCalib=kTRUE;
            //cout<<(*runIterator).runNumber<<" is calib"<<endl;
        } else {
            continue;
        }

        //settaggio del flag beamPresence
        isBeamPresent = (beamEnergy > 1.) ? kTRUE : kFALSE ;

        printf("-------------\n#####   Beam energy:%f Beam presence:%s Beam type:%s LHC State:%s \n-------------\n",beamEnergy,(isBeamPresent) ? "true" : "false",beamType->Data(), LHCState->Data());

        //cout<<isCalib<<endl;

        //inizializzazione dell'entry contente i valori di corrente
        AliCDBEntry *entryCurrent = managerCurrent->Get("MUON/Calib/TriggerDCS");
        if(CheckPointer((TNamed*)entryCurrent)) continue;

        //mappa delle correnti
        TMap *mapCurrent = (TMap*)entryCurrent->GetObject();
        if(CheckPointer((TNamed*)mapCurrent)) continue;

        //inizializzazione dell'entry contente i valori di tensione
        AliCDBEntry *entryVoltage = managerVoltage->Get("MUON/Calib/TriggerDCS");
        if(CheckPointer((TNamed*)entryVoltage)) continue;

        //mappa delle tensioni
        TMap *mapVoltage = (TMap*)entryVoltage->GetObject();
        if(CheckPointer((TNamed*)mapVoltage)) continue;

        //inizializzazone dell'entry contenente le letture degli scalers
        AliCDBEntry *entryScalers = managerScaler->Get("MUON/Calib/TriggerScalers");
        if(CheckPointer((TNamed*)entryScalers)) continue;

        //array delle letture
        TClonesArray *arrayScalers = (TClonesArray*)entryScalers->GetObject();
        if(CheckPointer((TNamed*)arrayScalers)) continue;


        //loop sui piani, i lati (inside e outside) e le RPC (9 per side)
        for (Int_t plane=0; plane<kNPlanes; plane++) {
            for (Int_t side=0; side<kNSides; side++) {
                for (Int_t RPC=1; RPC<=kNRPC; RPC++) {
                    //creazione di un pointer all'elemento della mappa delle tensioni
                    TObjArray *dataArrayVoltage;
                    dataArrayVoltage=(TObjArray*)(mapCurrent->GetValue(Form("MTR_%s_MT%d_RPC%d_HV.vEff",fSides[side].Data(),fPlanes[plane],RPC)));

                    if(!dataArrayVoltage) {
                        printf(" Problems getting dataArrayVoltage\n");
                        break;
                    }

                    //cout<<"N voltage entries= "<<dataArrayVoltage->GetEntries()<<endl;

                    Bool_t isVoltageOk=kTRUE;

                    //loop sulle entry del vettore di misure di tensione
                    for (Int_t arrayIndex=0; arrayIndex<(dataArrayVoltage->GetEntries()); arrayIndex++) {
                        AliDCSValue *value = (AliDCSValue*)dataArrayVoltage->At(arrayIndex);
                        if(value->GetFloat()<8500.){
                            isVoltageOk=kFALSE;
                            //cout<<"\t"<<value->GetFloat()<<"\tBAD"<<endl;
                            break;
                        } else {
                            //cout<<"\t"<<value->GetFloat()<<endl;
                            dataList[side][plane][RPC-1]->Add(new AliRPCValueVoltage((*runIterator).runNumber,value->GetTimeStamp(),value->GetFloat(),isCalib,*beamType,beamEnergy,*LHCState));
                        }
                        //cout<<"\t"<<value->GetFloat()<<endl;
                        dataList[side][plane][RPC-1]->Add(new AliRPCValueVoltage((*runIterator).runNumber,value->GetTimeStamp(),value->GetFloat(),isCalib,*beamType,beamEnergy,*LHCState));
                        delete value;
                    }

                    if (isVoltageOk==kFALSE) break;

                    //creazione di un pointer all'elemento della mappa delle correnti
                    TObjArray *dataArrayCurrents;
                    dataArrayCurrents=(TObjArray*)(mapCurrent->GetValue(Form("MTR_%s_MT%d_RPC%d_HV.actual.iMon",fSides[side].Data(),fPlanes[plane],RPC)));
                    if(!dataArrayCurrents) {
                        printf(" Problems getting dataArrayCurrents\n");
                        break;
                    }
                    //printf("%d-%d-%d\n",plane,side,RPC);
                    //loop sulle entry del vettore di misure di corrente
                    //cout<<"N current entries= "<<dataArrayCurrents->GetEntries()<<endl;
                    for (Int_t arrayIndex=0; arrayIndex<(dataArrayCurrents->GetEntries()); arrayIndex++) {
                        //puntatore all'elemento i-esimo
                        AliDCSValue *value = (AliDCSValue*)dataArrayCurrents->At(arrayIndex);
                        //se il run è di calibrazione corrente e corrente di buio coincidono
                        if (isCalib) {
                            dataList[side][plane][RPC-1]->Add(new AliRPCValueCurrent((*runIterator).runNumber,value->GetTimeStamp(),value->GetFloat(),value->GetFloat(),isCalib,*beamType,beamEnergy,*LHCState ,0));
                            ((AliRPCValueDCS*)dataList[side][plane][RPC-1]->Last())->PrintBeamStatus();
                            //altrimenti imposto la corrente di buio a 0 (la cambio dopo)
                        } else {
                            dataList[side][plane][RPC-1]->Add(new AliRPCValueCurrent((*runIterator).runNumber,value->GetTimeStamp(),value->GetFloat(),0.,isCalib,*beamType,beamEnergy,*LHCState,0));
                            ((AliRPCValueDCS*)dataList[side][plane][RPC-1]->Last())->PrintBeamStatus();
                        }
                        //cout<<"\t"<<value->GetFloat()<<"   "<<value->GetTimeStamp()<<endl;
                        delete value;
                    }
                    //Form("OCDB_Data_MTR_%s_MT%d_RPC%d",(fSides[side]).Data(),fPlanes[plane],RPC)
                }
            }
        }

        //if(arrayScalers)printf("\n\n\n##### Scalers %d ######\n\n\n",arrayScalers->GetEntries());

        ULong64_t elapsedTime=0;
        UInt_t readLB[kNCathodes][kNSides][kNPlanes][kNRPC];
        UInt_t overflowLB[kNCathodes][kNSides][kNPlanes][kNRPC];
        for (Int_t plane=0; plane<kNPlanes; plane++) {
            for (Int_t side=0; side<kNSides; side++) {
                for (Int_t RPC=1; RPC<=kNRPC; RPC++) {
                    for (Int_t cathode=0; cathode<kNCathodes; cathode++) {
                        readLB[cathode][side][plane][RPC-1]=0;
                        overflowLB[cathode][side][plane][RPC-1]=0;
                    }
                }
            }
        }
        //loop sulle entries, sui piani, i catodi (bending e non bending) e le Local Boards (234 per piano)
        //cout<<"entries: "<<arrayScalers->GetEntries()<<endl;
        AliMUONTriggerScalers *scalersData;
        for (Int_t scalerEntry=0; scalerEntry<arrayScalers->GetEntries() && isCalib==kFALSE; scalerEntry++){
            scalersData=(AliMUONTriggerScalers*)arrayScalers->At(scalerEntry);
            //cout<<"deltat: "<<scalersData->GetDeltaT()<<endl;
            Int_t arrayScalersEntries=arrayScalers->GetEntries();
            //if(scalerEntry%100==0)cout<<scalerEntry<<"/"<<arrayScalersEntries<<endl;
            elapsedTime+=scalersData->GetDeltaT();
            for (Int_t plane=0; plane<kNPlanes; plane++) {
                for (Int_t cathode=0; cathode<kNCathodes; cathode++) {
                    //cout<<"{";
                    for (Int_t localBoard=0; localBoard<kNLocalBoards; localBoard++) {
                        Int_t iRPC017=(ddlStore->GetDEfromLocalBoard(localBoard+1, plane+10))-(plane+1+10)*100;
                        Int_t iRPC09=kRPCIndexes[iRPC017];
                        Int_t iSide=kRPCSides[iRPC017];

                        //cout<<"{"<<localBoard<<","<<iRPC09<<","<<iSide<<"},";

                        // se c'è overflow scarto il dato
                        readLB[cathode][iSide][plane][iRPC09-1]++;
                        Bool_t isOverflow=kFALSE;
                        if (scalersData->GetLocScalStripOver(cathode, plane, localBoard)!=0. ){//| scalersData->GetLocScalStrip(cathode, plane, localBoard)==0.) {
                            //printf(" MTR %d cathode %d LB %d RPC %d or %d_%s timestamp %lu data %d OVERFLOW\n",fPlanes[plane],cathode,localBoard,iRPC017,iRPC09,(fSides[iSide]).Data(),SOR+elapsedTime,scalersData->GetLocScalStrip(cathode, plane, localBoard));
                            overflowLB[cathode][iSide][plane][iRPC09-1]++;
                            readLB[cathode][iSide][plane][iRPC09-1]--;
                            isOverflow=kTRUE;
                        }
                        // se la lettura non è quella a fine run immagazzino il dato con timestamp pari a SOR+DeltaT
                        if(scalerEntry!=arrayScalersEntries-1){
                            AliRPCValueScaler *buffer=new AliRPCValueScaler((*runIterator).runNumber, SOR+elapsedTime, scalersData->GetLocScalStrip(cathode, plane, localBoard), isCalib,*beamType,beamEnergy,*LHCState, scalersData->GetDeltaT(), isOverflow);
                            scalersDataList[cathode][iSide][plane][iRPC09-1]->Add(buffer);
                            scalersLocalBoardList[cathode][plane][localBoard]->Add(buffer);
                            //delete buffer;
                        }
                        // altrimenti il timestamp è pari all'EOR
                        else {
                            AliRPCValueScaler *buffer=new AliRPCValueScaler((*runIterator).runNumber, EOR, scalersData->GetLocScalStrip(cathode, plane, localBoard), isCalib,*beamType,beamEnergy,*LHCState, scalersData->GetDeltaT(), isOverflow);
                            scalersDataList[cathode][iSide][plane][iRPC09-1]->Add(buffer);
                            scalersLocalBoardList[cathode][plane][localBoard]->Add(buffer);
                            //delete buffer;
                        }
                        //printf(" MTR %d cathode %d LB %d RPC %d or %d_%s timestamp %lu data %d\n",fPlanes[plane],cathode,localBoard,iRPC017,iRPC09,(fSides[iSide]).Data(),SOR+elapsedTime,scalersData->GetLocScalStrip(cathode, plane, localBoard));
                    }
                    //cout<<"}"<<endl;
                }
            }
            scalersData=0x0;
        }
        delete scalersData;

        printf("scalers reading complete.\n");

        for (Int_t plane=0; plane<kNPlanes; plane++) {
            for (Int_t side=0; side<kNSides; side++) {
                for (Int_t RPC=1; RPC<=kNRPC; RPC++) {
                    for (Int_t cathode=0; cathode<kNCathodes; cathode++) {
                        scalersDataList[cathode][side][plane][RPC-1]->Add(new AliRPCOverflowStatistics((*runIterator).runNumber, EOR, overflowLB[cathode][side][plane][RPC-1], readLB[cathode][side][plane][RPC-1], isCalib,*beamType,beamEnergy,*LHCState ));
                        //cout<<"RUN"<<(*runIterator).runNumber<<" side:"<<side<<" plane "<<plane<<" RPC "<<RPC<<" cathode "<<cathode<<" READ="<<readLB[cathode][side][plane][RPC-1]<<" OVERFLOW="<<overflowLB[cathode][side][plane][RPC-1]<<endl<<endl;
                    }
                }
            }
        }
    }

    printf("\n\n\nData retrieving complete\n\n\n");

    //loop sui piani, i lati (inside e outside) e le RPC (9 per side)
    for (Int_t plane=0; plane<kNPlanes; plane++) {
        for (Int_t side=0; side<kNSides; side++) {
            for (Int_t RPC=1; RPC<=kNRPC; RPC++) {
                //alias all'elemento dell'array 3D di TLists*
                TList *sortedList=dataList[side][plane][RPC-1];
                sortedList->Sort();

                //contenitore per il run number del run dal quale deriva la misura di dark current
                UInt_t calibRunNumber=0;

                //contenitore per il valore di corrente di buio misurata
                Double_t iDarkCalib=0.;

                //flag positivo se la precedente misura di tensione è >=8500.V
                Bool_t voltageOkFlag=kFALSE;

                //printf("MT%d_%s_%d\n",fPlanes[plane],fSides[side].Data(),RPC);

                //loop sulle entries della lisa di dati
                for(Int_t iList=0; iList<sortedList->GetEntries(); iList++){
                    //cout<<iList<<"/"<<sortedList->GetEntries()<<endl;
                    //L'elemento può essere una tensione o una corrente
                    AliRPCValueDCS *valueDCS=(AliRPCValueDCS*)(sortedList->At(iList));
                    //se è una tensione
                    if (valueDCS->IsVoltage()) {
                        //cast a tensione
                        AliRPCValueVoltage* valueVoltage=(AliRPCValueVoltage*)valueDCS;
                        //settaggio del flag
                        voltageOkFlag=(Bool_t)(valueVoltage->GetVSupp()>=8500.);
                        valueVoltage=0x0;
                        //se è una corrente
                    } else if (valueDCS->IsCurrent()) {
                        //cast a corrente
                        AliRPCValueCurrent* valueCurrent=(AliRPCValueCurrent*)valueDCS;
                        //se è un run di calibrazione fatto a tensione di lavoro
                        if (valueCurrent->IsCalib()==kTRUE && voltageOkFlag==kTRUE) {
                            //rimangono alcune letture a 0.0A, così si tolgono ###GIUSTO?###
                            if(valueCurrent->GetIDark()!=0.){
                                iDarkCalib=valueCurrent->GetIDark();
                                calibRunNumber=valueCurrent->GetCalibRunNumber();
                            }
                            //se è un run di fisica setto la dark current prendendola dal precedente run di calibrazione eseguito in condizioni di lavoro
                        } else {
                            valueCurrent->SetIDark(iDarkCalib);
                            valueCurrent->SetCalibRunNumber(calibRunNumber);
                            //cout<<valueCurrent<<"\t"<<calibRunNumber<<endl;
                        }
                        valueCurrent=0x0;
                    }
                    //cout<<valueDCS->IsCalib()<<endl;
                    valueDCS=0x0;
                }

                fOCDBDataContainer->cd();
                sortedList->Sort();
                scalersDataList[0][side][plane][RPC-1]->Sort();
                scalersDataList[1][side][plane][RPC-1]->Sort();
                sortedList->Write(Form("OCDB_Data_MTR_%s_MT%d_RPC%d",(fSides[side]).Data(),fPlanes[plane],RPC),TObject::kSingleKey | TObject::kOverwrite);
                scalersDataList[0][side][plane][RPC-1]->Write(Form("OCDB_Scalers_MTR_%s_%s_MT%d_RPC%d",(fSides[side]).Data(),(fCathodes[0]).Data(),fPlanes[plane],RPC),TObject::kSingleKey | TObject::kOverwrite);
                scalersDataList[1][side][plane][RPC-1]->Write(Form("OCDB_Scalers_MTR_%s_%s_MT%d_RPC%d",(fSides[side]).Data(),(fCathodes[1]).Data(),fPlanes[plane],RPC),TObject::kSingleKey | TObject::kOverwrite);
            }
        }
    }

    for(Int_t cathode=0;cathode<kNCathodes;cathode++){
        for(Int_t plane=0;plane<kNPlanes;plane++){
            for(Int_t local=0;local<kNLocalBoards;local++){
                fOCDBDataContainer->cd();
                scalersLocalBoardList[cathode][plane][local]->Sort();
                scalersLocalBoardList[cathode][plane][local]->Write(Form("OCDB_Scalers_MTR_%s_MT%d_LB%d",(fCathodes[cathode]).Data(),fPlanes[plane],local+1),TObject::kSingleKey | TObject::kOverwrite);
                //printf("OCDB_Scalers_MTR_%s_MT%d_LB%d\n",(fCathodes[cathode]).Data(),fPlanes[plane],local+1);
            }
        }
    }

    printf("\n\n\nDark currents setting complete\n\n\n");
}

void AliRPCAutoIntegrator::FillAliRPCData() {
    fMeanDataContainer=new AliRPCData();

    TList *listBuffer;
    AliRPCRunStatistics *statsBuffer;
    Double_t DarkCurrentCumulus=0.;
    Int_t DarkCurrentCounter=0;
    Double_t TotalCurrentCumulus=0.;
    Int_t TotalCurrentCounter=0;
    Double_t HVCumulus=0.;
    Int_t HVCounter=0;
    Bool_t isDark=kFALSE;
    Bool_t isCalib=kFALSE;

    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                fGlobalDataContainer->GetObject(Form("TLists/OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),listBuffer);

                // if any data list is missing, then the channel
                // (aka {iSide,iPlane,iRPC}) is skipped
                if (!listBuffer) {
                    printf("OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d NOT FOUND\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);
                    continue;
                }


                TIter iterValue(listBuffer);
                UInt_t runNumberBuffer=0;
                //it is intended as runnumber extends from runbegin to runend then
                //at newbegin begins another run
                ULong64_t runBeginBuffer=0;
                ULong64_t newRunBeginBuffer=runBeginBuffer;
                ULong64_t runEndBuffer=0;

                //iter until runnumber changes
                while(iterValue()){
                    UInt_t RunNumber= ((AliRPCValueDCS *) *iterValue)->GetRunNumber();
                    ULong64_t TimeStamp= ((AliRPCValueDCS *) *iterValue)->GetTimeStamp();

                    //check if Timestamp is updated and differs from zero (AMANDA data between two runs)
                    if(RunNumber>runNumberBuffer&&RunNumber!=0.){
                        //if is updated im in a new run and last endbuffer should not be updated
                        newRunBeginBuffer=TimeStamp;

                        //printf("run: %d, start %d, stop %d \n",OCDBRunNumber,runBeginBuffer,runEndBuffer);



                        //calulate the means
                        Double_t meanDarkCurrent=DarkCurrentCumulus/(Double_t)DarkCurrentCounter;
                        Double_t meanTotalCurrent=TotalCurrentCumulus/(Double_t)TotalCurrentCounter;
                        Double_t meanHV=HVCumulus/(Double_t)HVCounter;

                        //save means in AliRPCData and resets all variable
                        statsBuffer=new AliRPCRunStatistics(runNumberBuffer,runBeginBuffer,runEndBuffer,isDark,isCalib,meanDarkCurrent,meanTotalCurrent,meanHV,0,0);
                        fMeanDataContainer->AddRunStatistics(iPlane,iSide,iPlane,statsBuffer);

                        statsBuffer=0x0;
                        DarkCurrentCounter=0;
                        DarkCurrentCumulus=0.;
                        TotalCurrentCounter=0;
                        TotalCurrentCumulus=0.;
                        HVCounter=0;
                        HVCumulus=0.;

                        //here all is done for this run
                        //printf("run: %u,\tmeanIDark: %f,\tmeanITot: %f,\tmeanHV: %f\n",runNumberBuffer,meanDarkCurrent,meanTotalCurrent,meanHV);

                        runBeginBuffer=newRunBeginBuffer;
                        runNumberBuffer=RunNumber;

                    }else{
                        //if is a current value
                        if(((AliRPCValueDCS*)*iterValue)->IsCurrent()){
                            //Calculate mean dark current
                            DarkCurrentCounter++;
                            DarkCurrentCumulus+=((AliRPCValueCurrent*)*iterValue)->GetIDark();

                            //Calculate mean total current
                            TotalCurrentCounter++;
                            TotalCurrentCumulus+=((AliRPCValueCurrent*)*iterValue)->GetITot();
                        }

                        //if is a voltage value
                        if(!((AliRPCValueDCS*)*iterValue)->IsAMANDA()&&((AliRPCValueDCS*)*iterValue)->IsVoltage()){
                            HVCounter++;
                            HVCumulus+=((AliRPCValueVoltage*)*iterValue)->GetVSupp();
                        }

                        isDark=((AliRPCValueDCS*)*iterValue)->IsCalib();
                        isCalib=((AliRPCValueDCS*)*iterValue)->IsCalib();

                        runEndBuffer=TimeStamp;
                    }

                }

                WhichRPC(iRPC, iSide, iPlane);

                listBuffer = 0x0;
            }
        }
    }

    //save AliRPCData on a File
    TFile *AliRPCDataFile=new TFile("AliRPCData.root","RECREATE");
    AliRPCDataFile->cd();
    fMeanDataContainer->Write("AliRPCData",TObject::kSingleKey|TObject::kOverwrite);
    AliRPCDataFile->Close();
}

void AliRPCAutoIntegrator::AMANDASetDataMembers(){
    TList *listBufferAMANDA=0x0;
    TList *listBufferOCDB=0x0;
    TList *DataWithRunNumber[kNSides][kNPlanes][kNRPC];
    //fGlobalDataContainer->mkdir("TLists/AMANDA_DATA_withRunnumber");


    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                fOCDBDataContainer->GetObject(Form("OCDB_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),listBufferOCDB);
                fAMANDADataContainer->GetObject(Form("AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),listBufferAMANDA);

                DataWithRunNumber[iSide][iPlane][iRPC]=new TList();
                DataWithRunNumber[iSide][iPlane][iRPC]->SetName(Form("AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1));

                // if any data list is missing, then the channel
                // (aka {iSide,iPlane,iRPC}) is skipped
                if (!listBufferAMANDA) {
                    printf("AMANDA_Data_MTR_%s_MT%d_RPC%d NOT FOUND\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);
                    continue;
                }
                if (!listBufferOCDB) {
                    printf("OCDB_Data_MTR_%s_MT%d_RPC%d NOT FOUND\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);
                    continue;
                }

                if(!listBufferOCDB->IsSorted()) listBufferOCDB->Sort();
                if(!listBufferAMANDA->IsSorted()) listBufferAMANDA->Sort();

                TIter iterValueOCDB(listBufferOCDB);
                TIter iterValueAMANDA(listBufferAMANDA);
                TIter iterValueAMANDANext(listBufferAMANDA);
                iterValueAMANDANext();

                UInt_t runNumberBuffer=0;
                //it is intended as runnumber extends from runbegin to runend then
                //at newbegin begins another run
                ULong64_t runBeginBuffer=0;
                ULong64_t newRunBeginBuffer=runBeginBuffer;
                ULong64_t runEndBuffer=0;
                //iter on OCDB until runnumber changes
                while(iterValueOCDB()){
                    TBeamType OCDBCurrentRunType=((AliRPCValueDCS *) *iterValueOCDB)->GetfBeamType();
                    float_t OCDBCurrentBeamEnergy=((AliRPCValueDCS *) *iterValueOCDB)->GetfBeamEnergy();
                    TLHCStatus OCDBCurrentLHCStatus=((AliRPCValueDCS *) *iterValueOCDB)->GetfLHCStatus();
                    UInt_t OCDBCurrentRunNumber=((AliRPCValueDCS *) *iterValueOCDB)->GetRunNumber();
                    ULong64_t OCDBTimeStamp= ((AliRPCValueDCS *) *iterValueOCDB)->GetTimeStamp();

                    //check if OCDBTimestamp is updated
                    if(OCDBCurrentRunNumber>runNumberBuffer){
                        //if is updated iterator is in a new run and last endbuffer should not be updated
                        newRunBeginBuffer=OCDBTimeStamp;

                        //printf("run: %d, start %d, stop %d \n",OCDBRunNumber,runBeginBuffer,runEndBuffer);

                        //iter over amanda to update the run from start to stop then look for next run
                        while(iterValueAMANDA()){
                            UInt_t AMANDATimeStamp=((AliRPCValueDCS *) *iterValueAMANDA)->GetTimeStamp();
                            if((AMANDATimeStamp>=runBeginBuffer)&&(AMANDATimeStamp<=runEndBuffer)){
                                ((AliRPCValueDCS *) *iterValueAMANDA)->SetRunNumber(runNumberBuffer);
                                ((AliRPCValueDCS *) *iterValueAMANDA)->SetfBeamType(OCDBCurrentRunType);
                                ((AliRPCValueDCS *) *iterValueAMANDA)->SetfBeamEnergy(OCDBCurrentBeamEnergy);
                                ((AliRPCValueDCS *) *iterValueAMANDA)->SetfLHCStatus(OCDBCurrentLHCStatus);
                                DataWithRunNumber[iSide][iPlane][iRPC]->Add(*iterValueAMANDA);
                            }

                            //when the run is passed exit the cycle
                            if(iterValueAMANDANext()){
                                if(((AliRPCValueDCS *) *iterValueAMANDANext)->GetRunNumber()>runEndBuffer) break;
                            } else break;
                        }

                        runBeginBuffer=newRunBeginBuffer;
                        runNumberBuffer=OCDBCurrentRunNumber;

                    }else{
                        runEndBuffer=OCDBTimeStamp;
                    }

                }

//                //now I add data with runnumber =0 (values get between two different runs)
                TIter iterValueAMANDAzero(listBufferAMANDA);
                while(iterValueAMANDAzero()){
                    if(((AliRPCValueDCS *) *iterValueAMANDA)->GetRunNumber()==0){
                        DataWithRunNumber[iSide][iPlane][iRPC]->Add(*iterValueAMANDA);
                    }
                }
                DataWithRunNumber[iSide][iPlane][iRPC]->Sort();

                //Update File
                fAMANDADataContainer->cd();
                DataWithRunNumber[iSide][iPlane][iRPC]->Write(Form("AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kSingleKey|TObject::kOverwrite);

                WhichRPC(iRPC, iSide, iPlane);

                listBufferAMANDA = 0x0;
                listBufferOCDB = 0x0;

            }
        }
    }

}


void AliRPCAutoIntegrator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TList *list, std::vector<UInt_t> RunNumberList, Int_t whichValue) {
    Int_t counter=0;
    TIter iterValue(list);
    counter=0;
    while (iterValue()) {
        if(IsRunInList(RunNumberList,((AliRPCValueDCS *) *iterValue)->GetRunNumber())) {
            if ((((AliRPCValueDCS *) *iterValue)->*funky)() &&
                ((AliRPCValueDCS *) *iterValue)->GetTimeStamp() > 8000) {
                Graph->SetPoint(counter++, ((AliRPCValueDCS *) *iterValue)->GetTimeStamp(),(((AliRPCValueDCS *) *iterValue)->GetValue(whichValue)));
            }
        }
    }
    return;
}

void AliRPCAutoIntegrator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TList *list, UInt_t RunNumber, Int_t whichValue){
    std::vector<UInt_t> RunDummyList;
    RunDummyList.push_back(RunNumber);
    PlotSomethingVersusTime(Graph, funky, list, RunDummyList, whichValue);
    return;
}

void AliRPCAutoIntegrator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TList *list,std::vector<OCDBRun> RunNumberList, Int_t whichValue){
    std::vector<UInt_t> RunDummyList;
    for(OCDBRun iter :RunNumberList){
        UInt_t temp=((UInt_t)iter.runNumber);
        RunDummyList.push_back(temp);
    }
    PlotSomethingVersusTime(Graph, funky, list, RunDummyList, whichValue);
    return;
}

void AliRPCAutoIntegrator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const,TList *list, Int_t whichValue){
    Int_t counter=0;
    TIter iterValue(list);
    counter=0;
    while (iterValue()) {
        if ((((AliRPCValueDCS *) *iterValue)->*funky)() &&
            ((AliRPCValueDCS *) *iterValue)->GetTimeStamp() > 8000) {
            Graph->SetPoint(counter++, ((AliRPCValueDCS *) *iterValue)->GetTimeStamp(),(((AliRPCValueDCS *) *iterValue)->GetValue(whichValue)));
        }
    }
    return;
}

void AliRPCAutoIntegrator::CreateDistributionSomething(TH1 *Graph, Bool_t (AliRPCValueDCS::*funky)() const, UInt_t RunNumber, Int_t whichValue, Bool_t normalizedToArea){
    TList *listBuffer;

    for (Int_t iSide = 0; iSide < kNSides; iSide++) {
        for (Int_t iPlane = 0; iPlane < kNPlanes; iPlane++) {
            for (Int_t iRPC = 0; iRPC < kNRPC; iRPC++) {

                //get Object from merged data
                fGlobalDataContainer->GetObject(
                        Form("TLists/OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d", (fSides[iSide]).Data(),
                             fPlanes[iPlane], iRPC + 1),
                        listBuffer);

                if (!listBuffer) {
                    printf("TLists/OCDB_AMANDA_Data_MTR_%s_MT%d_RPC%d NOT FOUND\n", (fSides[iSide]).Data(),
                           fPlanes[iPlane],
                           iRPC + 1);
                    continue;
                }


                TIter iterValue(listBuffer);
                while (iterValue()) {
                    if(((AliRPCValueDCS *) *iterValue)->GetRunNumber()==RunNumber){
                        if (((AliRPCValueDCS *) *iterValue)->GetTimeStamp() > 8000 &&
                            (((AliRPCValueDCS *) *iterValue)->*funky)()) {
                                Graph->Fill(
                                        (((AliRPCValueCurrent *) *iterValue)->GetValue(whichValue))/(normalizedToArea?fRPCAreas[iRPC][iPlane]:1.));
                        }
                    }
                    //when run is finished exit the cycle
                    if (((AliRPCValueDCS *) *iterValue)->GetRunNumber() > RunNumber) break;
                }

                listBuffer = 0x0;
                WhichRPC(iRPC, iSide, iPlane);
            }
        }
    }
}

void AliRPCAutoIntegrator::CreateDarkCurrentDistribution(TH1 *Graph, UInt_t RunNumber) {
    AliRPCAutoIntegrator::CreateDistributionSomething(Graph, &AliRPCValueDCS::IsCurrent, RunNumber, AliRPCValueCurrent::kIDark, kTRUE);
    return;
}

void AliRPCAutoIntegrator::VoltagePlotter(TGraph *Graph, TList* list, UInt_t RunNumber){
    AliRPCAutoIntegrator::PlotSomethingVersusTime(Graph, &AliRPCValueDCS::IsVoltage, list, RunNumber, 0);
    return;
}

void AliRPCAutoIntegrator::VoltagePlotter(TGraph *Graph, TList* list, std::vector<UInt_t> RunNumberList){
    AliRPCAutoIntegrator::PlotSomethingVersusTime(Graph,&AliRPCValueDCS::IsVoltage,list,RunNumberList,0);
    return;
}

void AliRPCAutoIntegrator::VoltagePlotter(TGraph *Graph, TList* list){
    AliRPCAutoIntegrator::PlotSomethingVersusTime(Graph,&AliRPCValueDCS::IsVoltage,list);
    return;
}

/*
 * print which RPC corresponds to iSide, iPlane, iRPC
 */
void AliRPCAutoIntegrator::WhichRPC(Int_t iRPC, Int_t iSide, Int_t iPlane){
  Int_t NTot=kNRPC*kNPlanes*kNSides;
  printf("RPC:%3d out of %3d\n",kNRPC*kNPlanes*iSide+kNRPC*iPlane+iRPC+1,NTot);
  return;
}

Bool_t AliRPCAutoIntegrator::IsRunInList(std::vector<UInt_t> RunList, UInt_t RunNumber) {
    return std::find(RunList.begin(), RunList.end(), RunNumber) != RunList.end();
}


