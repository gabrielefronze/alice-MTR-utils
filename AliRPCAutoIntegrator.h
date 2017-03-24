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

    void OCDBRunListReader();
    void OCDBDataToCParser();
    void AMANDATextToCParser();
    void Aggregator();
    void Subtractor();
    void Integrator();

    static void whichRPC(Int_t iRPC, Int_t iSide, Int_t iPlane);
    ClassDef(AliRPCAutoIntegrator,3);
};

#endif
