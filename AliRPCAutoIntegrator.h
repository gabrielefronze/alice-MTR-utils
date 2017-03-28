//
// AliRPCAutoIntegrator.h
//
//   Created by Gabriele Gaetano Fronzé
//   Copyright © 2016 Gabriele Gaetano Fronzé. All rights reserved.

#ifndef AliRPCAutoIntegrator_h
#define AliRPCAutoIntegrator_h

#include "TObject.h"
#include "TNamed.h"
#include "TString.h"
#include "TFile.h"
#include "AliRPCValueDCS.h"
#include "AliRPCRunStatistics.h"
#include "AliRPCValueCurrent.h"
#include "AliRPCData.h"
#include <vector>

typedef struct OCDBRun {
    Int_t runNumber;
    Int_t year;
} OCDBRun;

class AliRPCAutoIntegrator : public TObject{
public:
    AliRPCAutoIntegrator();
    AliRPCAutoIntegrator(TString RunListFileName, TString AMANDAInputFileName, TString OutputFileName, Bool_t updateOCDB=kTRUE, Bool_t updateAMANDA=kTRUE);
	AliRPCAutoIntegrator(const AliRPCAutoIntegrator &obj)  : TObject(obj){};
	~AliRPCAutoIntegrator();
    void RunAutoIntegrator();

    void VoltagePlotter(UInt_t RunNumber);
    void VoltagePlotter(std::vector<UInt_t> RunNumberList);
    void CreateDarkCurrentDistribution(UInt_t RunNumber);
private:
    std::vector<OCDBRun> fOCDBRunList;

    TString fRunListFileName;
    TString fAMANDAInputFileName;
    TString fOutputFileName;

    TFile *fOCDBDataContainer;
    TFile *fAMANDADataContainer;
    TFile *fGlobalDataContainer;

    Bool_t fUpdateOCDB;
    Bool_t fUpdateAMANDA;

    AliRPCData *fMeanDataContainer;

    static const Int_t kNSides=2;
    static const Int_t kNPlanes=4;
    static const Int_t kNRPC=9;
    static const Int_t kNLocalBoards=234;
    static const Int_t kNCathodes=2;
    static const TString *fSides;
    static const Int_t *fPlanes;
    static const TString *fCathodes;
    //array per la conversione di iRPC={0,17} in iRPC={1,9}x{inside,outside}
    static const Int_t *kRPCIndexes;
    static const Int_t *kRPCSides;
    Double_t fRPCAreas[kNRPC][kNPlanes];
    static const Int_t *fColors;//[kNRPC] = {kBlack,kRed,kGreen,kBlue,kYellow,kMagenta,kCyan,kGray,kOrange};
    static const Int_t *fStyles;//[kNPlanes]={20,24,21,25};

    Bool_t CheckPointer(TNamed *pointer){
        Bool_t output=!pointer;
        if(output) {
            printf("\nproblem with %s\n",pointer->GetName());
        }
        return output;
    }

public:
    void OCDBRunListReader();
    void OCDBDataToCParser();
    void AMANDATextToCParser();
    void Aggregator();
    void Subtractor();
    void Integrator();

private:
    void CreateDistributionSomething(UInt_t RunNumber, Bool_t (AliRPCValueDCS::*funky)() const, Double_t (AliRPCValueDCS::*GetFunky)() const,TString WhatIsThis, Bool_t normalizedToArea=kTRUE,Int_t nbins=10, Double_t xlow=0., Double_t xup=1.);
    void PlotSomethingVersusTime(std::vector<UInt_t> RunNumberList, Bool_t (AliRPCValueDCS::*funky)() const, Double_t (AliRPCValueDCS::*GetFunky)() const, TString WhatIsThis);
    void PlotSomethingVersusTime(UInt_t RunNumber, Bool_t (AliRPCValueDCS::*funky)() const, Double_t (AliRPCValueDCS::*GetFunky)() const,TString WhatIsThis);

    void AMANDASetRunNumber();
    void FillAliRPCData();

    static void whichRPC(Int_t iRPC, Int_t iSide, Int_t iPlane);
    Bool_t IsRunInList(std::vector<UInt_t> vector, UInt_t number);


ClassDef(AliRPCAutoIntegrator,4);
};

#endif
