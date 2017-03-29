//
//  AliRPCValueCurrent.h
//  Aliroot project
//
//  Created by Gabriele Gaetano Fronzé on 20/11/15.
//  Copyright © 2015 Gabriele Gaetano Fronzé. All rights reserved.
//

#ifndef AliRPCValueCurrent_h
#define AliRPCValueCurrent_h

#include <stdio.h>
#include "AliRPCValueDCS.h"

class AliRPCValueCurrent : public AliRPCValueDCS{
public:
    AliRPCValueCurrent();
    AliRPCValueCurrent(const AliRPCValueCurrent &obj)  : AliRPCValueDCS(obj){};
    AliRPCValueCurrent(UInt_t runNumber,ULong64_t timeStamp,Double_t iTot,Double_t iDark,Bool_t isCalib, TString beamType, Float_t beamEnergy, TString LHCStatus, UInt_t calibRunNumber,Bool_t isAMANDA=kFALSE);
    ~AliRPCValueCurrent(){};
    inline Double_t GetITot() const { return fITot; };
    inline void SetITot(Double_t iTot) { fITot=iTot; };
    inline Double_t GetIDark() const { return fIDark; };
    inline void SetIDark(Double_t iDark) { fIDark=iDark; };
    inline Double_t GetINet() const {
        if (fITot-fIDark>0.) return fITot-fIDark;
        else return 0.;
    };
    inline UInt_t GetCalibRunNumber() const { return fCalibRunNumber; };
    inline void SetCalibRunNumber(UInt_t calibRunNumber) { fCalibRunNumber=calibRunNumber; };

    Double_t GetValue(Int_t whichValue=0) const {
        switch (whichValue){
            case kITot: return fITot;
            case kIDark: return fIDark;
            case kINet: return GetINet();
            default: return fITot;
        }
    };

    enum {
        kITot,
        kIDark,
        kINet
    };

private:
    UInt_t fCalibRunNumber;
    Double_t fITot;
    Double_t fIDark;

    ClassDef(AliRPCValueCurrent,2);
};

#endif /* AliRPCValueCurrent_h */
