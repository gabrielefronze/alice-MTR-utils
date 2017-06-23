/* Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------

#ifndef AliRPCValueScaler_h
#define AliRPCValueScaler_h

#include <stdio.h>
#include "AliRPCValueDCS.h"

class AliRPCValueScaler : public AliRPCValueDCS{
public:
    AliRPCValueScaler();
    AliRPCValueScaler(const AliRPCValueScaler &obj)  : AliRPCValueDCS(obj){};
    AliRPCValueScaler(UInt_t runNumber,ULong64_t timeStamp,UInt_t runYear, Long64_t scalerCounts,Bool_t isCalib,TString beamType, Float_t beamEnergy, TString LHCStatus, Double_t deltaT=-1., Bool_t overflow=kFALSE);
    ~AliRPCValueScaler(){};
    inline Long64_t GetScalerCounts() const { return fScalerCounts; };
    inline void SetScalerCounts(Long64_t scalerCounts) { fScalerCounts=scalerCounts; };
    inline Bool_t GetHasOverflow() const { return fOverflow; };
    inline Float_t GetDeltaT() const { return fDeltaT; };

    ClassDef(AliRPCValueScaler,4);
};

#endif /* AliRPCValueScaler_h */
