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

#include "AliRPCValueVoltage.h"

AliRPCValueVoltage::AliRPCValueVoltage() : AliRPCValueDCS(){
    fDataType=DCSDataType::kVoltage;
    fVSupp=(Float_t)0.;
};

AliRPCValueVoltage::AliRPCValueVoltage(UInt_t runNumber,ULong64_t timeStamp,UInt_t runYear,Double_t vSupp,Bool_t isCalib, TString beamType, Float_t beamEnergy, TString LHCStatus) : AliRPCValueDCS(
        runNumber, timeStamp, runYear, isCalib, beamType, beamEnergy, LHCStatus, kFALSE, DCSDataType::kVoltage) {
    fVSupp=(Float_t)vSupp;
};
