//
//  AliRPCValueVoltage.C
//  Aliroot project
//
//  Created by Gabriele Gaetano Fronzé on 20/11/15.
//  Copyright © 2015 Gabriele Gaetano Fronzé. All rights reserved.
//

#include "AliRPCValueVoltage.h"

AliRPCValueVoltage::AliRPCValueVoltage() : AliRPCValueDCS(){
    fVSupp=0.;
};

AliRPCValueVoltage::AliRPCValueVoltage(UInt_t runNumber,ULong64_t timeStamp,UInt_t runYear,Double_t vSupp,Bool_t isCalib, TString beamType, Float_t beamEnergy, TString LHCStatus) : AliRPCValueDCS(
        runNumber, timeStamp, runYear, isCalib, beamType, beamEnergy, LHCStatus, kFALSE) {
    fVSupp=vSupp;
};