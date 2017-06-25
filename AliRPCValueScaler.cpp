/**************************************************************************
 * Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------

#include "AliRPCValueScaler.h"

AliRPCValueScaler::AliRPCValueScaler() : AliRPCValueDCS(){
    fDataType=DCSDataType::kScaler;
    fScalerCounts=0;
    fOverflow=kFALSE;
    fDeltaT=(Float_t)-1.;
};

AliRPCValueScaler::AliRPCValueScaler(UInt_t runNumber,ULong64_t timeStamp,UInt_t runYear, Long64_t scalerCounts,Bool_t isCalib,TString beamType, Float_t beamEnergy, TString LHCStatus, Double_t deltaT, Bool_t overflow) : AliRPCValueDCS(
        runNumber, timeStamp, runYear, isCalib, beamType, beamEnergy, LHCStatus, kFALSE, DCSDataType::kScaler) {
    fScalerCounts=scalerCounts;
    fOverflow=overflow;
    fDeltaT=(Float_t)deltaT;
};
