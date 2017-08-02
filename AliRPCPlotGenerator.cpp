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

#include "AliRPCPlotGenerator.h"

using namespace std;

ClassImp(AliRPCPlotGenerator);

static const Int_t RPCIndexes[] = {5,6,7,8,9,9,8,7,6,5,4,3,2,1,1,2,3,4};
const Int_t *AliRPCPlotGenerator::kRPCIndexes = RPCIndexes;

static const Int_t RPCSides[] = {0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0};
const Int_t *AliRPCPlotGenerator::kRPCSides = RPCSides;

/*{index from 1 to 234, RPC [1-9], side [0,1]}*/
static const Int_t LBToRPC[][3]={{1,1,0},{2,2,0},{3,2,0},{4,3,0},{5,3,0},{6,4,0},{7,4,0},{8,4,0},{9,6,0},{10,6,0},{11,6,0},{12,7,0},{13,7,0},{14,8,0},{15,8,0},{16,9,0},{17,1,0},{18,2,0},{19,2,0},{20,3,0},{21,3,0},{22,4,0},{23,4,0},{24,4,0},{25,4,0},{26,5,0},{27,5,0},{28,5,0},{29,5,0},{30,6,0},{31,6,0},{32,6,0},{33,6,0},{34,7,0},{35,7,0},{36,8,0},{37,8,0},{38,9,0},{39,1,0},{40,2,0},{41,2,0},{42,3,0},{43,3,0},{44,4,0},{45,4,0},{46,4,0},{47,4,0},{48,5,0},{49,5,0},{50,5,0},{51,5,0},{52,6,0},{53,6,0},{54,6,0},{55,6,0},{56,7,0},{57,7,0},{58,8,0},{59,8,0},{60,9,0},{61,1,0},{62,2,0},{63,2,0},{64,3,0},{65,3,0},{66,4,0},{67,4,0},{68,5,0},{69,5,0},{70,6,0},{71,6,0},{72,7,0},{73,7,0},{74,8,0},{75,8,0},{76,9,0},{77,1,0},{78,2,0},{79,2,0},{80,3,0},{81,3,0},{82,4,0},{83,4,0},{84,5,0},{85,5,0},{86,6,0},{87,6,0},{88,7,0},{89,7,0},{90,8,0},{91,8,0},{92,9,0},{93,1,0},{94,2,0},{95,2,0},{96,3,0},{97,3,0},{98,4,0},{99,4,0},{100,5,0},{101,5,0},{102,6,0},{103,6,0},{104,7,0},{105,7,0},{106,8,0},{107,8,0},{108,9,0},{109,1,0},{110,2,0},{111,3,0},{112,4,0},{113,5,0},{114,6,0},{115,7,0},{116,8,0},{117,9,0},{118,1,1},{119,2,1},{120,2,1},{121,3,1},{122,3,1},{123,4,1},{124,4,1},{125,4,1},{126,6,1},{127,6,1},{128,6,1},{129,7,1},{130,7,1},{131,8,1},{132,8,1},{133,9,1},{134,1,1},{135,2,1},{136,2,1},{137,3,1},{138,3,1},{139,4,1},{140,4,1},{141,4,1},{142,4,1},{143,5,1},{144,5,1},{145,5,1},{146,5,1},{147,6,1},{148,6,1},{149,6,1},{150,6,1},{151,7,1},{152,7,1},{153,8,1},{154,8,1},{155,9,1},{156,1,1},{157,2,1},{158,2,1},{159,3,1},{160,3,1},{161,4,1},{162,4,1},{163,4,1},{164,4,1},{165,5,1},{166,5,1},{167,5,1},{168,5,1},{169,6,1},{170,6,1},{171,6,1},{172,6,1},{173,7,1},{174,7,1},{175,8,1},{176,8,1},{177,9,1},{178,1,1},{179,2,1},{180,2,1},{181,3,1},{182,3,1},{183,4,1},{184,4,1},{185,5,1},{186,5,1},{187,6,1},{188,6,1},{189,7,1},{190,7,1},{191,8,1},{192,8,1},{193,9,1},{194,1,1},{195,2,1},{196,2,1},{197,3,1},{198,3,1},{199,4,1},{200,4,1},{201,5,1},{202,5,1},{203,6,1},{204,6,1},{205,7,1},{206,7,1},{207,8,1},{208,8,1},{209,9,1},{210,1,1},{211,2,1},{212,2,1},{213,3,1},{214,3,1},{215,4,1},{216,4,1},{217,5,1},{218,5,1},{219,6,1},{220,6,1},{221,7,1},{222,7,1},{223,8,1},{224,8,1},{225,9,1},{226,1,1},{227,2,1},{228,3,1},{229,4,1},{230,5,1},{231,6,1},{232,7,1},{233,8,1},{234,9,1}};
const Int_t *AliRPCPlotGenerator::kLBToRPC = LBToRPC[0];


static const TString fSides_[] = {"INSIDE","OUTSIDE"};
const TString *AliRPCPlotGenerator::fSides = fSides_;
static const Int_t fPlanes_[] = {11,12,21,22};
const Int_t *AliRPCPlotGenerator::fPlanes = fPlanes_;
static const TString fCathodes_[] = {"BENDING","NOT BENDING"};
const TString *AliRPCPlotGenerator::fCathodes = fCathodes_;

static const Int_t fColors_[] = {kBlack,kRed,kGreen,kBlue,kViolet+7,kMagenta,kCyan,kGray,kOrange};
const Int_t *AliRPCPlotGenerator::fColors = fColors_;
static const Int_t fStyles_[]={20,24,21,25};
const Int_t *AliRPCPlotGenerator::fStyles = fStyles_;

//call this in constructors
void AliRPCPlotGenerator::InitDataMembers(){
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

AliRPCPlotGenerator::AliRPCPlotGenerator(){
    InitDataMembers();
}


AliRPCPlotGenerator::AliRPCPlotGenerator(TString filename){
    InitDataMembers();
    fPlotContainer=new TFile("AliRPCPlotContainer.root","UPDATE");

    if(checkFileExistance(filename)){
        fGlobalDataContainer=new TFile(filename,"READ");

        
        //check if AliRPCData already exists
        AliRPCData *AliRPCDataBuffer;
        fGlobalDataContainer->cd();
        fGlobalDataContainer->GetObject("AliRPCDataObj", AliRPCDataBuffer);
        
        if (!AliRPCDataBuffer) {
            AliRPCAutoIntegrator *integrator=new AliRPCAutoIntegrator();
            integrator->RunAutoIntegrator();
        } else {
            fAliRPCDataObject = AliRPCDataBuffer;
            cout << "Reading old AliRPCData with " << fAliRPCDataObject->GetTotalEntries() << " entries"<< endl << flush;
        }
        
    }else{
        cout<<"File not found"<<endl;
        fAliRPCDataObject=new AliRPCData();
        fGlobalDataContainer=new TFile();
    }
    
}


/*Method to generate default useful plots.
 it generates iTot, iDark, Voltage plots vs time
 Note that plots are stored in GlobalDataContainer
 In order to reduce size of GlobalDataContainer, use of GeneratePlotFromFile is suggested
 */
void AliRPCPlotGenerator::GeneratePlots() {
    TGraph *PlotsITot[kNSides][kNPlanes][kNRPC];
    TGraph *PlotsIDark[kNSides][kNPlanes][kNRPC];
    TGraph *PlotsVoltage[kNSides][kNPlanes][kNRPC];
    TString ObjectName;
    
    //Chech if directory are already there
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
                
                //Plot iTot, getting data from TSmartTree named ObjectName
                PlotsITot[iSide][iPlane][iRPC] = new TGraph();
                PlotsITot[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsITot[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsITot[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsITot[iSide][iPlane][iRPC]->SetMarkerSize(0.15);
                
                PlotSomethingVersusTime(PlotsITot[iSide][iPlane][iRPC],&AliRPCValueDCS::IsOkForITot, ObjectName, AliRPCValueCurrent::kITot);
                
                fGlobalDataContainer->cd("iTot_Graphs");
                PlotsITot[iSide][iPlane][iRPC]->Write(Form("iTot_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kSingleKey|TObject::kOverwrite);
                
                //Plot iDark, getting data from TSmartTree named ObjectName
                PlotsIDark[iSide][iPlane][iRPC]=new TGraph();
                PlotsIDark[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsIDark[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsIDark[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsIDark[iSide][iPlane][iRPC]->SetMarkerSize(0.15);
                
                PlotSomethingVersusTime(PlotsIDark[iSide][iPlane][iRPC],&AliRPCValueDCS::IsOkForIDark, ObjectName, AliRPCValueCurrent::kITot);
                
                fGlobalDataContainer->cd("iDark_Graphs");
                PlotsIDark[iSide][iPlane][iRPC]->Fit("pol1","Q");
                PlotsIDark[iSide][iPlane][iRPC]->Write(Form("iDark_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kSingleKey|TObject::kOverwrite);
                
                //Plot Voltage, getting data from TSmartTree named ObjectName
                PlotsVoltage[iSide][iPlane][iRPC]=new TGraph();
                PlotsVoltage[iSide][iPlane][iRPC]->SetLineColor(fColors[iRPC]);
                PlotsVoltage[iSide][iPlane][iRPC]->SetMarkerColor(fColors[iRPC]);
                PlotsVoltage[iSide][iPlane][iRPC]->SetMarkerStyle(fStyles[iPlane]);
                PlotsVoltage[iSide][iPlane][iRPC]->SetMarkerSize(0.15);
                
                PlotSomethingVersusTime(PlotsVoltage[iSide][iPlane][iRPC],&AliRPCValueDCS::IsVoltage,ObjectName);
                
                fGlobalDataContainer->cd("Voltage_Graphs");
                PlotsVoltage[iSide][iPlane][iRPC]->Write(Form("Voltage_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC+1),TObject::kSingleKey|TObject::kOverwrite);
                
                PrintWhichRPC(iRPC+1, iSide, iPlane);
                
            }
        }
    }
    fGlobalDataContainer->Flush();
}


/*
 This method integrates iNet from AliRPCRunStatistics in order to have
 one integratdecharge plot per RPC;
 each plot has one point per run.
 Plots are stored in plot container
 */

void AliRPCPlotGenerator::IntegratorPerRun(Bool_t showFeeric){
    cout<<"\nGenerating integrated charge graphs"<<endl;
    TGraph *PlotsIntegratedCharge[kNSides][kNPlanes][kNRPC];
    TString dirName("integrated_charge_Graphs");
    
    //check if graphs are already there
    TObject *check=0x0;
    fPlotContainer->GetObject(dirName,check);
    if(!check){
        fPlotContainer->mkdir(dirName);
    }
    
    //arrays contains {iSide, IPlane, IRPC, integratedCharge}
    RPC LessExposedRPC;
    RPC MostExposedRPC;
    Double_t maxcharge=0.;
    //note that every Double_t is minor than max(), so it is sure that at least one RPC has minCharge
    Double_t minCharge=std::numeric_limits<Double_t>::max();
    
    //Plot the most exposed and the less exposed RPC
    //For each plane it is generated a multigraph with most exposed, less exposed and mean
    //Most and less exposed TGraph are pointer to graph already generated
    //MeanGraph is generated apart
    
    TMultiGraph *MostAndLessExposedRPCMultiGraph[kNPlanes];
    TGraph *MeanGraph[kNPlanes];
    TGraph *MostGraph[kNPlanes], *LessGraph[kNPlanes];
    Int_t counter=0;
    Int_t meanCounter=0;
    Double_t IntegratedCharge=0.;
    Double_t MeanIntegratedCharge=0.;
    
    // Data are stored in AliRPCData as vector of AliRPCRunStatistics*
    // note that is stored one vector per RPC
    vector<AliRPCRunStatistics*> list;
    
    for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
        MostAndLessExposedRPCMultiGraph[iPlane]=new TMultiGraph();
        MeanGraph[iPlane]=new TGraph();
        
        //reset values to calculate max and min per each plane
        minCharge=std::numeric_limits<Double_t>::max();
        maxcharge=0.;
        
        for(Int_t iSide=0;iSide<kNSides;iSide++){
            for(Int_t iRPC=1;iRPC<=kNRPC;iRPC++){
                
                //user can ask to skip RPC equipped with FEERIC
                if(IsFEERIC(iRPC, iSide, iPlane)&&!showFeeric) continue;
                
                counter=0;
                IntegratedCharge=0.;
                
                //get and sort list of run for this RPC
                list=((AliRPCData*)fAliRPCDataObject)->GetRunStatistics(iPlane,iSide,iRPC);
                std::sort(list.begin(),list.end(),AliRPCRunStatistics::SortRunStatistics);
                
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]=new TGraph();
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetFillColor(0);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetLineColor(fColors[iRPC-1]);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetMarkerColor(fColors[iRPC-1]);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetMarkerStyle(fStyles[iPlane]);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetMarkerSize(0.15);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->GetXaxis()->SetTitle("timestamp [s]");
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->GetYaxis()->SetTitle("integrated charge [#muC/cm^{2}]");
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->GetYaxis()->SetTitleOffset(1);
                
                Double_t timestamp0=0.;
                Double_t timestamp1=timestamp0;
                Double_t iNet0=0.;
                Double_t iNet1=iNet0;
                
                //iter over runs and integrate
                //integral is calculated between two runs as (iNet first+iNet second)*deltaTime/2
                for(std::vector<AliRPCRunStatistics*>::iterator iter=list.begin();iter!=list.end();iter++){
                    Double_t IntChargeBuffer=(iNet0+iNet1)*(timestamp1-timestamp0)/2;
                    
                    //check that integaral is effective positive
                    if(!(IntChargeBuffer>0)) IntChargeBuffer=0.;
                    
                    //if timestamps differe for more than 3 days (e.g. shutdown)
                    //integrated charge should not be incremented
                    if((timestamp1-timestamp0)>3*24*60*60) {
                        timestamp0=timestamp1;
                        iNet0=iNet1;
                        continue;
                    }
                    
                    //sum integrated charge for new run
                    IntegratedCharge+=IntChargeBuffer;
                    
                    //x value is set as the mean timestampStart of the two runs
                    PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetPoint(counter++,(timestamp0+timestamp1)/2,IntegratedCharge);
                    
                    //reset values
                    timestamp0=timestamp1;
                    iNet0=iNet1;
                    timestamp1=(*iter)->GetTimeStampStart();
                    iNet1=(*iter)->GetMeanNetCurrent()/fRPCAreas[iPlane][iRPC-1];
                }
                
                //estimate most and less exposed RPC
                if(IntegratedCharge<minCharge){
                    LessExposedRPC.Plane=iPlane;
                    LessExposedRPC.Side=iSide;
                    LessExposedRPC.RPC=iRPC;
                    minCharge=IntegratedCharge;
                    LessGraph[iPlane]=PlotsIntegratedCharge[iSide][iPlane][iRPC-1];
                }else if(IntegratedCharge>=maxcharge){
                    MostExposedRPC.Plane=iPlane;
                    MostExposedRPC.Side=iSide;
                    MostExposedRPC.RPC=iRPC;
                    maxcharge=IntegratedCharge;
                    MostGraph[iPlane]=PlotsIntegratedCharge[iSide][iPlane][iRPC-1];
                }
                
                printf("RPC: MTR_%s_MT%d_RPC%d\t charge:%f \n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC,IntegratedCharge);
                fPlotContainer->cd(dirName);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->Write(Form("integrated_charge_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC),TObject::kOverwrite||TObject::kSingleKey);
                
            }
        }
        
        MeanIntegratedCharge=0.;
        
        //populate meangraph plot.
        //mean integratedcharge is the sum of mean integratedcharge got from AliRPCRunStatistics
        //note that the run list is retrieved from the last RPC in previous cycle,
        //it's possible that not all RPCs have data for the same runs
        for(std::vector<AliRPCRunStatistics*>::iterator iter=list.begin();iter!=list.end()-1;iter++){
            Double_t value=((AliRPCData*)(fAliRPCDataObject))->GetMeanIntegratedCharge((*iter)->GetRunNumber(),kTRUE);
            
            //skip if run was too long
            if((*iter)->GetElapsedTime()>3*24*60*60) continue;
            MeanIntegratedCharge+=value;
            MeanGraph[iPlane]->SetPoint(meanCounter++,(*iter)->GetTimeStampStart(),MeanIntegratedCharge);
        }
        
        TString LessString(Form("Less exposed RPC: MTR_%s_MT%d_RPC%d\t charge:%f \n",(fSides[LessExposedRPC.Side]).Data(),fPlanes[LessExposedRPC.Plane],LessExposedRPC.RPC,minCharge));
        TString MostString(Form("Most exposed RPC: MTR_%s_MT%d_RPC%d\t charge:%f \n",(fSides[MostExposedRPC.Side]).Data(),fPlanes[MostExposedRPC.Plane],MostExposedRPC.RPC,maxcharge));
        
        
        //print resume for this plane
        printf("\n\n###########\n");
        printf("Plane: MT_%d\n",fPlanes[iPlane]);
        printf("%s",MostString.Data());
        printf("%s",LessString.Data());
        printf("Mean:%f \n",MeanIntegratedCharge);
        
        MeanGraph[iPlane]->SetLineColor(kCyan-3);
        MeanGraph[iPlane]->SetMarkerSize(0.15);
        MeanGraph[iPlane]->SetMarkerColor(kCyan-3);
        MeanGraph[iPlane]->SetMarkerStyle(24);
        MostAndLessExposedRPCMultiGraph[iPlane]->Add(MeanGraph[iPlane]);
        MostAndLessExposedRPCMultiGraph[iPlane]->Add(MostGraph[iPlane]);
        MostAndLessExposedRPCMultiGraph[iPlane]->Add(LessGraph[iPlane]);
        
        //        MostAndLessExposedRPCMultiGraph[iPlane]->Draw("AP");
        //        MostAndLessExposedRPCMultiGraph[iPlane]->GetXaxis()->SetTitle("timestamp [s]");
        //        MostAndLessExposedRPCMultiGraph[iPlane]->GetXaxis()->SetRangeUser(1460000000, 1480000000);
        //        MostAndLessExposedRPCMultiGraph[iPlane]->GetYaxis()->SetTitle("integrated charge [#muC/cm^{2}]");
        //        MostAndLessExposedRPCMultiGraph[iPlane]->GetYaxis()->SetTitleOffset(1.5);
        
        
        fPlotContainer->cd(dirName);
        MostAndLessExposedRPCMultiGraph[iPlane]->Write(Form("MT_%d_MultiGraph",fPlanes[iPlane]),TObject::kOverwrite||TObject::kSingleKey);
        fPlotContainer->Flush();
    }
    
}


//this method returns a pointer to a TGraph of integrated charge for a specific RPC
TGraph* AliRPCPlotGenerator::GetIntegratedChargePlot(Int_t iRPC, Int_t iSide, Int_t iPlane){
    TString sourceDir("integrated_charge_Graphs");
    
    //check if plot are already stored
    TObject *check=0x0;
    fPlotContainer->GetObject(sourceDir,check);
    
    //if plot are not there they have to be generated
    if(!check){
        IntegratorPerRun();
    }
    
    TString source(Form("integrated_charge_Graphs/integrated_charge_Graph_MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC));
    
    TGraph *graphBuffer=0x0;
    
    fPlotContainer->GetObject(source,graphBuffer);
    
    //check that object was correctly retrieved
    if(!graphBuffer){
        return nullptr;
    }
    
    return graphBuffer;
}

// This method create a canvas with plot of integratedcharge
// shown as they are physically mounted in planes.
// canvas is divided in 18 (kNSides*kNRPC) pads one per RPC position
// in each pads there are 4 (kNPlanes) TGraph, one per plane
void AliRPCPlotGenerator::PlotRPCPerMT(Bool_t showFeeric){
    
    //it is useful to show all graph with same scale
    //{min X, max X}{min Y, max Y}
    //X=timestamp, Y=integratedcharge
    Long64_t limits[][2]={{1460000000,1470000000},{0,10000}};
    
    TMultiGraph *PlotsIntegratedCharge[kNSides][kNRPC];
    TGraph *graphBuffer=0x0;
    
    TString outputDirName("integrated_charge_SamePosition");
    
    
    //Check if graphs are already there
    TObject *check=0x0;
    fPlotContainer->GetObject(outputDirName,check);
    
    if(!check){
        fPlotContainer->mkdir(outputDirName);
    }
    
    for(Int_t iSide=0;iSide<kNSides;iSide++){
        for(Int_t iRPC=1;iRPC<=kNRPC;iRPC++){
            PlotsIntegratedCharge[iSide][iRPC-1]=new TMultiGraph();
            //PlotsIntegratedCharge[iSide][iRPC-1]->SetTitle(Form("MTR_%s_RPC%d",(fSides[iSide]).Data(),iRPC-1));
            
            for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
                //user can ask to skip RPC with FEERIC electronics
                if(IsFEERIC(iRPC, iSide, iPlane)&&!showFeeric) continue;
                
                graphBuffer=GetIntegratedChargePlot(iRPC,iSide,iPlane);
                
                //skip if GetIntegratedChargePlot returns nullptr
                if(!graphBuffer){
                    continue;
                }
                
                
                graphBuffer->SetFillColor(0);
                graphBuffer->SetMarkerStyle(fStyles[iPlane]);
                graphBuffer->SetMarkerColor(fColors[iPlane]);
                graphBuffer->SetMarkerSize(0.3);
                graphBuffer->SetTitle(Form("MTR_%s_MT%d_RPC%d",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC));
                PlotsIntegratedCharge[iSide][iRPC-1]->Add(graphBuffer);
            }
            
            fPlotContainer->cd(outputDirName);
            PlotsIntegratedCharge[iSide][iRPC-1]->Write(Form("MTR_%s_RPC%d",(fSides[iSide]).Data(),iRPC));
        }
    }
    
    
    TCanvas *planeCanvas=new TCanvas("planeCanvas","planeCanvas",0,0,1280,960);
    
    //divide canvas in pads
    //Divide(columns, rows, margins)
    //note that one row is for labels that's why second argoument has +1
    planeCanvas->Divide(kNSides,kNRPC+1,0,0);
    
    //put labels for the sides
    planeCanvas->cd(1);
    auto *outsideLabel = new TText(0.5,0.5,"OUTSIDE");
    outsideLabel->SetTextAlign(22);
    outsideLabel->SetTextSize(0.9);
    outsideLabel->Draw();
    
    planeCanvas->cd(2);
    auto *insideLabel = new TText(0.5,0.5,"INSIDE");
    insideLabel->SetTextAlign(22);
    insideLabel->SetTextSize(0.9);
    insideLabel->Draw();
    
    //RPC are numbered from down to top so indexs start from KNRPC and decrease to 1
    Int_t iRPCInside=kNRPC;
    Int_t iRPCOutside=kNRPC;
    
    //iter over remaning pads to put there the correct TMultiGraph
    for(Int_t iPad=3;iPad<=kNRPC*kNSides+2;iPad++){
        
        //even pads are INSIDE
        if(iPad%2==0){
            planeCanvas->cd(iPad);
            
            //Note that TMultiGraph must be Drawn before call Get*axis()
            //line color differs for each position
            //point color and style differ for each plane
            PlotsIntegratedCharge[0][iRPCInside-1]->Draw("AL");
            //Note that limits on X and Y axses are set differently ( https://root.cern.ch/doc/master/classTMultiGraph.html )
            //Ndivisions is set to have graph grids aligned
            PlotsIntegratedCharge[0][iRPCInside-1]->GetXaxis()->SetLimits(limits[0][0],limits[0][1]);
            PlotsIntegratedCharge[0][iRPCInside-1]->SetMinimum(limits[1][0]);
            PlotsIntegratedCharge[0][iRPCInside-1]->GetYaxis()->SetLabelSize(0.13);
            PlotsIntegratedCharge[0][iRPCInside-1]->GetYaxis()->SetNdivisions(5);
            PlotsIntegratedCharge[0][iRPCInside-1]->SetMaximum(limits[1][1]);
            
            //index should be decremented fo this side
            iRPCInside--;
        }else{
            //odd pads are OUTSIDE
            planeCanvas->cd(iPad);
            PlotsIntegratedCharge[1][iRPCOutside-1]->Draw("AL");
            PlotsIntegratedCharge[1][iRPCOutside-1]->GetXaxis()->SetLimits(limits[0][0],limits[0][1]);
            PlotsIntegratedCharge[1][iRPCOutside-1]->SetMinimum(limits[1][0]);
            PlotsIntegratedCharge[1][iRPCOutside-1]->GetYaxis()->SetLabelSize(0.13);
            PlotsIntegratedCharge[1][iRPCOutside-1]->GetYaxis()->SetNdivisions(5);
            PlotsIntegratedCharge[1][iRPCOutside-1]->SetMaximum(limits[1][1]);
            //index should be decremented fo this side
            iRPCOutside--;
        }
    }
    
    
    fPlotContainer->cd(outputDirName);
    planeCanvas->Write("planeCanvas",TObject::kOverwrite|TObject::kSingleKey);
    //planeCanvas->Draw();
}

//This method fills *Graph with one point per reading
//data are retrieved from trees in GlobalDataContiner
//only data whose run is in runNumerList are considered
//ObjectName should be in format MTR_fSides[].Data()_MT_fPlanes[].Data()_RPC# (e.g. MTR_INSIDE_MT11_RPC1)
//at this point *Graph must be instantiated
void AliRPCPlotGenerator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, std::vector<UInt_t> RunNumberList, Int_t whichValue) {
    Int_t counter=0;
    
    TSmartTree *tree;
    TBranch *branch;
    AliRPCValueDCS *iterValue=new AliRPCValueDCS();
    
    fGlobalDataContainer->GetObject(ObjectName,tree);
    
    if(!tree) return;
    
    branch = tree->GetBranch(ObjectName);
    tree->SetBranchAddress(ObjectName,&iterValue);
    
    //iter over tree so that iterValue assume all values
    for(Int_t i=0; i<tree->GetEntries(); i++) {
        tree->GetSortedEntry(i);
        if(IsRunInList(RunNumberList,iterValue->GetRunNumber())){
            if (((iterValue->*funky)()) &&
                (iterValue->GetTimeStamp() > 8000)) {
                Graph->SetPoint(counter++, iterValue->GetTimeStamp(),iterValue->GetValue(whichValue));
            }
        }
    }
    return;
}


//This method fills *Graph with one point per reading for a specific RPC
//data are retrieved from trees in GlobalDataContiner
//only data with specific RunNumber are considered
//ObjectName specify which RPC and should be in format MTR_fSides[].Data()_MT_fPlanes[].Data()_RPC# (e.g. MTR_INSIDE_MT11_RPC1)
//you can generate that string with:
// Form("MTR_%s_MT%d_RPC%d", (fSides[side]).Data(), fPlanes[plane], RPC)
//at this point *Graph must be instantiated
void AliRPCPlotGenerator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, UInt_t RunNumber, Int_t whichValue){
    std::vector<AliOCDBRun*> RunDummyList;
    RunDummyList.push_back(new AliOCDBRun(RunNumber,0));
    PlotSomethingVersusTime(Graph, funky, ObjectName, RunDummyList, whichValue);
    return;
}


//This method fills *Graph with one point per reading for a specific RPC
//data are retrieved from trees in GlobalDataContiner
//only data whose run is in runNumerList are considered
//ObjectName specify which RPC and should be in format MTR_fSides[].Data()_MT_fPlanes[].Data()_RPC# (e.g. MTR_INSIDE_MT11_RPC1)
//you can generate that string with:
// Form("MTR_%s_MT%d_RPC%d", (fSides[side]).Data(), fPlanes[plane], RPC)
//at this point *Graph must be instantiated
void AliRPCPlotGenerator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName,std::vector<AliOCDBRun*> RunNumberList, Int_t whichValue){
    std::vector<UInt_t> RunDummyList;
    for(AliOCDBRun* iter :RunNumberList){
        UInt_t temp=((UInt_t)iter->GetRunNumber());
        RunDummyList.push_back(temp);
    }
    PlotSomethingVersusTime(Graph, funky, ObjectName, RunDummyList, whichValue);
    return;
}

//This method fills *Graph with one point per reading for a specific RPC
//data are retrieved from trees in GlobalDataContiner
//ObjectName specify which RPC and should be in format MTR_fSides[].Data()_MT_fPlanes[].Data()_RPC# (e.g. MTR_INSIDE_MT11_RPC1)
//you can generate that string with:
// Form("MTR_%s_MT%d_RPC%d", (fSides[side]).Data(), fPlanes[plane], RPC)
//at this point *Graph must be instantiated
void AliRPCPlotGenerator::PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, Int_t whichValue){
    Int_t counter=0;
    
    TSmartTree *tree;
    TBranch *branch;
    AliRPCValueDCS *iterValue=new AliRPCValueDCS();
    
    fGlobalDataContainer->GetObject(ObjectName,tree);
    
    if(!tree) return;
    
    branch = tree->GetBranch(ObjectName);
    tree->SetBranchAddress(ObjectName,&iterValue);
    
    //iter over the tree so tha iterValue will assume every value stored and added to the graph
    for (Int_t i=0; i<tree->GetEntries(); i++) {
        tree->GetSortedEntry(i);
        if (((iterValue->*funky)()) &&
            (iterValue->GetTimeStamp() > 8000)) {
            Graph->SetPoint(counter++, iterValue->GetTimeStamp(),iterValue->GetValue(whichValue));
        }
    }
    return;
}

//This method creates 72 (kNPlanes*kNSides*kNRPC) graphs, one per RPC using
//data retrieved from AliRPCData for a specific observable
//each graph has one point per run
//on X axis is shown the timestamp
//observableName should contain current/voltage/rate and eventually dark/net/total bending/not bending
void AliRPCPlotGenerator::SomethingPerRun(TString observableName, Bool_t isDarkGraph, Bool_t isNormalizedGraph, Bool_t showFeeric){
    cout<<"\nGenerating "<<observableName<<" plots"<<endl;
    
    //set a pointer Yptr to the correct function of AliRPCRunStatistics to get the correct observable
    Double_t (AliRPCRunStatistics::*Yptr)() const;
    if(observableName.Contains("current")) {
        if(observableName.Contains("total")) Yptr =&AliRPCRunStatistics::GetMeanTotalCurrent;
        if(observableName.Contains("net")) Yptr =&AliRPCRunStatistics::GetMeanNetCurrent;
        if(observableName.Contains("dark")) Yptr =&AliRPCRunStatistics::GetMeanDarkCurrent;
    }else if(observableName.Contains("voltage")){
        Yptr =&AliRPCRunStatistics::GetMeanHV;
    }else if(observableName.Contains("rate")&&observableName.Contains("bending")){
        if(observableName.Contains("not")) Yptr =&AliRPCRunStatistics::GetMeanRateNotBending;
        else Yptr =&AliRPCRunStatistics::GetMeanRateBending;
    }
    
    
    //check and eventually create a folder for the plot in plotcontainer
    TString dirName(observableName);
    
    TObject *check=0x0;
    fPlotContainer->GetObject(dirName,check);
    if(!check){
        fPlotContainer->mkdir(dirName);
    }
    
    TGraph *PlotsIntegratedCharge[kNSides][kNPlanes][kNRPC];
    //planeGraphs contains all graph of the same plane
    TMultiGraph *planeGraphs[4];
    Int_t counter=0;
    vector<AliRPCRunStatistics*> list;
    
    for(Int_t iPlane=0;iPlane<kNPlanes;iPlane++){
        
        planeGraphs[iPlane]=new TMultiGraph();
        planeGraphs[iPlane]->SetTitle(Form("MT%d",fPlanes[iPlane]));
        
        for(Int_t iSide=0;iSide<kNSides;iSide++){
            for(Int_t iRPC=1;iRPC<=kNRPC;iRPC++){
                if(IsFEERIC(iRPC, iSide, iPlane)&&!showFeeric) continue;
                
                counter=0;
                //get and sort list of run for this RPC
                list=((AliRPCData*)fAliRPCDataObject)->GetRunStatistics(iPlane,iSide,iRPC);
                std::sort(list.begin(),list.end(),AliRPCRunStatistics::SortRunStatistics);
                
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]=new TGraph();
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetFillColor(0);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetLineColor(fColors[iRPC-1]);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetMarkerColor(fColors[iRPC-1]);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetMarkerStyle(fStyles[iPlane]);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetMarkerSize(0.15);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->GetXaxis()->SetTitle("timestamp [s]");
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->GetYaxis()->SetTitle(observableName);
                
                //iter over data and add point to graph
                for(auto iter:list){
                    if(isDarkGraph&&!iter->GetIsCalib()) continue;
                    //note that data are eventually normalized
                    PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->SetPoint(counter++, iter->GetTimeStampStart(), (iter->*Yptr)()/(isNormalizedGraph?fRPCAreas[iRPC][iPlane]:1.));
                }
                
                planeGraphs[iPlane]->Add(PlotsIntegratedCharge[iSide][iPlane][iRPC-1]);
                
                
                fPlotContainer->cd(dirName);
                PlotsIntegratedCharge[iSide][iPlane][iRPC-1]->Write(Form("%s_plot_MTR_%s_MT_%d_RPC%d",observableName.Data(),fSides[iSide].Data(),fPlanes[iPlane],iRPC),TObject::kSingleKey|TObject::kOverwrite);
                
            }
        }
        planeGraphs[iPlane]->Write(Form("MT%d",fPlanes[iPlane]),TObject::kSingleKey|TObject::kOverwrite);
    }
    
    fPlotContainer->Flush();
}

//This method fills Graph using
//data retrieved from AliRPCData for a specific observable
//each graph has one point per run
//each point is the mean over RPC for that specific run
//on X axis is shown the timestamp
//at this point *Graph must be instantiated
//funky is a function ( GetMean* ) of AliRPCData
void AliRPCPlotGenerator::PlotSomethingVersusRun(TGraph *Graph, Double_t (AliRPCData::*funky)(UInt_t, Bool_t) const, Bool_t normalizedToArea, Bool_t onlyDarkPoints){
    Int_t counter=0;
    
    Graph->SetLineColor(0);
    Graph->SetMarkerSize(1.5);
    Graph->SetMarkerStyle(24);
    
    //we need a list of all run available for this analysis
    //we must be sure to cath all possible runs
    vector<UInt_t> OCDBRunListComplete;
    
    for(Int_t iSide=0;iSide<kNSides;iSide++) {
        for (Int_t iPlane = 0; iPlane < kNPlanes; iPlane++) {
            for (Int_t iRPC = 1; iRPC <= kNRPC; iRPC++) {
                vector<AliRPCRunStatistics*> bufferList=fAliRPCDataObject->GetRunStatistics(iPlane,iSide,iRPC);
                
                for(auto iter=bufferList.begin(); iter!=bufferList.end(); iter++){
                    auto run=(*iter)->GetRunNumber();
                    
                    cout<<run<<endl;
                    //if this runs are not skipped the program bump into seg violation for obscure reasons
                    if(run==267165) continue;
                    if(run ==262492) continue;
                    
                    //skip if onlyDrak is true and iter is not dark
                    if(onlyDarkPoints&&!(*iter)->GetIsCalib()) continue;
                    
                    //if that run is not already there is added to the list
                    if(!IsRunInList(OCDBRunListComplete, run)) OCDBRunListComplete.push_back(run);
                }
            }
        }
    }
    
    //iter over the list and add point to graph
    for(UInt_t iter:OCDBRunListComplete){
        Double_t time=(fAliRPCDataObject->GetMeanTimeStampStart(iter));
        Double_t y=(fAliRPCDataObject->*funky)(iter, normalizedToArea);
        if(time>8000&&y>0) Graph->SetPoint(counter++,time,y);
    }
}

//This method fills Graph using
//data retrieved from AliRPCData for two specific observables
//each graph has one point per RPC
//each point is the average over time for that specific RPC
//funky is a function ( GetAverage* ) of AliRPCData
//at this point *Graph must be instantiated
//the plot obtained from this plot is a "correlation plot" between funkyX and funkyY
void AliRPCPlotGenerator::PlotSomethingVersusRPC(TGraph *Graph, Double_t (AliRPCData::*funkyX)(Int_t, Int_t, Int_t, Bool_t)const, Double_t (AliRPCData::*funkyY)(Int_t, Int_t, Int_t, Bool_t)const, Bool_t normalizedToArea){
    Int_t counter=0;
    
    Graph->SetLineColor(0);
    Graph->SetMarkerSize(1.5);
    Graph->SetMarkerStyle(24);
    
    for(Int_t iSide=0;iSide<kNSides;iSide++) {
        for (Int_t iPlane = 0; iPlane < kNPlanes; iPlane++) {
            for (Int_t iRPC = 0; iRPC < kNRPC; iRPC++) {
                Double_t x=(fAliRPCDataObject->*funkyX)(iPlane,iSide,iRPC,normalizedToArea);
                Double_t y=(fAliRPCDataObject->*funkyY)(iPlane,iSide,iRPC,normalizedToArea);
                //                printf("I read x: %f, y: %f\n",x,y);
                
                //all observables make sense for positive values, so negatives are skipped
                if(x>0&&y>0) Graph->SetPoint(counter++,x,y);
            }
        }
    }
}

//This method fills Graph using
//data retrieved from AliRPCData for one specific observables
//each graph has one point per RPC
//point is created so that is abscissa is the total integrated charge
//and the ordinate is the deltaY/Y,
//delta Y is the difference between the mean of firsts 10 (NData) data and lasts 10 data
//Y is the mean of firsts
//at this point *Graph must be instantiated
//funky is a function ( GetMean* ) of AliRPCRunStatistics
void AliRPCPlotGenerator::PlotVariationVsIntegratedCharge(TGraph *Graph, Double_t (AliRPCRunStatistics::*funky)()const){
    TGraph *graphBuffer;
    Int_t counter=0;
    
    Graph->SetLineColor(0);
    Graph->SetMarkerSize(1.5);
    Graph->SetMarkerStyle(24);
    
    for(Int_t iSide=0;iSide<kNSides;iSide++) {
        for (Int_t iPlane = 0; iPlane < kNPlanes; iPlane++) {
            for (Int_t iRPC = 1; iRPC <= kNRPC; iRPC++) {
                
                //get the list of data and the integrtedcharge plot
                //from the plot I get the last integratedcharge value elaborated before
                
                std::vector<AliRPCRunStatistics*> list=fAliRPCDataObject->GetRunStatistics(iPlane, iSide, iRPC);
                graphBuffer=GetIntegratedChargePlot(iRPC, iSide, iPlane);
                
                //skip if GetIntegratedChargePlot returns nullptr
                if(!graphBuffer) continue;
                
                //calculate mean for first NData and lasts NData
                //x(First/Lasts)Cumulus
                Double_t xFCumulus=0.;
                Double_t xLCumulus=0.;
                Int_t NData=10;
                
                //iter over lists to get correct data
                for(auto iter=list.begin();iter!=list.end();iter++){
                    
                    Double_t xValue=((*iter)->*funky)();
                    if(xValue!=0&&iter<list.begin()+NData) {
                        //sum only if values is in first NData positions
                        xFCumulus+=xValue;
                    }
                    
                    if(xValue!=0&&iter>=list.end()-NData) {
                        //sum only if values is in lastst NData positions
                        xLCumulus+=xValue;
                    }
                    
                }
                
                //calculate means
                //note that surely NData!=0
                Double_t firstX=xFCumulus/NData;
                Double_t lastX=xLCumulus/NData;
                
                //calculate variation
                Double_t x=TMath::Abs(firstX-lastX)/firstX;
                
                //get integrated charge
                Double_t y, dummyX;
                graphBuffer->GetPoint(graphBuffer->GetN()-1, dummyX, y);
                
                //all observables make sense for positive values, so negatives are skipped
                if(x>0&&y>0) Graph->SetPoint(counter++,y,x);
                
                PrintWhichRPC(iRPC, iSide, iPlane);
            }
        }
    }
    
    Graph->GetXaxis()->SetTitle("integrated charge [#mu C/cm^{2}]");
    Graph->GetYaxis()->SetTitle("#frac{#Delta y}{y} [%]");
}

//the idea is that this method calls the correct PlotVariationVs* function
//Graph is propagated to the next method
//at this point *Graph must be instantiated
//x and y should contain current/voltage/rate total/dark/net bending/notbending
//NOTE THAT AT THIS POINT IT ONLY WORKS IF y IS integratedcharge
//a method PlotVariationVsSomething should be implemented!!
//method PlotVariationVsIntegratedCharge was implemented because integratedcharge is got from graph and not from data
void AliRPCPlotGenerator::PlotVariationSomething(TGraph *Graph, TString x, TString y){
    Double_t (AliRPCRunStatistics::*fPtr)()const;
    
    if(y.Contains("current")) {
        if(y.Contains("total")) fPtr=&AliRPCRunStatistics::GetMeanTotalCurrent;
        if(y.Contains("dark")) fPtr=&AliRPCRunStatistics::GetMeanDarkCurrent;
        if(y.Contains("net")) fPtr=&AliRPCRunStatistics::GetMeanNetCurrent;
    }else if(y.Contains("voltage")){
        fPtr=&AliRPCRunStatistics::GetMeanHV;
    }else if(y.Contains("rate")&&y.Contains("bending")){
        if(y.Contains("not")){
            fPtr=&AliRPCRunStatistics::GetMeanRateNotBending;
        } else {
            fPtr=&AliRPCRunStatistics::GetMeanRateBending;
        }
    }else return;
    
    //TO DO
    //PlotVariationVsSomething(Graph,fPtr, Xptr)
    
    if(x.Contains("charge")||x.Contains("integrated")){
        PlotVariationVsIntegratedCharge(Graph, fPtr);
    }else return;
}

//the idea is that this method calls the correct Plot*Vs* method based on x and y strings
//this method is the key to let GeneratePlotFromFile() working
//Graph is propagated to the next method
//*Graph must be istantiated at this point
//x and y should contain current/voltage/rate total/dark/net bending/notbending
//if TF1::fitFunc is not null Graph is fitted
//ObjectName must be passed for PlotSomethingVsTime(), format is specified there
void AliRPCPlotGenerator::PlotSomethingVersusSomethingElse(TGraph *Graph, const TString y, const TString x,  Bool_t onlyDarkPoints, Bool_t normalizedToArea, TF1 *fitFunc, TString ObjectName){
    
    //in case the user asks for PlotSomethingVsTime, tree must be there
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
        
        //calls for PlotSomethingVsRun()
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
        
        //call for a correlation plot with PlotSomethingVsRPC()
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
        }else if(y.Contains("integrated")&&y.Contains("charge")){
            if(x.Contains("plane")){
                Graph=0x0;
                IntegratorPerRun();
                PlotRPCPerMT();
            }
        }
        else return;
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
    
    //if Graph was created somewhere axis are set
    if(Graph){
        Graph->GetXaxis()->SetTitle(x);
        Graph->GetYaxis()->SetTitle(y);
        
        //if user asks for a fit fit is done
        if(fitFunc) {
            cout<<"Fit of "<<y.Data()<<" vs "<<x.Data()<<":";
            Graph->Fit(fitFunc,"M+");
            //cout<<"\n\n##########\n\n";
        }
        
    }
};

//this method fills Graph retrieving data
//from trees in GlobalDataContainer
//Graph is filled with every entrys whose runnumber is in RunList for a specifc RPC
//ObjectName specify which RPC and should be in format MTR_fSides[].Data()_MT_fPlanes[].Data()_RPC# (e.g. MTR_INSIDE_MT11_RPC1)
//you can generate that string with:
// Form("MTR_%s_MT%d_RPC%d", (fSides[side]).Data(), fPlanes[plane], RPC)
//at this point *Graph must be instantiated
void AliRPCPlotGenerator::CreateDistributionSomething(TH1 *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, vector<AliOCDBRun*> RunNumberList, Int_t whichValue){
    
    //check if tree is there
    TSmartTree *tree;
    TBranch *branch;
    AliRPCValueDCS *iterValue=new AliRPCValueDCS();
    
    fGlobalDataContainer->GetObject(ObjectName,tree);
    
    if(!tree) return;
    
    branch = tree->GetBranch(ObjectName);
    tree->SetBranchAddress(ObjectName,&iterValue);
    
    
    //create a list with runnumbers
    //note that user passes a list of AliOCDBRun*
    vector<UInt_t> RunNumberInTObjArray;
    for(auto iter=RunNumberList.begin();iter!=RunNumberList.end();iter++){
        //cout<<(*iter)->GetRunNumber()<<endl<<flush;
        RunNumberInTObjArray.push_back((*iter)->GetRunNumber());
    }
    
    //fill the histogram with data in tree
    //data must have a runnumber in runlist
    //iterValue assume all values
    for (Int_t i=0; i<tree->GetEntries(); i++) {
        tree->GetSortedEntry(i);
        
        UInt_t run=iterValue->GetRunNumber();
        if(!IsRunInList(RunNumberInTObjArray,run)) continue;
        if (iterValue->GetTimeStamp() > 8000 && (iterValue->*funky)()) {
            ((TH1F*)Graph)->Fill(iterValue->GetValue(whichValue));
        }
    }
}

//the idea is that this method calls correctly CreateDistributionSomething based on what label contains
//Graph is propagated to the next method
//*Graph must be istantiated at this point
//label should contain current/voltage/rate total/dark/net bending/notbending
//histos are filled only with data in runNumberList
//ObjectName specify wich RPC is being analyzed
//you can generate an ObjectName string with:
// Form("MTR_%s_MT%d_RPC%d", (fSides[side]).Data(), fPlanes[plane], RPC)

void AliRPCPlotGenerator::CreateDistributionSomething(TH1 *Graph, TString label, TString ObjectName, vector <AliOCDBRun*> RunNumberList){
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

//Default graphs generator for voltage
//this methods are called in GeneratePlot()
void AliRPCPlotGenerator::VoltagePlotter(TGraph *Graph, TString ObjectName, UInt_t RunNumber){
    AliRPCPlotGenerator::PlotSomethingVersusTime(Graph, &AliRPCValueDCS::IsVoltage, ObjectName, RunNumber, 0);
    return;
}

void AliRPCPlotGenerator::VoltagePlotter(TGraph *Graph, TString ObjectName, std::vector<UInt_t> RunNumberList){
    AliRPCPlotGenerator::PlotSomethingVersusTime(Graph,&AliRPCValueDCS::IsVoltage,ObjectName,RunNumberList,0);
    return;
}

void AliRPCPlotGenerator::VoltagePlotter(TGraph *Graph, TString ObjectName){
    AliRPCPlotGenerator::PlotSomethingVersusTime(Graph,&AliRPCValueDCS::IsVoltage,ObjectName);
    return;
}


/*
 This method is useful to create a plot with simple specification written on a textfile
 
 
 format for file is
 [plot/distribution/delta], [darkcurrent, netcurrent, voltage, ratenotbending....], [time, run, totalcurrent...], [fit, normalized, dark, MTR_INSIDE_MT11_RPC1...]
 
 example
 plot, netcurrent, ratebending, all&normalized&fit
 
 more examples at https://github.com/gabrielefronze/alice-MTR-utils/blob/devel-filippo/plots.txt
 */
//note that this method closes fPlotContainer
//so it cannot be called twice, but this is useful to let a user inside root
//to analyze results just opening a TBrowser
//before recall this please recall the AliRPCPlotGenerator::Constuctor
void AliRPCPlotGenerator::GeneratePlotFromFile(TString filename){
    
    //open the file with specifications
    ifstream file;
    file.open(filename.Data(), ios::in);
    
    if(!file.is_open()) {
        cout << "File " << filename << " not found." << endl;
        return;
    }
    
    string line;
    TObject *graphBuffer=0x0;
    TF1 *fitFunction=0x0;
    
    //read file line per line
    while(getline(file,line)){
        
        //exit if line is empty
        if(line.empty()) continue;
        cout<<"Reading new line"<<endl;
        
        //convert string to TString
        TString Tline(line);
        
        //separate string when find ','
        //TSTring::Tokenize() documentation at
        // https://root.cern.ch/doc/master/classTString.html#a14f461641af875f6bd042d4c1216cbbe
        TObjArray *commands;
        commands=Tline.Tokenize(", ");
        Int_t commandsSize=commands->GetEntries();
        TString plotType, yaxsis, xaxsis, options;
        
        //plottype, xasxis and yaxsis are mandatory
        //specification are optional
        //so we expect 3 or 4 argouments
        if(commandsSize<3) {
            printf("Too few argouments");
        }else if(commandsSize<=4){
            plotType=((TObjString*)(commands->At(0)))->GetString();
            yaxsis=((TObjString*)(commands->At(1)))->GetString();
            xaxsis=((TObjString*)(commands->At(2)))->GetString();
            if(commandsSize==4){
                options=((TObjString*)(commands->At(3)))->GetString();
            }else {
                options= TString("");
            }
        }else{
            printf("Too many argouments");
            continue;
        }
        
        //print a resume of command read
        cout<<"Plot type:\t"<<plotType.Data()<<endl;
        cout<<yaxsis.Data()<<"\tversus\t"<<xaxsis.Data()<<"\t"<<options.Data()<<endl;
        
        //get List useful on vs time plots
        //tree names are "Global_Data_MTR_INSIDE_MT...." user is requested onlyessential "MTR_INSIDE_MT...."
        options.Prepend("Global_Data_");
        
        Bool_t isDarkGraph=kFALSE;
        Bool_t isNormalizedGraph=kFALSE;
        Bool_t FEERICIsOK=kFALSE;
        
        if(options.Contains("dark")) isDarkGraph=kTRUE;
        if(options.Contains("FEERIC")) FEERICIsOK=kTRUE;
        if(options.Contains("normalized")) isNormalizedGraph=kTRUE;
        
        //create a defalut linear function if fit is requested
        //TODO
        //it should be useful to create fitFunction based on specification in options
        
        if(options.Contains("fit"))  fitFunction=new TF1("linear fit","[0]+[1]*x");
        
        //call correct function plot, distribution or delta
        //note that objects are instanziated here
        
        if(plotType.Contains("plot")){
            graphBuffer=new TGraph();
            PlotSomethingVersusSomethingElse((TGraph*)graphBuffer, yaxsis, xaxsis, isDarkGraph, isNormalizedGraph, fitFunction, options);
            
            //by default is created a plot for each RPC after the mean plot created by PlotSomethingVersusSomethingElse
            SomethingPerRun(yaxsis, isDarkGraph, isNormalizedGraph, FEERICIsOK);
            
        }else if(plotType.Contains("distribution")){
            graphBuffer=new TH1F();
            
            //By default run list is got from first RPC
            vector<AliOCDBRun*> RunList=fAliRPCDataObject->GetRunList(0,0,0);
            
            CreateDistributionSomething((TH1*)graphBuffer,yaxsis, options, RunList);
            
            if(isNormalizedGraph){
                //check that *Graph is a TH1F
                //operator * is overloaded correctly for TH1F not for TH1
                if (strcmp(graphBuffer->ClassName(),"TH1F")==0)
                {
                    //normalize to 72 (kNPlanes*kNSides*kNRPC) RPC
                    Double_t Normfactor=(Double_t)kNRPC/((TH1*)graphBuffer)->GetEntries();
                    //GraphF is a TH1F* which point to TH1::*Graph
                    TH1F *GraphF=(TH1F*)graphBuffer;
                    TH1F GraphNorm=Normfactor*(*GraphF);
                    graphBuffer=&GraphNorm;
                }
            }
            
        }else if(plotType.Contains("delta")){
            graphBuffer=new TGraph();
            PlotVariationSomething((TGraph*)graphBuffer,xaxsis,yaxsis);
        }
        
        fPlotContainer->cd();
        if(graphBuffer&&!yaxsis.Contains("charge")) graphBuffer->Write(Form("%sof%svs%s",plotType.Data(),yaxsis.Data(),xaxsis.Data()),TObject::kSingleKey|TObject::kOverwrite);
        graphBuffer=0x0;
        commands=0x0;
    }
    
    file.close();
    fPlotContainer->Close();
}


/*
 * print which RPC corresponds to iSide, iPlane, iRPC
 */
void AliRPCPlotGenerator::PrintWhichRPC(Int_t iRPC, Int_t iSide, Int_t iPlane){
    Int_t NTot=kNRPC*kNPlanes*kNSides;
    printf("RPC: MTR_%s_MT%d_RPC%d %3d out of %3d\n",(fSides[iSide]).Data(),fPlanes[iPlane],iRPC,kNRPC*kNPlanes*iSide+kNRPC*iPlane+iRPC,NTot);
    return;
}

//check if RunNumber is a member of RunList
Bool_t AliRPCPlotGenerator::IsRunInList(std::vector<UInt_t> RunList, UInt_t RunNumber) {
    return std::find(RunList.begin(), RunList.end(), RunNumber) != RunList.end();
}
