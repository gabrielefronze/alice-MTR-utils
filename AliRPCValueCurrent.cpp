//
//  AliRPCValueCurrent.C
//  Aliroot project
//
//  Created by Gabriele Gaetano Fronzé on 20/11/15.
//  Copyright © 2015 Gabriele Gaetano Fronzé. All rights reserved.
//

#include "AliRPCValueCurrent.h"

AliRPCValueCurrent::AliRPCValueCurrent() : AliRPCValueDCS(){
	fCalibRunNumber=0;
    fITot=0.;
    fIDark=0.;
};

AliRPCValueCurrent::AliRPCValueCurrent(UInt_t runNumber,ULong64_t timeStamp,UInt_t runYear,Double_t iTot,Double_t iDark,Bool_t isCalib, TString beamType, Float_t beamEnergy, TString LHCStatus, UInt_t calibRunNumber,Bool_t isAMANDA) : AliRPCValueDCS(
        runNumber, timeStamp, runYear, isCalib, beamType, beamEnergy, LHCStatus, isAMANDA, DCSDataType::kCurrent) {
    fCalibRunNumber=calibRunNumber;
    fITot=iTot;
    fIDark=iDark;
};
