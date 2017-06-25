/* Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------

#ifndef OCDBRUN_H
#define OCDBRUN_H

#include "TObject.h"

class AliOCDBRun : public TObject{
public:
    UInt_t fRunNumber;
    Int_t fYear;

    inline UInt_t GetRunNumber()const{return fRunNumber;};

    AliOCDBRun() : TObject() {};
    AliOCDBRun(UInt_t runNumber, Int_t year) : TObject(), fRunNumber(runNumber), fYear(year){};
    virtual ~AliOCDBRun(){};

    Bool_t IsEqual (const TObject *obj) const{
        return fRunNumber == ((AliOCDBRun*)obj)->fRunNumber;
    };

    Int_t Compare(const TObject *obj) const{
        if ( fRunNumber <  ((AliOCDBRun*)obj)->fRunNumber ) return -1;
        if ( fRunNumber == ((AliOCDBRun*)obj)->fRunNumber ) return 0;
        else return 1;
    }

    Bool_t IsSortable() const { return kTRUE; };

    ClassDef(AliOCDBRun,4);
};
#endif //OCDBRUN_H
