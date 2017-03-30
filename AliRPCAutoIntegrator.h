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
#include "AliOCDBRun.h"
#include "TGraph.h"
#include <vector>

typedef struct {
    Int_t Side;
    Int_t Plane;
    Int_t RPC;
} RPC;

class AliRPCAutoIntegrator : public TObject{
public:
    AliRPCAutoIntegrator();
    AliRPCAutoIntegrator(TString RunListFileName, TString AMANDAInputFileName, TString OutputFileName, Bool_t updateOCDB=kTRUE, Bool_t updateAMANDA=kTRUE);
	AliRPCAutoIntegrator(const AliRPCAutoIntegrator &obj)  : TObject(obj){};
	~AliRPCAutoIntegrator();
    void RunAutoIntegrator();

    void VoltagePlotter(TGraph *Graph, TList* list, UInt_t RunNumber);
    void VoltagePlotter(TGraph *Graph, TList* list, std::vector<UInt_t> RunNumberList);
    void VoltagePlotter(TGraph *Graph, TList* list);
    void CreateDarkCurrentDistribution(TH1 *Graph, UInt_t RunNumber);


private:
    std::vector<AliOCDBRun> fOCDBRunListToAdd;

    TString fRunListFileName;
    TString fAMANDAInputFileName;
    TString fOutputFileName;

    TFile *fOCDBDataContainer;
    TFile *fAMANDADataContainer;
    TFile *fGlobalDataContainer;

    Bool_t fUpdateOCDB;
    Bool_t fUpdateAMANDA;

    AliRPCData *fAliRPCDataObject;

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
    void GeneratePlots();
    void Subtractor();
    void Integrator();

private:
    void CreateDistributionSomething(TH1 *Graph, Bool_t (AliRPCValueDCS::*funky)() const, UInt_t RunNumber, Int_t whichValue=0, Bool_t normalizedToArea=kTRUE);
    void CreateDistributionSomething(TH1 *Graph, Bool_t (AliRPCValueDCS::*funky)() const, std::vector<AliOCDBRun> RunNumberList, Int_t whichValue=0, Bool_t normalizedToArea=kTRUE);
    void PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)() const, TList *list, std::vector<UInt_t> RunNumberList, Int_t whichValue=0);
    void PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)()const, TList *list, UInt_t RunNumber, Int_t whichValue=0);
    void PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)()const, TList *list, std::vector<AliOCDBRun> RunNumberList, Int_t whichValue=0);
    void PlotSomethingVersusTime(TGraph *Graph, Bool_t (AliRPCValueDCS::*funky)()const, TList *list, Int_t whichValue=0);

    void PlotSomethingVersusRun(TGraph *Graph, Double_t (AliRPCData::*funky)(Int_t)const);
    void PlotSomethingVersusRPC(TGraph *Graph[kNSides][kNPlanes][kNRPC], Double_t (AliRPCData::*funkyX)(Int_t, Int_t, Int_t)const, Double_t (AliRPCData::*funkyY)(Int_t, Int_t, Int_t)const);

    void AMANDASetDataMembers();
    void FillAliRPCData();

    static void WhichRPC(Int_t iRPC, Int_t iSide, Int_t iPlane);
    Bool_t IsRunInList(std::vector<UInt_t> vector, UInt_t number);


ClassDef(AliRPCAutoIntegrator,4);
};

#endif
