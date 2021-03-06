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
    knone
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
    AliRPCValueDCS(UInt_t runNumber, ULong64_t timeStamp, UInt_t runYear, Bool_t isCalib, TString beamType, Float_t beamEnergy, TString LHCStatus, Bool_t isAMANDA);
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
    inline void SetIsCalib(Bool_t isCalib){fIsCalib=isCalib;};
    void SetRunNumber(UInt_t RunNumber);
    inline void SetRunYear(UInt_t RunYear){fRunYear=RunYear;};
    inline void SetBeamType(TBeamType BeamType) {fBeamType = BeamType;}
    inline void SetBeamEnergy(Float_t BeamEnergy) {fBeamEnergy = BeamEnergy;}
    inline void SetLHCStatus(TLHCStatus LHCStatus) {fLHCStatus = LHCStatus;}

    TBeamType GetBeamType() const {return fBeamType;}
    Float_t GetBeamEnergy() const {return fBeamEnergy;}
    TLHCStatus GetLHCStatus() const {return fLHCStatus;}
    UInt_t GetYear(){return fRunYear;};

    inline Bool_t IsCalib() const { return fIsCalib; };
    inline Bool_t IsBeamPresent() const { return fLHCStatus == kBEAM; };
    //simplified original expression was IsCurrent() && (!fIsAMANDA && fLHCStatus > kNONE)
    inline Bool_t IsOkForIDark() const {
    if(IsCurrent()) {
        if (!fIsAMANDA) {
            return fLHCStatus > kNONE; //OCDB No beam
        }
    }return kFALSE;
    };

    //original expression was IsCurrent() && (fIsAMANDA || (!fIsAMANDA && fLHCStatus == kBEAM))
    inline Bool_t IsOkForITot() const {
        if(fIsAMANDA){
            return kTRUE;   //AMANDA
        }else if (IsCurrent()){
            return fLHCStatus==kBEAM;   //OCDB Beam
        }
        return kFALSE;
    };

    virtual Double_t GetValue(Int_t ) const {return 0.;};

    inline void PrintBeamStatus() const {printf("%s sqrt(s)=%fGeV %s\n",fHumanBeamType().Data(),fBeamEnergy,fHumanLHCStatusType().Data()); };

private:
    UInt_t fRunNumber;
    UInt_t fRunYear;
    TBeamType fBeamType;
    Float_t fBeamEnergy;
    TLHCStatus fLHCStatus;
    ULong64_t fTimeStamp;

    Bool_t fIsCalib;
    Bool_t fIsAMANDA;

    TString fHumanBeamType() const;
    TString fHumanLHCStatusType() const;

ClassDef(AliRPCValueDCS,4);
};

#endif /* AliRPCValueDCS_h */
