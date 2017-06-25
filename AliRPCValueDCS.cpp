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

#include "AliRPCValueDCS.h"
#include "TClass.h"

AliRPCValueDCS::AliRPCValueDCS() : TObject(){
    fRunNumber=0;
    fRunYear=0;
    fTimeStamp=0;
    fIsCalib=kFALSE;
    fIsAMANDA=kFALSE;
    fBeamEnergy=0.f;
    fBeamType=knone;
    fLHCStatus=kNONE;
};

AliRPCValueDCS::AliRPCValueDCS(UInt_t runNumber, ULong64_t timeStamp, UInt_t runYear, Bool_t isCalib, TString beamType,
                               Float_t beamEnergy, TString LHCStatus, Bool_t isAMANDA, DCSDataType dataType) : TObject(){
    fRunNumber=runNumber;
    fRunYear=runYear;
    fTimeStamp=timeStamp;
    fIsCalib=isCalib;
    fIsAMANDA=isAMANDA;
    fBeamType = knone;
    fDataType = dataType;

    if(LHCStatus.Contains("BEAM")){
        if(LHCStatus.Contains("NO")){
            fLHCStatus = kNOBEAM;
        } else if(LHCStatus.Contains("DUMP")){
            fLHCStatus = kDUMP;
        } else {
            fLHCStatus = kBEAM;
        }
    } else if(LHCStatus.Contains("RAMP")){
        fLHCStatus = kRAMP;
    } else {
        fLHCStatus = kNONE;
    }

    if(fLHCStatus == 0) {
        if (beamType.Contains("p")) {
            if (beamType.Contains("A")) {
                fBeamType = kpA;
            } else {
                fBeamType = kpp;
            }
        } else if (beamType.Contains("A")) {
            fBeamType = kAA;
        }
        fBeamEnergy=beamEnergy;
    } else {
        fBeamType = knone;
        fBeamEnergy=0.f;
    }
};

Bool_t AliRPCValueDCS::IsEqual (const TObject *obj) const {
    return fTimeStamp == ((AliRPCValueDCS*)obj)->GetTimeStamp();
};

Int_t AliRPCValueDCS::Compare(const TObject *obj) const {
    if ( fTimeStamp <  ((AliRPCValueDCS*)obj)->GetTimeStamp() ) return -1;
    if ( fTimeStamp == ((AliRPCValueDCS*)obj)->GetTimeStamp() ) return 0;
    else return 1;
};

TString* AliRPCValueDCS::WhatIsThis() const {
    TString *className =new TString(((TClass*)this->IsA())->GetName());
    if (className->Contains("AliRPCValueCurrent")) {
        delete className;
        return new TString("current");
    } else if (className->Contains("AliRPCValueVoltage")) {
        delete className;
        return new TString("voltage");
    } else if (className->Contains("AliRPCValueScaler")) {
        delete className;
        return new TString("scaler");
    } else if (className->Contains("AliRPCOverflowStatistics")) {
        delete className;
        return new TString("overflow");
    } else {
        delete className;
        return new TString("unknown");
    }

};


void AliRPCValueDCS::SetRunNumber(UInt_t RunNumber){
    //AMANDA needs to be set, OCDB run Rumber are set when created
    if(this->IsAMANDA()) fRunNumber=RunNumber;
}

TString AliRPCValueDCS::fHumanBeamType() const {
    switch (fBeamType){
        case kpp: return "p-p";

        case kpA: return "p-A";

        case kAA: return "A-A";

        case knone: return "None";

        default: return "Unknown";
    }
};

TString AliRPCValueDCS::fHumanLHCStatusType() const {
    switch (fLHCStatus){
        case kBEAM: return "STABLE BEAMS";

        case kNONE: return "UNKNOWN";

        case kNOBEAM: return "NO BEAM";

        case kDUMP: return "BEAM DUMP";

        case kRAMP: return "RAMP DOWN";

        default: return "UNKNOWN";
    }
}
