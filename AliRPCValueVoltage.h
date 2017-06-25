/* Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------


#ifndef AliRPCValueVoltage_h
#define AliRPCValueVoltage_h

#include <stdio.h>
#include "AliRPCValueDCS.h"

class AliRPCValueVoltage : public AliRPCValueDCS{
public:
    AliRPCValueVoltage();
    AliRPCValueVoltage(const AliRPCValueVoltage &obj)  : AliRPCValueDCS(obj){};
    AliRPCValueVoltage(UInt_t runNumber,ULong64_t timeStamp,UInt_t runYear,Double_t vSupp,Bool_t isCalib, TString beamType, Float_t beamEnergy, TString LHCStatus);
    ~AliRPCValueVoltage(){};
    inline Float_t GetVSupp() const { return fVSupp; };
    inline void SetVSupp(Float_t vSupp) { fVSupp=vSupp; };

    Double_t GetValue(Int_t) const {return (Double_t)fVSupp; };
    
    ClassDef(AliRPCValueVoltage,2);
};

#endif /* AliRPCValueVoltage_h */
