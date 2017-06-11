//
//  AliRPCOverflowStatistics.C
//  Aliroot project
//
//  Created by Gabriele Gaetano Fronzé on 21/12/15.
//  Copyright © 2015 Gabriele Gaetano Fronzé. All rights reserved.
//

#include "AliRPCOverflowStatistics.h"

AliRPCOverflowStatistics::AliRPCOverflowStatistics() : AliRPCValueDCS(){
    fDataType=DCSDataType::kOverflowData;
    fOverflowLBCount=0;
    fUnderflowLBCount=0;
};

AliRPCOverflowStatistics::AliRPCOverflowStatistics(UInt_t runNumber,ULong64_t timeStamp,UInt_t overflowLBCount,UInt_t underflowLBCount,Bool_t isCalib, TString beamType, Float_t beamEnergy, TString LHCStatus) : AliRPCValueDCS(
        runNumber, timeStamp, 0, isCalib, beamType, beamEnergy, LHCStatus, kFALSE, DCSDataType::kOverflowData) {
    fOverflowLBCount=overflowLBCount;
    fUnderflowLBCount=underflowLBCount;
};