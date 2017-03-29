//
//  AliRPCValueDCS.C
//  Aliroot project
//
//  Created by Gabriele Gaetano Fronzé on 20/11/15.
//  Copyright © 2015 Gabriele Gaetano Fronzé. All rights reserved.
//

#include "AliRPCValueDCS.h"
#include "TClass.h"

AliRPCValueDCS::AliRPCValueDCS() : TObject(){
    fRunNumber=0;
    fTimeStamp=0;
    fIsCalib=kFALSE;
    fIsAMANDA=kFALSE;
    fBeamType=knone;
    fBeamEnergy=0.;
    fLHCStatus=kNONE;
};

AliRPCValueDCS::AliRPCValueDCS(UInt_t runNumber, ULong64_t timeStamp, Bool_t isCalib, TString beamType, Float_t beamEnergy,
                               TString LHCStatus, Bool_t isAMANDA) : TObject(){
    fRunNumber=runNumber;
    fTimeStamp=timeStamp;
    fIsCalib=isCalib;
    fIsAMANDA=isAMANDA;
    fBeamType = knone;

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
        fBeamEnergy=0.;
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