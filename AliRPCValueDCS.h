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

typedef enum{
    kpp,
    kpA,
    kAA,
    kNone
} TBeamType;

typedef enum{
    kBEAM,
    kNONE,
    kNOBEAM,
    kDUMP,
    kRAMP
} TLHCStatus;

class AliRPCValueDCS : public TObject{
public:
    AliRPCValueDCS();
    AliRPCValueDCS(const AliRPCValueDCS &obj)  : TObject(obj){};
    AliRPCValueDCS(UInt_t runNumber, ULong64_t timeStamp, Bool_t isCalib, TString beamType, Float_t beamEnergy, TString LHCStatus, Bool_t isAMANDA);
    ~AliRPCValueDCS(){};
    inline UInt_t GetRunNumber() const { return fRunNumber; };
    inline ULong64_t GetTimeStamp() const { return fTimeStamp; };
    Bool_t IsEqual (const TObject *obj) const;
    Int_t Compare(const TObject *obj) const;
    Bool_t IsSortable() const { return kTRUE; };
    TString *WhatIsThis() const;
    inline Bool_t IsCurrent() const {return (Bool_t)((TString*)(WhatIsThis())->Contains("current"));};
    inline Bool_t IsVoltage() const {return (Bool_t)((TString*)(WhatIsThis())->Contains("voltage"));};
    inline Bool_t IsScaler() const {return (Bool_t)((TString*)(WhatIsThis())->Contains("scaler"));};
    inline Bool_t IsOverflow() const {return (Bool_t)((TString*)(WhatIsThis())->Contains("overflow"));};
    inline Bool_t IsAMANDA() const {return fIsAMANDA;};
    inline void SetIsAMANDA(Bool_t isAMANDA){fIsAMANDA = isAMANDA;};
    void SetRunNumber(UInt_t RunNumber);

    inline Bool_t IsCalib() const { return fIsCalib; };
    inline Bool_t IsBeamPresent() const { return fLHCStatus == kBEAM; };
    // The return value has been reduced using De Morgan laws. The original form is ( fIsCalib || ( !fIsCalib && !fIsBeamPresent ) )
    inline Bool_t IsDark() const { return  fLHCStatus > 1; };

    inline virtual Double_t GetIDark()const {return -1;};
    inline virtual Double_t GetVSupp()const {return -1;};

    inline void PrintBeamStatus() const {printf("%d sqrt(s)=%fGeV %d\n",fBeamType,fBeamEnergy,fLHCStatus); };

private:
    UInt_t fRunNumber;
    TBeamType fBeamType;
    Float_t fBeamEnergy;
    TLHCStatus fLHCStatus;
    ULong64_t fTimeStamp;
    Bool_t fIsCalib;
    Bool_t fIsAMANDA;

    ClassDef(AliRPCValueDCS,4);
};

#endif /* AliRPCValueDCS_h */
