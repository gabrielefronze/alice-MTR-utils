/* Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------

#ifndef AliRPCOverflowStatistics_h
#define AliRPCOverflowStatistics_h

#include <stdio.h>
#include "AliRPCValueDCS.h"

class AliRPCOverflowStatistics : public AliRPCValueDCS{
public:
	AliRPCOverflowStatistics();
    AliRPCOverflowStatistics(const AliRPCOverflowStatistics &obj)  : AliRPCValueDCS(obj){};
    AliRPCOverflowStatistics(UInt_t runNumber,ULong64_t timeStamp,UInt_t overflowLBCount,UInt_t underflowLBCount,Bool_t isCalib, TString beamType, Float_t beamEnergy, TString LHCStatus);
    ~AliRPCOverflowStatistics(){};
    inline UInt_t GetOverflowNumber() const {return fOverflowLBCount;};
    inline UInt_t GetUnderNumber() const {return fUnderflowLBCount;};

	ClassDef(AliRPCOverflowStatistics,1);
};

#endif
