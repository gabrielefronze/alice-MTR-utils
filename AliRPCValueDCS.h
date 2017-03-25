//
//  AliRPCValueDCS.h
//  Aliroot project
//
//  Created by Gabriele Gaetano Fronzé on 20/11/15.
//  Copyright © 2015 Gabriele Gaetano Fronzé. All rights reserved.
//

#ifndef AliRPCValueDCS_h
#define AliRPCValueDCS_h

#include <stdio.h>
#include "TObject.h"
#include "TString.h"

class AliRPCValueDCS : public TObject{
public:
    AliRPCValueDCS();
    AliRPCValueDCS(const AliRPCValueDCS &obj)  : TObject(obj){};
    AliRPCValueDCS(UInt_t runNumber,ULong64_t timeStamp,Bool_t isCalib, Bool_t isAMANDA=kFALSE);
    ~AliRPCValueDCS(){};
    inline UInt_t GetRunNumber() const { return fRunNumber; };
    inline ULong64_t GetTimeStamp() const { return fTimeStamp; };
    inline Bool_t IsCalib() const { return fIsCalib; };
    Bool_t IsEqual (const TObject *obj) const;
    Int_t Compare(const TObject *obj) const;
    Bool_t IsSortable() const { return kTRUE; };
    TString *WhatIsThis();
    inline Bool_t IsCurrent(){return (TString*)(this->WhatIsThis())->Contains("current");};
    inline Bool_t IsVoltage(){return (TString*)(this->WhatIsThis())->Contains("voltage");};
    inline Bool_t IsScaler(){return (TString*)(this->WhatIsThis())->Contains("scaler");};
    inline Bool_t IsOverflow(){return (TString*)(this->WhatIsThis())->Contains("overflow");};
    inline Bool_t IsAMANDA(){return fIsAMANDA;};
    inline void SetIsAMANDA(Bool_t isAMANDA){fIsAMANDA = isAMANDA;};
    void SetRunNumber(UInt_t RunNumber);

private:
    UInt_t fRunNumber;
    ULong64_t fTimeStamp;
    Bool_t fIsCalib;
    Bool_t fIsAMANDA;

    ClassDef(AliRPCValueDCS,3);
};

#endif /* AliRPCValueDCS_h */
