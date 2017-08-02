/* Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------

#ifndef AliRPCPlotGenerator_h
#define AliRPCPlotGenerator_h

#include "TObject.h"
#include "TNamed.h"
#include "TString.h"
#include "TFile.h"

// #include "TClass.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdio.h>

#include "TROOT.h"
#include "TSmartTree.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TObjString.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TText.h"

#include "AliRPCData.h"
#include "AliRPCRunStatistics.h"
#include "AliRPCValueDCS.h"
#include "AliRPCValueCurrent.h"
#include "AliRPCValueVoltage.h"
#include "AliRPCValueScaler.h"
#include "AliRPCAutoIntegrator.h"


class AliRPCPlotGenerator : public TObject{
public:
    AliRPCPlotGenerator();
    AliRPCPlotGenerator(TString filename);
    
private:
    static const Int_t kNSides=2;
    static const Int_t kNPlanes=4;
    static const Int_t kNRPC=9;
    static const Int_t kNLocalBoards=234;
    static const ULong64_t kFullScale=65535;
    static const Int_t kNCathodes=2;
    static const TString *fSides;
    static const Int_t *fPlanes;
    static const TString *fCathodes;
    //array per la conversione di iRPC={0,17} in iRPC={1,9}x{inside,outside}
    static const Int_t *kRPCIndexes;
    static const Int_t *kRPCSides;
    static const Int_t *kLBToRPC;
    Double_t fRPCAreas[kNRPC][kNPlanes];
    Double_t fLBAreas[kNLocalBoards][kNPlanes];
    Double_t fTinyArea[kNPlanes];
    Double_t fLittleArea[kNPlanes];
    Double_t fNormalArea[kNPlanes];
    Double_t fBigArea[kNPlanes];
    static const Int_t *fColors;//[kNRPC] = {kBlack,kRed,kGreen,kBlue,kYellow,kMagenta,kCyan,kGray,kOrange};
    static const Int_t *fStyles;//[kNPlanes]={20,24,21,25};
    
    TFile *fGlobalDataContainer;
    TFile *fPlotContainer;
    AliRPCData *fAliRPCDataObject;
    
    void InitDataMembers();
    
public:
    TGraph* GetIntegratedChargePlot(Int_t iRPC, Int_t iSide, Int_t iPlane);
    void PlotRPCPerMT(Bool_t showFeeric=kFALSE);
    
    void IntegratorPerRun(Bool_t showFeeric=kFALSE);
    void SomethingPerRun(TString observableName, Bool_t isDarkGraph=kFALSE, Bool_t isNormalizedGraph=kFALSE, Bool_t showFeeric=kFALSE);
    
    void GeneratePlots();
    void GeneratePlotFromFile(TString filename = "plots.txt");
    void VoltagePlotter(TGraph *Graph, TString ObjectName, UInt_t RunNumber);
    void VoltagePlotter(TGraph *Graph, TString ObjectName, std::vector<UInt_t> RunNumberList);
    void VoltagePlotter(TGraph *Graph, TString ObjectName);

    
private:
    inline Bool_t checkFileExistance(TString fileName) {
        ifstream f(fileName.Data());
        return f.good();
    }
    
    
    void CreateDistributionSomething(TH1 *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, vector<AliOCDBRun*> RunNumberList, Int_t whichValue);
    void CreateDistributionSomething(TH1 *Graph, TString label, TString ObjectName, vector <AliOCDBRun*> RunNumberList);
    void PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TString ObjectName, std::vector<UInt_t> RunNumberList, Int_t whichValue=0);
    void PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)()const, TString ObjectName, UInt_t RunNumber, Int_t whichValue=0);
    void PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)()const, TString ObjectName, std::vector<AliOCDBRun*> RunNumberList, Int_t whichValue=0);
    void PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)()const, TString ObjectName, Int_t whichValue=0);
    
    void PlotSomethingVersusRun(TGraph *Graph, Double_t (AliRPCData::*funky)(UInt_t, Bool_t)const, Bool_t normalizedToArea=kFALSE,Bool_t onlyDarkPoints=kFALSE);
    void PlotSomethingVersusRPC(TGraph *Graph, Double_t (AliRPCData::*funkyX)(Int_t, Int_t, Int_t, Bool_t)const, Double_t (AliRPCData::*funkyY)(Int_t, Int_t, Int_t, Bool_t)const, Bool_t normalizedToArea=kTRUE);
    void PlotVariationVsIntegratedCharge(TGraph *Graph, Double_t (AliRPCRunStatistics::*funky)()const);
    
    void PlotVariationSomething(TGraph *Graph, TString, TString);
    void PlotSomethingVersusSomethingElse(TGraph *Graph, const TString x, const TString y, Bool_t onlyDarkPoints=kFALSE, Bool_t normalizedToArea=kFALSE,  TF1 *fitFunc=nullptr, TString ObjectName="");
    
    
    Bool_t IsFEERIC(Int_t iRPC, Int_t iSide, Int_t iPlane){
        if(iPlane==3&&iSide==0&&iRPC==3) return kTRUE;
        else return kFALSE;
    }
    static void PrintWhichRPC(Int_t iRPC, Int_t iSide, Int_t iPlane);
    Bool_t IsRunInList(std::vector<UInt_t> vector, UInt_t number);
    
    ClassDef(AliRPCPlotGenerator,0);
};

#endif
