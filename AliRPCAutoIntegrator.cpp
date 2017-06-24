/**************************************************************************
 * Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------

#include "AliRPCAutoIntegrator.h"

using namespace std;

ClassImp(AliRPCAutoIntegrator);

////////////////////////////////////////////////////////////////////////////////
// Filling of fAMANDAData members (const) needed for conversione notation.
// This method allows a direct filling of arrays even if defined as const void*
////////////////////////////////////////////////////////////////////////////////

static const Int_t RPCIndexes[] = {5,6,7,8,9,9,8,7,6,5,4,3,2,1,1,2,3,4};
const Int_t *AliRPCAutoIntegrator::kRPCIndexes = RPCIndexes;

static const Int_t RPCSides[] = {0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0};
const Int_t *AliRPCAutoIntegrator::kRPCSides = RPCSides;

/*{index from 1 to 234, RPC [1-9], side [0,1]}*/
static const Int_t LBToRPC[][3]={{1,1,0},{2,2,0},{3,2,0},{4,3,0},{5,3,0},{6,4,0},{7,4,0},{8,4,0},{9,6,0},{10,6,0},{11,6,0},{12,7,0},{13,7,0},{14,8,0},{15,8,0},{16,9,0},{17,1,0},{18,2,0},{19,2,0},{20,3,0},{21,3,0},{22,4,0},{23,4,0},{24,4,0},{25,4,0},{26,5,0},{27,5,0},{28,5,0},{29,5,0},{30,6,0},{31,6,0},{32,6,0},{33,6,0},{34,7,0},{35,7,0},{36,8,0},{37,8,0},{38,9,0},{39,1,0},{40,2,0},{41,2,0},{42,3,0},{43,3,0},{44,4,0},{45,4,0},{46,4,0},{47,4,0},{48,5,0},{49,5,0},{50,5,0},{51,5,0},{52,6,0},{53,6,0},{54,6,0},{55,6,0},{56,7,0},{57,7,0},{58,8,0},{59,8,0},{60,9,0},{61,1,0},{62,2,0},{63,2,0},{64,3,0},{65,3,0},{66,4,0},{67,4,0},{68,5,0},{69,5,0},{70,6,0},{71,6,0},{72,7,0},{73,7,0},{74,8,0},{75,8,0},{76,9,0},{77,1,0},{78,2,0},{79,2,0},{80,3,0},{81,3,0},{82,4,0},{83,4,0},{84,5,0},{85,5,0},{86,6,0},{87,6,0},{88,7,0},{89,7,0},{90,8,0},{91,8,0},{92,9,0},{93,1,0},{94,2,0},{95,2,0},{96,3,0},{97,3,0},{98,4,0},{99,4,0},{100,5,0},{101,5,0},{102,6,0},{103,6,0},{104,7,0},{105,7,0},{106,8,0},{107,8,0},{108,9,0},{109,1,0},{110,2,0},{111,3,0},{112,4,0},{113,5,0},{114,6,0},{115,7,0},{116,8,0},{117,9,0},{118,1,1},{119,2,1},{120,2,1},{121,3,1},{122,3,1},{123,4,1},{124,4,1},{125,4,1},{126,6,1},{127,6,1},{128,6,1},{129,7,1},{130,7,1},{131,8,1},{132,8,1},{133,9,1},{134,1,1},{135,2,1},{136,2,1},{137,3,1},{138,3,1},{139,4,1},{140,4,1},{141,4,1},{142,4,1},{143,5,1},{144,5,1},{145,5,1},{146,5,1},{147,6,1},{148,6,1},{149,6,1},{150,6,1},{151,7,1},{152,7,1},{153,8,1},{154,8,1},{155,9,1},{156,1,1},{157,2,1},{158,2,1},{159,3,1},{160,3,1},{161,4,1},{162,4,1},{163,4,1},{164,4,1},{165,5,1},{166,5,1},{167,5,1},{168,5,1},{169,6,1},{170,6,1},{171,6,1},{172,6,1},{173,7,1},{174,7,1},{175,8,1},{176,8,1},{177,9,1},{178,1,1},{179,2,1},{180,2,1},{181,3,1},{182,3,1},{183,4,1},{184,4,1},{185,5,1},{186,5,1},{187,6,1},{188,6,1},{189,7,1},{190,7,1},{191,8,1},{192,8,1},{193,9,1},{194,1,1},{195,2,1},{196,2,1},{197,3,1},{198,3,1},{199,4,1},{200,4,1},{201,5,1},{202,5,1},{203,6,1},{204,6,1},{205,7,1},{206,7,1},{207,8,1},{208,8,1},{209,9,1},{210,1,1},{211,2,1},{212,2,1},{213,3,1},{214,3,1},{215,4,1},{216,4,1},{217,5,1},{218,5,1},{219,6,1},{220,6,1},{221,7,1},{222,7,1},{223,8,1},{224,8,1},{225,9,1},{226,1,1},{227,2,1},{228,3,1},{229,4,1},{230,5,1},{231,6,1},{232,7,1},{233,8,1},{234,9,1}};
const Int_t *AliRPCAutoIntegrator::kLBToRPC = LBToRPC[0];


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

//call this in constructors
void AliRPCAutoIntegrator::InitDataMembers(){
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

    for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
        fTinyArea[iPlane]=(fRPCAreas[9-1][iPlane]/7.*6.-fRPCAreas[5-1][iPlane])/4.;
        fLittleArea[iPlane]=fRPCAreas[9-1][iPlane]/28.;
        fNormalArea[iPlane]=fRPCAreas[9-1][iPlane]/14.;
        fBigArea[iPlane]=fRPCAreas[9-1][iPlane]/7.;
    }

    for(Int_t iLB=0;iLB<kNLocalBoards;iLB++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            if((iLB>=25 && iLB<=28) || (iLB>=142 && iLB<=145)){
                fLBAreas[iLB][iPlane]=fTinyArea[iPlane];
            } else if((iLB>=43 && iLB<=54) || (iLB>=21 && iLB<=24) || (iLB>=29 && iLB<=32) || (iLB>=5 && iLB<=10) || (iLB>=122 && iLB<=127) || (iLB>=138 && iLB<=141) || (iLB>=146 && iLB<=149) || (iLB>=160 && iLB<=171)){
                fLBAreas[iLB][iPlane]=fLittleArea[iPlane];
            } else if((iLB>=108 && iLB<=116) || (iLB>=225 && iLB<=233) || iLB==0 || iLB==16 || iLB==38 || iLB==60 || iLB==76 || iLB==92 || iLB==117 || iLB==133 || iLB==155 || iLB==177 || iLB==193 || iLB==209 || iLB==224 || iLB==208 || iLB==192 || iLB==176 || iLB==154 || iLB==132 || iLB==15 || iLB==37 || iLB==59 || iLB==75 || iLB==91 || iLB==107){
                fLBAreas[iLB][iPlane]=fBigArea[iPlane];
            } else {
                fLBAreas[iLB][iPlane]=fNormalArea[iPlane];
            }

            //cout<<iLB+1<<" "<<LBAreas[iLB][iPlane]<<endl;
        }
    }
}

// Default constructor
AliRPCAutoIntegrator::AliRPCAutoIntegrator(){
    InitDataMembers();
};

// Class constructor
AliRPCAutoIntegrator::AliRPCAutoIntegrator(TString RunListFileName, TString AMANDAInputFileName, TString OutputFileName, Bool_t updateOCDB, Bool_t updateAMANDA):
fRunListFileName(RunListFileName),
fAMANDAInputFileName(AMANDAInputFileName),
fOutputFileName(OutputFileName),
fUpdateOCDB(updateOCDB),
fUpdateAMANDA(updateAMANDA){
    
    
    // The update fAMANDAData variable allows the user to decide wether the fAMANDAData has to
    // be reloaded in the *DataContainer.root files or not (fAMANDAData already there)
    if(fUpdateOCDB){
        fOCDBDataContainer= new TFile("OCDBDataContainer.root","UPDATE");
        //fAMANDADataContainer= new TFile("AMANDADataContainer.root","RECREATE");
    } else {
        if(checkFileExistance("OCDBDataContainer.root")){
            fOCDBDataContainer= new TFile("OCDBDataContainer.root","READ");
        }else{
            //user ask to read a file that doesn't exists
            fOCDBDataContainer= new TFile("OCDBDataContainer.root","CREATE");
        }
    }

    if(fUpdateAMANDA){
        fAMANDADataContainer= new TFile("AMANDADataContainer.root","UPDATE");
    } else {
        if(checkFileExistance("AMANDADataContainer.root")){
            fAMANDADataContainer= new TFile("AMANDADataContainer.root","READ");
        }else{
            //user ask to read a file that doesn't exists
            fAMANDADataContainer= new TFile("AMANDADataContainer.root","CREATE");
        }
    }

    fExistsRPCDataObject = checkFileExistance(OutputFileName);

    //fGlobalDataContainer= new TFile(Form("%s",OutputFileName.Data()),"RECREATE");
    if(!fExistsRPCDataObject){
        cout<<"File doesn't exist"<<endl;
        fGlobalDataContainer= new TFile(Form("%s",OutputFileName.Data()),"RECREATE");
        fGlobalDataContainer->cd();
//        fGlobalDataContainer->mkdir("TObjArrays");
        fGlobalDataContainer->mkdir("iNet_Graphs");
        fGlobalDataContainer->mkdir("integrated_charge_Graphs");
    } else {
        cout<<"File exists"<<endl;
        fGlobalDataContainer = new TFile(Form("%s",OutputFileName.Data()),"UPDATE");;
    }

    // Purge all files
    fOCDBDataContainer->Purge();
    fAMANDADataContainer->Purge();
    fGlobalDataContainer->Purge();

    TString ObjectName;
    for (Int_t plane=0; plane<kNPlanes; plane++) {

        fOCDBDataContainer->cd();

        for (Int_t side = 0; side < kNSides; side++) {
            for (Int_t RPC = 0; RPC < kNRPC; RPC++) {
                fOCDBDataTreeBufferW[side][plane][RPC] = new AliRPCValueDCS();
//                fOCDBDataTreeBufferR[side][plane][RPC] = fOCDBDataTreeBufferW[side][plane][RPC];
                ObjectName = Form("OCDB_Data_MTR_%s_MT%d_RPC%d", (fSides[side]).Data(), fPlanes[plane], RPC + 1);
                if (!(fOCDBDataContainer->GetListOfKeys()->Contains(ObjectName))) {
                    fOCDBDataContainer->cd();
                    fOCDBDataTree[side][plane][RPC] = new TSmartTree(ObjectName, ObjectName);
                    fOCDBDataTreeBranch[side][plane][RPC] = fOCDBDataTree[side][plane][RPC]->Branch(ObjectName, &fOCDBDataTreeBufferW[side][plane][RPC],32000,0);
                    fOCDBDataTree[side][plane][RPC]->Write(ObjectName);
                    cout << "Created ";
                } else {
                    fOCDBDataContainer->GetObject(ObjectName,fOCDBDataTree[side][plane][RPC]);
                    fOCDBDataTreeBranch[side][plane][RPC] = fOCDBDataTree[side][plane][RPC]->GetBranch(ObjectName);
                    fOCDBDataTree[side][plane][RPC]->SetBranchAddress(ObjectName,&fOCDBDataTreeBufferW[side][plane][RPC]);
                    cout << "Loading ";
                }
                cout << ObjectName << " #Entries: "<< fOCDBDataTree[side][plane][RPC]->GetEntries() << "\r";
            }
        }

        cout<<endl;

        for (Int_t side = 0; side < kNSides; side++) {
            for (Int_t RPC = 0; RPC < kNRPC; RPC++) {
                fOCDBRPCScalersTreeBufferW[0][side][plane][RPC] = new AliRPCValueScaler();
//                fOCDBRPCScalersTreeBufferR[0][side][plane][RPC] = fOCDBRPCScalersTreeBufferW[0][side][plane][RPC];
                ObjectName = Form("OCDB_Scalers_MTR_%s_%s_MT%d_RPC%d",(fSides[side]).Data(),(fCathodes[0]).Data(),fPlanes[plane],RPC+1);
                if ( !(fOCDBDataContainer->GetListOfKeys()->Contains(ObjectName)) ) {
                    fOCDBDataContainer->cd();
                    fOCDBRPCScalersTree[0][side][plane][RPC] = new TSmartTree(ObjectName, ObjectName);
                    fOCDBRPCScalersTreeBranch[0][side][plane][RPC] = fOCDBRPCScalersTree[0][side][plane][RPC]->Branch(ObjectName,&fOCDBRPCScalersTreeBufferW[0][side][plane][RPC],32000,0);
                    fOCDBRPCScalersTree[0][side][plane][RPC]->Write(ObjectName);
                    cout<<"Created ";
                } else {
                    fOCDBDataContainer->GetObject(ObjectName,fOCDBRPCScalersTree[0][side][plane][RPC]);
                    fOCDBRPCScalersTreeBranch[0][side][plane][RPC] = fOCDBRPCScalersTree[0][side][plane][RPC]->GetBranch(ObjectName);
                    fOCDBRPCScalersTree[0][side][plane][RPC]->SetBranchAddress(ObjectName,&fOCDBRPCScalersTreeBufferW[0][side][plane][RPC]);
                    cout<<"Loading ";
                }
                cout<<ObjectName << " #Entries: "<< fOCDBRPCScalersTree[0][side][plane][RPC]->GetEntries() << "\r";
            }
        }

        cout<<endl;

        for (Int_t side = 0; side < kNSides; side++) {
            for (Int_t RPC = 0; RPC < kNRPC; RPC++) {
                fOCDBRPCScalersTreeBufferW[1][side][plane][RPC] = new AliRPCValueScaler();
//                fOCDBRPCScalersTreeBufferR[1][side][plane][RPC] = fOCDBRPCScalersTreeBufferW[1][side][plane][RPC];
                ObjectName = Form("OCDB_Scalers_MTR_%s_%s_MT%d_RPC%d",(fSides[side]).Data(),(fCathodes[1]).Data(),fPlanes[plane],RPC+1);
                if ( !(fOCDBDataContainer->GetListOfKeys()->Contains(ObjectName)) ) {
                    fOCDBDataContainer->cd();
                    fOCDBRPCScalersTree[1][side][plane][RPC] = new TSmartTree(ObjectName, ObjectName);
                    fOCDBRPCScalersTreeBranch[1][side][plane][RPC] = fOCDBRPCScalersTree[1][side][plane][RPC]->Branch(ObjectName,&fOCDBRPCScalersTreeBufferW[1][side][plane][RPC],32000,0);
                    fOCDBRPCScalersTree[1][side][plane][RPC]->Write(ObjectName);
                    cout<<"Created ";
                } else {
                    fOCDBDataContainer->GetObject(ObjectName,fOCDBRPCScalersTree[1][side][plane][RPC]);
                    fOCDBRPCScalersTreeBranch[1][side][plane][RPC] = fOCDBRPCScalersTree[1][side][plane][RPC]->GetBranch(ObjectName);
                    fOCDBRPCScalersTree[1][side][plane][RPC]->SetBranchAddress(ObjectName,&fOCDBRPCScalersTreeBufferW[1][side][plane][RPC]);
                    cout<<"Loading ";
                }
                cout<<ObjectName << " #Entries: "<< fOCDBRPCScalersTree[1][side][plane][RPC]->GetEntries() << "\r";
            }
        }

        cout<<endl;
        
        fGlobalDataContainer->cd();
        
        for (Int_t side = 0; side < kNSides; side++) {
            for (Int_t RPC = 0; RPC < kNRPC; RPC++) {
                fGlobalDataTreeBufferW[side][plane][RPC] = new AliRPCValueDCS();
//                fGlobalDataTreeBufferR[side][plane][RPC] = fGlobalDataTreeBufferW[side][plane][RPC];
                ObjectName = Form("Global_Data_MTR_%s_MT%d_RPC%d", (fSides[side]).Data(), fPlanes[plane], RPC + 1);
                if (!(fGlobalDataContainer->GetListOfKeys()->Contains(ObjectName))) {
                    fGlobalDataContainer->cd();
                    fGlobalDataTree[side][plane][RPC] = new TSmartTree(ObjectName, ObjectName);
                    fGlobalDataTreeBranch[side][plane][RPC] = fGlobalDataTree[side][plane][RPC]->Branch(ObjectName, &fGlobalDataTreeBufferW[side][plane][RPC],32000,0);
                    fGlobalDataTree[side][plane][RPC]->Write(ObjectName);
                    cout << "Created ";
                } else {
                    fGlobalDataContainer->GetObject(ObjectName,fGlobalDataTree[side][plane][RPC]);
                    fGlobalDataTreeBranch[side][plane][RPC] = fGlobalDataTree[side][plane][RPC]->GetBranch(ObjectName);
                    fGlobalDataTree[side][plane][RPC]->SetBranchAddress(ObjectName,&fGlobalDataTreeBufferW[side][plane][RPC]);
                    cout << "Loading ";
                }
                cout << ObjectName << " #Entries: "<< fGlobalDataTree[side][plane][RPC]->GetEntries() << "\r";
            }
        }
        
        cout<<endl;

        fAMANDADataContainer->cd();

        for (Int_t side = 0; side < kNSides; side++) {
            for (Int_t RPC = 0; RPC < kNRPC; RPC++) {
                fAMANDADataTreeBufferW[side][plane][RPC] = new AliRPCValueDCS();
//                fAMANDADataTreeBufferR[side][plane][RPC] = fAMANDADataTreeBufferW[side][plane][RPC];
                ObjectName = Form("AMANDA_Data_MTR_%s_MT%d_RPC%d",(fSides[side]).Data(),fPlanes[plane],RPC+1);
                if ( !(fAMANDADataContainer->GetListOfKeys()->Contains(ObjectName)) ) {
                    fAMANDADataContainer->cd();
                    fAMANDADataTree[side][plane][RPC] = new TSmartTree(ObjectName, ObjectName);
                    fAMANDADataTreeBranch[side][plane][RPC] = fAMANDADataTree[side][plane][RPC]->Branch(ObjectName,&fAMANDADataTreeBufferW[side][plane][RPC],32000,0);
                    fAMANDADataTree[side][plane][RPC]->Write(ObjectName);
                    cout<<"Created ";
                } else {
                    fAMANDADataContainer->GetObject(ObjectName,fAMANDADataTree[side][plane][RPC]);
                    fAMANDADataTreeBranch[side][plane][RPC] = fAMANDADataTree[side][plane][RPC]->GetBranch(ObjectName);
                    fAMANDADataTree[side][plane][RPC]->SetBranchAddress(ObjectName,&fAMANDADataTreeBufferW[side][plane][RPC]);
                    cout<<"Loading ";
                }
                cout<<ObjectName << " #Entries: "<< fAMANDADataTree[side][plane][RPC]->GetEntries() << "\r";
            }
        }

        cout<<endl;

        fOCDBDataContainer->cd();

        cout<<"Beginning LBs"<<endl;
        for(Int_t local=0;local<kNLocalBoards;local++) {
            fOCDBLBScalersTreeBufferW[0][plane][local] = new AliRPCValueScaler();
//            fOCDBLBScalersTreeBufferR[0][plane][local] = fOCDBLBScalersTreeBufferW[0][plane][local];
            ObjectName = Form("OCDB_Scalers_MTR_%s_MT%d_LB%d", (fCathodes[0]).Data(), fPlanes[plane], local + 1);
            if (!(fOCDBDataContainer->GetListOfKeys()->Contains(ObjectName))) {
                fOCDBDataContainer->cd();
                fOCDBLBScalersTree[0][plane][local] = new TSmartTree(ObjectName, ObjectName);
                fOCDBLBScalersTreeBranch[0][plane][local] = fOCDBLBScalersTree[0][plane][local]->Branch(ObjectName, &fOCDBLBScalersTreeBufferW[0][plane][local],32000,0);
                fOCDBLBScalersTree[0][plane][local]->Write(ObjectName);
                cout << "Created ";
            } else {
                fOCDBDataContainer->GetObject(ObjectName,fOCDBLBScalersTree[0][plane][local]);
                fOCDBLBScalersTreeBranch[0][plane][local] = fOCDBLBScalersTree[0][plane][local]->GetBranch(ObjectName);
                fOCDBLBScalersTree[0][plane][local]->SetBranchAddress(ObjectName,&fOCDBLBScalersTreeBufferW[0][plane][local]);
                cout << "Loading ";
            }
            cout << ObjectName << " #Entries: "<< fOCDBLBScalersTree[0][plane][local]->GetEntries() << "\r";
        }

        cout<<endl;

        for(Int_t local=0;local<kNLocalBoards;local++){
            fOCDBLBScalersTreeBufferW[1][plane][local] = new AliRPCValueScaler();
//            fOCDBLBScalersTreeBufferR[1][plane][local] = fOCDBLBScalersTreeBufferW[1][plane][local];
            ObjectName = Form("OCDB_Scalers_MTR_%s_MT%d_LB%d",(fCathodes[1]).Data(),fPlanes[plane],local+1);
            if ( !(fOCDBDataContainer->GetListOfKeys()->Contains(ObjectName)) ) {
                fOCDBDataContainer->cd();
                fOCDBLBScalersTree[1][plane][local] = new TSmartTree(ObjectName, ObjectName);
                fOCDBLBScalersTreeBranch[1][plane][local] = fOCDBLBScalersTree[1][plane][local]->Branch(ObjectName,&fOCDBLBScalersTreeBufferW[1][plane][local],32000,0);
                fOCDBLBScalersTree[1][plane][local]->Write(ObjectName);
                cout<<"Created ";
            } else {
                fOCDBDataContainer->GetObject(ObjectName,fOCDBLBScalersTree[1][plane][local]);
                fOCDBLBScalersTreeBranch[1][plane][local] = fOCDBLBScalersTree[1][plane][local]->GetBranch(ObjectName);
                fOCDBLBScalersTree[1][plane][local]->SetBranchAddress(ObjectName,&fOCDBLBScalersTreeBufferW[1][plane][local]);
                cout<<"Loading ";
            }
            cout<<ObjectName << " #Entries: "<< fOCDBLBScalersTree[1][plane][local]->GetEntries() << "\r";
        }
        cout<<endl<<endl;
    }

    TObjArray *check=0x0;
    fOCDBDataContainer->cd();
    if(fOCDBDataContainer) fOCDBDataContainer->GetObject("DownloadedRuns",check);
    if(check){
        fOCDBDataContainer->cd();
        fOCDBRunListDownloaded=check;
        cout<<"Loaded downloaded run list with "<<fOCDBRunListDownloaded->GetEntries()<<" entries"<<endl<<flush;

    }else{
        fOCDBDataContainer->cd();
        fOCDBRunListDownloaded=new TObjArray();
        fOCDBRunListDownloaded->Write("DownloadedRuns",kSingleKey);
        cout<<"Creating new downloaded run list"<<endl<<flush;
    }

    fPlotContainer=new TFile("AutoIntegratorPlotContainer.root","RECREATE");

    //check if AliRPCData already exists
    AliRPCData *AliRPCDataBuffer;
    fGlobalDataContainer->cd();
    fGlobalDataContainer->GetObject("AliRPCDataObj", AliRPCDataBuffer);

    if (!AliRPCDataBuffer) {
        AliRPCDataBuffer = new AliRPCData();
        fAliRPCDataObject=AliRPCDataBuffer;
        cout << "Creating new AliRPCData" << endl << flush;
    } else {
        fAliRPCDataObject = AliRPCDataBuffer;
        cout << "Reading old AliRPCData with " << fAliRPCDataObject->GetTotalEntries() << " entries"<< endl << flush;
    }

    InitDataMembers();

    
    // Calling this method to preload the runs of which the OCDB has to be
    // downloaded
    OCDBRunListReader();
}

//  Destructor will dellaocate any fAMANDAData member allocated in the heap
AliRPCAutoIntegrator::~AliRPCAutoIntegrator(){

//    for (Int_t plane=0; plane<kNPlanes; plane++) {
//        for (Int_t side = 0; side < kNSides; side++) {
//            for (Int_t RPC = 1; RPC <= kNRPC; RPC++) {
//                delete fOCDBDataTreeBufferW[side][plane][RPC - 1];
//                delete fOCDBRPCScalersTreeBufferW[0][side][plane][RPC - 1];
//                delete fOCDBRPCScalersTreeBufferW[1][side][plane][RPC - 1];
//                delete fAMANDADataTreeBufferW[side][plane][RPC-1];
//            }
//        }
//
//        for (Int_t local = 0; local < kNLocalBoards; local++) {
//            delete fOCDBLBScalersTreeBufferW[0][plane][local + 1];
//            delete fOCDBLBScalersTreeBufferW[1][plane][local + 1];
//        }
//    }

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

    // If user wants to update the fAMANDAData then the macro has to connect to OCDB.
    // Note that if fAMANDAData are already in the file there is nothing else to do.
    if (fUpdateOCDB){
        cout<<"Starting OCDB fAMANDAData retrieving...\n";
        OCDBDataToCParser(true,10);
        cout<<"DONE\n"<<endl;

        cout<<"Setting OCDB dark currents...\n";
        OCDBDarkCurrentSetter();
        cout<<"DONE\n"<<endl;
    }

    if (fUpdateAMANDA){
        cout<<"Starting AMANDA fAMANDAData parsing...\n";
        AMANDATextToCParser();

        cout<<"Setting Amanda Parameters...\n";
        AMANDASetDataMembers();
        cout<<"DONE\n"<<endl;
    }


    cout<<"Starting fAMANDAData aggregation...\n";
    Aggregator();
    cout<<"DONE\n"<<endl;

    cout<<"Starting fAMANDAData elaboration...\n";
    Subtractor();
    cout<<"DONE\n"<<endl;

    cout<<"Starting filling AliRPCdata...\n";
    FillAliRPCData();
    cout<<"DONE\n"<<endl;

    cout<<"Starting integrated charge calculation...\n";
    Integrator();
    cout<<"DONE\n"<<endl;

    fOCDBDataContainer->Close();
    fAMANDADataContainer->Close();
    fGlobalDataContainer->Close();
}

// Method to parse a text file containing the run list for OCDB downloader
void AliRPCAutoIntegrator::OCDBRunListReader(){
    ifstream fin;
    TObject *runBuffer;
    fin.open(fRunListFileName.Data());
    if(!fin.is_open()) {
     cout<<"File not found"<<endl<<flush;
        return;
    }
    while(kTRUE){
        runBuffer=new AliOCDBRun();
        ((AliOCDBRun*)runBuffer)->fYear = 0000;
        
        fin >> ((AliOCDBRun*)runBuffer)->fRunNumber;
        if(fin.eof()) break;
        //check if run is already downloaded
        if(!fOCDBRunListDownloaded->FindObject(runBuffer)){
            fOCDBRunListToAdd.push_back(*((AliOCDBRun*)runBuffer));
        }
        runBuffer=0x0;
        //cout<<runBuffer.fRunNumber<<endl<<flush;
    }
    fin.close();
}

// Method to merge the two files: AMANDA fAMANDAData and OCDB fAMANDAData.
// This procedure is needed to have chronologically ordered fAMANDAData with continuous
// current measurements (AMANDA) and "dark current" measurements from without
// beam runs (OCDB). This allows one to use subtract the dark current from
// AMANDA fAMANDAData in the most detailed way.
void AliRPCAutoIntegrator::Aggregator(){

    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){

                fGlobalDataContainer->cd();

                fGlobalDataTree[iSide][iPlane][iRPC]->SetBranchAddress(fGlobalDataTree[iSide][iPlane][iRPC]->GetName(),&fAMANDADataTreeBufferW[iSide][iPlane][iRPC]);

                for (Long64_t iAMANDA=0; iAMANDA < fAMANDADataTree[iSide][iPlane][iRPC]->GetEntries(); fAMANDADataTree[iSide][iPlane][iRPC]->GetEntry(iAMANDA++)) {
                    fGlobalDataTree[iSide][iPlane][iRPC]->Fill();
                }

                fGlobalDataTree[iSide][iPlane][iRPC]->SetBranchAddress(fGlobalDataTree[iSide][iPlane][iRPC]->GetName(),&fOCDBDataTreeBufferW[iSide][iPlane][iRPC]);

                for (Long64_t iOCDB=0; iOCDB < fOCDBDataTree[iSide][iPlane][iRPC]->GetEntries(); fOCDBDataTree[iSide][iPlane][iRPC]->GetEntry(iOCDB++)) {
                    fGlobalDataTree[iSide][iPlane][iRPC]->Fill();
                }

                fGlobalDataTree[iSide][iPlane][iRPC]->Sort("fTimeStamp");

                fGlobalDataContainer->cd();
                fGlobalDataTree[iSide][iPlane][iRPC]->Write("",TObject::kOverwrite);
            }
        }
    }
    fGlobalDataContainer->Flush();
}

void AliRPCAutoIntegrator::GeneratePlots() {
    TGraph *PlotsITot[kNSides][kNPlanes][kNRPC];
    TGraph *PlotsIDark[kNSides][kNPlanes][kNRPC];
    TGraph *PlotsVoltage[kNSides][kNPlanes][kNRPC];
    TString ObjectName;
    
    TObject *IsDirThere;
    fGlobalDataContainer->GetObject("iTot_Graphs",IsDirThere);
    if(!IsDirThere) {
        fGlobalDataContainer->mkdir("iTot_Graphs");
        fGlobalDataContainer->mkdir("iDark_Graphs");
        fGlobalDataContainer->mkdir("Voltage_Graphs");
        cout<<"directories created"<<endl;
    }


    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                ObjectName=Form("Global_Data_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);

                PlotsITot[iSide][iPlane][iRPC] = new TGraph();
                PlotsITot[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsITot[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsITot[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsITot[iSide][iPlane][iRPC]->SetMarkerSize(0.15);

                PlotSomethingVersusTime(PlotsITot[iSide][iPlane][iRPC],&AliRPCValueDCS::IsOkForITot, ObjectName, AliRPCValueCurrent::kITot);

                fGlobalDataContainer->cd("iTot_Graphs");
                PlotsITot[iSide][iPlane][iRPC]->Write(Form("iTot_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kSingleKey|TObject::kOverwrite);

                PlotsIDark[iSide][iPlane][iRPC]=new TGraph();
                PlotsIDark[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsIDark[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsIDark[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsIDark[iSide][iPlane][iRPC]->SetMarkerSize(0.15);

                PlotSomethingVersusTime(PlotsIDark[iSide][iPlane][iRPC],&AliRPCValueDCS::IsOkForIDark, ObjectName, AliRPCValueCurrent::kITot);

                fGlobalDataContainer->cd("iDark_Graphs");
                PlotsIDark[iSide][iPlane][iRPC]->Fit("pol1","Q");
                PlotsIDark[iSide][iPlane][iRPC]->Write(Form("iDark_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kSingleKey|TObject::kOverwrite);

                PlotsVoltage[iSide][iPlane][iRPC]=new TGraph();
                PlotsVoltage[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsVoltage[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsVoltage[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsVoltage[iSide][iPlane][iRPC]->SetMarkerSize(0.15);

                PlotSomethingVersusTime(PlotsVoltage[iSide][iPlane][iRPC],&AliRPCValueDCS::IsVoltage,ObjectName);

                fGlobalDataContainer->cd("Voltage_Graphs");
                PlotsVoltage[iSide][iPlane][iRPC]->Write(Form("Voltage_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kSingleKey|TObject::kOverwrite);

                PrintWhichRPC(iRPC, iSide, iPlane);

            }
        }
    }
    fGlobalDataContainer->Flush();
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
//    TObjArray *buffer;
    TGraph *AMANDAPlotsINet[kNSides][kNPlanes][kNRPC];

    TSmartTree *GlobalDataTree;
    TBranch *GlobalDataTreeBranch;
    AliRPCValueDCS *GlobalDataTreeBuffer;

    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){

                TString NameGlobal = fGlobalDataTree[iSide][iPlane][iRPC]->GetName();
                TString TitleGlobal = fGlobalDataTree[iSide][iPlane][iRPC]->GetTitle();


                fGlobalDataContainer->cd();
                GlobalDataTreeBuffer = new AliRPCValueDCS();
                GlobalDataTree = new TSmartTree(NameGlobal,TitleGlobal);
                //if ( !GlobalDataTree->GetIsSorted() ) GlobalDataTree->Sort("fTimeStamp");
                GlobalDataTreeBranch = GlobalDataTree->Branch(NameGlobal, &GlobalDataTreeBuffer,32000,0);
                GlobalDataTreeBranch->SetAddress(&GlobalDataTreeBuffer);

                AMANDAPlotsINet[iSide][iPlane][iRPC]=new TGraph();
                AMANDAPlotsINet[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                AMANDAPlotsINet[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                AMANDAPlotsINet[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                AMANDAPlotsINet[iSide][iPlane][iRPC]->SetMarkerSize(0.15);

                Double_t darkCurrentValue = 0.;
                Double_t startTimeStamp = 0.;
                Int_t counter=0;

                Double_t iDarkt0 = 0.;
                Double_t t0 = 0.;
                Double_t iDarkt1 = 0.;
                Double_t t1 = 0.;

                fGlobalDataTree[iSide][iPlane][iRPC]->Sort("fTimeStamp");
                fGlobalDataTree[iSide][iPlane][iRPC]->SetBranchAddress(fGlobalDataTree[iSide][iPlane][iRPC]->GetName(),&fGlobalDataTreeBufferW[iSide][iPlane][iRPC]);

                for (Long64_t iGlobalData=0; iGlobalData < fGlobalDataTree[iSide][iPlane][iRPC]->GetEntries(); fGlobalDataTree[iSide][iPlane][iRPC]->GetEntry(iGlobalData++)) {

                    AliRPCValueDCS *bufferValue = fGlobalDataTreeBufferW[iSide][iPlane][iRPC];

                    fGlobalDataTree[iSide][iPlane][iRPC]->GetSortedEntry(iGlobalData);

                    //skip if is not current
                    if ( !bufferValue->IsCurrent() ) continue;

                    // if the read value is an AMANDA or OCDB with run reading, then the dark
                    // current subtraction must take place. To do that via
                    // interpolation, having stored the previous dark current
                    // reading in darkCurrentValue, one should look for the
                    // following dark current value and the interpolate.

                    // If the current value is flagged as a dark current we want to get the following dark current
                    // and load the current values and timestamps. These values will be used later.
                    if ( bufferValue->IsOkForIDark() ) {

                        iDarkt0 = 0.;
                        t0 = 0.;
                        iDarkt1 = 0.;
                        t1 = 0.;


                        if (((AliRPCValueCurrent*)bufferValue)) {
                            iDarkt0 = ((AliRPCValueCurrent *) bufferValue)->GetITot();
                            t0 = ((AliRPCValueCurrent *) bufferValue)->GetTimeStamp();


                            for (Long64_t iGlobalDataNext=iGlobalData+1; iGlobalDataNext < fGlobalDataTree[iSide][iPlane][iRPC]->GetEntries(); fGlobalDataTree[iSide][iPlane][iRPC]->GetEntry(iGlobalDataNext++)) {

                                fGlobalDataTree[iSide][iPlane][iRPC]->GetSortedEntry(iGlobalDataNext);

                                if (((AliRPCValueDCS *) bufferValue)->IsOkForIDark()) break;


                                if (((AliRPCValueCurrent*)bufferValue)) {
                                    iDarkt1 = ((AliRPCValueCurrent *) bufferValue)->GetITot();
                                    t1 = ((AliRPCValueCurrent *) bufferValue)->GetTimeStamp();
                                }

                                    //printf("%f %f %f %f \n", iDarkt0, t0, iDarkt1, t1);
                            }

                            fGlobalDataTree[iSide][iPlane][iRPC]->GetSortedEntry(iGlobalData);
                        }
                    }

                    // If the current value is relative to a NON dark run we want to set the dark current for that
                    // current object to the interpolated value of the dark currents taken from the dark runs.
                    else {

                        //printf("IDark %f %f %f %f \n", iDarkt0, t0, iDarkt1, t1);

                        // current timestamp is neede for the linear interpolation of the dark current
                        Double_t tnow = ((AliRPCValueCurrent*)bufferValue)->GetTimeStamp();
                        Double_t darkCurrent = tnow * (iDarkt1 - iDarkt0) / (t1 - t0) + iDarkt0;

                        //printf("tnow %f \n", tnow);

                        // negative dark current is non physical...
                        if (darkCurrent < 0.) darkCurrent = 0;
                        ((AliRPCValueCurrent *) bufferValue)->SetIDark(darkCurrent);
                        if ( ((AliRPCValueCurrent *) bufferValue)->GetINet() == 0.)  ((AliRPCValueCurrent *) bufferValue)->SetIDark((iDarkt1 + iDarkt0)/2.);

                        //cout<<" dark current set "<< ((AliRPCValueCurrent *) *iterValueGlobal)->GetIDark() << endl;

                        // the subtraction is not direct: the dark current
                        // value is set for each reading.
                        // The subtraction will take place at the moment of
                        // asking the reading the iNET value
                        // (since it returns iTOT-iDARK).
                        if (((AliRPCValueCurrent *) bufferValue)->GetTimeStamp() > 8000 &&
                            ((AliRPCValueCurrent *) bufferValue)->GetINet() >= 0.)
                            AMANDAPlotsINet[iSide][iPlane][iRPC]->SetPoint(counter++,
                                                                           ((AliRPCValueCurrent *) bufferValue)->GetTimeStamp(),
                                                                           ((AliRPCValueCurrent *) bufferValue)->GetINet() /
                                                                           fRPCAreas[iRPC][iPlane]);
                    }


                    GlobalDataTreeBuffer = bufferValue;
                    GlobalDataTree->Fill();

                    bufferValue = 0x0;
                }

                fGlobalDataContainer->cd("iNet_Graphs");
                AMANDAPlotsINet[iSide][iPlane][iRPC]->Write(Form("iNet_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1));

                //Update TTree by replacing the old TTree with the updated one
                fGlobalDataContainer->cd();
                // Data members have to be updated after the update of the tree
                fGlobalDataTreeBranch[iSide][iPlane][iRPC] = nullptr;
                //delete fAMANDADataTreeBufferW[iSide][iPlane][iRPC];

                cout << "Entries " << fAMANDADataTree[iSide][iPlane][iRPC]->GetEntries() << "    " << GlobalDataTree->GetEntries() << endl << flush;

                fGlobalDataTree[iSide][iPlane][iRPC]->Delete();
                fGlobalDataTree[iSide][iPlane][iRPC] = GlobalDataTree;
                fGlobalDataTreeBufferW[iSide][iPlane][iRPC] = new AliRPCValueDCS();
                fGlobalDataTreeBranch[iSide][iPlane][iRPC] = fGlobalDataTree[iSide][iPlane][iRPC]->Branch(NameGlobal,&fGlobalDataTreeBufferW[iSide][iPlane][iRPC],32000,0);
                fGlobalDataTree[iSide][iPlane][iRPC]->Write(NameGlobal, TObject::kOverwrite);

                PrintWhichRPC(iRPC, iSide, iPlane);
            }
        }
    }
}


// This method integrates vs time the iNET valueDCS in order to get the
// integrated charge for each channel. And plots it.
void AliRPCAutoIntegrator::Integrator(){
    TGraph *buffer;
    TGraph *AMANDAPlotsIntegratedCharge[kNSides][kNPlanes][kNRPC];

   //arrays contains {iSide, IPlane, IRPC, integratedCharge}
    RPC LessExposedRPC;
    RPC MostExposedRPC;
    Double_t MaxCharge=0.;
    Double_t MinCharge=std::numeric_limits<Double_t>::max();

    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                fGlobalDataContainer->GetObject(Form("iNet_Graphs/iNet_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),buffer);

                // if any fAMANDAData list is missing, then the channel
                // (aka {iSide,iPlane,iRPC}) is skipped from the whole following
                // analysis
                if (!buffer){
                    printf("iNet_Graphs/iNet_Graph_MTR_%s_MT%d_RPC%d NOT FOUND\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1);
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
                    
                    if((timestamp1-timestamp0)>3*24*60*60) continue;

                    // the integrated charge is an incremental value
                    integratedCharge+=(timestamp1-timestamp0)*(iNet0+iNet1)/2.;

                    AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]->SetPoint(counter++, (timestamp0+timestamp1)/2., integratedCharge);
                }

                if(integratedCharge<MinCharge){
                    LessExposedRPC.Plane=iPlane;
                    LessExposedRPC.Side=iSide;
                    LessExposedRPC.RPC=iRPC+1;
                    MinCharge=integratedCharge;
                }else if(integratedCharge>=MaxCharge){
                    MostExposedRPC.Plane=iPlane;
                    MostExposedRPC.Side=iSide;
                    MostExposedRPC.RPC=iRPC+1;
                    MaxCharge=integratedCharge;
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

                fGlobalDataContainer->cd("integrated_charge_Graphs");
                AMANDAPlotsIntegratedCharge[iSide][iPlane][iRPC]->Write(Form("integrated_charge_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kOverwrite|TObject::kSingleKey);

                PrintWhichRPC(iRPC, iSide, iPlane);

            }
        }
    }

    //Plot the best and the worst chamber
    TMultiGraph *MostAndLessExposedRPCMultiGraph=new TMultiGraph("MostAndLessExposedRPCMultiGraph","MostAndLessExposedRPCMultiGraph");
    //GetBest
    fGlobalDataContainer->GetObject(Form("integrated_charge_Graphs/integrated_charge_Graph_MTR_%s_MT%d_RPC%d",(fSides[LessExposedRPC.Side]).Data(),fPlanes[LessExposedRPC.Plane],LessExposedRPC.RPC+1),buffer);
    MostAndLessExposedRPCMultiGraph->Add(buffer);
    //GetWorst
    fGlobalDataContainer->GetObject(Form("integrated_charge_Graphs/integrated_charge_Graph_MTR_%s_MT%d_RPC%d",(fSides[MostExposedRPC.Side]).Data(),fPlanes[MostExposedRPC.Plane],MostExposedRPC.RPC+1),buffer);
    MostAndLessExposedRPCMultiGraph->Add(buffer);
    fGlobalDataContainer->cd("integrated_charge_Graphs");
    MostAndLessExposedRPCMultiGraph->Write(Form("integrated_charge_Graph"),TObject::kOverwrite|TObject::kSingleKey);
    printf("Best RPC: MTR_%s_MT%d_RPC%d\t charge:%f \n",(fSides[LessExposedRPC.Side]).Data(),fPlanes[LessExposedRPC.Plane],LessExposedRPC.RPC,MinCharge);
    printf("Worst RPC: MTR_%s_MT%d_RPC%d\t charge:%f \n",(fSides[MostExposedRPC.Side]).Data(),fPlanes[MostExposedRPC.Plane],MostExposedRPC.RPC,MaxCharge);

    fGlobalDataContainer->Flush();
}

void AliRPCAutoIntegrator::IntegratorPerRun(){
    cout<<"\nGenerating integrated charge graphs"<<endl;
    TGraph *PlotsIntegratedCharge[kNSides][kNPlanes][kNRPC];
    TString dirName("integrated_charge_Graphs");
    
    TObject *check=0x0;
    fPlotContainer->GetObject(dirName,check);
    if(!check){
        fPlotContainer->mkdir(dirName);
    }
    
    //arrays contains {iSide, IPlane, IRPC, integratedCharge}
    RPC LessExposedRPC;
    RPC MostExposedRPC;
    Double_t MaxCharge=0.;
    Double_t MinCharge=std::numeric_limits<Double_t>::max();
    
    //Plot the best and the worst chamber
    TMultiGraph *MostAndLessExposedRPCMultiGraph=new TMultiGraph("MostAndLessExposedRPCMultiGraph","MostAndLessExposedRPCMultiGraph");
    TGraph *MeanGraph=new TGraph();
    TGraph *MostGraph, *LessGraph;
    Int_t counter=0;
    Int_t meanCounter=0;
    Double_t IntegratedCharge=0.;
    Double_t MeanIntegratedCharge=0.;
    
    vector<AliRPCRunStatistics*> list;
    
    for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
        for(Int_t iSide=0;iSide<kNSides;iSide++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){
                counter=0;
                IntegratedCharge=0.;
                //get and sort list of run for this RPC
                list=((AliRPCData*)fAliRPCDataObject)->GetRunStatistics(iPlane,iSide,iRPC);
                std::sort(list.begin(),list.end(),AliRPCRunStatistics::SortRunStatistics);
                
                PlotsIntegratedCharge[iSide][iPlane][iRPC]=new TGraph();
                PlotsIntegratedCharge[iSide][iPlane][iRPC]->SetFillColor(0);
                PlotsIntegratedCharge[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsIntegratedCharge[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsIntegratedCharge[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsIntegratedCharge[iSide][iPlane][iRPC]->SetMarkerSize(0.15);
                
                
                Double_t timestamp0=0;
                Double_t timestamp1=timestamp0;
                Double_t iNet0=0;
                Double_t iNet1=iNet0;
                
                for(std::vector<AliRPCRunStatistics*>::iterator iter=list.begin();iter!=list.end();iter++){
                    Double_t IntChargeBuffer=(iNet0+iNet1)*(timestamp1-timestamp0)/2;
                    if(!(IntChargeBuffer>0)) IntChargeBuffer=0;
                    
                    
                    //if timestamps differe for more than 3 days (eg shutdown)
                    //integrated charge should not be incremented
                    if((timestamp1-timestamp0)>3*24*60*60) {
                        timestamp0=timestamp1;
                        iNet0=iNet1;
                        continue;
                    }
                    
                    //sum integrated charge for new run
                    IntegratedCharge+=IntChargeBuffer;
                    
                    PlotsIntegratedCharge[iSide][iPlane][iRPC]->SetPoint(counter++,(timestamp0+timestamp1)/2,IntegratedCharge);
                    
                    timestamp0=timestamp1;
                    iNet0=iNet1;
                    timestamp1=(*iter)->GetTimeStampStart();
                    iNet1=(*iter)->GetMeanNetCurrent()/fRPCAreas[iPlane][iRPC];
                }
                
                //estimate max and min RPC
                if(IntegratedCharge<MinCharge){
                    LessExposedRPC.Plane=iPlane;
                    LessExposedRPC.Side=iSide;
                    LessExposedRPC.RPC=iRPC+1;
                    MinCharge=IntegratedCharge;
                    LessGraph=PlotsIntegratedCharge[iSide][iPlane][iRPC];
                }else if(IntegratedCharge>=MaxCharge){
                    MostExposedRPC.Plane=iPlane;
                    MostExposedRPC.Side=iSide;
                    MostExposedRPC.RPC=iRPC+1;
                    MaxCharge=IntegratedCharge;
                    MostGraph=PlotsIntegratedCharge[iSide][iPlane][iRPC];
                }
                
                printf("RPC: MTR_%s_MT%d_RPC%d\t charge:%f \n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1,IntegratedCharge);
                fPlotContainer->cd(dirName);
                PlotsIntegratedCharge[iSide][iPlane][iRPC]->Write(Form("integrated_charge_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kOverwrite||TObject::kSingleKey);
                
            }
        }
    }
    
    //calculate mean integratedcharge
    for(std::vector<AliRPCRunStatistics*>::iterator iter=list.begin();iter!=list.end()-1;iter++){
        MeanIntegratedCharge+=((AliRPCData*)(fAliRPCDataObject))->GetMeanIntegratedCharge((*iter)->GetRunNumber(),kTRUE);
        MeanGraph->SetPoint(meanCounter++,(*iter)->GetTimeStampStart(),MeanIntegratedCharge);
    }
    
    TString LessString(Form("Less exposed RPC: MTR_%s_MT%d_RPC%d\t charge:%f \n",(fSides[LessExposedRPC.Side]).Data(),fPlanes[LessExposedRPC.Plane],LessExposedRPC.RPC,MinCharge));
    TString MostString(Form("Most exposed RPC: MTR_%s_MT%d_RPC%d\t charge:%f \n",(fSides[MostExposedRPC.Side]).Data(),fPlanes[MostExposedRPC.Plane],MostExposedRPC.RPC,MaxCharge));
    
    printf("%s",MostString.Data());
    printf("%s",LessString.Data());
    
    MeanGraph->SetLineColor(kCyan-3);
    MeanGraph->SetMarkerSize(0.15);
    MeanGraph->SetMarkerColor(kCyan-3);
    MeanGraph->SetMarkerStyle(24);
    MostAndLessExposedRPCMultiGraph->Add(MeanGraph);
    MostAndLessExposedRPCMultiGraph->Add(MostGraph);
    MostAndLessExposedRPCMultiGraph->Add(LessGraph);
    
    fPlotContainer->cd(dirName);
    MostAndLessExposedRPCMultiGraph->Write("integrated_charge_Graph",TObject::kOverwrite|TObject::kSingleKey);
    
    fPlotContainer->Flush();
}


void AliRPCAutoIntegrator::AMANDATextToCParser(){

    Int_t mts[23];
    mts[11]=0;
    mts[12]=1;
    mts[21]=2;
    mts[22]=3;

    Bool_t isZero=kFALSE;

    ULong64_t dummyTimeStamp=0;
    Double_t timeStamp=0;
    Double_t current=0.;
    Int_t MT=0;
    Int_t RPC=0;
    char InsideOutside='I';

    string line;
    ifstream fin (fAMANDAInputFileName.Data());
    Int_t linesCounter = 0;
    if (fin.is_open())
    {
        while (! fin.eof() )
        {
          getline (fin,line);
          if (fin.eof()) break;
          cout<<linesCounter++<<"\r";
          const char *charbuffer = (char*)line.c_str();
          if (!charbuffer) continue;
          sscanf(charbuffer,"%llu;MTR_%c",&dummyTimeStamp,&InsideOutside);
          char pattern[200];
          sprintf(pattern,"%%lf;MTR_%sSIDE_MT%%d_RPC%%d_HV.actual.iMon;%%lf",(InsideOutside=='I'?"IN":"OUT"));
          sscanf(charbuffer,pattern,&timeStamp,&MT,&RPC,&current);
        //   cout<<current<<endl;

          if((current!=0. || (current==0. && isZero)) && timeStamp>8000000.){
//            printf("%f %c=%d %d %d %.17f\n\n",timeStamp,InsideOutside,(InsideOutside=='I'?0:1),MT,RPC,current);
//            cout<<timeStamp<<endl;
              fAMANDADataTreeBufferW[(InsideOutside=='I'?0:1)][mts[MT]][RPC-1] = new AliRPCValueCurrent(0, timeStamp, 0, current, 0., kFALSE,"",0.f,"", 0, kTRUE);
              fAMANDADataTreeBufferW[(InsideOutside=='I'?0:1)][mts[MT]][RPC-1]->SetIsAMANDA(kTRUE);
              fAMANDADataTree[(InsideOutside=='I'?0:1)][mts[MT]][RPC-1]->Fill();
//              cout<<"filled"<<endl;
            // //if (timeStamp<8000000) continue;
            //if (!(fAMANDAData[(InsideOutside=='I'?0:1)][mts[MT]][RPC-1])) continue;
        } //else cout<<"#### skip ###"<<endl;
        }
        cout<<endl;
        fin.close();
    }
    else cout << "Unable to open file";

    fAMANDADataContainer->Write();
}

void AliRPCAutoIntegrator::OCDBDataToCParser(bool blockMode, UInt_t blockSize){

    if (!checkAlienToken()){
        TString userName;
        cerr << "Alien token is not valid" << endl;
        cout << "Please enter your Alien username " << endl;
        cin >> userName;
        initAlienToken(userName);
        if(!checkAlienToken()){
            cerr << "Alien token not valid even after reset. Proceed manually!" << endl;
            return;
        }
    }

    if (!blockMode) OCDBDataToCParserBlocks(-1);
    else{
        Int_t blockNumber = 0;
        while(!OCDBDataToCParserBlocks(blockNumber++, blockSize));
    }
}

bool AliRPCAutoIntegrator::OCDBDataToCParserBlocks(Int_t blockNumber, UInt_t blockSize){

    bool allBlocksDone = false;

    //flag che è kTRUE se l'evento è di calibrazione
    Bool_t isCalib=kFALSE;
    Bool_t isBeamPresent=kFALSE;

    auto beginOfBlock = fOCDBRunListToAdd.begin();
    auto endOfBlock = fOCDBRunListToAdd.end();

    if ( blockNumber == -1 ){

        printf("Running in single block mode.\n");

    } else {

        printf("Running in block mode.\n");

        beginOfBlock = fOCDBRunListToAdd.begin() + blockSize * blockNumber;
        endOfBlock = beginOfBlock + blockSize;

        if (beginOfBlock > fOCDBRunListToAdd.end()) {
            printf("Download of fAMANDAData ended.\n");
            allBlocksDone = true;
            return allBlocksDone;
        }

        if (endOfBlock > fOCDBRunListToAdd.end()) {
            printf("Last block to process.\n");
            endOfBlock = fOCDBRunListToAdd.end();
            allBlocksDone = true;
        }
    }

    //manager per interfacciarsi con gli OCDB
    AliCDBManager *managerPrototype = AliCDBManager::Instance();

    //loop sui run inseriti
    for (std::vector<AliOCDBRun>::iterator runIterator = beginOfBlock; runIterator != endOfBlock; ++runIterator) {
        TObject *downloadedRunToAdd=new AliOCDBRun(runIterator->fRunNumber,runIterator->fYear);

        if (fOCDBRunListDownloaded->FindObject(downloadedRunToAdd)){
            printf("Run %d(%d) has already been downloaded. Skipping...",runIterator->fRunNumber,runIterator->fYear);
            continue;
        }

        AliCDBManager *managerYearCheck = managerPrototype;

        for (Int_t year = 2017; year>2009; year--){
            managerYearCheck->SetDefaultStorage(Form("alien://folder=/alice/data/%d/OCDB",year));
            AliCDBStorage *defStorageYear = managerYearCheck->GetDefaultStorage();
            defStorageYear->QueryCDB((*runIterator).fRunNumber);
            TObjArray* arrCDBIDYear = defStorageYear->GetQueryCDBList();
            TIter nxt(arrCDBIDYear);
            AliCDBId* cdbIDYear = 0;
            Bool_t hasGRP = kFALSE;
            while ((cdbIDYear=(AliCDBId*)nxt())) {
                if (cdbIDYear->GetPath() == "GRP/GRP/Data") {hasGRP = kTRUE; break;}
            }
            if(!hasGRP){
                printf("\n\n### Can't find run %d\n\n",(*runIterator).fRunNumber);
                (*runIterator).fYear = 0000;
                continue;
            } else {
                printf("\n\n### Run %d belongs to year %d\n\n",(*runIterator).fRunNumber,year);
                (*runIterator).fYear = year;
                break;
            }
            continue; //if year is not found skip run number
        }

        if ( (*runIterator).fYear==0 ) {
            printf("\n\n\tWARNING: Run %d can't be found in any storage. Will be skipped.\n\n",(*runIterator).fRunNumber);
            continue;
        }

        Int_t RunYear=(*runIterator).fYear;
        
        if(fOCDBRunListDownloaded->FindObject(downloadedRunToAdd)){
            continue;
        }
        

        //cout<<"YEar retrieved"<<endl;

        //inizializzazione dei manager
        managerPrototype->SetDefaultStorage(Form("alien://folder=/alice/data/%d/OCDB",(*runIterator).fYear));

        //i manager puntano al run desiderato
        managerPrototype->SetRun((*runIterator).fRunNumber);

        if(CheckPointer((TNamed*)managerPrototype)) continue;

        AliCDBManager *managerCurrent = managerPrototype;
        AliCDBManager *managerVoltage = managerPrototype;
        AliCDBManager *managerRunType = managerPrototype;
        AliCDBManager *managerScaler  = managerPrototype;

        AliCDBStorage *defStorage = managerCurrent->GetDefaultStorage();
        if(CheckPointer((TNamed*)defStorage)) continue;

        defStorage->QueryCDB((*runIterator).fRunNumber);
        TObjArray* arrCDBID = defStorage->GetQueryCDBList();
        if(CheckPointer((TNamed*)arrCDBID)) continue;
        TIter nxt(arrCDBID);
        AliCDBId* cdbID = 0;
        Bool_t hasGRP = kFALSE;
        while ((cdbID=(AliCDBId*)nxt())) {
            if (cdbID->GetPath() == "GRP/GRP/Data") {hasGRP = kTRUE; break;}
        }
        if(!hasGRP){
            printf("\n\nSkipping run %d\n\n",(*runIterator).fRunNumber);
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

        //cout<<"Beam type retrieved"<<endl;

        //settaggio del flag beamPresence
        isBeamPresent = (beamEnergy > 1.) ? kTRUE : kFALSE ;

        //printf("-------------\n#####   Beam energy:%f Beam presence:%s Beam type:%s LHC State:%s \n-------------\n",beamEnergy,(isBeamPresent) ? "true" : "false",beamType->Data(), LHCState->Data());

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

        //cout<<"ready to go"<<endl;

        //loop sui piani, i lati (inside e outside) e le RPC (9 per side)
        for (Int_t plane=0; plane<kNPlanes; plane++) {
            for (Int_t side=0; side<kNSides; side++) {
                for (Int_t RPC=1; RPC<=kNRPC; RPC++) {

                    Int_t dummyIndex = 0;
                    if(fAliRPCDataObject->IsThereThisRun(plane,side,RPC-1,runIterator->fRunNumber,dummyIndex)) {
                        printf("Run %d already there for ", runIterator->fRunNumber);
                        PrintWhichRPC(RPC - 1, side, plane);
                        continue;
                    }

                    //creazione di un pointer all'elemento della mappa delle tensioni
                    TObjArray *dataArrayVoltage;
                    dataArrayVoltage=(TObjArray*)(mapCurrent->GetValue(Form("MTR_%s_MT%d_RPC%d_HV.vEff",fSides[side].Data(),fPlanes[plane],RPC)));

                    if(!dataArrayVoltage) {
                        printf(" Problems getting dataArrayVoltage\n");
                        break;
                    }

//                    cout<<"N voltage entries= "<<dataArrayVoltage->GetEntries()<<endl;

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
                            fOCDBDataTreeBufferW[side][plane][RPC-1] = new AliRPCValueVoltage((*runIterator).fRunNumber,value->GetTimeStamp(),RunYear,value->GetFloat(),isCalib,*beamType,beamEnergy,*LHCState);
                            fOCDBDataTreeBranch[side][plane][RPC-1]->SetAddress(&fOCDBDataTreeBufferW[side][plane][RPC-1]);
                            fOCDBDataTree[side][plane][RPC-1]->Fill();
//                                    cout<<"Filled tree! "<<fOCDBDataTree[side][plane][RPC-1]->GetEntries()<<endl;
                        }
//                        //cout<<"\t"<<value->GetFloat()<<endl;
//                        auto buffer = new AliRPCValueVoltage((*runIterator).fRunNumber,value->GetTimeStamp(),RunYear,value->GetFloat(),isCalib,*beamType,beamEnergy,*LHCState);
//                        fOCDBData[side][plane][RPC-1]->Add(buffer->Clone());
//                        fOCDBData[side][plane][RPC-1]->SetOwner(kTRUE);
//                        delete buffer;
//                        value = 0x0;
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
//                    cout<<"N current entries= "<<dataArrayCurrents->GetEntries()<<endl;
                    for (Int_t arrayIndex=0; arrayIndex<(dataArrayCurrents->GetEntries()); arrayIndex++) {
                        //puntatore all'elemento i-esimo
                        AliDCSValue *value = (AliDCSValue*)dataArrayCurrents->At(arrayIndex);
                        //se il run è di calibrazione corrente e corrente di buio coincidono
                        if (isCalib) {
                            fOCDBDataTreeBufferW[side][plane][RPC-1] = new AliRPCValueCurrent((*runIterator).fRunNumber,value->GetTimeStamp(),RunYear,value->GetFloat(),value->GetFloat(),isCalib,*beamType,beamEnergy,*LHCState ,0);
                            fOCDBDataTreeBranch[side][plane][RPC-1]->SetAddress(&fOCDBDataTreeBufferW[side][plane][RPC-1]);
                            fOCDBDataTree[side][plane][RPC-1]->Fill();
                            //((AliRPCValueDCS*)fOCDBData[side][plane][RPC-1]->Last())->PrintBeamStatus();
                            //altrimenti imposto la corrente di buio a 0 (la cambio dopo)
                        } else {
                            fOCDBDataTreeBufferW[side][plane][RPC-1] = new AliRPCValueCurrent((*runIterator).fRunNumber,value->GetTimeStamp(),RunYear,value->GetFloat(),0.,isCalib,*beamType,beamEnergy,*LHCState,0);
                            fOCDBDataTreeBranch[side][plane][RPC-1]->SetAddress(&fOCDBDataTreeBufferW[side][plane][RPC-1]);
                            fOCDBDataTree[side][plane][RPC-1]->Fill();
                            //((AliRPCValueDCS*)fOCDBData[side][plane][RPC-1]->Last())->PrintBeamStatus();
                        }
                        //cout<<"\t"<<value->GetFloat()<<"   "<<value->GetTimeStamp()<<endl;
                        value = 0x0;
                    }

//                    fOCDBDataTree[side][plane][RPC-1]->Write(0,TObject::kWriteDelete,0);
                    //Form("OCDB_Data_MTR_%s_MT%d_RPC%d",(fSides[side]).Data(),fPlanes[plane],RPC)
                }
            }
        }

//        if(arrayScalers)printf("\n\n\n##### Scalers %d ######\n\n\n",arrayScalers->GetEntries());

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
//        cout<<"entries: "<<arrayScalers->GetEntries()<<endl;
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

//                        cout<<"{"<<localBoard<<","<<iRPC09<<","<<iSide<<"},";


                        // se c'è overflow scarto il dato
                        readLB[cathode][iSide][plane][iRPC09-1]++;
                        Bool_t isOverflow=kFALSE;
                        if (scalersData->GetLocScalStripOver(cathode, plane, localBoard)!=0. ){//| scalersData->GetLocScalStrip(cathode, plane, localBoard)==0.) {
//                            printf(" MTR %d cathode %d LB %d RPC %d or %d_%s timestamp %lu fAMANDAData %d OVERFLOW\n",fPlanes[plane],cathode,localBoard,iRPC017,iRPC09,(fSides[iSide]).Data(),SOR+elapsedTime,scalersData->GetLocScalStrip(cathode, plane, localBoard));
                            overflowLB[cathode][iSide][plane][iRPC09-1]++;
                            readLB[cathode][iSide][plane][iRPC09-1]--;
                            isOverflow=kTRUE;
                        }

                        // se la lettura non è quella a fine run immagazzino il dato con timestamp pari a SOR+DeltaT
                        if(scalerEntry!=arrayScalersEntries-1){
                            fOCDBRPCScalersTreeBufferW[cathode][iSide][plane][iRPC09-1] = new AliRPCValueScaler((*runIterator).fRunNumber, SOR+elapsedTime,RunYear, scalersData->GetLocScalStrip(cathode, plane, localBoard), isCalib,*beamType,beamEnergy,*LHCState, scalersData->GetDeltaT(), isOverflow);
                        }
                            // altrimenti il timestamp è pari all'EOR
                        else {
                            fOCDBRPCScalersTreeBufferW[cathode][iSide][plane][iRPC09-1] = new AliRPCValueScaler((*runIterator).fRunNumber, EOR, RunYear,scalersData->GetLocScalStrip(cathode, plane, localBoard), isCalib,*beamType,beamEnergy,*LHCState, scalersData->GetDeltaT(), isOverflow);
                        }

                        fOCDBRPCScalersTreeBranch[cathode][iSide][plane][iRPC09-1]->SetAddress(&fOCDBRPCScalersTreeBufferW[cathode][iSide][plane][iRPC09-1]);
                        fOCDBLBScalersTreeBranch[cathode][plane][localBoard]->SetAddress(&fOCDBRPCScalersTreeBufferW[cathode][iSide][plane][iRPC09-1]);

                        fOCDBRPCScalersTree[cathode][iSide][plane][iRPC09-1]->Fill();
                        fOCDBLBScalersTree[cathode][plane][localBoard]->Fill();

//                        printf(" MTR %d cathode %d LB %d RPC %d or %d_%s timestamp %lu fAMANDAData %d\n",fPlanes[plane],cathode,localBoard,iRPC017,iRPC09,(fSides[iSide]).Data(),SOR+elapsedTime,scalersData->GetLocScalStrip(cathode, plane, localBoard));
                    }
                    //cout<<"}"<<endl;
                }
            }
            scalersData=0x0;
        }
        delete scalersData;

        printf("scalers reading complete.\n");

        fOCDBRunListDownloaded->Add(downloadedRunToAdd);
        
        delete runType;
        delete beamType;
        delete LHCState;
    }

//        fOCDBDataContainer->Write();

        
        //delete runType, beamType, LHCState;
    printf("\n\n\nData retrieving complete\n\n\n");

    for (Int_t plane=0; plane<kNPlanes; plane++) {
        for (Int_t side = 0; side < kNSides; side++) {
            for (Int_t RPC = 1; RPC <= kNRPC; RPC++) {
                fOCDBDataContainer->cd();
                fOCDBDataTree[side][plane][RPC - 1]->Write("", TObject::kOverwrite);
                fOCDBDataContainer->cd();
                fOCDBRPCScalersTree[0][side][plane][RPC - 1]->Write("", TObject::kOverwrite);
                fOCDBDataContainer->cd();
                fOCDBRPCScalersTree[1][side][plane][RPC - 1]->Write("", TObject::kOverwrite);
            }

            for (Int_t localBoard=0; localBoard<kNLocalBoards; localBoard++) {
                fOCDBDataContainer->cd();
                fOCDBLBScalersTree[0][plane][localBoard]->Write("", TObject::kOverwrite);
                fOCDBDataContainer->cd();
                fOCDBLBScalersTree[1][plane][localBoard]->Write("", TObject::kOverwrite);
            }
        }
    }

    fOCDBDataContainer->cd();
    fOCDBRunListDownloaded->Write("DownloadedRuns",TObject::kOverwrite | TObject::kSingleKey);

    printf("Downloaded list saved with %d entries\n",fOCDBRunListDownloaded->GetEntries());

    return allBlocksDone;
}

void AliRPCAutoIntegrator::OCDBDarkCurrentSetter() {

    TSmartTree *OCDBDataTree;
    TBranch *OCDBDataTreeBranch;
    AliRPCValueDCS *OCDBDataTreeBuffer;

    //loop sui piani, i lati (inside e outside) e le RPC (9 per side)
    for (Int_t plane=0; plane<kNPlanes; plane++) {
        cout << plane <<endl;
        for (Int_t side=0; side<kNSides; side++) {
            cout << side <<endl;
            for (Int_t RPC=0; RPC<kNRPC; RPC++) {

                printf("Processing: MT%d_%s_%d\r",fPlanes[plane],fSides[side].Data(),RPC+1);

                fOCDBDataContainer->cd();

                TString Name = fOCDBDataTree[side][plane][RPC]->GetName();
                TString Title = fOCDBDataTree[side][plane][RPC]->GetTitle();

                OCDBDataTreeBuffer = new AliRPCValueDCS();

                OCDBDataTree = new TSmartTree(Name,Title);
                OCDBDataTreeBranch = OCDBDataTree->Branch(Name, &OCDBDataTreeBuffer,32000,0);
                OCDBDataTreeBranch->SetAddress(&OCDBDataTreeBuffer);

                //contenitore per il run number del run dal quale deriva la misura di dark current
                UInt_t calibRunNumber=0;

                //contenitore per il valore di corrente di buio misurata
                Double_t iDarkCalib=0.;

                //flag positivo se la precedente misura di tensione è >=8500.V
                Bool_t voltageOkFlag=kFALSE;

                cout << fOCDBDataTree[side][plane][RPC]->GetEntries() << endl << flush;

                fOCDBDataContainer->cd();
                fOCDBDataTree[side][plane][RPC]->Sort("fRunNumber","fTimeStamp");
                fOCDBDataTreeBranch[side][plane][RPC]->SetAddress(&fOCDBDataTreeBufferW[side][plane][RPC]);

                //loop on entries of the retrieved tree
                AliRPCValueDCS *input = fOCDBDataTreeBufferW[side][plane][RPC];
                for (int iOCDBData = 0; iOCDBData < fOCDBDataTree[side][plane][RPC]->GetEntries() ; ++iOCDBData) {

                    //L'elemento può essere una tensione o una corrente
                    fOCDBDataTreeBranch[side][plane][RPC]->SetAddress(&fOCDBDataTreeBufferW[side][plane][RPC]);

                    if (fOCDBDataTree[side][plane][RPC]->GetSortedEntry(iOCDBData) == 0) continue;

                    //se è una tensione
                    if (input->IsVoltage()) {
                        //cast a tensione
                        AliRPCValueVoltage* valueVoltage = reinterpret_cast<AliRPCValueVoltage*>(input);
                        //settaggio del flag
                        voltageOkFlag=(Bool_t)(valueVoltage->GetVSupp()>=8500.);
                        OCDBDataTreeBranch->SetAddress(&valueVoltage);
                        OCDBDataTree->Fill();
                        valueVoltage=0x0;
                        //se è una corrente
                    } else if (input->IsCurrent()) {
                        //cast a corrente
                        AliRPCValueCurrent* valueCurrent = reinterpret_cast<AliRPCValueCurrent*>(input);
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
                        }
                        OCDBDataTreeBranch->SetAddress(&valueCurrent);
                        OCDBDataTree->Fill();
                        valueCurrent=0x0;
                    } else {
                        cout<<"ERROR: Neither current or voltage"<<endl;
                    }
                }

                // Data members have to be updated after the update of the tree
                fOCDBDataContainer->cd();
                fOCDBDataTreeBranch[side][plane][RPC] = nullptr;
                delete fOCDBDataTreeBufferW[side][plane][RPC];

                fOCDBDataTree[side][plane][RPC]->Delete();
                fOCDBDataTree[side][plane][RPC] = OCDBDataTree;
                fOCDBDataTreeBufferW[side][plane][RPC] = new AliRPCValueDCS();
                fOCDBDataTreeBranch[side][plane][RPC] = fOCDBDataTree[side][plane][RPC]->Branch(Name,&fOCDBDataTreeBufferW[side][plane][RPC],32000,0);
                fOCDBDataTree[side][plane][RPC]->Write(Name, TObject::kOverwrite);

                cout << OCDBDataTree->GetEntries() << endl << flush;
            }
        }
    }

    printf("\n\n\nDark currents setting complete\n\n\n");

    for (Int_t plane=0; plane<kNPlanes; plane++) {
        for (Int_t side = 0; side < kNSides; side++) {
            for (Int_t RPC = 1; RPC <= kNRPC; RPC++) {
                fOCDBDataContainer->cd();
                fOCDBDataTree[side][plane][RPC - 1]->Write("", TObject::kOverwrite);
                fOCDBDataContainer->cd();
                fOCDBRPCScalersTree[0][side][plane][RPC - 1]->Write("", TObject::kOverwrite);
                fOCDBDataContainer->cd();
                fOCDBRPCScalersTree[1][side][plane][RPC - 1]->Write("", TObject::kOverwrite);
            }

            for (Int_t localBoard=0; localBoard<kNLocalBoards; localBoard++) {
                fOCDBDataContainer->cd();
                fOCDBLBScalersTree[0][plane][localBoard]->Write("", TObject::kOverwrite);
                fOCDBDataContainer->cd();
                fOCDBLBScalersTree[1][plane][localBoard]->Write("", TObject::kOverwrite);
            }
        }
    }
}


void AliRPCAutoIntegrator::FillAliRPCData(){

//    TObjArray *sortedListData[kNSides][kNPlanes][kNRPC];
//    TObjArray *sortedListScalers[kNSides][kNPlanes][kNRPC][kNCathodes];
//    TObjArray *scalersLocalBoardList[kNCathodes][kNPlanes][kNLocalBoards];

    Int_t previousScalers[kNCathodes][kNPlanes][kNLocalBoards];

    for (Int_t plane=0; plane<kNPlanes; plane++) {
        for (Int_t cathode=0; cathode<kNCathodes; cathode++) {
            for (Int_t localBoard=1; localBoard<=kNLocalBoards; localBoard++) {
                previousScalers[cathode][plane][localBoard-1]=0;
            }
        }
    }

    cout<<"Init done"<<endl;

    for (Int_t iSide=0; iSide<kNSides; iSide++) {
        for (Int_t iPlane = 0; iPlane < kNPlanes; iPlane++) {
            for (Int_t iRPC = 1; iRPC <= kNRPC; iRPC++) {
                UInt_t actualRunNumber = 0;
                UInt_t actualYear=0;
                UInt_t previousRunNumber = 0;
                ULong64_t timeStampStart = 0;
                ULong64_t timeStampStop = 0;
                Bool_t isCalib = kFALSE;
                Bool_t isDark=kFALSE;
                Double_t meanDarkCurrent = 0.;
                UInt_t nDarkCurrent = 0;
                Double_t meanTotalCurrent = 0.;
                UInt_t nTotalCurrent = 0;
                Double_t meanHV = 0.;
                UInt_t nHV = 0;
                Double_t RPCTotalRatePerArea[2] = {0., 0.};
                ULong64_t totalScalerCounts[2] = {0, 0};
                
                UInt_t AMANDAActualRunNumber=0;
                ULong64_t AMANDATimeStampStart = 0;
                ULong64_t AMANDATimeStampStop = 0;
                ULong64_t AMANDAFakeRunTime=3*24*60*60;
                Double_t AMANDAMeanTotalCurrent = 0.;
                UInt_t AMANDANTotalCurrent = 0;

                //printf("Beginning MT%d %s RPC%d -> ",fPlanes[iPlane],fSides[iSide].Data(),iRPC);
//                fGlobalDataTree[iSide][iPlane][iRPC]->Sort("fTimeStamp");
//                fGlobalDataTree[iSide][iPlane][iRPC]->SetBranchAddress(fGlobalDataTree[iSide][iPlane][iRPC]->GetName(),&fGlobalDataTreeBufferW[iSide][iPlane][iRPC]);
                AliRPCValueDCS *valueDCS = fGlobalDataTreeBufferW[iSide][iPlane][iRPC-1];

                auto NEntries = fGlobalDataTree[iSide][iPlane][iRPC-1]->GetEntries();

                for (int iGlobal = 0; iGlobal < fGlobalDataTree[iSide][iPlane][iRPC-1]->GetEntries(); ++iGlobal) {

                    cout << (Double_t)iGlobal/(Double_t)NEntries << "\r";

                    //generica entry della sorted list
                    //AliRPCValueDCS *valueDCS = ((AliRPCValueDCS*)sortedListData[iSide][iPlane][iRPC-1]->At(iDataList));
                    if ( fGlobalDataTree[iSide][iPlane][iRPC-1]->GetSortedEntry(iGlobal) == 0 ) continue;

                    if(valueDCS){
                        if (previousRunNumber == 0) {
                            previousRunNumber = valueDCS->GetRunNumber();
                            timeStampStart = valueDCS->GetTimeStamp();
                            isCalib = valueDCS->IsCalib();
                            isDark = valueDCS->GetLHCStatus()>kNONE;
                        }
                        
                        actualRunNumber = valueDCS->GetRunNumber();
                        actualYear = valueDCS->GetYear();
                    }

                   
                    Int_t dummyIndex = 0;
                    if(fAliRPCDataObject->IsThereThisRun(iPlane,iSide,iRPC-1,actualRunNumber,dummyIndex)) {
//                        printf("Run %d already there for ",actualRunNumber);
                        //PrintWhichRPC(iRPC-1,iSide,iPlane);
                            continue;
                    } else {
//                        printf("Adding run %d for ",actualRunNumber);
                        //PrintWhichRPC(iRPC-1,iSide,iPlane);
                    }
                    
                    //store AMANDA as negative run numbers;
                    
                    if(actualRunNumber==0){
                        
                        if(AMANDAActualRunNumber==0)
                        {
                            AMANDAActualRunNumber--;
                            AMANDATimeStampStart=valueDCS->GetTimeStamp();
                        }else {
                            AMANDATimeStampStop=valueDCS->GetTimeStamp();
                        }
                        
                        ULong64_t AMANDAdeltaT= AMANDATimeStampStop - AMANDATimeStampStart;
                        
                        if(AMANDAdeltaT<=AMANDAFakeRunTime){
//                            cout<<"new AMANDA value"<<endl;
                            AliRPCValueCurrent *currentBuffer= reinterpret_cast<AliRPCValueCurrent*>(valueDCS);
                            AMANDAMeanTotalCurrent+=currentBuffer->GetITot();
                            AMANDANTotalCurrent++;
                        }else{
                            if(AMANDANTotalCurrent!=0) AMANDAMeanTotalCurrent/=AMANDANTotalCurrent;
                            else AMANDAMeanTotalCurrent=0.;
//                            cout<<"new AMANDA Run"<<endl<<endl;
//                            cout<<"start: "<<AMANDATimeStampStart<<endl;
//                            cout<<"stop "<<AMANDATimeStampStop<<endl;
//                            cout<<"current "<<AMANDAMeanTotalCurrent;
//                            cout<<"RunNumber"<<AMANDAActualRunNumber;
//                            cout<<"N data "<<AMANDANTotalCurrent<<endl;
                            AliRPCRunStatistics *statsBuffer=new AliRPCRunStatistics(AMANDAActualRunNumber, AMANDATimeStampStart, AMANDATimeStampStop, actualYear, kFALSE, kFALSE, 0., AMANDAMeanTotalCurrent, 0., 0, 0);
                            fAliRPCDataObject->AddRunStatistics(iPlane, iSide, iRPC-1, statsBuffer);
                            
                            AMANDAActualRunNumber-=1;
                            AMANDATimeStampStart=AMANDATimeStampStop;
                            AMANDATimeStampStop=0;
                            AMANDAMeanTotalCurrent=0.;
                            AMANDANTotalCurrent=0;
                            
                        }
                    }
                    

                    if (actualRunNumber == previousRunNumber && valueDCS) {
                        if (valueDCS->IsVoltage()){
                            //cast a tensione
                            AliRPCValueVoltage *voltageBuffer=reinterpret_cast<AliRPCValueVoltage*>(valueDCS);
                            meanHV+=voltageBuffer->GetVSupp();
                            nHV++;
                            timeStampStop=valueDCS->GetTimeStamp();
                        } else if(valueDCS->IsCurrent()){
                            //cast a corrente
                            AliRPCValueCurrent *currentBuffer=reinterpret_cast<AliRPCValueCurrent*>(valueDCS);
                            meanTotalCurrent+=currentBuffer->GetITot();
                            nTotalCurrent++;
                            meanDarkCurrent+=currentBuffer->GetIDark();
                            nDarkCurrent++;
                            timeStampStop=valueDCS->GetTimeStamp();
                        }
                    } else if (actualRunNumber < previousRunNumber && valueDCS) {
                        continue;
                    } else if (actualRunNumber > previousRunNumber || !valueDCS){
                        Double_t ratesTimesLBArea[2] = {0., 0.};
                        Double_t LBRateSum[2] = {0., 0.};
                        Double_t notOverflowLBTotalArea[2] = {0., 0.};

                        //printf("\n######################\nRun=%d MT=%d iRPC=%d SIDE=%s\n", previousRunNumber, fPlanes[iPlane], iRPC, fSides[iSide].Data());
                        for (Int_t cathode = 0; cathode < kNCathodes; cathode++) {
                            for (Int_t localBoard = 1; localBoard <= kNLocalBoards; localBoard++) {
                                Int_t acceptedCount = 0;
                                Int_t LBScalers = 0;
                                Double_t elapsedTime = 0.;
                                Double_t LBRate = 0.;
                                Int_t iRPC09 = (kLBToRPC+localBoard - 1)[1];
                                Int_t side = (kLBToRPC+localBoard - 1)[2];
                                Bool_t isAccepted = kFALSE;
                                Int_t readingCount = 0;
                                if (iSide == side && iRPC09 == iRPC) {

                                    //printf("\n\t%s LB=%d area=%f\n",cathodes[cathode].Data(),localBoard,LBAreas[localBoard-1][iPlane]);
                                    //printf("\t\tReading\tScaler\t\tIsOF\n");
                                    //cout<<"\t"<<localBoard<<endl;
                                    //cout<<previousScalers[cathode][iPlane][localBoard-1]<<endl;
//                                    TIter iterValueScaler(scalersLocalBoardList[cathode][iPlane][localBoard - 1]);
                                    if (!fOCDBLBScalersTree[cathode][iPlane][localBoard - 1]->GetIsSorted()) fOCDBLBScalersTree[cathode][iPlane][localBoard - 1]->Sort("fRunNumber","fTimeStamp");

                                    AliRPCValueScaler *valueScaler = fOCDBLBScalersTreeBufferW[cathode][iPlane][localBoard - 1];
//                                    for (Long64_t iScaler = fOCDBLBScalersTree[cathode][iPlane][localBoard - 1]->GetLastCall(); iScaler < fOCDBLBScalersTree[cathode][iPlane][localBoard - 1]->GetEntries(); ++iScaler) {
//
//                                        if ( fOCDBLBScalersTree[cathode][iPlane][localBoard - 1]->Next() == 0 ) continue;
//
//                                        if (valueScaler->GetScalerCounts() <= 0.) continue;
//                                        //for(Int_t iScaler=previousScalers[cathode][iPlane][localBoard-1];iScaler<fOCDBLBScalers[cathode][iPlane][localBoard-1]->GetEntries();iScaler++){
//                                        //AliRPCValueScaler *valueScaler=((AliRPCValueScaler*)fOCDBLBScalers[cathode][iPlane][localBoard-1]->At(iScaler));
//                                        readingCount++;
//                                        if (valueScaler->GetRunNumber() < previousRunNumber) {
//                                            //cout<<"continue"<<iScaler<<" "<<valueScaler->GetRunNumber()<<"<"<<previousRunNumber<<endl;
//                                            //cout<<"continue "<<iScaler<<" "<<valueScaler->GetRunNumber()<<"<"<<previousRunNumber<<endl;
//                                            continue;
//                                        } else if (valueScaler->GetRunNumber() > previousRunNumber) {
//                                            //cout<<"breaking"<<iScaler<<" "<<valueScaler->GetRunNumber()<<">"<<previousRunNumber<<endl;
//                                            previousScalers[cathode][iPlane][localBoard - 1] = 0;//iScaler;
//                                            break;
//                                        } else {
//                                            //cout<<"filling"<<iScaler<<" "<<valueScaler->GetRunNumber()<<">"<<previousRunNumber<<endl;
//                                            if (valueScaler->GetScalerCounts() == 0) continue;
//                                            //(!valueScaler->GetHasOverflow())? printf("\t\t%d\t%d\t\t%s\n",readingCount,valueScaler->GetScalerCounts(),(valueScaler->GetHasOverflow())?"TRUE":"false") : printf("\t>>>\t%d\t%d\t\t%s\n",readingCount,valueScaler->GetScalerCounts(),(valueScaler->GetHasOverflow())?"TRUE":"false");
//                                            if (valueScaler->GetHasOverflow() == kTRUE) {
//                                                //cout<<"################ "<<localBoard<<" has overflow"<<endl;
//                                                continue;
//                                            } else {
////                                                cout<<"pippo"<<endl;
//                                                acceptedCount++;
//                                                isAccepted = kTRUE;
//                                                LBScalers += valueScaler->GetScalerCounts();
//                                                elapsedTime += valueScaler->GetDeltaT();
//                                            }
//                                        }
//                                        //fOCDBLBScalers[cathode][iPlane][localBoard-1]->RemoveAt(iScaler);
//                                    }

                                    if (isAccepted == kTRUE) {
                                        //cout<<localBoard<<" accepted"<<endl;
                                        LBRate = (Double_t) LBScalers / elapsedTime;
                                        LBRateSum[cathode] += LBRate;
                                        notOverflowLBTotalArea[cathode] += fLBAreas[localBoard - 1][iPlane];
                                        //printf("MT %d iRPC %d LB %d cath %d area %f totarea %f\n",iPlane,iRPC,localBoard,cathode,LBAreas[localBoard-1][iPlane],notOverflowLBTotalArea[cathode]);
                                        //cout<<notOverflowLBTotalArea[cathode]<<endl;
                                        //printf("%d %f %f %f %f\n",acceptedCount,LBRate,LBRate,LBRateSum[cathode],notOverflowLBTotalArea[cathode]);
                                    }
                                }
                            }

                            if (notOverflowLBTotalArea[cathode] != 0)
                                RPCTotalRatePerArea[cathode] = LBRateSum[cathode] / notOverflowLBTotalArea[cathode];
                            else RPCTotalRatePerArea[cathode] = -1.;
                            totalScalerCounts[cathode] = (ULong64_t) (RPCTotalRatePerArea[cathode] * (Double_t) (timeStampStop - timeStampStart) *fRPCAreas[iRPC - 1][iPlane]);
                            //if (cathode == 1)
                              //  printf("-Rates:\n\t·Bending: %f Hz/cm^2\n\t·Not bending: %f Hz/cm^2\n\n-Total scaler counts:\n\t·Bending: %llu\n\t·Not bending: %llu\n####################\n",RPCTotalRatePerArea[0], RPCTotalRatePerArea[1], totalScalerCounts[0],totalScalerCounts[1]);
                            //cout<<cathode<<": "<<totalScalerCounts[cathode]<<endl<<endl;

                        }

                        if(nDarkCurrent!=0)meanDarkCurrent=meanDarkCurrent/(Double_t)nDarkCurrent;
                        if(nTotalCurrent!=0)meanTotalCurrent=meanTotalCurrent/(Double_t)nTotalCurrent;
                        if(nHV!=0)meanHV=meanHV/(Double_t)nHV;
//                        cout<<"setting "<<previousRunNumber<<" complete"<<endl;
                        //cout<<totalScalerCounts[0]<<"\t"<<totalScalerCounts[1]<<endl;
                        //cout<<(isDark?"Dark":"NODARK")<<endl;
                        
                        //if data are overflow (aka RPCTotalRatePerArea=-1) than we must be sure count are set to 0
                        //note that only scalercounts are saved for future analysis
                        if(RPCTotalRatePerArea[0]<0) totalScalerCounts[0]=0;
                        if(RPCTotalRatePerArea[1]<0) totalScalerCounts[1]=0;
                        
                        AliRPCRunStatistics *statsBuffer=new AliRPCRunStatistics(previousRunNumber, timeStampStart, timeStampStop, actualYear, isCalib, isDark, meanDarkCurrent, meanTotalCurrent, meanHV, totalScalerCounts[0], totalScalerCounts[1]);
                        fAliRPCDataObject->AddRunStatistics(iPlane, iSide, iRPC-1, statsBuffer);

//                        statsBuffer->PrintHumanReadable();
                        
                        actualRunNumber=0;
                        timeStampStart=0;
                        timeStampStop=0;
                        isCalib=kFALSE;
                        isDark=kFALSE;
                        meanDarkCurrent=0.;
                        nDarkCurrent=0;
                        meanTotalCurrent=0.;
                        nTotalCurrent=0;
                        meanHV=0.;
                        nHV=0;
                        totalScalerCounts[0]=0;
                        totalScalerCounts[1]=0;
                        //statsBuffer=0x0;
                        //force reset of previous and start timestamp
                        previousRunNumber=0;
                        if (valueDCS){
                            previousRunNumber=valueDCS->GetRunNumber();
                            timeStampStart=valueDCS->GetTimeStamp();
                            isCalib=valueDCS->IsCalib();
                            isDark = valueDCS->GetLHCStatus()>kNONE;
                        }
                        ratesTimesLBArea[0]=0;
                        ratesTimesLBArea[1]=0;
                    }
                }
                //cout<<"DONE"<<endl;
                
                PrintWhichRPC(iRPC-1,iSide,iPlane);
            }
        }
    }

    //save AliRPCData on File
    fGlobalDataContainer->cd();
    fAliRPCDataObject->Write("AliRPCDataObj", TObject::kSingleKey | TObject::kOverwrite);
    fGlobalDataContainer->Write();

};

void AliRPCAutoIntegrator::AMANDASetDataMembers(){

    TSmartTree *AMANDADataTree;
    TBranch *AMANDADataTreeBranch;
    AliRPCValueDCS *AMANDADataTreeBuffer;

    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
            for(Int_t iRPC=0;iRPC<kNRPC;iRPC++){

                printf("Processing: MT%d_%s_%d\r",fPlanes[iPlane],fSides[iSide].Data(),iRPC+1);

                fAMANDADataContainer->cd();

                // Creating a clone of the AMANDA tree: it will replace the standard one!
                TString NameAMANDA = fAMANDADataTree[iSide][iPlane][iRPC]->GetName();
                TString TitleAMANDA = fAMANDADataTree[iSide][iPlane][iRPC]->GetTitle();

                AMANDADataTreeBuffer = new AliRPCValueDCS();
                AMANDADataTree = new TSmartTree(NameAMANDA,TitleAMANDA);
                //if ( !AMANDADataTree->GetIsSorted() ) AMANDADataTree->Sort("fTimeStamp");
                AMANDADataTreeBranch = AMANDADataTree->Branch(NameAMANDA, &AMANDADataTreeBuffer,32000,0);
                AMANDADataTreeBranch->SetAddress(&AMANDADataTreeBuffer);

                UInt_t runNumberBuffer=0;
                //it is intended as runnumber extends from runbegin to runend
                ULong64_t runBeginBuffer=0;
                ULong64_t runEndBuffer=0;

                Int_t nCallOCDB = 0;
                Int_t nCallAMANDA = 0;

                ULong64_t OCDBTimeStampBuffer;
                TBeamType OCDBRunTypeBuffer;
                Float_t OCDBBeamEnergyBuffer;
                TLHCStatus OCDBLHCStatusBuffer;
                Bool_t OCDBIsCalibBuffer;
                UInt_t OCDBRunYearBuffer;

                OCDBRunTypeBuffer = TBeamType::knone;
                OCDBBeamEnergyBuffer = 0;
                OCDBLHCStatusBuffer = TLHCStatus::kNONE;
                OCDBIsCalibBuffer = 0;
                OCDBRunYearBuffer = 0;

                fOCDBDataContainer->cd();
//                PrintWhichRPC(iRPC,iSide,iPlane);
                cout<<fOCDBDataTree[iSide][iPlane][iRPC]->GetEntries()<<"   "<< fAMANDADataTree[iSide][iPlane][iRPC]->GetEntries() <<endl;

               // cout << fOCDBDataTree[iSide][iPlane][iRPC]->Sort("fRunNumber","fTimeStamp") << endl;
                fOCDBDataTree[iSide][iPlane][iRPC]->SetBranchAddress(fOCDBDataTree[iSide][iPlane][iRPC]->GetName(),&fOCDBDataTreeBufferW[iSide][iPlane][iRPC]);

                AliRPCValueDCS *OCDBDataPtr = fOCDBDataTreeBufferW[iSide][iPlane][iRPC];


                fAMANDADataContainer->cd();
                //cout << fAMANDADataTree[iSide][iPlane][iRPC]->Sort("fTimeStamp") << endl;
                fAMANDADataTree[iSide][iPlane][iRPC]->SetBranchAddress(fAMANDADataTree[iSide][iPlane][iRPC]->GetName(),&fAMANDADataTreeBufferW[iSide][iPlane][iRPC]);

                AliRPCValueDCS *AMANDADataPtr = fAMANDADataTreeBufferW[iSide][iPlane][iRPC];

                //iter on OCDB until runnumber changes
                for (Long64_t iOCDB = 0; iOCDB < fOCDBDataTree[iSide][iPlane][iRPC]->GetEntries(); iOCDB++) {

                    nCallOCDB++;
                    //cout << nCallOCDB << endl;

//                    cout << "Getting data 1" << endl << flush;
                    if ( fOCDBDataTree[iSide][iPlane][iRPC]->GetSortedEntry(iOCDB) == 0 ) continue;

                    //if ( fOCDBDataTree[iSide][iPlane][iRPC]->GetSortedEntry(iOCDB) == 0 ) continue;

                    UInt_t localOCDBRunNumberBuffer= OCDBDataPtr->GetRunNumber();
                    ULong64_t localOCDBTimeStampBuffer= OCDBDataPtr->GetTimeStamp();

//                    cout << "Getting data 2" << endl << flush;

                    if (localOCDBRunNumberBuffer > runNumberBuffer){

                        for (Long64_t iAMANDA = 0; iAMANDA < fAMANDADataTree[iSide][iPlane][iRPC]->GetEntries(); iAMANDA++) {

                            nCallAMANDA++;

                            if ( fAMANDADataTree[iSide][iPlane][iRPC]->GetSortedEntry(iAMANDA) == 0 ) continue;

//                            cout << "Getting data 3" << endl << flush;

                            ULong64_t AMANDATimeStamp = AMANDADataPtr->GetTimeStamp();
//
                            if ( AMANDATimeStamp < runBeginBuffer ) {
//                                AMANDADataTreeBuffer = AMANDADataPtr;
//                                AMANDADataTree->Fill();
                                continue;
                            } //AMANDA value has been registered sooner than OCDB fAMANDAData skip AMANDA value
                            else if ( AMANDATimeStamp >= runBeginBuffer ){ //if AMANDA value is after run begin
                                if ( AMANDATimeStamp <= runEndBuffer ){ //if AMANDA value is sooner than run end
//                                    cout<<"Value is between"<<endl;

                                    AMANDADataTreeBuffer->SetRunNumber(runNumberBuffer);
                                    AMANDADataTreeBuffer->SetBeamType(OCDBRunTypeBuffer);
                                    AMANDADataTreeBuffer->SetBeamEnergy(OCDBBeamEnergyBuffer);
                                    AMANDADataTreeBuffer->SetLHCStatus(OCDBLHCStatusBuffer);
                                    AMANDADataTreeBuffer->SetIsCalib(OCDBIsCalibBuffer);
                                    AMANDADataTreeBuffer->SetRunYear(OCDBRunYearBuffer);
                                    AMANDADataTree->Fill();

                                } else { //if AMANDA value is later than run end
                                    if( AMANDATimeStamp < localOCDBTimeStampBuffer ){ //if AMANDA fAMANDAData refers to a moment without ongoing runs
                                        AMANDADataTreeBuffer = AMANDADataPtr;
                                        AMANDADataTree->Fill();
                                    }
                                }
                            }

                            // FIlling the TTree copy with updated (or not!) data
                            //cout<<nCallAMANDA<<endl;
                        }

                        runBeginBuffer = localOCDBTimeStampBuffer;
                        runNumberBuffer = localOCDBRunNumberBuffer;

                        OCDBRunTypeBuffer = OCDBDataPtr->GetBeamType();
                        OCDBBeamEnergyBuffer = OCDBDataPtr->GetBeamEnergy();
                        OCDBLHCStatusBuffer = OCDBDataPtr->GetLHCStatus();
                        OCDBIsCalibBuffer = OCDBDataPtr->IsCalib();
                        OCDBRunYearBuffer = OCDBDataPtr->GetYear();

                    } else {
                        runEndBuffer = localOCDBTimeStampBuffer;
                    }
                }

                AMANDADataTree->Sort("fTimeStamp");


                cout << "#### Writing " << AMANDADataTree->GetEntries() << endl;
                //Update TTree by replacing the old TTree with the updated one
                fAMANDADataContainer->cd();
                // Data members have to be updated after the update of the tree
                fAMANDADataTreeBranch[iSide][iPlane][iRPC] = nullptr;
                //delete fAMANDADataTreeBufferW[iSide][iPlane][iRPC];

                cout << "Entries " << fAMANDADataTree[iSide][iPlane][iRPC]->GetEntries() << "    " << AMANDADataTree->GetEntries() << endl << flush;

                fAMANDADataTree[iSide][iPlane][iRPC]->Delete();
                fAMANDADataTree[iSide][iPlane][iRPC] = AMANDADataTree;
                fAMANDADataTreeBufferW[iSide][iPlane][iRPC] = new AliRPCValueDCS();
                fOCDBDataTreeBranch[iSide][iPlane][iRPC] = fAMANDADataTree[iSide][iPlane][iRPC]->Branch(NameAMANDA,&fAMANDADataTreeBufferW[iSide][iPlane][iRPC],32000,0);
                fAMANDADataTree[iSide][iPlane][iRPC]->Write(NameAMANDA, TObject::kOverwrite);

//                PrintWhichRPC(iRPC, iSide, iPlane);
                //cout<<"Ncalls "<<nCallOCDB<< "   " <<nCallAMANDA<<endl;
                nCallAMANDA = 0;
                nCallOCDB = 0;

                delete AMANDADataTreeBuffer;
                delete AMANDADataTree;
            }
        }
    }

    fAMANDADataContainer->Flush();

}

void AliRPCAutoIntegrator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, std::vector<UInt_t> RunNumberList, Int_t whichValue) {
    Int_t counter=0;
    
    TSmartTree *tree;
    TBranch *branch;
    AliRPCValueDCS *iterValue=new AliRPCValueDCS();
    
    fGlobalDataContainer->GetObject(ObjectName,tree);
    
    if(!tree) return;
    
    branch = tree->GetBranch(ObjectName);
    tree->SetBranchAddress(ObjectName,&iterValue);
    
    for(int i=0; i<tree->GetEntries(); tree->GetSortedEntry(i++)) {
        if(IsRunInList(RunNumberList,iterValue->GetRunNumber())){
            if (((iterValue->*funky)()) &&
                (iterValue->GetTimeStamp() > 8000)) {
                Graph->SetPoint(counter++, iterValue->GetTimeStamp(),iterValue->GetValue(whichValue));
            }
        }
    }
    return;
}

void AliRPCAutoIntegrator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, UInt_t RunNumber, Int_t whichValue){
    std::vector<AliOCDBRun*> RunDummyList;
    RunDummyList.push_back(new AliOCDBRun(RunNumber,0));
    PlotSomethingVersusTime(Graph, funky, ObjectName, RunDummyList, whichValue);
    return;
}

void AliRPCAutoIntegrator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName,std::vector<AliOCDBRun*> RunNumberList, Int_t whichValue){
    std::vector<UInt_t> RunDummyList;
    for(AliOCDBRun* iter :RunNumberList){
        UInt_t temp=((UInt_t)iter->GetRunNumber());
        RunDummyList.push_back(temp);
    }
    PlotSomethingVersusTime(Graph, funky, ObjectName, RunDummyList, whichValue);
    return;
}

void AliRPCAutoIntegrator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, Int_t whichValue){
    Int_t counter=0;
    
    TSmartTree *tree;
    TBranch *branch;
    AliRPCValueDCS *iterValue=new AliRPCValueDCS();
    
    fGlobalDataContainer->GetObject(ObjectName,tree);
    
    if(!tree) return;
    tree->Print();
    
    branch = tree->GetBranch(ObjectName);
    tree->SetBranchAddress(ObjectName,&iterValue);
    tree->Print();
    
    for (int i=0; i<tree->GetEntries(); tree->GetSortedEntry(i++)) {
        if (((iterValue->*funky)()) &&
            (iterValue->GetTimeStamp() > 8000)) {
            Graph->SetPoint(counter++, iterValue->GetTimeStamp(),iterValue->GetValue(whichValue));
        }
    }
    return;
}


void AliRPCAutoIntegrator::PlotSomethingVersusRun(TGraph *Graph, Double_t (AliRPCData::*funky)(UInt_t, Bool_t)const, Bool_t normalizedToArea, Bool_t onlyDarkPoints){
    Int_t counter=0;
    Graph->SetLineColor(0);
    Graph->SetMarkerSize(1.5);
    Graph->SetMarkerStyle(24);
    vector<UInt_t> OCDBRunListComplete;
    for(Int_t iSide=0;iSide<kNSides;iSide++) {
        for (Int_t iPlane = 0; iPlane < kNPlanes; iPlane++) {
            for (Int_t iRPC = 0; iRPC < kNRPC; iRPC++) {
                vector<AliRPCRunStatistics*> bufferList=fAliRPCDataObject->GetRunStatistics(iPlane,iSide,iRPC);
                for(auto iter=bufferList.begin(); iter!=bufferList.end(); iter++){
                    auto run=(*iter)->GetRunNumber();
                    if(run==267165) continue;
                    //skip if onlyDrak is true and iter is not dark
                    if(onlyDarkPoints&&!(*iter)->GetIsDark()) continue;
                    if(!IsRunInList(OCDBRunListComplete, run)) OCDBRunListComplete.push_back(run);
                }
            }
        }
    }

    for(UInt_t iter:OCDBRunListComplete){
        Double_t x=(fAliRPCDataObject->GetMeanTimeStampStart(iter));
        if(x>8000) Graph->SetPoint(counter++,x,(fAliRPCDataObject->*funky)(iter, normalizedToArea));
    }
}

void AliRPCAutoIntegrator::PlotSomethingVersusRPC(TGraph *Graph, Double_t (AliRPCData::*funkyX)(Int_t, Int_t, Int_t, Bool_t)const, Double_t (AliRPCData::*funkyY)(Int_t, Int_t, Int_t, Bool_t)const, Bool_t normalizedToArea){
    Int_t counter=0;
    Graph->SetLineColor(0);
    Graph->SetMarkerSize(1.5);
    Graph->SetMarkerStyle(24);
    for(Int_t iSide=0;iSide<kNSides;iSide++) {
        for (Int_t iPlane = 0; iPlane < kNPlanes; iPlane++) {
            for (Int_t iRPC = 0; iRPC < kNRPC; iRPC++) {
                Double_t x=(fAliRPCDataObject->*funkyX)(iSide,iPlane,iRPC,normalizedToArea);
                if(x>=0) Graph->SetPoint(counter++,x,(fAliRPCDataObject->*funkyY)(iSide,iPlane,iRPC,normalizedToArea));
            }
        }
    }
}

void AliRPCAutoIntegrator::PlotSomethingVersusSomethingElse(TGraph *Graph, const TString y, const TString x,  Bool_t onlyDarkPoints, Bool_t normalizedToArea, Bool_t toFit, TString ObjectName){
    if(x.Contains("time")){
        TSmartTree *check=0x0;
        fGlobalDataContainer->GetObject(ObjectName,check);
        if(!check) {
            cout<<"Branch with data not found\n";
            return;
        }
        if(y.Contains("voltage")) PlotSomethingVersusTime(Graph,&AliRPCValueDCS::IsVoltage,ObjectName);
        else if(y.Contains("current")){
            if(y.Contains("dark")) PlotSomethingVersusTime(Graph,&AliRPCValueDCS::IsCurrent,ObjectName,AliRPCValueCurrent::kIDark);
            else  PlotSomethingVersusTime(Graph,&AliRPCValueDCS::IsCurrent,ObjectName,AliRPCValueCurrent::kITot);
        }
    }
    else if(y.Contains("integrated")&&y.Contains("charge")){
        Graph=0x0;
        IntegratorPerRun();
    }else if(x.Contains("run")){
        if(y.Contains("current")) {
            if(y.Contains("total")) PlotSomethingVersusRun(Graph, &AliRPCData::GetMeanTotalCurrent,normalizedToArea,onlyDarkPoints);
            if(y.Contains("dark")) PlotSomethingVersusRun(Graph, &AliRPCData::GetMeanDarkCurrent,normalizedToArea,onlyDarkPoints);
            if(y.Contains("net")) PlotSomethingVersusRun(Graph, &AliRPCData::GetMeanNetCurrent,normalizedToArea,onlyDarkPoints);
        }else if(y.Contains("voltage")){
            PlotSomethingVersusRun(Graph, &AliRPCData::GetMeanHV);
        }else if(y.Contains("rate")&&y.Contains("bending")){
            if(y.Contains("not")) PlotSomethingVersusRun(Graph, &AliRPCData::GetMeanRateNotBending,kTRUE,onlyDarkPoints);
            else PlotSomethingVersusRun(Graph, &AliRPCData::GetMeanRateBending,kTRUE,onlyDarkPoints);
        }else if(y.Contains("integrated")||y.Contains("charge")){
            PlotSomethingVersusRun(Graph, &AliRPCData::GetMeanIntegratedCharge,normalizedToArea,onlyDarkPoints);
        }
    }else{
        Double_t (AliRPCData::*Xptr)(Int_t, Int_t, Int_t, Bool_t)const;
        Double_t (AliRPCData::*Yptr)(Int_t, Int_t, Int_t, Bool_t)const;
        if(y.Contains("current")) {
            if(y.Contains("total")) Yptr =&AliRPCData::GetAverageTotalCurrent;
            if(y.Contains("net")) Yptr =&AliRPCData::GetAverageNetCurrent;
        }else if(y.Contains("voltage")){
            Yptr =&AliRPCData::GetAverageHV;
        }else if(y.Contains("rate")&&y.Contains("bending")){
            if(y.Contains("not")) Yptr =&AliRPCData::GetAverageRateNotBending;
            else Yptr =&AliRPCData::GetAverageRateBending;
        }else return;
        if(x.Contains("current")) {
            if(x.Contains("total")) Xptr =&AliRPCData::GetAverageTotalCurrent;
            if(x.Contains("net")) Xptr =&AliRPCData::GetAverageNetCurrent;
        }else if(x.Contains("voltage")){
            Xptr =&AliRPCData::GetAverageHV;
        }else if(x.Contains("rate") && x.Contains("bending")){
            if(x.Contains("not")) Xptr =&AliRPCData::GetAverageRateNotBending;
            else Xptr =&AliRPCData::GetAverageRateBending;
        }else return;
        PlotSomethingVersusRPC(Graph,Xptr,Yptr,normalizedToArea);
    }
    
    if(Graph){
        Graph->GetXaxis()->SetTitle(x);
        Graph->GetYaxis()->SetTitle(y);
        
        if(toFit) {
            cout<<"Fit of "<<y.Data()<<" vs "<<x.Data()<<":";
            Graph->Fit("pol1","M0");
            //cout<<"\n\n##########\n\n";
        }
    }
};

void AliRPCAutoIntegrator::CreateDistributionSomething(TH1 *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, vector<AliOCDBRun*> RunNumberList, Int_t whichValue){
    
    TSmartTree *tree;
    TBranch *branch;
    AliRPCValueDCS *iterValue=new AliRPCValueDCS();
    
    fGlobalDataContainer->GetObject(ObjectName,tree);
    
    if(!tree) return;
    
    branch = tree->GetBranch(ObjectName);
    tree->SetBranchAddress(ObjectName,&iterValue);
    
    
    
  vector<UInt_t> RunNumberInTObjArray;
  for(auto iter=RunNumberList.begin();iter!=RunNumberList.end();iter++){
      //cout<<(*iter)->GetRunNumber()<<endl<<flush;
      RunNumberInTObjArray.push_back((*iter)->GetRunNumber());
  }

  for (Int_t i=0; i<tree->GetEntries(); i++) {
    tree->GetSortedEntry(i);
    UInt_t run=iterValue->GetRunNumber();
    if(!IsRunInList(RunNumberInTObjArray,run)) continue;
      if (iterValue->GetTimeStamp() > 8000 && (iterValue->*funky)()) {
          ((TH1F*)Graph)->Fill(iterValue->GetValue(whichValue));
      }
    }

//operator * is overloaded correctly for TH1F not for TH1
if (strcmp(Graph->ClassName(),"TH1F")==0)
{
    //normalize to 72 RPC
    Double_t Normfactor=(Double_t)kNRPC/Graph->GetEntries();
    TH1F *GraphF=(TH1F*)Graph;
    TH1F GraphNorm=Normfactor*(*GraphF);
    Graph=&GraphNorm;
    }

}

void AliRPCAutoIntegrator::CreateDistributionSomething(TH1 *Graph, TString label, TString ObjectName, vector <AliOCDBRun*> RunNumberList){
        if(label.Contains("current")) {
            ((TH1F*)Graph)->GetXaxis()->SetRangeUser(0,50);
            if(label.Contains("total")) CreateDistributionSomething(Graph,&AliRPCValueDCS::IsCurrent,ObjectName, RunNumberList, AliRPCValueCurrent::kITot);
            if(label.Contains("dark")) CreateDistributionSomething(Graph,&AliRPCValueDCS::IsCurrent,ObjectName, RunNumberList, AliRPCValueCurrent::kIDark);
            if(label.Contains("net")) CreateDistributionSomething(Graph,&AliRPCValueDCS::IsCurrent,ObjectName, RunNumberList, AliRPCValueCurrent::kINet);
        }else if(label.Contains("voltage")){
            CreateDistributionSomething(Graph,&AliRPCValueDCS::IsVoltage, ObjectName, RunNumberList, 0);
        }else if(label.Contains("rate")){
            if(label.Contains("not")) CreateDistributionSomething(Graph,&AliRPCValueDCS::IsScaler,ObjectName, RunNumberList, 0);
        }
}

void AliRPCAutoIntegrator::VoltagePlotter(TGraph *Graph, TString ObjectName, UInt_t RunNumber){
    AliRPCAutoIntegrator::PlotSomethingVersusTime(Graph, &AliRPCValueDCS::IsVoltage, ObjectName, RunNumber, 0);
    return;
}

void AliRPCAutoIntegrator::VoltagePlotter(TGraph *Graph, TString ObjectName, std::vector<UInt_t> RunNumberList){
    AliRPCAutoIntegrator::PlotSomethingVersusTime(Graph,&AliRPCValueDCS::IsVoltage,ObjectName,RunNumberList,0);
    return;
}

void AliRPCAutoIntegrator::VoltagePlotter(TGraph *Graph, TString ObjectName){
    AliRPCAutoIntegrator::PlotSomethingVersusTime(Graph,&AliRPCValueDCS::IsVoltage,ObjectName);
    return;
}

void AliRPCAutoIntegrator::GeneratePlotFromFile(TString filename){
    /*
     * format for file is
     * [plot/distribution], [darcurrent, netcurrent, voltage, ratenotbending....], [time, run, ...], [fit, normalized, dark, MTR_INSIDE_MT11_RPC1...]
     *
     * example
     * plot, netcurrent, ratebending, all&normalized&fit
     */
    ifstream file;
    file.open(filename.Data(), ios::in);
    
    if(!file.is_open()) {
        cout << "File " << filename << " not found." << endl;
        return;
    }

    string line;
    TObject *graphBuffer=0x0;
    
    while(getline(file,line)){
        if(line.empty()) continue;
        //convert string to TString
        TString Tline(line);
        cout<<"Reading new line"<<endl;
        //separate string when find ,
        TObjArray *commands;
        commands=Tline.Tokenize(", ");
        TString plotType=((TObjString*)(commands->At(0)))->GetString();
        TString yaxsis=((TObjString*)(commands->At(1)))->GetString();
        TString xaxsis=((TObjString*)(commands->At(2)))->GetString();
        TString options=((TObjString*)(commands->At(3)))->GetString();
        
        cout<<"Plot type:\t"<<plotType.Data()<<endl;
        cout<<yaxsis.Data()<<"\tversus\t"<<xaxsis.Data()<<"\t"<<options.Data()<<endl;
        
        //Get List useful on vs time plots
        options.Prepend("Global_Data_");
        
        Bool_t isDarkGraph=kFALSE;
        Bool_t isNormalizedGraph=kFALSE;
        Bool_t isFittedGraph=kFALSE;
        
        if(options.Contains("dark")) isDarkGraph=kTRUE;
        if(options.Contains("normalized")) isNormalizedGraph=kTRUE;
        if(options.Contains("fit")) isFittedGraph=kTRUE;
        
        //call correct function plot or distribution
        if(plotType.Contains("plot")){
            graphBuffer=new TGraph();
            PlotSomethingVersusSomethingElse((TGraph*)graphBuffer, yaxsis, xaxsis, isDarkGraph, isNormalizedGraph, isFittedGraph, options);
        }else if(plotType.Contains("distribution")){
            graphBuffer=new TH1F();
            vector<AliOCDBRun*> RunList=fAliRPCDataObject->GetRunList(0,0,0);
            CreateDistributionSomething((TH1*)graphBuffer,yaxsis, options, RunList);
        }
        
        fPlotContainer->cd();
        if(graphBuffer&&!yaxsis.Contains("charge")) graphBuffer->Write(Form("%svs%s",yaxsis.Data(),xaxsis.Data()),TObject::kSingleKey|TObject::kOverwrite);
        graphBuffer=0x0;
        commands=0x0;
    }
    
    file.close();
    fPlotContainer->Close();
}

/*
 * print which RPC corresponds to iSide, iPlane, iRPC
 */
void AliRPCAutoIntegrator::PrintWhichRPC(Int_t iRPC, Int_t iSide, Int_t iPlane){
  Int_t NTot=kNRPC*kNPlanes*kNSides;
    printf("RPC: MTR_%s_MT%d_RPC%d %3d out of %3d\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1,kNRPC*kNPlanes*iSide+kNRPC*iPlane+iRPC+1,NTot);
  return;
}

Bool_t AliRPCAutoIntegrator::IsRunInList(std::vector<UInt_t> RunList, UInt_t RunNumber) {
    return std::find(RunList.begin(), RunList.end(), RunNumber) != RunList.end();
}
